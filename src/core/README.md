# NDM Core Module (v2.5.0)

This directory contains the primary logic for NDM. Version 2.5.0 introduces automated self-updates and comprehensive system auditing.

## Module Overview

| File | Component | Technical Responsibility |
| :--- | :--- | :--- |
| **`update.c`** | **Self-Updater** | **New in v2.5.0**. Fetches latest releases from GitHub and executes automated installer. |
| **`status.c`** | **Resource Audit** | **New in v2.5.0**. Calculates disk usage for `/opt/nodeman` and cache size. |
| **`start.c`** | **Lifecycle Init** | Refined in v2.5.0 with strict buffer validation for `.ndmrc`. |
| **`doctor.c`** | **Diagnostics** | Validates $PATH integrity and symlink health. |

## Implementation Details (v2.5.0 Updates)

### 1. Self-Update Engine (`update.c`)
The `update` command automates the NDM lifecycle:
- **GitHub Integration**: Fetches release metadata using `curl` and parses tags via `jq`.
- **Atomic Execution**: Downloads and executes the official `install.sh` if a newer version is detected.
- **Safety**: Registered `atexit` cleanup to purge temporary workspace in `/tmp/nodeman`.

### 2. Storage & Health Audit (`status.c`)
Provides a snapshot of the NDM footprint:
- **Recursive Sizing**: Implements `get_dir_size` using `lstat` to calculate actual disk usage without following symlinks.
- **Threshold Warnings**: Warns the user if the cache exceeds 1GB, suggesting an `ndm prune`.

### 3. Hardened Climbing Logic (`start.c`)
- **Buffer Safety**: Added `memset` and explicit length checks for `.ndmrc` content to prevent garbage data injection into the `use` flow.

---
*NDM Project - Core Logic Documentation (v2.5.0)*