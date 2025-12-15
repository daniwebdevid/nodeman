#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils/print.h"
#include "utils/arch.h" // Harus ada header arch.h yang mendeklarasikan kedua fungsi

// Fungsi asli: Untuk mendapatkan suffix yang kompatibel dengan URL Node.js
int get_arch_suffix(char *arch_suffix, size_t size) {
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

    // Map Raw Architecture to Node.js Suffix (x64, arm64, armv7l)
    if (strcmp(raw_arch, "x86_64") == 0 || strcmp(raw_arch, "amd64") == 0) {
        snprintf(arch_suffix, size, "x64"); // <-- Tetap x64 untuk URL Node.js
    } else if (strcmp(raw_arch, "aarch64") == 0 || strcmp(raw_arch, "arm64") == 0) {
        snprintf(arch_suffix, size, "arm64");
    } else if (strncmp(raw_arch, "arm", 3) == 0) {
        snprintf(arch_suffix, size, "armv7l"); 
    } else {
        snprintf(arch_suffix, size, "unsupported");
        char err_msg[128];
        int err_len = snprintf(err_msg, sizeof(err_msg), "Error: Architecture '%s' is not supported by Node.js.\n", raw_arch);
        write(2, err_msg, err_len); 
        return 1;
    }
    
    return 0;
}


// FUNGSI BARU: Khusus untuk arsitektur paket DEB (amd64, arm64, i386)
// Ini dipanggil oleh apt.c
int get_deb_arch_suffix(char *deb_suffix, size_t size) {
    char raw_arch[32];
    int result = 1;
    
    // Panggil fungsi asli untuk mendapatkan raw arch dari uname -m
    FILE *fp = popen("uname -m", "r");
    if (fp == NULL) {
        snprintf(deb_suffix, size, "unsupported");
        return 1;
    }
    if (fgets(raw_arch, sizeof(raw_arch), fp) != NULL) {
        // Hapus newline
        size_t read_len = strlen(raw_arch);
        if (read_len > 0 && raw_arch[read_len - 1] == '\n') {
            raw_arch[read_len - 1] = '\0';
        }
        
        // Map ke nama arsitektur DEBIAN resmi
        if (strcmp(raw_arch, "x86_64") == 0 || strcmp(raw_arch, "amd64") == 0) {
            snprintf(deb_suffix, size, "amd64"); // <-- FIX UTAMA
            result = 0;
        } else if (strcmp(raw_arch, "aarch64") == 0 || strcmp(raw_arch, "arm64") == 0) {
            snprintf(deb_suffix, size, "arm64");
            result = 0;
        } else if (strcmp(raw_arch, "i686") == 0 || strcmp(raw_arch, "i386") == 0) {
            snprintf(deb_suffix, size, "i386");
            result = 0;
        } else {
            snprintf(deb_suffix, size, "unsupported");
            char err_msg[128];
            int err_len = snprintf(err_msg, sizeof(err_msg), "Error: Debian architecture '%s' not recognized.\n", raw_arch);
            write(2, err_msg, err_len); 
        }
    }
    pclose(fp);
    return result;
}