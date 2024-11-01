#include <sysCalls.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>
#include <interrupts.h>
#include <soundDriver.h>

void sys_write(char * buff, int count, int fd){
    switch(fd){
        case STDOUT:
            vdPrintUpto(buff, count);
            break;
        case STDERR:
            vdPrintColorUpto(buff, 0x00FF0000, 0x00000000, count);
            //ncPrintStyle(buff, 0x04);
            break;
    }
    //CASO INVALID FD
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

void sys_sleep(uint64_t millis) {
    unsigned long long initial_time = ms_elapsed(); // tiempo
    unsigned long long currentTime = initial_time;
    _sti();// Habilito interrupciones para permitir que el sistema siga funcionando
    while ((currentTime - initial_time) <= millis) { //espero transcurra lo especificado
        currentTime = ms_elapsed();
    }
    
    _cli();// Deshabilito interrupciones para asegurar que el sistema no se interrumpa
}

