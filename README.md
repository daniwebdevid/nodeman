# NDM (Node Manager) v2.5.0

NDM is a high-performance, lightweight Node.js version manager written in C11. It is engineered for Linux environments that require a native, zero-overhead alternative to shell-based managers. By utilizing POSIX system calls and direct binary execution, NDM eliminates the latency and dependency overhead inherent in traditional shell-script wrappers.

## Technical Capabilities

* **Automated Lifecycle Management**: Implements a "directory climbing" algorithm that recursively scans from the current working directory to the system root for `.ndmrc` files, ensuring environment-to-project parity.
* **Self-Update Engine**: Integrated GitHub API consumer that automates the update lifecycle. It handles metadata parsing via `jq` and performs atomic binary replacement through the official installer.
* **Resource Auditing**: Provides a granular report of the NDM footprint. It uses `lstat` for recursive disk usage calculation to ensure symlinks do not skew storage metrics.
* **Production-Grade Integrity**: Every installation undergoes mandatory SHA256 checksum verification. The engine includes an automated 3-attempt retry mechanism for recovering from corrupted network streams.
* **Smart Entry Logic**: The binary acts as a dual-mode interface. It dispatches to an Ncurses TUI if no arguments are provided, or routes to the CLI logic layer when specific commands are invoked.

## Installation

### 1. Binary Deployment (Recommended)

The installation script manages directory preparation, binary extraction, and environment persistence.

```bash
curl -fsSL https://github.com/daniwebdevid/nodeman/releases/download/v2.5.0/install.sh | sudo bash

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
| `ndm update` | - | Fetches latest release metadata and executes self-update logic. |
| `ndm status` | - | Reports active version, installation footprint, and cache size. |
| `ndm doctor` | - | Validates $PATH integrity and verifies symlink health. |
| `ndm start` | - | Triggers manual `.ndmrc` discovery and synchronization. |
| `ndm prune` | - | Purges `/var/cache/nodeman` to reclaim system storage. |
| `ndm install` | `<version>` | Performs acquisition with SHA256 verification. |
| `ndm use` | `<version>` | Switches version (supports `-s` for session-only scope). |

## Engineering Standards

* **Standardization**: C11 / POSIX compliant for maximum portability across Linux distributions.
* **Environment Persistence**: Unlike shell-based managers, NDM manages pathing via `/etc/security/pam_env.conf` and `/etc/profile.d/` for robust inheritance.
* **Memory Discipline**: Strictly enforced deallocation for discovery arrays and recursive data structures to prevent leaks during long-running TUI sessions.
* **Security Architecture**: Enforces root privilege validation for all system-level modifications (`/opt/nodeman`) and implements path traversal protection on all version inputs.

## License

Licensed under the GNU General Public License v3.

---

**Developed by Dany Saputra** | *Native Node.js Version Management for Linux*

