#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "nodeman/utils.h"

int symlink_force(bool *verbose, const char *target, const char *linkpath) {
    // 1. Coba buat symlink langsung
    if (symlink(target, linkpath) == 0) {
        return 0; // Berhasil jika file belum ada
    }

    // 2. Jika gagal karena file sudah ada (EEXIST), kita 'force'
    if (errno == EEXIST) {
        // Hapus link/file yang lama (efek dari -f)
        if (unlink(linkpath) != 0) {
            log_error("Failed to ");
            return -1;
        }
        // Coba buat symlink lagi setelah dihapus
        if (symlink(target, linkpath) == 0) {
            return 0;
        }
    }

    log_error("Gagal membuat symlink");
    return -1;
}