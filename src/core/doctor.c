#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#include "nodeman/utils.h"
#include "nodeman/core.h"

/**
 * Diagnostic tool to verify NDM environment health.
 * Checks for path integrity, configuration, and binary availability.
 */
int doctor(bool *verbose) {
    char *home_path = getenv("HOME");
    bool issues_found = false;

    // 1. Config State Validation
    if (home_path) {
        char active_path[PATH_MAX];
        snprintf(active_path, sizeof(active_path), "%s/.ndm/active", home_path);
        
        char version[64];
        if (open_file(version, sizeof(version), active_path)) {
            log_info(true, "Current active version: %s", version);
        } else {
            log_warn("No active version found in ~/.ndm/active");
            issues_found = true;
        }
    }

    // 2. PATH Environment Inspection
    const char *path_env = getenv("PATH");
    if (!path_env) {
        log_error("CRITICAL: $PATH environment variable is empty");
        return 1;
    }

    char *paths_copy = strdup(path_env);
    if (!paths_copy) return 1;

    char *dir = strtok(paths_copy, ":");
    int ndm_count = 0;
    bool node_found = false;

    while (dir != NULL) {
        if (strstr(dir, ".ndm/bin")) {
            ndm_count++;
        }

        char node_exe[PATH_MAX];
        snprintf(node_exe, sizeof(node_exe), "%s/node", dir);
        if (access(node_exe, X_OK) == 0) {
            node_found = true;
        }
        dir = strtok(NULL, ":");
    }
    free(paths_copy);

    if (ndm_count == 0) {
        log_error("NDM bin directory is missing from $PATH");
        issues_found = true;
    } else if (ndm_count > 1) {
        log_warn("Found %d duplicate NDM entries in $PATH", ndm_count);
        issues_found = true;
    }

    if (!node_found) {
        log_error("'node' binary is not reachable via $PATH");
        issues_found = true;
    }

    // 3. Symlink & Binary Integrity
    if (home_path) {
        char sym_node[PATH_MAX];
        snprintf(sym_node, sizeof(sym_node), "%s/.ndm/bin/node", home_path);
        
        if (access(sym_node, F_OK) != 0) {
            log_error("Active symlink is broken: %s", sym_node);
            issues_found = true;
        } else {
            char target[PATH_MAX];
            ssize_t len = readlink(sym_node, target, sizeof(target)-1);
            if (len != -1) {
                target[len] = '\0';
                log_info(*verbose, "Symlink target: %s", target);
            }
        }
    }

    // 4. Final Verdict
    if (!issues_found) {
        log_info(true, "Health Check: OK. NDM is ready for production.");
        return 0;
    }

    log_warn("Health Check: Found issues that might affect NDM performance.");
    return 1;
}