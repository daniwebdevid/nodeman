# Changelog

All notable changes to the **NDM (Node Manager)** project will be documented in this file.

## [2.2.0] - 2026-01-21

### Added
* [cite_start]**Command Output Capture**: Added a specialized utility `command_output` in `utils.c` to capture and process string outputs from system commands (e.g., SHA256 sums). [cite: 971, 998, 1039]
* [cite_start]**Symlink Force Utility**: Introduced `src/utils/symlink.c` with `symlink_force` logic to handle atomic link updates and prevent conflicts during version switching. [cite: 972, 1001]
* [cite_start]**Dependency Validation**: The `install.sh` script now explicitly verifies the presence of system dependencies (`tar`, `xz`, `curl`) before starting the installation. [cite: 1002, 1003, 1004]
* [cite_start]**Multi-Filter Remote Search**: Enhanced `list_remote` to support multiple version filters simultaneously, allowing for more precise discovery. [cite: 694, 700]

### Changed
* [cite_start]**Flat Directory Structure**: Migrated Node.js installations to a flatter directory structure in `/opt/nodeman` for better performance and easier management. [cite: 1113, 1114]
* [cite_start]**Improved Install Logic**: Refactored `install.c` to use the new caching and integrity verification flow, ensuring production-grade stability. [cite: 1040, 1042]
* [cite_start]**Standardized Logging**: Updated internal logging to use pointer-based verbosity flags for consistent state management across modules. [cite: 991, 993]
* [cite_start]**Documentation Update**: Overhauled `README.md` and internal documentation to reflect the new architecture and zero-config principles. [cite: 917, 944, 1017]

### Security
* [cite_start]**Checksum Verification Engine**: Finalized the integration of SHA256 verification using `awk` and `sha256sum` to validate Node.js binaries against official signatures. [cite: 913, 1097, 1101]
* [cite_start]**Input Normalization**: Added version normalization in the installation process to prevent path-based attacks and ensure version consistency. [cite: 1048, 1055, 1060]

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