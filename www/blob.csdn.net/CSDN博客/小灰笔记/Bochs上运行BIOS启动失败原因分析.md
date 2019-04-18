# Bochs上运行BIOS启动失败原因分析 - 小灰笔记 - CSDN博客





2017年08月24日 00:16:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：779








       之前写了一段BIOS启动代码，在Bochs中测试启动的时候失败。其实当时的代码出了很多问题，只是文件能够编译通过而已。错误的代码如下：

;

; boot.s--bootsect.S framework

;

.globlbegtext,begdata,begbss,endtext,enddata,endbss ;global flag

.text

begtext:

.data

begdata:

.bss

begbss:

.text

BOOTSEG = 0x07c0



entry start

start:

       jmpi go,BOOTSEG

go:  mov ax,cs

       mov ds,ax

       mov es,ax

       mov [msg1+17],ah ; need to understand

       mov cx,#20

       mov dx,#0x1004

       mov bx,#0x000c

       mov bp,#msg1

       mov ax,1301

       int 0x10

loop0:     jmp loop0

msg1:     .ascii "Loading system ..."

       .byte       31,10;need to understand

.org 510

       .word 0xAA55

.text

endtext:

.data

enddata:

.bss

endbss:

       其实，这里面的错误有好几处。其中一处是回车换行的代号错了，但是这个还不是主要的问题。最主要的问题还是字符串的显示位置，如果使用十六进制的话现实的效果刚刚好。但是如果使用十进制的话，那么位置就会回退好多，直接超出显示区域。这个才是导致显示失败的原因。

       仅仅修改显示位置，代码修改如下：

;

; boot.s--bootsect.S framework

;

.globlbegtext,begdata,begbss,endtext,enddata,endbss ;global flag

.text

begtext:

.data

begdata:

.bss

begbss:

.text

BOOTSEG = 0x07c0



entry start

start:

       jmpi go,BOOTSEG

go:  mov ax,cs

       mov ds,ax

       mov es,ax

       mov [msg1+17],ah ; need to understand

       mov cx,#20

       mov dx,#0x1004

       mov bx,#0x000c

       mov bp,#msg1

       mov ax,#0x1301

       int 0x10

loop0:     jmp loop0

msg1:     .ascii "Loading system ..."

       .byte       31,10;need to understand

.org 510

       .word 0xAA55

.text

endtext:

.data

enddata:

.bss

endbss:

       编译执行之后的执行效果：

![](https://img-blog.csdn.net/20170824001458135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






