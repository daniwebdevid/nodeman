# Changelog 📜

All notable changes to the **NDM (Node Manager)** project will be documented in this file.

## [2.1.0] - 2026-01-18

### ✨ Added

-   **Production Integrity**: Implementasi verifikasi **SHA256 checksum** otomatis untuk setiap download Node.js guna menjamin keamanan binary.
    
-   **Smart Caching**: Sistem caching baru di `/var/cache/nodeman` untuk mempercepat instalasi ulang versi yang sama tanpa download ulang.
    
-   **Smart Version Resolution**: Sekarang user bisa install cukup dengan major version (contoh: `ndm install 20`), dan NDM akan otomatis mencarikan versi stabil terbaru.
    
-   **Atomic Symlinking**: Logic `symlink_force` di `use.c` untuk memastikan perpindahan versi bersifat instan dan anti-corrupt (aman dari broken links).
    
-   **Centralized Headers**: Refaktor struktur header ke `core.h` dan `utils.h` untuk standarisasi kontrak antar modul.
    

### 🔧 Changed

-   **Path Management**: Integrasi otomatis dengan `/etc/profile.d/nodeman.sh` dan `/etc/environment.d/` untuk manajemen `$PATH` yang lebih "Zero-Config".
    
-   **Optimized Discovery**: Proses `list --remote` sekarang lebih kencang karena menggunakan pipe langsung ke `index.tab` Node.js.
    
-   **Standardized Exit Codes**: Definisi `NdmError` di `utils.h` untuk kode keluar yang lebih konsisten (0: Success, 1: Runtime Error, 2: Privilege/User Error).
    

### 🛡️ Security

-   **Path Traversal Protection**: Penguatan validasi input di setiap modul core untuk mencegah serangan manipulasi directory.
    
-   **Privilege Enforcement**: Pengecekan `getuid() == 0` yang lebih ketat pada fungsi-fungsi yang memodifikasi sistem (install, remove, global use).
-----

## [2.0.0] - 2026-01-15

### ⚠️ BREAKING CHANGES
* **New Command Syntax**: Updated from short flags to descriptive commands:
    * `-i` / `--install` → `install <version>`
    * `-c` / `--change` → `use <version>`
    * `-l` / `--list` → `list --remote`
    * `-iL` / `--installed-list` → `list`
* **License Change**: Migrated from **MIT** to **GNU GPLv3**.
* **Global Path**: Primary installation directory moved to `/opt/nodeman`.

### ✨ Added
* **Dual-Scope Switching**: Introduced `ndm use <version>` for local user scope and `ndm use <version> --default` for global system scope.
* **Verbosity Control**: Added `--verbose` flag to display internal system commands and network logs.
* **Security Hardening**: Added root privilege checks for system-wide commands and path traversal protection.
* **Extended Architecture Support**: Added support for `s390x` and `ppc64le`.
* **Internal Modularization**: Refactored C code into `/core` and `/utils` for better maintainability.

### 🔧 Fixed
* Fixed broken symlinks for `npm` and `npx` during version swaps.
* Improved error handling with custom `NdmError` codes.

---

## [1.0.0] - 2025-06-19
* Initial release of **ndm**.
* Support for basic install, list, and change version via flags.
* Basic architecture detection (x64/arm64).