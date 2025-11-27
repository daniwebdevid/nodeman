#include "../print/print.h"
#include "install.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include <sys/wait.h>

// --- Helper Functions ---

// Helper to execute command, print details, and get real exit code
static int execute_command_real(const char *cmd_name, const char *cmd_str) {
    print("--------------------------------------------------\n");
    print(cmd_name);
    print(":\n");
    print(cmd_str); 
    print("\n");
    
    // Execute the command via shell
    int result = system(cmd_str);
    
    print("COMMAND EXIT CODE: ");
    char result_str[10];
    snprintf(result_str, sizeof(result_str), "%d", WEXITSTATUS(result));
    print(result_str);
    print("\n");
    
    print("--------------------------------------------------\n");
    
    // Return 0 for success, 1 for error (based on command's exit status)
    return (WEXITSTATUS(result) == 0) ? 0 : 1; 
}


// Helper: Detects system architecture and maps it to Node.js suffix
static int get_arch_suffix(char *arch_suffix, size_t size) {
    FILE *fp;
    char raw_arch[32];
    int read_len;

    // Execute uname -m
    fp = popen("uname -m", "r");
    if (fp == NULL) {
        snprintf(arch_suffix, size, "unsupported");
        return 1;
    }

    // Read the output
    if (fgets(raw_arch, sizeof(raw_arch), fp) == NULL) {
        pclose(fp);
        snprintf(arch_suffix, size, "unsupported");
        return 1;
    }

    pclose(fp);

    // Remove newline
    read_len = strlen(raw_arch);
    if (read_len > 0 && raw_arch[read_len - 1] == '\n') {
        raw_arch[read_len - 1] = '\0';
    }

    // Map Raw Architecture to Node.js Suffix
    if (strcmp(raw_arch, "x86_64") == 0 || strcmp(raw_arch, "amd64") == 0) {
        snprintf(arch_suffix, size, "x64");
    } else if (strcmp(raw_arch, "aarch64") == 0 || strcmp(raw_arch, "arm64") == 0) {
        snprintf(arch_suffix, size, "arm64");
    } else if (strncmp(raw_arch, "arm", 3) == 0) {
        // Fallback for 32-bit ARM
        snprintf(arch_suffix, size, "armv7l"); 
    } else {
        // Unknown architecture - Use snprintf buffer and write(2) for error
        snprintf(arch_suffix, size, "unsupported");
        char err_msg[128];
        int err_len = snprintf(err_msg, sizeof(err_msg), "Error: Architecture '%s' is not supported by Node.js.\n", raw_arch);
        write(2, err_msg, err_len); 
        return 1;
    }
    
    return 0;
}

// --- Main Installation Function ---

int install(const char *version) {

    char arch_suffix[16];
    
    // Get architecture suffix
    if (get_arch_suffix(arch_suffix, sizeof(arch_suffix)) != 0) {
        return 1; // Failed to detect or unsupported architecture
    }
    
    // Use snprintf buffer for combined initial message
    char initial_msg[128];
    snprintf(initial_msg, sizeof(initial_msg), 
        "Starting REAL installation for Node.js version v%s on architecture %s...\n", 
        version, arch_suffix);
    print(initial_msg);


    char command_buffer[512]; 
    char node_dir[128];
    int len;

    // 1. Path Preparation: /usr/local/lib/nodejsXX
    len = snprintf(node_dir, sizeof(node_dir), 
                   "/usr/local/lib/nodejs%s", version);
    if (len < 0 || len >= sizeof(node_dir)) {
        write(2, "Error: Path buffer creation failed.\n", 36);
        return 1;
    }

    // 2. CHECK DIRECTORY EXISTENCE
    len = snprintf(command_buffer, sizeof(command_buffer), "test -d %s", node_dir);
    print("\n[STEP 1/5] Checking target directory: ");
    int check_result = execute_command_real("COMMAND: CHECK", command_buffer);

    if (check_result == 0) { 
        print("RESULT: Node.js directory already exists. Installation aborted.\n");
        return 0; 
    } 
    
    print("RESULT: Directory NOT found. Proceeding with installation.\n");

    // 3. DOWNLOAD AND CD (Dynamic URL)
    len = snprintf(
        command_buffer, sizeof(command_buffer), 
        "cd /usr/local/lib && aria2c -s 16 https://nodejs.org/dist/v%s/node-v%s-linux-%s.tar.xz -o nodejs%s.tar.xz", 
        version, 
        version,
        arch_suffix, 
        version
    );
    print("\n[STEP 2/5] Downloading Tarball: ");
    if (execute_command_real("COMMAND: DOWNLOAD", command_buffer) != 0) {
        write(2, "Error: Download failed.\n", 24);
        return 1;
    }

    // 4. EXTRACT (mkdir first, then tar with --strip-components 1)
    len = snprintf(command_buffer, sizeof(command_buffer), "mkdir -p %s", node_dir);
    execute_command_real("COMMAND: MKDIR", command_buffer); 

    len = snprintf(
        command_buffer, sizeof(command_buffer), 
        "tar -xJf /usr/local/lib/nodejs%s.tar.xz -C %s --strip-components 1", 
        version,
        node_dir
    );
    print("\n[STEP 3/5] Extracting Files: ");
    if (execute_command_real("COMMAND: EXTRACT", command_buffer) != 0) {
        write(2, "Error: Extraction failed.\n", 26);
        return 1;
    }

    // 5. REMOVE TARBALL
    len = snprintf(
        command_buffer, sizeof(command_buffer), 
        "rm /usr/local/lib/nodejs%s.tar.xz", 
        version
    );
    print("\n[STEP 4/5] Removing Tarball: ");
    execute_command_real("COMMAND: REMOVE TAR", command_buffer);


    // 6. SYMLINK DEFERRED
    // Use snprintf buffer for step 6 message
    char step_six_msg[128];
    snprintf(step_six_msg, sizeof(step_six_msg), 
        "\n[STEP 5/5] Symlink deferred. Use 'ndm -c %s' to activate.\n", version);
    print(step_six_msg);

    
    // FINAL MESSAGE (Using snprintf buffer)
    char final_msg[256];
    snprintf(final_msg, sizeof(final_msg), 
        "\n==================================================\n"
        "Node.js v%s successfully INSTALLED. Run 'ndm -c %s' to activate it.\n"
        "==================================================\n",
        version, version
    );
    print(final_msg);

    return 0;
}