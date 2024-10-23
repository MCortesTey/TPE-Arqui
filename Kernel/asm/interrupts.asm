
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
GLOBAL _irq60Handler

GLOBAL _exception0Handler
GLOBAL saveRegisters
GLOBAL printRegStatusASM

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN printRegStatus


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



%macro exceptionHandler 1
	pushState
	//guardado en Struct de registers

	//muevo a rsi los register
	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

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
	irqHandlerMaster 1

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
_irq60Handler: 
	pushState

	mov rdi, rax
	mov rsi, rbx
	mov rdx, rcx
	
	call sysCaller

	popState ; Restaura el estado guardado
	iretq ; Retorna de la interrupción

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

haltcpu:
	cli
	hlt
	ret



; macro para guardar los registros en regBackup (esta en section.bss) en orden en que se los pushean en pushState

%macro saveRegistersASM 0  ;0 porque no recibe argumentos la macro

    mov [regBackup.r_rbp], rbp  ; Guarda el valor de rbp

    mov rbp, [rsp]              ; rbp
    mov [regBackup.r_r15], rbp

    mov rbp, [rsp + 8]          ; r15
    mov [regBackup.r_r14], rbp

    mov rbp, [rsp + 16]         ; r14
    mov [regBackup.r_r13], rbp

    mov rbp, [rsp + 24]         ; r13
    mov [regBackup.r_r12], rbp

    mov rbp, [rsp + 32]         ; r12
    mov [regBackup.r_r11], rbp

    mov rbp, [rsp + 40]         ; r11
    mov [regBackup.r_r10], rbp

    mov rbp, [rsp + 48]         ; r10
    mov [regBackup.r_r9], rbp

    mov rbp, [rsp + 56]         ; r9
    mov [regBackup.r_r8], rbp

    mov rbp, [rsp + 64]         ; rsi
    mov [regBackup.r_rsi], rbp

    mov rbp, [rsp + 72]         ; rdi
    mov [regBackup.r_rdi], rbp

    mov rbp, [rsp + 88]         ; rdx
    mov [regBackup.r_rdx], rbp

    mov rbp, [rsp + 96]         ; rcx
    mov [regBackup.r_rcx], rbp

    mov rbp, [rsp + 104]        ; rbx
    mov [regBackup.r_rbx], rbp

    mov rbp, [rsp + 112]        ; rax
    mov [regBackup.r_rax], rbp

    mov rbp, [rsp + 120]        ; rip
    mov [regBackup.r_rip], rbp

    mov rbp, [rsp + 128]        ; cs
    mov [regBackup.r_rcs], rbp

    mov rbp, [rsp + 136]        ; rflags
    mov [regBackup.r_rflags], rbp

    mov rbp, [rsp + 144]        ; rsp
    mov [regBackup.r_rsp], rbp

    mov rbp, [rsp + 152]        ; ss
    mov [regBackup.r_rss], rbp

    mov rbp, [regBackup.r_rbp]  ; Restaura el valor original de rbp

%endmacro


saveRegisters:
	push pushState
	saveRegistersASM
	popState
	ret

;printRegStatusASM:
;	mov qword rdi, regBackup
;	call printRegStatus
;	ret

section .bss 
	aux resq 1	
	
	GLOBAL regBackup
	regBackup:
	.r_rax resq 1
	.r_rbx resq 1
	.r_rcx resq 1
	.r_rdx resq 1
	.r_rsi resq 1
	.r_rdi resq 1
	.r_rsp resq 1
	.r_rbp resq 1 
	.r_r8 resq 1
	.r_r9 resq 1
	.r_r10 resq 1
	.r_r11 resq 1
	.r_r12 resq 1
	.r_r13 resq 1
	.r_r14 resq 1
	.r_r15 resq 1
	.r_rss resq 1
	.r_rcs resq 1
	.r_rip resq 1
	.r_rflags resq 1
