%include "head.include"
; you code here

your_if:
    mov eax,[a1]
    cmp eax,12
    jge else_if
    mov edx,0
    mov eax,[a1]
    mov ebx,2
    idiv ebx
    mov dword [if_flag],eax
    add dword [if_flag],1
    jmp end_if
else_if:
    cmp eax,24
    jge else
    mov eax,24
    sub eax,[a1]
    imul eax,[a1]
    mov [if_flag],eax
    jmp end_if
else:
    mov eax,[a1]
    shl eax,4
    mov [if_flag],eax

end_if:

your_while:
    mov ecx,[while_flag]
while_loop:
    mov eax,[a2]
    cmp eax,12
    jl end_while
    push ecx
    call my_random
    pop ecx
    mov ebx,[a2]
    sub ebx,12

    mov byte [ecx+ebx],al
    dec dword [a2]
    jmp while_loop
end_while:


%include "end.include"

your_function:
    xor esi,esi
function_loop:
    mov al,[your_string+esi]
    cmp al,0
    je function_end
    pushad
    push ax
    call print_a_char
    pop ax
    popad
    inc esi
    jmp function_loop
function_end:
    ret