// C99 Standard 7.5 Errors <errno.h> Implementation
// For E-comOS v0.1
// Defines the global errno variable and reserved for future error handling auxiliary functions
// Dependencies: <errno.h> for error code macros

#include <errno.h>

// -------------------------- Global Error Variable Definition (C99 Required) --------------------------
// Define the global errno variable (declared in errno.h)
// Initialized to 0 (no error) on system startup
int errno = 0;

// -------------------------- Future Error Handling Auxiliary Functions (Reserved) --------------------------
// Optional auxiliary functions for advanced error management (expand as needed for E-comOS v1.0+)
// Example: Clear the current error code (reset errno to 0)
// void clear_errno(void) {
//     errno = 0;
// }

// Example: Get a human-readable string description for an error code
// const char* strerror(int error_code) {
//     switch (error_code) {
//         case EOK: return "No error";
//         case EINVAL: return "Invalid argument";
//         case ENOMEM: return "Out of memory";
//         default: return "Unknown error";
//     }
// }