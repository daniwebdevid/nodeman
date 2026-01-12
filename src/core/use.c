#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "nodeman/core.h"
#include "nodeman/utils.h"

#define NODE_INSTALL_DIR "/opt/nodeman"

/**
 * Entry point for switching Node versions.
 * Dispatches to either system-wide (default) or user-specific installation.
 */
int use(bool *verbose, int argc, char *argv[]) {
    bool default_flags = false;
    
    // Check for --default flag in arguments
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--default") == 0) {
            default_flags = true;
            break;
        }
    }

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
    if (getuid() != 0) {
        log_error("Root privileges required for global configuration (current UID: %d)", getuid());
        return (errno = EACCES, 2);
    }

    char path[512];
    snprintf(path, sizeof(path), "%s/%s/source", NODE_INSTALL_DIR, argv[0]);
    
    if (access(path, F_OK) != 0) {
        log_error("Node.js source directory not found: %s", path);
        return (errno = ENOENT, 2);
    }

    log_info(*verbose, "Synchronizing version '%s' to global default...", argv[0]);
    
    // Perform copy and update global symlinks
    command(verbose, "cp -r %s/%s/source %s/default/", NODE_INSTALL_DIR, argv[0], NODE_INSTALL_DIR);
    command(verbose, "ln -snf %s/default/source/bin %s/default/bin", NODE_INSTALL_DIR, NODE_INSTALL_DIR);
    
    return 0;
}

/**
 * Handles user-specific Node.js version switching.
 * Creates symlinks within the user's home directory.
 */
int use_user(bool *verbose, char *argv[]) {
    char *home = getenv("HOME");
    if (!home) {
        log_error("Environment variable $HOME is not set");
        return 1;
    }

    if (getuid() == 0) {
        log_error("Running 'use_user' as root is not recommended; use --default for global setup");
        return (errno = EPERM, 2);
    }

    // 1. Source Path Validation
    char pathNodejs[512];
    snprintf(pathNodejs, sizeof(pathNodejs), "%s/%s/source", NODE_INSTALL_DIR, argv[0]);
    if (access(pathNodejs, F_OK) != 0) {
        log_error("Node.js version '%s' is not installed in %s", argv[0], NODE_INSTALL_DIR);
        return (errno = ENOENT, 2);
    }

    // 2. Setup Local Directory Structure (~/.ndm)
    log_info(*verbose, "Preparing local directory structure in %s/.ndm", home);
    command(verbose, "mkdir -p %s/.ndm/%s/bin", home, argv[0]);

    // 3. Link Node.js Binaries
    char *binaries[] = {"node", "npm", "npx", "corepack"};
    for (int i = 0; i < 4; i++) {
        log_info(*verbose, "Linking binary [%s] for version %s", binaries[i], argv[i]);
        command(verbose, "ln -snf %s/%s/bin/%s %s/.ndm/%s/bin/%s", 
                NODE_INSTALL_DIR, argv[0], binaries[i], home, argv[0], binaries[i]);
    }

    // 4. Update Active Symlink (Atomic Switch)
    // Using -n to ensure the symlink is overwritten rather than creating a nested link
    log_info(*verbose, "Updating active environment symlink...");
    if (command(verbose, "ln -snf %s/.ndm/%s/bin %s/.ndm/bin", home, argv[0], home) != 0) {
        log_error("Failed to update active symlink at %s/.ndm/bin", home);
        return 1;
    }

    // 5. Update Configuration Files (.npmrc & active version tracker)
    char path_buf[1024];
    
    log_info(*verbose, "Updating .npmrc and active version file");
    snprintf(path_buf, sizeof(path_buf), "%s/.npmrc", home);
    file_write(path_buf, false, "prefix=%s/.ndm/%s/", home, argv[0]);

    snprintf(path_buf, sizeof(path_buf), "%s/.ndm/active", home);
    file_write(path_buf, false, "%s", argv[0]);

    log_info(true, "Successfully switched to Node.js %s", argv[0]);
    return 0;
}