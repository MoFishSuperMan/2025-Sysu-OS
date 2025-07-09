org 0x7c00
[bits 16]

mov ah,0x02
mov bh,0
mov dh,8
mov dl,8
int 0x10

mov ah,0x03
mov bh,0
int 0x10

mov al,dh
add al,'0'
mov ah,0x0e
mov bl,10001111b
mov cx,1
int 0x10

mov al,dl
add al,'0'
mov ah,0x0e
mov bl,10001111b
mov cx,1
int 0x10

jmp $ ; 死循环

times 510 - ($ - $$) db 0
db 0x55, 0xaa