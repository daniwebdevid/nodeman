# checking for minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# setting the project name, version, and language
project(nodeman VERSION 2.5.0 LANGUAGES C)

# Export compile commands for IDEs (e.g. VS Code) and tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set C standard to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# searching for ncurses package
find_package(Curses REQUIRED)

# adding the executable target
add_executable(ndm
    src/main.c
    src/core/help.c
    src/core/install.c
    src/core/list.c
    src/core/remove.c
    src/core/use.c
    src/core/start.c
    src/core/doctor.c
    src/core/prune.c
    src/core/update.c
    src/core/status.c
    src/core/uninstall.c

    src/utils/arch.c
    src/utils/command.c
    src/utils/file.c
    src/utils/print.c
    src/utils/symlink.c

    src/tui/main_menu.c
    src/tui/tui_render.c
    src/tui/handler.c
)

# specifying include directories for the target
target_include_directories(ndm PRIVATE
    include/
    ${CURSES_INCLUDE_DIRS}
)

# linking libraries to the target executable
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(MY_ARCH "arm64")
else()
    set(MY_ARCH "x86_64")
endif()

set(LIB_DIR "${CMAKE_SOURCE_DIR}/libs/${MY_ARCH}")

# Link in order (Linker needs order: dependents first then dependencies)
target_link_libraries(ndm PRIVATE 
    ${LIB_DIR}/libform.a
    ${LIB_DIR}/libncurses.a
    ${LIB_DIR}/libtinfo.a
    m   # Math library (compiler default)
    dl  # Dynamic link (compiler default)
    pthread # Threading (compiler built-in)
)

# install binary to system
install(TARGETS ndm DESTINATION bin)
