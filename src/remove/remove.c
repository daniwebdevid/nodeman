#include "../print/print.h"
#include "remove.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 
#include <stdio.h>

int remove_version(const char *version) {
    print("Starting removal process...\n");
    
    // Buffer yang aman
    char target_path[256];        
    char check_active_cmd[300];   
    char check_exist_cmd[300];    
    char remove_cmd[300];         
    
    // version di sini sudah TIDAK mengandung 'v' (misal: "22" atau "22.9.0")

    // 1. Tentukan path instalasi
    // Path direktori instalasi TIDAK memiliki 'v' (nodejs22)
    snprintf(target_path, sizeof(target_path),
             "/usr/local/lib/nodejs%s", version);

    // 2. Cek apakah versi yang akan dihapus sedang aktif
    // Perintah: readlink -f /usr/local/bin/node | grep 'nodejs<version>'
    snprintf(check_active_cmd, sizeof(check_active_cmd),
             "readlink -f /usr/local/bin/node | grep 'nodejs%s'", version);

    // Jika grep berhasil (exit code 0), berarti versi ini sedang aktif.
    if (system(check_active_cmd) == 0) {
        char error_msg[128];
        // PERBAIKAN: Hapus 'v' yang hardcoded
        snprintf(error_msg, sizeof(error_msg), 
                 "Error: Cannot remove active version %s. Change version first.\n", version);
        write(2, error_msg, strlen(error_msg));
        return 1;
    }
    
    // 3. Konfirmasi path tersebut memang ada
    snprintf(check_exist_cmd, sizeof(check_exist_cmd), "ls %s", target_path);

    if (system(check_exist_cmd) != 0) {
        char not_found_msg[512]; 
        // PERBAIKAN: Hapus 'v' yang hardcoded
        snprintf(not_found_msg, sizeof(not_found_msg), 
                 "Error: Version %s is not installed at %s.\n", version, target_path);
        write(2, not_found_msg, strlen(not_found_msg));
        return 1;
    }

    // 4. Eksekusi perintah penghapusan (rm -rf)
    snprintf(remove_cmd, sizeof(remove_cmd),
             "rm -rf %s", target_path);

    print("Executing: ");
    print(remove_cmd);
    print("\n");
    
    int result = system(remove_cmd);
    
    if (result == 0) {
        char success_msg[128];
        // PERBAIKAN: Hapus 'v' yang hardcoded
        snprintf(success_msg, sizeof(success_msg),
                 "Successfully removed version %s.\n", version);
        print(success_msg);
        return 0;
    } else {
        write(2, "Error: Removal failed (Permission denied or system error).\n", 59);
        return 1;
    }
}