#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "nodeman/utils.h"

#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\e[0;32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BOLD_RED      "\033[1;31m"

__attribute__((format(printf, 2, 3)))
void log_info(bool verbose, const char *fmt, ...) {
    if (verbose) {
        va_list args;
        va_start(args, fmt);
        printf(ANSI_COLOR_BLUE "[info]  " ANSI_COLOR_RESET);
        vprintf(fmt, args);
        printf("\n");
        va_end(args);
    }
}

__attribute__((format(printf, 1, 2)))
void warn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, ANSI_COLOR_YELLOW "[warn]  " ANSI_COLOR_RESET);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

__attribute__((format(printf, 1, 2)))
void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, ANSI_BOLD_RED "[error] " ANSI_COLOR_RESET);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    fflush(stderr);
}