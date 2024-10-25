#include <stdarg.h>
#include <stdio.h>

#include "functions.h"
#include "syscalls_shell.h"


void printf_s (char *fmt, ... ){
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
                //syscall_write(str, strlen(s), STDOUT);
                break;
            
            case 'd':
                int d = va_arg(args, int *);
                // hacer funcion para pasar de int a str y guardar en buffer
                //syscall_write(buffer, strlen(buffer), STDOUT);
                break;
            case 'c':
                char c = va_arg(args, int);
                //syscall_write(&c, 1, STDOUT);
                break;
            }
        }
        else {
            syscall_write(fmt, 1, STDOUT);
        }
        fmt++;
    }
    va_end(args);
    return;
}

void putChar_s(char c){
    //syscall_write(&c, 1, STDOUT);
}

char getChar_s (){
    char c;
    //syscall_read(&c, 1, STDIN);
    return c;
}


int strcmp_s(char *s1, char *s2)
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