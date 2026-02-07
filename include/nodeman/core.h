#ifndef CORE_H
#define CORE_H

#include <stdbool.h>

/**
 * Global Configuration
 * Production-ready paths and versioning.
 */
#define NODE_INSTALL_DIR "/opt/nodeman"
#define NDM_VERSION      "2.5.0" 

/**
 * CLI Information & Help
 */
void help();

/**
 * System Lifecycle & Health
 * Core routines for environment setup and diagnostics.
 */
int start(bool *verbose);
int doctor(bool *verbose);
int status(bool *verbose);
int prune_cache();
int update(bool *verbose);

/**
 * Installation Management
 */
int install(bool *verbose, char *argv[]);
int remove_node_js(bool *verbose, int argc, char *argv[]);

/**
 * Version Switching
 * Supports: --default (global), user-scope, and --session (temporary).
 */
int use(bool *verbose, int argc, char *argv[]);
int use_default(bool *verbose, char *argv[]);
int use_user(bool *verbose, char *argv[]);

/**
 * Version Listing & Discovery
 */
int list(bool *verbose, int argc, char *argv[]);
int list_remote(bool *verbose, int argc, char *argv[]);

/**
 * Version Data Handling
 * Low-level memory and discovery utilities.
 */
char* get_latest_of_major(int major);
char** get_remote_versions_array(bool *verbose, char **filters, int filter_count, int *out_count);
char** get_local_versions_array(const char *path, int *out_count);
void free_versions_array(char **versions, int count);

#endif // CORE_H