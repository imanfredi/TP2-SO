GLOBAL getChar
GLOBAL printString
GLOBAL putChar
GLOBAL clear
GLOBAL printError
GLOBAL nextScreen
GLOBAL needScreen
GLOBAL initVisualEnvironment
GLOBAL addNewProcess
GLOBAL getSecondsElapsed
GLOBAL getPid
GLOBAL free2
GLOBAL malloc2
GLOBAL sem_open
GLOBAL sem_close
GLOBAL sem_wait
GLOBAL sem_post
GLOBAL semInfo
GLOBAL memInfo
GLOBAL changeValue
GLOBAL pipeOpen
GLOBAL closePipe
GLOBAL writeInPipe

extern strlen

getChar:

    push rbp
    mov rbp,rsp

    mov rax,0 ;para leer el buffer
    int 80h

    mov rsp,rbp
    pop rbp
    ret    

;Recibo en rdi el puntero al string
printString:

    push rbp
    mov rbp,rsp
    
    push rsi
    push rdx
    push rdi
    
    call strlen
    
    mov rdx,rax
    mov rax,1
    mov rsi,rdi
    mov rdi,1
    
    int 80h

    pop rdi
    pop rdx
    pop rsi
    
    mov rsp,rbp
    pop rbp
    ret

printError:
    push rbp
    mov rbp,rsp
    
    push rsi
    push rdx
    push rdi
    
    call strlen
    
    mov rdx,rax
    mov rax,1
    mov rsi,rdi
    mov rdi,2
    
    int 80h

    pop rdi
    pop rdx
    pop rsi
    
    mov rsp,rbp
    pop rbp
    ret

putChar:

    push rbp
    mov rbp,rsp

    push rdi
    push rsi
    push rdx
    
    mov [buffer],rdi ;muevo la letra que quiero escribir a un buffer
    mov rax,1 ;muevo a 1 porque es la syscall de write
    mov rdi,1 ;salida estandar
    mov rsi, buffer ;recibe en rsi, el buffer
    mov rdx, 1 ; quiero escribir solo un byte
    
    int 80h

    pop rdx
    pop rsi
    pop rdi
    
    mov rsp,rbp
    pop rbp
    
    ret

clear:
    
    push rbp
    mov rbp,rsp
    push rax    
    mov rax,2
    int 80h
    pop rax
    mov rsp,rbp
    pop rbp
    
    ret

;Recibo en rdi la pantalla a la que quiero acceder y envio en rdi la pantalla que quiero acceder

nextScreen:

    push rbp
    mov rbp,rsp
    push rax
    push rdi
    mov rax,3
    int 80h
    pop rdi
    pop rax
    mov rsp,rbp
    pop rbp
    
    ret

needScreen:
    push rbp
    mov rbp,rsp
    mov rax,10
    int 80h
    mov rsp,rbp
    pop rbp
    ret

initVisualEnvironment:

    push rbp
    mov rbp,rsp
    mov rax,11
    int 80h
    mov rsp,rbp
    pop rbp
    ret

addNewProcess:
    
    push rbp
    mov rbp,rsp
    mov rax,12
    int 80h
    mov rsp,rbp
    pop rbp
    ret

getSecondsElapsed:
    push rbp
    mov rbp,rsp
    mov rax,17
    int 80h
    mov rsp,rbp
    pop rbp
    ret


getPid:
    push rbp
    mov rbp,rsp
    mov rax,18
    int 80h
    mov rsp,rbp
    pop rbp
    ret

malloc2:
    push rbp
    mov rbp,rsp
    mov rax,20
    int 80h
    mov rsp,rbp
    pop rbp
    ret

free2:

    push rbp
    mov rbp,rsp
    mov rax,21
    int 80h
    mov rsp,rbp
    pop rbp
    ret


sem_open:

    push rbp
    mov rbp,rsp
    mov rax,22
    int 80h
    mov rsp,rbp
    pop rbp
    ret

sem_close:

    push rbp
    mov rbp,rsp
    mov rax,23
    int 80h
    mov rsp,rbp
    pop rbp
    ret

sem_post:

    push rbp
    mov rbp,rsp
    mov rax,24
    int 80h
    mov rsp,rbp
    pop rbp
    ret

sem_wait:

    push rbp
    mov rbp,rsp
    mov rax,25
    int 80h
    mov rsp,rbp
    pop rbp
    ret


semInfo:
    push rbp
    mov rbp,rsp
    mov rax,26
    int 80h
    mov rsp,rbp
    pop rbp
    ret


memInfo:
    push rbp
    mov rbp,rsp
    mov rax,27
    int 80h
    mov rsp,rbp
    pop rbp
    ret

changeValue:
    push rbp
    mov rbp,rsp
    mov rax,28
    int 80h
    mov rsp,rbp
    pop rbp
    ret


pipeOpen:
    push rbp
    mov rbp,rsp
    mov rax,29
    int 80h
    mov rsp,rbp
    pop rbp
    ret


closePipe:
    push rbp
    mov rbp,rsp
    mov rax,30
    int 80h
    mov rsp,rbp
    pop rbp
    ret

writeInPipe:
    push rbp
    mov rbp,rsp
    mov rax,31
    int 80h
    mov rsp,rbp
    pop rbp
    ret



section .bss

    buffer resb 2