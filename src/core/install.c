#include <stdbool.h>
#include <errno.h>
#include <stdbool.h>

#include "nodeman/utils.h"
#include "nodeman/core.h"

#define NODE_INSTALL_DIR "/opt/nodeman"

int install(bool *verbose, char *argv[]) {
    log_info(verbose, " start installing node %s", argv[0]);
    

    //check are valiable in internet
    log_info(verbose, "check the version in dist");
    int isAvaliableDist = command(verbose, "curl -s -o /dev/null -I -w '%%{http_code}' https://nodejs.org/dist/v20.11.0/");
    if(isAvaliableDist != 200) {
        log_error("No such version on dist");
        errno = ENOENT;
        return 2;
    }

    //check version in local
    int isAvaliableLocal = command(verbose, "[ -d '%s/%s/source' ]", NODE_INSTALL_DIR, argv[0]);
    if(isAvaliableLocal == 0) { 
        log_error("Found source path. install skiped");
        return 2;
    }

    //make dir for node version
    log_info(verbose, "Downloading node %s to %s/%s", argv[0], NODE_INSTALL_DIR, argv[0]);
    command(verbose, "mkdir -p %s/%s", NODE_INSTALL_DIR, argv[0]);

    return 0;
}