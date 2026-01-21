# Changelog

All notable changes to the **NDM (Node Manager)** project will be documented in this file.

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