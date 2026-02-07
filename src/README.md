# NDM Source Core

This directory contains the entry point and high-level orchestrators for NDM. In version 2.4.0, the source architecture is optimized for diagnostic health, cache management, and seamless transition between TUI and CLI modes.

## Application Lifecycle (v2.4.0)

The execution flow in `main.c` follows a strict dispatching hierarchy:

1. **Smart Entry Check**: If the application is executed without arguments (`argc < 2`), it immediately launches the **Interactive TUI** mode.
2. **Global Flag Parsing**: Scans for the `--verbose` flag to propagate logging behavior across all child modules.
3. **Command Dispatching**: Routes inputs to specialized logic blocks:
    - **Logic Layer (`core/`)**: Standard operations (install, use, list, remove).
    - **Maintenance Layer**: New diagnostic and cleanup tools (`doctor`, `prune`).
    - **UI Layer (`tui/`)**: Ncurses-based visual management.

## Directory Structure

| Component | Role | Technical Responsibility |
| :--- | :--- | :--- |
| **`main.c`** | Entry Point | CLI routing, global state management, and TUI triggering. |
| **`core/`** | Logic Engine | Primary operations including the new `doctor` and `prune` modules. |
| **`tui/`** | Visual Interface | Ncurses state machine for interactive version control. |
| **`utils/`** | System Abstraction | POSIX wrappers for file I/O, process execution, and symlinks. |

## Key Implementation Details (v2.4.0)

### Maintenance and Diagnostics
Version 2.4.0 introduces two critical maintenance commands dispatched from `main.c`:
- **`ndm doctor`**: Triggers environment health checks, including $PATH integrity and symlink validation.
- **`ndm prune`**: Safely purges the system cache in `/var/cache/nodeman` to reclaim disk space.

### Advanced Dispatching Logic
The dispatcher in `main.c` has been refined for better CLI/TUI parity:
- **`ndm start`**: Explicitly triggers the directory climbing logic to sync with project-level `.ndmrc` files.
- **`ndm use`**: Now supports `--session` or `-s` flags for transient shell environments via stdout evaluation.

### Resource Discipline
- **Exit Codes**: Adheres to standardized exit mapping (e.g., `2` for invalid arguments, `1` for runtime errors, `0` for success).
- **Error Propagation**: Uses `errno` and custom `log_error` wrappers to provide detailed feedback during failed system calls.

## Engineering Standards

1. **Interface Flexibility**: Designed to work as both a direct binary for automation (CLI) and a visual tool for developers (TUI).
2. **Memory Safety**: Entry points ensure that any dynamic data required for version listing is cleared before process termination.
3. **Zero-Overhead Dispatch**: Command matching is performed using optimized string comparisons to ensure near-instantaneous startup.

---
*NDM Project - Source Architecture Documentation (v2.4.0)*