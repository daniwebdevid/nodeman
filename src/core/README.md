# NDM Core Components 🧠

This directory contains the core logic for **NDM**. Each module handles a specific command-line action, ensuring a modular and maintainable architecture.

## 📌 Overview

The core logic is written in C11 and interacts directly with the Linux filesystem and official Node.js distributions. All functions are designed to be called by the main CLI entry point.

## 🛠 Modules Breakdown

| File | Purpose | Key Features |
| :--- | :--- | :--- |
| **`install.c`** | Handles Node.js installation. | Arch detection, integrity verification, and tarball extraction. |
| **`use.c`** | Manages version switching. | Atomic symlinking, dual-scope (user/global), and `.npmrc` updates. |
| **`list.c`** | Versions discovery. | Remote fetching via `curl`, local directory scanning, and filtering. |
| **`remove.c`** | Uninstallation logic. | Privilege validation, path traversal protection, and clean removal. |
| **`help.c`** | Interface guidance. | Formatted CLI instructions and usage examples. |

## ⚙️ Logic Details

### 📥 Installation (`install.c`)
- Fetches binaries directly from `https://nodejs.org/dist/`.
- Validates system architecture (x64, arm64, etc.) before downloading.
- Requires **Root Privileges** as it writes to the global installation directory.

### 🔄 Switching (`use.c`)
- **User Mode**: Creates local symlinks in `$HOME/.ndm/bin` and updates the user's `$PATH` configuration.
- **Global Mode (`--default`)**: Replaces the system-wide source to update the default Node.js version for all users.

### 🔍 Listing (`list.c`)
- **Local**: Scans `$HOME/.ndm`.
- **System**: Scans the global `NODE_INSTALL_DIR`.
- **Remote**: Uses `curl` and `awk` to parse the official Node.js version table.

### 🗑 Removal (`remove.c`)
- Performs a safety check to prevent accidental deletion of system directories.
- Uses a recursive removal process to ensure no leftover artifacts remain.

## 📜 Security Principles

* **Privilege Escalation**: Functions that modify global state (install/remove/default use) strictly enforce `getuid() == 0` checks.
* **Path Sanitization**: All version inputs are checked for `/` to prevent path traversal attacks.
* **Atomic Operations**: Environment updates (like switching versions) are done via symlink swaps to prevent broken states.

---
*Part of the NDM Project - High Performance Node Management*