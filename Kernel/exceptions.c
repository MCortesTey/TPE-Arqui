#define ZERO_EXCEPTION_ID 0

static void zero_division();
#include "exceptions.h"
#include "videoDriver.h"
#include "lib.h"


void exceptionDispatcher(int exception, uint64_t exceptionRegisters[18]) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	else{
		invalidOperation();
	}
	printRegStatus(exceptionRegisters); // printea los registros guardados en exceptionHandler 
}

void zero_division() {
	vdPrintError("Error: Division by cero is not permitted");
	vdNewline();
}

void invalidOperation(){
	vdPrintError("Error: Invalid Operation");
	vdNewline();
}

static char * regs[] = {"RAX","RBX","RCX","RDX","RSI","RDI","RBP","RSP", "R8","R9","R10","R11","R12","R13","R14","R15", "RIP", "RFLAGS"};

void printRegStatus(uint64_t exceptionRegisters[18]){
	vdPrint("Exception registers:\n");
    for (int i = 0; i<18; i++){
        vdPrintRegister(regs[i], exceptionRegisters[i]);
    }
}
