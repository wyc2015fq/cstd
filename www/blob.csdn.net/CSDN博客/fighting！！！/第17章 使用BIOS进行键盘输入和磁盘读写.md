# 第17章 使用BIOS进行键盘输入和磁盘读写 - fighting！！！ - CSDN博客
2015年05月22日 19:49:53[dujiahei](https://me.csdn.net/dujiahei)阅读数：173

assume cs:code
data segment
 db 2879 dup (0)
data ends
code segment
;0:200h
rwstart:jmp short rw
table   dw sub0,sub1
table1  db 4 dup (0)
rw: push ax
 push bx
 push cx
 push dx
 cmp ah,1
 ja rwret
 mov bl,ah
 mov bh,0
 add bx,bx ;计算功能号
 push bx
 mov bx,1440 
 mov dx,ax
 mov dx,0
 div bx
        mov table1[0],al ;面号
 mov ax,dx
 mov dx,0
 mov bx,18
 div bx
        mov table1[1],al ;磁道号
 inc ah
        mov table1[2],ah ;扇区号
 mov al,1
        mov ch,table1[1] ;磁道号
        mov cl,table1[2] ;扇区号
        mov dh,table1[0]  ;面号
 mov dl,0  ;软驱号 A
 pop bx
 call word ptr table[bx]
rwret: pop dx 
 pop cx
 pop bx
 pop ax
 iret
sub0:   push ax
 mov ah,2
 int 13h
 pop ax 
 ret
sub1:    push ax 
 mov ah,3
 int 13h
 pop ax
 ret
rwe: nop
start: mov ax,cs
 mov ds,ax
 mov si,offset rwstart
 mov ax,0
 mov es,ax
 mov di,200h
 mov cx,offset rwe - offset rwstart
 cld
 rep movsb
 mov ax,0
 mov es,ax
 mov word ptr es:[7ch*4],0h ;偏移地址从0开始
 mov word ptr es:[7ch*4+2],20h ;使得程序被安装前后偏移地址一致
 mov ax,4c00h
 int 21h
code ends
end start
1-a
assume cs:code
data segment
dd 512 dup (0)
data ends
code segment
start: 
 mov ah,0
 mov dx,0
 mov bx,0b800h ;0c8f
 mov es,bx
 mov bx,40
 int 7ch
 mov ax,4c00h
 int 21h
code ends
end start
1-b
assume cs:code
data segment
dd 256 dup (0)
data ends
code segment
start: 
 mov ah,1
 mov dx,0
 mov bx,data
 mov es,bx
 mov bx,4
 int 7ch
 mov ax,4c00h
 int 21h
code ends
end start
