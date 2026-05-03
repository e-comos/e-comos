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

#ifndef STRING_H
#define STRING_H
#include <stddef.h>
// ========== Copy Functions ==========
/**
 * Copy memory area
 * @param dest target memory area
 * @param src source memory area
 * @param n number of bytes to copy
 * @return pointer to dest
 */
void* memcpy(void* restrict dest, const void* restrict src, size_t n);

/**
 * Move memory area (handles overlapping regions)
 * @param dest target memory area
 * @param src source memory area
 * @param n number of bytes to move
 * @return pointer to dest
 */

void* memmove(void* dest, const void* src, size_t n);

/**
 * Copy string (null-terminated)
 * @param dest target string buffer
 * @param src source string
 * @return pointer to dest
 */
char* strcpy(char* dest, const char* src);

/**
 * Copy string with length limit
 * @param dest target string buffer
 * @param src source string
 * @param n maximum number of characters to copy
 * @return pointer to dest
 */
char* strncpy(char* restrict dest, const char* restrict src, size_t n);

// ========== Concat Functions ==========
/**
 * Concatenate strings (null-terminated)
 * @param dest target string buffer
 * @param src source string
 * @return pointer to dest
 */
char* strcat(char* dest, const char* src);

/**
 * Concatenate strings with length limit
 * @param dest target string buffer
 * @param src source string
 * @param n maximum number of characters to copy
 * @return pointer to dest
 */
char* strncat(char* restrict dest, const char* restrict src, size_t n);

// ========== Comparison functions ===========
/**
* Compare memory regions
* @param s1 Memory region 1
* @param s2 Memory region 2
* @param n The number of bytes to compare
* @return An integer, less than 0 means s1 < s2, equal to 0 means s1 = s2, greater than 0 means s1 > s2
*/
int memcmp(const void* s1, const void* s2, size_t n);

/**
 * Compare strings (null-terminated)
 * @param s1 String 1
 * @param s2 String 2
 * @return An integer, less than 0 means s1 < s2, equal to 0 means s1 = s2, greater than 0 means s1 > s2
 */
int strcmp(const char* s1, const char* s2);

/**
 * Compare strings (With length limit)
 * @param s1 String 1
 * @param s2 String 2
 * @param n Maximum number of characters to compare
 * @return An integer, less than 0 means s1 < s2, equal to 0 means s1 = s2, greater than 0 means s1 > s2
 */
int strncmp(const char* s1, const char* s2, size_t n);

// ========== Search Functions ==========
/**
 * In memory area to search string (null-terminated)
 * @param s Will search string
 * @param c Want to search character (converted to unsigned char)
 * @param n The number of bytes to search
 * @return If found, return a pointer to the first occurrence of c in s; otherwise, return NULL
 */
void* memchr(const void* s, int c, size_t n);

/**
 * In string to search string (null-terminated)
 * @param s Will search string
 * @param c Want to search character
 * @return If found, return a pointer to the first occurrence of c in s; otherwise, return NULL
 */
char* strchr(const char* s, int c);

/**
 * In string to search string from last
 * @param s Will search string
 * @param c Want to search character
 * @return If found , return a pointer to the last occurrence of c in s; otherwise, return NULL
 */
char* strrchr(const char* s, int c);

/**
 * Find the first character in a string that matches a specified character set.
 * @param s1 Will search string
 * @param s2 Have will search string's string
 * @return If found, return a pointer to the first occurrence of any character from s2 in s1; otherwise, return NULL
 */
char* strpbrk(const char* s1, const char* s2);

/**
 * In strings to search sub-string
 * @param haystack Will search string
 * @param needle Want to search sub-string
 * @return If found, return a pointer to the first occurrence of needle in haystack; otherwise, return NULL
 */
char* strstr(const char* haystack, const char* needle);

// ========== Other Functions ==========
/**
 * Get string length (null-terminated)
 * @param s Input string
 * @return Length of string
 */
size_t strlen(const char* s);

/**
 * Set a value to memory area
 * @param s Memory area
 * @param c Value to set (converted to unsigned char)
 * @param n Number of bytes to set
 * @return s
 */
void* memset(void* s, int c, size_t n);

/**
 * Calculate the length of the initial segment of the string, which consists entirely of the specified characters.
 * @param s1 Search string
 * @param s2 Character set string
 * @return long
 */
size_t strspn(const char* s1, const char* s2);

/**
 * Calculate the length of the initial segment of the string, which consists not of the specified characters.
 * @param s1 Search string
 * @param s2 Character set string
 * @return long
 */
size_t strcspn(const char* s1, const char* s2);

/**
* Split the string
* @param str The string to split (pass in the string on the first call, pass NULL on subsequent calls)
* @param delim The delimiter string
* @return A pointer to the next delimiter; returns NULL if no more delimiters are found
*/ 
char* strtok(char* restrict str, const char* restrict delim);

/**
* Find the error message string
* @param errnum The error number
* @return A pointer to the error message string
*/ 
char* strerror(int errnum);
#endif /* STRING_H */