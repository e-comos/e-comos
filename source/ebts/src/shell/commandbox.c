/**
 * ebts - E-comOS Basic Terminal Shell
 * Copyright (C) 2025 E-comOS Project
 */
#include "commandbox.h"
#include "command.h"
#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ECOMOS_BUILD
#define COMMANDBOX_PATH "/system/commandbox/bin/commands"
#else
#define COMMANDBOX_PATH "/Users/ddd/ebts/commandbox/bin/commands"
#endif

static const char* external_commands[] = {
    "lookup", "saw", "del", "copy", "goto", "info-of", "output", NULL
};

void commandbox_init(void) {
    printf("CommandBox library loaded\n");
    commandbox_load_commands();
}

void commandbox_cleanup(void) {
    // Cleanup commandbox resources
}

error_code_t commandbox_execute_external(const char* cmd_line) {
    char command[512];
    char* cmd_copy = strdup(cmd_line);
    char* cmd_name = strtok(cmd_copy, " ");
    char* args = strtok(NULL, "");
    
    if (args) {
        snprintf(command, sizeof(command), "%s/%s %s", COMMANDBOX_PATH, cmd_name, args);
    } else {
        snprintf(command, sizeof(command), "%s/%s", COMMANDBOX_PATH, cmd_name);
    }
    
    int result = system(command);
    free(cmd_copy);
    return (result == 0) ? EC_SUCCESS : EC_ERROR;
}

error_code_t commandbox_load_commands(void) {
    // Register built-in shell commands
    command_register("whoami", cmd_whoami, "Show current user", "Usage: whoami");
    command_register("users", cmd_users, "List all users", "Usage: users");
    command_register("adduser", cmd_adduser, "Create new user", "Usage: adduser <username> [password]");  
    
    // Add all commands as FileObjects in commandbox folder
    object_t* commandbox = object_get_commandbox();
    if (commandbox) {
        // Built-in shell commands
        object_add_child(commandbox, object_create("whoami", OBJ_TYPE_FILE));
        object_add_child(commandbox, object_create("users", OBJ_TYPE_FILE));
        object_add_child(commandbox, object_create("switch", OBJ_TYPE_FILE));
        object_add_child(commandbox, object_create("adduser", OBJ_TYPE_FILE));
        
        // External commandbox commands
        for (int i = 0; external_commands[i]; i++) {
            object_add_child(commandbox, object_create(external_commands[i], OBJ_TYPE_FILE));
        }
    }
    
    return EC_SUCCESS;
}