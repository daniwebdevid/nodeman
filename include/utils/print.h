#ifndef PRINT_H
#define PRINT_H

#include <stddef.h> 

// Custom snprintf function implementation
int snprintf(char *str, size_t size, const char *format, ...);

// Custom print function (uses write(1, ...))
int print(const char *text);

#endif // PRINT_H