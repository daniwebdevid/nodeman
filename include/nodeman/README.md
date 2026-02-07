# NDM Source Core (v2.5.0)

This directory manages the entry point and routing for NDM. Version 2.5.0 introduces automated system updates and resource auditing while maintaining the smart-entry transition between CLI and TUI.

## Application Flow (v2.5.0)

1. **Smart Entry**: If executed without arguments (`argc < 2`), the application immediately launches the **Ncurses TUI mode**.
2. **Global Flag Parsing**: Scans for `--verbose` to enable extended logging across all child modules.
3. **Dispatcher**: Routes commands to specialized layers:
    - **Logic Layer**: `install`, `use`, `list`, `remove`.
    - **Maintenance Layer**: `start`, `doctor`, `prune`.
    - **System Layer**: `status`, `update` (New in v2.5.0).

## Directory Structure

| Component | Role | Technical Responsibility |
| :--- | :--- | :--- |
| **`main.c`** | Entry Point | CLI routing, global state propagation, and TUI triggering. |
| **`core/`** | Logic Engine | Implements version control, self-update, and auditing logic. |
| **`tui/`** | Visual Interface | Ncurses state machine for interactive version management. |
| **`utils/`** | System Helpers | Low-level POSIX abstractions for I/O and process orchestration. |

## Key Implementation Details

### 1. System Maintenance & Lifecycle
Version 2.5.0 expands the dispatcher to include critical system-level tasks:
- **`ndm update`**: Dispatches to the self-update engine to keep the binary current via GitHub.
- **`ndm status`**: Triggers a resource audit to report disk usage and active version integrity.
- **`ndm start`**: Manually invokes the directory climbing algorithm for `.ndmrc` discovery.

### 2. Standardized Routing
The dispatcher in `main.c` utilizes optimized `strcmp` routing with standardized exit mapping:
- **Success**: Returns `0`.
- **Runtime Error**: Returns `1`.
- **Invalid Input/Privilege**: Returns `2` (maps to `EINVAL` or `EACCES`).

### 3. Resource Discipline
- **Memory**: The entry point ensures any discovery data arrays are localized to their respective commands to prevent TUI-mode leaks.
- **Verbosity**: Uses a pointer-based boolean propagation (`&verbose`) to control log output intensity throughout the call stack.

---
*NDM Project - Source Architecture Documentation (v2.5.0)*