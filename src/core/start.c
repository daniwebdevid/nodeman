#include <linux/limits.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#include "nodeman/core.h"
#include "nodeman/utils.h"

/**
 * Initializes environment v2.5.0
 * Added: Strict buffer validation and improved root-level escape.
 */
int start(bool *verbose) {
    char current_dir[PATH_MAX];
    char config_path[PATH_MAX];
    char version_buffer[64];

    // 1. Context Acquisition
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        log_error("CRITICAL: Unable to determine current working directory");
        return 1;
    }

    // 2. Directory Tree Traversal
    while (1) {
        snprintf(config_path, sizeof(config_path), "%s/.ndmrc", current_dir);

        if (access(config_path, F_OK) == 0) {
            log_info(*verbose, "Auto-config found: %s", config_path);
            
            memset(version_buffer, 0, sizeof(version_buffer));
            if (open_file(version_buffer, sizeof(version_buffer), config_path)) {
                
                // 3. Integrity Check
                if (strlen(version_buffer) == 0) {
                    log_warn("Empty .ndmrc detected at %s. Skipping...", current_dir);
                } else {
                    char *args[] = { version_buffer, NULL }; 
                    return use(verbose, 1, args);
                }
            }
        }

        // 4. Upward Navigation
        char *last_slash = strrchr(current_dir, '/');
        
        if (last_slash == NULL || strlen(current_dir) <= 1) {
            break;
        }

        if (last_slash == current_dir) {
            current_dir[1] = '\0'; 
        } else {
            *last_slash = '\0';
        }
    }

    log_info(*verbose, "Lifecycle: No project-specific .ndmrc found.");
    return 1;
}