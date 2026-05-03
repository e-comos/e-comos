/**
 * ebts - E-comOS Basic Terminal Shell
 * Copyright (C) 2025 E-comOS Project
 */
#ifndef COMMANDBOX_H
#define COMMANDBOX_H

#include "../config.h"

// CommandBox library interface
void commandbox_init(void);
void commandbox_cleanup(void);
error_code_t commandbox_load_commands(void);
error_code_t commandbox_execute_external(const char* cmd_line);

#endif /* COMMANDBOX_H */