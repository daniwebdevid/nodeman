#ifndef REMOVE_H
#define REMOVE_H

/*
 * @brief the specified version of NodeMan along with its cached package if applicable.
 * @return 0 on successful removal, non-zero on failure.
 */
int remove_version(const char *version);

#endif // REMOVE_H