#include "utils/print.h"
#include "utils/arch.h"
#include "utils/command.h"
#include "install.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include <sys/wait.h>
#include "command.h"
#include "utils/distro_detection.h"
#include "build/apt.h" 


int install(const char *version) {

    char arch_suffix[16];
    
    // Get architecture suffix
    if (get_arch_suffix(arch_suffix, sizeof(arch_suffix)) != 0) {
        return 1; // Failed to detect or unsupported architecture
    }
    
    // Use snprintf buffer for combined initial message
    char initial_msg[128];
    snprintf(initial_msg, sizeof(initial_msg), 
        "Starting installation for Node.js version v%s on architecture %s...\n", 
        version, arch_suffix);
    print(initial_msg);

    // Ensure base directory exists
    char base_dir_cmd[64];
    snprintf(base_dir_cmd, sizeof(base_dir_cmd), "mkdir -p /opt/nodeman");

    print("Ensuring base directory /opt/nodeman exists...\n");
    // Create base directory if it doesn't exist
    if (command("COMMAND: BASE_DIR_CHECK", base_dir_cmd) != 0) {
        write(2, "Error: Failed to create base directory /opt/nodeman.\n", 54);
        return 1;
    }


    char command_buffer[512]; 
    char node_dir[128];
    int len;

    print("\n[STEP 0/5] Detecting OS and installing dependencies...\n");
    
    // call get_os_type from distro_detection.c
    DetectedDistro os = get_os_type(); // <<< DEKLARASI PERTAMA: INI TETAP

    // Dispatching Logic based on detected OS
    if (os == OS_DEBIAN) {
        print("RESULT: Detected Debian/Ubuntu family. Using APT.\n");
        // This function will run 'apt-get install -y aria2' via command()
        if (install_build_dependencies(os) != 0) {
            write(2, "Error: Failed to install APT dependencies.\n", 43);
            return 1;
        }

    } else if (os == OS_ARCH) {
        print("RESULT: Detected Arch family. Using Pacman.\n");
        // this function will run 'pacman -S --noconfirm aria2' via command()
        if (install_build_dependencies(os) != 0) {
            write(2, "Error: Failed to install Pacman dependencies.\n", 46);
            return 1;
        }
        
    } else {
        print("RESULT: OS/Distro not supported or unknown. Continuing without automatic dependency check.\n");
        // in this case, we assume user has aria2 installed manually
    }

    // 1. Path Preparation: /opt/nodeman/v<version>
    len = snprintf(node_dir, sizeof(node_dir), 
                   "/opt/nodeman/v%s", version);
    if (len < 0 || len >= sizeof(node_dir)) {
        write(2, "Error: Path buffer creation failed.\n", 36);
        return 1;
    }

    // 2. CHECK DIRECTORY EXISTENCE
    len = snprintf(command_buffer, sizeof(command_buffer), "test -d %s", node_dir);
    print("\n[STEP 1/5] Checking target directory: ");
    int check_result = command("COMMAND: CHECK", command_buffer);

    if (check_result == 0) { 
        print("RESULT: Node.js directory already exists. Installation aborted.\n");
        return 0; 
    } 
    
    print("RESULT: Directory NOT found. Proceeding with installation.\n");

    // 3. DOWNLOAD AND CD (Dynamic URL)
    len = snprintf(
        command_buffer, sizeof(command_buffer), 
        "aria2c -s 16 https://nodejs.org/dist/v%s/node-v%s-linux-%s.tar.xz -o v%s.tar.xz -d %s", 
        version, 
        version,
        arch_suffix, 
        version,
        node_dir
    );
    print("\n[STEP 2/5] Downloading Tarball: ");
    if (command("COMMAND: DOWNLOAD", command_buffer) != 0) {
        write(2, "Error: Download failed.\n", 24);
        return 1;
    }

    // 4. EXTRACT (mkdir first, then tar with --strip-components 1)
    len = snprintf(
        command_buffer, sizeof(command_buffer), 
        "tar -xJf %s/v%s.tar.xz -C %s --strip-components 1", 
        node_dir,  // path tarball is in: /opt/nodeman/v<version>/v<version>.tar.xz
        version,
        node_dir
    );
    print("\n[STEP 3/5] Extracting Files: ");
    if (command("COMMAND: EXTRACT", command_buffer) != 0) {
        write(2, "Error: Extraction failed.\n", 26);
        return 1;
    }

    // 5. REMOVE TARBALL
    len = snprintf(
        command_buffer, sizeof(command_buffer), 
        "rm %s/v%s.tar.xz",
        node_dir,  // path tarball is in: /opt/nodeman/v<version>/v<version>.tar.xz 
        version
    );
    print("\n[STEP 4/5] Removing Tarball: ");
    command("COMMAND: REMOVE TAR", command_buffer);


    // --- STEP 6: DISTRO-SPECIFIC PACKAGING ATAU SYMLINK DEFERRED ---
    print("\n[STEP 5/5] Finalizing installation...\n"); // Mengubah penomoran step

    // DetectedDistro os = get_os_type(); // <<< ERROR FATAL HILANG: Variabel 'os' sudah dideklarasikan di Baris 48
    int final_step_result = 0;
    int is_packaged = 0; // Flag for whether packaging was done


    if (os == OS_DEBIAN) {
        print("RESULT: Detected Debian/Ubuntu. Dispatching to DEB packaging...\n");
        // Call create_deb_package from src/build/apt.c
        final_step_result = create_deb_package(version, node_dir); 
        is_packaged = 1;

    } else if (os == OS_ARCH) {
        print("RESULT: Detected Arch Linux. Dispatching to PACMAN build...\n");
        // example: final_step_result = create_pacman_package(version, node_dir);
        // final_step_result = create_pacman_package(version, node_dir); 
        // is_packaged = 1;
        
    }
    
    // Check final step result
    if (final_step_result != 0) {
        // If packaging/build failed for the detected distro 
        write(2, "Error: Build/Packaging failed for the detected distro. Manual cleanup may be needed.\n", 83);
        return 1; 
    }


    // Tampilkan pesan akhir sesuai hasil: Packaging sukses ATAU Symlink deferral
    if (!is_packaged) { 
        // Jika Distro tidak didukung/dikenal, kembali ke pesan deferred lama
        char step_six_msg[128];
        snprintf(step_six_msg, sizeof(step_six_msg), 
            "RESULT: Generic install completed. Symlink deferred. Use 'ndm -c %s' to activate.\n", version);
        print(step_six_msg);
    } else {
        // Jika packaging berhasil, berikan pesan sukses packaging (Telah membuat DEB/RPM/dsb.)
        char success_msg[128];
        snprintf(success_msg, sizeof(success_msg), 
            "✅ SUCCESS: %s package creation completed in current directory. Use 'ndm -c %s' to activate.\n", 
            (os == OS_DEBIAN ? "DEB" : "Distro"), version);
        print(success_msg);
    }

    return 0;
}