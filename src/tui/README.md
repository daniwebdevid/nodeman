# NDM Source Core

This directory contains the main entry point and orchestrator for the Node Manager (NDM). It serves as the gateway that routes user requests to either the standard CLI modules or the interactive Terminal User Interface (TUI).

## Application Flow

The execution flow is divided into three distinct phases:
1. **Interrupt & Flag Parsing**: `main.c` scans for global arguments (e.g., `--verbose`, `--version`) and initializes the system state.
2. **Interface Dispatching**: Based on the command provided, the application switches between:
    - **CLI Mode**: Direct execution of core functions for scripts and CI/CD.
    - **TUI Mode**: Interactive ncurses-based management.
3. **Execution**: Control is handed to the specialized modules in `src/core/`, `src/utils/`, or `src/tui/`.

## Directory Overview

| Component | Role | Description |
| :--- | :--- | :--- |
| **`main.c`** | Entry Point | Handles CLI routing, global flag parsing, and exit code management. |
| **`core/`** | Engine | Contains the primary logic for installation, switching, and removal. |
| **`tui/`** | Interactive UI | Contains the ncurses-based state machine and rendering logic. |
| **`utils/`** | System Helpers | Low-level wrappers for POSIX operations and hardware detection. |

## Implementation Details

### Command Routing (`main.c`)
The dispatcher uses optimized string comparisons to map CLI arguments to their respective internal functions:
- **`install` / `use` / `list` / `remove`**: Direct mapping to core engine logic.
- **`tui`**: New entry point that launches the `main_menu()` interactive loop.

### Global Verbosity Control
A global `verbose` boolean is initialized in `main.c` and passed by reference across all modules. This ensures that system-level logs (such as `curl` output or `tar` extraction details) are only exposed when explicitly requested.

### Exit Codes and Error Handling
To ensure production reliability, NDM implements standardized exit codes:
- `0`: Successful operation.
- `1`: Runtime error (system failure, network timeout).
- `2`: User error (invalid arguments, insufficient privileges).

## Principles

1. **Modular Routing**: `main.c` remains lean by delegating all heavy lifting to sub-directories.
2. **Operational Flexibility**: Supports both non-interactive (CLI) and interactive (TUI) usage patterns within a single binary.
3. **Resource Efficiency**: No heavy data structures are initialized before the routing phase is completed.

---
*NDM Project - Source Architecture Documentation*