#include <stdarg.h>
#include <stdio.h>

#include "functions.h"
#include "syscalls_shell.h"


void printf_s (char *fmt, ... ){
    va_list args;  
    va_start(args, fmt);  // inicializa args con los argumentos q estan despues del parametro fijo osea fmt
    
    while ( *fmt != '\0'){
        // chequeo %d %s %c
        //syscall_write(fmt, 1, STDOUT);
        fmt++;
    }
    va_end(args);
    return;
}

void putChar_s(char c){
    //syscall_write(&c, 1, STDOUT);
}

void lowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += ('a' - 'A');
    }
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