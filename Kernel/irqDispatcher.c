#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <sysCalls.h>
#include <videoDriver.h>
#include <soundDriver.h>

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq, uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10) {
	
    // Se verifica si el valor de irq es válido (entre 0 y 255) y si hay una función asignada al índice irq.
    if(irq < 2){
        switch(irq){
            case 0:
                int_20();
                break;
            case 1:
                int_21();
                break;	
        }
    }
}

void int_20() {
	timer_handler();
}

void int_21(){
	keyboard_handler();
}
