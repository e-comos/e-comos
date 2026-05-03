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
// C99 Standard 7.19 Common definitions <stddef.h>
// Defines basic macros and types for general-purpose use
// Compliant with C99 standard; no implementation file (.c) required for basic usage
#ifndef STDDEF_H
#define STDDEF_H
// -------------------------- Core Macro Definition (C99 7.19.3) --------------------------
// Null pointer constant: represents an invalid or uninitialized pointer
#define NULL ((void *)0)

// -------------------------- Core Type Definitions (C99 7.19.1 & 7.19.2) --------------------------
// size_t: unsigned integer type for representing object sizes, array indices, and return values of sizeof
#if defined(__x86_64__) || defined(__aarch64__) || defined(__riscv) && __riscv_xlen == 64
    // 64-bit system
    typedef unsigned long size_t;
    #define ECON_ARCH_64BIT
#else
    // 32-bit system (default)
    typedef unsigned int size_t;
    #define ECON_ARCH_32BIT
#endif

// ptrdiff_t: signed integer type for representing the difference between two pointers
// Compatible with x86 32-bit architecture (E-comOS v0.1)
typedef int ptrdiff_t;

// wchar_t: wide character type (optional for E-comOS v0.1, reserved for future Unicode support)
// Represents a single wide character (larger than 8-bit ASCII)
typedef unsigned short wchar_t;

// -------------------------- Structure Member Offset Macro (C99 7.19.4) --------------------------
// offsetof: computes the offset (in bytes) of a member from the start of a structure
// Parameters:
//   - type: the type of the structure
//   - member: the name of the member within the structure
// Return: the offset of the member (as size_t) from the start of the structure
#define offsetof(type, member) ((size_t) &((type *)0)->member)
#endif /* STDDEF_H */