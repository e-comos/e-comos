// C99 Standard 7.5 Errors <errno.h>
// Defines error codes and the errno global variable for error reporting
// Compliant with C99 standard; requires errno.c for errno variable definition
#ifndef ERRNO_H
#define ERRNO_H
// -------------------------- Global Error Variable Declaration (C99 Required) --------------------------
// errno: global variable to store the last error code from system calls or library functions
// - 0 indicates no error; non-zero indicates a specific error (defined below)
// - Modified only when a function fails; not reset to 0 on successful function calls
extern int errno;

// -------------------------- C99 Standard Error Codes (Required, Minimal Set) --------------------------
// Basic standard error codes for E-comOS v0.1 (expand as needed for full C99 compliance)
#define EOK        0   // No error (success, non-standard but convenient for E-comOS)
#define EINVAL     1   // Invalid argument (invalid parameter passed to function)
#define ENOMEM     2   // Out of memory (insufficient memory to complete operation)
#define ENOENT     3   // No such file or directory (requested resource does not exist)
#define EIO        4   // Input/output error (failed to read/write from/to device/resource)
#define EBADF      5   // Bad file descriptor (invalid or closed file descriptor used)
#define EAGAIN     6   // Resource temporarily unavailable (try again later)
#define EACCES     7   // Permission denied (insufficient privileges to access resource)

// -------------------------- E-comOS Extended Error Codes (Custom, for Kernel/Drivers) --------------------------
// Additional error codes tailored for E-comOS kernel and device drivers (expand as needed)
#define EIPC       100 // IPC communication failure (inter-process communication error)
#define EDEVICE    101 // Device error (hardware device malfunction or unavailable)
#define EPROTO     102 // Protocol error (invalid protocol sequence or response)
#define EOVERFLOW  103 // Overflow error (numeric value too large for target type)
#define EUNIMP     104 // Unimplemented function (function not yet implemented in E-comOS)
#endif /* ERRNO_H */
