# 第10章 CALL和RET 指令 - fighting！！！ - CSDN博客
2015年05月22日 19:39:19[dujiahei](https://me.csdn.net/dujiahei)阅读数：248
1.
assume cs:code
data segment
   db 'Welcome to masm!',0
data ends
code segment
start: mov dh,8
 mov dl,3
 mov cl,2
 mov ax,data
 mov ds,ax
 mov si,0
 call show_str
        mov ax,4c00h
        int 21h
show_str:push cx
 push dx 
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
show: pop cx
 mov bh,cl
 push cx
 mov cl,ds:[si]
 mov ch,0
        jcxz ok
 mov bl,ds:[si]
        mov es:[di],bx
        inc si
        add di,2
 jmp show
ok: pop dx
 pop cx
 ret
code ends
end start
2.
assume cs:code
data segment
 db 10 dup (0)  
data ends
code segment
start:  mov ax,data
 mov ds,ax
 mov ax,4240h
 mov dx,000fh
 mov cx,0ah
 call divdw
 mov ax,4c00h
 int 21h
divdw: push bx ;为了子程序的通用性而入栈
 push ax  ;为了子程序后面调用
 mov ax,dx
 mov dx,0
 div cx 
 mov bx,ax        
 pop ax 
 div cx
 mov cx,dx
 mov dx,bx 
        pop bx  
 ret
code ends
end start
3.
assume cs:code
data segment
 db 16 dup (0)
 dw 123,12666,1,8,3,38
data ends
code segment
start: mov bx,data
 mov ds,bx
 mov bx,10h 
s: mov ax,ds:[bx]
        mov cx,ax
 jcxz e
 mov si,0
 mov dx,0
 inc si
 call dtoc
 mov dh,8
 mov dl,3
 mov cl,2
 call show_str
 add bx,2
        jmp s
e: mov ax,4c00h
 int 21h
dtoc: push bx
s1: mov cx,ax
 jcxz ok1
 mov bx,10
 call divdw
 mov dx,ax
 jcxz ok1
 mov ds:[si],cx
 inc si
 jmp s1
ok1: pop bx
 ret
divdw: push bx ;为了子程序的通用性而入栈
 push ax  ;为了子程序后面调用
 mov ax,dx
 mov dx,0
 mov cx,bx
 div cx 
 mov bx,ax        
 pop ax
 mov dx,0 
 div cx
 mov cx,dx
 mov dx,bx 
        pop bx  
 ret
show_str:
        push bx
        push cx
 push dx 
 mov ax,0b800h
 mov es,ax
        mov al,dl
        dec al
        mov ah,0
        add ax,ax
        mov di,ax
        dec si
 mov dl,dh  
 mov al,160
 mul dl
 add di,ax  
show: pop cx
 mov bh,cl
 push cx
 mov cl,ds:[si]
 mov ch,0
        jcxz ok
 mov bl,ds:[si]
  add bl,30h
        mov es:[di],bx
        dec si
        add di,2
 jmp show
ok: pop dx
 pop cx
 pop bx 
        ret
code ends
end start
