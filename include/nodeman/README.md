# NDM Headers Interface 📑

This directory contains the public definitions, global configurations, and function prototypes for **NDM**. These headers act as the technical contract between the CLI entry point (`main.c`) and the internal logic modules.

## 📌 Overview

We separate the interface into two main domains to maintain a clean modular architecture:
1. **`core.h`**: Defines high-level business logic (Install, Use, List, Remove).
2. **`utils.h`**: Defines low-level system helpers (Logging, Arch detection, Command execution).

## 🛠 Header Breakdown

### 🔹 `core.h` (The Business Logic)
This header defines how the user interacts with Node.js versions.

* **Global Constants**: 
    * `NODE_INSTALL_DIR`: Hardcoded to `/opt/nodeman` for production stability.
    * `NDM_VERSION`: Centralized version tracking (currently `2.1.0`).
* **Key Prototypes**:
    * `install()`: Handles the full lifecycle of fetching and verifying Node.js binaries.
    * `use()`: Orchestrates version switching for both User and Global scopes.
    * `list_remote()`: Interface for fetching data from `nodejs.org`.

### 🔹 `utils.h` (The System Helpers)
This header provides standardized tools to interact with the Linux OS.

* **Standardized Error Handling**: Provides the `NdmError` enum to ensure consistent exit codes across all modules.
* **Logging Wrapper**: Macros like `LOG_DEBUG` and functions like `log_info` that respect the `--verbose` flag.
* **Execution Engine**:
    * `command()`: Safe wrapper for `fork/execvp`.
    * `command_output()`: Specialized for capturing command results into buffers (e.g., SHA256 sums).
    * `symlink_force()`: Ensures atomic updates by handling existing link conflicts.

## ⚙️ Coding Standards

* **Include Guards**: Every header is wrapped in `#ifndef HEADER_H` to prevent double-inclusion bloat.
* **Minimal Dependencies**: Headers only include essential system types (`stdbool.h`, `stdio.h`) to keep compilation times near-instant.
* **Thread Safety**: While NDM is single-threaded by design, the utilities avoid global mutable state where possible.

## 🚀 Versioning Strategy
We use **Semantic Versioning** defined in `core.h`. 
- **Major**: Breaking architectural changes.
- **Minor**: New commands or features (like the new installer).
- **Patch**: Bug fixes.

---
*Part of the NDM Project - High Performance Node Management*