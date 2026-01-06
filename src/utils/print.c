#include <stdio.h>
#include <stdarg.h>
#include "utils.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\e[0;32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf(ANSI_COLOR_BLUE "[log]   " ANSI_COLOR_RESET);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

void warn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf(ANSI_COLOR_YELLOW "[warn]  " ANSI_COLOR_RESET);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, ANSI_COLOR_RED "[error] " ANSI_COLOR_RESET);
    vfprintf(stderr, fmt, args);

    if (errno != 0) {
        fprintf(stderr, ": %s (errno: %d)", strerror(errno), errno);
    }

    fprintf(stderr, "\n");
    va_end(args);
    
    errno = 0; 
}