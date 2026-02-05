#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

/**
 * Helper macro for quick debug logging.
 * Consistent with your verbose flag approach.
 */
#define LOG_DEBUG(v, fmt, ...) do { \
    if (v != NULL && *v) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__); \
} while (0)

/**
 * Custom error codes for internal nodeman operations.
 * Keeping your efficient negative-value mapping.
 */
typedef enum {
    NDM_SUCCESS     = 0,
    NDM_ERR_SUDO    = -1,  
    NDM_ERR_PATH    = -2,  
    NDM_ERR_VERSION = -10, 
    NDM_ERR_NETWORK = -11, 
    NDM_ERR_EXTRACT = -12, 
    NDM_ERR_UNKNOWN = -99
} NdmError;

/* --- Logging Functions --- */

// Prints to stdout if verbose is true
void log_info(bool verbose, const char *fmt, ...);

// Prints a warning message to stderr
void log_warn(const char *fmt, ...);

// Prints a critical error message to stderr
void log_error(const char *fmt, ...);

/* --- System & Command Functions --- */

// Executes a shell command using fork/execvp; returns exit status
int command(bool *verbose, const char *fmt, ...);

// Executes a command and captures its output string
int command_output(char *out_buf, size_t out_size, bool *verbose, const char *fmt, ...);

// Detects the system architecture (x64, arm64, etc.)
const char* get_arch();

/* --- File & Link Operations --- */

// Writes or appends formatted text to a specified file
int file_write(const char *filename, bool append, const char *fmt, ...);

// Forcefully creates a symlink (removes existing if necessary)
int symlink_force(bool *verbose, const char *target, const char *linkpath);

// Reads the first line of a file into a buffer (New in 2.3.0)
int open_file(char *buffer, size_t size, const char *path);

#endif // UTILS_H