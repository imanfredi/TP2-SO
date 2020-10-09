GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _systemCallHandler
GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN systemCallDispatcher
EXTERN saveRegisters
EXTERN printRegisters
EXTERN schedule

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


%macro pushStateNoRax 0
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

%macro popStateNoRax 0
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
%endmacro


%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi,rsp
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro


%macro exceptionHandler 1
	pushState
	mov rdi,rsp
	call saveRegisters
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
	pushState
	
	mov rdi, 0 ; pasaje de parametro
	call irqDispatcher

	mov rdi,rsp
	call schedule
	mov rsp,rax
	
	;Send EOI
	mov al,20h
	out 20h,al

	popState
	iretq


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


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcde Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

_systemCallHandler:
		
        push rbp
        mov rbp,rsp

		pushStateNoRax
		mov [register],rax
		mov [register+8],rdi
		mov [register+16],rsi
		mov [register+24],rdx
		mov [register+32],rcx
		mov [register+40],r8
		mov [register+48],r9

		mov rdi,register
		
		call systemCallDispatcher
		
	
		popStateNoRax

		mov rsp,rbp
		pop rbp

		iretq


SECTION .bss
	aux resq 1

	register:
			rrax resq 1 ;Reservo 64 bits
			rrdi resq 1
			rrsi resq 1
			rrdx resq 1
			rrcx resq 1
			rr8 resq 1
			rr9 resq 1
SECTION .data