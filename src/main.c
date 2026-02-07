#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#include "nodeman/core.h"
#include "nodeman/utils.h"
#include "nodeman/tui.h"

/**
 * Main entry point for ndm (Node Manager) v2.5.0.
 * Handles command dispatching and global flag parsing.
 */
int main(int argc, char *argv[]) {

    // 0. Argument Check (Enter TUI mode if no args)
    if(argc < 2) {
        return main_menu();
    }

    // 1. Global Flags Parsing
    bool verbose = false;
    for (int i = 1; i < argc; i++) {
        if(argv[i] && strcmp(argv[i], "--verbose") == 0) {
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
            log_error("Usage: %s use <version> [--default|--session|-s]", argv[0]);
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
    
    // --- MAINTENANCE & SYSTEM ---
    else if(strcmp(argv[1], "start") == 0) {
        return start(&verbose);
    }
    else if (strcmp(argv[1], "doctor") == 0) {
        return doctor(&verbose);
    }
    else if(strcmp(argv[1], "prune") == 0) {
        return prune_cache();
    }
    else if(strcmp(argv[1], "status") == 0) {
        return status(&verbose);
    }
    else if(strcmp(argv[1], "update") == 0) {
        return update(&verbose);
    }
    
    // --- UTILS (Version & Help) ---
    else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        printf("%s\n", NDM_VERSION);
        return 0;
    } 
    else if(strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0) {
        help();
        return 0;
    } 
    
    // --- UNKNOWN COMMAND ---
    else {
        log_error("Unknown command: '%s'", argv[1]);
        log_error("Type '%s help' for available commands", argv[0]);
        return 1;
    }

    return 0;
}