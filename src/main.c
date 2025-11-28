#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "print/print.h" 
#include "install/install.h" 
#include "change/change.h"   
#include "list/list.h"   
#include "remove/remove.h" 

// Prototypes 
int install(const char *version);
int change_version(const char *version);
int list_versions(const char *filter_version); 
int list_installed();
int remove_version(const char *version); 

// HELPER: Membersihkan awalan 'v' dari string versi jika ada
void clean_version_prefix(char *version) {
    if (version != NULL && version[0] == 'v') {
        // Geser string ke kiri, efektif menghapus 'v'
        memmove(version, version + 1, strlen(version));
    }
}

int main(int argc, char *argv[]) {
    print("Hello, World! (nodeman v1.1.0)\n"); 
    
    // Initial test...
    char system_cmd_output[64];
    snprintf(system_cmd_output, sizeof(system_cmd_output), "echo 'Hello from system call!'");
    system(system_cmd_output);
    
    // Loop through command line arguments
    for (int i = 1; i < argc; i++) {
        
        // Buffer untuk menyimpan salinan dan membersihkan argumen
        char version_string_copy[64];
        
        // --- FEATURE: INSTALL (-i / --install) ---
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--install") == 0) {
            if (i + 1 < argc) {
                // Salin argumen dan bersihkan
                strncpy(version_string_copy, argv[i+1], sizeof(version_string_copy) - 1);
                version_string_copy[sizeof(version_string_copy) - 1] = '\0';
                clean_version_prefix(version_string_copy); 
                
                install(version_string_copy); 
                i++; 
            } else {
                char error_msg[128];
                snprintf(error_msg, sizeof(error_msg), "Error: flag '%s' requires a version value.\n", argv[i]);
                print(error_msg);
                return 1; 
            }
        }
        
        // --- FEATURE: CHANGE (-c / --change) ---
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--change") == 0) {
            if (i + 1 < argc) {
                // Salin argumen dan bersihkan
                strncpy(version_string_copy, argv[i+1], sizeof(version_string_copy) - 1);
                version_string_copy[sizeof(version_string_copy) - 1] = '\0';
                clean_version_prefix(version_string_copy); 
                
                change_version(version_string_copy);
                i++;
            } else {
                char error_msg[128];
                snprintf(error_msg, sizeof(error_msg), "Error: flag '%s' requires a version value.\n", argv[i]);
                print(error_msg);
                return 1;
            }
        }
        
        // --- FEATURE: REMOVE (-r / --remove) ---
        else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--remove") == 0) {
            if (i + 1 < argc) {
                // Salin argumen dan bersihkan
                strncpy(version_string_copy, argv[i+1], sizeof(version_string_copy) - 1);
                version_string_copy[sizeof(version_string_copy) - 1] = '\0';
                clean_version_prefix(version_string_copy); 
                
                remove_version(version_string_copy);
                i++;
            } else {
                char error_msg[128];
                snprintf(error_msg, sizeof(error_msg), "Error: flag '%s' requires a version value.\n", argv[i]);
                print(error_msg);
                return 1;
            }
        }
        
        // --- FEATURE: LIST AVAILABLE (-l / --list) ---
        else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0) {
            const char *filter = NULL; 
            
            if (i + 1 < argc && argv[i+1][0] != '-') {
                filter = argv[i + 1];
                i++; 
            }
            
            list_versions(filter); 
            continue;
        }
        
        // --- FEATURE: LIST INSTALLED (-iL / --installed-list) ---
        else if (strcmp(argv[i], "-iL") == 0 || strcmp(argv[i], "--installed-list") == 0) {
            list_installed();
            continue;
        }
        
        // --- UNKNOWN FLAG ---
        else {
            char error_msg[128];
            snprintf(error_msg, sizeof(error_msg), "Error: Unknown flag '%s'.\n", argv[i]);
            print(error_msg);
        }
    }
    
    return 0;
}