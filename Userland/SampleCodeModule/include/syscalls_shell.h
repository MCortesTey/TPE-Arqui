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
void _invalidOpcode();

#endif
