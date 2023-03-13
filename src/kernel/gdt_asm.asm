global reloadSegments
reloadSegments:
    push 0x08                   ; push code segment to stack
    lea rax, [rel .reloadCS]    ; load fun addr into rax
    push rax                    ; push to stack
    retfq                       ; far return using return address just pushed to stack (reloadCS)

.reloadCS:
    mov ax, 0x10                ; data segment (2nd index, shifted left 3 times)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
