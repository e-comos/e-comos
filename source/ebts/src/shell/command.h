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
#ifndef COMMAND_H
#define COMMAND_H

#include "../config.h"

typedef struct shell_state shell_state_t;
typedef error_code_t (*command_handler_t)(shell_state_t* shell, int argc, char* argv[]);

typedef struct command_entry {
    char command_name[50];
    command_handler_t command_function;
    char command_describe[70];
    char command_help[105];
} command_entry_t;

void command_system_init(void);
void command_system_cleanup(void);
error_code_t command_execute(shell_state_t* shell, const char* input);
int command_register(const char* command_name, command_handler_t command_function, 
                     const char* command_describe, const char* command_help);

// Built-in commands
error_code_t cmd_whoami(shell_state_t* shell, int argc, char* argv[]);
error_code_t cmd_users(shell_state_t* shell, int argc, char* argv[]);
error_code_t cmd_switch(shell_state_t* shell, int argc, char* argv[]);
error_code_t cmd_adduser(shell_state_t* shell, int argc, char* argv[]);
error_code_t cmd_lookup(shell_state_t* shell, int argc, char* argv[]);
error_code_t command_execute_pipe(shell_state_t* shell, const char* input, char* pipe_pos);
#endif /* COMMAND_H */