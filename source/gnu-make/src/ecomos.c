/* E-comOS specific implementation for GNU Make */

#include "makeint.h"
#include "eclib_adapter.h"
#include <eclib/process.h>
#include <eclib/filesystem.h>
#include <eclib/file.h>
#include <eclib/pipe.h>

/* E-comOS shell path */
#define ECOMOS_SHELL ">bin>esh"

/* Execute a command using E-comOS shell */
int ecomos_exec_shell(char **argv, char **envp) {
    char *shell_argv[4];
    
    shell_argv[0] = ECOMOS_SHELL;
    shell_argv[1] = "-c";
    shell_argv[2] = argv[0];  /* Command string */
    shell_argv[3] = NULL;
    
    return eclib_execve(ECOMOS_SHELL, shell_argv, envp);
}

/* Check if file exists and get timestamp */
int ecomos_file_stat(const char *path, eclib_stat_t *st) {
    char *epath = ecomos_path(path);
    if (!epath) return -1;
    
    int result = eclib_stat(epath, st);
    eclib_free(epath);
    
    return result;
}

/* Compare file timestamps for dependency checking */
int ecomos_file_newer(const char *file1, const char *file2) {
    eclib_stat_t st1, st2;
    
    if (ecomos_file_stat(file1, &st1) != 0) return -1;
    if (ecomos_file_stat(file2, &st2) != 0) return -1;
    
    if (st1.mtime_sec > st2.mtime_sec) return 1;
    if (st1.mtime_sec < st2.mtime_sec) return -1;
    
    return 0;
}

/* Create a child process for executing commands */
pid_t ecomos_spawn_child(char *cmd, char **envp) {
    pid_t pid = eclib_fork();
    
    if (pid == 0) {
        /* Child process */
        char *argv[4];
        argv[0] = ECOMOS_SHELL;
        argv[1] = "-c";
        argv[2] = cmd;
        argv[3] = NULL;
        
        eclib_execve(ECOMOS_SHELL, argv, envp);
        eclib_exit(127);  /* exec failed */
    }
    
    return pid;
}

/* Wait for child process and return status */
int ecomos_wait_child(pid_t pid) {
    int status;
    pid_t result = eclib_waitpid(pid, &status, 0);
    
    if (result == pid) {
        return status;
    }
    
    return -1;
}

/* Setup pipe for inter-process communication */
int ecomos_setup_pipe(int fds[2]) {
    return eclib_pipe(fds);
}

/* Redirect file descriptor */
int ecomos_redirect_fd(int oldfd, int newfd) {
    return eclib_dup2(oldfd, newfd);
}
