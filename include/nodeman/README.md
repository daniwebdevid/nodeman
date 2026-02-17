# NDM Headers Interface (v2.5.0)

This directory houses the technical contracts, macro definitions, and global configurations for NDM. These headers are designed with strict idempotency (header guards) to ensure safe inclusion across the CLI, TUI, and Core modules.

## Global Configuration (`core.h`)

| Macro | Value | Description |
| --- | --- | --- |
| `NODE_INSTALL_DIR` | `/opt/nodeman` | The system-wide root for Node.js installations. |
| `NDM_VERSION` | `2.5.0` | Current software iteration used for self-update comparisons. |

## Functional API Contracts

### 1. System Lifecycle & Maintenance

* **`int start(bool *verbose)`**: Triggers project-level `.ndmrc` discovery via directory climbing.
* **`int doctor(bool *verbose)`**: Executes diagnostic suites for $PATH and symlink health.
* **`int status(bool *verbose)`**: Reports recursive disk usage and active state via `lstat`.
* **`int update(bool *verbose)`**: Performs automated self-update via GitHub API integration.
* **`int prune_cache()`**: Purges `/var/cache/nodeman` to reclaim storage.

### 2. Version Management

* **`int install(bool *verbose, char *argv[])`**: Handles acquisition with SHA256 integrity checks.
* **`int uninstall(bool *verbose, char *argv[])`**: **(Added in v2.5.0)** Securely removes installed versions and cleans associated symlinks.
* **`int use(bool *verbose, int argc, char *argv[])`**: Manages atomic switching between global, user, and session scopes.
* **`int list(bool *verbose, int argc, char *argv[])`**: Orchestrates local and remote version discovery.

### 3. Memory & Discovery Utilities

* **`char** get_local_versions_array(...)`**: Programmatic access to installed versions.
* **`void free_versions_array(...)`**: Mandatory deallocation routine for discovery results to prevent memory leaks.

## Utility Abstractions (`utils.h`)

* **`NdmError`**: Centralized enumeration for consistent error propagation across the engine.
* **Process Orchestration**: Contracts for secure `fork/execvp` execution via `command()`.
* **Filesystem Wrappers**: `symlink_force()` for atomic updates and `file_write()` for variadic config management.

## Usage Engineering

All functions follow a consistent return pattern:

* `0`: Success.
* `Negative`: Internal engine error (mapped via `NdmError`).
* `Positive`: System/POSIX error or invalid user input (mapped via `errno`).

---

*NDM Project - Technical Interface Documentation (v2.5.0)*
