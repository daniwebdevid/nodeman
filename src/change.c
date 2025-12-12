#include "utils/print.h"
#include "change.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/wait.h>


int change_version(const char *version) {

    print("Starting version change process to v");
    print(version);
    print("...\n");

    char command_buffer[256]; 
    char node_dir[128];
    int len;

    // 1. Path Preparation: Source directory: /usr/local/lib/nodejsXX
    len = snprintf(node_dir, sizeof(node_dir), 
                   "/usr/local/lib/nodejs%s", version);
    if (len < 0 || len >= sizeof(node_dir)) {
        write(2, "Error: Path buffer failed.\n", 27);
        return 1;
    }

    // 2. CHECK SOURCE DIRECTORY EXISTENCE (test -d)
    len = snprintf(command_buffer, sizeof(command_buffer), "test -d %s", node_dir);
    print("\n[STEP 1/2] Checking Source Directory: ");
    int check_result = execute_command_real("COMMAND: CHECK", command_buffer);

    if (check_result != 0) { 
        // Non-zero result means directory NOT FOUND
        print("\n❌ FAILED: Node.js directory v");
        print(version);
        print(" not found at /usr/local/lib/. Change aborted.\n");
        return 1; 
    } 
    
    print("RESULT: Directory found. Proceeding with Symlink creation.\n");

    // 3. CREATE NEW SYMLINK
    // ln -sf /usr/local/lib/nodejsXX/bin/* /usr/local/bin
    len = snprintf(
        command_buffer, sizeof(command_buffer), 
        "ln -sf %s/bin/* /usr/local/bin", 
        node_dir
    );
    print("\n[STEP 2/2] Creating New Symlinks: ");
    
    if (execute_command_real("COMMAND: SYMLINK", command_buffer) != 0) {
        write(2, "Error: Symlink creation failed.\n", 32);
        return 1;
    }

    print("\n==================================================");
    print("\nSUCCESS! Node.js version successfully switched to v");
    print(version);
    print(". Check with 'node -v'.\n");
    print("==================================================\n");

    return 0;
}