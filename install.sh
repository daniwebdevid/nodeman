#!/bin/bash

# =======================================================
# NDM (Node Version Manager) Installation Script
# Designed for one-line installation: curl -sSL <url> | sudo bash
# =======================================================

# --- Configuration ---
REPO_URL="https://github.com/daniwebdevid/nodeman.git" 
PROJECT_NAME="ndm" 
TARGET_VERSION="1.1.0"
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
if ! git clone --depth 1 --branch "$TARGET_VERSION" "$REPO_URL" "$INSTALL_DIR/$PROJECT_NAME"; then
    echo "⚠️  Warning: Failed to clone specific branch '$TARGET_VERSION'. Trying default branch..."
    if ! git clone --depth 1 "$REPO_URL" "$INSTALL_DIR/$PROJECT_NAME"; then
        echo "❌ Error: Failed to clone repository."
        rm -rf "$INSTALL_DIR"
        exit 1
    fi
    
    # Attempt to checkout the tag manually if not cloned by branch
    cd "$INSTALL_DIR/$PROJECT_NAME" || exit
    echo "   Switching to version $TARGET_VERSION..."
    git fetch --tags
    if ! git checkout "$TARGET_VERSION"; then
        echo "❌ Error: Version $TARGET_VERSION not found."
        rm -rf "$INSTALL_DIR"
        exit 1
    fi
fi
echo "✅ Repository cloned and switched to $TARGET_VERSION."

# --- 3. Build Setup and Configuration ---
echo "3. Preparing build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit

echo "   Running CMake configuration..."
if ! cmake ../; then
    echo "❌ Error: CMake configuration failed."
    rm -rf "$INSTALL_DIR"
    exit 1
fi

# --- 4. Build the Project ---
echo "4. Building the ndm executable..."
if ! cmake --build .; then
    echo "❌ Error: Project build failed. Check C source files for errors."
    rm -rf "$INSTALL_DIR"
    exit 1
fi
echo "✅ Build successful."

# --- 5. Install to System Path ---
echo "5. Installing ndm to /usr/local/bin/..."
echo "   (This step may require sudo privileges)"

if sudo cp ./ndm /usr/local/bin/ndm; then
    sudo chmod +x /usr/local/bin/ndm
    echo "================================================="
    echo "🎉 SUCCESS: ndm $TARGET_VERSION has been installed."
    echo "   You can now run 'ndm --help' or 'ndm install <version>'."
    echo "================================================="
else
    echo "❌ Error: Installation failed. Check sudo permissions."
    rm -rf "$INSTALL_DIR"
    exit 1 
fi

# --- 6. Cleanup ---
echo "6. Cleaning up temporary files..."
rm -rf "$INSTALL_DIR"
echo "✅ Cleanup complete."

exit 0
