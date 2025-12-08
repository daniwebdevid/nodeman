# Node Version Manager (ndm) 🚀

**ndm** (Node Version Manager) is a simple, command-line tool built in C to easily install and switch between multiple versions of Node.js on Linux systems. It is designed to be lightweight and fast, directly managing installation directories and system symlinks.

## ✨ Features

* **Dynamic Architecture Detection:** Automatically detects the host Linux architecture (x64, arm64, etc.) to download the correct Node.js binary tarball.
* **Version Installation (`-i`):** Downloads, extracts, and installs specified Node.js versions into `/usr/local/lib/`.
* **Version Switching (`-c`):** Manages the active Node.js version by updating the system symlinks in `/usr/local/bin/`.
* **Version Listing (`-l`, `-iL`):** Lists both available versions from the Node.js remote server and versions already installed locally.

---

## 💾 Installation & Building

Since `ndm` is written in C and uses a modern build system, we use **CMake** to handle the compilation process.

### Prerequisites

1.  **CMake:** Version 3.10 or higher.
2.  **C Compiler:** `gcc` or `clang`.
3.  **Make Utility:** `make` (or Ninja).
4.  **System Dependencies:** `aria2c`, `tar`, **`curl`**, **`tr`**, **`grep`**, **`awk`** (required for version listing).

### Build Steps (Using CMake)

Assuming you have a `CMakeLists.txt` file configured to find all source files:

```bash
# 1. Clone the repository
git clone [https://github.com/daniwebdevid/nodeman.git](https://github.com/daniwebdevid/nodeman.git)
cd nodeman

# 2. Create the build directory
mkdir build
cd build

# 2. Configure the project using CMake
# This generates the platform-specific build files (e.g., Makefiles)
cmake ..

# 3. Compile the project
# The resulting 'ndm' binary will be in the 'build' directory.
cmake --build .

# 4. Install the binary to a system path (Requires sudo/root access)
sudo cmake --install .

```

----------

## 👨‍💻 Usage

**WARNING:** All installation and switching commands require **root privileges** (`sudo`) as they interact with system directories (`/usr/local/lib` and `/usr/local/bin`).

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

**Command**

**Flag**

**Description**

`ndm -l`

`--list`

Lists **ALL AVAILABLE** versions from the remote Node.js server.

`ndm -iL`

`--installed-list`

Lists **LOCALLY INSTALLED** versions, marking the currently active version.

Bash

```
# Example listing installed versions:
./ndm -iL

```

----------

## 📝 Directory Structure Managed by `ndm`

**Path**

**Purpose**

`/usr/local/lib/nodejs<version>`

**Storage:** Where all Node.js tarballs are extracted and stored (e.g., `/usr/local/lib/nodejs20.13.0/`).

`/usr/local/bin/`

**Execution:** Contains the symlinks (`node`, `npm`, `npx`) that point to the currently active version's `/bin` directory.

----------

## ⚖️ License

This project is licensed under the **[MIT License](./LICENSE)**.

## 🤝 Contribution

This project is a small, focused utility. If you find bugs or want to add support for more Node.js architectures (like `s390x` or `ppc64le`), feel free to contribute!

Would you like me to upload this translated version as a file or check the formatting for any platform (like GitHub)?