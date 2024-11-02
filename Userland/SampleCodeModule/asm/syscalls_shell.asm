GLOBAL syscall_write
GLOBAL syscall_read
GLOBAL syscall_clear
GLOBAL syscall_regs_ok
GLOBAL syscall_time
global syscall_changesize
global syscall_drawsquare
global syscall_sleep

global syscall_timerms
global syscall_makeSound
global syscall_getbufferpos
global syscall_getcharat
global _invalidOpcode
global zero_division
section .text

%macro syscall_entry 1 ; recibe un argumento que seria numero de syscall 
    push rbp
    mov rbp, rsp
    mov rax, %1
    int 80h
    mov rsp, rbp
    pop rbp
    ret
%endmacro

syscall_write:
    syscall_entry 1

syscall_read:
    syscall_entry 2 

syscall_time:
    syscall_entry 3

syscall_regs_ok:
    syscall_entry 4

syscall_changesize:
    syscall_entry 5

syscall_sleep:
    syscall_entry 6

syscall_clear:
    syscall_entry 7

syscall_makeSound:
    syscall_entry 9

syscall_drawsquare:
    syscall_entry 12

syscall_timerms:
    syscall_entry 13
syscall_getbufferpos:
    syscall_entry 16

syscall_getcharat:
    syscall_entry 17
_invalidOpcode:
    ud2

zero_division:
    mov rax, 1      ; Dividendo
    xor rcx, rcx    ; Divisor = 0
    div rcx      