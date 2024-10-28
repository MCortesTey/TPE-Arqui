GLOBAL syscall_write
GLOBAL syscall_read
GLOBAL syscall_clear

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

syscall_clear:
    syscall_entry 7