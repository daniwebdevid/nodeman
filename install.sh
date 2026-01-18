#!/bin/sh

VERSION="2.0.0"
TAR_NAME="nodeman-${VERSION}-linux.tar.xz"
EXTRACTED_DIR="nodeman-${VERSION}-linux"
INSTALL_PATH="/opt/nodeman"
DEPENDENCIES=("tar" "xz" "curl")

for cmd in "${DEPENDENCIES[@]}"; do
    if ! command -v "$cmd" &> /dev/null; then
        echo "[ERROR] Command $cmd not found. please install $cmd"
        exit 1
    fi
done

if ! command -v systemctl &> /dev/null; then echo "Error: This tool requires a systemd-based OS."

# 0. Privilege Check
if [ "$(id -u)" -ne 0 ]; then
    echo "Error: Root privileges (sudo) are required for installation"
    exit 2
fi

# 1. System Requirement Check
echo "Verifying system requirements..."
if ! command -v systemctl > /dev/null 2>&1; then
    echo "Error: systemd not found. This version of NDM requires systemd."
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
echo "Setting up symbolic links..."
mkdir -p /etc/profile.d/
ln -sf "${INSTALL_PATH}/config/profile.d.sh" /etc/profile.d/nodeman.sh
mkdir -p /etc/environment.d/ 
ln -sf  "${INSTALL_PATH}/config/systemd.conf" /etc/environment.d/60-nodeman.conf 

# 6. Install Binary
echo "Installing binary to /usr/local/bin..."
chmod +x "${EXTRACTED_DIR}/usr/local/bin/ndm"
if ! mv "${EXTRACTED_DIR}/usr/local/bin/ndm" /usr/local/bin/; then
    echo "Error: Failed to install binary"
    exit 1
fi

# 7. Cleanup
echo "Cleaning up temporary files..."
rm -rf "${TAR_NAME}" "${EXTRACTED_DIR}"

echo "Successfully installed NDM v${VERSION}"
/usr/local/bin/ndm --version