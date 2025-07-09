org 0x7c00
[bits 16]

mov ah,0x02
mov bh,0
mov dh,0
mov dl,0
int 0x10

mov al,'2'
mov ah,0x09
mov bl,10001111b
mov cx,1
int 0x10

mov ah,0x02
mov bh,0
mov dh,0
mov dl,1
int 0x10

mov al,'3'
mov ah,0x09
mov bl,10001111b
mov cx,1
int 0x10

mov ah,0x02
mov bh,0
mov dh,0
mov dl,2
int 0x10

mov al,'3'
mov ah,0x09
mov bl,10001111b
mov cx,1
int 0x10

mov ah,0x02
mov bh,0
mov dh,0
mov dl,3
int 0x10

mov al,'2'
mov ah,0x09
mov bl,10001111b
mov cx,1
int 0x10

mov ah,0x02
mov bh,0
mov dh,0
mov dl,4
int 0x10

mov al,'0'
mov ah,0x09
mov bl,10001111b
mov cx,1
int 0x10

mov ah,0x02
mov bh,0
mov dh,0
mov dl,5
int 0x10

mov al,'1'
mov ah,0x09
mov bl,10001111b
mov cx,1
int 0x10

mov ah,0x02
mov bh,0
mov dh,0
mov dl,6
int 0x10

mov al,'0'
mov ah,0x09
mov bl,10001111b
mov cx,1
int 0x10

mov ah,0x02
mov bh,0
mov dh,0
mov dl,7
int 0x10

mov al,'4'
mov ah,0x09
mov bl,10001111b
mov cx,1
int 0x10

jmp $ ; 死循环

times 510 - ($ - $$) db 0
db 0x55, 0xaa