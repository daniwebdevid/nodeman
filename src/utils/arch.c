#include <sys/utsname.h>
#include <string.h>

/**
 * Detects the system architecture and maps it to Node.js naming conventions.
 * Returns "unknown" if uname fails or "unsupported" for non-mapped archs.
 */
const char* get_arch() {
    struct utsname buffer;

    if (uname(&buffer) != 0) {
        return "unknown";
    }

    // Map system machine names to Node.js dist naming
    if (strcmp(buffer.machine, "x86_64") == 0) {
        return "x64";
    } 
    else if (strcmp(buffer.machine, "aarch64") == 0 || strcmp(buffer.machine, "arm64") == 0) {
        return "arm64";
    } 
    else if (strncmp(buffer.machine, "armv7", 5) == 0) {
        return "armv7l";
    }
    else if (strcmp(buffer.machine, "ppc64le") == 0) {
        return "ppc64le";
    }
    else if (strcmp(buffer.machine, "s390x") == 0) {
        return "s390x";
    }

    return "unsupported";
}