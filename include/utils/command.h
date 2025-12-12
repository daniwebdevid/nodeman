#ifndef COMMAND_H
#define COMMAND_H

/** @brief Executes a system command with a given label for logging.
* @param label A string label to identify the command in logs.
* @param command The command string to execute.
* @return 0 on success, non-zero on failure.
*/
int execute_command_real(const char *label, const char *command);

#endif // COMMAND_H