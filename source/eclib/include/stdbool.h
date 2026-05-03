// C99 Standard 7.16 Boolean type and values <stdbool.h>
// Defines boolean macros based on compiler-builtin _Bool keyword
// Compliant with C99 standard; no implementation file (.c) required (never needed)
#ifndef STDBOOL_H
#define STDBOOL_H
// -------------------------- Core Boolean Macros (C99 7.16 Required) --------------------------
// bool: macro for the built-in _Bool type (C99 keyword)
#define bool _Bool

// true: macro for the boolean true value (non-zero, C99 requires 1)
#define true 1

// false: macro for the boolean false value (zero, C99 requires 0)
#define false 0

// -------------------------- Optional: Boolean Type Indicator (C99 7.16 Optional) --------------------------
// __bool_true_false_are_defined: macro to indicate that bool/true/false are defined (C99 optional)
#define __bool_true_false_are_defined 1
#endif /* STDBOOL_H */
