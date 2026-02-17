#!/bin/sh

VERSION="2.5.0"
TAR_NAME="nodeman-${VERSION}-linux.tar.xz"
EXTRACTED_DIR="nodeman-${VERSION}-linux"
INSTALL_PATH="/opt/nodeman"
FILE="/etc/login.defs"

DEPENDENCIES="tar xz curl"

# Loop versi sh
for cmd in $DEPENDENCIES; do
    if ! command -v "$cmd" > /dev/null 2>&1; then
        echo "[ERROR] Command $cmd not found. please install $cmd"
        exit 1
    fi
done

# 1. Privilege Check
if [ "$(id -u)" -ne 0 ]; then
    echo "Error: Root privileges (sudo) are required for installation"
    exit 2
fi

# 2. Download Tarball
echo "Downloading NDM v${VERSION}..."
if ! curl -L -O "https://github.com/daniwebdevid/nodeman/releases/download/v${VERSION}/${TAR_NAME}"; then
    echo "Error: Failed to download tarball"
    exit 1
fi

# 3. Extraction
echo "Extracting package..."
if ! tar -xf "${TAR_NAME}"; then
    echo "Error: Extraction failed"
    exit 1
fi

# 4. Directory Preparation
echo "Creating installation directory: ${INSTALL_PATH}"
mkdir -p "${INSTALL_PATH}"
if ! cp -rf "${EXTRACTED_DIR}/opt/nodeman/"* "${INSTALL_PATH}/"; then
    echo "Error: Failed to copy files to ${INSTALL_PATH}"
    exit 1
fi

# 5. Finalizing Symlinks
echo "Setting up profile.d symlink..."
mkdir -p /etc/profile.d/
ln -sf "${INSTALL_PATH}/config/profile.d.sh" /etc/profile.d/nodeman.sh

# 6. echo "Configuring system PATH via /etc/login.defs..."
if grep -q "^ENV_PATH" "$FILE"; then
    # Update existing ENV_PATH
    sudo sed -i '/^ENV_PATH/c\ENV_PATH     PATH=/home/%u/.ndm/bin:/usr/local/bin:/usr/bin:/bin' "$FILE"
else
    # Append new ENV_PATH
    echo 'ENV_PATH     PATH=/home/%u/.ndm/bin:/usr/local/bin:/usr/bin:/bin' | sudo tee -a "$FILE"
fi

if grep -q "^ENV_SUPATH" "$FILE"; then
    # Update existing ENV_SUPATH  
    sudo sed -i '/^ENV_SUPATH/c\ENV_SUPATH   PATH=/home/%u/.ndm/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin' "$FILE"
else
    # Append new ENV_SUPATH
    echo 'ENV_SUPATH   PATH=/home/%u/.ndm/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin' | sudo tee -a "$FILE"
fi

# 7. Install Binary
echo "Installing binary to /usr/local/bin..."
chmod +x "${EXTRACTED_DIR}/usr/local/bin/ndm"
if ! mv "${EXTRACTED_DIR}/usr/local/bin/ndm" /usr/local/bin/; then
    echo "Error: Failed to install binary"
    exit 1
fi

# 8. Cleanup
echo "Cleaning up temporary files..."
rm -rf "${TAR_NAME}" "${EXTRACTED_DIR}"

echo " Successfully installed NDM v${VERSION}"
echo " PATH configured in /etc/login.defs (PID1-independent)"
echo " Restart login session atau 'su - $USER' untuk PATH baru"
/usr/local/bin/ndm --version
