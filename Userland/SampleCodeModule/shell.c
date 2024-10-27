#include "shell.h"
#include "functions.h"
#include "syscalls_shell.h"
#include "commands.h"
#define INPUT_MAX 1000
#define ESC 27
#define MSG "Welcome to our shell! Type 'help' for a list of commands\n"

static void getBuffer(char * input);

void startShell() 
{
    printf_s(MSG);
    char input[INPUT_MAX] = {0};
    char copy[INPUT_MAX] = {0};
    int exit = 0 ;

    while (!exit){
        printf_s("$>");
        getBuffer(input);

        if (input[0] != 0 ) { // chequeo si el usuario ingreso algo
            strcpy_s(copy, input);
            CommandParse(copy);
        } 
    }
}

static void getBuffer(char * input) { // lee lo que escribio el usuario y lo almacena en input
    char c; 
    int i = 0;

    while ((c = getchar_s()) != '\n') {
        if (c == '\b') {
            if (i > 0) {
                i--;
            }
        }
        else {
        if (c != ESC) {
            if (i < (INPUT_MAX - 1)) {
                input[i++] = c;
            }
        }
        }
    input[i] = 0;
    }
}