#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>

/**
 * Helper macro for quick debug logging.
 */
#define LOG_DEBUG(v, fmt, ...) do { \
    if (v != NULL && *v) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__); \
} while (0)

/**
 * Custom error codes for internal nodeman operations.
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

// Executes a shell command; returns exit status
int command(bool *verbose, const char *fmt, ...);

// Executes a command and captures the first line of output as an integer
int command_output(bool *verbose, const char *fmt, ...);

// Detects the system architecture (x64, arm64, etc.)
const char* get_arch();

/* --- File Operations --- */

// Writes or appends formatted text to a specified file
int file_write(const char *filename, bool append, const char *fmt, ...);

#endif // UTILS_H