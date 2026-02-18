#!/usr/bin/env bash

set -euo pipefail

# --- Configuration ---
PROJECT_NAME="nodeman"
DIST_DIR="$(pwd)/dist"
STAGING_ROOT="$DIST_DIR/tarball"
TOOLCHAIN_FILE="arm_toolchain.cmake"
CPU_CORES=$(nproc)

log_step() { echo -e "\033[1;34m[BUILD]\033[0m $1"; }
log_done() { echo -e "\033[1;32m[DONE]\033[0m $1"; }
log_err()  { echo -e "\033[1;31m[ERROR]\033[0m $1" >&2; exit 1; }

# --- Initialization ---
log_step "Cleaning distribution directory..."
rm -rf "$DIST_DIR"
mkdir -p "$STAGING_ROOT"

echo ""
read -rp "Enter release version (e.g., 2.6.1): " VERSION
[[ -z "$VERSION" ]] && log_err "Version is required."

# --- Helper: Setup Assets ---
# Kita isi file pendukung (config, desktop, dll) ke staging root sekali saja
# karena file-file ini sama untuk semua arsitektur.
setup_assets() {
    log_step "Setting up assets in staging root..."
    install -dm755 "$STAGING_ROOT/opt/nodeman/config"
    cp -rp config/* "$STAGING_ROOT/opt/nodeman/config/"
    install -Dm644 "desktop/${PROJECT_NAME}.desktop" "$STAGING_ROOT/usr/share/applications/${PROJECT_NAME}.desktop"
    install -dm755 "$STAGING_ROOT/etc/profile.d"
    ln -sf "/opt/nodeman/config/profile.d.sh" "$STAGING_ROOT/etc/profile.d/nodeman.sh"
    install -dm777 "$STAGING_ROOT/var/cache/nodeman"
}

# --- SIKLUS 1: x86_64 ---
log_step "Siklus 1: Building for x86_64..."
# 1. Build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS="-march=x86-64"
cmake --build build -j"$CPU_CORES"

# 2. Install ke Staging
setup_assets
DESTDIR="$STAGING_ROOT" cmake --install build --prefix /usr/local

# 3. Archive
TAR_X86="${PROJECT_NAME}-${VERSION}-linux-x86_64.tar.xz"
tar --owner=0 --group=0 -cJf "$DIST_DIR/$TAR_X86" \
    --transform "s|^tarball|${PROJECT_NAME}-${VERSION}-linux|" \
    -C "$DIST_DIR" tarball
log_done "Saved: $DIST_DIR/$TAR_X86"

# 4. Flush Build & 5. Flush Staging Binary
log_step "Flushing build and staging for next architecture..."
rm -rf build
rm -f "$STAGING_ROOT/usr/local/bin/ndm"

# --- SIKLUS 2: aarch64 ---
if command -v aarch64-linux-gnu-gcc &> /dev/null && [ -f "$TOOLCHAIN_FILE" ]; then
    log_step "Siklus 2: Building for aarch64..."
    # 6. Build ARM (Karena build/ kosong, ini dipaksa fresh)
    cmake -B build -S . \
        --fresh \
        -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
        -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
        -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j"$CPU_CORES"

    # Verifikasi Arsitektur sebelum ditar
    BINARY_TYPE=$(file -b build/ndm)
    log_step "Verified Binary: $BINARY_TYPE"

    # 7. Archive Lagi (Setelah install binary ARM ke staging)
    DESTDIR="$STAGING_ROOT" cmake --install build --prefix /usr/local
    
    TAR_ARM="${PROJECT_NAME}-${VERSION}-linux-aarch64.tar.xz"
    tar --owner=0 --group=0 -cJf "$DIST_DIR/$TAR_ARM" \
        --transform "s|^tarball|${PROJECT_NAME}-${VERSION}-linux|" \
        -C "$DIST_DIR" tarball
    log_done "Saved: $DIST_DIR/$TAR_ARM"
else
    log_step "Skipping aarch64: Toolchain or $TOOLCHAIN_FILE not found."
fi

log_done "Workflow completed successfully."