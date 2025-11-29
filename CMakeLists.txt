# Tentukan versi minimum CMake yang diperlukan
cmake_minimum_required(VERSION 3.10)

# Tentukan nama proyek dan bahasa pemrograman yang digunakan
project(nodeman C)

# Tentukan file sumber mana yang akan dikompilasi menjadi sebuah executable
add_executable(ndm src/main.c src/print/print.c src/install/install.c src/change/change.c src/list/list.c src/remove/remove.c)
