[bits 32]

global asm_hello_world
asm_hello_world:
    push eax
    xor eax, eax
    mov ecx, name_and_student_number_end - name_and_student_number
    mov ebx, 0
    mov esi, name_and_student_number
    mov ah, 0x03 ;青色
loop_table:
    mov al, [esi]
    mov word[gs:ebx], ax
    add ebx, 2
    inc esi
    loop loop_table
    pop eax
    ret
name_and_student_number db '23320104 ltx' ;学号和名字 23320104ltx
name_and_student_number_end: