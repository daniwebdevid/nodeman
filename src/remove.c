#include "utils/print.h"
#include "remove.h"
#include "utils/command.h"
#include "utils/distro_detection.h"
#include "utils/arch.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 
#include <stdio.h>

// --- KONSTANTA (Sesuai dengan apt.c dan konsisten) ---
#define NDM_INSTALL_DIR         "/opt/nodeman" 
#define NDM_BUILT_DIR           "/opt/nodeman/built"
#define NDM_PACKAGE_NAME_PREFIX "nodeman-nodejs-v" // Package: nodeman-nodejs-v<version>
#define NDM_NODE_BIN_PATH       "/usr/bin/node" // Digunakan untuk check aktif via update-alternatives

// --- PROTOTYPE HELPER ---
static int remove_cache_deb(const char *version); 

// -----------------------------------------------------------------------------
// FUNGSI UTAMA: MENGHAPUS VERSI (FIXED)
// -----------------------------------------------------------------------------
int remove_version(const char *version) {
    print("Starting removal process for v");
    print(version);
    print("...\n");
    
    char command_buffer[768];
    char remove_target[512];
    char arch_suffix[16]; 
    
    int result = 1; // Default fails


    // 1. Distro Dispatcher & Arsitektur
    DetectedDistro os = get_os_type(); 
    
    // Ambil Arsitektur di Awal (Hanya perlu untuk Cache Cleanup)
    if (get_arch_suffix(arch_suffix, sizeof(arch_suffix)) != 0) {
        write(2, "Warning: Failed to determine architecture. Skipping cache cleanup.\n", 67);
        arch_suffix[0] = '\0'; // Kosongkan agar aman
    }

    // -------------------------------------------------------------------------
    // --- LOGIC DEBIAN (Distro-Aware DPKG Removal) ---
    // -------------------------------------------------------------------------
    if (os == OS_DEBIAN) {
        
        // --- 1.A. Tentukan Nama Paket DPKG ---
        snprintf(remove_target, sizeof(remove_target), 
                 "%s%s", NDM_PACKAGE_NAME_PREFIX, version); // nodeman-nodejs-v<version>
        
        // --- 1.B. Pre-Check: Apakah versi ini AKTIF? (UX Safeguard) ---
        // Cek via readlink -f /usr/bin/node (Symlink yang dikontrol update-alternatives)
        char check_active_deb_cmd[512];
        snprintf(check_active_deb_cmd, sizeof(check_active_deb_cmd),
             "readlink -f %s | grep -Fq '/opt/nodeman/v%s/bin/node'", NDM_NODE_BIN_PATH, version);
        
        print("\n[STEP 1/3] Checking if version is currently active (via DPKG control)...\n");
        if (command("COMMAND: CHECK ACTIVE", check_active_deb_cmd) == 0) {
            write(2, "Error: Cannot remove active version. Please use 'ndm -c' to switch first.\n", 74);
            return 1;
        }

        // --- 2. Remove Paket via DPKG (INTI PROSES) ---
        // Ini akan memicu postrm, yang menghapus symlink, dan DPKG menghapus file.
    char dpkg_remove_cmd[530];
    snprintf(dpkg_remove_cmd, sizeof(dpkg_remove_cmd),
         "dpkg --remove %s", remove_target);

        print("\n[STEP 2/3] Executing DPKG package removal...\n");
        if (command("COMMAND: DPKG REMOVE", dpkg_remove_cmd) != 0) {
            char fail_msg[580];
            snprintf(fail_msg, sizeof(fail_msg), 
                     "Error: DPKG removal failed for package %s. Is it installed?\n", remove_target);
            write(2, fail_msg, strlen(fail_msg));
            return 1; 
        }
        
        print("DPKG successfully removed the installed files and symlinks.\n");

        // --- 3. Cache Cleanup (DEB) ---
        print("\n[STEP 3/3] Cleaning up DEB package cache...\n");
        remove_cache_deb(version); // Helper handles arch_suffix
        
        result = 0;
        

    // -------------------------------------------------------------------------
    // --- LOGIC UNIVERSAL (Fallback for Non-Debian) ---
    // -------------------------------------------------------------------------
    } else {
        print("Warning: Detected non-Debian system. Falling back to universal removal method.\n");
        
        // --- 1. Check Active (Universal Method) ---
        char check_active_uni_cmd[512];
        // Asumsi: Symlink universal ada di /usr/local/bin/node
        snprintf(check_active_uni_cmd, sizeof(check_active_uni_cmd),
             "readlink -f /usr/local/bin/node | grep -q '%s/v%s'", NDM_INSTALL_DIR, version);

        print("\n[STEP 1/3] Checking if version is currently active (Universal method)...\n");
        if (command("COMMAND: CHECK ACTIVE (UNI)", check_active_uni_cmd) == 0) {
            write(2, "Error: Cannot remove active version. Change version first.\n", 59);
            return 1;
        }

        // --- 2. Remove Installation Directory (Manual RM) ---
        snprintf(remove_target, sizeof(remove_target), "%s/v%s", NDM_INSTALL_DIR, version);
        snprintf(command_buffer, sizeof(command_buffer), "test -d %s", remove_target);

        print("\n[STEP 2/3] Checking installation directory (Universal)...\n");
        if (command("COMMAND: CHECK EXIST", command_buffer) == 0) {
            snprintf(command_buffer, sizeof(command_buffer), "rm -rf %s", remove_target);
            if (command("COMMAND: REMOVE DIR", command_buffer) != 0) {
                 write(2, "Error: Manual removal failed (Permission denied or system error).\n", 66);
                 return 1;
            }
            print("Successfully removed installation directory.\n");
            result = 0; 
        } else {
             print("Warning: Installation directory not found. Nothing removed.\n");
             result = 0;
        }
    }
    
    // --- FINAL RESULT ---
    if (result == 0) {
        char success_msg[128];
        snprintf(success_msg, sizeof(success_msg),
                 "\n✅ Successfully removed version %s.\n", version);
        print(success_msg);
        return 0;
    } 

    write(2, "Error: Version was not found/removed, or an internal error occurred.\n", 69);
    return 1;
}

