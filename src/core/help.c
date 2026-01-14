#include <stdio.h>
#include "nodeman/core.h"

/**
 * Displays the usage and help information for nodeman.
 * Structured for quick reference and clarity.
 */
void help() {
    printf("\nNode Manager (NDM) v%s - Fast & Lightweight Node Version Manager\n", NDM_VERSION);
    printf("Usage: ndm <command> [argument] [options]\n\n");

    printf("Commands:\n");
    // Column alignment dibuat rata biar enak di scan mata
    printf("  install <version>        Install a specific Node.js version (Root required)\n");
    printf("  use <version>            Switch to a version for current user\n");
    printf("  use <version> --default  Switch global default version (Root required)\n");
    printf("  list                     List installed versions for current user\n");
    printf("  list --system            List globally installed versions\n");
    printf("  list --remote            List available versions from nodejs.org\n");
    printf("  remove <version>         Uninstall a specific version (Root required)\n");

    printf("\nGlobal Options:\n");
    printf("  -v, --version            Show NDM version\n");
    printf("  -h, --help               Show this help message\n");
    printf("  --verbose                Show detailed execution logs\n");

    printf("\nExamples:\n");
    printf("  ndm install 20.11.0      # Install version 20.11.0\n");
    printf("  ndm use 18.15.0          # Switch current user to 18.15.0\n");
    printf("  sudo ndm use 20.11.0 --default\n");
    
    printf("\nFor more info or reporting bugs, visit the documentation.\n\n");
}