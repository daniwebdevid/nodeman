# NDM (Node Manager) v2.4.0

NDM is a high-performance, lightweight Node.js version manager written in C11. It is engineered for Linux environments that require a native, zero-overhead alternative to shell-based managers.

## Key Features

- **Smart Entry Dispatcher**: Automatically launches the interactive TUI if no arguments are provided.
- **On-Demand Project Sync**: Use `ndm start` to recursively detect `.ndmrc` files and synchronize the environment.
- **Environment Diagnostics**: The `doctor` command validates $PATH integrity, active symlinks, and environment health.
- **Storage Optimization**: The `prune` command safely purges the system cache in `/var/cache/nodeman`.
- **Production-Grade Integrity**: Mandatory SHA256 verification with a 3-attempt retry logic for corrupted streams.
- **Zero Runtime Overhead**: A compiled native binary that eliminates the latency of shell-script wrappers.

## Installation

### 1. Quick Install (Binary Release)
The easiest way to install NDM on Linux. This script handles binary extraction, directory setup, and environment configuration.

```bash
curl -fsSL https://github.com/daniwebdevid/nodeman/releases/download/v2.4.0/install.sh | sudo bash

```

### 2. Build from Source

Ensure you have `gcc`, `cmake`, `libncurses-dev`, `libcurl`, and `xz-utils` installed.

Bash

```
git clone https://github.com/daniwebdevid/nodeman.git
cd nodeman && mkdir build && cd build
cmake .. && make
sudo make install

```

## Usage

**Command**

**Action**

**`ndm`**

**Launches the Interactive TUI (Default behavior).**

`ndm start`

Triggers `.ndmrc` discovery (climbing to root) and switches version.

`ndm doctor`

Performs environment health checks and path diagnostics.

`ndm prune`

Clears downloaded tarballs from the system cache (Requires Sudo).

`ndm install <v>`

Installs a specific version with integrity verification.

`ndm use <v> -s`

Switches version or outputs shell-eval string for sessions.

`ndm list`

Displays local and system installations.

## Technical Implementation

-   **Standard**: C11 / POSIX compliant.
    
-   **Environment Persistence**: Managed via `/etc/security/pam_env.conf` and `/etc/profile.d/` for robust path inheritance.
    
-   **Memory Safety**: Strictly enforced deallocation for discovery arrays.
    
-   **Security**: Root privilege validation for all system-level operations (`/opt/nodeman`).
    

## License

Licensed under the GNU General Public License v3.

----------

**Developed by Dany Saputra** | _High-Performance Node Management for Linux_


