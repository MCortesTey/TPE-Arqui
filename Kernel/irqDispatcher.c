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

uint64_t int_80(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10){
	switch(rax){
		case 1: 
			sys_write((char *)rdi, rsi, rdx);
			break;
		case 2:
			sys_read((char *)rdi, rsi, rdx);
			break;
		case 3: 
			TimeClock((char *)rdi); 
			break;
		case 4:
		 	return sys_regs_ok((RegsSaved *) rdi); 
			break;
		case 5: 
			changeSize(rdi);
			break;
		case 6: 
			sys_sleep(rdi);
			break;
		case 7: 
			clear();
	 		break;
		case 9:
	 		beep(rdi, rsi);
	 		break;
		case 12:
		 	drawSquare(rdi, rsi, rdx, r10);
		 	break;
		case 13:
			timer_ms((int *)rdi);
			break;
		// case 14:
		// 		sleeps(rdi);
		// 		break;
		// case 15:
		// 		pixelColorAt(rdi, rsi);
		// 		break;
		case 16:
				return getPos();
				break;
		case 17:
				return getBuffCharAt(rdi);
				break;
	
		default:
				return 0;
				
	}
	return 0;
}
