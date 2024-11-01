#include <stdarg.h>
//#include <stdio.h>
#include <stdint.h>
#include "functions.h"
#include "syscalls_shell.h"

#define STDOUT 1
#define WHITE 0xFFFFFFFF
#define BUFFER_SIZE 256

static unsigned int log(uint64_t n, int base) {
    unsigned int count = 1;
    while (n /= base)
        count++;
    return count;
}

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
                int d = va_arg(args, int);
                char buffer[BUFFER_SIZE];
                itoa(d, buffer,10);
                for(int i = 0; buffer[i] != '\0'; i++) {
                    putchar_s(buffer[i]);
                }
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

char getchar_s() {
    char c = 0;
    while(c == 0) { 
        syscall_read(&c, 1, STDIN);
    }
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

void showTime(){
    char buff[50];
    syscall_time(buff);
    char c;
    for(int i = 0; (c = buff[i]) != 0; i++){
        putchar_s(c);
    }
    putchar_s('\n');
}

void changeSize(int size){
    syscall_changesize(size);
}
void setSize(int size){
    changeSize(size);
}

void resetSize(){
    setSize(0);
}

void drawSquare(uint64_t x, uint64_t y, uint32_t size, uint64_t color){
    syscall_drawsquare(x,y,size,color);
}

int itoa(int n, char* buffer, int base){
    if(n==0){
        buffer[0] = '0';
        buffer[1] = '\0';
        return 1;
    }

    unsigned int len=0;
    int i = 0;
    if( n<0 && base==10){
        n = -n;
        buffer[i] = '-';
        len++;
        i++;
    }
    
    len += log(n, base);
    while( n!=0){
        int res = n%base;
        buffer[len - i++ - 1] = (res > 9)? (res-10) + 'A' : res + '0';
        n /= base;
    }
    buffer[i] = '\0';
    return len;
}
int atoi(char *str) {
    int res = 0; 
    int sign = 1; 
    int i = 0; 

    if (str[0] == '-') {
        sign = -1;
        i++; 
    }

    for (; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';

    return sign*res;
}


void makeSound(uint64_t freq, uint64_t duration) {
    syscall_makeSound(freq, duration);
}