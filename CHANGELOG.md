# Changelog 📜

All notable changes to the **NDM (Node Manager)** project will be documented in this file.

## [2.1.0] - 2026-01-18

### ✨ Added
* **Production Integrity**: Implemented automatic **SHA256 checksum verification** for all Node.js downloads to ensure binary authenticity.
* **Smart Caching**: Introduced a caching mechanism in `/var/cache/nodeman` to speed up repeated installations and reduce bandwidth usage.
* **Smart Version Resolution**: Added support for major-only version strings (e.g., `ndm install 20`), which automatically resolves to the latest stable release.
* **Atomic Symlinking**: Implemented `symlink_force` logic in `use.c` to ensure instantaneous version swaps and prevent broken states.
* **Standardized Interface**: Refactored header architecture into `core.h` and `utils.h` for better modularity and strict function prototyping.

### 🔧 Changed
* **Zero-Config Pathing**: Enhanced system integration via `/etc/profile.d/nodeman.sh` and `/etc/environment.d/` for seamless environment management.
* **Optimized Discovery**: Improved `list --remote` performance by utilizing direct pipes to the official Node.js `index.tab`.
* **Standardized Exit Codes**: Integrated `NdmError` enum to provide consistent Unix exit codes (0: Success, 1: Runtime Error, 2: Privilege/User Error).

### 🛡️ Security
* **Path Traversal Protection**: Hardened input validation across all core modules to prevent directory manipulation attacks.
* **Strict Privilege Validation**: Reinforced `getuid() == 0` checks for operations requiring root access (install, remove, and global default switching).

---

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