# NDM Source Entry (CLI) 🚀

This directory contains the main entry point for the **NDM** application. It acts as the "brain" that orchestrates user commands, parses global flags, and dispatches tasks to the core modules.

## 📌 Architecture Flow

The application follows a simple but powerful dispatch pattern:
1. **CLI Input**: User runs a command (e.g., `ndm install 20`).
2. **Flag Parsing**: `main.c` scans for global options like `--verbose`.
3. **Dispatcher**: The command is matched, and control is handed over to the appropriate module in `src/core/`.

## 🛠 File Overview

| File | Role | Responsibility |
| :--- | :--- | :--- |
| **`main.c`** | **Entry Point** | Handles command routing, error exit codes, and global state initialization. |

## ⚙️ How it Works

### 🧩 Command Dispatching
`main.c` menggunakan perbandingan string yang efisien untuk memetakan argumen CLI ke fungsi internal:
- **`install`** → Memanggil `install()` dengan offset argumen `+2`.
- **`use`** → Mengatur *scope* switching (user/global) melalui fungsi `use()`.
- **`list`** → Menangani penemuan versi lokal maupun remote.
- **`remove`** → Menjalankan logika uninstaller.

### 🚩 Global Flag: `--verbose`
NDM mendukung flag `--verbose` di posisi mana pun dalam argumen. Jika terdeteksi, variabel boolean `verbose` akan diaktifkan dan dipassing ke semua fungsi core dan utils untuk membuka log sistem yang detail.

### 🛡 Error Handling & Exit Codes
Aplikasi ini dirancang untuk produksi dengan kode keluar (exit codes) yang standar:
- `0`: Operasi sukses.
- `2`: Kesalahan input user atau masalah hak akses (Privilege Error).
- `1`: Kesalahan sistem internal atau kegagalan perintah luar (Runtime Error).

## 📜 Principles

1. **Efficiency**: Tidak ada alokasi memori yang berat di tahap routing awal.
2. **Predictability**: Pesan error yang seragam jika user lupa memasukkan argumen versi.
3. **Clean Exit**: Menggunakan `errno` untuk memberikan konteks tambahan saat aplikasi gagal.

---
*Part of the NDM Project - High Performance Node Management*