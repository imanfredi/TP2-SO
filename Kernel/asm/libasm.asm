GLOBAL cpuVendor
GLOBAL _temperature
GLOBAL cpuModel
GLOBAL getInitialSP
section .text


cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx
	push rcx
	push rdx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+12], 0

	mov rax, rdi

	pop rdx
	pop rcx
	pop rbx

	mov rsp, rbp
	pop rbp

	ret

cpuModel:


	push rbp
	mov rbp,rsp
	
	push rbx
	push rcx
	push rdx
	mov rax,1
	cpuid
	shr rax,4
	and eax,0xFF
	
	pop rbx
	pop rcx
	pop rdx

	mov rsp,rbp
	pop rbp
	ret
	
;Esta implementacion para conocer la temperatura del procesador es en particular para los procesadores de intel
_temperature:

    push rbp
    mov rbp,rsp

    push rcx
    push rdx
  
    mov ecx,0x19C ; valor que corresponde a IA-32-THERM-STATUS
    rdmsr ; en eax tengo los primero 32 bits que necesito
	mov edx,eax 
    AND edx, 0x007F0000 ; en los bit (16-22) tengo la diferencia entre la temperatura actual y el target.
    
	mov ecx,0X1A2 ;valor que corresponde a MSR_TEMP_TARGET
    rdmsr ; en eax tengo los primero 32 bits que necesito
    AND eax,0x00FF0000; en los bit (16-23) tengo el target 
	
	sub eax,edx; Target-tempActual = diferencia ---> tempActual=target - diferencia
	shr eax,16 ;realizo shifteo para acomodar el numero
    pop rdx
    pop rcx

    leave
    ret

getInitialSP:
	mov rax,rsp
	add rax,8
	ret
