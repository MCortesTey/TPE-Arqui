#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h> 
#include <stdlib.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
} RegsSaved;

void sys_write(char * buff, int count, int fd);
void sys_read(char * buff, int len, int fds);
void sys_sleep(uint64_t millis);
int sys_regs_ok(RegsSaved * regs);
void saveRegs(const int64_t * regs);

#endif