#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#include "nodeman/utils.h"
#include "nodeman/core.h"

#define NODE_INSTALL_DIR "/opt/nodeman"

/**
 * Iterates through a directory and prints installed Node.js versions.
 * Filters out internal utility directories like 'bin', 'config', etc.
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
        // Skip hidden files/folders
        if (dir->d_name[0] == '.') continue;

        // Skip internal management directories
        if (strcmp(dir->d_name, "bin") == 0 || 
            strcmp(dir->d_name, "config") == 0 || 
            strcmp(dir->d_name, "active") == 0 || 
            strcmp(dir->d_name, "default") == 0) {
            continue;
        }

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
    char *search_term = NULL;

    // 1. Parse search flag
    for (int i = 0; i < argc; i++) {
        if (argv[i] != NULL && strcmp(argv[i], "-s") == 0 && (i + 1) < argc) {
            search_term = argv[i + 1];
            break;
        }
    }

    // 2. Build Remote Command
    char cmd[512];
    if (search_term != NULL) {
        log_info(*verbose, "Filtering remote versions for pattern: %s", search_term);
        snprintf(cmd, sizeof(cmd), "curl -s https://nodejs.org/dist/index.tab | awk 'NR>1 {print $1}' | grep '%s'", search_term);
    } else {
        log_info(*verbose, "Fetching available remote versions from nodejs.org...");
        snprintf(cmd, sizeof(cmd), "curl -s https://nodejs.org/dist/index.tab | awk 'NR>1 {print $1}' | head -n 30");
    }

    // 3. Execution via popen
    FILE *fp = popen(cmd, "r");
    if (!fp) {
        log_error("Failed to establish connection to remote repository");
        errno = ECOMM;
        return 1;
    }

    printf("\n--- Available Remote Versions ---\n");
    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        printf("  %s", line);
    }

    return pclose(fp);
}

/**
 * Lists Node.js versions installed in the user's home directory.
 */
int list_local(bool *verbose, int argc, char *argv[]) {
    char path[512];
    const char *home = getenv("HOME");
    if (!home) {
        log_error("Environment variable $HOME not found");
        return 1;
    }

    snprintf(path, sizeof(path), "%s/.ndm", home);
    print_dir_contents(path, "Local (User)", verbose);
    return 0;
}

/**
 * Lists Node.js versions installed in the global system directory.
 */
int list_system(bool *verbose, int argc, char *argv[]) {
    print_dir_contents(NODE_INSTALL_DIR, "System (Global)", verbose);
    return 0;
}

/**
 * Main entry point for the 'list' command.
 */
int list(bool *verbose, int argc, char *argv[]) {
    bool is_remote = false;
    bool is_system = false;

    // 1. Identify flags
    for (int i = 0; i < argc; i++) {
        if (argv[i] == NULL) continue;
        if (strcmp(argv[i], "--remote") == 0 || strcmp(argv[i], "-r") == 0) {
            is_remote = true;
        } else if (strcmp(argv[i], "--system") == 0 || strcmp(argv[i], "-g") == 0) {
            is_system = true;
        }
    }

    // 2. Dispatch to appropriate list function
    if (is_remote) {
        return list_remote(verbose, argc, argv);
    }
    
    if (is_system) {
        return list_system(verbose, argc, argv);
    }

    // 3. Default behavior
    return list_local(verbose, argc, argv);
}

/**
 * Fetches Node.js versions and stores them in a dynamic array.
 * @param search_term  Filter pattern (e.g., "v20") or NULL for all.
 * @param out_count    Pointer to store the number of versions found.
 * @return            Array of version strings (must be freed by caller).
 */
char** get_remote_versions_array(bool *verbose, const char *search_term, int *out_count) {
    char cmd[512];
    char line[128];
    char **versions = NULL;
    int count = 0;
    int capacity = 10; // Initial capacity

    // 1. Build Command
    if (search_term != NULL) {
        snprintf(cmd, sizeof(cmd), "curl -s https://nodejs.org/dist/index.tab | awk 'NR>1 {print $1}' | grep '%s'", search_term);
    } else {
        snprintf(cmd, sizeof(cmd), "curl -s https://nodejs.org/dist/index.tab | awk 'NR>1 {print $1}' | head -n 50");
    }

    // 2. Open Pipe
    FILE *fp = popen(cmd, "r");
    if (!fp) {
        log_error("Failed to fetch remote versions for array mapping");
        return NULL;
    }

    // 3. Allocate Initial Memory
    versions = malloc(capacity * sizeof(char *));
    if (!versions) return NULL;

    // 4. Fill Array
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;

        // Resize array if capacity is reached
        if (count >= capacity) {
            capacity *= 2;
            versions = realloc(versions, capacity * sizeof(char *));
        }

        // Store duplicate of the string
        versions[count] = strdup(line);
        count++;
    }

    pclose(fp);
    *out_count = count;
    
    log_info(*verbose, "Mapped %d versions to memory array.", count);
    return versions;
}

/**
 * Helper to free the allocated version array.
 */
void free_versions_array(char **versions, int count) {
    if (!versions) return;
    for (int i = 0; i < count; i++) {
        free(versions[i]);
    }
    free(versions);
}