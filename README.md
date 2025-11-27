# Node Version Manager (ndm) 🚀

**ndm** (Node Version Manager) is a simple, command-line tool built in C to easily install and switch between multiple versions of Node.js on Linux systems. It is designed to be lightweight and fast, directly managing installation directories and system symlinks.

## ✨ Features

* **Dynamic Architecture Detection:** Automatically detects the host Linux architecture (x64, arm64, etc.) to download the correct Node.js binary tarball.
* **Version Installation (`-i`):** Downloads, extracts, and installs specified Node.js versions into `/usr/local/lib/`.
* **Version Switching (`-c`):** Manages the active Node.js version by updating the system symlinks in `/usr/local/bin/`.
* **Dependency-Lite:** Relies only on standard system tools (`aria2c`, `tar`, `ln`, `test`) and minimal C libraries.

## 💾 Installation & Building

Since `ndm` is written in C, you need a C compiler (`gcc` or `clang`) to build it.

### Prerequisites

1.  **C Compiler:** `gcc` or `clang`
2.  **Make Utility:** `make` (Optional, for building efficiency)
3.  **System Dependencies:** `aria2c`, `tar` (These are typically available on most modern Linux distributions).

### Build Steps

Assuming your source files are organized correctly (`main.c`, `print/*.c`, `install/*.c`, `change/*.c`):

```bash
# Create the build directory (optional)
mkdir -p build

# Compile all source files
gcc -o build/ndm main.c print/print.c install/install.c change/change.c

# Move the binary to a system path (optional, but recommended)
# sudo mv build/ndm /usr/local/bin/
````

## 👨‍💻 Usage

**WARNING:** All installation and switching commands require **root privileges** (`sudo`) as they interact with system directories (`/usr/local/lib` and `/usr/local/bin`).

### 1\. Install a New Node.js Version (`-i` or `--install`)

This command downloads the specified version, checks if it's already installed, and installs it to `/usr/local/lib/nodejs<version>`.

```bash
# Syntax: sudo ndm -i <version>
sudo ./ndm -i 20.13.0
```

*(The install process does **not** automatically switch your active version.)*

### 2\. Switch the Active Node.js Version (`-c` or `--change`)

This command changes the system's active Node.js version by replacing symlinks in `/usr/local/bin/`. It first checks if the target version is already installed.

```bash
# Syntax: sudo ndm -c <version>
sudo ./ndm -c 20.13.0

# Example: Switch to a previously installed version
sudo ./ndm -c 22.1.0 
```

### Verification

After switching the version, verify the change using the standard Node.js command:

```bash
node -v
# Output should match the version you switched to (e.g., v22.1.0)
```

## 📝 Directory Structure Managed by `ndm`

`ndm` primarily manages two key locations:

| Path | Purpose |
| :--- | :--- |
| `/usr/local/lib/nodejs<version>` | **Storage:** Where all Node.js tarballs are extracted and stored (e.g., `/usr/local/lib/nodejs20.13.0/`). |
| `/usr/local/bin/` | **Execution:** Contains the symlinks (`node`, `npm`, `npx`) that point to the currently active version's `/bin` directory. |

## 🤝 Contribution

This project is a small, focused utility. If you find bugs or want to add support for more Node.js architectures (like `s390x` or `ppc64le`), feel free to contribute\!
