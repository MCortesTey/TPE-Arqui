#include <stdint.h>
#include <sysCalls.h>

enum syscalls {WRITE = 0, READ};

void sysDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax) {
    switch (rax) {
        case WRITE: sys_write((char *) rdi, rsi, rdx );
    }
}