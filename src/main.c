#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "print/print.h" 
#include "install/install.h" 
#include "change/change.h"   

// Prototypes for functions defined in other files
int install(const char *version);
int change_version(const char *version);

int main(int argc, char *argv[]) {
    print("Hello, World! (nodeman v1.0.0)\n");
    
    // Initial test of custom snprintf and standard system()
    char system_cmd_output[64];
    snprintf(system_cmd_output, sizeof(system_cmd_output), "echo 'Hello from system call!'");
    system(system_cmd_output);
    
    // Loop through command line arguments
    for (int i = 1; i < argc; i++) {
        
        // --- FEATURE: INSTALL (-i / --install) ---
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--install") == 0) {
            
            if (i + 1 < argc) {
                const char *version_str = argv[i + 1];
                install(version_str);
                
                i++; // Skip the value argument
                continue; 
            } else {
                // Error: -i without value
                char error_msg[128];
                snprintf(error_msg, sizeof(error_msg), "Error: flag '%s' requires a version value (e.g., -i 22).\n", argv[i]);
                print(error_msg);
                return 1; 
            }
        }
        
        // --- FEATURE: CHANGE (-c / --change) ---
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--change") == 0) {
            
            if (i + 1 < argc) {
                const char *version_str = argv[i + 1];
                change_version(version_str);
                
                i++; // Skip the value argument
                continue; 
            } else {
                // Error: -c without value
                char error_msg[128];
                snprintf(error_msg, sizeof(error_msg), "Error: flag '%s' requires a target version (e.g., -c 22).\n", argv[i]);
                print(error_msg);
                return 1; 
            }
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