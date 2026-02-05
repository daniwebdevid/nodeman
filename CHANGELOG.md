# Changelog

All notable changes to the **NDM (Node Manager)** project will be documented in this file.

## [2.3.0] - 2026-02-05

### Added
* [cite_start]**Project-based Auto Switching**: Introduced `start()` lifecycle module that performs directory climbing to detect `.ndmrc` files and automatically syncs the Node.js version[cite: 14, 79, 120].
* [cite_start]**Robust Installation Retries**: Implemented automated recovery flow in `install.c` that retries corrupted downloads up to 3 times after checksum failure[cite: 115, 124, 141].
* [cite_start]**Environment Initialization**: Added `start` command to handle initial environment validation and system setup[cite: 14, 42, 153].
* [cite_start]**File Read Utility**: Added `open_file` to the utility kit for efficient first-line buffer capture, used for state and configuration discovery[cite: 56, 179, 208].
* [cite_start]**TUI Versioning**: Updated TUI interface to reflect the latest version and improved state persistence[cite: 160].

### Changed
* [cite_start]**Environment Persistence**: Migrated PATH configuration from `systemd.conf` to `/etc/security/pam_env.conf` for broader shell compatibility[cite: 57].
* [cite_start]**Memory Safety Discovery**: Refactored `list.c` to pair discovery calls with standardized deallocation via `free_versions_array`[cite: 19, 128].
* [cite_start]**Optimized Download**: Updated `curl` flags to use `-continue-at` for better handling of interrupted downloads[cite: 140].
* [cite_start]**Interface Parity**: Synchronized CLI and TUI modes to utilize identical core functions for behavioral consistency[cite: 93].

### Security
* [cite_start]**Strict Path Sanitization**: Reinforced validation in all version inputs to strictly prohibit path traversal attacks[cite: 130, 133].
* [cite_start]**Production Path Hardening**: Hardcoded `NODE_INSTALL_DIR` to `/opt/nodeman` to ensure system-wide stability and privilege integrity[cite: 11, 40].

---

## [2.2.0] - 2026-01-21

### Added
* **Command Output Capture**: Introduced `command_output` utility to capture and sanitize stdout from system processes, enabling programmatic handling of external tool results.
* **Atomic Symlink Utility**: Added `symlink_force` implementation to provide robust, fail-safe symlink creation by automatically handling existing path conflicts.
* **Dependency Guard**: Added pre-install validation in `install.sh` to ensure `curl`, `tar`, and `xz` are present before execution.
* **Enhanced Remote Filtering**: Improved version discovery logic to handle advanced filtering patterns for remote Node.js releases.

### Changed
* **Internal Command Architecture**: Refactored command execution to use a cleaner fork/exec pattern with explicit pipe management for better resource handling.
* **Path Management Refinement**: Updated installation paths and directory structures for better compliance with Linux filesystem hierarchy standards.
* **Header-Driven Configuration**: Centralized core definitions and utility prototypes to improve compilation efficiency and maintainability.
* **Error Reporting**: Improved terminal output for system-level errors, providing more context when commands or file operations fail.

### Security
* **Hardened Checksum Logic**: Refined the SHA256 verification engine to strictly validate binary integrity before any installation proceeds.
* **Privilege Escalation Checks**: Standardized root privilege validation across all modules affecting the `/opt/nodeman` directory.

---

## [2.1.0] - 2026-01-18

### Added
* **Production Integrity**: Implemented automatic SHA256 checksum verification for all Node.js downloads to ensure binary authenticity.
* **Smart Caching**: Introduced a caching mechanism in /var/cache/nodeman to speed up repeated installations and reduce bandwidth usage.
* **Smart Version Resolution**: Added support for major-only version strings (e.g., ndm install 20), which automatically resolves to the latest stable release.
* **Atomic Symlinking**: Implemented symlink_force logic in use.c to ensure instantaneous version swaps and prevent broken states.
* **Standardized Interface**: Refactored header architecture into core.h and utils.h for better modularity and strict function prototyping.

### Changed
* **Zero-Config Pathing**: Enhanced system integration via /etc/profile.d/nodeman.sh and /etc/environment.d/ for seamless environment management.
* **Optimized Discovery**: Improved list --remote performance by utilizing direct pipes to the official Node.js index.tab.
* **Standardized Exit Codes**: Integrated NdmError enum to provide consistent Unix exit codes.

### Security
* **Path Traversal Protection**: Hardened input validation across all core modules to prevent directory manipulation attacks.
* **Strict Privilege Validation**: Reinforced getuid() == 0 checks for operations requiring root access.

---

## [2.0.0] - 2026-01-15

### BREAKING CHANGES
* **New Command Syntax**: Updated from short flags to descriptive commands.
* **License Change**: Migrated from MIT to GNU GPLv3.
* **Global Path**: Primary installation directory moved to /opt/nodeman.