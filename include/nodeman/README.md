# NDM Source Core (v2.4.0)

This directory manages the entry point and routing for the Node Manager.

## Application Flow
1. **Smart Entry**: If `argc < 2`, the application immediately launches the **TUI mode**.
2. **Initialization**: Invokes `start()` to perform on-demand project synchronization via `.ndmrc` discovery.
3. **Dispatching**: Routes CLI commands (`install`, `use`, `list`, `remove`) to their respective core functions.

## Command Mapping
| Command | Implementation |
| :--- | :--- |
| `ndm` | `main_menu()` (TUI) |
| `ndm start` | `start()` (Manual Discovery) |
| `ndm install` | `install()` (Version Acquisition) |

---
*NDM Project - Source Architecture (v2.4.0)*