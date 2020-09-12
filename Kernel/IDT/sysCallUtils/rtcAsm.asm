GLOBAL getTime

section .text

getTime:

    push rbp
    mov rbp,rsp

    mov rax, rdi


    out 70h,al
    in al,71h
    
    mov rsp,rbp
    pop rbp
   
    ret