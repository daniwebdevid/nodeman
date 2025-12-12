#ifndef GET_ARCH_SUFFIX_H
#define GET_ARCH_SUFFIX_H

#include <stddef.h>

/*
* @brief Retrieves the architecture suffix of the system.
* Maps system architecture to Node.js compatible suffixes.
*/
static int get_arch_suffix(char *arch_suffix, size_t size);

#endif // GET_ARCH_SUFFIX_H