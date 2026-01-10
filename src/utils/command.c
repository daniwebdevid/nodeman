#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "nodeman/utils.h"

int command(bool *verbose, const char *fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (verbose != NULL && *verbose) {
        printf("[DEBUG] Executing: %s\n", buffer);
    }

    char *exec_argv[64];
    int i = 0;
    
    char *token = strtok(buffer, " ");
    while (token != NULL && i < 63) {
        exec_argv[i++] = token;
        token = strtok(NULL, " ");
    }
    exec_argv[i] = NULL;

    if (i == 0) return 0; 

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        if (verbose != NULL && !(*verbose)) {
            int dev_null = open("/dev/null", O_WRONLY);
            if (dev_null != -1) {
                dup2(dev_null, STDOUT_FILENO);
                dup2(dev_null, STDERR_FILENO);
                close(dev_null);
            }
        }
        
        execvp(exec_argv[0], exec_argv);
        
        fprintf(stderr, "[ERROR] Command '%s' not found\n", exec_argv[0]);
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        
        int exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        if (exit_code != 0 && (verbose != NULL && *verbose)) {
            printf("[DEBUG] Command exited with code: %d\n", exit_code);
        }
        
        return exit_code;
    }
}

int command_output(bool *verbose, const char *fmt, ...) {
    char command_buf[1024];
    char result_buf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(command_buf, sizeof(command_buf), fmt, args);
    va_end(args);

    if (verbose != NULL && *verbose) {
        printf("[DEBUG] Fetching output from: %s\n", command_buf);
    }

    FILE *fp = popen(command_buf, "r");
    if (fp == NULL) {
        return -1;
    }

    if (fgets(result_buf, sizeof(result_buf), fp) == NULL) {
        pclose(fp);
        return -1;
    }

    int status = pclose(fp);
    if (status == -1) return -1;

    return atoi(result_buf);
}