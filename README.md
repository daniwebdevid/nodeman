# NDM (Node Manager)

NDM is a high-performance, lightweight Node.js version manager written in C11. It is engineered for Linux environments that require a native, zero-overhead alternative to shell-based managers. Version 2.3.0 introduces project-based environment synchronization and an interactive TUI.

## Key Features

- **On-Demand Project Sync**: Provides the `start` command to manually trigger a recursive "directory climbing" algorithm. It detects `.ndmrc` files and switches the environment to the required version.
- **Smart Entry Dispatcher**: Automatically launches the TUI if no arguments are provided, ensuring quick access for interactive management.
- **Production-Grade Integrity**: Mandatory SHA256 checksum verification with an automated 3-attempt retry mechanism to recover from corrupted network streams.
- **Interactive TUI**: A full-featured ncurses interface for visual management of local, system, and remote versions.
- **Zero Runtime Overhead**: Compiled as a native binary, eliminating the latency and dependency overhead of shell-script wrappers.

## Installation

### 1. Quick Install (Recommended)
For a fast, automated installation, execute the following command:

```bash
curl -fsSL https://github.com/daniwebdevid/nodeman/releases/download/v2.3.0/install.sh | sudo bash

```

### 2. Build from Source

If you prefer to compile NDM manually, ensure you have `gcc`, `cmake`, `libncurses-dev`, `libcurl`, and `xz-utils` installed.

```bash
git clone https://github.com/daniwebdevid/nodeman.git
cd nodeman
mkdir build && cd build
cmake .. && make
sudo make install

```

## Usage

### Interactive TUI

NDM enters TUI mode by default if no arguments are passed:

```bash
ndm

```
or you can open from application menu and search with keyword "nodeman"

### CLI Mode & Manual Sync

| Command | Action |
| --- | --- |
| **`ndm start`** | **Triggers `.ndmrc` discovery (climbing to root) and switches version.** |
| `ndm install <v>` | Installs a specific version with integrity verification. |
| `ndm use <v>` | Switches version for the current user scope. |
| `ndm list` | Displays local and system installations. |
| `ndm remove <v>` | Uninstalls a version and purges associated symlinks. |

## Project Structure

* **`src/`**: Main entry point and system lifecycle orchestration.
* **`src/core/`**: Core logic for version acquisition and the `start` sequence.
* **`src/utils/`**: Low-level POSIX wrappers for process execution and file I/O.
* **`include/`**: Technical contracts and global configurations.

## Technical Implementation

* **Standard**: C11 / POSIX compliant.
* **Manual Discovery**: The `start()` function is explicitly called via the `start` command to sync project configurations.
* **Memory Safety**: Uses dynamic array collection with strictly enforced deallocation.
* **Security**: Enforces privilege validation and path traversal protection.

## License

Licensed under the GNU General Public License v3.

---

**Developed by Dany Saputra** | *High-Performance Node Management for Linux*
