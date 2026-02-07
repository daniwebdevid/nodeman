# NDM Utilities Kit

This directory houses the shared utility functions and system abstractions for NDM. These modules handle low-level operations such as process orchestration, filesystem manipulation, and hardware detection.

## Module Overview (v2.4.0)

| File | Tool Type | Responsibility |
| :--- | :--- | :--- |
| **`command.c`** | **Process Exec** | Wrapper for `fork` and `execvp`. Handles silent vs. verbose execution modes. |
| **`file.c`** | **File I/O** | Formatted writes and first-line stream reading for configuration management. |
| **`print.c`** | **Logger** | ANSI-colored terminal logging (INFO, WARN, ERROR) with verbosity control. |
| **`symlink.c`** | **Link Manager** | Atomic symlink creation with forced overwrite capabilities. |
| **`arch.c`** | **System Info** | Maps `uname` machine names to official Node.js architecture strings. |

## Functional Specifications

### 1. Enhanced File I/O (`file.c`)
The v2.4.0 update introduces robust reading capabilities to support automated environment discovery:
- **`file_write()`**: A variadic wrapper for `fprintf` that ensures files are opened, written, and flushed atomically.
- **`open_file()`**: **New in v2.4.0**. Reads the primary line of a file (e.g., `.ndmrc` or `active` state) into a buffer, automatically stripping newline characters.

### 2. Secure Process Orchestration (`command.c`)
Decouples the core logic from shell dependencies:
- **`command()`**: Executes system binaries directly via `execvp`, bypassing `/bin/sh` for better performance and security.
- **Output Capture**: `command_output()` utilizes pipes to capture specific command results, essential for SHA256 verification.

### 3. Atomic Link Management (`symlink.c`)
Crucial for the `use` command to prevent race conditions or "file exists" errors:
- **`symlink_force()`**: Implements an `unlink`-then-`symlink` pattern to ensure the target link always points to the desired version without manual intervention.

### 4. Smart Logging (`print.c`)
- **Verbosity Levels**: `log_info` messages are suppressed unless the `--verbose` flag is active.
- **Stream Redirection**: Critical errors and warnings are dispatched to `stderr` to ensure they are captured even if `stdout` is piped.

## Engineering Principles

- **POSIX Compliance**: Uses standard system headers to ensure portability across different Linux distributions.
- **Zero-Dependency**: No external libraries required beyond standard C and ncurses (for TUI).
- **Error Propagation**: Most utilities return integer status codes, allowing the Core module to handle failures gracefully.

---
*NDM Project - Utility Layer Documentation (v2.4.0)*