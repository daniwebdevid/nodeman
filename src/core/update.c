#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

#include "nodeman/utils.h"
#include "nodeman/core.h"

static char temp_dir[PATH_MAX] = "/tmp/nodeman";

/**
 * Cleanup function registered via atexit.
 * Ensures no leftover files in /tmp.
 */
static void cleanup_nodeman_dir(void) {
    char path[PATH_MAX];
    
    snprintf(path, sizeof(path), "%s/latest.json", temp_dir);
    unlink(path);
    
    snprintf(path, sizeof(path), "%s/install.sh", temp_dir);
    unlink(path);
    
    rmdir(temp_dir);
}

/**
 * Self-update logic for NDM.
 * Fetches latest tag from GitHub and executes official installer.
 */
int update(bool *verbose) {
    // 0. Environment & Privilege Check
    if (getuid() != 0) {
        log_error("Root privileges required for system update");
        errno = EACCES; 
        return 2;
    }

    // 1. Workspace Preparation
    if (mkdir(temp_dir, 0755) != 0 && errno != EEXIST) {
        log_error("Failed to create workspace: %s", temp_dir);
        return 1;
    }

    atexit(cleanup_nodeman_dir);

    // 2. Fetch Remote State
    if (*verbose) log_info(true, "Checking for updates...");

    char curl_cmd[512];
    snprintf(curl_cmd, sizeof(curl_cmd), 
             "curl -s -o %s/latest.json https://api.github.com/repos/daniwebdevid/nodeman/releases/latest", 
             temp_dir);

    if (command(verbose, curl_cmd) != 0) {
        log_error("Failed to fetch release metadata");
        return 1;
    }

    // 3. Version Comparison
    char lts_version[16] = {0};
    char jq_cmd[512];
    snprintf(jq_cmd, sizeof(jq_cmd), "jq -r .tag_name %s/latest.json", temp_dir);

    if (command_output(lts_version, sizeof(lts_version), verbose, jq_cmd) != 0) {
        log_error("Failed to parse remote version");
        return 1;
    }

    if (strcmp(lts_version, NDM_VERSION) == 0 || strcmp(lts_version + 1, NDM_VERSION) == 0) {
        log_info(true, "NDM is already at the latest version (%s)", NDM_VERSION);
        return 0;
    }

    log_info(true, "Update found: %s -> %s", NDM_VERSION, lts_version);

    // 4. Execution
    char download_cmd[1024];
    snprintf(download_cmd, sizeof(download_cmd), 
        "curl -fL -o %s/install.sh https://github.com/daniwebdevid/nodeman/releases/download/%s/install.sh", 
        temp_dir, lts_version);

    if (command(verbose, download_cmd) != 0) {
        log_error("Failed to download installer");
        return 1;
    }

    char exec_cmd[1024];
    snprintf(exec_cmd, sizeof(exec_cmd), "chmod +x %s/install.sh && %s/install.sh", temp_dir, temp_dir);

    if (command(verbose, exec_cmd) != 0) {
        log_error("Update execution failed");
        return 1;
    }

    log_info(true, "NDM successfully updated to %s", lts_version);
    return 0;
}