# NDM Core Module (v2.4.0)

This directory contains the primary business logic and system management modules for NDM. The v2.4.0 update introduces diagnostic tools and cache optimization for production environments.

## Module Overview

| File | Component | Technical Responsibility |
| :--- | :--- | :--- |
| **`start.c`** | **Lifecycle Init** | Implements directory climbing to detect `.ndmrc` and auto-switch versions. |
| **`doctor.c`** | **Diagnostics** | **New in v2.4.0**. Validates $PATH integrity, active symlinks, and environment health. |
| **`prune.c`** | **Cache Manager** | **New in v2.4.0**. Purges downloaded tarballs from `/var/cache/nodeman` to reclaim disk space. |
| **`use.c`** | **Env Manager** | Handles atomic version switching with new shell session support (`--session`). |
| **`install.c`** | **Acquisition** | SHA256 verification with automated 3-attempt retry logic. |
| **`list.c`** | **Discovery** | Remote index parsing and local directory scanning via array-based collection. |

## Implementation Details (v2.4.0 Updates)

### 1. Environment Diagnostics (`doctor.c`)
The `doctor` module performs a comprehensive health check of the NDM environment:
- **Path Validation**: Scans `$PATH` for duplicate or missing NDM binary entries.
- **Link Integrity**: Verifies that active symlinks in `~/.ndm/bin` are pointing to valid installations.
- **State Check**: Validates the consistency of the `active` version file in the user's home directory.

### 2. Cache Optimization (`prune.c`)
Designed for long-term server maintenance:
- **Storage Recovery**: Safely unlinks all cached `.tar.xz` and `.txt` files from the system cache directory.
- **Privilege Enforcement**: Requires root access to ensure `/var/cache/nodeman` is managed securely.

### 3. Advanced Version Switching (`use.c`)
Updated to support transient environments:
- **Shell Session Mode**: Using the `--session` or `-s` flag, NDM outputs an `export PATH` string that can be evaluated directly by the shell, bypassing permanent symlink changes.
- **Atomic Overwrites**: Continues to utilize `symlink_force` for stable, race-condition-free version transitions.

### 4. Robust Directory Climbing (`start.c`)
Refined "climbing" logic using the new `open_file` utility to efficiently parse project-level `.ndmrc` configurations from current directory up to root.

## Security & Engineering Standards

- **Standardization**: All modules return `NdmError` compatible integers for consistent exit codes.
- **Privilege Scoping**: Functions affecting system-wide paths (`prune`, `install`, `use --default`) strictly validate root ownership.
- **Memory Discipline**: All discovery operations are paired with deallocation procedures to prevent leaks during TUI sessions.

---
*NDM Project - Core Logic Documentation (v2.4.0)*