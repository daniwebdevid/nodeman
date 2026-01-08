#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h> 
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
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

    pid_t pid = fork();
    if (pid < 0) return -1;

    if (pid == 0) {
        if (verbose != NULL && !(*verbose)) {
            int dev_null = open("/dev/null", O_WRONLY);
            dup2(dev_null, STDOUT_FILENO);
            dup2(dev_null, STDERR_FILENO);
            close(dev_null);
        }
        
        execvp(exec_argv[0], exec_argv);
        exit(errno);
    } else {
        int status;
        waitpid(pid, &status, 0);
        printf("\n"); 
        fflush(stdout);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}