#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "nodeman/utils.h"
#include "nodeman/core.h"

#define NODE_INSTALL_DIR "/opt/nodeman"

int install(bool *verbose, char *argv[]) {
    log_info(true, "start installing node %s", argv[0]);


    if(geteuid() != 0) {
        log_error("need sudo for this command");
        errno = EACCES;
        return 0;
    }
    

    log_info(*verbose, "check the version in dist");
    int isAvaliableDist = command_output(verbose, "curl -s -o /dev/null -I -w '%%{http_code}' https://nodejs.org/dist/v%s/", argv[0]);
    if(isAvaliableDist != 200) {
        log_error("No such version on dist");
        errno = ENOENT;
        return 2;
    }


    char path[512];
    snprintf(path, sizeof(path), "%s/%s/source", NODE_INSTALL_DIR, argv[0]);
    if (verbose != NULL && *verbose) {
        log_info(*verbose,"[DEBUG] Checking local path: %s\n", path);
    }
    if (access(path, F_OK) == 0) {
        log_error("Found source path. install skiped");
        return 2;
    }


    log_info(*verbose, "check architecture system");
    const char* arch = get_arch();
    if(strcmp(arch, "unsupported") == 0) {
        log_error("The architecture is not supported.");
        errno = ENOTSUP;
        return 1;
    }


    log_info(*verbose,"Create node path %s to %s/%s", argv[0], NODE_INSTALL_DIR, argv[0]);
    if (command(verbose, "mkdir -p %s/%s/source", NODE_INSTALL_DIR, argv[0]) != 0) {
        log_error("Failed to create directory");
        errno = EACCES;
        return 1;
    }


    log_info(*verbose, "Download nodejs from https://nodejs.org/dist/v%s/node-v%s-linux-%s.tar.xz", argv[0], argv[0], arch);
    if((command(verbose, "curl -L -o %s/%s/node.tar.xz https://nodejs.org/dist/v%s/node-v%s-linux-%s.tar.xz", 
        NODE_INSTALL_DIR, argv[0], argv[0], argv[0], arch) != 0))  {

            log_error("Failed to download Node.js binary");
        errno = ECOMM; 
        return 1;
    }

    log_info(*verbose, "Exctract nodejs tarball");
    if(command(verbose, "tar -xf %s/%s/node.tar.xz -C %s/%s/source --strip-components=1", NODE_INSTALL_DIR, argv[0], NODE_INSTALL_DIR, argv[0]) != 0) {
        errno = EBADMSG;
        return 1;
    }

    log_info(*verbose, "link binary folder to %s/%s/bin", NODE_INSTALL_DIR, argv[0]);
    if(command(verbose, "ln -sf %s/%s/source/bin %s/%s/bin", NODE_INSTALL_DIR, argv[0], NODE_INSTALL_DIR, argv[0]) != 0) {
        log_error("Failed to create symlink");
        errno = EEXIST;
        return 1;
    }

    log_info(true, "Success to install node@%s", argv[0]);

    return 0;
}