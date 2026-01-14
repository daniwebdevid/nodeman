#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "nodeman/core.h"
#include "nodeman/utils.h"

/**
 * Uninstalls a specific Node.js version.
 * Removes the directory structure from the global installation path.
 */
int remove_node_js(bool *verbose, int argc, char *argv[]) {
    log_info(true, "Initializing removal for Node.js version: %s", argv[0]);

    // 0. Input Validation
    if (argv[0] == NULL || strchr(argv[0], '/') != NULL) {
        log_error("Invalid version name: '%s'. Path traversal is strictly prohibited.", 
                  argv[0] ? argv[0] : "NULL");
        errno = EINVAL;
        return 2;
    }

    // 1. Privilege Check
    if (getuid() != 0) {
        log_error("Root privileges (sudo) are required to remove from %s", NODE_INSTALL_DIR);
        errno = EACCES;
        return 2;
    }

    // 2. Parse Flags
    bool force_flags = false;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--force") == 0) {
            force_flags = true;
            break;
        }
    }

    // 3. Existence Check
    char version_path[PATH_MAX];
    snprintf(version_path, sizeof(version_path), "%s/%s", NODE_INSTALL_DIR, argv[0]);

    if (access(version_path, F_OK) != 0) {
        log_error("Uninstall failed: Version '%s' is not installed at %s", argv[0], version_path);
        errno = ENOENT;
        return 2;
    }

    // 4. Removal Execution
    // Kita pakai 'rm -rf' lewat helper command() lo karena rmdir() pasti gagal kalau folder ada isinya
    log_info(*verbose, "Removing directory: %s", version_path);
    
    char cmd_buf[1024];
    if (force_flags) {
        log_info(*verbose, "Force mode enabled. Bypassing additional checks...");
    }

    if (command(verbose, "rm -rf %s", version_path) != 0) {
        log_error("Failed to delete version directory. Check permissions or disk state.");
        return 1;
    }

    log_info(true, "Successfully uninstalled Node.js v%s", argv[0]);
    return 0;
}