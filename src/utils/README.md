# NDM Utilities Kit 🛠️

This directory houses the shared utility functions used across **NDM**. These modules abstract system-level operations like process forking, file I/O, and terminal logging to keep the core logic clean and readable.

## 📌 Utility Overview

These utilities are designed to be "plug-and-play" within the NDM ecosystem. They handle the "dirty work" of interacting with the Linux OS.

## 🛠 Helper Modules

| File | Tool Type | Description |
| :--- | :--- | :--- |
| **`command.c`** | **Process Exec** | Wrapper for `fork`, `execvp`, and `popen`. Handles silent vs verbose execution. |
| **`print.c`** | **Logger** | Pretty-prints colored logs (`INFO`, `WARN`, `ERROR`) to the terminal. |
| **`file.c`** | **File I/O** | Simple formatted wrapper for `fopen` to write/append config files. |
| **`arch.c`** | **System Info** | Maps `uname` results to official Node.js architecture strings. |

## ⚙️ How They Work

### 🚀 Command Execution (`command.c`)
- **`command()`**: Uses `fork()` and `execvp()` to run system tools like `tar`, `ln`, and `rm`. 
- **Output Management**: If `--verbose` is off, it redirects `stdout` and `stderr` to `/dev/null` to keep the UI clean.
- **`command_output()`**: Specifically designed to capture the first line of a command's output (like HTTP status codes from `curl`).

### 📝 Smart Logging (`print.c`)
- Uses ANSI escape codes for colored output.
- **Blue [INFO]**: Only visible if `--verbose` is active.
- **Yellow [WARN]** & **Bold Red [ERROR]**: Always visible and printed to `stderr` for better error tracking.

### 📂 File Management (`file.c`)
- **`file_write()`**: A variadic function (like `printf`) that opens a file, writes formatted content, and closes it immediately.
- Primarily used for updating `.npmrc` and the `active` version file.

### 💻 Arch Mapping (`arch.c`)
- Converts Linux machine names (e.g., `x86_64`) into Node.js compatible strings (e.g., `x64`).
- Supports `arm64`, `armv7l`, `ppc64le`, and `s390x`.

## 📜 Principles

1. **No Bloat**: Every utility solves exactly one problem.
2. **Error Handling**: Most utilities return status codes that the core logic can use to trigger clean exits.
3. **Safety**: Redirects output to `/dev/null` when not needed to avoid cluttering the user's terminal during background tasks.

---
*Part of the NDM Project - High Performance Node Management*