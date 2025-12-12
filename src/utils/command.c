#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include "utils/print.h"

#define MAX_ARGS 32

static int split_command_to_argv(char *cmd_str, char **argv_list, size_t max_args) {

    // Tokenize the command string by spaces
    int count = 0;
    char *token;
    
    // Using strtok to split the command string
    token = strtok(cmd_str, " ");

    // Loop through tokens and fill argv_list
    while (token != NULL && count < max_args - 1) {
        argv_list[count] = token;
        count++;
        token = strtok(NULL, " ");
    }
    
    // Null-terminate the argv_list
    argv_list[count] = NULL; 
    
    // Return the count of arguments found
    return count;
}

// --- FUNGSI UTAMA YANG DIREFACTOR ---
int command(const char *cmd_name, const char *cmd_str) {
    pid_t pid;
    int status;
    int exit_code = 1; // Default: error

    print("--------------------------------------------------\n");
    print(cmd_name);
    print(" (Low-Level Exec):\n");
    print(cmd_str); 
    print("\n");
    
    // 1. DUPLIKASI STRING DAN SPLIT ARGUMEN
    // execvp memodifikasi string, jadi kita harus duplikasi
    char *cmd_copy = strdup(cmd_str); 
    if (cmd_copy == NULL) {
        write(2, "Error: Memory allocation failed for command copy.\n", 50);
        return 1;
    }

    char *argv_list[MAX_ARGS];
    if (split_command_to_argv(cmd_copy, argv_list, MAX_ARGS) == 0) {
        write(2, "Error: Failed to parse command string.\n", 39);
        free(cmd_copy);
        return 1;
    }

    // 2. FORK (Membuat proses anak)
    pid = fork(); 

    if (pid == -1) {
        // Gagal membuat proses
        write(2, "Error: Failed to fork process.\n", 31);
        free(cmd_copy);
        return 1;
    } else if (pid == 0) {
        // KODE PROSES ANAK
        // Di sini kita mengganti program proses anak dengan perintah yang dieksekusi
        
        // execvp akan mencari program di PATH sistem
        execvp(argv_list[0], argv_list);

        // execvp hanya kembali jika terjadi kesalahan (misal: perintah tidak ditemukan)
        perror("Error during execvp"); // Mencetak error ke stderr
        exit(127); // Kode keluar non-standar untuk Exec Gagal
    } else {
        // KODE PROSES INDUK
        // 3. WAITPID (Menunggu proses anak selesai)
        if (waitpid(pid, &status, 0) != -1) {
            // Memeriksa apakah proses anak berakhir secara normal
            if (WIFEXITED(status)) {
                // Mengambil kode keluar program yang dieksekusi (contoh: test -d)
                exit_code = WEXITSTATUS(status);
            }
        } else {
            // Gagal menunggu proses anak
            write(2, "Error: waitpid failed.\n", 23);
            exit_code = 1;
        }
    }
    
    // 4. CLEANUP
    free(cmd_copy);

    // 5. LOGGING DAN RETURN
    print("COMMAND EXIT CODE: ");
    char result_str[10];
    snprintf(result_str, sizeof(result_str), "%d", exit_code);
    print(result_str);
    print("\n");
    
    print("--------------------------------------------------\n");
    
    // Mengembalikan 0 untuk Sukses (exit_code 0), 1 untuk Gagal (exit_code lainnya)
    return (exit_code == 0) ? 0 : 1;
}