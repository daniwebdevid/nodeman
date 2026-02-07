#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include "nodeman/utils.h"
#include "nodeman/core.h"

/**
 * Recursively calculates directory size.
 * Uses lstat to avoid following symlinks (preventing infinite loops).
 */
long long get_dir_size(const char *path) {
    long long size = 0;
    DIR *dir = opendir(path);
    struct dirent *entry;
    
    if (!dir) return -1;
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
            
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        
        struct stat st;
        if (lstat(fullpath, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                long long subsize = get_dir_size(fullpath);
                if (subsize >= 0) size += subsize;
            } else {
                size += st.st_size;
            }
        }
    }
    closedir(dir);
    return size;
}

/**
 * Reports current Node Manager status and resource usage.
 */
int status(bool *verbose) {
    char *home_path = getenv("HOME");
    
    // 1. Version State
    if (home_path) {
        char active_path[PATH_MAX];
        snprintf(active_path, sizeof(active_path), "%s/.ndm/active", home_path);
        
        char version[64];
        if (open_file(version, sizeof(version), active_path)) {
            log_info(true, "Current active version: %s", version);
        } else {
            log_warn("No active version found in ~/.ndm/active");
        }
    }

    // 2. Storage Audit: /opt/nodeman
    long long nodeman_size = get_dir_size(NODE_INSTALL_DIR);
    if (nodeman_size >= 0) {
        double size_mb = nodeman_size / (1024.0 * 1024.0);
        log_info(true, "Storage [%s]: %.2f MB", NODE_INSTALL_DIR, size_mb);
    } else {
        log_warn("Access denied or path missing: %s", NODE_INSTALL_DIR);
    }

    // 3. Storage Audit: Cache
    const char *cache_path = "/var/cache/nodeman";
    long long cache_size = get_dir_size(cache_path);
    if (cache_size >= 0) {
        double size_mb = cache_size / (1024.0 * 1024.0);
        log_info(true, "Cache [%s]: %.2f MB", cache_path, size_mb);
        
        if (size_mb > 1000.0) {
            log_warn("Cache size is high (%.2f MB). Consider running 'ndm prune'", size_mb);
        }
    }

    // 4. Binary Integrity Check
    if (home_path) {
        char sym_node[PATH_MAX];
        snprintf(sym_node, sizeof(sym_node), "%s/.ndm/bin/node", home_path);
        
        if (access(sym_node, F_OK) != 0) {
            log_error("Active symlink is broken: %s", sym_node);
        } else {
            char target[PATH_MAX];
            ssize_t len = readlink(sym_node, target, sizeof(target)-1);
            if (len != -1) {
                target[len] = '\0';
                log_info(*verbose, "Symlink points to: %s", target);
            }
        }
    }

    log_info(true, "Status check completed.");
    return 0;
}