# NDM Core Module

This directory contains the primary business logic for the Node Manager. These modules handle version acquisition, environment orchestration, and filesystem state management.

## Module Overview

The core logic is implemented in C11 and follows a modular design where each file corresponds to a specific CLI command or lifecycle stage.

| File | Component | Technical Responsibility |
| :--- | :--- | :--- |
| **`install.c`** | Version Acquisition | SHA256 integrity verification, architecture mapping, and automated caching. |
| **`use.c`** | Environment Manager | Atomic symlink manipulation for user and system-wide scopes. |
| **`list.c`** | Discovery Engine | Remote index parsing and local directory scanning via array-based collection. |
| **`remove.c`** | Cleanup Utility | Secure recursive deletion with privilege and path traversal validation. |
| **`help.c`** | Interface Doc | CLI usage documentation and command-line examples. |

## Implementation Details

### Installation & Integrity (`install.c`)
The installation process is designed for production stability:
- **Checksum Validation**: Downloads the official `SHASUMS256.txt` and uses `awk` to extract the specific hash for the detected architecture. It then performs a `sha256sum` check before extraction.
- **Cache Management**: Binaries are cached in `/var/cache/nodeman` to reduce bandwidth consumption for repeated installations.
- **Resolution**: Supports major-version aliases (e.g., `20` -> `20.x.x`) by resolving the latest stable release from the remote index.

### Atomic Version Switching (`use.c`)
NDM ensures that version switching does not leave the system in a broken state:
- **Symlink Force**: Uses the `symlink_force` utility to atomically swap binary links in `/usr/local/bin` (system) or `$HOME/.ndm/bin` (user).
- **Dual Scope**: Supports independent user environments while maintaining a global system default if requested via the `--default` flag.

### Version Discovery (`list.c`)
The discovery module provides two methods of data retrieval:
- **Standard Output**: Prints formatted lists directly to the terminal.
- **Data Arrays**: Provides `get_local_versions_array` and `get_remote_versions_array` for programmatic access, specifically utilized by the TUI module.
- **Filtering**: Automatically excludes internal management directories such as `bin`, `active`, and `config` during local scans.

## Security Principles

- **Privilege Validation**: Commands affecting the global state (`install`, `remove`, `use --default`) enforce `getuid() == 0` checks.
- **Input Sanitization**: Strictly prohibits path traversal by checking for `/` characters in version strings before filesystem operations.
- **Process Isolation**: External tools like `tar` and `curl` are executed via `fork/execvp` wrappers to maintain controlled execution environments.

---
*NDM Project - Core Logic Documentation*