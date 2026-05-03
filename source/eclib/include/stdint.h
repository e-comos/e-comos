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
// C99 Standard 7.18 Integer types <stdint.h>
// Defines fixed-width, minimum-width, and maximum-width integer types
// Compliant with C99 standard; no implementation file (.c) required for basic usage
#ifndef STDINT_H
#define STDINT_H
// -------------------------- Fixed-Width Integer Types (C99 7.18.1.1) --------------------------
// Exact-width integer types: guaranteed to have the specified width and no padding bits
// Signed exact-width types
typedef signed char         int8_t;    // 8-bit signed integer
typedef signed short        int16_t;   // 16-bit signed integer
typedef signed int          int32_t;   // 32-bit signed integer
typedef signed long long    int64_t;   // 64-bit signed integer

// Unsigned exact-width types
typedef unsigned char       uint8_t;   // 8-bit unsigned integer
typedef unsigned short      uint16_t;  // 16-bit unsigned integer
typedef unsigned int        uint32_t;  // 32-bit unsigned integer
typedef unsigned long long  uint64_t;  // 64-bit unsigned integer

// -------------------------- Minimum-Width Integer Types (C99 7.18.1.2) --------------------------
// Minimum-width integer types: guaranteed to have at least the specified width
// Signed minimum-width types
typedef signed char         int_least8_t;    // At least 8-bit signed integer
typedef signed short        int_least16_t;   // At least 16-bit signed integer
typedef signed int          int_least32_t;   // At least 32-bit signed integer
typedef signed long long    int_least64_t;   // At least 64-bit signed integer

// Unsigned minimum-width types
typedef unsigned char       uint_least8_t;   // At least 8-bit unsigned integer
typedef unsigned short      uint_least16_t;  // At least 16-bit unsigned integer
typedef unsigned int        uint_least32_t;  // At least 32-bit unsigned integer
typedef unsigned long long  uint_least64_t;  // At least 64-bit unsigned integer

// -------------------------- Fastest Minimum-Width Integer Types (C99 7.18.1.3) --------------------------
// Fastest minimum-width integer types: optimal for performance on the target system
// Signed fastest types
typedef signed char         int_fast8_t;     // Fastest at least 8-bit signed integer
typedef signed int          int_fast16_t;    // Fastest at least 16-bit signed integer
typedef signed int          int_fast32_t;    // Fastest at least 32-bit signed integer
typedef signed long long    int_fast64_t;    // Fastest at least 64-bit signed integer

// Unsigned fastest types
typedef unsigned char       uint_fast8_t;    // Fastest at least 8-bit unsigned integer
typedef unsigned int        uint_fast16_t;   // Fastest at least 16-bit unsigned integer
typedef unsigned int        uint_fast32_t;   // Fastest at least 32-bit unsigned integer
typedef unsigned long long  uint_fast64_t;   // Fastest at least 64-bit unsigned integer

// -------------------------- Greatest-Width Integer Types (C99 7.18.1.4) --------------------------
// Greatest-width integer types: can hold any value of any other integer type on the system
typedef signed long long    intmax_t;        // Greatest-width signed integer
typedef unsigned long long  uintmax_t;       // Greatest-width unsigned integer

// -------------------------- Integer Type Limits (C99 7.18.2) --------------------------
// Limits for exact-width signed integers
#define INT8_MIN            (-128)
#define INT16_MIN           (-32768)
#define INT32_MIN           (-2147483648LL)
#define INT64_MIN           (-9223372036854775808LL)

#define INT8_MAX            127
#define INT16_MAX           32767
#define INT32_MAX           2147483647
#define INT64_MAX           9223372036854775807LL

// Limits for exact-width unsigned integers
#define UINT8_MAX           255U
#define UINT16_MAX          65535U
#define UINT32_MAX          4294967295U
#define UINT64_MAX          18446744073709551615ULL

// -------------------------- Optional: Pointer-Width Integer Types (C99 7.18.3) --------------------------
// Integer types capable of holding a pointer (optional, E-comOS v0.1 supports for x86_64)
typedef long                intptr_t;
typedef unsigned long       uintptr_t;

#define INTPTR_MIN          INT32_MIN
#define INTPTR_MAX          INT32_MAX
#define UINTPTR_MAX         UINT32_MAX
#endif // STDINT_H