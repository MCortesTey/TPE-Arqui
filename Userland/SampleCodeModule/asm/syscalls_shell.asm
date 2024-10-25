GLOBAL syscall_write
GLOBAL syscall_read

section .text

%macro syscall_entry 1 // recibe un argumento que seria numero de syscall 
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
    syscall_read 2 
