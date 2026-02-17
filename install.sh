#!/bin/sh

VERSION="2.6.0"


ARCH=$(uname -m)
case "$ARCH" in
    x86_64)
        TAR_ARCH="x86_64"
        ;;
    aarch64|arm64)
        TAR_ARCH="aarch64"
        ;;
    *)
        echo "[ERROR] Architecture $ARCH is not supported."
        exit 1
        ;;
esac

TAR_NAME="nodeman-${VERSION}-linux-${TAR_ARCH}.tar.xz"
EXTRACT_TMP="ndm_temp_extract"
INSTALL_PATH="/opt/nodeman"
FILE="/etc/login.defs"

DEPENDENCIES="tar xz curl"

# Loop check dependencies
for cmd in $DEPENDENCIES; do
    if ! command -v "$cmd" > /dev/null 2>&1; then
        echo "[ERROR] Command $cmd not found. Please install $cmd"
        exit 1
    fi
done

# Privilege Check
if [ "$(id -u)" -ne 0 ]; then
    echo "Error: Root privileges (sudo) are required for installation"
    exit 2
fi

# 2. Download Tarball
echo "Detected architecture: ${TAR_ARCH}"
echo "Downloading NDM v${VERSION} for ${TAR_ARCH}..."
URL="https://github.com/daniwebdevid/nodeman/releases/download/v${VERSION}/${TAR_NAME}"

if ! curl -L -O "$URL"; then
    echo "Error: Failed to download tarball from $URL"
    exit 1
fi

# 3. Extraction
echo "Extracting package..."
mkdir -p "$EXT_TMP"
if ! tar -xf "${TAR_NAME}"; then
    echo "Error: Extraction failed"
    exit 1
fi

# 4. Directory Preparation (Sesuai struktur folder tarball lu)
echo "Creating installation directory: ${INSTALL_PATH}"
mkdir -p "${INSTALL_PATH}"

# Asumsi di dalam tarball lu ada folder opt/nodeman/
if [ -d "opt/nodeman" ]; then
    cp -rf opt/nodeman/* "${INSTALL_PATH}/"
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
