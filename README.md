# Node Version Manager (ndm) 🚀

**ndm** is a fast, lightweight Node.js version manager written in C. Manage multiple Node.js versions effortlessly with zero dependencies—install, switch, and list versions with a single command.

## ✨ Features

* 🚀 **Dynamic Architecture Detection:** Automatically detects your Linux architecture (x64, arm64, etc.) and downloads the correct Node.js binary.
* 📦 **Seamless Installation:** Install Node.js versions instantly with a single command—no configuration required.
* 🔄 **Quick Version Switching:** Switch between installed Node.js versions in seconds by updating system symlinks.
* ✅ **Comprehensive Version Listing:** View available versions from the official Node.js repository and check your locally installed versions.
* ⚡ **Lightweight & Fast:** Built entirely in C with minimal dependencies—optimized for speed and efficiency.

---

## 💾 Installation

### Quick Install (For Users)

Get started with a single command using our automated installation script:

```bash
curl -sL https://raw.githubusercontent.com/daniwebdevid/nodeman/bashinstallTest/install.sh | sudo bash
```

Once installed, you're ready to start managing Node.js versions:

```bash
ndm -i 20.13.0    # Install a version
ndm -c 20.13.0    # Switch versions
ndm -iL            # List installed versions
```

### Building from Source (For Contributors)

`ndm` is built with **CMake**, a modern build system that handles compilation across different platforms.

#### Prerequisites

1.  **CMake:** Version 3.10 or higher.
2.  **C Compiler:** `gcc` or `clang`.
3.  **Make Utility:** `make` (or Ninja).
4.  **System Dependencies:** **`aria2c`**, **`tar`**, **`curl`**, **`tr`**, **`grep`**, **`awk`** (required for version listing).

#### Build Steps

```bash
# 1. Create a dedicated build directory
mkdir -p build
cd build

# 2. Configure the project using CMake
cmake ..

# 3. Compile the project
cmake --build .

# 4. Install the binary to your system (requires sudo/root access)
sudo cmake --install .
```

----------

## 👨‍💻 Usage

> [!CAUTION]
> **Root Privileges Required:** All installation and version switching commands require **`sudo`** (root access) as they interact with system directories (`/usr/local/lib` and `/usr/local/bin`).

### 1. Install a New Node.js Version (`-i` or `--install`)

Bash

```
# Syntax: sudo ndm -i <version>
sudo ndm -i 20.13.0

```

### 2. Switch the Active Node.js Version (`-c` or `--change`)

Bash

```
# Syntax: sudo ndm -c <version>
sudo ndm -c 20.13.0

```

### 3. Listing Versions

| Command | Flag | Description |
|---------|------|-------------|
| `ndm -l` | `--list` | Lists **all available versions** from the official Node.js repository. |
| `ndm -iL` | `--installed-list` | Lists **locally installed versions**, marking the currently active version with an asterisk. |

#### Example Output

```
--- Installed Node.js Versions ---
  - v20.13.0
  - v22.9.0
  * v24.11.1 (ACTIVE)

--- End of List (3 versions found) ---
```

----------

## 📝 Directory Structure Managed by `ndm`

| Path | Purpose |
|------|---------|
| `/usr/local/lib/nodejsX.X.X/` | **Storage:** Where Node.js versions are extracted and stored (e.g., `/usr/local/lib/nodejs20.13.0/`). |
| `/usr/local/bin/` | **Execution:** Contains the symlinks (`node`, `npm`, `npx`) pointing to the active version's binaries. |

----------

## ⚖️ License

This project is licensed under the **[MIT License](./LICENSE)**.

## 🤝 Contribution

We welcome contributions! Whether you've found a bug, want to optimize performance, or wish to add support for additional Node.js architectures (e.g., `s390x`, `ppc64le`), please feel free to contribute.

For detailed contribution guidelines, see [CONTRIBUTING.md](./CONTRIBUTING.md).

---

**Copyright © 2025 ndm Contributors. All rights reserved.**
