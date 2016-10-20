#ifndef PRINTF_H_
#define PRINTF_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "comm.h"

// Begin C++ extern to C
#ifdef __cplusplus
extern "C" {
#endif

// FILE is an integer referring to a stream; we use (invalid) pointers for compability
typedef int FILE;
// Standard input and output streams use the PC debug terminal
#define stdout ((FILE *)1)
#define stdin ((FILE *)1)

// Prints the simple string to debug terminal
void print(const char *string);
// Same as "print" function, with trailing newline
int puts(const char *string);
// Prints the simple string to the specified stream
void fprint(const char *string, FILE *stream);
// Same as "fprint" function, with trailing newline
int fputs(const char *string, FILE *stream);

// Prints the formatted string to the debug stream (the PC terminal)
int printf(const char *formatString, ...);
// Prints the formatted string to the specified output stream
int fprintf(FILE *stream, const char *formatString, ...);
// Prints the formatted string to the string buffer, which must be big enough
int sprintf(char *buffer, const char *formatString, ...);
// Prints the formatted string to the string buffer with the specified length limit
// The length limit, as per the C standard, includes the trailing null character
int snprintf(char *buffer, size_t limit, const char *formatString, ...);

// Variadic macro variations of the above, which are only useful in a handful of cases
// Prints the formatted string to the specified output stream
int vfprintf(FILE *stream, const char *formatString, va_list arguments);
// Prints the formatted string to a string buffer
int vsprintf(char *buffer, const char *formatString, va_list arguments);
// Prints the formatted string to a string buffer with the given length limit
int vsnprintf(char *buffer, size_t limit, const char *formatString, va_list arguments);

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
