#!/bin/bash

# =======================================================
# NDM (Node Version Manager) Installation Script
# Designed for one-line installation via curl | bash
# =======================================================

REPO_URL="https://github.com/daniwebdevid/nodeman.git"
INSTALL_DIR=$(mktemp -d)
PROJECT_NAME="nodeman"
BUILD_DIR="$INSTALL_DIR/$PROJECT_NAME/build"

echo "--- Starting ndm (Node Version Manager) Installation ---"

# --- 1. Check for Prerequisites ---
echo "1. Checking prerequisites (git, cmake, gcc, aria2c)..."
check_command() {
    if ! command -v "$1" &> /dev/null; then
        echo "❌ Error: Required command '$1' not found. Please install it first."
        exit 1
    fi
}
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
cd "$BUILD_DIR" || exit

echo "   Running CMake configuration..."
if ! cmake ../; then
    echo "❌ Error: CMake configuration failed."
    cd ~ && rm -rf "$INSTALL_DIR"
    exit 1
fi

# --- 4. Build the Project ---
echo "4. Building the ndm executable..."
if ! cmake --build .; then
    echo "❌ Error: Project build failed."
    cd ~ && rm -rf "$INSTALL_DIR"
    exit 1
fi
echo "✅ Build successful."

# --- 5. Install to System Path ---
echo "5. Installing ndm to /usr/local/bin/..."
echo "   (This step requires sudo privileges)"
if sudo cmake --install . ; then
    echo "================================================="
    echo "🎉 SUCCESS: ndm v1.0.0 has been installed."
    echo "   Try: sudo ndm -i 22.1.0"
    echo "================================================="
else
    echo "❌ Error: Installation failed. Check sudo permissions."
fi

# --- 6. Cleanup ---
echo "6. Cleaning up temporary files..."
cd ~
rm -rf "$INSTALL_DIR"
echo "✅ Cleanup complete."

exit 0