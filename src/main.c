#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#include "nodeman/core.h"
#include "nodeman/utils.h"

/**
 * Main entry point for ndm (Node Manager).
 * Handles command dispatching and global flag parsing.
 */
int main(int argc, char *argv[]) {

    // 0. Argument Check
    if(argc < 2) {
        help();
        return 0;
    }

    // 1. Global Flags Parsing (e.g., --verbose)
    bool verbose = false;
    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
            break;
        }
    }

    // 2. Command Dispatching
    
    // --- INSTALL ---
    if (strcmp(argv[1], "install") == 0) {
        if (argc < 3) {
            log_error("No version selected for installation");
            log_error("Usage: %s install <version>", argv[0]);
            errno = EINVAL; 
            return 2; 
        }
        return install(&verbose, argv + 2);
    } 
    
    // --- USE ---
    else if(strcmp(argv[1], "use") == 0) {
        if (argc < 3) {
            log_error("No version selected to use");
            log_error("Usage: %s use <version> [--default]", argv[0]);
            errno = EINVAL; 
            return 2; 
        }
        return use(&verbose, argc - 2, argv + 2);
    } 
    
    // --- LIST ---
    else if(strcmp(argv[1], "list") == 0) {
        return list(&verbose, argc - 2, argv + 2);
    } 
    
    // --- REMOVE ---
    else if(strcmp(argv[1], "remove") == 0) {
        if (argc < 3) {
            log_error("No version selected for removal");
            log_error("Usage: %s remove <version>", argv[0]);
            errno = EINVAL;
            return 2;
        }
        return remove_node_js(&verbose, argc - 2, argv + 2);
    } 
    
    // --- UTILS (Version & Help) ---
    else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        printf("2.1.0\n");
    } 
    else if(strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0) {
        help();
    } 
    else {
        log_error("Unknown command: '%s'", argv[1]);
        log_error("Type '%s help' for available commands", argv[0]);
        return 1;
    }

    return 0;
}