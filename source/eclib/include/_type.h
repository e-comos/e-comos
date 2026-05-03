/**
 * ECLib - E-comOS C Library
 * Copyright (C) 2026 Saladin5101
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */
#ifndef _TYPES_H
#define _TYPES_H

// Arch check 
// If you used <stddef.h> , you will get size_t type and the macro of architecture check (ECON_ARCH_64BIT or ECON_ARCH_32BIT) , so you can use it to do some arch-specific code
/*
#if defined(__x86_64__) || defined(__aarch64__) || defined(__riscv) && __riscv_xlen == 64
    // 64-bit system
    typedef unsigned long size_t;
    #define ECON_ARCH_64BIT
#else
    // 32-bit system (default)
    typedef unsigned int size_t;
    #define ECON_ARCH_32BIT
#endif

typedef size_t;
*/ // Note , it is moved into <stddef.h> , 

// Other E-comOS types
typedef int econ_pid_t;
typedef unsigned short econ_msg_id_t;
typedef int econ_err_t; // Note , in E-comOS we use eclib_err_t are NOT econ_err_t

#endif // _TYPES_H