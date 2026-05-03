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
#ifndef SHELL_H
#define SHELL_H

#include "../config.h"
#include "object.h"
#include "user.h"

typedef struct shell_state {
    object_t* current_object;
    char hostname[64];
    int running;
} shell_state_t;

void shell_init(void);
void shell_run(void);
void shell_cleanup(void);
void shell_print_prompt(shell_state_t* shell);
char* shell_read_input(void);
void shell_set_hostname(const char* hostname);
const char* shell_get_hostname(void);

#endif /* SHELL_H */