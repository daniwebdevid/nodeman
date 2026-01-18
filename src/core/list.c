#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#include "nodeman/utils.h"
#include "nodeman/core.h"

/**
 * Iterates through a directory and prints installed Node.js versions.
 * Adheres to the new flat directory structure.
 */
void print_dir_contents(const char *path, const char *label, bool *verbose) {
    DIR *d = opendir(path);
    if (!d) {
        log_warn("Path %s not found or access denied.", path);
        return;
    }

    printf("\n--- %s Node Versions (%s) ---\n", label, path);
    struct dirent *dir;
    int count = 0;

    while ((dir = readdir(d)) != NULL) {
        // Skip hidden files/folders and parent/current pointers
        if (dir->d_name[0] == '.') continue;

        // Skip internal management directories/files
        if (strcmp(dir->d_name, "bin") == 0 || 
            strcmp(dir->d_name, "config") == 0 || 
            strcmp(dir->d_name, "active") == 0 || 
            strcmp(dir->d_name, "default") == 0) {
            continue;
        }

        // Standardized output: already assumes version names as folder names
        printf("  v%s\n", dir->d_name);
        count++;
    }

    if (count == 0) printf("  (No versions found)\n");
    closedir(d);
}

/**
 * Fetches available Node.js versions from the official remote repository.
 */
int list_remote(bool *verbose, int argc, char *argv[]) {
    char *filters[10];
    int filter_count = 0;

    // 0. Filter Extraction
    for (int i = 0; i < argc; i++) {
        if (argv[i] && argv[i][0] != '-' && filter_count < 10) {
            filters[filter_count++] = argv[i];
        }
    }

    // 1. Fetching Logic
    int count = 0;
    char **versions = get_remote_versions_array(verbose, filters, filter_count, &count);

    if (!versions || count == 0) {
        printf("\n--- No versions found matching criteria ---\n");
        if (versions) free(versions);
        return 0;
    }

    // 2. Display Result
    printf("\n--- Available Remote Versions ---\n");
    for (int i = 0; i < count; i++) {
        printf("  %s\n", versions[i]);
        // Limit output to 30 items if no specific filter is provided
        if (filter_count == 0 && i >= 29) break; 
    }

    free_versions_array(versions, count);
    return 0;
}

/**
 * Main entry point for the 'list' command.
 */
int list(bool *verbose, int argc, char *argv[]) {
    bool is_remote = false;
    bool is_system = false;

    // 1. Identify Flags
    for (int i = 0; i < argc; i++) {
        if (argv[i] == NULL) continue;
        if (strcmp(argv[i], "--remote") == 0 || strcmp(argv[i], "-r") == 0) {
            is_remote = true;
        } else if (strcmp(argv[i], "--system") == 0 || strcmp(argv[i], "-g") == 0) {
            is_system = true;
        }
    }

    // 2. Dispatch Logic
    if (is_remote) {
        return list_remote(verbose, argc, argv);
    }
    
    if (is_system) {
        print_dir_contents(NODE_INSTALL_DIR, "System (Global)", verbose);
        return 0;
    }

    // 3. Default (Local) Logic
    char path[512];
    const char *home = getenv("HOME");
    if (!home) {
        log_error("Environment error: $HOME is not set");
        errno = ENOENT;
        return 1;
    }

    snprintf(path, sizeof(path), "%s/.ndm", home);
    print_dir_contents(path, "Local (User)", verbose);
    return 0;
}

/**
 * Fetches versions and filters them in-memory from index.tab.
 */
char** get_remote_versions_array(bool *verbose, char **filters, int filter_count, int *out_count) {
    const char *cmd = "curl -s https://nodejs.org/dist/index.tab | awk 'NR>1 {print $1}'";
    char line[128];
    char **versions = NULL;
    int count = 0;
    int capacity = 10;

    FILE *fp = popen(cmd, "r");
    if (!fp) {
        log_error("Network error: Failed to fetch remote index");
        return NULL;
    }

    versions = malloc(capacity * sizeof(char *));

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;

        bool match = (filter_count == 0); 

        // Efficient prefix filtering
        for (int i = 0; i < filter_count; i++) {
            char prefix[16];
            snprintf(prefix, sizeof(prefix), "v%s", filters[i]);
            if (strncmp(line, prefix, strlen(prefix)) == 0) {
                match = true;
                break;
            }
        }

        if (match) {
            if (count >= capacity) {
                capacity *= 2;
                char **tmp = realloc(versions, capacity * sizeof(char *));
                if (!tmp) break; 
                versions = tmp;
            }
            versions[count++] = strdup(line);
        }
    }

    pclose(fp);
    *out_count = count;
    return versions;
}

/**
 * Fetches the latest stable version for a specific major version.
 */
char* get_latest_of_major(int major) {
    char target_prefix[16];
    snprintf(target_prefix, sizeof(target_prefix), "v%d.", major);

    FILE *fp = popen("curl -s https://nodejs.org/dist/index.tab", "r");
    if (!fp) return NULL;

    char line[256];
    char *latest_version = NULL;
    bool is_header = true;

    while (fgets(line, sizeof(line), fp)) {
        if (is_header) {
            is_header = false;
            continue;
        }

        char *version = strtok(line, "\t");
        if (!version) continue;

        // Since index.tab is sorted from newest to oldest, the first match is the latest.
        if (strncmp(version, target_prefix, strlen(target_prefix)) == 0) {
            latest_version = strdup(version);
            break; 
        }
    }

    pclose(fp);
    return latest_version;
}

void free_versions_array(char **versions, int count) {
    if (!versions) return;
    for (int i = 0; i < count; i++) {
        free(versions[i]);
    }
    free(versions);
}