#include "commands.h"
#include "functions.h"
#include "snakes.h"
#include "syscalls_shell.h"

#define COMMAND_COUNT sizeof(commands) / sizeof(commands[0]) // Ajustado para calcular correctamente el número de comandos

static char * commands[][2] = {
        {"clear", "Clears the screen."},
        {"exit", "Exits the shell."},
        {"help", "Shows the available commands."},
        {"invalidOpcode", "Test for the Invalid Opcode Exception"},
        {"zero", "Test for the Zero Division Exception"},
        {"inforeg", "Shows the registers values."},
        {"snakes", "Starts the Snakes game."},
        {"time", "Shows the current time."},
        {"sizeUp","Increment the size of the font."},
        {"sizeDown","Decrement the size of the font."},
        {"beep", "Make a beep sound."}
};

static int (*commandsFunctions[])(int argc, char* argv[]) = {
    clearCommand,
    exitCommand,
    helpCommand,
    invalidOp_exceptionCommand,
    zero_exceptionCommand,
    show_registersCommand,
    snakesCommand,
    timeCommand,
    size_up,
    size_down,
    beepCommand
};

int beepCommand(int argc, char* argv[]) {
    syscall_makeSound(2000, 1000);  // Frecuencia más alta y duración más corta
    return 0;
}


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
        if(strcmp_s(command, commands[i][0]) == 0) { // Comparar correctamente
            return commandsFunctions[i](argsCount, args);
        }
    }

    return INPUT_ERROR;
}


char* fillCommandAndArgs(char* args[], char *commandInput, int *argsCount){
    *argsCount = 0;
    char *current = commandInput;
    char *finalCommand = current;

    while(*current == ' ') {
        current++;
    }
    finalCommand = current;

    while(*current != 0 && *argsCount < MAX_ARGS){
        if(*current == ' '){
            *current = 0;
            current++;
            
            // Saltamos espacios múltiples
            while(*current == ' ') {
                current++;
            }

            if(*current != 0) {
                args[*argsCount] = current;
                (*argsCount)++;
            }
        } else {
            current++; 
        }
    }
    return finalCommand;
}

int helpCommand(int argc, char * argv[] ){
    for (int i=0; i<COMMAND_COUNT; i++)
    {
        printf_s("%s: %s\n", commands[i][0], commands[i][1]);
    }
    return 0;
}


int clearCommand(int argc, char *argv[]) {
    clearScreen();
    return 0;
}

int exitCommand(int argc, char *argv[]) {
    clearScreen();
    return EXIT;  
}

int invalidOp_exceptionCommand(int argc, char* argv[]) {
    _invalidOpcode();
    return 0;  
}

int show_registersCommand(int argc, char* argv[]) {
    showRegisters();
    return 0;
}

int snakesCommand(int argc, char* argv[]) {
    clearScreen();
    snakes();
    return 0;  
}

int timeCommand(int argc, char* argv[]) {
    showTime();
    return 0; 
}

int zero_exceptionCommand(int argc, char *argv[]) {
    zero_division();
    return 0;
}

int size_up(int argc, char *argv[]) {
    changeSize(1);
    clearScreen();
    return 0;
}
int size_down(int argc, char *argv[]) {
    changeSize(-1);
    clearScreen();
    return 0;
}
