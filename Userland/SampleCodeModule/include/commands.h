#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

#define INPUT_ERROR -1
#define MAX_ARGS 3

static int helpCommand(int argc, char * argv[]);
static int exitCommand(int argc, char * argv[]);

int CommandParse(char *commandInput);

#endif