# NDM (Node Manager)

NDM is a high-performance, lightweight Node.js version manager written in C11. It is engineered for Linux environments that require a native, zero-overhead alternative to shell-based managers. Version 2.3.0 introduces automated project-based environment synchronization and an interactive TUI.

## Key Features

- **Automated Lifecycle**: Implements a recursive "directory climbing" algorithm that detects `.ndmrc` files and automatically synchronizes the Node.js environment upon execution.
- **Interactive TUI**: Built-in ncurses interface for visual management of local, system, and remote versions.
- **Production-Grade Integrity**: Mandatory SHA256 checksum verification with an automated 3-attempt retry mechanism to recover from corrupted downloads.
- **Zero Runtime Overhead**: Compiled as a native binary to eliminate the latency and dependency overhead associated with shell-script wrappers.
- **Dual-Scope Orchestration**: Independent management of user-local environments and system-wide global defaults via atomic symlinking.
- **Smart Architecture Mapping**: Automatically detects host architecture (x64, arm64, armv7l) to fetch compatible binary distributions.

## Project Structure

The codebase follows a modular architecture for maximum maintainability:

- **`src/`**: Main entry point and system lifecycle orchestration.
- **`src/core/`**: Core logic for version acquisition, removal, and the `start` sequence.
- **`src/tui/`**: State machine and rendering engine for the interactive terminal interface.
- **`src/utils/`**: Low-level POSIX wrappers for process execution, atomic file I/O, and link management.
- **`include/`**: Technical contracts and global configurations.

## Installation

### Prerequisites
Requires `gcc`, `cmake`, `libncurses-dev`, `libcurl`, and `xz-utils`.

### Build and Install
```bash
git clone https://github.com/daniwebdevid/nodeman.git
cd nodeman
mkdir build && cd build
cmake ..
make
sudo make install

```

## Usage

### 1. Interactive TUI

Launch the visual management console:

Bash

```
ndm

```

_Navigation: `1-3` to switch views, `j/k` to scroll, `Enter` to install/use, `q` to quit._

### 2. CLI Mode

Direct execution for terminal use or CI/CD pipelines:

**Command**

**Action**

`ndm install <version>`

Installs Node.js with integrity verification.

`ndm use <version>`

Switches version for the current user scope.

`ndm use <version> --default`

Sets global system version (Requires Sudo).

`ndm list`

Displays local and system installations.

`ndm list --remote`

Fetches available versions from nodejs.org.

`ndm remove <version>`

Uninstalls a version and purges associated symlinks.

## Technical Implementation

-   **Standard**: C11 / POSIX compliant.
    
-   **Auto-Switching**: The `start()` function is triggered at the entry phase to check for project-specific configuration files.
    
-   **Memory Safety**: Uses dynamic array collection with strictly enforced deallocation via `free_versions_array`.
    
-   **Security**: Enforces privilege validation for system-wide changes and implements path traversal protection on all version inputs.
    

## License

Licensed under the GNU General Public License v3.

----------

**Developed by Dany Saputra** | _High-Performance Node Management for Linux_

