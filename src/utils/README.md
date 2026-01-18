# NDM Core Components 🧠

This directory contains the core business logic of **NDM**. These modules define how Node.js versions are installed, switched, and managed within the Linux filesystem.

## 📌 Logic Overview

The core modules interact with the utilities in `src/utils` to perform high-level operations. Every command available in the CLI maps directly to a function here.

## 🛠 Modules Breakdown

| File | Purpose | Key Features |
| :--- | :--- | :--- |
| **`install.c`** | Version Acquisition | Integrity check (SHA256), architecture detection, and cached downloads. |
| **`use.c`** | Environment Switching | Atomic symlinking (User/Global) and `.npmrc` configuration. |
| **`list.c`** | Discovery | Local directory scanning and remote version fetching via `index.tab`. |
| **`remove.c`** | System Cleanup | Safe recursive deletion and privilege validation. |
| **`help.c`** | UI/UX Guidance | Standardized CLI documentation and command examples. |

## ⚙️ Core Implementation Details

### 📥 Production-Grade Installation (`install.c`)
- **Integrity**: Uses `sha256sum` and `awk` to verify downloaded tarballs against official Node.js SHASUMS.
- **Caching**: Implements a caching mechanism in `/var/cache/nodeman` to prevent redundant downloads.
- **Normalization**: Supports major-only input (e.g., `ndm install 20`) by resolving it to the latest stable release.

### 🔄 Dual-Scope Switching (`use.c`)
- **User Scope**: Links binaries to `$HOME/.ndm/bin/` for isolated user environments.
- **System Scope (`--default`)**: Re-links the global default version in `/opt/nodeman/default` (requires root).
- **Atomic**: Leverages `symlink_force` to ensure version swaps are instantaneous and never leave broken links.

### 🔍 Discovery Engine (`list.c`)
- **Remote**: Parses `https://nodejs.org/dist/index.tab` using pipes to provide a fast, searchable list of versions.
- **Local**: Efficiently scans `NODE_INSTALL_DIR` while ignoring internal management folders like `bin` or `active`.

## 📜 Principles

1. **Security**: Every module that modifies the system (install/remove) enforces `getuid() == 0` checks.
2. **Efficiency**: Avoids over-engineering by using standard system calls and optimized pipes for remote data.
3. **Safety**: Path traversal protection is baked into every input validation step.

---
*Part of the NDM Project - High Performance Node Management*