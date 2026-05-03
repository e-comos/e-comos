/**
 * ebts - E-comOS Basic Terminal Shell
 * Copyright (C) 2025 E-comOS Project
 */
#include "user.h"
#include <stdio.h>
#include <string.h>

static user_t users[MAX_USERS];
static int user_count = 0;
static int current_user_index = -1;

void user_system_init(void) {
    user_count = 0;
    current_user_index = -1;
    
    // Add default users
    user_add("user", "password");
    user_add("admin", "admin123");
    user_add("guest", "");
    // TODO : It is too simple. We should implement a more secure user management system with password hashing and permissions.
    // TODO : UAL developing.
}

error_code_t user_add(const char* username, const char* password) {
    if (user_count >= MAX_USERS) return EC_ERROR;
    
    strncpy(users[user_count].username, username, MAX_USERNAME - 1);
    users[user_count].username[MAX_USERNAME - 1] = '\0';
    strncpy(users[user_count].password, password, 63);
    users[user_count].password[63] = '\0';
    users[user_count].is_active = 1;
    
    user_count++;
    return EC_SUCCESS;
}

error_code_t user_create(const char* username, const char* password) {
    // Check if user already exists
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return EC_ERROR; // User already exists
        }
    }
    
    return user_add(username, password);
}

error_code_t user_switch(const char* username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            current_user_index = i;
            return EC_SUCCESS;
        }
    }
    return EC_ERROR;
}

const char* user_get_current(void) {
    if (current_user_index >= 0 && current_user_index < user_count) {
        return users[current_user_index].username;
    }
    return "unknown";
}

void user_list_all(void) {
    printf("Available users:\n");
    for (int i = 0; i < user_count; i++) {
        printf("  %s%s\n", users[i].username, 
               (i == current_user_index) ? " (current)" : "");
    }
}

void user_prompt_login(void) {
    char username[MAX_USERNAME];
    
    printf("Available users: ");
    for (int i = 0; i < user_count; i++) {
        printf("%s%s", users[i].username, (i < user_count - 1) ? ", " : "");
    }
    printf("\n");
    
    while (current_user_index == -1) {
        printf("Login as: ");
        fflush(stdout);
        
        if (fgets(username, sizeof(username), stdin)) {
            size_t len = strlen(username);
            if (len > 0 && username[len-1] == '\n') {
                username[len-1] = '\0';
            }
            
            if (user_switch(username) == EC_SUCCESS) {
                printf("Welcome, %s!\n", username);
                break;
            } else {
                printf("User '%s' not found. Try again.\n", username);
            }
        }
    }
}