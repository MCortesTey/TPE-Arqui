#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdarg.h>

void printf(char *fmt, ... );
void putchar(char c);
void putcharColor(char c, uint32_t color);
char getchar();
void lowercase(char *str);
int strcmp(char *s1, char *s2);
void strcpy(char *dest, const char *src);

#endif