#ifndef CORE_H
#define CORE_H

#include <stdbool.h>

/**
 * Displays the usage and help information for nodeman.
 */
void help();

/**
 * Installs a specific Node.js version.
 * @return 0 on success, non-zero on error.
 */
int install(bool *verbose, char *argv[]);

/**
 * Dispatches the version switch to either global or user scope.
 */
int use(bool *verbose, int argc, char *argv[]);

/**
 * Switches the global (system-wide) Node.js version.
 * Requires root privileges.
 */
int use_default(bool *verbose, char *argv[]);

/**
 * Switches the Node.js version for the current user only (~/.ndm).
 */
int use_user(bool *verbose, char *argv[]);

int list(bool *verbose, int argc, char *argv[]);
int list_remote(bool *verbose, int argc, char *argv[]);
int list_system(bool *verbose, int argc, char *argv[]);
int list_local(bool *verbose, int argc, char *argv[]);
char** get_remote_versions_array(bool *verbose, const char *search_term, int *out_count);
void free_versions_array(char **versions, int count);

#endif // CORE_H