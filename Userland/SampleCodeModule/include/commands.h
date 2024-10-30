#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

#define EXIT 0
#define INPUT_ERROR -1
#define MAX_ARGS 3


static char* fillCommandAndArgs(char* args[], char *commandInput, int *argsCount);
static int clearCommand(int argc, char * argv[]);
static int exitCommand(int argc, char * argv[]);
static int helpCommand(int argc, char * argv[]);
static int invalidOp_exceptionCommand(int argc, char* argv[]);
static int show_registersCommand(int argc, char* argv[]);
static int snakesCommand(int argc, char* argv[]);
static int timeCommand(int argc, char* argv[]);
static int zero_exceptionCommand(int argc, char * argv[]);
static int size_up(int argc, char *argv[]);
static int size_down(int argc, char *argv[]);
int CommandParse(char *commandInput);

#endif