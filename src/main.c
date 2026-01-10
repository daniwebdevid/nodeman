#include <string.h>
#include "nodeman/core.h"
#include "nodeman/utils.h"
#include <errno.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {

    // check if no ARGS
    if(argc < 2) {
        help();
        return 0;
    }

    bool verbose = 0;
    for (int i =0; i < argc; i++) {
        if(strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        }
    }

    if (strcmp(argv[1], "install") == 0) {
        if (argc < 3) {
            log_error("No version sellected");
            log_error("Use: %s install <nama_paket>", argv[0]);

            errno = EINVAL; 
        
            return 2; 
        }
        install(&verbose, argv+2);
    } else if(strcmp(argv[1], "use") == 0) {
            if (argc < 3) {
            log_error("No version sellected");
            log_error("Use: %s use <nama_paket>", argv[0]);

            errno = EINVAL; 
        
            return 2; 
        }
        use(&verbose, argv+2);
    }
    return 0;
}