# gdb常用调试命令以及多线程堆栈的查看 - DoubleLi - 博客园






GDB是GNU开源组织发布的一个强大的UNIX下的程序调试工具。或许，各位比较喜欢那种图形界面方式的，像VC、BCB等IDE的调试，但如果你是在UNIX平台下做软件，你会发现GDB这个调试工具有比VC、BCB的图形化调试器更强大的功能。所谓“寸有所长，尺有所短”就是这个道理。

一般来说GDB主要调试的是C/C++的程序。要调试C/C++的程序，首先在编译时，我们必须要把调试信息加到可执行文件中。使用编译器（cc/gcc/g++）的 -g 参数可以做到这一点。如：



    > cc -g hello.c -o hello
    > g++ -g hello.cpp -o hello

gdb +程序名  运行（gdb main）

使用 "--tui" 参数，可以在终端窗口上部显示一个源代码查看窗。

gdb --tui main



list(l) 查看最近10行源码

list fun 查看fun函数源码

list file:fun 查看file文件中的fun函数源码

list num1 num2 查看num1~num2行的源码



break 行号

break fun 在函数处设置断点

break file:行号 

break file:fun

break if <condition> 条件成立时程序停住

info break (i b)查看断点

watch expr 当expr的值发生改变时，程序停住

delete  n  删除断点



run/r 启动程序

continue(c) 运行至下一个断点

step(s) 单步跟踪，进入函数

next(n) 单步跟踪，不进入函数

finish 运行程序，直到当前函数完成返回，并打印函数返回时的堆栈地址和返回值及参数信息

until 运行程序直到程序退出循环体

print i (p i) 查看变量的值

ptype 查看变量类型

print array 查看数组

print *array@len 查看动态内存

print x=5 改变运行时的数据

print &array 查看数组的地址

回车代表上一个命令



core文件

在程序崩溃时，一般会生成一个文件叫core文件。core文件记录的是程序崩溃时的内存映像，并加入调试信息。core文件生成的过程叫做core dump

设置生成core文件

ulimit -c 查看core-dump状态

ulimit -c 数字 

ulimit -c unlimited 

gdb利用core文件调试

gdb 文件名 core文件

Backtrace(bt)   查看堆栈



多线程调试

info threads 显示当前可调试的所有线程

thread ID 切换当前调试的线程为指定ID的线程

attach process-id 在gdb状态下，开始调试一个正在运行的进程

thread apply all command 所有线程执行command

下面给大家一个好玩的多线程程序，答案就在某个线程的堆栈中，先不要看下面的答案，自己试着用gdb玩一下哈(Linux 64bit)。

http://download.csdn.net/detail/nk_test/9452236

答案在下面图中，先不要看。。

![](http://img.blog.csdn.net/20160303232502440?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](http://img.blog.csdn.net/20160303232519909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**The answer is orangutan.**









