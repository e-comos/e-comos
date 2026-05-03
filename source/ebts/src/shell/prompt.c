/**
 * ebts - E-comOS Basic Terminal Shell
 * Copyright (C) 2025 E-comOS Project
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <eclib/all.h>
#include "command.h"
error_code_t command_execute_pipe(shell_state_t* shell, const char* input, char* pipe_pos) {
    // Split the command into left and right parts
    char left_c.   md[256];
    char right_cmd[256];
    
    // Copy left command (before the pipe operator)
    int left_len = pipe_pos - input;
    strncpy(left_cmd, input, left_len);
    left_cmd[left_len] = '\0';
    
    // Copy right command (skip " -> ")
    strcpy(right_cmd, pipe_pos + 4);
    
    // Trim whitespace from both commands
    trim(left_cmd);
    trim(right_cmd);
    
    // Create pipe for IPC communication
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        printf("Error: Failed to create pipe\n");
        return EC_ERROR;
    }
    
    pid_t left_pid, right_pid;
    
    // Fork for left command
    left_pid = fork();
    if (left_pid == 0) {
        // Child process for left command
        close(pipefd[0]);  // Close read end
        
        // Redirect stdout to pipe write end
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        // Execute left command
        error_code_t result = command_execute(shell, left_cmd);
        exit(result == EC_SUCCESS ? 0 : 1);
    } else if (left_pid < 0) {
        printf("Error: Failed to fork for left command\n");
        close(pipefd[0]);
        close(pipefd[1]);
        return EC_ERROR;
    }
    
    // Fork for right command
    right_pid = fork();
    if (right_pid == 0) {
        // Child process for right command
        close(pipefd[1]);  // Close write end
        
        // Redirect stdin to pipe read end
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        
        // Execute right command
        error_code_t result = command_execute(shell, right_cmd);
        exit(result == EC_SUCCESS ? 0 : 1);
    } else if (right_pid < 0) {
        printf("Error: Failed to fork for right command\n");
        close(pipefd[0]);
        close(pipefd[1]);
        kill(left_pid, SIGTERM);  // Clean up left process
        return EC_ERROR;
    }
    
    // Parent process - close pipe ends and wait for children
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Wait for both processes to complete
    int left_status, right_status;
    waitpid(left_pid, &left_status, 0);
    waitpid(right_pid, &right_status, 0);
    
    // Return success if both commands succeeded
    if (WIFEXITED(left_status) && WIFEXITED(right_status) &&
        WEXITSTATUS(left_status) == 0 && WEXITSTATUS(right_status) == 0) {
        return EC_SUCCESS;
    } else {
        return EC_ERROR;
    }
}