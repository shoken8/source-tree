#ifndef EXT_NT_H
#define EXT_NT_H

/*
 *  Copyright (c) 1993, Intergraph Corporation
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License or the Artistic License, as specified in the perl README file.
 *
 */

#if defined(IMPORT)
#define EXTERN extern __declspec(dllimport)
#elif defined(EXPORT)
#define EXTERN extern __declspec(dllexport)
#endif

//
// Definitions for NT port of Perl
//

//
// GRRRR!!!!  Windows Nonsense.
// Define the following so we don't get tons of extra stuff
// when we include windows.h 
//
#if 0
#define NOGDICAPMASKS     
#define NOVIRTUALKEYCODES 
#define NOWINMESSAGES     
#define NOWINSTYLES       
#define NOSYSMETRICS      
#define NOMENUS           
#define NOICONS           
#define NOKEYSTATES       
#define NOSYSCOMMANDS     
#define NORASTEROPS       
#define NOSHOWWINDOW      
#define OEMRESOURCE       
#define NOATOM            
#define NOCLIPBOARD       
#define NOCOLOR           
#define NOCTLMGR          
#define NODRAWTEXT        
#define NOGDI             
//#define NOKERNEL        
//#define NOUSER          
#define NONLS             
#define NOMB              
#define NOMEMMGR          
#define NOMETAFILE        
#define NOMINMAX          
#define NOMSG             
#define NOOPENFILE        
#define NOSCROLL          
#define NOSERVICE         
#define NOSOUND           
#define NOTEXTMETRIC      
#define NOWH              
#define NOWINOFFSETS      
#define NOCOMM            
#define NOKANJI           
#define NOHELP            
#define NOPROFILER        
#define NODEFERWINDOWPOS  
#endif

//
// Ok now we can include the normal include files.
//

// #include <stdarg.h> conflict with varargs.h?
// There is function-name conflitct, so we rename it
#if !defined(IN) && !defined(FLOAT)
#define OpenFile  WINAPI_OpenFile
#include <windows.h>
#include <winsock.h>
#undef OpenFile
#endif
//
// We\'re not using Microsoft\'s "extensions" to C for
// Structured Exception Handling (SEH) so we can nuke these
//
#undef try
#undef except
#undef finally
#undef leave

#if defined(__cplusplus)
}
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <process.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/utime.h>
#include <io.h>
#include <malloc.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define UIDTYPE int
#define GIDTYPE int
#define pid_t   int
#define WNOHANG -1

#define access	   _access
#define chmod	   _chmod
#define chsize	   _chsize
#define close	   _close
#define creat	   _creat
#define dup	   _dup
#define dup2	   _dup2
#define eof	   _eof
#define filelength _filelength
#define isatty	   _isatty
#define locking    _locking
#define lseek	   _lseek
#define mktemp	   _mktemp
#define open	   _open
#define perror     _perror
#define read	   _read
#define setmode    _setmode
#define sopen	   _sopen
#define tell	   _tell
#define umask	   _umask
#define unlink	   _unlink
#define write	   _write
#define execl	   _execl
#define execle	   _execle
#define execlp	   _execlp
#define execlpe    _execlpe
#define execv	   _execv
#define execve	   _execve
#define execvp	   _execvp
#define execvpe    _execvpe
#define getpid	   _getpid
#define sleep(x)   Sleep((x)*1000)
#define spawnl	   _spawnl
#define spawnle    _spawnle
#define spawnlp    _spawnlp
#define spawnlpe   _spawnlpe
#define spawnv	   _spawnv
#define spawnve    _spawnve
#define spawnvp    _spawnvp
#define spawnvpe   _spawnvpe
#if _MSC_VER < 800
#define fileno	   _fileno
#endif
#define utime      _utime
#define vsnprintf  _vsnprintf
#define snprintf   _snprintf
#define popen      _popen
#define pclose     _pclose
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
/* these are defined in nt.c */

