#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "nodeman/core.h"
#include "nodeman/utils.h"

/**
 * Entry point for switching Node versions.
 * Dispatches to either system-wide (default) or user-specific installation.
 */
int use(bool *verbose, int argc, char *argv[]) {
    bool default_flags = false;
    
    // 0. Argument Parsing
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--default") == 0) {
            default_flags = true;
            break;
        }
    }

    // 1. Input Validation (Security & Path Traversal Check)
    if (argv[0] == NULL || strchr(argv[0], '/') != NULL) {
        log_error("Invalid version name: '%s'. Path traversal is not allowed.", 
                  argv[0] ? argv[0] : "NULL");
        errno = EINVAL; 
        return 2;
    }

    // 2. Dispatch Logic
    if (default_flags) {
        return use_default(verbose, argv);
    } else {
        return use_user(verbose, argv);
    }
}

/**
 * Handles system-wide Node.js version switching.
 * Requires root privileges.
 */
int use_default(bool *verbose, char *argv[]) {
    log_info(true, "Initializing global switch to Node.js v%s", argv[0]);

    // 1. Privilege Check
    if (getuid() != 0) {
        log_error("Root privileges required for global configuration (current UID: %d)", getuid());
        errno = EACCES;
        return 2;
    }

    // 2. Source Existence Check
    char path[512];
    snprintf(path, sizeof(path), "%s/%s/source", NODE_INSTALL_DIR, argv[0]);
    if (access(path, F_OK) != 0) {
        log_error("Installation not found: version '%s' is not in %s", argv[0], NODE_INSTALL_DIR);
        errno = ENOENT;
        return 2;
    }

    // 3. Global Synchronization
    log_info(*verbose, "Synchronizing version '%s' to global default...", argv[0]);
    
    if (command(verbose, "cp -r %s/%s/source %s/default/", NODE_INSTALL_DIR, argv[0], NODE_INSTALL_DIR) != 0) {
        log_error("Failed to copy source to default directory");
        return 1;
    }

    if (command(verbose, "ln -snf %s/default/source/bin %s/default/bin", NODE_INSTALL_DIR, NODE_INSTALL_DIR) != 0) {
        log_error("Failed to update global symlinks");
        return 1;
    }
    
    log_info(true, "Successfully set v%s as the global default", argv[0]);
    return 0;
}

/**
 * Handles user-specific Node.js version switching.
 * Creates symlinks within the user's home directory.
 */
int use_user(bool *verbose, char *argv[]) {
    char *home = getenv("HOME");

    // 1. Environment Check
    if (!home) {
        log_error("Environment error: $HOME is not set");
        errno = ENOENT;
        return 1;
    }

    if (getuid() == 0) {
        log_error("Running 'use' as root is not recommended; use --default for global setup");
        errno = EPERM;
        return 2;
    }

    // 2. Source Path Validation
    char pathNodejs[512];
    snprintf(pathNodejs, sizeof(pathNodejs), "%s/%s/source", NODE_INSTALL_DIR, argv[0]);
    if (access(pathNodejs, F_OK) != 0) {
        log_error("Node.js version '%s' is not installed in %s", argv[0], NODE_INSTALL_DIR);
        errno = ENOENT;
        return 2;
    }

    // 3. Local Directory Preparation
    log_info(*verbose, "Preparing local directory structure in %s/.ndm", home);
    if (command(verbose, "mkdir -p %s/.ndm/%s/bin", home, argv[0]) != 0) {
        log_error("Failed to create local directory structure");
        return 1;
    }

    // 4. Link Node.js Binaries
    const char *binaries[] = {"node", "npm", "npx", "corepack"};
    for (int i = 0; i < 4; i++) {
        log_info(*verbose, "Linking binary [%s] for version %s", binaries[i], argv[0]);
        if (command(verbose, "ln -snf %s/%s/bin/%s %s/.ndm/%s/bin/%s", 
                    NODE_INSTALL_DIR, argv[0], binaries[i], home, argv[0], binaries[i]) != 0) {
            log_error("Failed to link binary: %s", binaries[i]);
            return 1;
        }
    }

    // 5. Update Active Symlink (Atomic Switch)
    log_info(*verbose, "Updating active environment symlink...");
    if (command(verbose, "ln -snf %s/.ndm/%s/bin %s/.ndm/bin", home, argv[0], home) != 0) {
        log_error("Failed to update active symlink at %s/.ndm/bin", home);
        return 1;
    }

    // 6. Update Configuration Files
    char path_buf[1024];
    log_info(*verbose, "Updating .npmrc and active version file");

    snprintf(path_buf, sizeof(path_buf), "%s/.npmrc", home);
    file_write(path_buf, false, "prefix=%s/.ndm/%s/", home, argv[0]);

    snprintf(path_buf, sizeof(path_buf), "%s/.ndm/active", home);
    file_write(path_buf, false, "%s", argv[0]);

    log_info(true, "Successfully switched to Node.js v%s (User Mode)", argv[0]);
    return 0;
}