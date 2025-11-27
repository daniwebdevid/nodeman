#include "print.h"
#include <stdarg.h>
#include <stddef.h> 
#include <unistd.h> 
#include <limits.h> 

// --- Helper Functions ---

// Helper function to calculate string length (like strlen)
static size_t strlen_custom(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// Helper function to convert integer (long) to string (base 10)
// Safely handles INT_MIN case.
static void itoa_custom(long value, char* buf, int* len) {
    unsigned long abs_val;
    int is_negative = 0;
    int i = 0;

    if (value == 0) {
        buf[i++] = '0';
    } else {
        if (value < 0) {
            is_negative = 1;
            // Safe way to get absolute value, critical for INT_MIN
            abs_val = (unsigned long)-(value + 1) + 1; 
        } else {
            abs_val = (unsigned long)value;
        }

        // Convert digits in reverse order
        while (abs_val > 0) {
            buf[i++] = (abs_val % 10) + '0';
            abs_val /= 10;
        }
    
        // Add negative sign
        if (is_negative) {
            buf[i++] = '-';
        }

        // Reverse the string
        for (int j = 0; j < i / 2; j++) {
            char t = buf[j];
            buf[j] = buf[i - 1 - j];
            buf[i - 1 - j] = t;
        }
    }
    *len = i;
}

// --- Public Functions ---

// Custom snprintf implementation
int snprintf(char *str, size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int written_count = 0;
    int needed_count = 0;
    const char *fmt_ptr = format;
    char *write_ptr = str; 

    // Enable 'count-only' mode if size is 0 or str is NULL
    if (size == 0 || str == NULL) write_ptr = NULL; 

    while (*fmt_ptr != '\0') {
        if (*fmt_ptr == '%') {
            fmt_ptr++;
            if (*fmt_ptr == '\0') break;

            if (*fmt_ptr == 'd' || *fmt_ptr == 'i') {
                // Handle integer
                long val = va_arg(args, int);
                char num_buf[20];
                int num_len;
                itoa_custom(val, num_buf, &num_len);

                // Count and write
                for (int k = 0; k < num_len; k++) {
                    if (write_ptr != NULL && written_count < size - 1) {
                        *write_ptr++ = num_buf[k];
                        written_count++;
                    }
                    needed_count++;
                }
            } else if (*fmt_ptr == 's') {
                // Handle string
                char *s = va_arg(args, char*);
                if (s == NULL) s = "(null)";
                
                while (*s) {
                    if (write_ptr != NULL && written_count < size - 1) {
                        *write_ptr++ = *s;
                        written_count++;
                    }
                    needed_count++;
                    s++;
                }
            } else if (*fmt_ptr == 'c') {
                // Handle character
                char c = (char)va_arg(args, int);
                if (write_ptr != NULL && written_count < size - 1) {
                    *write_ptr++ = c;
                    written_count++;
                }
                needed_count++;
            } else if (*fmt_ptr == '%') {
                 // Handle %% (literal percent sign)
                if (write_ptr != NULL && written_count < size - 1) {
                    *write_ptr++ = '%';
                    written_count++;
                }
                needed_count++;
            }
        } else {
            // Regular character
            if (write_ptr != NULL && written_count < size - 1) {
                *write_ptr++ = *fmt_ptr;
                written_count++;
            }
            needed_count++;
        }
        fmt_ptr++;
    }

    va_end(args);

    // Ensure the buffer is null-terminated if size > 0
    if (size > 0 && str != NULL) {
        str[written_count < size ? written_count : size - 1] = '\0';
    }

    // Return the number of characters that would have been written
    return needed_count;
}

// Custom print function (uses write(1, ...))
int print(const char *text) {
    if (text == NULL) return 1;
    size_t len = strlen_custom(text); 
    ssize_t bytes_written = write(1, text, len); // FD 1 = stdout

    if (bytes_written != (ssize_t)len) {
        return 1; // Return non-zero on error
    }
    
    return 0; // Return 0 on success
}