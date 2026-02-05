#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "nodeman/utils.h"

/**
 * Writes formatted text to a file.
 * Production-ready handling for config updates.
 */
int file_write(const char *filename, bool append, const char *fmt, ...) {
    // 0. Mode Selection
    const char *mode = append ? "a" : "w";
    
    // 1. Stream Initialization
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        log_error("Failed to open file for writing: %s", filename);
        return 1;
    }

    // 2. Variadic Formatting
    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
    va_end(args);

    fclose(file);
    return 0;
}

/**
 * Reads the first line of a file into a buffer.
 * Commonly used to fetch the 'active' version state.
 */
int open_file(char *buffer, size_t size, const char *path) {
    // 1. Open for Reading
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return 0;
    }

    // 2. Buffer Capture & Cleanup
    if (fgets(buffer, size, f)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        fclose(f);
        return 1;
    }

    fclose(f);
    return 0;
}