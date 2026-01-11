#ifndef CORE_H
#define CORE_H

#include <stdbool.h>

void help();

int install(bool *verbose, char *argv[]);
int use(bool *verbose, int argc, char *argv[]);

#endif // CORE_H