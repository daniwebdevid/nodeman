# NDM Headers Interface 📑

This directory contains the public definitions and function prototypes for the **NDM** project. These headers act as the contract between the CLI entry point and the underlying logic.

## 📌 Overview

We separate the interface into two main domains to keep the project modular:
1. **`core.h`**: Business logic (Install, Use, List, Remove).
2. **`utils.h`**: System helpers (Logging, Arch detection, Command execution).

## 🛠 Header Breakdown

### 💎 `core.h` (The Logic)
Defines the primary actions of the Node Manager.

* **Global Config**: Defines `NODE_INSTALL_DIR` (`/opt/nodeman`) and `NDM_VERSION` (`2.0.0`).
* **Prototypes**:
    * `install()`: The entry for downloading and setting up Node.js.
    * `use()`: The dispatcher for switching versions (user vs global).
    * `list()`: Functions for local, system, and remote version discovery.

### 🔧 `utils.h` (The Helpers)
Defines internal tools and cross-platform abstractions.

* **Error Codes**: Provides `NdmError` enum for standardized error handling (e.g., `NDM_ERR_SUDO`, `NDM_ERR_NETWORK`).
* **Logging**: Macros and functions for colored terminal output (`log_info`, `log_warn`, `log_error`).
* **System Tools**:
    * `command()`: Wrapper for executing shell commands safely.
    * `get_arch()`: Architecture mapping tool.
* **File I/O**: `file_write()` for persistent configuration updates.

## 📜 Coding Standard

- **Include Guards**: All headers use `#ifndef HEADER_H` to prevent double-inclusion errors.
- **Dependency Minimalization**: Headers only include what is absolutely necessary (`stdbool.h`, `stdio.h`) to keep compilation fast.
- **Documentation**: Every function prototype is preceded by a comment block explaining its purpose and requirements.

---
*Part of the NDM Project - High Performance Node Management*