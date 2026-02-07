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
 * Supports: --default (system), --session (temporary), and user scope (default).
 */
int use(bool *verbose, int argc, char *argv[]) {
    bool default_flags = false;
    bool session_flags = false;
    char target_version[64];
    char *version_input = NULL;
    
    // 0. Argument Parsing
    for (int i = 0; i < argc; i++) {
        if (argv[i] == NULL) continue;

        if (strcmp(argv[i], "--default") == 0) {
            default_flags = true;
        } else if (strcmp(argv[i], "--session") == 0 || strcmp(argv[i], "-s") == 0) {
            session_flags = true;
        } else if (argv[i][0] != '-') {
            version_input = argv[i];
        }
    }

    if (version_input == NULL) {
        log_error("No version specified");
        return 2;
    }

    // 0.1 Session Switch Logic (Shell Eval Mode)
    if (session_flags) {
        printf("export PATH=\"%s/%s/bin:$PATH\";\n", NODE_INSTALL_DIR, version_input);
        printf("echo '[ndm] Switched to %s for this session only';\n", version_input);
        return 0;
    }

    // 1. Input Validation (Security & Path Traversal Check)
    if (strchr(version_input, '/') != NULL) {
        log_error("Invalid version name: '%s'. Path traversal is not allowed.", version_input);
        errno = EINVAL; 
        return 2;
    }

    // 1.1 Version Normalization (Major to Latest)
    if (strncmp(version_input, "v", 1) == 0) {
        int major = atoi(version_input + 1);
        char* latest = get_latest_of_major(major);
        if (latest != NULL) {
            strncpy(target_version, latest + 1, sizeof(target_version) - 1);
            free(latest);
        } else {
            strncpy(target_version, version_input + 1, sizeof(target_version) - 1);
        }
    } else {
        strncpy(target_version, version_input, sizeof(target_version) - 1);
    }

    // 2. Dispatch Logic
    char *dispatch_argv[] = { target_version };
    if (default_flags) {
        return use_default(verbose, dispatch_argv);
    } else {
        return use_user(verbose, dispatch_argv);
    }
}

/**
 * Handles system-wide Node.js version switching.
 */
int use_default(bool *verbose, char *argv[]) {
    log_info(true, "Initializing global switch to Node.js v%s", argv[0]);

    if (getuid() != 0) {
        log_error("Root privileges required for global configuration");
        errno = EACCES;
        return 2;
    }

    char version_path[512];
    snprintf(version_path, sizeof(version_path), "%s/%s", NODE_INSTALL_DIR, argv[0]);
    
    if (access(version_path, F_OK) != 0) {
        log_error("Version '%s' is not installed in %s", argv[0], NODE_INSTALL_DIR);
        errno = ENOENT;
        return 2;
    }

    log_info(*verbose, "Updating global default symlink to v%s", argv[0]);
    
    char default_path[512];
    snprintf(default_path, sizeof(default_path), "%s/default", NODE_INSTALL_DIR);

    if (symlink_force(verbose, version_path, default_path) != 0) {
        log_error("Failed to update global 'default' symlink");
        return 1;
    }
    
    log_info(true, "Successfully set v%s as the global default", argv[0]);
    return 0;
}

/**
 * Handles user-specific Node.js version switching.
 */
int use_user(bool *verbose, char *argv[]) {
    char *home = getenv("HOME");

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

    char node_bin[512];
    snprintf(node_bin, sizeof(node_bin), "%s/%s/bin/node", NODE_INSTALL_DIR, argv[0]);
    
    if (access(node_bin, X_OK) != 0) {
        log_error("Node.js v%s is not properly installed (binary not found)", argv[0]);
        errno = ENOENT;
        return 2;
    }

    log_info(*verbose, "Preparing isolated environment in %s/.ndm/%s", home, argv[0]);
    char user_version_bin[512];
    snprintf(user_version_bin, sizeof(user_version_bin), "%s/.ndm/%s/bin", home, argv[0]);
    
    if (command(verbose, "mkdir -p %s", user_version_bin) != 0) {
        log_error("Failed to create user directory structure");
        return 1;
    }

    const char *binaries[] = {"node", "npm", "npx", "corepack"};
    char source_file[512];
    char dest_file[512];

    for (int i = 0; i < 4; i++) {
        snprintf(source_file, sizeof(source_file), "%s/%s/bin/%s", NODE_INSTALL_DIR, argv[0], binaries[i]);
        snprintf(dest_file, sizeof(dest_file), "%s/%s", user_version_bin, binaries[i]);

        log_info(*verbose, "Linking %s -> %s", binaries[i], dest_file);
        if (symlink_force(verbose, source_file, dest_file) != 0) {
            log_error("Failed to link binary: %s", binaries[i]);
            return 1;
        }
    }

    log_info(*verbose, "Updating active environment symlink...");
    char active_link[512];
    snprintf(active_link, sizeof(active_link), "%s/.ndm/bin", home);

    if (symlink_force(verbose, user_version_bin, active_link) != 0) {
        log_error("Failed to update active symlink");
        return 1;
    }

    char path_buf[1024];
    log_info(*verbose, "Finalizing .npmrc and active state");

    snprintf(path_buf, sizeof(path_buf), "%s/.npmrc", home);
    file_write(path_buf, false, "prefix=%s/.ndm/%s/", home, argv[0]);

    snprintf(path_buf, sizeof(path_buf), "%s/.ndm/active", home);
    file_write(path_buf, false, "%s", argv[0]);

    log_info(true, "Successfully switched to Node.js v%s (User Mode)", argv[0]);
    return 0;
}