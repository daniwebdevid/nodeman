Tentu, saya akan menerjemahkan `README.md` lengkap tentang **Node Version Manager (ndm)** ke dalam Bahasa Inggris.

Berikut adalah versi Bahasa Inggris dari file `README.md` tersebut:

-----

# Node Version Manager (ndm) 🚀

**ndm** (Node Version Manager) is a simple version manager developed in **C**, designed for **fast and minimalist** installation and management of Node.js versions on Linux systems.

The main goal of this project is to provide a lightweight and quick tool for switching between and managing various Node.js versions without the overhead of complex Bash scripts.

-----

## ✨ Features

  * **Dynamic Architecture Detection:** Automatically detects the host Linux architecture (x64, arm64, etc.) to download the correct Node.js binary tarball.
  * **Version Installation (`-i`):** Downloads, extracts, and installs specified Node.js versions.
  * **Version Switching (`-c`):** Manages the active Node.js version by updating system symlinks in `/usr/local/bin/`.
  * **Version Listing (`-l`, `-iL`):** Lists available versions from the Node.js server, or versions already installed locally.
  * **Version Removal (`-r`):** Removes a specified Node.js version from the system.

-----

## 👨‍💻 Usage

**WARNING:** All installation, switching, and removal commands require **root privileges** (`sudo`) as they interact with system directories (`/usr/local/lib` and `/usr/local/bin`).

### Core Functionality (CLI Flags)

| Long Flag | Short Flag | Description |
| :--- | :--- | :--- |
| `--install <version>` | `-i <version>` | Installs a specific Node.js version (e.g., `sudo ndm -i 20.13.0`). |
| `--change <version>` | `-c <version>` | Switches the currently active Node.js version (e.g., `sudo ndm -c 22.1.0`). |
| `--remove <version>` | `-r <version>` | Removes a specific Node.js version (e.g., `sudo ndm -r 20.0.0`). |
| `--list [filter]` | `-l [filter]` | Lists **ALL AVAILABLE VERSIONS**. Can be filtered (e.g., `ndm -l 22`). |
| `--installed-list` | `-iL` | Lists **LOCALLY INSTALLED VERSIONS**, marking the active one (e.g., `sudo ndm -iL`). |

### Usage Examples

```bash
# Install the latest Node.js version
sudo ndm -i 22.1.0

# Switch to the newly installed version
sudo ndm -c 22.1.0

# View all installed versions
sudo ndm -iL

# Remove a specific version
sudo ndm -r 20.0.0
```

-----

## 💾 Installation & Building

### Prerequisites

Ensure you have the following dependencies installed:

1.  **CMake:** Version 3.10 or higher.
2.  **C Compiler:** `gcc` or `clang`.
3.  **Make Utility:** `make` (or Ninja).
4.  **Dependencies:** `git`, `aria2c` (for fast downloading), `curl`, `tar`, `tr`, `grep`, `awk`.

### 1\. Quick Installation Guide

Use `curl` to download and run the `install.sh` script. This command will automatically compile the project from the source code and copy the executable to `/usr/local/bin/`.

> **Important:** The script must be run with `sudo` as it requires root privileges to copy the `ndm` executable to the system directory (`/usr/local/bin`).

```bash
# Run this command in your terminal:
curl -sL [https://raw.githubusercontent.com/daniwebdevid/nodeman/bashinstallTest/install.sh](https://raw.githubusercontent.com/daniwebdevid/nodeman/bashinstallTest/install.sh) | sudo bash
```

### 2\. Manual Build

If you wish to compile `ndm` manually without the installation script, follow these steps:

```bash
# 1. Clone the repository
git clone [https://github.com/daniwebdevid/nodeman.git](https://github.com/daniwebdevid/nodeman.git)
cd nodeman

# 2. Create the build directory
mkdir build
cd build

# 3. Configure and Compile (Using CMake)
cmake ..
cmake --build .

# 4. Install to the system (requires root access)
sudo cp ./ndm /usr/local/bin
# Or using the CMake install command:
# sudo cmake --install .
```

-----

## 📝 Directory Structure Managed by `ndm`

| Path | Purpose |
| :--- | :--- |
| `/usr/local/lib/nodejs<version>` | **Storage:** Where all Node.js tarballs are extracted and stored (e.g., `/usr/local/lib/nodejs20.13.0/`). |
| `/usr/local/bin/` | **Execution:** Contains the symlinks (`node`, `npm`, `npx`) that point to the `/bin` directory of the currently active version. |

-----

## ⚖️ License

This project is licensed under the **[MIT License](https://www.google.com/search?q=%23&authuser=1)**.

## 🤝 Contribution

This project is a small, focused utility. If you find bugs or want to add support for more Node.js architectures (like `s390x` or `ppc64le`), feel free to contribute\!

-----

Would you like me to upload this translated version as a file or check the formatting for any platform (like GitHub)?