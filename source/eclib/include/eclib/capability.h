/**
 * ECLib - E-comOS C Library
 * Copyright (C) 2026 Saladin5101
 * 
 */
#ifndef ECLIB_CAPABILITY_H
#define ECLIB_CAPABILITY_H

#include <stdint.h>


#define CAPABILITY_SHUTDOWN      (1 << 0)  
#define CAPABILITY_REBOOT        (1 << 1)  
#define CAPABILITY_SUSPEND       (1 << 2) 
#define CAPABILITY_HIBERNATE     (1 << 3)  
#define CAPABILITY_KILL_PROCESS  (1 << 4) 
#define CAPABILITY_CHANGE_CONFIG (1 << 5)  
#define CAPABILITY_ALL           (0xFFFFFFFF)  


struct capability_token {
    uint64_t id;          
    uint32_t capabilities;
    uint32_t owner_pid; 
    uint32_t valid_until;  
    char     description[64]; 
};


struct capability_token* acquire_capability(uint32_t capabilities, 
                                           const char* description,
                                           uint32_t timeout_sec);
int release_capability(struct capability_token* token);
int renew_capability(struct capability_token* token, uint32_t add_time_sec);
int validate_capability(struct capability_token* token, uint32_t required_caps);


int send_token_to_virtual_port(struct capability_token* token);
struct capability_token* receive_token_from_virtual_port(void);
int clear_virtual_port_tokens(void);


int check_capability(uint32_t required_caps);
int check_capability_for_pid(uint32_t required_caps, uint32_t pid);

#endif // ECLIB_CAPABILITY_H