extern int NtMakeCmdVector(char *, char ***, int);
extern void NtInitialize(int *, char ***);
extern char *NtGetLib(void);
extern char *NtGetBin(void);
extern FILE *mypopen(char *, char *);
extern int   mypclose(FILE *);
extern int  flock(int fd, int oper);
extern FILE *  myfdopen(int, const char *);
extern void  myfdclose(FILE *);
extern SOCKET  myaccept(SOCKET, struct sockaddr *, int *);
extern int  mybind(SOCKET, struct sockaddr *, int);
extern int  myconnect(SOCKET, struct sockaddr *, int);
extern void myfdset(int, fd_set*);
extern int  myfdisset(int, fd_set*);
extern long myselect(int, fd_set *, fd_set *, fd_set *, struct timeval *);
extern int  mygetpeername(SOCKET, struct sockaddr *, int *);
extern int  mygetsockname(SOCKET, struct sockaddr *, int *);
extern int  mygetsockopt(SOCKET, int, int, char *, int *);
extern int  myioctlsocket(SOCKET, long, u_long *);
extern int  mylisten(SOCKET, int);
extern int  myrecv(SOCKET, char *, int, int);
extern int  myrecvfrom(SOCKET, char *, int, int, struct sockaddr *, int *);
extern int  mysend(SOCKET, char *, int, int);
extern int  mysendto(SOCKET, char *, int, int, struct sockaddr *, int);
extern int  mysetsockopt(SOCKET, int, int, char *, int);
extern int  myshutdown(SOCKET, int);
extern SOCKET  mysocket(int, int, int);
extern SOCKET  myget_osfhandle(int);
extern struct hostent *  mygethostbyaddr(char *, int, int);
extern struct hostent *  mygethostbyname(char *);
extern int  mygethostname(char *, int);
extern struct protoent *  mygetprotobyname(char *);
extern struct protoent *  mygetprotobynumber(int);
extern struct servent *  mygetservbyname(char *, char *);
extern struct servent * mygetservbyport(int, char *);
extern char *win32_getenv(const char *);

extern int chown(const char *, int, int);
extern int link(char *, char *);
extern int gettimeofday(struct timeval *, struct timezone *);
extern pid_t waitpid (pid_t, int *, int);
extern int do_spawn(char *);
extern int kill(int, int);
extern int isinf(double);
extern int isnan(double);


//
// define this so we can do inplace editing
//

#define SUFFIX

//
// stubs
//
extern int       ioctl (int, unsigned int, long);
extern UIDTYPE   getuid (void);
extern UIDTYPE   geteuid (void);
extern GIDTYPE   getgid (void);
extern GIDTYPE   getegid (void);
extern int       setuid (int);
extern int       setgid (int);


#if 0
extern int sys_nerr;
extern char *sys_errlist[];
#endif
extern char *mystrerror(int);

#define strerror(e) mystrerror(e)

#define PIPE_BUF 1024

#define LOCK_SH 1
#define LOCK_EX 2
#define LOCK_NB 4
#define LOCK_UN 8
#ifndef EWOULDBLOCK
#define EWOULDBLOCK 10035 /* EBASEERR + 35 (winsock.h) */
#endif

#ifdef popen
#undef popen
#define popen    mypopen
#endif
#ifdef pclose
#undef pclose
#define pclose   mypclose
#endif

/* #undef va_start */
/* #undef va_end */

#ifdef fdopen
#undef fdopen
#endif
#define fdopen myfdopen

#ifdef accept
#undef accept
#endif
#define accept myaccept

#ifdef bind
#undef bind
#endif
#define bind mybind

#ifdef connect
#undef connect
#endif
#define connect myconnect

#undef FD_SET
#define FD_SET myfdset

#undef FD_ISSET
#define FD_ISSET myfdisset

#undef select
#define select myselect

#ifdef getpeername
#undef getpeername
#endif
#define getpeername mygetpeername

#ifdef getsockname
#undef getsockname
#endif
#define getsockname mygetsockname

#ifdef getsockopt
#undef getsockopt
#endif
#define getsockopt mygetsockopt

#ifdef ioctlsocket
#undef ioctlsocket
#endif
#define ioctlsocket myioctlsocket

#ifdef listen
#undef listen
#endif
#define listen mylisten

#ifdef recv
#undef recv
#endif
#define recv myrecv

#ifdef recvfrom
#undef recvfrom
#endif
#define recvfrom myrecvfrom

#ifdef send
#undef send
#endif
#define send mysend

#ifdef sendto
#undef sendto
#endif
#define sendto mysendto

#ifdef setsockopt
#undef setsockopt
#endif
#define setsockopt mysetsockopt

#ifdef shutdown
#undef shutdown
#endif
#define shutdown myshutdown

#ifdef socket
#undef socket
#endif
#define socket mysocket

#ifdef gethostbyaddr
#undef gethostbyaddr
#endif
#define gethostbyaddr mygethostbyaddr

#ifdef gethostbyname
#undef gethostbyname
#endif
#define gethostbyname mygethostbyname

#ifdef gethostname
#undef gethostname
#endif
#define gethostname mygethostname

#ifdef getprotobyname
#undef getprotobyname
#endif
#define getprotobyname mygetprotobyname

#ifdef getprotobynumber
#undef getprotobynumber
#endif
#define getprotobynumber mygetprotobynumber

#ifdef getservbyname
#undef getservbyname
#endif
#define getservbyname mygetservbyname

#ifdef getservbyport
#undef getservbyport
#endif
#define getservbyport mygetservbyport

#ifdef get_osfhandle
#undef get_osfhandle
#endif
#define get_osfhandle myget_osfhandle

#ifdef getenv
#undef getenv
#endif
#define getenv win32_getenv

#endif
