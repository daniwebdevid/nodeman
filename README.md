# NDM (Node Manager)

A high-performance, lightweight Node.js version manager written in C for Linux systems.

## 📌 Overview

**NDM** is a native Linux utility designed to manage Node.js installations with zero overhead. Unlike shell-based managers, NDM is compiled for speed and handles version switching through atomic symlink manipulation and environment configuration.

## ✨ Features

* **Zero Shell Overhead**: Written in C for instant execution without interpreting large shell scripts.
* **Dual-Scope Management**: Supports system-wide (global) defaults and user-specific local environments.
* **Security Minded**: Includes path traversal protection and strict privilege validation for root-level operations.
* **Architecture Aware**: Automatically detects system architecture (x64, arm64, armv7l, etc.) to fetch compatible binaries.
* **Atomic Switching**: Updates user environments by re-linking binary paths and updating `.npmrc` automatically.

## 🛠 Tech Stack

* **Language**: C11.
* **Build System**: CMake 3.10+.
* **System Dependencies**: `curl` (for fetching versions), `tar` (for extraction).

## 🚀 Installation

### Prerequisites

* GCC or Clang
* CMake 3.10 or higher
* `curl` and `xz-utils` installed on your Linux distribution

### Building from Source

```bash
git clone https://github.com/yourusername/nodeman.git
cd nodeman
mkdir build && cd build
cmake ..
make
sudo make install

```

## 📖 Usage

| Command | Description |
| --- | --- |
| `ndm install <version>` | Downloads and installs a specific Node.js version (Requires Root). |
| `ndm use <version>` | Switches the current user to the specified version. |
| `ndm use <version> --default` | Sets the global system default version (Requires Root). |
| `ndm list` | Lists versions installed for the current user. |
| `ndm list --system` | Lists globally installed versions. |
| `ndm list --remote` | Fetches available versions from nodejs.org. |
| `ndm remove <version>` | Uninstalls a specific version from the system (Requires Root). |

### Global Options

* `--verbose`: Show detailed execution logs and system commands.
* `-v, --version`: Show NDM version.

## ⚙️ Configuration

To use NDM in user mode, add the following to your `.bashrc` or `.zshrc`:

```bash
export PATH="$HOME/.ndm/bin:$PATH"

```

## 📜 License

This program is free software: you can redistribute it and/or modify it under the terms of the **GNU General Public License** as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but **WITHOUT ANY WARRANTY**; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the [LICENSE](LICENSE) file for more details.
---

## 📜 Changelog

Detailed changes for each release are documented in the [CHANGELOG.md](./CHANGELOG.md) file.

---

**Developed by Dany Saputra**
*Contributions are welcome. Please open an issue to discuss proposed changes.*

---
