#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "nodeman/utils.h"

/**
 * Writes formatted text to a file.
 * Supports both overwrite (w) and append (a) modes.
 */
int file_write(const char *filename, bool append, const char *fmt, ...) {
    const char *mode = append ? "a" : "w";
    
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        log_error("Failed to open file for writing: %s", filename);
        return 1;
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
    va_end(args);

    fclose(file);
    return 0;
}