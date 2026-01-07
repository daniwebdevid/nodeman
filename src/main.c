#include <stdio.h>
#include <string.h>
#include "nodeman/core.h"
#include "nodeman/utils.h"

int main(int argc, char *argv[]) {

    // check if no ARGS
    if(argc < 2) {
        help();
        return 0;
    }

    bool verbose = 0;
    int opt;
    static struct option long_options[] = {
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

    if (strcmp(argv[1], "install") == 0) {
        if (argc < 3) {
            
        }
        log_info("install");
    }
    return 0;
}