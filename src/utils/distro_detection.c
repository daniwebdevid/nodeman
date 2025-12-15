#include <stdio.h>
#include <unistd.h>
#include "utils/print.h"
#include "utils/command.h"
#include <string.h>
#include "utils/distro_detection.h"
#define OS_RELEASE_PATH "/etc/os-release"

int install_build_dependencies(DetectedDistro os) {
    if (os == OS_DEBIAN) {
        print("Installing build dependencies (apt-get install -y aria2)...\n");
        
        // Memanggil command() untuk menjalankan instalasi APT
        // Kita menggunakan aria2 (seperti di install.c Anda)
        if (command("COMMAND: INSTALL DEPS (APT)", "apt-get install -y aria2") != 0) {
            write(2, "Error: Failed to install 'aria2' via APT. Install manually and try again.\n", 74);
            return 1;
        }
        print("Dependencies installed successfully.\n");

    } else {
        // Fallback untuk non-Debian (Pacman, RPM, Unknown)
        print("Skipping automatic dependency installation for non-Debian system.\n");
    }
    return 0; 
}

// function to detect OS type
DetectedDistro get_os_type() {
    FILE *fp;
    char buffer[128];
    DetectedDistro result = OS_UNKNOWN; // Default

    // 1. try to open /etc/os-release
    fp = fopen(OS_RELEASE_PATH, "r");
    if (fp == NULL) {
        // if not found, return UNKNOWN
        return OS_UNKNOWN; 
    }

    // 2. read file line by line
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Check for ID line
        if (strncmp(buffer, "ID=", 3) == 0) {
            
        
            // Detect based on ID value
            // ID="debian"
            char *id_start = buffer + 3; // skip "ID="
            // delete leading quote if present
            if (id_start[0] == '"') id_start++;
            
            if (strstr(id_start, "debian") || strstr(id_start, "ubuntu")) {
                result = OS_DEBIAN;
            } else if (strstr(id_start, "arch")) {
                result = OS_ARCH;
            } else if (strstr(id_start, "centos") || strstr(id_start, "fedora")) {
                result = OS_REDHAT;
            }
            
            // Kita sudah dapat ID, bisa keluar dari loop
            break; 
        }
    }

    fclose(fp);
    return result;
}