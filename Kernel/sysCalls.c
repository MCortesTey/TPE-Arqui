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

void sysCaller(uint64_t rax, ...) { //funcion para manejar diferentes syscall
    // va_list args;
    // va_start(args, rax);
    // switch(rax){
    //     case 4:; //sys_write
    //         FDS fd = va_arg(args, FDS);
    //         const char* buf = va_arg(args, const char*);
    //         size_t count = va_arg(args, size_t);
    //         sys_write(fd, buf, count);
    //         break;
    // }
    // va_end(args);
    return;
}
