/**
 * ECLib - E-comOS C Library
 * Copyright (C) 2026 Saladin5101
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Softwar
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

// Dependencies: C99 standard basic types
#include <stddef.h>
#include <stdint.h>
#ifndef STDIO_H
#define STDIO_H
// -------------------------- C99 Required Macros --------------------------
// 7.23.1 Macro definitions
// End of File/Input marker (negative integer, not matching any valid character)
#define EOF (-1)

// Buffer size for standard I/O operations (C99 required, default to 512 for embedded)
#define BUFSIZ 512

// Maximum length of a file name (C99 required, default to 256 for E-comOS)
#define FILENAME_MAX 256

// Maximum number of simultaneously open files (C99 required, default to 8 for early-stage)
#define FOPEN_MAX 8

// File positioning constants (7.23.9.2 fseek function)
#define SEEK_SET 0  // Set position to offset from the start of the file
#define SEEK_CUR 1  // Set position to current position plus offset
#define SEEK_END 2  // Set position to end of file plus offset

// -------------------------- C99 Required Types --------------------------
// 7.23.2 Types
// File stream object (C99 required, encapsulates stream state for terminal/file I/O)
// For E-comOS v0.1: only supports terminal stdin/stdout/stderr
typedef struct FILE {
    int fd;                 // File descriptor (0=stdin, 1=stdout, 2=stderr for v0.1)
    char* buf;              // I/O buffer (unused for v0.1, reserved for future caching)
    size_t buf_size;        // Buffer size (matches BUFSIZ for C99 compliance)
    int error;              // Stream error flag (0=no error, non-zero=error occurred)
    int eof;                // Stream EOF flag (0=no EOF, non-zero=EOF reached)
} FILE;

// File position type (C99 required, for file positioning operations)
// For E-comOS v0.1: simplified to 32-bit integer (enough for terminal/early file I/O)
typedef uint32_t fpos_t;

// -------------------------- Forward Declarations --------------------------
// E-comOS terminal service helper functions (backing implementation for stdio)
// Prints a single ASCII character to the terminal screen
void term_print_char(char c);
// Reads a single ASCII character from the keyboard input
char term_read_char(void);

// -------------------------- C99 Required Global Streams --------------------------
// 7.23.3 Standard streams (C99 requires stdin/stdout/stderr to be declared)
extern FILE* stdin;    // Standard input (keyboard, fd=0)
extern FILE* stdout;   // Standard output (terminal, fd=1)
extern FILE* stderr;   // Standard error (terminal, fd=2)

// -------------------------- C99 Required Function Prototypes (Full Set) --------------------------
// 7.23.4 Formatted input/output functions (stubs for v0.1, core implemented later)
int printf(const char* format, ...);
int fprintf(FILE* stream, const char* format, ...);
int sprintf(char* s, const char* format, ...);
int snprintf(char* s, size_t n, const char* format, ...);
int scanf(const char* format, ...);
int fscanf(FILE* stream, const char* format, ...);
int sscanf(const char* s, const char* format, ...);

// 7.23.5 Character input/output functions
int fgetc(FILE* stream);
int getc(FILE* stream);
int getchar(void);
int fputc(int c, FILE* stream);
int putc(int c, FILE* stream);
int fflush(FILE *stream);
char* fgets(char* s, int n, FILE* stream);
int puts(const char* s);

// 7.23.6 Direct input/output functions
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);

// 7.23.7 File positioning functions
int fseek(FILE* stream, long int offset, int whence);
long int ftell(FILE* stream);
void rewind(FILE* stream);
int fgetpos(FILE* stream, fpos_t* pos);
int fsetpos(FILE* stream, const fpos_t* pos);

// 7.23.8 Error handling functions
void clearerr(FILE* stream);
int feof(FILE* stream);
int ferror(FILE* stream);
void perror(const char* s);

// 7.23.9 File operations
FILE* fopen(const char* filename, const char* mode);
int fclose(FILE* stream);
int remove(const char* filename);
int rename(const char* old, const char* new);
FILE* tmpfile(void);
char* tmpnam(char* s);

#endif /* STDIO_H */
