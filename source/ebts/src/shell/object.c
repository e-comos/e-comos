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
#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static object_t* root_object = NULL;

void object_system_init(void) {
    root_object = object_create("root", OBJ_TYPE_FOLDER);
    
    // Create empty commandbox folder (commands will be added by commandbox library)
    object_t* commandbox = object_create("commandbox", OBJ_TYPE_FOLDER);
    object_add_child(root_object, commandbox);
}

void object_system_cleanup(void) {
    // Cleanup implementation would go here
}

object_t* object_create(const char* name, object_type_t type) {
    object_t* obj = malloc(sizeof(object_t));
    if (!obj) return NULL;
    
    strncpy(obj->name, name, MAX_OBJECT_NAME - 1);
    obj->name[MAX_OBJECT_NAME - 1] = '\0';
    obj->type = type;
    obj->parent = NULL;
    obj->child_count = 0;
    obj->data = NULL;
    obj->data_size = 0;
    
    for (int i = 0; i < MAX_CHILDREN; i++) {
        obj->children[i] = NULL;
    }
    
    return obj;
}

object_t* object_find(object_t* parent, const char* name) {
    if (!parent || parent->type != OBJ_TYPE_FOLDER) return NULL;
    
    for (int i = 0; i < parent->child_count; i++) {
        if (strcmp(parent->children[i]->name, name) == 0) {
            return parent->children[i];
        }
    }
    return NULL;
}

void object_add_child(object_t* parent, object_t* child) {
    if (!parent || !child || parent->type != OBJ_TYPE_FOLDER) return;
    if (parent->child_count >= MAX_CHILDREN) return;
    
    parent->children[parent->child_count] = child;
    child->parent = parent;
    parent->child_count++;
}

object_t* object_get_root(void) {
    return root_object;
}

object_t* object_get_commandbox(void) {
    return object_find(root_object, "commandbox");
}