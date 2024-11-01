#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <stdint.h>
#include <stdio.h>


typedef struct regStruct{
	uint64_t rax, rbx, rcx, rdx;
	uint64_t rsi, rdi, rbp, rsp;
	uint64_t  r8,  r9, r10, r11;
	uint64_t r12, r13, r14, r15;
	uint64_t ss, cs, rflags, rip;
} regStruct;

extern regStruct regBackup;
extern int regsChecked;


void printRegStatus (int n ); // regStruct * regs


#endif
