# NDM Headers Interface

This directory contains the primary header files and technical contracts for the Node Manager (NDM). These interfaces ensure type safety and consistent function signatures across the CLI, TUI, and core engine modules.

## Directory Structure

| File | Domain | Responsibility |
| :--- | :--- | :--- |
| **`core.h`** | Business Logic | Version management, filesystem paths, and data collection. |
| **`utils.h`** | System Abstraction | Low-level process execution, logging, and hardware detection. |
| **`tui.h`** | Interface State | State definitions and view logic for the ncurses interface. |

## Detailed Header Specification

### 1. core.h (Core Engine Interface)
Defines the operational lifecycle of Node.js versions within the system.

- **Global Definitions**:
    - `NODE_INSTALL_DIR`: Hardcoded to `/opt/nodeman` for production-grade stability.
    - `NDM_VERSION`: Semantic versioning identifier, currently **2.3.0**.
- **Functional Prototypes**:
    - `start()`: **New in v2.3.0**. Handles initial environment validation and system setup.
    - `install()`: Orchestrates the full lifecycle of fetching, verifying, and extracting binaries.
    - `use()`: Dispatches version switching for both user-local and system-global scopes.
    - `get_local_versions_array()`: Programmatically retrieves installed versions as a dynamic `char**` array.
    - `free_versions_array()`: Standardized memory deallocation procedure to prevent memory leaks.

### 2. utils.h (System Utility Interface)
Provides a standardized wrapper for POSIX-compliant system operations.

- **Error Management**: Defines the `NdmError` enumeration to map internal failures to standardized exit codes (e.g., `NDM_ERR_SUDO`, `NDM_ERR_NETWORK`).
- **Process Orchestration**: 
    - `command()`: Secure `fork/execvp` wrapper for external tools like `tar` and `curl`.
    - `command_output()`: Captures specific output lines for data validation, such as SHA256 hashes.
- **Atomic Operations**: `symlink_force()` ensures environment updates are instantaneous and robust against existing link conflicts.

### 3. tui.h (TUI State & Logic)
Defines the structures required for the interactive ncurses session.

- **State Persistence**: Utilizes the `TuiState` struct to track current view modes, cursor position, and scroll offsets.
- **View Enumeration**: Manages the UI state machine via `VIEW_LOCAL`, `VIEW_SYSTEM`, and `VIEW_REMOTE`.

## Engineering Standards

- **Include Guarding**: Every header utilizes `#ifndef` guards to prevent recursive inclusion overhead.
- **Header Autonomy**: Designed to be self-contained, requiring only standard POSIX libraries (`stdbool.h`, `stdio.h`) for compilation.
- **Resource Discipline**: Every allocation-providing interface is paired with a clear deallocation procedure to ensure production-grade memory safety.

---
*NDM Project - Technical Interface Documentation (v2.3.0)*