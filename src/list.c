#include "list.h"
#include "utils/print.h"
#include <stdio.h>    
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   
#include <sys/wait.h> 

// Helper function to run command and get exit code
static int execute_command_real(const char *cmd_name, const char *cmd_str) {
    int result = system(cmd_str);
    return (WEXITSTATUS(result) == 0) ? 0 : 1; 
}

// ------------------------------------------------------------------
// Function to list installed versions (Unchanged)
// ------------------------------------------------------------------

int list_installed() {
    print("Listing installed Node.js versions from /usr/local/lib/...\n");

    const char *cmd = "ls -1 /usr/local/lib/ 2>/dev/null | grep '^nodejs' | sed 's/^nodejs//'";
    
    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        write(2, "Error: Failed to run installation list command.\n", 48);
        return 1;
    }

    char buffer[128];
    print("\n--- Installed Node.js Versions ---\n");
    
    const char *current_version_cmd = "readlink -f /usr/local/bin/node | sed -E 's/.*nodejs([0-9.]+).*/\\1/'";
    char current_version[32] = "";
    FILE *fp_current = popen(current_version_cmd, "r");
    
    if (fp_current != NULL && fgets(current_version, sizeof(current_version), fp_current) != NULL) {
        current_version[strcspn(current_version, "\n")] = 0; 
        pclose(fp_current);
    } else if (fp_current != NULL) {
        pclose(fp_current);
    }
    

    int count = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; 

        if (strcmp(buffer, current_version) == 0) {
            char active_line[160];
            snprintf(active_line, sizeof(active_line), "  * v%s (ACTIVE)\n", buffer);
            print(active_line);
        } else {
            char normal_line[160];
            snprintf(normal_line, sizeof(normal_line), "  - v%s\n", buffer);
            print(normal_line);
        }
        
        count++;
    }
    
    pclose(fp);
    
    char final_msg[64];
    snprintf(final_msg, sizeof(final_msg), "\n--- End of List (%d versions found) ---\n", count);
    print(final_msg);
    
    return 0;
}


// ------------------------------------------------------------------
// Function to list available versions (NOW WITH DYNAMIC FILTERING)
// ------------------------------------------------------------------

int list_versions(const char *filter_version) {
    print("Fetching available Node.js versions (This may take a moment)...\n");
    
    // Command to get the list of ALL versions from nodejs.org
    const char *cmd = "curl -s https://nodejs.org/dist/index.json | tr ',' '\\n' | grep 'version' | awk -F '\"' '{print $4}'";
    
    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        write(2, "Error: Failed to run version listing command (curl, tr, grep, awk needed).\n", 75);
        return 1;
    }

    char buffer[128];
    char filtered_line[160];
    print("\n--- Available Node.js Versions ---\n");
    print("Latest versions are listed first. (Format: vX.Y.Z)\n\n");
    
    int filter_active = (filter_version != NULL && strlen(filter_version) > 0);
    int count = 0;
    
    // Read the output of the shell command line by line
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0; 
        
        // --- DYNAMIC FILTER LOGIC ---
        if (filter_active) {
            // Check if the current version string contains the filter string
            if (strstr(buffer, filter_version) == NULL) {
                continue; // Skip if it doesn't match
            }
        }
        
        // --- PRINT THE VERSION ---
        snprintf(filtered_line, sizeof(filtered_line), "- %s\n", buffer);
        print(filtered_line);
        
        count++;
    }
    
    pclose(fp);
    
    char final_msg[64];
    snprintf(final_msg, sizeof(final_msg), "\n--- End of List (%d versions found) ---\n", count);
    print(final_msg);
    
    return 0;
}