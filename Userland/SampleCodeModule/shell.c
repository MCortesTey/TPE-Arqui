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
    printf_s(MSG);
    char input[INPUT_MAX] ;
    char copy[INPUT_MAX];
    int exit = 0 ;

    while (!exit){
        printf_s("$>");
        getBuffer(input);

        if (input[0] != 0 ) { // chequeo si el usuario ingreso algo
            strcpy_s(copy, input);
            exit = CommandParse(copy);

            if (exit == INPUT_ERROR) {
                printf_s(INVALID_MSG);
            }
        } 
    }
}

static void getBuffer(char * input) {
    char c;
    int i = 0;
    
    while ((c = getchar_s()) != '\n') {
        if (c == '\b') {  // Backspace handling
            if (i > 0) {
                i--;
                putchar_s('\b'); 
            }
        } else if (c != ESC) {
            if (i < (INPUT_MAX - 1)) {
                input[i++] = c;
                putchar_s(c);
            }
        }
    }
    
    input[i] = 0; 
    putchar_s('\n');
}