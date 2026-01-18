# checking for minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# setting the project name, version, and language
project(nodeman VERSION 2.0.0 LANGUAGES C)

# Export compile commands for IDEs (e.g. VS Code) and tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set C standard to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# adding the executable target
add_executable(ndm
    src/main.c
    src/core/help.c
    src/core/install.c
    src/core/list.c
    src/core/remove.c
    src/core/use.c

    src/utils/arch.c
    src/utils/command.c
    src/utils/file.c
    src/utils/print.c
    src/utils/symlink.c
)

# specifying include directories for the target
target_include_directories(ndm PRIVATE
    include/
)

# install binary to system
install(TARGETS ndm DESTINATION bin)