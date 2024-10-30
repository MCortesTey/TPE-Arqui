#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>


void printf_s(char *fmt, ... );
void putchar_s(char c);
void putcharColor(char c, uint32_t color);
char getchar_s();
int strcmp_s(char *s1, char *s2);
void strcpy_s(char *dest, const char *src);
void clearScreen();
void showRegisters();
void showTime();
void changeSize(int a);


#endif