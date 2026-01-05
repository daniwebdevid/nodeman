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
	src/utils/log.c
)

# specifying include directories for the target
target_include_directories(ndm PRIVATE
    include/
)
