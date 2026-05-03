/**
 * ebts - E-comOS Basic Terminal Shell
 * Copyright (C) 2025 E-comOS Project
 */
#ifndef USER_H
#define USER_H

#include "../config.h"

#define MAX_USERS 10
#define MAX_USERNAME 32

typedef struct user {
    char username[MAX_USERNAME];
    char password[64];
    int is_active;
} user_t;

void user_system_init(void);
error_code_t user_add(const char* username, const char* password);
error_code_t user_create(const char* username, const char* password);
error_code_t user_login(const char* username, const char* password);
error_code_t user_switch(const char* username);
const char* user_get_current(void);
void user_list_all(void);
void user_prompt_login(void);

#endif /* USER_H */