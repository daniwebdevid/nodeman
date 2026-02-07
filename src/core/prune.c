#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "nodeman/core.h"
#include "nodeman/utils.h"

/**
 * Clears downloaded archives and temporary files from the system cache.
 * Requires root privileges for modification of /var/cache/nodeman.
 */
int prune_cache() {
    // 0. Privilege Check
    if(getuid() != 0) {
        log_error("Root privileges (sudo) are required to clear cache");
        errno = EACCES;
        return 2;
    }

    // 1. Directory Access
    const char *cache_path = "/var/cache/nodeman";
    struct dirent *entry;
    DIR *dp = opendir(cache_path);

    if (dp == NULL) {
        if (errno == ENOENT) {
            log_info(true, "Cache directory does not exist. Nothing to prune.");
            return 0;
        }
        log_error("Failed to access cache directory: %s", cache_path);
        return 1;
    }

    // 2. Iterative Deletion
    int count = 0;
    while ((entry = readdir(dp))) {
        if (entry->d_name[0] == '.') continue;

        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", cache_path, entry->d_name);
        
        if (unlink(file_path) == 0) {
            log_info(true, "Pruned: %s", entry->d_name);
            count++;
        } else {
            log_warn("Failed to delete: %s (Check permissions)", entry->d_name);
        }
    }

    // 3. Finalization
    closedir(dp);
    log_info(true, "Cache pruning complete. Removed %d files.", count);
    
    return 0;
}