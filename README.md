# NDM (Node Manager) v2.6.1


NDM is a high-performance, lightweight Node.js version manager written in C11. It is engineered for Linux environments that require a native, zero-overhead alternative to shell-based managers. By utilizing POSIX system calls and direct binary execution, NDM eliminates the latency and dependency overhead inherent in traditional shell-script wrappers.

## Technical Capabilities

* **Automated Lifecycle Management**: Implements a "directory climbing" algorithm that recursively scans from the current working directory to the system root for `.ndmrc` files, ensuring environment-to-project parity.
* **Hybrid Installation Engine**: Supports both pre-compiled binary acquisition and local source-based compilation (`--build`). The build engine utilizes `sysconf(_SC_NPROCESSORS_ONLN)` to optimize parallel compilation across available CPU cores.
* **Self-Update Engine**: Integrated GitHub API consumer that automates the update lifecycle. It handles metadata parsing via `jq` and performs atomic binary replacement through the official installer.
* **Deep System Uninstaller**: A specialized module for the total removal of NDM. It sanitizes user data, system binaries, and performs automated cleanup of `/etc/security/pam_env.conf` and `/etc/profile.d/`.
* **Resource Auditing**: Provides a granular report of the NDM footprint using `lstat` for recursive disk usage calculation to ensure symlink-heavy directories are measured accurately.

## Installation

### 1. Binary Deployment (Recommended)

The installation script manages directory preparation, binary extraction, and environment persistence.

```bash
curl -fsSL https://github.com/daniwebdevid/nodeman/releases/download/v2.6.1/install.sh | sudo bash

```

### 2. Manual Build

Requires `gcc`, `cmake`, `libncurses-dev`, `libcurl`, and `xz-utils`.

```bash
git clone https://github.com/daniwebdevid/nodeman.git
cd nodeman && mkdir build && cd build
cmake .. && make
sudo make install

```

## Command Reference

| Command | Argument | Functional Description |
| --- | --- | --- |
| **`ndm`** | - | **Launches the Interactive Ncurses TUI.** |
| `ndm install` | `<version> [--build]` | Installs Node.js. Use `--build` for local compilation. |
| `ndm use` | `<version> [-s]` | Switches version (supports `-s` for session-only scope). |
| `ndm uninstall` | - | Performs a full system purge of NDM and its configs. |
| `ndm update` | - | Fetches latest release metadata and executes self-update. |
| `ndm status` | - | Reports active version, storage footprint, and cache health. |
| `ndm doctor` | - | Validates $PATH integrity and verifies symlink health. |
| `ndm start` | - | Triggers manual `.ndmrc` discovery and synchronization. |
| `ndm prune` | - | Purges `/var/cache/nodeman` to reclaim system storage. |

## Engineering Standards

* **Standardization**: C11 / POSIX compliant for maximum portability across Linux distributions.
* **Environment Persistence**: Managed via `/etc/security/pam_env.conf` and `/etc/profile.d/` for robust path inheritance across all shell types.
* **Memory Discipline**: Strictly enforced deallocation for discovery arrays and recursive data structures to prevent leaks during TUI sessions.
* **Security Architecture**: Enforces root privilege validation (`getuid() == 0`) for all system-level modifications.

## License

Licensed under the GNU General Public License v3.

---

**Developed by Dany Saputra** | *Native Node.js Version Management for Linux*
