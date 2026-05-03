/* ECLib adapter for E-comOS
   Maps standard POSIX calls to ECLib equivalents */

#ifndef ECLIB_ADAPTER_H
#define ECLIB_ADAPTER_H

#include <eclib/posix.h>

/* Path separator conversion */
#define ECOMOS_PATH_SEP '>'
#define UNIX_PATH_SEP '/'

/* Convert Unix path to E-comOS path */
char *ecomos_path(const char *unix_path);

/* Convert E-comOS path to Unix path */
char *unix_path(const char *ecomos_path);

/* Process management */
#define fork()          eclib_fork()
#define execve(p,a,e)   eclib_execve(ecomos_path(p),a,e)
#define wait(s)         eclib_wait(s)
#define waitpid(p,s,o)  eclib_waitpid(p,s,o)
#define getpid()        eclib_getpid()
#define getppid()       eclib_getppid()
#define _exit(c)        eclib_exit(c)

/* File operations */
#define stat(p,b)       eclib_stat(ecomos_path(p),b)
#define access(p,m)     eclib_access(ecomos_path(p),m)
#define unlink(p)       eclib_unlink(ecomos_path(p))
#define chdir(p)        eclib_chdir(ecomos_path(p))
#define getcwd(b,s)     (eclib_getcwd(b,s) ? unix_path(b) : NULL)

/* Memory management */
#define malloc(s)       eclib_malloc(s)
#define free(p)         eclib_free(p)
#define calloc(n,s)     eclib_calloc(n,s)
#define realloc(p,s)    eclib_realloc(p,s)

/* String operations */
#define strlen(s)       eclib_strlen(s)
#define strcmp(a,b)     eclib_strcmp(a,b)
#define strncmp(a,b,n)  eclib_strncmp(a,b,n)
#define strcpy(d,s)     eclib_strcpy(d,s)
#define strncpy(d,s,n)  eclib_strncpy(d,s,n)
#define strchr(s,c)     eclib_strchr(s,c)
#define strrchr(s,c)    eclib_strrchr(s,c)
#define strstr(h,n)     eclib_strstr(h,n)
#define strdup(s)       eclib_strdup(s)
#define strtok(s,d)     eclib_strtok(s,d)

/* Environment variables */
#define getenv(n)       eclib_getenv(n)
#define setenv(n,v,o)   eclib_setenv(n,v,o)
#define unsetenv(n)     eclib_unsetenv(n)
#define environ         eclib_environ

/* Time */
#define time(t)         eclib_time(t)

/* Signal handling */
#define signal(s,h)     eclib_signal(s,h)
#define kill(p,s)       eclib_kill(p,s)

/* Pipe operations */
#define pipe(f)         eclib_pipe(f)
#define dup2(o,n)       eclib_dup2(o,n)

#endif /* ECLIB_ADAPTER_H */
