#include <stdarg.h>
//#include <stdio.h>
#include "functions.h"
#include "syscalls_shell.h"

#define STDOUT 1
#define WHITE 0xFFFFFFFF
#define BUFFER_SIZE 256


void putcharColor(char c, uint32_t color) {
    syscall_write(STDOUT, &c, 1, color);
}

void putchar(char c) {
    putcharColor(c, WHITE);
}

void printf(char *fmt, ... ){
    va_list args;  
    va_start(args, fmt);  // inicializa args con los argumentos q estan despues del parametro fijo osea fmt

    char buffer[BUFFER_SIZE];
    while ( *fmt != '\0'){
        if ( *fmt == '%'){
            fmt++;
            switch (*fmt)
            {
            case 's':
                char * str = va_arg(args, char *); // la primera vez que se llama a va_arg me devuelve el primer arg, en la segunda vez el segundo arg y asi
                putchar(fmt);
                break;
            
            case 'd':
                int d = va_arg(args, int *);
                // hacer funcion para pasar de int a str y guardar en buffer
                putchar(fmt);
                break;
            case 'c':
                char c = va_arg(args, int);
                putchar(fmt);
                break;
            }
        }
        else {
            //syscall_write(fmt, 1, STDOUT);
        }
        fmt++;
    }
    va_end(args);
    return;
}

char getchar(){
    char c;
    syscall_read(&c, 1, STDIN);
    return c;
}


int strcmp(char *s1, char *s2)
{
    int i, toReturn = 0, checked = 0;
    for (i = 0; s1[i] && s2[i]; i++)
    {
        if (!checked)
        {
            toReturn += (s1[i] - s2[i]);
            checked = toReturn == 0 ? 0 : 1;
        }
    }
    if (s1[i])
    {
        toReturn = s1[i];
    }
    else if (s2[i])
    {
        toReturn = s2[i];
    }
    return toReturn;
}