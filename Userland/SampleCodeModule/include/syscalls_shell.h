#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

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


void syscall_write(char *buf, int size, int fd);
void syscall_read(char * c, int size, int fd);
void syscall_clear();
int syscall_regs_ok(RegsSaved *regs);
void syscall_time(char *buf);
void syscall_changesize(int a);
void syscall_drawsquare(uint64_t x, uint64_t y, uint32_t size, uint64_t color);
void syscall_sleep(uint64_t millis);
void syscall_makeSound(uint64_t freq, uint64_t duration);
void _invalidOpcode();
void zero_division();

#endif
