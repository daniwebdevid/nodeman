#ifndef CORE_H
#define CORE_H

#include <stdbool.h>

/**
 * Global Configuration
 * Production-ready paths and versioning.
 */
#define NODE_INSTALL_DIR "/opt/nodeman"
#define NDM_VERSION      "2.1.0" 

/**
 * CLI Information & Help
 * Displays usage instructions for the user.
 */
void help();

/**
 * Installation Management
 * Handles the lifecycle of Node.js binaries.
 */
int install(bool *verbose, char *argv[]);
int remove_node_js(bool *verbose, int argc, char *argv[]);

/**
 * Version Switching
 * Dispatches the version switch to either global or user scope.
 */
int use(bool *verbose, int argc, char *argv[]);
int use_default(bool *verbose, char *argv[]);
int use_user(bool *verbose, char *argv[]);

/**
 * Version Listing & Discovery
 * Handles both local inspection and remote version fetching.
 */
int list(bool *verbose, int argc, char *argv[]);
int list_remote(bool *verbose, int argc, char *argv[]);

/**
 * Remote Data Handling
 * Low-level functions for version discovery and memory management.
 */
char* get_latest_of_major(int major);
char** get_remote_versions_array(bool *verbose, char **filters, int filter_count, int *out_count);
void free_versions_array(char **versions, int count);
char** get_local_versions_array(const char *path, int *out_count);

#endif // CORE_H