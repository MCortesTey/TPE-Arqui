#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

#define EXIT 0
#define INPUT_ERROR -1
#define MAX_ARGS 3


int CommandParse(char *commandInput);

#endif