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

/**
 * Executes a system command using fork and execvp.
 * If verbose is false, stdout/stderr are redirected to /dev/null.
 */
int command(bool *verbose, const char *fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (verbose != NULL && *verbose) {
        log_info(true, "Executing system command: %s", buffer);
    }

    char *exec_argv[64];
    int i = 0;
    
    // Simple tokenizer for command arguments
    char *token = strtok(buffer, " ");
    while (token != NULL && i < 63) {
        exec_argv[i++] = token;
        token = strtok(NULL, " ");
    }
    exec_argv[i] = NULL;

    if (i == 0) return 0; 

    pid_t pid = fork();
    if (pid < 0) {
        log_error("Process fork failed");
        return -1;
    }

    if (pid == 0) {
        // Quiet mode: discard output
        if (verbose != NULL && !(*verbose)) {
            int dev_null = open("/dev/null", O_WRONLY);
            if (dev_null != -1) {
                dup2(dev_null, STDOUT_FILENO);
                dup2(dev_null, STDERR_FILENO);
                close(dev_null);
            }
        }
        
        execvp(exec_argv[0], exec_argv);
        
        // This part only runs if execvp fails
        fprintf(stderr, "[ERROR] Command binary '%s' not found in PATH\n", exec_argv[0]);
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        
        int exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        if (exit_code != 0 && (verbose != NULL && *verbose)) {
            log_warn("Command exited with non-zero status: %d", exit_code);
        }
        
        return exit_code;
    }
}


int command_output(char *out_buf, size_t out_size, bool *verbose, const char *fmt, ...) {
    char cmd_buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(cmd_buffer, sizeof(cmd_buffer), fmt, args);
    va_end(args);

    bool is_verbose = (verbose != NULL && *verbose);
    log_info(is_verbose, "Executing (Direct Exec): %s", cmd_buffer);

    // --- 1. Parser Manual Tanpa Shell ---
    char *exec_argv[64];
    int i = 0;
    char *p = cmd_buffer;
    bool in_quotes = false;
    char quote_char = 0;

    while (*p && i < 63) {
        // Lewati spasi di luar tanda kutip
        while (*p == ' ' && !in_quotes) p++;
        if (*p == '\0') break;

        exec_argv[i++] = p; // Simpan awal argumen

        while (*p) {
            if ((*p == '"' || *p == '\'') && !in_quotes) {
                in_quotes = true;
                quote_char = *p;
                memmove(p, p + 1, strlen(p)); // Hapus tanda kutip pembuka
                continue; 
            } else if (in_quotes && *p == quote_char) {
                in_quotes = false;
                quote_char = 0;
                memmove(p, p + 1, strlen(p)); // Hapus tanda kutip penutup
                continue;
            } else if (*p == ' ' && !in_quotes) {
                *p = '\0'; // Pecah argumen di sini
                p++;
                break;
            }
            p++;
        }
    }
    exec_argv[i] = NULL;

    if (i == 0) return -1;

    // --- 2. Proses Pipe dan Fork ---
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        log_error("Pipe creation failed");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        log_error("Fork failed");
        close(pipefd[0]); close(pipefd[1]);
        return -1;
    }

    if (pid == 0) { // CHILD
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        
        if (!is_verbose) {
            int dev_null = open("/dev/null", O_WRONLY);
            if (dev_null != -1) {
                dup2(dev_null, STDERR_FILENO);
                close(dev_null);
            }
        } else {
            dup2(pipefd[1], STDERR_FILENO);
        }
        close(pipefd[1]);

        // Eksekusi langsung tanpa melalui /bin/sh
        execvp(exec_argv[0], exec_argv);
        _exit(EXIT_FAILURE);

    } else { // PARENT
        close(pipefd[1]);

        if (out_buf != NULL && out_size > 0) {
            memset(out_buf, 0, out_size);
        }

        ssize_t total_read = 0;
        char temp_char;
        while (read(pipefd[0], &temp_char, 1) > 0) {
            if (out_buf != NULL && (size_t)total_read < out_size - 1) {
                out_buf[total_read++] = temp_char;
            }
            if (is_verbose) {
                putchar(temp_char);
                fflush(stdout); 
            }
        }
        
        if (out_buf != NULL && out_size > 0) {
            out_buf[total_read] = '\0';
            // Hapus karakter newline di akhir
            out_buf[strcspn(out_buf, "\r\n")] = 0;
        }

        close(pipefd[0]);
        int status;
        waitpid(pid, &status, 0);
        
        int exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        if (exit_code != 0) {
            log_warn("Command '%s' failed with exit code %d", exec_argv[0], exit_code);
        }
        return exit_code;
    }
}
