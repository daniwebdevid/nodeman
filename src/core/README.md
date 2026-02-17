# NDM Source Core (v2.6.0)

This directory manages the entry point and routing for the Node Manager. Version 2.6.0 introduces deep system uninstallation and enhanced acquisition logic.

## Application Flow

1. **Smart Entry**: If `argc < 2`, the application immediately launches the **TUI mode** via `main_menu()`.
2. **Global Flag Parsing**: Scans for `--verbose` to propagate logging state across all modules.
3. **Dispatching**: Routes CLI commands to their respective core functions with sanitized argument pointers.

## Command Mapping (v2.6.0)

| Command | Implementation | Technical Scope |
| --- | --- | --- |
| `ndm` | `main_menu()` | Ncurses interactive interface. |
| `ndm install` | `install()` | Acquisition (Binary/Source) with `argc` awareness. |
| `ndm use` | `use()` | Version switching (Global/User/Session). |
| `ndm remove` | `remove_node_js()` | Specific version deletion. |
| `ndm start` | `start()` | Project-based `.ndmrc` synchronization. |
| `ndm doctor` | `doctor()` | Environment and $PATH health diagnostics. |
| `ndm prune` | `prune_cache()` | Cache cleanup in `/var/cache/nodeman`. |
| `ndm status` | `status()` | Resource audit and binary integrity check. |
| `ndm update` | `update()` | Self-updater via GitHub API (v2.6.0 signature). |
| `ndm uninstall` | `uninstall()` | **New in v2.6.0**. Full system purge and PAM cleanup. |

## Implementation Details

### 1. Command Dispatching Logic

The dispatcher in `main.c` uses optimized string comparisons. Version 2.6.0 update:

* **`install`**: Sekarang mengirimkan `argc` untuk mendukung parsing flag tambahan seperti `--build`.
* **`update`**: Menggunakan parameter `argc-1` untuk kontrol eksekusi yang lebih presisi.
* **`uninstall`**: Entry point baru untuk menghapus seluruh footprint NDM dari sistem.

### 2. Resource Discipline

* **Exit Codes**: Mengikuti standar POSIX (`0` success, `1` runtime error, `2` invalid args/privilege).
* **Error Propagation**: Menggunakan `errno` yang dipadukan dengan `log_error` untuk feedback sistem yang akurat.

### 3. Engineering Standards

* **Zero-Overhead Dispatch**: Startup instan karena tidak ada ketergantungan pada shell scripting di level entry point.
* **Interface Parity**: CLI dan TUI menggunakan core logic yang sama untuk menjamin konsistensi perilaku sistem.

---

*NDM Project - Source Architecture Documentation (v2.6.0)*
