#include "shell.h"
#include "functions.h"
#include "syscalls_shell.h"
#define INPUT_MAX 1000

static char *instructions[] = {"help", "zero_division", "invalid_opcode", "time", "registers",  "snakes", 0};
static int functionId(char *buffer);

int startShell() 
{
    char *msg = "Welcome to our shell! Type 'help' for a list of commands.\n";
    printf_s(msg);
    
    int exit = 0 ;
    char buffer[INPUT_MAX] = {0};

    while (!exit){
        printf_s("$>");
        int id = functionId(buffer);
        functionsHandler(id);
    }
    return 0;
}

void functionsHandler(int id){

}

static int functionId(char *buffer){ //recibe el buffer y devuelve el indice de la funcion q ingreso el usuario 
    char functionInput[INPUT_MAX] = {0};  // solo va a guardar el modulo que el usuario le pide, no los argumentos tmb
    int i = 0;

    while(i < INPUT_MAX && buffer[i] != 0 && buffer[i] != ' ' && buffer[i] != '\t' )
    {
        functionInput[i] = buffer[i];
        lowerCase(functionInput);
        i++;
    }
    if ( i == INPUT_MAX && functionInput[i] != 0 ) // verifica si el usuario ingreso un comando mas largo del esperado
    {
        return -1;
    }
    i = 0;
    while (instructions[i] != 0)
    {
        if (strcmp_s(functionInput, instructions[i]) == 0)   // se fija si lo que ingreso el usuario es alguna de las instrucciones posibles
        {
            return i; 
        }
        i++;
    }
    
}