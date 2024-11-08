#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

#define EXIT 0
#define INPUT_ERROR -1
#define MAX_ARGS 3

int CommandParse(char *commandInput);
char* fillCommandAndArgs(char* args[], char *commandInput, int *argsCount);
int helpCommand(int argc, char * argv[]);
int clearCommand(int argc, char * argv[]);
int invalidOp_exceptionCommand(int argc, char* argv[]);
int show_registersCommand(int argc, char* argv[]);
int snakesCommand(int argc, char* argv[]);
int timeCommand(int argc, char* argv[]);
int zero_exceptionCommand(int argc, char * argv[]);
int size_up(int argc, char *argv[]);
int size_down(int argc, char *argv[]);
int beepCommand(int argc, char* argv[]);
int randomCommand(int argc, char *argv[]);
#endif