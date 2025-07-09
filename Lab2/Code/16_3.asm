org 0x7c00
[bits 16]
;功能号6H实现清屏操作
    mov ah, 0x06 
    mov al, 0    
    mov bh, 0x07 
    mov ch, 0    ;左上角行号
    mov cl, 0    ;左上角列号
    mov dh, 24   ;右下角行号
    mov dl, 79   ;右下角列号
    int 0x10

	mov ah,0x02
	mov bh,0
	mov dh,0
	mov dl,0
	int 0x10

loop:
	mov ah,0x0
	int 16H

	cmp al,0x0D
	je end

	mov ah,0x0e
	mov bl,10001111b
	mov cx,1
	int 0x10
	jmp loop
end:
	jmp $ ; 死循环

	times 510 - ($ - $$) db 0
	db 0x55, 0xaa