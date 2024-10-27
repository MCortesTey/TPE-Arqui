#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h> 
#include <stdlib.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2


void sys_write(char * buff, int count, int fd);
void sys_read(char * buff, int len, int fds);

#endif