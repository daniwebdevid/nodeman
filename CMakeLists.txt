# CMakeLists.txt asli Anda

# checking for minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# setting the project name and language
project(nodeman C)

# Export compile commands for IDEs (e.g. VS Code) and tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# adding the executable target
add_executable(ndm
	src/main.c
	src/install.c
	src/change.c
	src/list.c
	src/remove.c
	# utility sources
	src/utils/arch.c
	src/utils/command.c
	src/utils/distro_detection.c
	src/utils/print.c
	# build system sources
	src/build/apt.c
)

# specifying include directories for the target
target_include_directories(ndm PRIVATE
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/utils
    ${CMAKE_SOURCE_DIR}/include/builds
)
