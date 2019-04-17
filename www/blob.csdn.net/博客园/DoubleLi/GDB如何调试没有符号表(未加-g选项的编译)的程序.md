# GDB如何调试没有符号表(未加-g选项的编译)的程序 - DoubleLi - 博客园






/*********************************************************************
 * Author  : Samson
 * Date    : 01/30/2015
 * Test platform:
 *              3.13.0-24-generic
 *              GNU bash, 4.3.11(1)-release 
 * *******************************************************************/
很多时候，发行版的程序在编译的时候都是没有加上-g这个选项的，那么若是想调试一个程序，应该怎么办呢？

在加了-g选项时，是可以通过行号、函数名等进行断点的设置的，但是没有符号表的情况下，那么怎么来进行程序的断点的设置并进行调试呢？

这就要用到反汇编然后再对地址进行断点的设置来进行调试，具体情况可参看以下例子的过程：

test.c代码如下：

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int m = 0, n =9;
    int k = m+n;
    printf("k is %d\n", k);
    m = k + n;
    printf("m is %d\n", m);
    n = m-n;
    printf("n is %d\n", n);
    return 0;
}
使用不带-g参数的编译命令行进行编译：

linuxidc@linuxidc:~$ gcc test.c
使用gdb进行程序的调试：

linuxidc@linuxidc:~$ gdb a.out 
GNU gdb (GDB) 7.5.91.20130417-cvs-[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2)
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>...
Reading symbols from a.out...(no debugging symbols found)...done.
(gdb) l
没有符号表被读取。请使用 "file" 命令。
(gdb) disassemble main 
Dump of assembler code for function main:
  0x0804841c <+0>:    push  %ebp
  0x0804841d <+1>:    mov    %esp,%ebp
  0x0804841f <+3>:    and    $0xfffffff0,%esp
  0x08048422 <+6>:    sub    $0x20,%esp
  0x08048425 <+9>:    movl  $0x0,0x14(%esp)
  0x0804842d <+17>:    movl  $0x9,0x18(%esp)
  0x08048435 <+25>:    mov    0x18(%esp),%eax
  0x08048439 <+29>:    mov    0x14(%esp),%edx
  0x0804843d <+33>:    add    %edx,%eax
  0x0804843f <+35>:    mov    %eax,0x1c(%esp)
  0x08048443 <+39>:    mov    0x1c(%esp),%eax
  0x08048447 <+43>:    mov    %eax,0x4(%esp)
  0x0804844b <+47>:    movl  $0x8048540,(%esp)
  0x08048452 <+54>:    call  0x80482f0 <printf@plt>
  0x08048457 <+59>:    mov    0x18(%esp),%eax
  0x0804845b <+63>:    mov    0x1c(%esp),%edx
  0x0804845f <+67>:    add    %edx,%eax
  0x08048461 <+69>:    mov    %eax,0x14(%esp)
  0x08048465 <+73>:    mov    0x14(%esp),%eax
  0x08048469 <+77>:    mov    %eax,0x4(%esp)
  0x0804846d <+81>:    movl  $0x8048549,(%esp)
  0x08048474 <+88>:    call  0x80482f0 <printf@plt>
  0x08048479 <+93>:    mov    0x18(%esp),%eax
  0x0804847d <+97>:    mov    0x14(%esp),%edx
  0x08048481 <+101>:    mov    %edx,%ecx
  0x08048483 <+103>:    sub    %eax,%ecx
  0x08048485 <+105>:    mov    %ecx,%eax
  0x08048487 <+107>:    mov    %eax,0x18(%esp)
  0x0804848b <+111>:    mov    0x18(%esp),%eax
  0x0804848f <+115>:    mov    %eax,0x4(%esp)
  0x08048493 <+119>:    movl  $0x8048552,(%esp)
  0x0804849a <+126>:    call  0x80482f0 <printf@plt>
  0x0804849f <+131>:    mov    $0x0,%eax
  0x080484a4 <+136>:    leave  
  0x080484a5 <+137>:    ret    
End of assembler dump.
(gdb) b *0x08048452
Breakpoint 1 at 0x8048452
(gdb) b *0x08048474
Breakpoint 2 at 0x8048474
(gdb) b *0x0804849a
Breakpoint 3 at 0x804849a
(gdb) info b
Num    Type          Disp Enb Address    What
1      breakpoint    keep y  0x08048452 <main+54>
2      breakpoint    keep y  0x08048474 <main+88>
3      breakpoint    keep y  0x0804849a <main+126>
(gdb) r
Starting program: a.out

Breakpoint 1, 0x08048452 in main ()
(gdb) c
Continuing.
k is 9

Breakpoint 2, 0x08048474 in main ()
(gdb) c
Continuing.
m is 18

Breakpoint 3, 0x0804849a in main ()
(gdb) c
Continuing.
n is 9
[Inferior 1 (process 19933) exited normally]

由以上的步骤可以看出，使用了disassemble main 进行主函数的反汇编，然后使用了b *address进行三处printf的地址的断点的设置。

GDB调试程序用法 [http://www.linuxidc.com/Linux/2013-06/86044.htm](http://www.linuxidc.com/Linux/2013-06/86044.htm)

GDB+GDBserver无源码调试[Android](http://www.linuxidc.com/topicnews.aspx?tid=11) 动态链接库的技巧 [http://www.linuxidc.com/Linux/2013-06/85936.htm](http://www.linuxidc.com/Linux/2013-06/85936.htm)

使用hello-gl2建立ndk-GDB环境(有源码和无源码调试环境) [http://www.linuxidc.com/Linux/2013-06/85935.htm](http://www.linuxidc.com/Linux/2013-06/85935.htm)

在Ubuntu上用GDB调试printf源码 [http://www.linuxidc.com/Linux/2013-03/80346.htm](http://www.linuxidc.com/Linux/2013-03/80346.htm)

Linux下用GDB调试可加载模块 [http://www.linuxidc.com/Linux/2013-01/77969.htm](http://www.linuxidc.com/Linux/2013-01/77969.htm)

Ubuntu下使用GDB断点Go程序 [http://www.linuxidc.com/Linux/2012-06/62941.htm](http://www.linuxidc.com/Linux/2012-06/62941.htm)

使用GDB命令行调试器调试C/C++程序 [http://www.linuxidc.com/Linux/2014-11/109845.htm](http://www.linuxidc.com/Linux/2014-11/109845.htm)

**GDB 的详细介绍**：[请点这里](http://www.linuxidc.com/Linux/2014-02/96251.htm)
****GDB **的下载地址**：[请点这里](http://www.linuxidc.com/down.aspx?id=1232)

**本文永久更新链接地址**：[http://www.linuxidc.com/Linux/2015-02/113324.htm](http://www.linuxidc.com/Linux/2015-02/113324.htm)









