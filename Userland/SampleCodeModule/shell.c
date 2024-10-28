#include "shell.h"
#include "functions.h"
#include "syscalls_shell.h"
#include "commands.h"
#define INPUT_MAX 1000
#define ESC 27
#define MSG "Welcome to our shell! Type 'help' for a list of commands\n"
#define INVALID_MSG "Invalid command. Type 'help' for a list of commands\n"

static void getBuffer(char * input);

void startShell() 
{
    printf_s("%s", "cacapedopis\n"); //era para probar el %
    printf_s(MSG);
    char input[INPUT_MAX] = {0};
    char copy[INPUT_MAX] = {0};
    int exit = 0 ;

    while (!exit){
        printf_s("$>");
        getBuffer(input);
        //printf_s("hola %s",input);
        if (input[0] != 0 ) { // chequeo si el usuario ingreso algo
            strcpy_s(copy, input);
            exit = CommandParse(copy);

            if (exit == INPUT_ERROR) {
                printf_s(INVALID_MSG);
            }
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
        else if (c != ESC){
            if (i < (INPUT_MAX - 1)) {
                input[i++] = c;
                putchar_s(c);
            }
        }
    }
    //printf_s("no se que pasa\n");
    // input[++i] = 0;
    // printf_s("%s",input);
}