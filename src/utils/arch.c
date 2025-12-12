#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils/print.h"

static int get_arch_suffix(char *arch_suffix, size_t size) {
    FILE *fp;
    char raw_arch[32];
    int read_len;

    // Execute uname -m
    fp = popen("uname -m", "r");
    if (fp == NULL) {
        snprintf(arch_suffix, size, "unsupported");
        return 1;
    }

    // Read the output
    if (fgets(raw_arch, sizeof(raw_arch), fp) == NULL) {
        pclose(fp);
        snprintf(arch_suffix, size, "unsupported");
        return 1;
    }

    pclose(fp);

    // Remove newline
    read_len = strlen(raw_arch);
    if (read_len > 0 && raw_arch[read_len - 1] == '\n') {
        raw_arch[read_len - 1] = '\0';
    }

    // Map Raw Architecture to Node.js Suffix
    if (strcmp(raw_arch, "x86_64") == 0 || strcmp(raw_arch, "amd64") == 0) {
        snprintf(arch_suffix, size, "x64");
    } else if (strcmp(raw_arch, "aarch64") == 0 || strcmp(raw_arch, "arm64") == 0) {
        snprintf(arch_suffix, size, "arm64");
    } else if (strncmp(raw_arch, "arm", 3) == 0) {
        // Fallback for 32-bit ARM
        snprintf(arch_suffix, size, "armv7l"); 
    } else {
        // Unknown architecture - Use snprintf buffer and write(2) for error
        snprintf(arch_suffix, size, "unsupported");
        char err_msg[128];
        int err_len = snprintf(err_msg, sizeof(err_msg), "Error: Architecture '%s' is not supported by Node.js.\n", raw_arch);
        write(2, err_msg, err_len); 
        return 1;
    }
    
    return 0;
}
