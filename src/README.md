# NDM Source Core

This directory contains the entry point and high-level orchestrators for the Node Manager (NDM). As of version 2.3.0, the source architecture focuses on automated environment initialization and multi-interface support.

## Application Lifecycle (v2.3.0)

The execution flow follows a prioritized sequence to ensure the environment is always synchronized:

1. **System Initialization (`start`)**: Before parsing any CLI arguments, `main.c` invokes the `start()` logic to perform environment validation and project-based version discovery.
2. **Flag and Scope Parsing**: The application identifies global flags (e.g., `--verbose`) and determines if the user is requesting an interactive session or a direct command.
3. **Module Dispatching**: Control is handed off to one of the three primary sub-systems:
    - **Interactive Layer**: The `tui/` module for visual management.
    - **Logic Layer**: The `core/` module for standard CLI operations.
    - **Utility Layer**: The `utils/` module for low-level system tasks.

## Directory Structure

| Component | Role | Technical Responsibility |
| :--- | :--- | :--- |
| **`main.c`** | Entry Point | Handles global state, CLI routing, and lifecycle orchestration. |
| **`core/`** | Logic Engine | Implements installation, removal, and the new `start` lifecycle. |
| **`tui/`** | Interactive UI | Provides an ncurses-based interface for version management. |
| **`utils/`** | System Helpers | Abstractions for file I/O, process forking, and architecture detection. |

## Key Implementation Details

### Automated Lifecycle (`start.c`)
Unlike previous versions, v2.3.0 introduces a mandatory startup phase. The `start()` function performs "directory climbing" to find `.ndmrc` files, ensuring that your Node.js version automatically matches your project's requirements as soon as you execute NDM.

### Command Dispatching logic
The dispatcher in `main.c` uses optimized string comparisons to route user inputs:
- **`ndm tui`**: Suspends standard CLI output to launch the ncurses interactive terminal.
- **`ndm install/use/remove`**: Routes directly to the core engine with sanitized arguments.

### Resource and Error Management
- **Verbosity**: A pointer-based verbose flag is propagated through all modules to control system log output.
- **Exit Codes**: Implements standardized POSIX exit codes (0 for success, 1 for runtime failures, 2 for privilege or input errors).

## Engineering Standards

1. **Non-Blocking Initialization**: The `start()` phase is designed to be lightweight, ensuring no perceived lag during standard CLI usage.
2. **Interface Parity**: Both CLI and TUI modes utilize the exact same core functions to ensure behavioral consistency.
3. **Memory Safety**: Entry points are responsible for triggering the cleanup of dynamic structures used during the execution phase.

---
*NDM Project - Source Architecture Documentation (v2.3.0)*