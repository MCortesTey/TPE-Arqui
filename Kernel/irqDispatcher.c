#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <sysCalls.h>

static void int_20();
static void int_21();
static uint64_t int_80(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10);
static void (*interrupt_arr[])(void) = {int_20,int_21};

void irqDispatcher(uint64_t irq) {
	interrupt_arr[irq]();
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
		// case 3: 
		// 		TimeClock((char *)rdi); 
		// 		break;
		// case 4:
		// 		printRegistersASM(); 
		// 		break;
		// case 5: 
		// 		sizeUp();
		// 		break;
		// case 6: 
		// 		sizeDown();
		// 		break;
		// case 7: 
		// 		clear();
		// 		break;
		// case 8:
		// 		fontSize();
		// 		break;
		// case 9:
		// 		beep(rdi, rsi);
		// 		break;
		// case 10:
		// 		clearColor(rdi);
		// 		break;
		// case 11:
		// 		drawWordColorAt((char *)rdi, rsi, rdx, r10);
		// 		break;
		// case 12:
		// 		putSquare(rdi, rsi, rdx, r10);
		// 		break;
		// case 13:
		// 		sleepms(rdi);
		// 		break;
		// case 14:
		// 		sleeps(rdi);
		// 		break;
		// case 15:
		// 		pixelColorAt(rdi, rsi);
		// 		break;
		// case 16:
		// 		return getPos();
		// 		break;
		// case 17:
		// 		return getBuffCharAt(rdi);
		// 		break;
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
