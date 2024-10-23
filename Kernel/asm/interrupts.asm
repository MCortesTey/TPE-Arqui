
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

EXTERN irqDispatcher
EXTERN exceptionDispatcher

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



; macro para guardar los registros para el modulo de registros y cada vez que se relice las excepciones
; cuando se "llama" a la macro en las funciones se la llama como 'saveRegistersASM regArray"
;cuando se relizan las excepciones llamar a esta macro para que guarde los registros y cambiar el regChecked a 1
; orden en que se guardan : rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15, rip, rflags

%macro saveRegistersASM 1

	mov [%1  + 8*0 ], rax
	mov [%1 +8*1], rbx
	mov [%1 +8*2], rcx
	mov [%1 +8*3], rdx
	mov [%1 +8*4], rsi
	mov [%1 +8*5], rdi
	mov [%1 +8*6], rbp
	mov [%1 +8*8], r8
	mov [%1 +8*9], r9
	mov [%1 +8*10], r10
	mov [%1 +8*11], r11
	mov [%1 +8*12], r12
	mov [%1 +8*13], r13
	mov [%1 +8*14], r14
	mov [%1 +8*15], r15

	mov rax, rsp 
	add rax, 160
	mov [%1 +56], rax  ; se guarda rsp
	mov rax, [rsp] 
	mov [%1 +128], rax   ; se guarda rip
	mov rax, [rsp+8] 
	mov [%1 +136], rax   ; se guarda las rflags

	mov rdi, %1 
	call saveRegisters

%endmacro

;luego de que hagamos la excepcion, y llamemos a la macro para guardar el estado de los registros hacer:
; ...
; mov byte[regChecked], 1   
; mov rdi, [regChecked]
; mov rsi, regArray
; call saveRegisters
; ...
; no estoy segura si es asi pero seria la idea 

section .bss 
	aux resq 1
	regArray	resq	18	
	regChecked resb 1  ; lugar para guardar si ya se se hizo un backup de los registros o no, deberiamos ponerlo 
						; en 1 cuando se realizan las excepciones y se llama a la macro

	