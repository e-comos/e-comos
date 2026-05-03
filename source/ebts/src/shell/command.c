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
#include "command.h"
#include "shell.h"
#include "object.h"
#include "user.h"
#include "commandbox.h"
#include <stdio.h>
#include <string.h>

#define MAX_COMMANDS 250
static command_entry_t command_registry[MAX_COMMANDS];
static int command_count = 0;

void command_system_init(void) {
    command_count = 0;
    printf("Command system initialized\n");
}

void command_system_cleanup(void) {
    command_count = 0;
}

int command_register(const char* command_name, command_handler_t command_function, 
                     const char* command_describe, const char* command_help) {
    if (command_count >= MAX_COMMANDS) {
        printf("Cannot register command '%s': registry full\n", command_name);
        return -1;
    }
    
    strncpy(command_registry[command_count].command_name, command_name, 49);
    command_registry[command_count].command_name[49] = '\0';
    command_registry[command_count].command_function = command_function;
    strncpy(command_registry[command_count].command_describe, command_describe, 69);
    command_registry[command_count].command_describe[69] = '\0';
    strncpy(command_registry[command_count].command_help, command_help, 104);
    command_registry[command_count].command_help[104] = '\0';
    
    command_count++;
    return 0;
}

error_code_t command_execute(shell_state_t* shell, const char* input) {
    // Check for pipe operator ->
    char* pipe_pos = strstr(input, " -> ");
    if (pipe_pos) {
        return command_execute_pipe(shell, input, pipe_pos);
    }
    
    // Check registered commands first
    for (int i = 0; i < command_count; i++) {
        if (strcmp(input, command_registry[i].command_name) == 0) {
            return command_registry[i].command_function(shell, 1, (char*[]){(char*)input});
        }
    }
    
    // Handle commands with arguments
    if (strncmp(input, "switch ", 7) == 0) {
        char* username = (char*)input + 7;
        char* args[] = {"switch", username};
        return cmd_switch(shell, 2, args);
    } else if (strncmp(input, "adduser ", 8) == 0) {
        char* username = (char*)input + 8;
        char* args[] = {"adduser", username};
        return cmd_adduser(shell, 2, args);
    } else if (strncmp(input, "lookup ", 7) == 0) {
        char* target = (char*)input + 7;
        char* args[] = {"lookup", target};
        return cmd_lookup(shell, 2, args);
    }
    
    // Try external commandbox
    return commandbox_execute_external(input);
}

error_code_t cmd_whoami(shell_state_t* shell, int argc, char* argv[]) {
    (void)shell; (void)argc; (void)argv;
    printf("%s\n", user_get_current());
    return EC_SUCCESS;
}

error_code_t cmd_users(shell_state_t* shell, int argc, char* argv[]) {
    (void)shell; (void)argc; (void)argv;
    user_list_all();
    return EC_SUCCESS;
}

error_code_t cmd_switch(shell_state_t* shell, int argc, char* argv[]) {
    (void)shell;
    if (argc < 2) {
        printf("Usage: switch <username>\n");
        return EC_ERROR;
    }
    
    if (user_switch(argv[1]) == EC_SUCCESS) {
        printf("Switched to user: %s\n", argv[1]);
        return EC_SUCCESS;
    } else {
        printf("User '%s' not found\n", argv[1]);
        return EC_ERROR;
    }
}

error_code_t cmd_adduser(shell_state_t* shell, int argc, char* argv[]) {
    (void)shell;
    if (argc < 2) {
        printf("Usage: adduser <username> [password]\n");
        return EC_ERROR;
    }
    
    const char* password = (argc > 2) ? argv[2] : "";
    
    if (user_create(argv[1], password) == EC_SUCCESS) {
        printf("User '%s' created successfully\n", argv[1]);
        return EC_SUCCESS;
    } else {
        printf("Failed to create user '%s' (user may already exist or max users reached)\n", argv[1]);
        return EC_ERROR;
    }
}

error_code_t cmd_lookup(shell_state_t* shell, int argc, char* argv[]) {
    object_t* target = shell->current_object;
    
    if (argc > 1 && strcmp(argv[1], ".") != 0) {
        target = object_find(shell->current_object, argv[1]);
        if (!target) {
            printf("Object '%s' not found\n", argv[1]);
            return EC_ERROR;
        }
    }
    
    if (target->type != OBJ_TYPE_FOLDER) {
        printf("'%s' is a FileObject, not a FolderObject\n", target->name);
        return EC_ERROR;
    }
    
    printf("Contents of %s:\n", target->name);
    for (int i = 0; i < target->child_count; i++) {
        const char* type = (target->children[i]->type == OBJ_TYPE_FOLDER) ? "FolderObject" : "FileObject";
        printf("  %s (%s)\n", target->children[i]->name, type);
    }
    
    return EC_SUCCESS;
}