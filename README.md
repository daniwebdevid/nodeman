# NDM (Node Manager) v2.5.0

NDM is a high-performance, lightweight Node.js version manager written in C11. It is engineered for Linux environments that require a native, zero-overhead alternative to shell-based managers. By bypassing the latency of shell-script wrappers, NDM provides instantaneous environment switching and robust system management.

## 🚀 Key Features

- **Smart Entry Dispatcher**: Zero-config entry. Running `ndm` without arguments automatically launches the Ncurses TUI for interactive management.
- **Automated Self-Update**: Keep your manager current. The `update` command fetches metadata from GitHub and executes the official installer automatically.
- **Deep Resource Auditing**: Use `status` to get a snapshot of your NDM footprint, including total disk usage of installed versions and cache health.
- **Project-Level Sync (Start)**: Implements a "directory climbing" algorithm to detect `.ndmrc` files from your CWD up to the root, ensuring the environment always matches your project.
- **Environment Diagnostics**: The `doctor` command validates $PATH integrity, active symlinks, and overall environment health.
- **Storage Management**: Safely purge system cache in `/var/cache/nodeman` to reclaim space.
- **Production Integrity**: Mandatory SHA256 checksum verification with an automated 3-attempt retry mechanism.

## 🛠 Installation

### 1. Quick Install (Binary)
Requires `curl`, `tar`, `xz`, and `jq`.
```bash
curl -fsSL https://github.com/daniwebdevid/nodeman/releases/download/v2.5.0/install.sh | sudo bash

```

### 2. Build from Source

Ensure `gcc`, `cmake`, `libncurses-dev`, and `libcurl` are installed.

```bash
git clone https://github.com/daniwebdevid/nodeman.git
cd nodeman && mkdir build && cd build
cmake .. && make
sudo make install

```

## 📖 Usage Reference

| Command | Sub-command | Action |
| --- | --- | --- |
| **`ndm`** | - | **Launches Interactive TUI (Default)** |
| `ndm` | `update` | Self-updates NDM to the latest release |
| `ndm` | `status` | Reports storage usage and active version health |
| `ndm` | `start` | Triggers `.ndmrc` discovery & auto-switch |
| `ndm` | `doctor` | Performs path diagnostics and link validation |
| `ndm` | `install <v>` | Fetches & verifies a specific Node.js version |
| `ndm` | `use <v>` | Switches version (use `-s` for session-only) |
| `ndm` | `prune` | Clears downloaded tarballs from `/var/cache/nodeman` |

## ⚙️ Technical Implementation

* **Language Standard**: C11 / POSIX compliant.
* **Process Orchestration**: Uses direct `fork/execvp` calls, bypassing `/bin/sh` for maximum performance and security.
* **Environment Persistence**: Managed via `/etc/security/pam_env.conf` and `/etc/profile.d/` for robust path inheritance across all shell types.
* **Memory Discipline**: Strictly enforced deallocation for all dynamic arrays (e.g., version discovery) to prevent leaks in TUI sessions.
* **Privilege Scoping**: System-wide operations (`/opt/nodeman`) strictly enforce root validation (`getuid() == 0`).

## 📄 License

Licensed under the GNU General Public License v3.

---

**Developed by Dany Saputra** | *High-Performance Native Node Management*

