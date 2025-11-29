#!/bin/bash

# =======================================================
# NDM (Node Version Manager) Installation Script
# Designed for one-line installation via curl | sudo bash
# =======================================================

# --- Configuration ---
# HARAP GANTI DENGAN URL REPOSITORI GITHUB ANDA JIKA BERBEDA
REPO_URL="https://github.com/daniwebdevid/nodeman.git" 
PROJECT_NAME="ndm" # Nama proyek dan executable
INSTALL_DIR=$(mktemp -d)
BUILD_DIR="$INSTALL_DIR/$PROJECT_NAME/build"

echo "--- Starting ndm (Node Version Manager) Installation ---"

# --- HELPER FUNCTION: Check for Prerequisites ---
check_command() {
    if ! command -v "$1" &> /dev/null; then
        echo "❌ Error: Required command '$1' not found. Please install it first."
        exit 1
    fi
}

# --- 1. Check for Prerequisites ---
echo "1. Checking prerequisites (git, cmake, gcc, aria2c)..."
check_command git
check_command cmake
check_command gcc
check_command aria2c
check_command tar
check_command curl
echo "✅ All required dependencies found."

# --- 2. Clone the Repository ---
echo "2. Cloning source code from $REPO_URL..."
if ! git clone --depth 1 "$REPO_URL" "$INSTALL_DIR/$PROJECT_NAME"; then
    echo "❌ Error: Failed to clone repository."
    rm -rf "$INSTALL_DIR"
    exit 1
fi
echo "✅ Repository cloned successfully."

# --- 3. Build Setup and Configuration ---
echo "3. Preparing build directory..."
mkdir -p "$BUILD_DIR"

# Pindah ke direktori build di dalam folder yang baru di-clone
cd "$INSTALL_DIR/$PROJECT_NAME" || exit

# Jika ada perubahan nama folder, pastikan ini benar-benar masuk ke folder build
mkdir -p build
cd build || exit


echo "   Running CMake configuration..."
if ! cmake ../; then
    echo "❌ Error: CMake configuration failed."
    cd ~ 
    rm -rf "$INSTALL_DIR"
    exit 1
fi

# --- 4. Build the Project ---
echo "4. Building the ndm executable..."
if ! cmake --build .; then
    echo "❌ Error: Project build failed. Check the C source files for errors."
    cd ~ 
    rm -rf "$INSTALL_DIR"
    exit 1
fi
echo "✅ Build successful."

# --- 5. Install to System Path (Menggunakan 'cp' yang lebih handal) ---
echo "5. Installing ndm to /usr/local/bin/..."
echo "   (This step requires sudo privileges)"

# Kita ambil executable ndm dari direktori build (.) dan menyalinnya ke /usr/local/bin/
if sudo cp ./ndm /usr/local/bin ; then
    echo "================================================="
    echo "🎉 SUCCESS: ndm v1.1.0 has been installed."
    echo "   You can now run 'sudo ndm -i <version>'."
    echo "================================================="
else
    echo "❌ Error: Installation failed. Check sudo permissions."
    
    # Cleanup dan Exit jika gagal
    cd ~
    rm -rf "$INSTALL_DIR"
    echo "✅ Cleanup temporary directory after failed installation."
    exit 1 
fi

# --- 6. Cleanup ---
echo "6. Cleaning up temporary files..."
cd ~
rm -rf "$INSTALL_DIR"
echo "✅ Cleanup complete."

exit 0