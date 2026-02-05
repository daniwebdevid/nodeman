# NDM Core Module

This directory contains the primary business logic for the Node Manager (NDM). These modules handle version acquisition, environment orchestration, and automated lifecycle management.

## Module Overview

The core logic is implemented in C11, focusing on high-performance execution and system safety.

| File | Component | Technical Responsibility |
| :--- | :--- | :--- |
| **`start.c`** | **Lifecycle Init** | Implements directory climbing to detect `.ndmrc` and auto-switch versions. |
| **`install.c`** | **Acquisition** | SHA256 integrity verification with automated retry logic (up to 3 attempts). |
| **`use.c`** | **Env Manager** | Atomic symlink manipulation for user and system-wide scopes. |
| **`list.c`** | **Discovery** | Remote index parsing and local directory scanning via array-based collection. |
| **`remove.c`** | **Cleanup** | Secure recursive deletion with privilege and path traversal validation. |

## Implementation Details (v2.3.0 Updates)

### 1. Project-based Auto Switching (`start.c`)
Introduced in v2.3.0, the `start()` function implements a "directory climbing" algorithm:
- It searches for an `.ndmrc` file in the current working directory.
- If not found, it recursively moves up to the parent directory until it reaches the root (`/`).
- Upon discovery, it parses the version and triggers an automatic `use()` command to sync the environment with the project's requirements.

### 2. Robust Installation & Retries (`install.c`)
The installation module now features an automated recovery flow:
- **Integrity Check**: Compares local `sha256sum` against official `SHASUMS256.txt`.
- **Retry Mechanism**: If a checksum mismatch occurs (e.g., due to network corruption), NDM automatically unlinks the corrupted artifacts and retries the download up to 3 times before exiting with an error.

### 3. Atomic Environment Switching (`use.c`)
Ensures that version transitions are instantaneous:
- **Symlink Force**: Uses `symlink_force` to swap binary links in `/opt/nodeman` or `$HOME/.ndm/bin`.
- **Dual Scope**: Independent management of user-specific environments without affecting the global system default unless explicitly requested via `--default`.

### 4. Memory-Safe Discovery (`list.c`)
Provides programmatic access to version data:
- **Data Arrays**: Returns `char**` arrays for the TUI module to render.
- **Resource Discipline**: Every discovery call is paired with `free_versions_array()` to ensure zero memory leaks during long-running TUI sessions.

## Security Principles

- **Privilege Validation**: Operations affecting `/opt/nodeman` strictly enforce `getuid() == 0`.
- **Path Sanitization**: All version inputs are validated to prevent path traversal attacks (checking for `/`).
- **Process Isolation**: External tools (`tar`, `curl`, `awk`) are executed via secure `fork/execvp` wrappers.

---
*NDM Project - Core Logic Documentation (v2.3.0)*