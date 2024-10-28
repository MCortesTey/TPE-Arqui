#include <stdarg.h>
//#include <stdio.h>
#include <stdint.h>
#include "functions.h"
#include "syscalls_shell.h"

#define STDOUT 1
#define WHITE 0xFFFFFFFF
#define BUFFER_SIZE 256


// void putcharColor(char c, uint32_t color) { //puede que esta no la necesitemos 
//     syscall_write(&c, 1, STDOUT, color);
// }

// void putchar_s(char c) {
//     putcharColor(c, WHITE);
// }


void putchar_s(char c){
    syscall_write(&c, 1, STDOUT);
}

void printf_s(char *fmt, ... ){
    va_list args;  
    va_start(args, fmt);  // inicializa args con los argumentos q estan despues del parametro fijo osea fmt

    //char buffer[BUFFER_SIZE];
    while ( *fmt != '\0'){
        if ( *fmt == '%'){
            fmt++;
            switch (*fmt)
            {
            case 's':{
                char * str = va_arg(args, char *); // la primera vez que se llama a va_arg me devuelve el primer arg, en la segunda vez el segundo arg y asi
                for (int j=0; str[j] != '\0'; j++) {
                        putchar_s(str[j]);
                    }
                break;
            }
            case 'd':{
                //int d = va_arg(args, int );
                // hacer funcion para pasar de int a str y guardar en buffer
                // putchar de cada caracter del string q esta en buffer
                break;
            }
            case 'c':{
                char c = va_arg(args, int);
                putchar_s(c);
                break;
            }
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

char getchar_s(){
    char c;
    syscall_read(&c, 1, STDIN);
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

void strcpy_s(char *dest, const char *src) {
    size_t i = 0;
    while(src[i] != 0) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void clearScreen(){
    syscall_clear();
}

void showRegisters(){
    syscall_showRegisters();
}