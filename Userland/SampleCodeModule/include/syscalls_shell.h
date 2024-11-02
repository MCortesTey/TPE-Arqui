#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2


void syscall_write(char *buf, int size, int fd);
void syscall_read(char * c, int size, int fd);
void syscall_clear();
void syscall_showRegisters();
void syscall_time(char *buf);
void syscall_changesize(int a);
void syscall_drawsquare(uint64_t x, uint64_t y, uint32_t size, uint64_t color);
void syscall_sleep(uint64_t millis);
uint64_t syscall_timerms();
void syscall_makeSound(uint64_t freq, uint64_t duration);
int syscall_getbufferpos();
char syscall_getcharat(int pos);
void _invalidOpcode();
void zero_division();

#endif
