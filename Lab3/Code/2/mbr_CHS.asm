org 0x7c00
[bits 16]
xor ax, ax ; eax = 0
; 初始化段寄存器, 段地址全部设为0
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

; 初始化栈指针
mov sp, 0x7c00
mov ax, 1                ; 逻辑扇区号第0~15位
mov cx, 0
mov bx, 0x7e00           ; bootloader的加载地址
load_bootloader:
    call asm_read_hard_disk  ; 读取硬盘
jmp 0x0000:0x7e00        ; 跳转到bootloader

jmp $ ; 死循环

asm_read_hard_disk:
;mov ah, 02h ;功能号
;mov ch, 00h ; 柱面
;mov dh, 00h ; 磁头
;mov cl, 2 ; 扇区
;mov al, 05h ; 读取的扇区数
;int 0x13 ; BIOSint 13H中断
;扇区数63 磁道数18 一个柱面有63*18个扇区 
    pushad 
    lba dw 0    ;LBA
    c dw 0      ;柱面号c
    h dw 0      ;磁头号h
    s dw 0      ;磁区号s
    mov [lba], ax 

;计算柱面号 c= LBA // (18*63)
    mov ax,[lba]
    mov cx,1134 ;18*63=1134
    xor dx,dx   ;被除数dx:ax
    div cx      ;ax为商
    mov [c],al

;计算磁头号 h= (LBA // 63) % 18
    mov ax,[lba]
    mov cx,63
    xor dx,dx
    div cx      ;ax为LBA/63的商即LBA整除63的结果
    mov cx,18
    xor dx,dx
    div cx      ;除以18,dx为(LBA//63)mod 18的结果
    mov [h],dl

;计算扇区号 s= LBA % 63
    mov ax,[lba]
    mov cx,63
    xor dx,dx
    div cx      ;LBA mod 63 结果存放在dx即余数
    inc dx      ;扇区号从1开始所以要加1
    mov [s],dl
;调用BIOS
    mov ch,[c]  ;c
    mov dh,[h]  ;h
    mov cl,[s]  ;s
    mov al,5    ;读连续5块扇区 
    mov ah,02H  ;功能号02H
    mov dl,80H 
    int 13H 
    popad
    ret

times 510 - ($ - $$) db 0
db 0x55, 0xaa    