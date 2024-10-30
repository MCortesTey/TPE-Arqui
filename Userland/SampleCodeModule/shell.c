#include "shell.h"
#include "functions.h"
#include "syscalls_shell.h"
#include "commands.h"
#define INPUT_MAX 1000
#define ESC 27
#define MSG "Welcome to our shell! Type 'help' for a list of commands\n"
#define INVALID_MSG "Command '%s' not found, Type 'help' for a list of commands\n"

static void getBuffer(char * input);

void startShell() 
{
    printf_s(MSG);
    char input[INPUT_MAX] ;
    char copy[INPUT_MAX];
    int exit = 0 ;

    do{
        
        printf_s("$>");
        getBuffer(input);


        if (input[0] != 0 ) { // chequeo si el usuario ingreso algo
            strcpy_s(copy, input);
            exit = CommandParse(copy);
            if (exit == INPUT_ERROR) {

                printf_s(INVALID_MSG, input);
                exit = 0;
            }

        }
        

    }while(!exit);
    //mensaje de despedida
}


static void getBuffer(char * input) {
    char c;
    int i = 0;

    // Limpiamos el buffer antes de comenzar
    for(int j = 0; j < INPUT_MAX; j++) {
        input[j] = 0;
    }

    while ((c = getchar_s()) != '\n' && i < INPUT_MAX - 1) {  // Agregamos verificación de límite
        if (c == '\b') {
            if (i > 0) {
                i--;
                putchar_s(c);
            }
        } else {
            input[i] = c;
            i++;
            putchar_s(c);
        }
    }
    input[i] = 0;
    putchar_s('\n');
}