org 0x7c00
[bits 16]
xor ax,ax 
;初始化段寄存器, 段地址全部设为0
    mov ds,ax 
    mov ss,ax
    mov es,ax
    mov fs,ax
    mov gs,ax

;初始化栈指针
    mov sp,0x7c00	
    mov ax,0xb800	
    mov gs,ax

;功能号6H实现清屏操作
    mov ah,0x06 
    mov al,0    
    mov bh,0x07 
    mov ch,0    ;左上角行号
    mov cl,0    ;左上角列号
    mov dh,24   ;右下角行号
    mov dl,79   ;右下角列号
    int 0x10

;初始化方向和显示的数字及其颜色
    dirc_row db 1
    dirc_col db -1
    mov al,'0'
    mov bl,0x00

;初始化光标位置(2,0)
    mov ah,0x02
    mov bh,0
    mov dh,2
    mov dl,0
    int 0x10

loop:
;将寄存器存入栈中
    pushad
;计算与当前中心对称的位置坐标并输出字符
    sub dl,79
    neg dl      ;dl=79-dl
    mov ah,0x02
    mov bh,0
    int 0x10    ;dh=24-dh
    mov ah,0x09
    mov cx,1
    int 0x10
;出栈恢复之前的寄存器中的值
    popad
;2H移动光标到新位置,9H显示字符
    mov ah,0x02     
    mov bh,0
    int 0x10

    mov ah,0x09
    mov cx,1
    int 0x10

;获取当前光标的信息包括位置、颜色,用于后面更新新的颜色和位置
    mov ah,0x03
    mov bh,0
    int 0x10

;双重循环使CPU空转以达到延迟效果
    mov cx, 0x300    ;外层循环次数
out_delay:
    mov si, 0xFFFF   ;内层循环次数
in_delay:
    dec si
    jnz in_delay
    dec cx
    jnz out_delay


;调整方向和字符以及颜色
adjust_char:
    cmp al,'9'
    je reset_char

adjust_color:
    cmp bl,0xFF
    je reset_color

adjust_row:
    cmp dh,24
    je bounce_row
    cmp dh,0
    je bounce_row

adjust_col:
    cmp dl,79
    je bounce_col
    cmp dl,0
    je bounce_col

    jmp continue

reset_char:
    mov al,'0'
    jmp adjust_color

reset_color:
    mov bl,0x00
    jmp adjust_row

bounce_row:
    neg byte [dirc_row]
    jmp adjust_col

bounce_col:
    neg byte [dirc_col]
    jmp continue

;更新新的位置和颜色
continue:
    add dh,[dirc_row]
    add dl,[dirc_col]
    add bl,00000001b
    add al,1
    jmp loop

jmp $ ; 死循环

times 510 - ($ - $$) db 0
db 0x55, 0xaa