# NDM Source Entry Point 🚀

This directory contains the main entry point of the **NDM** application. It acts as the traffic controller that parses user input and routes it to the appropriate core logic.

## 📌 Overview

The `main.c` file is the brain of the CLI. It handles initial argument parsing, global flag detection (like `--verbose`), and command dispatching.

## 🛠 File Breakdown

| File | Role | Description |
| :--- | :--- | :--- |
| **`main.c`** | **CLI Orchestrator** | Handles the primary `main()` function and command routing. |

## ⚙️ Execution Flow

When you run an `ndm` command, the following happens in `main.c`:

1.  **Empty Check**: If no arguments are provided, it automatically triggers the `help()` function to guide the user.
2.  **Global Flag Detection**: It scans all arguments for the `--verbose` flag to enable detailed logging across all modules.
3.  **Command Routing**:
    * `install` → Calls `install()` in `core/install.c`.
    * `use` → Calls `use()` in `core/use.c`.
    * `list` → Calls `list()` in `core/list.c`.
    * `remove` → Calls `remove_node_js()` in `core/remove.c`.
    * `-v / --version` → Prints the current version (`2.0.0`).

## 🛡️ Error Handling in Main

- **Missing Arguments**: If a command like `install` or `use` is called without a version number, `main.c` catches it, logs an error, and returns exit code `2`.
- **Exit Codes**: The application follows standard Unix exit codes to ensure it can be used reliably in shell scripts.

---
*Part of the NDM Project - High Performance Node Management*