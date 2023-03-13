; Get externs from 'isr.c'
[extern intelInterruptHandler]
[extern genericInterruptHandler]
; TODO rename externs

; Macros to push/pop every registers
; SYS V ABI scratch regs : rbx, rsp, rbp, r12-15
; (rsp doesn't really need to get pushed because we need its value to
;  get back the saved vals, but saved anyway)
%macro pushaq 0
    sub rsp, 56     ; 7*8 = 56
    mov qword [rsp], rbx
    mov qword [rsp+8], rsp
    mov qword [rsp+16], rbp
    mov qword [rsp+24], r12
    mov qword [rsp+32], r13
    mov qword [rsp+40], r14
    mov qword [rsp+48], r15
%endmacro
%macro popaq 0
    mov rbx, qword [rsp]
    mov rsp, qword [rsp+8]
    mov rbp, qword [rsp+16]
    mov r12, qword [rsp+24]
    mov r13, qword [rsp+32]
    mov r14, qword [rsp+40]
    mov r15, qword [rsp+48]
    add rsp, 56     ; 7*8 = 56
%endmacro

intelISRWrapper:
    push rdi
    movzx rdi, byte [rsp+8]     ; copy interrupt # to rdi
    pushaq
    cld                         ; strings are big endian (from left to right)
    call intelInterruptHandler
    popaq                       ; restore registers
    pop rdi
    lea rsp, [rsp+8]            ; pop interrupt # push in isrN
    iretq                       ; return from ISR

irqWrapper:
    push rdi                    ; similar logic to above, calling different extern func
    movzx rdi, byte [rsp+8]
    pushaq
    cld
    call genericInterruptHandler
    popaq
    pop rdi
    lea rsp, [rsp+8]
    iretq

; Individual handlers
; All system interrupts go back to an intel ISR wrapper function
%assign i 0
%rep 22
global isr%+i
isr%+i:
    push i
    jmp intelISRWrapper
%assign i i+1
%endrep

%assign j 0
%rep 2
global irq%+j
irq%+j:
    push j
    jmp irqWrapper
%assign j j+1
%endrep
