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
#include "shell.h"
#include "command.h"
#include "commandbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static shell_state_t shell_state;

void shell_init(void) {
    strcpy(shell_state.hostname, "computer");
    shell_state.running = 1;
    
    user_system_init();
    user_prompt_login();
    command_system_init();
    object_system_init();
    commandbox_init();
    shell_state.current_object = object_get_root();
}

void shell_run(void) {
    char* input;
    
    while (shell_state.running) {
        shell_print_prompt(&shell_state);
        input = shell_read_input();
        
        if (!input) {
            shell_state.running = 0;
            break;
        }
        
        if (strlen(input) > 0) {
            if (strcmp(input, "exit") == 0) {
                shell_state.running = 0;
            } else {
                command_execute(&shell_state, input);
            }
        }
        
        free(input);
    }
}

void shell_cleanup(void) {
    commandbox_cleanup();
    command_system_cleanup();
    object_system_cleanup();
}

void shell_print_prompt(shell_state_t* shell) {
    const char* current_dir = shell->current_object ? shell->current_object->name : "unknown";
    const char* username = user_get_current();
    printf("[%s:%s] %s > ", shell->hostname, username, current_dir);
    fflush(stdout);
}

void shell_set_hostname(const char* hostname) {
    strncpy(shell_state.hostname, hostname, 63);
    shell_state.hostname[63] = '\0';
}

const char* shell_get_hostname(void) {
    return shell_state.hostname;
}

char* shell_read_input(void) {
    char* input = malloc(MAX_INPUT_LENGTH);
    if (!input) return NULL;
    
    if (fgets(input, MAX_INPUT_LENGTH, stdin)) {
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
        return input;
    }
    
    free(input);
    return NULL;
}