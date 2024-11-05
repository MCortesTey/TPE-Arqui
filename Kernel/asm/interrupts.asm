GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq80Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL saveRegisters
GLOBAL printRegStatusASM

EXTERN irqDispatcher
EXTERN sysDispatcher
EXTERN exceptionDispatcher
EXTERN printRegStatus
EXTERN load_main
EXTERN regsReady
EXTERN saveRegs
EXTERN keyboardDriver
EXTERN getStackBase

GLOBAL exception_regs
;GLOBAL registers
;GLOBAL regsChecked

EXTERN sysCaller

SECTION .text


%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	pushState

	mov rdi, 1 
	call irqDispatcher

	call regsReady
	cmp rax, 1
	jne .exit

	popState
	pushState

.saveRegs:
	
    mov [registers + 8 * 0 ], rax
    mov [registers + 8 * 1 ], rbx
    mov [registers + 8 * 2 ], rcx
    mov [registers + 8 * 3 ], rdx
    mov [registers + 8 * 4 ], rsi
    mov [registers + 8 * 5 ], rdi
    mov [registers + 8 * 6 ], rbp
    mov rax, [rsp+18*8]
    ;add rax, 16 * 8 ; es lo que se decremento rsp con la macro pushState y el pusheo de la dir. de retorno
    mov [registers + 8 * 7 ], rax ;rsp

    mov [registers + 8 * 8 ], r8
    mov [registers + 8 * 9 ], r9
    mov [registers + 8 * 10], r10
   	mov [registers + 8 * 11], r11
   	mov [registers + 8 * 12], r12
   	mov [registers + 8 * 13], r13
   	mov [registers + 8 * 14], r14
   	mov [registers + 8 * 15], r15
   	mov rax, [rsp+15*8]; posicion en el stack de la dir. de retorno (valor del rip previo al llamado de la interrupcion)
	mov [registers + 8 * 16], rax

    mov rdi, registers
    call saveRegs

.exit:
	mov al, 20h
	mov al, 20h
	out 20h, al

	popState
	iretq	

;Keyboard
;_irq01Handler:
;	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;SysCaller
; para maneja las interrupciones del sistema
_irq80Handler: 
	push rbp
	mov rbp, rsp
	push rbx
	push r12
	push r13
	push r14
	push r15
	


	mov r9, rcx
	mov r8, rdx
	mov rcx, rsi
	mov rdx, rdi
	mov rsi, rax
	mov rdi, 60h
	call irqDispatcher

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	mov rsp, rbp
	pop rbp
	iretq

%macro exceptionHandler 1

	pushState
	mov [exception_regs + 8*0 ], rax
	mov [exception_regs + 8*1 ], rbx
	mov [exception_regs + 8*2 ], rcx
	mov [exception_regs + 8*3 ], rdx
	mov [exception_regs + 8*4 ], rsi
	mov [exception_regs + 8*5 ], rdi
	mov [exception_regs + 8*6 ], rbp
	; mov rax, rsp
    ; add rax, 16 * 8                     ; RSP del contexto anterior
    mov rax, [rsp + 18 * 8]
	mov [exception_regs + 8*7 ], rax	;
	mov [exception_regs + 8*8 ], r8
	mov [exception_regs + 8*9 ], r9
	mov [exception_regs + 8*10], r10
	mov [exception_regs + 8*11], r11
	mov [exception_regs + 8*12], r12
	mov [exception_regs + 8*13], r13
	mov [exception_regs + 8*14], r14
	mov [exception_regs + 8*15], r15
	mov rax, [rsp+15*8]                     ;RIP del contexto anterior
	mov [exception_regs + 8*16], rax
	mov rax, [rsp+17*8]                     ; RFLAGS
	mov [exception_regs + 8*17], rax

	mov rdi, %1                             ; Parametros para exceptionDispatcher
	mov rsi, exception_regs

	call exceptionDispatcher

	popState
    call getStackBase
	mov [rsp+24], rax ; El StackBase
    mov rax, userland
    mov [rsp], rax ; PISO la dirección de retorno

    sti
    iretq

%endmacro

_exception0Handler:
	exceptionHandler 0
	jmp haltcpu

_exception6Handler:	
	exceptionHandler 6
	jmp haltcpu

haltcpu:
	cli
	hlt
	ret


section .bss 
	aux resq 1	
	registers resq 17
	
section .data
	exception_regs dq 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ; 18 zeros

SECTION .rodata
	userland equ 0x400000
	