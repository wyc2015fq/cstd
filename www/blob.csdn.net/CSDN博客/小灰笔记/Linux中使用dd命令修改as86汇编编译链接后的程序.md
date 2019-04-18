# Linux中使用dd命令修改as86汇编编译链接后的程序 - 小灰笔记 - CSDN博客





2017年08月20日 19:21:06[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：224








       之前抄写了一段代码：

;

; boot.s --bootsect.S framework

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

       mov ax,#1301

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

       由于.org的定制，这个程序设计的时候生成的程序大小应该是512B。实际编译之后的结果查看信息如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code$ls -l

total 1

-rwxrwxrwx 1 rootroot 544 Aug 20 18:55 boot

-rwxrwxrwx 1 rootroot 232 Aug 20 18:41 boot.o

-rwxrwxrwx 1 rootroot 554 Aug 20 16:05 boot.s

       从上面的结果，程序的实际大小是544B。原因是程序编译器是用于MINIX的，而多出的一部分信息是因为程序的头部多了MINIX系统执行头结构。修改的方式可以是用二级制编译的方式修改，当然还能够用Linux的dd修改。

       修改过程以及结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code$dd bs=32 if=boot of=./new_boot skip=1

16+0 records in

16+0 records out

512 bytes (512 B)copied, 0.004947 s, 103 kB/s

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code$ls -l

total 2

-rwxrwxrwx 1 rootroot 544 Aug 20 18:55 boot

-rwxrwxrwx 1 rootroot 232 Aug 20 18:41 boot.o

-rwxrwxrwx 1 rootroot 554 Aug 20 16:05 boot.s

-rwxrwxrwx 1 rootroot 512 Aug 20 19:12 new_boot

       从上面的结果来看，修改已经完成，文件的大小已经回到了512B。



