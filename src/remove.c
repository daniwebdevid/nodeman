#include "utils/print.h"
#include "remove.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 
#include <stdio.h>

int remove_version(const char *version) {
    print("Starting removal process...\n");
    
    // Secure buffer
    char target_path[256];        
    char check_active_cmd[300];   
    char check_exist_cmd[300];    
    char remove_cmd[300];         
    
    // The 'version' here does NOT contain 'v' (e.g., "22" or "22.9.0")

    // 1. Define the installation path
    // The installation directory path does NOT have 'v' (nodejs22)
    snprintf(target_path, sizeof(target_path),
             "/usr/local/lib/nodejs%s", version);

    // 2. Check if the version to be removed is currently active
    // Command: readlink -f /usr/local/bin/node | grep 'nodejs<version>'
    snprintf(check_active_cmd, sizeof(check_active_cmd),
             "readlink -f /usr/local/bin/node | grep 'nodejs%s'", version);

    // If grep succeeds (exit code 0), this version is active.
    if (system(check_active_cmd) == 0) {
        char error_msg[128];
        // FIX: Remove the hardcoded 'v'
        snprintf(error_msg, sizeof(error_msg), 
                 "Error: Cannot remove active version %s. Change version first.\n", version);
        write(2, error_msg, strlen(error_msg));
        return 1;
    }
    
    // 3. Confirm that the path actually exists
    snprintf(check_exist_cmd, sizeof(check_exist_cmd), "ls %s", target_path);

    if (system(check_exist_cmd) != 0) {
        char not_found_msg[512]; 
        // FIX: Remove the hardcoded 'v'
        snprintf(not_found_msg, sizeof(not_found_msg), 
                 "Error: Version %s is not installed at %s.\n", version, target_path);
        write(2, not_found_msg, strlen(not_found_msg));
        return 1;
    }

    // 4. Execute the removal command (rm -rf)
    snprintf(remove_cmd, sizeof(remove_cmd),
             "rm -rf %s", target_path);

    print("Executing: ");
    print(remove_cmd);
    print("\n");
    
    int result = system(remove_cmd);
    
    if (result == 0) {
        char success_msg[128];
        // FIX: Remove the hardcoded 'v'
        snprintf(success_msg, sizeof(success_msg),
                 "Successfully removed version %s.\n", version);
        print(success_msg);
        return 0;
    } else {
        write(2, "Error: Removal failed (Permission denied or system error).\n", 59);
        return 1;
    }
}
