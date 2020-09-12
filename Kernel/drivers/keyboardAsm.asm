GLOBAL hasKey
GLOBAL getKey

section .text

hasKey:

        push rbp
        mov rbp,rsp
        mov rax,0

        IN al,64h
        and al,0x1 ;Hago el and y si da 1 es que habia data en el puerto 64h
        
        mov rsp,rbp
        pop rbp

        ret

getKey:

        push rbp
        mov rbp,rsp

        mov rax,0
        IN al,60h

        mov rsp,rbp
        pop rbp

        ret