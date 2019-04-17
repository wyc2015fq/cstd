# Linux启动代码boot.s理解 - 小灰笔记 - CSDN博客





2017年08月20日 16:48:28[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：638








       为了理解Linux的启动代码，我自己也照着书中抄了一遍。具体的代码如下：

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



       原来书中的注释全都是用的”!”，而我抄写代码的时候全都改成了分号，这跟我大学时候学的计算机硬件课程上的内容更相似一些。具体的代码理解分条目列述如下：

1，.globl begtext,begdata,begbss,endtext,enddata,endbss ;global flag

       这一句定义了一些全局的标识符。

2，

.text

begtext:

.data

begdata:

.bss

begbss:

.text

       这部分代码本来适用于程序分段，但是这份代码所有的代码都在分段之后堆叠，其实也就是没分段。注意分段后面需要加冒号！

3，

BOOTSEG = 0x07c0



entry start

start:

       jmpi go,BOOTSEG

go:  mov ax,cs

       BOOTSEG相当于是定义了一个常量，在内存区中。而entry start是告诉链接器生成程序之后，系统加载的入口在start。而jmpi是段间跳转，后面指定的是调转地址，而前面的操作数是偏移量。这样，程序就实现了从0x07c0地址跳转到go。而cs也因此而具有了0x07c0的基地址。

4，

go:  mov ax,cs

       mov ds,ax

       mov es,ax

       这一段其实我是有一点点疑问的，为什么不直接用cs对另外两个寄存器初始化，这样就可以少一条语句。

5，

mov [msg1+17],ah ;need to understand

       抄代码的时候，我注释了一下这个需要理解一下。其实很好理解了，ax存储的是0x07c0，ah是他的高字节。这样其实就是把0x07的数字存储到msg1的17偏移量的位置。从下面的这条语句信息可以知道，这个位置正好是一个点号。

msg1:     .ascii "Loading system ..."

6，

       mov cx,#20

       mov dx,#0x1004

       mov bx,#0x000c

       mov bp,#msg1

       mov ax,#1301

int 0x10

这几条语句是实现屏幕显示，#20指示了显示的内容长度，msg1则指示了显示内容的开始位置。Int 10是屏幕显示中断。

7，

.org 510

       .word 0xAA55

       这个伪操作是从地址0x510开始填充数据，填充一个word。这样程序的最终大小就是512B。

8，

       剩下的信息都是数据段的结束处理标识符，也就没有太多说明的了。



