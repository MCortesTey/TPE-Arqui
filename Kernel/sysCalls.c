#include <sysCalls.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <stdint.h>
#include <stdarg.h>
#include <interrupts.h>



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
    
    int i;

    if (fd == STDIN) {
        char aux = 0;
        for (i = 0; i < len; i++) {
            _hlt(); //no avanzo hasta que haya otra interrupcion
            aux = getBuffAtCurrent();
            c[i] = aux;
            buffNext();
        }
    }
    return;
    // else{
    //     invalidFD();
    // }
}
