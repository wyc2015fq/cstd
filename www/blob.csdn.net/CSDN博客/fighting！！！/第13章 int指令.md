# 第13章 int指令 - fighting！！！ - CSDN博客
2015年05月22日 19:43:32[dujiahei](https://me.csdn.net/dujiahei)阅读数：282
1.
assume cs:code
code segment
start: mov ax,cs
 mov ds,ax
 mov si,offset show
 mov ax,0
 mov es,ax
 mov di,200h
 mov cx,offset showe-offset show
 cld
 rep movsb
 mov ax,0
 mov es,ax
 mov word ptr es:[7ch*4],200h
 mov word ptr es:[7ch*4+2],0
 mov ax,4c00h
 int 21h
show: push ax
 push dx
 mov ah,9
 mov dx,si
 int 21h
 pop dx
 pop ax
 iret
showe:nop
code ends
end start
1-a
assume cs:code
data segment
 db "welcome to masm! ",0
data ends
code segment
start: mov dh,10
 mov dl,10
 mov cl,2
 mov ax,data
 mov ds,ax
 mov si,0
 int 7ch
 mov ax,4c00h
 int 21h
code ends
end start
1-b
assume cs:code
code segment
start: mov ax,cs
 mov ds,ax
 mov si,offset showstr
 mov ax,0
 mov es,ax
 mov di,200h
 mov cx,offset showe-offset showstr
 cld
 rep movsb
 mov ax,0
 mov es,ax
 mov word ptr es:[7ch*4],200h
 mov word ptr es:[7ch*4+2],0
 mov ax,4c00h
 int 21h
showstr:push bx
        push cx
 push dx 
 push di 
 mov ax,0b800h
 mov es,ax
        mov al,dl
        dec al
        mov ah,0
        add ax,ax
        mov di,ax
 mov dl,dh  
 mov al,160
 mul dl
 add di,ax  
 mov bh,cl
show: mov cl,ds:[si]
 mov ch,0
        jcxz ok
 mov bl,ds:[si]
  mov es:[di],bx
        inc si
        add di,2
 jmp show
ok: pop di
 pop dx
 pop cx
 pop bx 
 mov ax,4c00h
 int 21h
showe:nop
code ends
end start
2.
assume cs:code
code segment
start: mov ax,cs
 mov ds,ax
 mov si,offset lp
 mov ax,0
 mov es,ax
 mov di,200h
 mov cx,offset lpe-offset lp
 cld
 rep movsb
 mov ax,0
 mov es,ax
 mov word ptr es:[7ch*4],200h
 mov word ptr es:[7ch*4+2],0
 mov ax,4c00h
 int 21h
lp: push bp
 mov bp,sp
 dec bx
 jcxz lpret
 add [bp+2],bx
lpret: pop bp
 iret
lpe: nop
code ends
end start
2-a
assume cs:code
code segment
start: mov ax,0b800h
 mov es,ax
 mov di,160*12
 mov bx,offset s-offset se
 mov cx,80
s: mov byte ptr es:[di],'!'
 add di,2
 int 7ch
 ;loop s
se:  nop
 mov ax,4c00h
 int 21h
code ends
end start
3.
assume cs:code
code segment
s1: db 'Good,better,best,','$'
s2: db 'Never let it rest,','$'
s3: db 'Till good is better,','$'
s4: db 'And better,best.','$'
s: dw offset s1,offset s2,offset s3,offset s4
row: db 2,4,6,8
start: mov ax,cs
 mov ds,ax
 mov bx,offset s
 mov si,offset row
 mov cx,4
ok: mov bh,0
 mov dh,ds:[si]
 mov dl,0
 mov ah,2
 int 10h
 mov dx,ds:[bx]
 mov ah,9
 int 21h
 inc si
 add bx,2
 loop ok
 mov ax,4c00h
 int 21h
code ends
end start
