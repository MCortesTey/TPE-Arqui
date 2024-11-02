#include <registers.h>
#include <videoDriver.h>

//int regsChecked = 0 ;
/*
void printRegStatus(regStruct *regs) {

    if ( !regsChecked ){
        vdPrintError("Error, registers are not updated. Press ESC to update.\n");
        return;
    }
    
    char *regNames[] = {
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

    for (int i = 0; i < 20 ; i++) {
        vdPrintRegister(regNames[i], regValues[i]);
    }
    vdEndPrintRegisters();
    
}
*/


uint64_t reg_snap[18] = {0};      //valores de los registros
static char * regs[] = {"RAX","RBX","RCX","RDX","RSI","RDI","RBP","RSP", "R8","R9","R10","R11","R12","R13","R14","R15", "RIP", "RFLAGS"};
extern  uint64_t exception_regs[18];
extern const uint64_t registers[18];
extern uint64_t regsChecked;

void printRegStatus(int n) {
    if ( n == 0 ){   //regsChecked cambia a 1 cuando se presiona esc
        if ( regsChecked == 0 ){   
        vdPrintError("Error, registers are not updated. Press ESC to update.\n");
        return;
        }
        for (int i = 0; i<17; i++){
            vdPrintRegister(regs[i], registers[i]);
        }
    } else {
        vdPrint("Exception registers:\n");
        for (int i = 0; i<18; i++){
            vdPrintRegister(regs[i], exception_regs[i]);
        }
    }
}
