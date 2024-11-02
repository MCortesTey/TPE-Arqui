#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <sysCalls.h>
#include <videoDriver.h>
#include <soundDriver.h>

static void int_20();
static void int_21();
static uint64_t int_80(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10);
typedef void (*InterruptHandler)(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10);


void irqDispatcher(uint64_t irq, uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10) {
	// Se declara un arreglo de punteros a funciones de tipo InterruptHandler, con espacio para 256 funciones.
    InterruptHandler interruptions[256] = {0};
    // Se asignan funciones específicas a índices específicos del arreglo.
    interruptions[0] = &int_20; // int_20 se asigna al índice 0
    interruptions[1] = &int_21; // int_21 se asigna al índice 1
    interruptions[96] = (InterruptHandler)int_80; // int_80 se asigna al índice 96, con un cast a InterruptHandler

    // Se verifica si el valor de irq es válido (entre 0 y 255) y si hay una función asignada al índice irq.
    if(irq >= 0 && irq < 256 && interruptions[irq] != 0 ){
        // Si se cumple la condición, se obtiene el puntero a la función correspondiente.
        InterruptHandler handler = interruptions[irq];
        // Se llama a la función correspondiente, pasando los parámetros rax, rdi, rsi, rdx, r10.
        handler(rax, rdi, rsi, rdx, r10);
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
		// case 8:
		// 		fontSize();
		// 		break;
		case 9:
	 		beep(rdi, rsi);
	 		break;
		// case 10:
		// 		clearColor(rdi);
		// 		break;
		// case 11:
		// 		drawWordColorAt((char *)rdi, rsi, rdx, r10);
		// 		break;
		case 12:
		 	drawSquare(rdi, rsi, rdx, r10);
		 	break;
		case 13:
			ms_elapsed();
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
		// case 18:
		// 		bufferClearAll();
		// 		break;
        // case 19:
        //         moveScreenRight();
        //         break;
		// case 20:
        //         setCursorFlag(1);
		// 		break;
		// case 21:
        //         setCursorFlag(0);
		// 		break;
        // case 22:
        //         printImage();
        //         break;
	
		default:
				return 0;
				
	}
	return 0;
}
