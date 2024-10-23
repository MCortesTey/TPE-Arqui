#include <registers.h>
#include <videoDriver.h>

void printRegStatus(regStruct *regs) {

    const char *regNames[] = {
        "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP",
        "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15",
        "SS", "CS", "RIP", "RFLAGS"
    };

    uint64_t regValues[] = {
        regs->rax, regs->rbx, regs->rcx, regs->rdx,
        regs->rsi, regs->rdi, regs->rbp, regs->rsp,
        regs->r8, regs->r9, regs->r10, regs->r11,
        regs->r12, regs->r13, regs->r14, regs->r15,
        regs->ss, regs->cs, regs->rip, regs->rflags
    };

    for (int i = 0; i < 18; i++) {
        vdPrintRegister(regNames[i], regValues[i]);
    }
}
