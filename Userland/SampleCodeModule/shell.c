#include "shell.h"
#include "functions.h"
#include "syscalls_shell.h"
#include "commands.h"
#define INPUT_MAX 1000
#define ESC 
#define MSG "Welcome to our shell! Type 'help for a list of commands\n "

static void getBuffer(char * input);

int startShell() 
{

    printf(MSG);
    char input[INPUT_MAX] = {0};
    char copy[INPUT_MAX] = {0};
    int exit = 0 ;
    int command;

    while (!exit){
        printf("$>");
        getBuffer(input);

        if (input[0] != 0 ) { // chequeo si el usuario ingreso algo
            strcpy(copy, input);
            command = CommandParse(copy);
        } 
    }
    return 0;
}

static void getBuffer(char * input) { // lee lo que escribio el usuario y lo almacena en input
    char c; 
    int i = 0;

    while ((c = getchar()) != '\n') {
        if (c == '\b') {
            if (i > 0) {
                i--;
            }
        }
        else if (c != ESC) {
            if (i < (MAX_COMMAND_SIZE - 1)) {
                input[i++] = c;
            }
        }
    }
    input[i] = 0;
}