// -----------------------------------------------------------------------------
// FUNGSI HELPER: MENGHAPUS CACHE DEB (FIXED DENGAN PANGGILAN ARCH BARU)
// -----------------------------------------------------------------------------
static int remove_cache_deb(const char *version) {
    char command_buffer[512];
    char deb_filename[128];
    char target_cache_path[256];
    char arch_suffix[16];

    // Ambil arsitektur lagi di sini
    if (get_arch_suffix(arch_suffix, sizeof(arch_suffix)) != 0) {
         print("Warning: Failed to determine architecture for cache cleanup.\n");
         return 1;
    }

    // File DEB di cache: nodeman-v<version>-<arch>.deb
    snprintf(deb_filename, sizeof(deb_filename), 
            "nodeman-v%s-%s.deb", version, arch_suffix);
    snprintf(target_cache_path, sizeof(target_cache_path), 
            "%s/%s", NDM_BUILT_DIR, deb_filename);

    snprintf(command_buffer, sizeof(command_buffer), "test -f %s", target_cache_path);
    
    // Cek keberadaan file di cache
    if (command("COMMAND: CHECK CACHE", command_buffer) == 0) {
        snprintf(command_buffer, sizeof(command_buffer), "rm -f %s", target_cache_path);
        if (command("COMMAND: REMOVE CACHE", command_buffer) == 0) {
            print("Cache DEB file successfully removed.\n");
            return 0;
        } else {
            print("Warning: Failed to remove DEB cache file.\n");
            return 1;
        }
    } else {
        print("DEB cache file not found, skipping cache cleanup.\n");
        return 0; 
    }
}
