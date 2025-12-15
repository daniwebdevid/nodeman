#include "utils/command.h"
#include "utils/print.h"
#include "change.h"
#include "utils/arch.h" 
#include "utils/distro_detection.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/wait.h>
#include <stdio.h>


int change_version(const char *version) {

    // --- NEW STEP 0: CHECK DISTRO ---
    if (get_os_type() != OS_DEBIAN) {
        // if not Debian-based, fallback to old universal symlink method
        write(2, "Error: Change system currently only supports Debian/APT packaging.\n", 67);
        return 1; 
    }

    print("Starting version change process to v");
    print(version);
    print(" (via DPKG)...\n");

    char command_buffer[512]; 
    char deb_arch_suffix[16]; // Mengganti nama arch_suffix untuk kejelasan
    int check_result;

    // 0. Get Debian Architecture Suffix
    // FIX: Menggunakan get_deb_arch_suffix() untuk mendapatkan 'amd64' atau 'arm64'
    if (get_deb_arch_suffix(deb_arch_suffix, sizeof(deb_arch_suffix)) != 0) {
        write(2, "Error: Failed to detect Debian architecture.\n", 45);
        return 1;
    }


    // 1. Path Preparation: DEB File Path
    // expecting file name: nodeman-v<version>-<amd64>.deb
    char deb_filename[128];
    char deb_path[256];
    
    // Filename: nodeman-v<version>-<deb_arch>.deb
    snprintf(deb_filename, sizeof(deb_filename), 
             "nodeman-v%s-%s.deb", version, deb_arch_suffix); 
    
    // Path: /opt/nodeman/built/nodeman-v<version>-<deb_arch>.deb
    snprintf(deb_path, sizeof(deb_path), 
             "/opt/nodeman/built/%s", deb_filename);


    // 2. CHECK DEB FILE EXISTENCE (test -f)
    snprintf(command_buffer, sizeof(command_buffer), "test -f %s", deb_path);
    print("\n[STEP 1/2] Checking Cached DEB File: ");
    check_result = command("COMMAND: CHECK", command_buffer);

    if (check_result != 0) { 
        // Jika deb file tidak ditemukan
        print("\n❌ FAILED: Package file ");
        print(deb_path);
        print(" not found. Please run 'ndm install ");
        print(version);
        print("' again to rebuild the package.\n");
        return 1; 
    } 
    
    print("RESULT: DEB package found. Proceeding with DPKG.\n");


    // 3. DPKG INSTALL (Re-install untuk memicu postinst)
    // FIX KRITIS: Tambahkan 'sudo' dan '--ignore-depends=update-alternatives'
    //           untuk mengatasi bug dependency sistem yang dialami user.
    snprintf(command_buffer, sizeof(command_buffer), 
             "sudo dpkg -i --force-overwrite --force-conflicts --ignore-depends=update-alternatives %s", 
             deb_path
    );
    
    print("\n[STEP 2/2] Dpkg re-install/switch: ");
    
    if (command("COMMAND: DPKG SWITCH", command_buffer) != 0) {
        write(2, "Error: dpkg install failed to switch version. Trying to fix dependencies...\n", 75);
        // Tambahkan perintah perbaikan dependency jika gagal (meskipun kita mengabaikannya)
        command("COMMAND: APT FIX", "sudo apt --fix-broken install");
        
        write(2, "Error: dpkg switch failed after dependency fix attempt.\n", 56);
        return 1;
    }

    print("\n==================================================");
    print("\n✅ SUCCESS! Node.js version successfully switched to v");
    print(version);
    print(". Check with 'node -v', 'npm -v', 'npx -v', and 'corepack -v'.\n");
    print("==================================================\n");

    return 0;
}