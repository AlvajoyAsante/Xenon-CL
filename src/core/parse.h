#ifndef PARSE_H
#define PARSE_H

#include <tice.h>

/**
 * Check command name at the front of the input string before running.
 */
void core_CheckCommand(char *str);

/**
 * Runs a set program in oxygen's file system.
 */
int core_RunProgram(uint8_t i);

#endif /* __PARSE_H__ */