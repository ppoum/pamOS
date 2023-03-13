global CPUID_get_vendor_asm
CPUID_get_vendor_asm:
    ; 
    ; Params
    ; rdi: Pointer to buffer
    ;
    
    mov eax, 0x0
    cpuid
    ; EBX
    mov byte [rdi], bl
    shr ebx, 8
    lea rdi, [rdi+1]
    mov byte [rdi], bl
    shr ebx, 8
    lea rdi, [rdi+1]
    mov byte [rdi], bl
    shr ebx, 8
    lea rdi, [rdi+1]
    mov byte [rdi], bl
    lea rdi, [rdi+1]
    
    ; EDX
    mov byte [rdi], dl
    shr edx, 8
    lea rdi, [rdi+1]
    mov byte [rdi], dl
    shr edx, 8
    lea rdi, [rdi+1]
    mov byte [rdi], dl
    shr edx, 8
    lea rdi, [rdi+1]
    mov byte [rdi], dl
    lea rdi, [rdi+1]

    ; ECX
    mov byte [rdi], cl
    shr ecx, 8
    lea rdi, [rdi+1]
    mov byte [rdi], cl
    shr ecx, 8
    lea rdi, [rdi+1]
    mov byte [rdi], cl
    shr ecx, 8
    lea rdi, [rdi+1]
    mov byte [rdi], cl
    ret
