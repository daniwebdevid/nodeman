# NDM (Node Manager)

NDM is a high-performance, lightweight Node.js version manager written in C11. It is engineered for Linux environments that require a native, zero-overhead alternative to shell-based managers. Version 2.3.0 introduces project-based environment synchronization and an interactive TUI.

## Key Features

- **On-Demand Project Sync**: Provides the `start` command to manually trigger a recursive "directory climbing" algorithm. It detects `.ndmrc` files and switches the environment to the required version.
- **Smart Entry Dispatcher**: Automatically launches the TUI if no arguments are provided, ensuring quick access for interactive management.
- **Interactive TUI**: A full-featured ncurses interface for visual management of local, system, and remote versions.
- **Production-Grade Integrity**: Mandatory SHA256 checksum verification with a 3-attempt retry mechanism to recover from corrupted network streams.
- **Zero Runtime Overhead**: Compiled as a native binary, eliminating the latency and dependency overhead of shell-script wrappers.
- **Dual-Scope Orchestration**: Independent management of user-local environments and system-wide global defaults via atomic symlinking.

## Project Structure

- **`src/`**: Main entry point and system lifecycle orchestration.
- **`src/core/`**: Core logic for version acquisition, removal, and the `start` sequence.
- **`src/tui/`**: State machine and rendering engine for the interactive terminal interface.
- **`src/utils/`**: Low-level POSIX wrappers for process execution, atomic file I/O, and link management.
- **`include/`**: Technical contracts and global configurations.

## Installation

Requires `gcc`, `cmake`, `libncurses-dev`, `libcurl`, and `xz-utils`.

```bash
git clone https://github.com/daniwebdevid/nodeman.git
cd nodeman
mkdir build && cd build
cmake .. && make
sudo make install

```

## Usage

### 1. Interactive TUI

NDM enters TUI mode by default if no arguments are passed:

Bash

```
ndm

```

_Alternatively, use `ndm tui` to open the interface explicitly._

### 2. CLI Mode & Manual Sync

Direct execution for terminal use or CI/CD pipelines:

**Command**

**Action**

**`ndm start`**

**Triggers `.ndmrc` discovery (climbing to root) and switches version.**

`ndm install <v>`

Installs a specific version with integrity verification.

`ndm use <v>`

Switches version for the current user scope.

`ndm list`

Displays local and system installations.

`ndm remove <v>`

Uninstalls a version and purges associated symlinks.

## Technical Implementation

-   **Standard**: C11 / POSIX compliant.
    
-   **Manual Discovery**: The `start()` function is explicitly called via the `start` command to sync project configurations.
    
-   **Memory Safety**: Uses dynamic array collection with strictly enforced deallocation.
    
-   **Security**: Enforces privilege validation and path traversal protection.
    

## License

Licensed under the GNU General Public License v3.

----------

**Developed by Dany Saputra** | _High-Performance Node Management for Linux_

