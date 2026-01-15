# Changelog 📜

All notable changes to the **NDM (Node Manager)** project will be documented in this file.

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