global _start

_start:
    ; Print 'A'
    mov al, 'A'
    mov [rsp-1], al
    mov rax, 1
    mov rdi, 1
    lea rsi, [rsp-1]
    mov rdx, 1
    syscall

    ; Print 'B'
    mov al, 'B'
    mov [rsp-1], al
    mov rax, 1
    mov rdi, 1
    lea rsi, [rsp-1]
    mov rdx, 1
    syscall

    ; Print 'C'
    mov al, 'C'
    mov [rsp-1], al
    mov rax, 1
    mov rdi, 1
    lea rsi, [rsp-1]
    mov rdx, 1
    syscall

    ; Exit
    mov rax, 60
    xor rdi, rdi
    syscall
