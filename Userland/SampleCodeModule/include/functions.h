#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

void putchar_s(char c);
void printf_s(char *fmt, ... );
char getchar_s();
int strcmp_s(char *s1, char *s2);
void strcpy_s(char *dest, const char *src);
void clearScreen();
void showRegisters();
void showTime();
void changeSize(int size);
void setSize(int size);
void resetSize();
void drawSquare(uint64_t x, uint64_t y, uint32_t size, uint64_t color);
int itoa(int n, char* buffer, int base);
int atoi(char *str);
void makeSound(uint64_t freq, uint64_t duration);
void setSeed();
unsigned long lcg();
int getRandom();
int getRandomInRange(int min, int max);

#endif