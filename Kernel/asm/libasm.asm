GLOBAL cpuVendor

GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHours
GLOBAL getKeyPressed

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getSeconds:
	push rbp
	mov rbp, rsp

	mov al, 0    ; Registro para segundos
	out 70h, al
	in AL, 71h

	mov rsp, rbp
	pop rbp
	ret


getMinutes:
	push rbp
	mov rbp, rsp

	mov al, 2    ; Registro para minutos
	out 70h, al
	in AL, 71h

	mov rsp, rbp
	pop rbp
	ret


getHours:
	push rbp
	mov rbp, rsp

	mov al, 4    ; Registro para horas
	out 70h, al
	in AL, 71h

	mov rsp, rbp
	pop rbp
	ret

getKeyPressed:
    mov rax, 0  ; Inicializa el registro RAX a 0
.cicle: 
    in al, 64h  ; Lee el estado del teclado en el puerto 0x64
    and al, 0x01  ; Verifica si el bit 0 (indicador de tecla presionada) está activo
    je .cicle  ; Si no está activo, vuelve a verificar
    in al, 60h  ; Si está activo, lee el código de la tecla presionada del puerto 0x60

    ret  ; Retorna el código de la tecla presionada
