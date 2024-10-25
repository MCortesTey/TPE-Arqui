
#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2


void syscall_write(int fd, char *buf, int size, uint32_t color);
void syscall_read(char * c, int size, int fd);
#endif
