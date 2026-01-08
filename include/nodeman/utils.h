#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

typedef enum {
    NDM_SUCCESS = 0,

    NDM_ERR_SUDO    = -1,  
    NDM_ERR_PATH    = -2,  

    NDM_ERR_VERSION = -10, 
    NDM_ERR_NETWORK = -11, 
    NDM_ERR_EXTRACT = -12, 

    NDM_ERR_UNKNOWN = -99
} NdmError;

void log_info(bool *verbose, const char *fmt, ...);

void log_warn(const char *fmt, ...);

void log_error(const char *format, ...);

int command(bool *verbose, const char *cmd, ...);

const char* get_arch();

#endif // UTILS_H