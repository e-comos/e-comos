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
// C99 Standard 7.23 Input/output <stdio.h> Implementation
// For E-comOS v0.1
// Core terminal I/O fully implemented (adapts to kernel's parameterized putchar function)
// Dependencies: Kernel printkit & standard headers

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

// -------------------------- Import Kernel Printkit (kernel-provided parameterized putchar) --------------------------
// Import kernel's putchar function with color parameter
#include <e-comos/printkit/print.h>

// -------------------------- Default Terminal Color (for kernel's parameterized putchar, C99 interface compliant) --------------------------
// Default terminal text color (E-comOS v0.1: white text on black background, standard VGA color)
// Modify this value to change the default terminal output color as needed
#define DEFAULT_TERMINAL_COLOR 0x07 // Standard VGA color: 0=black background, 7=white foreground

// -------------------------- Global Stream Instances (C99 Required) --------------------------
// Initialize standard streams for terminal I/O (v0.1: no buffer, terminal-only)
// stdin: keyboard input (fd=0), stdout/stderr: terminal output (fd=1/fd=2)
static FILE _stdin = {
    .fd = 0,
    .buf = NULL,
    .buf_size = BUFSIZ,
    .error = 0,
    .eof = 0
};

static FILE _stdout = {
    .fd = 1,
    .buf = NULL,
    .buf_size = BUFSIZ,
    .error = 0,
    .eof = 0
};

static FILE _stderr = {
    .fd = 2,
    .buf = NULL,
    .buf_size = BUFSIZ,
    .error = 0,
    .eof = 0
};

// Expose global stream pointers (C99 compliant, linked to static instances above)
FILE* stdin = &_stdin;
FILE* stdout = &_stdout;
FILE* stderr = &_stderr;

// -------------------------- User-Provided Keyboard Driver Prototype (to be implemented by user) --------------------------
// NOTE: Replace this with your actual kernel keyboard driver function to read a character
// Return: Read ASCII character on success, (char)EOF on error or no available input
char terminal_getchar(void);

// -------------------------- Core Implemented Functions (Terminal I/O, C99 Compliant) --------------------------
// C99 7.23.5: getchar() - Reads a single character from stdin (keyboard)
int getchar(void) {
    // Validate standard input stream
    if (stdin == NULL) {
        return EOF;
    }

    // Reset stream error flag before performing input operation (C99 compliant)
    stdin->error = 0;

    // Call user-provided keyboard driver to read a character
    char read_c = terminal_getchar();

    // Handle EOF/error condition
    if (read_c == (char)EOF) {
        stdin->eof = 1;       // Set EOF flag for the stream
        stdin->error = 1;     // Set error flag for the stream
        return EOF;
    }

    // Clear EOF flag on successful character read
    stdin->eof = 0;

    // Return the read character as int (C99 required behavior for character I/O)
    return (int)read_c;
}

// C99 7.23.5: puts() - Writes a null-terminated string to stdout, followed by a newline
int puts(const char* s) {
    // Validate input string and standard output stream
    if (s == NULL || stdout == NULL) {
        if (stdout != NULL) {
            stdout->error = 1; // Set error flag if stream is valid but string is NULL
        }
        return EOF;
    }

    // Reset stream error flag before performing output operation (C99 compliant)
    stdout->error = 0;

    // Traverse the null-terminated string and print each character (with default terminal color)
    const char* str_ptr = s;
    while (*str_ptr != '\0') {
        // Call kernel's putchar with default terminal color (adapt to kernel's parameterized putchar)
        kputchar(*str_ptr, DEFAULT_TERMINAL_COLOR);
        str_ptr++;
    }

    // Append mandatory newline character (C99 required behavior for puts())
    kputchar('\n', DEFAULT_TERMINAL_COLOR);

    // Return non-negative value to indicate success (C99 compliant; 0 = success for E-comOS v0.1)
    return 0;
}

// C99 7.23.5: putchar() - Writes a single character to stdout
int putchar(int c) {
    // Validate standard output stream
    if (stdout == NULL) {
        return EOF;
    }

    // Reset stream error flag before performing output operation (C99 compliant)
    stdout->error = 0;

    // Convert int argument to char (C99 allows non-ASCII values, truncate to 8-bit ASCII)
    char print_c = (char)c;

    // Call kernel's putchar with default terminal color (adapt to kernel's parameterized putchar)
    kputchar(print_c, DEFAULT_TERMINAL_COLOR);

    // Return the printed character as int (C99 required behavior for putchar())
    return c;
}


// -------------------------- Stub Functions (C99 Required, Future Extension) --------------------------
// NOTE: All stubs return valid C99 error values to avoid link errors.
// - `...` denotes "variable arguments" (variadic functions, C99 required for formatted I/O).
// - No need to process variable arguments in stubs; reserve for future implementation with <stdarg.h>.
// Replace with actual implementation when extending E-comOS (file system, formatted I/O, etc.)

// 7.23.4 Formatted I/O Stubs (variadic functions, C99 required)
int printf(const char* format, ...) { (void)format; return EOF; }
int fprintf(FILE* stream, const char* format, ...) { (void)stream; (void)format; return EOF; }
int sprintf(char* s, const char* format, ...) { (void)s; (void)format; return EOF; }
int snprintf(char* s, size_t n, const char* format, ...) { (void)s; (void)n; (void)format; return EOF; }
int scanf(const char* format, ...) { (void)format; return EOF; }
int fscanf(FILE* stream, const char* format, ...) { (void)stream; (void)format; return EOF; }
int sscanf(const char* s, const char* format, ...) { (void)s; (void)format; return EOF; }

// 7.23.5 Character I/O Stubs (additional)
int fgetc(FILE* stream) { (void)stream; return EOF; }
int getc(FILE* stream) { (void)stream; return EOF; }
int fputc(int c, FILE* stream) { (void)c; (void)stream; return EOF; }
int putc(int c, FILE* stream) { (void)c; (void)stream; return EOF; }
char* fgets(char* s, int n, FILE* stream) { (void)s; (void)n; (void)stream; return NULL; }

// 7.23.6 Direct I/O Stubs
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) { (void)ptr; (void)size; (void)nmemb; (void)stream; return 0; }
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) { (void)ptr; (void)size; (void)nmemb; (void)stream; return 0; }

// 7.23.7 File Positioning Stubs
int fseek(FILE* stream, long int offset, int whence) { (void)stream; (void)offset; (void)whence; return EOF; }
long int ftell(FILE* stream) { (void)stream; return -1; }
void rewind(FILE* stream) { (void)stream; }
int fgetpos(FILE* stream, fpos_t* pos) { (void)stream; (void)pos; return EOF; }
int fsetpos(FILE* stream, const fpos_t* pos) { (void)stream; (void)pos; return EOF; }

// 7.23.8 Error Handling Stubs
void clearerr(FILE* stream) { (void)stream; }
int feof(FILE* stream) { (void)stream; return 0; }
int ferror(FILE* stream) { (void)stream; return 0; }
void perror(const char* s) { (void)s; }

// 7.23.9 File Operation Stubs
FILE* fopen(const char* filename, const char* mode) { (void)filename; (void)mode; return NULL; }
int fclose(FILE* stream) { (void)stream; return EOF; }
int remove(const char* filename) { (void)filename; return EOF; }
int rename(const char* old, const char* new) { (void)old; (void)new; return EOF; }
FILE* tmpfile(void) { return NULL; }
char* tmpnam(char* s) { (void)s; return NULL; }