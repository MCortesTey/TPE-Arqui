#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h> 
#include <stdlib.h>

typedef enum{
    STDOUT = 1,
    STDERR
} FDS;


void sysCaller(uint64_t rax, ...);
size_t sys_write(FDS fd, const char *buf, size_t count);

#endif