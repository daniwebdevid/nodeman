# Changelog

All notable changes to the **NDM (Node Manager)** project will be documented in this file.

## [2.6.1] - 2026-02-18

### Added

* **Security-First Logic Reordering**: Refactored `src/core/use.c` to perform path traversal checks and version normalization *before* any shell evaluation or export.
* **Enhanced Version Normalization**: Updated the `use` workflow to handle "v" prefixes (e.g., `ndm use v20`). The resolution logic now prioritizes finding the latest matching version in local storage (`/opt/nodeman`) instead of querying remote repositories.

### Changed

* **Codebase Optimization**: Removed `src/core/status.c` and its references in `CMakeLists.txt` to reduce binary footprint and simplify the core command set.
* **Session Evaluation Flow**: Adjusted the `--session` (or `-s`) flag behavior to only output shell exports after all internal validation and normalization steps are successfully completed.
* **Source Formatting**: Cleaned up internal string formatting and memory handling in `use_user` and `install` modules for better readability and C11 compliance.

### Fixed

* **Validation Race Condition**: Fixed a logic flaw where session exports could potentially interfere with system-wide paths when using global flags in package managers.
* **Symbol Linker Consistency**: Standardized function signatures in `core.h` to match the latest implementation of major version discovery.

### Added

* **Command: uninstall**: Introduced `src/core/uninstall.c` to provide a dedicated deallocation routine for removing Node.js binaries while preserving global configuration states.
* **Static Library Integration**: Migrated build system to use architecture-specific static libraries (`libform.a`, `libncurses.a`, `libtinfo.a`) in `libs/${MY_ARCH}` for improved portability across different Linux distributions.
* **Architecture-Aware Build**: Enhanced `CMakeLists.txt` with `CMAKE_SYSTEM_PROCESSOR` detection to automatically link correct library binaries for `arm64` and `x86_64`.
* **Threading Support**: Explicitly linked `pthread` and `dl` in the utility layer to support upcoming asynchronous operations and dynamic library loading.

### Changed

* **TUI Input Handler**: Refactored `handle_input` in `src/tui/handler.c` with improved terminal state management (`def_prog_mode`, `endwin`) during command execution.
* **Installer Logic**: Updated `install()` signature to support extended argument passing for recursive installation routines.
* **Build System**: Hardened compiler flags and optimized the linker order to prevent unresolved symbol errors during static linking.

### Fixed

* **Terminal State**: Resolved an issue where the terminal would not correctly restore its state when returning from a CLI action (Install/Use) back to the TUI interface.
* **Linker Order**: Fixed dependency chain in `target_link_libraries` ensuring `libform` is linked before `libncurses`.

### Security

* **Buffer Hardening**: Increased scrutiny on `snprintf` buffer sizes across the TUI layer to prevent truncation during version rendering.
* **Path Validation**: Reinforced directory existence checks before initiating `uninstall` or `remove` operations to prevent orphan symlinks.

---

## [2.5.0] - 2026-02-7

### Added

* **Command: doctor**: Diagnostic tool in `doctor.c` using `readlink()` and `access()` to verify symlink integrity and `$PATH` health.
* **Command: prune**: Cleanup utility in `prune.c` using `unlink()` to remove cached `.tar.xz` files from `/var/cache/nodeman`.
* **Command: start**: Recursive directory climbing logic in `start.c` to detect `.ndmrc` files in parent directories.
* **Atomic Symlinking**: Integrated `symlink_force` in `use.c` to handle atomic link updates.

### Changed

* **TUI Architecture**: Refactored interface into modular components: `main_menu.c`, `tui_render.c`, and `handler.c`.
* **File I/O Layer**: Hardened `file_write` and `open_file` in `utils/file.c` for atomic configuration handling.
* **Core Dispatcher**: Updated main entry point to route `doctor`, `prune`, and `start` commands.

### Security

* **Path Traversal Protection**: Reinforced input validation with `strchr(argv[0], '/')` checks in `install` and `remove` modules.
* **Privilege Enforcement**: Standardized `getuid() == 0` checks across all system-modifying modules.
* **Download Recovery**: Implemented 3-cycle retry logic in `install.c` via `goto` labels for checksum failure recovery.

## [2.4.0] - 2026-02-07

### Added

* **TUI Dynamic Rendering**: Implemented real-time list updates and version metadata rendering in TUI mode.
* **Build System Optimization**: Updated `CMakeLists.txt` to include modular core components (`prune.c`, `doctor.c`).

### Changed

* **Buffer Safety**: Implemented stricter buffer management in TUI event handlers to prevent overflows during rapid input.
* **Documentation Sync**: Updated Utility Layer specs to reflect API changes in the core dispatcher.

## [2.3.0] - 2026-02-05

### Added

* **Project-based Auto Switching**: Introduced `start()` module for directory climbing and `.ndmrc` discovery.
* **File Read Utility**: Added `open_file` to `utils.c` for efficient first-line buffer capture.
* **TUI Versioning**: Updated TUI interface for state persistence and version display.

### Changed

* **Environment Persistence**: Migrated PATH configuration to `/etc/security/pam_env.conf` for shell compatibility.
* **Memory Safety**: Refactored `list.c` with `free_versions_array` for explicit heap deallocation.
* **Optimized Download**: Added `-continue-at` flags to `curl` operations.

### Security

* **Production Path Hardening**: Hardcoded `NODE_INSTALL_DIR` to `/opt/nodeman` for privilege integrity.

---

## [2.2.0] - 2026-01-21

### Added

* **Command Output Capture**: Added `command_output` in `utils.c` to capture string outputs from system commands (e.g., SHA256 sums).
* **Symlink Force Utility**: Introduced `src/utils/symlink.c` with `symlink_force` logic.
* **Dependency Validation**: `install.sh` now verifies `tar`, `xz`, and `curl` before execution.
* **Multi-Filter Remote Search**: Enhanced `list_remote` to support multiple version filters simultaneously.

### Changed

* **Flat Directory Structure**: Migrated Node.js installations to `/opt/nodeman`.
* **Standardized Logging**: Updated internal logging to use pointer-based verbosity flags.

### Security

* **Checksum Verification Engine**: Integrated SHA256 verification using `awk` and `sha256sum`.
* **Input Normalization**: Added version normalization to prevent path-based attacks.

---

## [2.1.0] - 2026-01-18

### Added

* **Production Integrity**: Implemented automatic SHA256 checksum verification for all Node.js downloads.
* **Smart Caching**: Introduced caching in `/var/cache/nodeman`.
* **Smart Version Resolution**: Support for major-only version strings (e.g., `ndm install 20`).
* **Standardized Interface**: Refactored header architecture into `core.h` and `utils.h`.

### Changed

* **Zero-Config Pathing**: Enhanced system integration via `/etc/profile.d/nodeman.sh`.
* **Standardized Exit Codes**: Integrated `NdmError` enum for consistent Unix exit codes.

---

## [2.0.0] - 2026-01-15

### ⚠️ BREAKING CHANGES

* **New Command Syntax**: Updated from short flags to descriptive commands (`install`, `use`, `list`).
* **License Change**: Migrated from MIT to GNU GPLv3.
* **Global Path**: Primary installation directory moved to `/opt/nodeman`.

### ✨ Added

* **Dual-Scope Switching**: Introduced local user scope and `--default` for global system scope.
* **Verbosity Control**: Added `--verbose` flag for internal logging.
* **Extended Architecture Support**: Added support for `s390x` and `ppc64le`.

---

## [1.0.0] - 2025-06-19

* Initial release of **ndm**.
* Support for basic install, list, and change version via flags.
* Basic architecture detection (x64/arm64).