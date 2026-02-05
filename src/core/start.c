#include <linux/limits.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#include "nodeman/core.h"
#include "nodeman/utils.h"

int start(bool *verbose) {
    char current_dir[PATH_MAX];
    char config_path[PATH_MAX];
    char version_buffer[64];

    // 1. Ambil folder aktif (tempat user ketik command)
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        log_error("Gagal dapet current directory");
        return 1;
    }

    while (1) {
        // 2. Target file: path/sekarang/.ndmrc
        snprintf(config_path, sizeof(config_path), "%s/.ndmrc", current_dir);

        // 3. Cek fisik filenya
        if (access(config_path, F_OK) == 0) {
            log_info(true, "Found config: %s", config_path);
            
            // 4. Pake fungsi lu buat ambil versinya
            if (open_file(version_buffer, sizeof(version_buffer), config_path)) {
                
                // 5. install() butuh char* argv[], kita buat array lokal
                char *args[] = { version_buffer, NULL }; 
                return use(verbose, 1, args);
            }
        }

        // 6. Logika Climbing (Naik ke parent)
        char *last_slash = strrchr(current_dir, '/');
        
        // Kalau gak ada slash lagi atau sudah di root
        if (last_slash == NULL || strlen(current_dir) <= 1) {
            break;
        }

        // Berhenti di root "/"
        if (last_slash == current_dir) {
            current_dir[1] = '\0'; 
        } else {
            *last_slash = '\0'; // Potong folder terakhir
        }
    }

    log_error("No .ndmrc found in this directory tree.");
    return 1;
}