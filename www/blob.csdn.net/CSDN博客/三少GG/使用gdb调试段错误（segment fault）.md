# 使用gdb调试段错误（segment fault） - 三少GG - CSDN博客
2014年03月05日 18:52:01[三少GG](https://me.csdn.net/scut1135)阅读数：824
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)

**必看文章：**
1.  [http://www.unknownroad.com/rtfm/gdbtut/gdbsegfault.html](http://www.unknownroad.com/rtfm/gdbtut/gdbsegfault.html)
2. [http://www.cs.bgu.ac.il/~caspl102/wiki.files/lab2/gdb-intro.html](http://www.cs.bgu.ac.il/~caspl102/wiki.files/lab2/gdb-intro.html)
我们打算使用gdb去解决为什么下面的程序（文件为segfault.c）引起了段错误的问题。下面的这段程序是从用户那里读入一行文本字串然后显示在屏幕上。然而，如下当前的程序并不会如期执行...
**[cpp]**[view
 plain](http://blog.csdn.net/deutschester/article/details/6739861#)[copy](http://blog.csdn.net/deutschester/article/details/6739861#)[print](http://blog.csdn.net/deutschester/article/details/6739861#)[?](http://blog.csdn.net/deutschester/article/details/6739861#)
- <span style="font-size:18px;">#include <stdio.h>  
- #include <stdlib.h>
- 
- int main(int argc, char **argv)  
- {  
- char *buf;  
- 
-    buf = malloc(1<<31);  
- 
-    fgets(buf, 1024, stdin);  
-    printf("%s\n", buf);  
- 
- return 1;  
- }</span>  
 第一步是使用带有调试标志（debugging flags）的方式编译这段代码，如下：
~# gcc -g segfault.c
然后运行：
~# a.out
Hello World!
Segmentation fault
这并不是我们所期待的。是时候启动强大的gdb了。
~# gdb a.out
GNU gdb 5.0
Copyright 2000 Free Software Foundation, Inc.
GDB is free software, covered by the GNU General Public License, and you are
welcome to change it and/or distribute copies of it under certain conditions.
Type "show copying" to see the conditions.
There is absolutely no warranty for GDB.  Type "show warranty" for details.
This GDB was configured as "i686-pc-linux-gnu"...
(gdb) 
我们直接运行就来看看到底发生了什么：
(gdb) run
Starting program: /home/dgawd/cpsc/363/a.out 
test string
Program received signal SIGSEGV, Segmentation fault.
0x4007fc13 in _IO_getline_info () from /lib/libc.so.6
``
`我们收到了来自操作系统的SIGSEGV信号。这就意味着我们试图去访问一段非法的内存，让我们试试backtrace（= bt）命令：`
``
(gdb) backtrace
#0  0x4007fc13 in _IO_getline_info () from /lib/libc.so.6
#1  0x4007fb6c in _IO_getline () from /lib/libc.so.6
#2  0x4007ef51 in fgets () from /lib/libc.so.6
#3  0x80484b2 in main (argc=1, argv=0xbffffaf4) at segfault.c:10
#4  0x40037f5c in __libc_start_main () from /lib/libc.so.6
``
这里我们只关心我们自己的代码，因此我们就切换到3号堆栈帧（stack frame3）来看看程序在哪里崩溃的：
(gdb) frame 3
#3  0x80484b2 in main (argc=1, argv=0xbffffaf4) at segfault.c:10
10        fgets(buf, 1024, stdin)
``
哦，原来是调用fgets引起的崩溃。一般的，我们都假设库函数比如fgets都可以正确地工作（如果不是这样的话，我们的麻烦就大了）。因此这个问题的原因就一定是其中我们的一个参数的问题。你也许不知道‘stdin’是一个全局的变量，它是被stdio 库创建的。因此我们假定这个参数是正确的。那么剩下的就只能是‘buf’了，然后查看buf当前的值：
(gdb) print buf
$1 = 0x0
`buf的值是0x0，也就是NULL指针。这并不是我们锁期待的 —— buf应该指向第8行代码分配到的内存。因此我们需要返回到第8行并看看在哪里发生了什么。首先kill掉我们程序当前运行的调用：`
``(gdb) kill
Kill the program being debugged? (y or n) y
`（注意:不用使用quit直接退出gdb，这样比较麻烦。直接kill掉当前的程序调用即可）`
``
`然后在第8行设置一个断点：`
``(gdb) break segfault.c:8
Breakpoint 1 at 0x8048486: file segfault.c, line 8.
再次运行程序：
(gdb) run
Starting program: /home/dgawd/cpsc/363/a.out 
Breakpoint 1, main (argc=1, argv=0xbffffaf4) at segfault.c:8
8         buf = malloc(1<<31);
``
`我们检查malloc调用前后buf值的变化。初始化buf以前，其值应该是一个随机杂乱值（garbage），就像这里的：`
``(gdb) print buf
$2 = 0xbffffaa8 "鳃?\177\003@t`\001@\001"
``
我们step over（单步执行）malloc调用然后再次检查buf的值：
(gdb) next
10        fgets(buf, 1024, stdin);
(gdb) print buf
$3 = 0x0
``
可见调用了malloc之后，buf是NULL。如果你查看malloc的手册页（man page），你就会发现malloc在不能分配够所需的内存的时候就会返回NULL。因此确定是我们的malloc失败了。让我们返回到代码再次看看：
> ``7 :   buf = malloc(1<<31);
`哦，表达式1<<31（整型1左移31次，原文错写为右移）是429497295, 或4GB (gigabytes).很少有机器会有这样的内存——大多数只有256MB（显然这篇文章有年头了，都什么年代了，这点内存操作系统估计启动一半就挂了）。因此malloc必然会失败。此外，在fgets中我们只读入1024字节。所有的额外空间都会白白浪费掉，尽管我们可以分配到。这里我们将1<<31改为1024（或者1<<9），这样程序就会按照我们的期望运行了：`
` `
~# a.out
Hello World!
Hello World!
这样你就可以知道怎样使用gdb来调试段错误了，这是非常有用的。这个例子同时也说明了一个非常重要的准则：总是检查malloc的返回值！
