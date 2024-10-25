#include "commands.h"

static char* commands[]={
    "clear",
    "exit",
    "help",
    "invalidOp_exception",
    "show_registers",
    "snake",
    "time",
    "zero_exception"
};

#define COMMAND_COUNT sizeof(commands) / sizeof(commands[0]) // Ajustado para calcular correctamente el número de comandos

static int (*commandsFunctions[])(int argc, char* argv[]) = {
    clearCommand,
    exitCommand,
    helpCommand,
    invalidOp_exceptionCommand,
    show_registersCommand,
    snakesCommand,
    timeCommand,
    zero_exceptionCommand
};


//recibe el string del comando y retorna la funcion del comando
int CommandParse(char *commandInput){
    if(commandInput == NULL)
        return INPUT_ERROR;
    
    char *command;
    char *args[MAX_ARGS]={0};
    int argsCount;

    command= fillCommandAndArgs(args, commandInput, &argsCount);
    if(command == NULL)
        return INPUT_ERROR;

    for(int i=0; i<COMMAND_COUNT; i++){
        if(strcmp(command, commands[i]) == 0) { // Comparar correctamente
            return commandsFunctions[i](argsCount, args);
        }
    }

    return INPUT_ERROR;
}

static char* fillCommandAndArgs(char* args[], char *commandInput, int *argsCount){
    *argsCount = 0;
    char *current = commandInput;
    char *finalCommand= current;

    while(*current != 0 && *argsCount < MAX_ARGS){
        if(*current == ' '){
            *current = 0;

            if(*(current + 1) != 0 && *(current + 1) != ' ') {
                args[*argsCount] = current + 1;
                (*argsCount)++;

        }

    current++;
    }
    return finalCommand;
}

