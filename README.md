# NDM (Node Manager) 🚀

A high-performance, lightweight Node.js version manager written in C for Linux systems.

## 📌 Overview

**NDM** is a native Linux utility designed to manage Node.js installations with zero overhead. Unlike shell-based managers, NDM is compiled for speed and handles version switching through atomic symlink manipulation and environment configuration.

## ✨ Features

* **Zero Shell Overhead**: Written in C for instant execution without interpreting large shell scripts.
* **Dual-Scope Management**: Supports system-wide (global) defaults and user-specific local environments.
* **Security Minded**: Includes path traversal protection and strict privilege validation for root-level operations.
* **Architecture Aware**: Automatically detects system architecture (x64, arm64, armv7l, etc.) to fetch compatible binaries.
* **Atomic Switching**: Updates environments by re-linking binary paths and updating `.npmrc` automatically.
* **Zero-Config PATH**: Automatically handles environment variables via system-wide profile integration.

## 🛠 Tech Stack

* **Language**: C11.
* **Build System**: CMake 3.10+.
* **System Dependencies**: `curl`, `tar`, `xz-utils`, `systemd`.

## 🚀 Installation

### 1. Quick Install (Recommended)
The fastest way to get NDM up and running. This script detects your architecture, installs the binary, and **automatically configures your PATH**.

```bash
curl -sL https://raw.githubusercontent.com/daniwebdevid/nodeman/main/install.sh | sudo bash

```

### 2. Build from Source

If you prefer to compile NDM yourself:

Bash

```
git clone https://github.com/daniwebdevid/nodeman.git
cd nodeman
mkdir build && cd build
cmake ..
make
sudo make install

```

## 📖 Usage

**Command**

**Description**

`ndm install <version>`

Downloads and installs a specific Node.js version (Requires Root).

`ndm use <version>`

Switches the current user to the specified version.

`ndm use <version> --default`

Sets the global system default version (Requires Root).

`ndm list`

Lists versions installed for the current user.

`ndm list --system`

Lists globally installed versions.

`ndm list --remote`

Fetches available versions from nodejs.org.

`ndm remove <version>`

Uninstalls a specific version from the system (Requires Root).

### Global Options

-   `--verbose`: Show detailed execution logs and system commands.
    
-   `-v, --version`: Show NDM version.
    
-   `-h, --help`: Show help message.
    

## ⚙️ How it Works (No Manual Config Required)

NDM is designed to be **Zero-Config**. Once installed via `install.sh`:

-   It creates `/etc/profile.d/nodeman.sh` to manage your shell's `$PATH`.
    
-   It uses `/etc/environment.d/` for systemd-wide environment compatibility.
    
-   **Note**: You might need to restart your terminal or re-login for the first time to apply the global PATH changes.
    

## 📜 License

This program is free software: you can redistribute it and/or modify it under the terms of the **GNU General Public License v3**.

See the [LICENSE](LICENSE) file for more details.

## 📜 Changelog

Detailed changes for each release are documented in the [CHANGELOG.md](CHANGELOG.md) file.

----------

Developed by Dany Saputra

Contributions are welcome. Please open an issue to discuss proposed changes.

----------
