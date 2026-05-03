/**
 * ebts - E-comOS Basic Terminal Shell
 * Copyright (C) 2025,2026 Saladin5101
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
#ifndef OBJECT_H
#define OBJECT_H

#include "../config.h"
#include <stddef.h>

typedef struct object {
    char name[MAX_OBJECT_NAME];
    object_type_t type;
    struct object* parent;
    struct object* children[MAX_CHILDREN];
    int child_count;
    char* data;
    size_t data_size;
} object_t;

void object_system_init(void);
void object_system_cleanup(void);
object_t* object_create(const char* name, object_type_t type);
object_t* object_find(object_t* parent, const char* name);
void object_add_child(object_t* parent, object_t* child);
object_t* object_get_root(void);
object_t* object_get_commandbox(void);

#endif /* OBJECT_H */