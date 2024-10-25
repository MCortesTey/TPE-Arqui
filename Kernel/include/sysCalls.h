#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h> 
#include <stdlib.h>

typedef enum{
    STDOUT = 1,
    STDERR
} FDS;


void sys_write(char * buffer, int len, int fd);
void sys_read(char * buff, int len, int fds);
int getShiftVal();

#endif