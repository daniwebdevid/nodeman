# Contributing to NDM

First of all, thank you for taking the time to contribute! NDM is built for high performance and zero-overhead, so we maintain strict standards regarding code quality and system resource management.

## Technical Stack
- **Language**: C11 (Strictly enforced).
- **Interface**: Ncurses (TUI) and Standard POSIX (CLI).
- **Build System**: CMake 3.10+.
- **Dependencies**: `libncurses`, `libcurl`, `xz-utils`.

## Coding Standards

As a low-level project, we prioritize efficiency over simplicity. Please adhere to these rules:

### 1. Memory Management
- Every allocation must have a corresponding deallocation.
- If you add a discovery function in `core/`, ensure it has a matching cleanup function (see `free_versions_array` in `list.c`).
- Use stack allocation for fixed paths (`PATH_MAX`) to avoid unnecessary heap fragmentation.

### 2. Error Handling
- Use the `NdmError` enum defined in `utils.h`.
- Map system failures to consistent negative integers.
- Always check `errno` after failed system calls and use `log_error()` for reporting.

### 3. File & Link Operations
- Do not use `system()` calls for filesystem tasks. Use our POSIX wrappers:
    - `symlink_force()` for atomic link updates.
    - `open_file()` for safe config reading.
    - `file_write()` for variadic writing.

### 4. Logging & Verbosity
- Use `log_info(verbose, ...)` for general flow updates.
- Information must only be printed to `stdout` if the `--verbose` flag is active.
- Warnings and Errors must always go to `stderr`.

## Development Workflow

### Building for Development
```bash
mkdir build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
make

```

### Submitting Changes

1.  **Branching**: Create a feature branch from `main`.
    
2.  **Implementation**: Ensure your logic is modular (Business logic in `src/core`, system utilities in `src/utils`).
    
3.  **Diagnostics**: Run `ndm doctor` after build to ensure your environment setup logic doesn't break pathing.
    
4.  **Pull Request**: Provide a clear description of the performance impact or the maintenance problem your PR solves.
    

## Project Structure Reference

-   `/include`: Global contracts (`core.h`, `utils.h`).
    
-   `/src/core`: Primary logic (Installation, switching, pruning).
    
-   `/src/utils`: POSIX wrappers and system abstractions.
    
-   `/src/tui`: Ncurses state machine and rendering logic.
    

----------

_Maintained by Dany Saputra_

