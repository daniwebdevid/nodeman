#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

#include "nodeman/core.h"
#include "nodeman/utils.h"

#define NODE_INSTALL_DIR "/opt/nodeman"

int use(bool *verbose, int  argc, char *argv[]) {
    
    //check flags
    bool default_flags = false;
    for (int i =0; i < argc; i++) {
        if(strcmp(argv[i], "--default") == 0) {
            default_flags = true;
        }
    }


    if (default_flags == true) {
        
        //check uid is not 0
        if (getuid() != 0) {
            log_error("Need root for this operation");
            errno = EACCES;
            return 2;
        }
        
        //is avaliable in local
        char path[512];
        snprintf(path, sizeof(path), "%s/%s/source", NODE_INSTALL_DIR, argv[0]);
        log_info(*verbose,"Checking local path: %s\n", path);
        if (access(path, F_OK) != 0) {
            log_error(" %s/%s/source Not found ", NODE_INSTALL_DIR, argv[0]);
            errno = ENOENT;
            return 2;
        }
        
        //copy folder to default path
        log_info(*verbose, "Copy folder in version to default folder");
        if(command(verbose, "cp -r %s/%s/source %s/default/", NODE_INSTALL_DIR, argv[0], NODE_INSTALL_DIR) != 0) {
            log_error("Error to copy file");
            errno = EIO;
            return 2;
        }

        //create symlink default folder
        log_info(*verbose, "create symlink to folder bin in default path");
        if(command(verbose, "ln -s %s/default/source/bin %s/default/bin", NODE_INSTALL_DIR, NODE_INSTALL_DIR) != 0) {
            log_error("Failed to create symlink");
            errno = EIO;
            return 2;
        }
    } else {
        log_info(true, "Start to change symlink to ~/.ndm/bin");

        //check uid is not 0
        if (getuid() == 0) {
            log_error("No need root for this operation");
            errno = EPERM;
            return 2;
        }

        //is avaliable in local
        char pathNodejs[512];
        snprintf(pathNodejs, sizeof(pathNodejs), "%s/%s/source", NODE_INSTALL_DIR, argv[0]);
        log_info(*verbose,"Checking node local path: %s\n", pathNodejs);
        if (access(pathNodejs, F_OK) != 0) {
            log_error(" %s/%s/source Not found ", NODE_INSTALL_DIR, argv[0]);
            errno = ENOENT;
            return 2;
        }

        log_info(*verbose, "Check dotfiles");
        char dotfile[512];
        snprintf(dotfile, sizeof(dotfile), "%s/.ndm/", getenv("HOME"));
        log_info(*verbose,"Checking dotfile path: %s\n", dotfile);
        if (access(dotfile, F_OK) != 0) {
            command(verbose, "mkdir -p %s/.ndm/", getenv("HOME"));
        }

        log_info(*verbose, "Check dotfiles");
        char workspace_path[512];
        snprintf(workspace_path, sizeof(workspace_path), "%s/.ndm/%s/bin", getenv("HOME"), argv[0]);
        log_info(*verbose,"Checking workspace path: %s\n", workspace_path);
        if (access(workspace_path, F_OK) != 0) {
            command(verbose, "mkdir -p %s/.ndm/%s/bin", getenv("HOME"), argv[0]);
        }

        char bin[4][10] = {"node", "npm", "npx", "corepack"};
        for (int i = 0; i < 4; i++) {
            log_info(*verbose, "create symlink for %s", bin[i]);
            if(command(verbose, "ln -sf %s/%s/bin/%s %s/.ndm/%s/bin/", NODE_INSTALL_DIR, argv[0], bin[i], getenv("HOME"), argv[0]) != 0) {
                log_error("Failed to create symlink %s", bin[i]);
                errno = EIO;
                return 1;
            }
        }

        //delete binary path on dotfile
        if(command(verbose, "rm -rf %s/.ndm/bin", getenv("HOME")) != 0) {
            log_error("Failed to remove %s/.ndm/bin", getenv("HOME"));
            errno = EIO;
            return 1;
        }

        //create symlink to user absolute Path
        if(command(verbose, "ln -sf %s/.ndm/%s/bin %s/.ndm/bin", getenv("HOME"), argv[0], getenv("HOME")) != 0) {
            log_error("Failed to create symlink to user absolute path");
            errno = EIO;
            return 1;
        }

        //write .npmrc for user workspace
        char nmprc_path[1024];
        snprintf(nmprc_path, sizeof(nmprc_path), "%s/.npmrc", getenv("HOME"));
        if(file_write(nmprc_path, false, "prefix=%s/.ndm/%s/", getenv("HOME"), argv[0]) != 0) {
            log_error("Failed to write npmrc");
            errno = EIO;
            return 1;
        }

        //write .ndm/active
        char active_path[1024];
        snprintf(active_path, sizeof(active_path), "%s/.ndm/active", getenv("HOME"));
        if(file_write(active_path, false, "%s", argv[0]) != 0) {
            log_error("Failed to write active file");
            errno = EIO;
            return 1;
        }
        
    }
    return 0;
}