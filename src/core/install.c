#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "nodeman/utils.h"
#include "nodeman/core.h"

#define CACHE "/var/cache/nodeman"

/**
 * Installs a specific Node.js version with integrity check.
 * Adheres to the production-grade caching and extraction structure.
 */
int install(bool *verbose, char *argv[]) {
    char tarball_path[512], shasum_path[512], install_path[512];
    char target_version[64]; 
    char *version_input = argv[0];

    log_info(true, "Initializing installation for Node.js version: %s", version_input);

    // 0. Privilege Check
    if(getuid() != 0) {
        log_error("Root privileges (sudo) are required for installation to %s", NODE_INSTALL_DIR);
        errno = EACCES;
        return 2;
    }

    // 1. Input Validation & Version Normalization
    if (version_input == NULL || strchr(version_input, '/') != NULL) {
        log_error("Invalid version name: '%s'. Path traversal is not allowed.", 
                  version_input ? version_input : "NULL");
        errno = EINVAL;
        return 2;
    }

    // Normalisasi input ke target_version
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

    snprintf(tarball_path, sizeof(tarball_path), "%s/node-v%s.tar.xz", CACHE, target_version);
    snprintf(shasum_path, sizeof(shasum_path), "%s/node-v%s.txt.asc", CACHE, target_version);
    snprintf(install_path, sizeof(install_path), "%s/%s", NODE_INSTALL_DIR, target_version);

    // 2. Local Conflict Check
    if (command(verbose, "%s/bin/node -v > /dev/null 2>&1", install_path) == 0) {
        log_error("Installation skipped: version '%s' already exists at %s", target_version, install_path);
        return 2;
    }

    // 3. Remote Availability & Cache Check
    bool has_tarball = (access(tarball_path, F_OK) == 0);
    bool has_shasum = (access(shasum_path, F_OK) == 0);

    if (!has_tarball || !has_shasum) {
        log_info(*verbose, "Cache incomplete. Verifying remote distribution for v%s...", target_version);
        
        char dist_status[64];
        command_output(dist_status, sizeof(dist_status), verbose, 
                       "curl -s -o /dev/null -I -w '%%{http_code}' https://nodejs.org/dist/v%s/", target_version);
        
        if(strncmp(dist_status, "200", 3) != 0) {
            log_error("Version '%s' not found on nodejs.org (HTTP %s)", target_version, dist_status);
            errno = ENOENT;
            return 2;
        }

        command(verbose, "mkdir -p %s", CACHE);

        // 4. Download Process
        if (!has_tarball) {
            log_info(*verbose, "Downloading Node.js tarball...");
            const char* arch = get_arch();
            if (command(verbose, "curl --remove-on-error -fL -o %s https://nodejs.org/dist/v%s/node-v%s-linux-%s.tar.xz", 
                tarball_path, target_version, target_version, arch) != 0) {
                log_error("Failed to download tarball");
                return 1;
            }
        }

        if (!has_shasum) {
            log_info(*verbose, "Downloading checksum signatures...");
            if (command(verbose, "curl --remove-on-error -fL -o %s https://nodejs.org/dist/v%s/SHASUMS256.txt.asc", 
                shasum_path, target_version) != 0) {
                log_error("Failed to download SHASUMS");
                return 1;
            }
        }
    }

    // 5. Integrity Verification
    log_info(*verbose, "Verifying file integrity using SHA256...");
    char expected_sha[70];
    char actual_sha_line[256];
    const char* arch = get_arch();

    command_output(expected_sha, sizeof(expected_sha), verbose, 
                   "awk '/linux-%s.tar.xz/ {print $1}' %s", arch, shasum_path);
    command_output(actual_sha_line, sizeof(actual_sha_line), verbose, 
                   "sha256sum %s", tarball_path);

    char *space = strchr(actual_sha_line, ' ');
    if (space != NULL) *space = '\0';

    expected_sha[strcspn(expected_sha, "\r\n")] = 0;
    actual_sha_line[strcspn(actual_sha_line, "\r\n")] = 0;

    if (strncmp(expected_sha, actual_sha_line, 64) != 0) {
        log_error("Integrity Verification failed! Checksum mismatch.");
        log_error("Expected: %s", expected_sha);
        log_error("Actual:   %s", actual_sha_line);
        errno = EBADMSG;
        return 1;
    }

    // 6. Extraction & Finalization
    log_info(*verbose, "Extracting Node.js binary to %s...", install_path);
    command(verbose, "mkdir -p %s", install_path);
    
    if(command(verbose, "tar -xf %s -C %s --strip-components=1", tarball_path, install_path) != 0) {
        log_error("Extraction failed: The downloaded file might be corrupted");
        errno = EIO;
        return 1;
    }

    log_info(true, "Successfully installed Node.js v%s", target_version);
    return 0;
}