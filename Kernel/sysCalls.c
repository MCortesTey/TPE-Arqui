#include <sysCalls.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <stdint.h>
#include <stdarg.h>



void sys_write(char * buff, int count, int fd){
    switch(fd){
        case STDOUT:
            vdPrintUpto(buff, count);
            break;
        case STDERR:
            ncPrintStyle(buff, 0x04);
            break;
    }
    //CASO INVALID FD
}
void sys_read(char * c, int len, int fd){
}