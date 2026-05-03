/*
 * ECLib - E-comOS C Library
 * Copyright (C) 2025 E-comOS Kernel Mode Team & Saladin5101
 * 
 * This file is part of ECLib.
 * ECLib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 */
#include <eclib/ipc_message.h>
#include <string.h>
#include <unistd.h>
#include <e-comos/syscall.h>
#include <time.h>
#include <stdio.h> // Added for snprintf

// System call numbers
#define SYS_IPC_SEND          1001
#define SYS_IPC_RECEIVE       1002
#define SYS_IPC_DO_NOT_KILL   1003
#define SYS_IPC_BROADCAST     1004

int ipc_send_msg(uint32_t type, uint32_t flags, uint32_t receiver_pid, 
                 uint32_t data_len, const void* data) {
    ipc_message_t msg = {0};
    
    msg.type = type;
    msg.sender_pid = getpid();
    msg.receiver_pid = receiver_pid;
    msg.data_len = (data_len > 256) ? 256 : data_len;
    msg.flags = flags;
    msg.timestamp = time(NULL);
    
    if (data && data_len > 0) {
        memcpy(msg.data, data, msg.data_len);
    }
    
    // System call to send the message
    return syscall(SYS_IPC_SEND, &msg);
}

int ipc_receive_msg(ipc_message_t* msg, int timeout_ms) {
    if (!msg) {
        return -1;
    }
    
    // System call to receive the message
    int ret = syscall(SYS_IPC_RECEIVE, msg, timeout_ms);
    
    if (ret >= 0 && msg->timestamp == 0) {
        msg->timestamp = time(NULL);
    }
    
    return ret;
}

int ipc_broadcast_msg(uint32_t type, uint32_t flags, uint32_t data_len, 
                     const void* data) {
    ipc_message_t msg = {0};
    
    msg.type = type;
    msg.sender_pid = getpid();
    msg.receiver_pid = 0xFFFFFFFF;  // Broadcast address
    msg.data_len = (data_len > 256) ? 256 : data_len;
    msg.flags = flags;
    msg.timestamp = time(NULL);
    
    if (data && data_len > 0) {
        memcpy(msg.data, data, msg.data_len);
    }
    
    return syscall(SYS_IPC_BROADCAST, &msg);
}

int ipc_do_not_kill_sub(void) {
    // System call to set the flag
    int ret = syscall(SYS_IPC_DO_NOT_KILL, 0);
    
    if (ret == 0) {
        // Send IPC message to notify PowerOffer
        char data[32];
        snprintf(data, sizeof(data), "DO_NOT_KILL:%d", getpid());
        ipc_broadcast_msg(IPC_MSG_DO_NOT_KILL, 0, strlen(data), data);
    }
    
    return ret;
}

int ipc_do_not_kill_sub_emergency_ok(void) {
    // System call to set the flag (emergency kill allowed)
    int ret = syscall(SYS_IPC_DO_NOT_KILL, 1);
    
    if (ret == 0) {
        char data[32];
        snprintf(data, sizeof(data), "DO_NOT_KILL_EMERGENCY:%d", getpid());
        ipc_broadcast_msg(IPC_MSG_DO_NOT_KILL, 0, strlen(data), data);
    }
    
    return ret;
}
