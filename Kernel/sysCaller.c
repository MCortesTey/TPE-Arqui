#include <sysCaller.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <stdarg.h>


size_t sys_write(FDS fd, const char* buff, size_t count){
    switch(fd){
        case STDOUT:
            ncPrintStyle(buff, 0x07);
            break;
        case STDERR:
            ncPrintStyle(buff, 0x04);
            break;
    }
    return 1;
}

void sysCaller(uint64_t rax, ...) { //funcion para manejar diferentes syscall
    va_list args;
    va_start(args, rax);
    switch(rax){
        case 4:; //sys_write
            FDS fd = va_arg(args, FDS);
            const char* buf = va_arg(args, const char*);
            size_t count = va_arg(args, size_t);
            sys_write(fd, buf, count);
            break;
    }
    va_end(args);
    return;
}
