=begin

monitor.rb
Author: Shugo Maeda <shugo@netlab.co.jp>
Version: 1.2.1

USAGE:

  foo = Foo.new
  foo.extend(MonitorMixin)
  cond = foo.new_cond

  thread1:
  foo.synchronize {
    ...
    cond.wait_until { foo.done? }
    ...
  }

  thread2:
  foo.synchronize {
    foo.do_something
    cond.signal
  }

=end
  
module MonitorMixin
  module Accessible
  protected
    attr_accessor :mon_owner, :mon_count
    attr_reader :mon_entering_queue, :mon_waiting_queue
  end
  
  module Initializable
  protected
    def mon_initialize
      @mon_owner = nil
      @mon_count = 0
      @mon_entering_queue = []
      @mon_waiting_queue = []
    end
  end
  
  class ConditionVariable
    class Timeout < Exception; end
    
    include Accessible
    
    def wait(timeout = nil)
      if @monitor.mon_owner != Thread.current
	raise ThreadError, "current thread not owner"
      end
      
      Thread.critical = true
      count = @monitor.mon_count
      @monitor.mon_count = 0
      @monitor.mon_owner = nil
      if @monitor.mon_waiting_queue.empty?
	t = @monitor.mon_entering_queue.shift
      else
	t = @monitor.mon_waiting_queue.shift
      end
      t.wakeup if t
      @waiters.push(Thread.current)
      
      if timeout
	t = Thread.current
	timeout_thread = Thread.start {
	  sleep(timeout)
	  t.raise(Timeout.new)
	}
      end
      begin
	Thread.stop
      rescue Timeout
	@waiters.delete(Thread.current)
      ensure
	if timeout && timeout_thread.alive?
	  Thread.kill(timeout_thread)
	end
      end
      
      Thread.critical = true
      while @monitor.mon_owner &&
	  @monitor.mon_owner != Thread.current
	@monitor.mon_waiting_queue.push(Thread.current)
	Thread.stop
	Thread.critical = true
      end
      @monitor.mon_owner = Thread.current
      @monitor.mon_count = count
      Thread.critical = false
    end
    
    def wait_while
      while yield
	wait
      end
    end
    
    def wait_until
      until yield
	wait
      end
    end
    
    def signal
      if @monitor.mon_owner != Thread.current
	raise ThreadError, "current thread not owner"
      end
      Thread.critical = true
      t = @waiters.shift
      t.wakeup if t
      Thread.critical = false
      Thread.pass
    end
    
    def broadcast
      if @monitor.mon_owner != Thread.current
	raise ThreadError, "current thread not owner"
      end
      Thread.critical = true
      for t in @waiters
	t.wakeup
      end
      @waiters.clear
      Thread.critical = false
      Thread.pass
    end
    
    def count_waiters
      return @waiters.length
    end
    
  private
    def initialize(monitor)
      @monitor = monitor
      @waiters = []
    end
  end
  
  include Accessible
  include Initializable
  extend Initializable
  
  def self.extend_object(obj)
    super(obj)
    obj.mon_initialize
  end
  
  def try_mon_enter
    result = false
    Thread.critical = true
    if mon_owner.nil?
      self.mon_owner = Thread.current
    end
    if mon_owner == Thread.current
      self.mon_count += 1
      result = true
    end
    Thread.critical = false
    return result
  end

  def mon_enter
    Thread.critical = true
    while mon_owner != nil && mon_owner != Thread.current
      mon_entering_queue.push(Thread.current)
      Thread.stop
      Thread.critical = true
    end
    self.mon_owner = Thread.current
    self.mon_count += 1
    Thread.critical = false
  end
  
  def mon_exit
    if mon_owner != Thread.current
      raise ThreadError, "current thread not owner"
    end
    Thread.critical = true
    self.mon_count -= 1
    if mon_count == 0
      self.mon_owner = nil
      if mon_waiting_queue.empty?
	t = mon_entering_queue.shift
      else
	t = mon_waiting_queue.shift
      end
    end
    t.wakeup if t
    Thread.critical = false
    Thread.pass
  end

  def mon_synchronize
    mon_enter
    begin
      yield
    ensure
      mon_exit
    end
  end
  alias synchronize mon_synchronize
  
  def new_cond
    return ConditionVariable.new(self)
  end
  
private
  def initialize(*args)
    super
    mon_initialize
  end
end

class Monitor
  include MonitorMixin
  alias try_enter try_mon_enter
  alias enter mon_enter
  alias exit mon_exit
  alias owner mon_owner
end

# Local variables:
# mode: Ruby
# tab-width: 8
# End:
