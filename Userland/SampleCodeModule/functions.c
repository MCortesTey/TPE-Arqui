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
            }case 'x': {
                uint64_t x = va_arg(args, uint64_t);
                static char buffer[64];
                char * ptr = &buffer[63];
                *ptr = '\0';
                do {
                    *--ptr = "0123456789abcdef"[x % 16];
                    x /= 16;
                } while(x != 0);
                char * hex_str = ptr;
                for(int i = 0; hex_str[i] != '\0'; i++) {
                    putchar_s(hex_str[i]);
                }
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
    RegsSaved regs;
    int regsChecked = syscall_regs_ok(&regs) ;

    if(regsChecked == 0) {
        printf_s("No register snapshot available. Press ESC to take a snapshot.\n");
        return;
    } 
    
    printf_s("Register snapshot:\n");
    printf_s("rax: 0x%x\n", regs.rax);
    printf_s("rbx: 0x%x\n", regs.rbx);
    printf_s("rcx: 0x%x\n", regs.rcx);
    printf_s("rdx: 0x%x\n", regs.rdx);
    printf_s("rsi: 0x%x\n", regs.rsi);
    printf_s("rdi: 0x%x\n", regs.rdi);
    printf_s("rbp: 0x%x\n", regs.rbp);
    printf_s("rsp: 0x%x\n", regs.rsp);
    printf_s("r8:  0x%x\n", regs.r8 );
    printf_s("r9:  0x%x\n", regs.r9 );
    printf_s("r10: 0x%x\n", regs.r10);
    printf_s("r11: 0x%x\n", regs.r11);
    printf_s("r12: 0x%x\n", regs.r12);
    printf_s("r13: 0x%x\n", regs.r13);
    printf_s("r14: 0x%x\n", regs.r14);
    printf_s("r15: 0x%x\n", regs.r15);
    printf_s("rIP: 0x%x\n", regs.rip);
    
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

static long seed = 1; // Semilla inicial

void setSeed() {
    syscall_timerms(&seed);
}

// Función LCG para generar un número aleatorio
unsigned long lcg() {
    seed = (1103515245 * seed + 12345) % (1UL << 31); // Parámetros típicos
    return seed;
}

// Función para obtener un número aleatorio
int getRandom() {
    setSeed();
    return lcg(); // Genera un número aleatorio usando LCG
}

int getRandomInRange(int min, int max) {
    setSeed();
    return (lcg() % (max - min + 1)) + min; // Genera un número en el rango [min, max]
}