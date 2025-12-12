#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "utils/print.h" 
#include "install.h" 
#include "change.h"   
#include "list.h"   
#include "remove.h" 

// Prototypes 
int install(const char *version);
int change_version(const char *version);
int list_versions(const char *filter_version); 
int list_installed();
int remove_version(const char *version); 

// HELPER: cleaning start 'v' from version string if present
void clean_version_prefix(char *version) {
    if (version != NULL && version[0] == 'v') {
        // Shift the string to the left, effectively removing the 'v'
        memmove(version, version + 1, strlen(version));
    }
}

int main(int argc, char *argv[]) {
    print("nodeman v1.1.0\n"); 
    
    // Initial test...
    char system_cmd_output[64];
    
    // Loop through command line arguments
    for (int i = 1; i < argc; i++) {
        
        // Buffer for save copy and cleaning arguments
        char version_string_copy[64];
        
        // --- FEATURE: INSTALL (-i / --install) ---
        if (strcmp(argv[i], "install") == 0) {
            if (i + 1 < argc) {
                // Copy argument and clean it
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
                // Copy argument and clean it
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
                // Copy argument and clean it
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
