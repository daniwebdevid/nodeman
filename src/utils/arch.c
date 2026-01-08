#include <sys/utsname.h>
#include <string.h>
#include "nodeman/utils.h"

const char* get_arch() {
    struct utsname buffer;

    if (uname(&buffer) != 0) {
        return "unknown";
    }

    // Mapping arsitektur kernel ke format URL Node.js
    if (strcmp(buffer.machine, "x86_64") == 0) {
        return "x64";
    } 
    else if (strcmp(buffer.machine, "aarch64") == 0) {
        return "arm64";
    } 
    else if (strncmp(buffer.machine, "armv7", 5) == 0) {
        return "armv7l";
    }

    return "unsupported";
}