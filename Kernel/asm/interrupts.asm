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
EXTERN keyboard_handler

GLOBAL exceptregs
GLOBAL registers
GLOBAL capturedReg

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
	
	;xor rax, rax
	;in al, 0x60
	;cmp al, 0x1D ; ctrl 
	;jne .no_control
	
	;RAX, RBX, RCX, RDX, RSI, RDI, RBP, R8, R9, R10, R11, R12, R13, R14, R15, RSP, RIP, RFLAGS
   	mov [registers+8*1],	rbx
	mov [registers+8*2],	rcx
	mov [registers+8*3],	rdx
	mov [registers+8*4],	rsi
	mov [registers+8*5],	rdi
	mov [registers+8*6],	rbp
	mov [registers+8*7], r8
	mov [registers+8*8], r9
	mov [registers+8*9], r10
	mov [registers+8*10], r11
	mov [registers+8*11], r12
	mov [registers+8*12], r13
	mov [registers+8*13], r14
	mov [registers+8*14], r15

	mov rax, rsp
	add rax, 160			  ;volvemos a antes de pushear los registros
	mov [registers + 8*15], rax  ;RSP

	mov rax, [rsp+15*8]
	mov [registers + 8*16], rax ;RIP

	mov rax, [rsp + 14*8]	;RAX
	mov [registers], rax

	mov rax, [rsp+15*9]
	mov [registers + 8*17], rax ;RFLAGS

	;mov byte [capturedReg], 1
	;jmp .exit

;.no_control:
	;cmp al, 0x9D	
	;je .exit

	call keyboard_handler
	
;.exit:
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
	; guardamos los registros en este orden: 
	;RAX, RBX, RCX, RDX, RSI, RDI, RBP, R8, R9, R10, R11, R12, R13
	; R14, R15, RSP,RIP, RFLAGS
    mov [exceptregs+8*0],	rax
	mov [exceptregs+8*1],	rbx
	mov [exceptregs+8*2],	rcx
	mov [exceptregs+8*3],	rdx
	mov [exceptregs+8*4],	rsi
	mov [exceptregs+8*5],	rdi
	mov [exceptregs+8*6],	rbp
	mov [exceptregs+8*7], r8
	mov [exceptregs+8*8], r9
	mov [exceptregs+8*9], r10
	mov [exceptregs+8*10], r11
	mov [exceptregs+8*11], r12
	mov [exceptregs+8*12], r13
	mov [exceptregs+8*13], r14
	mov [exceptregs+8*14], r15

	mov rax, rsp
	add rax, 160			  ;volvemos a antes de pushear los registros
	mov [exceptregs+ 8*15], rax  ;rsp
	mov rax, [rsp+15*8]
	mov [exceptregs + 128], rax ;rip
	mov rax, [rsp+15*9]
	mov [exceptregs + 136], rax ;rflags


	mov rdi, %1
	call exceptionDispatcher

	popState
	add rsp, 8
	push load_main
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

saveRegisters:
	pushState
	saveRegistersASM
	popState
	ret


printRegStatusASM:
	mov rdi, 0
	call printRegStatus
	ret

section .bss 
	aux resq 1	
	capturedReg resb 1		;flag para saber si se capturo un teclado
	exceptregs resq 18	;registros para la excepcion
	registers resq 18		;registros para el teclado
	