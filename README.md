# NDM (Node Manager) 🚀

A high-performance, lightweight Node.js version manager written in C for Linux systems.

## 📌 Overview

**NDM** is a native Linux utility designed to manage Node.js installations with zero overhead. Built with C11, it provides instant version switching through atomic symlink manipulation and system-wide environment integration.

## ✨ Features

* **Zero Shell Overhead**: Native C binary execution, no heavy shell scripts or slow interpretation.
* **Dual-Scope Management**: Manage system-wide (global) defaults and user-specific local environments.
* **Production Integrity**: Built-in **SHA256 checksum verification** for every Node.js download.
* **Smart Caching**: Implements a caching mechanism in `/var/cache/nodeman` to speed up repeated installs.
* **Architecture Mapping**: Automatically detects and maps system architecture (x64, arm64, etc.) to official Node.js distributions.
* **Atomic Switching**: Uses `symlink_force` logic to ensure binary swaps are clean and never leave broken states.
* **Zero-Config PATH**: Automatically handles environment variables via `/etc/profile.d/`—no manual `.bashrc` editing required.

## 🛠 Tech Stack

* **Language**: C11.
* **Build System**: CMake 3.10+.
* **Core Helpers**: `curl`, `tar`, `xz-utils`, `systemd`.

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

Installs Node.js (e.g., `ndm install 20` or `20.11.0`).

`ndm use <version>`

Switches the current user to the specified version.

`ndm use <version> --default`

Sets the global system default version (Requires Root).

`ndm list`

Lists installed versions for the current user.

`ndm list --system`

Lists globally installed versions.

`ndm list --remote`

Fetches available versions from nodejs.org.

`ndm remove <version>`

Uninstalls a specific version (Requires Root).

### Global Options

-   `--verbose`: Show detailed logs, system commands, and network output.
    
-   `-v, --version`: Show NDM version.
    
-   `-h, --help`: Show help guide.
    

## ⚙️ How it Works (Zero-Config)

NDM is designed to be production-ready out of the box:

1.  **Environment**: It integrates with `/etc/profile.d/nodeman.sh` and `/etc/environment.d/` for seamless PATH management.
    
2.  **Security**: Enforces `getuid() == 0` for system-wide changes and includes path traversal protection.
    
3.  **Smart Resolution**: If you type `ndm install 20`, NDM automatically finds the latest stable release of Node 20.
    

> **Note**: After the first installation, you may need to restart your terminal or re-login to apply global environment changes.

## 📜 License

This program is free software: you can redistribute it and/or modify it under the terms of the **GNU General Public License v3**.

See the [LICENSE](https://www.google.com/search?q=LICENSE) file for more details.

----------

Developed by Dany Saputra

High-performance Node.js management for Linux.

----------


