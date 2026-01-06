#ifndef UTILS_H
#define UTILS_H

typedef enum {
    NDM_SUCCESS = 0,

    NDM_ERR_SUDO    = -1,  
    NDM_ERR_PATH    = -2,  

    NDM_ERR_VERSION = -10, 
    NDM_ERR_NETWORK = -11, 
    NDM_ERR_EXTRACT = -12, 

    NDM_ERR_UNKNOWN = -99
} NdmError;

void log_info(const char *fmt, ...);

void log_warn(const char *fmt, ...);

void log_error(const char* str, const int code);

#endif // UTILS_H