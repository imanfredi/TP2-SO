GLOBAL getMem
GLOBAL _time
GLOBAL _temperature
GLOBAL _information
GLOBAL _model
GLOBAL _invalidOpcodeException
GLOBAL _inforeg
GLOBAL _ps
GLOBAL _nice
GLOBAL _block
GLOBAL _kill
section .text



;Recibe en rdi el buffer donde guardar lo leido del puntero recibido en rsi. Guarda la cantidad de bytes recibidos en rdx

getMem:
    push rbp
    mov rbp,rsp
    push rax
    mov rax,4
    
    int 80h
    pop rax
    mov rsp,rbp
    pop rbp
    ret
;Recibe en rdi el puntero a un buffer de size 9 para guardar la hora
_time:
    push rbp
    mov rbp,rsp

    push rax
    mov rax,5
    int 80h
    pop rax

    mov rsp,rbp
    pop rbp
    ret

;Recibe en rdi el string donde escribir 
_information:
    push rbp
    mov rbp,rsp

    push rax
    mov rax,6
    int 80h
    pop rax

    mov rsp,rbp
    pop rbp
    ret

_model:

    push rbp
    mov rbp,rsp

    mov rax,8
    int 80h

    mov rsp,rbp
    pop rbp
    ret
_temperature:

    push rbp
    mov rbp,rsp
    mov rax,7
    int 80h
    mov rsp,rbp
    pop rbp
    ret

_invalidOpcodeException:

    push rbp
    mov rbp,rsp
    ud2
    mov rsp,rbp
    pop rbp
    ret

;Funcion que retorna un vector con los valores de los registros
_inforeg:
    push rbp
    mov rbp,rsp
    mov rax,9
    int 80h
    mov rsp,rbp
    pop rbp
    ret

_ps:
    push rbp
    mov rbp,rsp
    mov rax, 13
    int 80h
    mov rsp,rbp
    pop rbp
    ret

_block:

    push rbp
    mov rbp,rsp
    mov rax,14
    int 80h
    mov rsp,rbp
    pop rbp
    ret


_nice:

    push rbp
    mov rbp,rsp
    mov rax,15
    int 80h
    mov rsp,rbp
    pop rbp
    ret


_kill:

    push rbp 
    mov rbp,rsp
    mov rax,16
    int 80h
    mov rsp,rbp
    pop rbp
    ret
