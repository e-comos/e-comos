#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 256
#define COMMANDS_PATH "./bin/commands/"

const char* commands[] = {
    "lookup", "saw", "del", "copy", "goto", "info-of", "output", NULL
};

int execute_external_command(char *cmd, char *args[], int argc) {
    char cmd_path[512];
    snprintf(cmd_path, sizeof(cmd_path), "%s%s", COMMANDS_PATH, cmd);
    
    // Check if command exists
    if (access(cmd_path, X_OK) != 0) {
        return -1;
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        char *argv[argc + 2];
        argv[0] = cmd_path;
        for (int i = 0; i < argc; i++) {
            argv[i + 1] = args[i];
        }
        argv[argc + 1] = NULL;
        
        execv(cmd_path, argv);
        exit(1);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        // Command line mode
        char *args[argc - 1];
        for (int i = 1; i < argc - 1; i++) {
            args[i - 1] = argv[i + 1];
        }
        
        int result = execute_external_command(argv[1], args, argc - 2);
        if (result == -1) {
            printf("Unknown command: %s\n", argv[1]);
            return 1;
        }
        return result;
    }
    
    // Interactive mode
    char input[MAX_CMD_LEN];
    printf("E-comOS CommandBox (Simple)\n");
    
    while (1) {
        printf("commandbox> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        if (strlen(input) == 0) continue;
        if (strcmp(input, "exit") == 0) break;
        
        // Parse command
        char *token = strtok(input, " \t");
        if (!token) continue;
        
        char *cmd = token;
        char *args[32];
        int arg_count = 0;
        
        while ((token = strtok(NULL, " \t")) && arg_count < 31) {
            args[arg_count++] = token;
        }
        
        int result = execute_external_command(cmd, args, arg_count);
        if (result == -1) {
            printf("Unknown command: %s\n", cmd);
        }
    }
    
    return 0;
}