#ifndef CORE_H
#define CORE_H

#include <stdbool.h>
#include <limits.h>

/**
 * Global Configuration
 */
#define NODE_INSTALL_DIR "/opt/nodeman"
#define NDM_VERSION "2.0.0" 

/**
 * CLI Information & Help
 */
void help();

/**
 * Installation Management
 */
int install(bool *verbose, char *argv[]);
int remove_node_js(bool *verbose, int argc, char *argv[]);

/**
 * Version Switching (Dispatchers)
 * Dispatches the version switch to either global or user scope.
 */
int use(bool *verbose, int argc, char *argv[]);
int use_default(bool *verbose, char *argv[]);
int use_user(bool *verbose, char *argv[]);

/**
 * Version Listing & Discovery
 */
int list(bool *verbose, int argc, char *argv[]);
int list_remote(bool *verbose, int argc, char *argv[]);
int list_system(bool *verbose, int argc, char *argv[]);
int list_local(bool *verbose, int argc, char *argv[]);

/**
 * Remote Data Handling
 * Functions for fetching and cleaning up version data.
 */
char** get_remote_versions_array(bool *verbose, const char *search_term, int *out_count);
void free_versions_array(char **versions, int count);

#endif // CORE_H