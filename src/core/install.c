#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "nodeman/utils.h"
#include "nodeman/core.h"

/**
 * Installs a specific Node.js version.
 * Downloads the binary from the official distribution, extracts it,
 * and sets up the internal directory structure.
 */
int install(bool *verbose, char *argv[]) {
    log_info(true, "Initializing installation for Node.js version: %s", argv[0]);

    // 0. Privilege Check
    if(getuid() != 0) {
        log_error("Root privileges (sudo) are required for installation to %s", NODE_INSTALL_DIR);
        errno = EACCES;
        return 2; // Returning 2 for permission errors consistent with previous code
    }

    // 1. Input Validation
    if (argv[0] == NULL || strcmp(argv[0], "/") == 0 || strchr(argv[0], '/') != NULL) {
        log_error("Invalid version name: '%s'. Path traversal or restricted access is not allowed.", 
                  argv[0] ? argv[0] : "NULL");
        errno = EINVAL; 
        return 2;
    }

    // 2. Remote Availability Check
    log_info(*verbose, "Verifying version '%s' on official Node.js distribution...", argv[0]);
    int isAvaliableDist = command_output(verbose, "curl -s -o /dev/null -I -w '%%{http_code}' https://nodejs.org/dist/v%s/", argv[0]);
    if(isAvaliableDist != 200) {
        log_error("Version '%s' not found on nodejs.org (HTTP %d)", argv[0], isAvaliableDist);
        errno = ENOENT;
        return 2;
    }

    // 3. Local Conflict Check
    char path[512];
    snprintf(path, sizeof(path), "%s/%s/source", NODE_INSTALL_DIR, argv[0]);
    
    if (access(path, F_OK) == 0) {
        log_error("Installation skipped: version '%s' already exists at %s", argv[0], path);
        return 2;
    }

    // 4. Architecture Validation
    log_info(*verbose, "Detecting system architecture...");
    const char* arch = get_arch();
    if(strcmp(arch, "unsupported") == 0) {
        log_error("Installation failed: System architecture is not supported.");
        errno = ENOTSUP;
        return 1;
    }

    // 5. Directory Preparation
    log_info(*verbose, "Creating installation directory: %s/%s", NODE_INSTALL_DIR, argv[0]);
    if (command(verbose, "mkdir -p %s/%s/source", NODE_INSTALL_DIR, argv[0]) != 0) {
        log_error("Failed to create directory structure in %s", NODE_INSTALL_DIR);
        errno = EACCES;
        return 1;
    }

    // 6. Download Tarball
    log_info(*verbose, "Downloading Node.js binary for %s...", arch);
    if((command(verbose, "curl -L -o %s/%s/node.tar.xz https://nodejs.org/dist/v%s/node-v%s-linux-%s.tar.xz", 
        NODE_INSTALL_DIR, argv[0], argv[0], argv[0], arch) != 0))  {

        log_error("Network error: Failed to download Node.js binary from dist");
        errno = ECOMM; 
        return 1;
    }

    // 7. Extraction
    log_info(*verbose, "Extracting Node.js tarball to source directory...");
    if(command(verbose, "tar -xf %s/%s/node.tar.xz -C %s/%s/source --strip-components=1", 
               NODE_INSTALL_DIR, argv[0], NODE_INSTALL_DIR, argv[0]) != 0) {
        log_error("Extraction failed: The downloaded file might be corrupted");
        errno = EBADMSG;
        return 1;
    }

    // 8. Finalizing Symlinks
    log_info(*verbose, "Linking binary directory for version %s", argv[0]);
    if(command(verbose, "ln -sf %s/%s/source/bin %s/%s/bin", 
               NODE_INSTALL_DIR, argv[0], NODE_INSTALL_DIR, argv[0]) != 0) {
        log_error("Failed to create internal symlink for binaries");
        errno = EEXIST;
        return 1;
    }

    log_info(true, "Successfully installed Node.js v%s", argv[0]);
    return 0;
}