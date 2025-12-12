#ifndef DISTRO_DETECTION_H
#define DISTRO_DETECTION_H

typedef enum {
    OS_UNKNOWN = 0,
    OS_DEBIAN,
    OS_ARCH,
    OS_REDHAT
} DetectedDistro;

DetectedDistro get_os_type();

#endif // DISTRO_DETECTION_H