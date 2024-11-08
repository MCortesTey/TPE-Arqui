#include <sysCalls.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>
#include <interrupts.h>
#include <soundDriver.h>

//RegsSaved definido en sysCalls.h
uint64_t registers[17];
uint64_t regsChecked;

void sys_write(char * buff, int count, int fd){
    switch(fd){
        case STDOUT:
            vdPrintUpto(buff, count);
            break;
        case STDERR:
            vdPrintColorUpto(buff, 0x00FF0000, 0x00000000, count);
            
            break;
    }
    
}

void sys_read(char * c, int len, int fd){
    if (fd == STDIN) {
        for (int i = 0; i < len; i++) {
            while(1) {
                _hlt();
                char aux = getBuffAtCurrent();
                if (aux != 0) {
                    c[i] = aux;
                    buffNext();
                    break;
                }
            }
        }
    }
    return;
}

void sys_sleep(uint64_t millis){
	int startTime = ticks_elapsed();
	while (millis > ticks_elapsed()* 55 - startTime * 55){
		_hlt();
	}
}

int sys_regs_ok ( RegsSaved * regs){
    
    if(regsChecked == 0) {
        return 0;
    }

    regs->rax = registers[0];
    regs->rbx = registers[1];
    regs->rcx = registers[2];
    regs->rdx = registers[3];
    regs->rsi = registers[4];
    regs->rdi = registers[5];
    regs->rbp = registers[6];
    regs->rsp = registers[7];
    regs->r8 = registers[8];
    regs->r9 = registers[9];
    regs->r10 = registers[10];
    regs->r11 = registers[11];
    regs->r12 = registers[12];
    regs->r13 = registers[13];
    regs->r14 = registers[14];
    regs->r15 = registers[15];
    regs->rip = registers[16];


    return 1;
}

void saveRegs(const int64_t *regs) {
    for (int i = 0; i < 17; i++)
        registers[i] = regs[i];
    regsChecked = 1;
}