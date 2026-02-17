#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <limits.h>

#include "nodeman/utils.h"
#include "nodeman/core.h"

#define NDM_CACHE "/var/cache/nodeman"
#define NDM_HOME ".ndm"
#define NODENV_HOME ".node-versions"

static int cleanup_user_data(bool *verbose) {
    struct passwd *pw = getpwuid(getuid());
    if (!pw) {
        log_error("Failed to determine user home directory");
        return 1;
    }

    char ndm_path[PATH_MAX], nodeversions_path[PATH_MAX];
    log_info(*verbose, "Cleaning user data from %s...", pw->pw_dir);
    
    snprintf(ndm_path, sizeof(ndm_path), "%s/%s", pw->pw_dir, NDM_HOME);
    snprintf(nodeversions_path, sizeof(nodeversions_path), "%s/%s", pw->pw_dir, NODENV_HOME);
    
    if (access(ndm_path, F_OK) == 0) {
        command(false, "rm -rf %s", ndm_path);
    }
    
    if (access(nodeversions_path, F_OK) == 0) {
        command(false, "rm -rf %s", nodeversions_path);
    }
    
    return 0;
}

static int cleanup_system_files(bool *verbose) {
    const char *system_paths[] = {
        "/etc/environment.d/99-nodeman.conf",
        "/etc/profile.d/nodeman.sh",
        "/usr/local/bin/ndm",
        "/usr/local/bin/node",
        "/usr/local/bin/npm", 
        "/usr/bin/ndm",
        NULL
    };
    
    log_info(*verbose, "Removing system configuration files...");
    for (int i = 0; system_paths[i]; i++) {
        if (access(system_paths[i], F_OK) == 0) {
            if (unlink(system_paths[i]) != 0) {
                log_warn("Failed to remove %s", system_paths[i]);
            }
        }
    }
    return 0;
}

static int cleanup_pam_configs(bool *verbose) {
    const char *pam_configs[] = {
        "/etc/pam.d/login",
        "/etc/pam.d/common-session",
        "/etc/pam.d/sshd",
        NULL
    };
    
    const char *pattern = "environment=/etc/environment.d/";
    log_info(*verbose, "Cleaning PAM configurations...");
    
    for (int i = 0; pam_configs[i]; i++) {
        if (access(pam_configs[i], F_OK) != 0) continue;

        FILE *src = fopen(pam_configs[i], "r");
        if (!src) continue;

        char tmp_path[PATH_MAX];
        snprintf(tmp_path, sizeof(tmp_path), "%s.tmp", pam_configs[i]);
        FILE *dst = fopen(tmp_path, "w");
        if (!dst) {
            fclose(src);
            continue;
        }

        char line[1024];
        bool changed = false;
        while (fgets(line, sizeof(line), src)) {
            if (strstr(line, pattern) == NULL) {
                fputs(line, dst);
            } else {
                changed = true;
            }
        }

        fclose(src);
        fclose(dst);

        if (changed) {
            rename(tmp_path, pam_configs[i]);
        } else {
            unlink(tmp_path);
        }
    }
    return 0;
}

int uninstall(bool *verbose, char *argv[]) {
    log_info(true, "=== NodeMan Uninstaller v2.6.0 ===");
    
    if (getuid() != 0) {
        log_error("Root privileges (sudo) required for uninstallation");
        return 2;
    }

    cleanup_user_data(verbose);
    cleanup_system_files(verbose);
    cleanup_pam_configs(verbose);
    
    log_info(*verbose, "Removing installation directory and cache...");
    command(verbose, "rm -rf /opt/nodeman");
    command(verbose, "rm -rf %s", NDM_CACHE);
    
    log_info(true, "Uninstall completed successfully!");
    return 0;
}