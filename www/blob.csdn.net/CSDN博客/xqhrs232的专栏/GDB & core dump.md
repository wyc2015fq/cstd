# GDB & core dump - xqhrs232的专栏 - CSDN博客
2016年10月20日 11:03:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：150
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/fivedoumi/article/details/8139038](http://blog.csdn.net/fivedoumi/article/details/8139038)
from:[http://debugo.blog.51cto.com/3189391/622222](http://debugo.blog.51cto.com/3189391/622222)

一、core dump
1.何谓 core dump？
    在使用半导体作为内存的材料前，人类是利用线圈当作内存的材料，线圈就叫作 core ，用线圈做的内存就叫作core memory。如今，半导体工业澎勃发展，已经没有人用core memory 了，不过，在许多情况下，人们还是把记忆体叫作core 。
    我们在开发（或使用）一个程序时，最怕的就是程序莫明其妙地当掉。虽然系统没事，但我们下次仍可能遇到相同的问题。于是这时操作系统就会把程序当掉 时的内存内容 dump 出来（现在通常是写在一个叫 core的file里面），让 我们或是debugger 做为参考。这个动作就叫作 core dump。
    在 C/C++语言中，最常发生错误的地方就是指 针有问题。您可以利用 core 文件和debugger工具(gdb)把错误找出来。除了SIGSEGV(无效内存访问)信号以外，SIGABRT(异常终止), SIGBUS(硬件故障),SIGEMT(硬件故障),SIGFPE(算术异常),SIGILL(非法硬件指令),SIGIOT(硬件故障),SIGQUIT,SIGSYS(无效系统调用),SIGTRAP(硬件故障)等。
2.设置core dump
    如果完全用不到调试程序，那么用不到核心转储功能。
    要怎么才不会让 core 文件出现
    如果用的是bash的话, 在/etc/profile里加上(或者修改)一条:
    ulimit -c 0
    如果想让系统在信号中断造成的错误时产生core文件, 我们需要在shell中按如下设置:
    首先查看一下现在的设置:#ulimit -a
    #ulimit -c unlimited
 #设置core大小为无限
    #ulimit unlimited
 #设置文件大小为无限
    有些系统发行版本没有默认开启core dump功能，需要将上面的命令加入shell的配置文件，例如~/.bashrc。如果要加入全局配置文件/etc/profile中，这些需要有root权限。
    配置生成core文件的相关参数/proc/sys/kernel/core_uses_pid文件可以控制core文件的文件名是否以pid作为扩展。如果文件内容为1表示添加pid作为扩展，生成core文件的格式为core.xxx;为0表示生成的core文件与运行的程序同名。
    /proc/sys/kernel/core_pattern文件可以控制core文件保存位置和文件名格式，例如
#echo "/corefile/core-e%-%p-%t" > /proc/sys/kernel/core_pattern
#mkdir /corefile
    则保存路径是~/corefile,格式是core-命令名-pid-时间戳 ，其他参数:
%u current uid
%g current gid
%s insert signal that caused the coredump
%h host name where the coredump occurred
二、gdb调试简介
(1).GDB can do four main kinds of things (plus other things in support of these) to help you catch bugs in the act:
·   Start your program, specifying anything that might affect its behavior.
·   Make your program  stop on specified  conditions.
·   Examine  what  has happened, when your program has stopped.
·   Change things in your program, so you can experiment  with correcting the effects of one bug and go on to learn about another.
(2).start GDB
$gdb
$gdb program
$gdb program core
$gdb program proc-pid
 #attch GDB to process proc-pid(unless you also have a core file same name as proc-pid; GDB check for a core dump file firsts) 
(3). frenquently GDB commands
break or b[file:]function
 Set a breakpoint at function (in file). e.g.b main
run or r [arglist]
 Start your program (with  arglist,  if  specified).
btBacktrace: display the program stack.
print exprDisplay the value of an expression.
cContinue running your program (after stopping,e.g. at a breakpoint).
next or n  Execute next program  line  (after  stopping);step over any function calls in the line.
edit [file:]function look at the program line where it is presently stopped.
list [file:]function type the text of the program in  the  vicinity  of where it is presently stopped.
step or s   Execute  next  program  line (after stopping); step into any function calls in the line.
help [name] Show information about GDB  command  name,  or general information about using GDB.
set width 70#设置标准屏幕为70列
quit or qExit from GDB.
for example:
//test.c
(gdb) b main
Breakpoint 1 at 0x400534: file test.c, line 19.
(gdb) b func1
Breakpoint 2 at 0x400524: file test.c, line 13.
(gdb) r
Starting program: /home/luffy/workspace/myapue/test 
Breakpoint 1, main () at test.c:19
19    func1();#noted that the next line is calling a function named func1
(gdb) l#list the current location
14}
15
16int
17main()
18{
19    func1();
20    return 0;
21}
(gbd) s#step into func1
Breakpoint 2, func1 () at test.c:13
13func();
(gbd) l5#let's have a view of this func
10void
11func1()
12{
13func();
14}
(gdb) c
Continuing.
Program received signal SIGSEGV, Segmentation fault.
0x0000000000400508 in func () at test.c:7
7   printf("%d\n", *p);
(gdb) bt
...show informations about stack...
(gdb) c
Continuing.
Program terminated with signal SIGSEGV, Segmentation fault.
The program no longer exists.
(gdb) q
另外，print可以显示当前变量的内容。例如p int_var, array_var或struct_var等等。
三、core dump调试技术
1.  为了使用gdb进行程序调试，需要在编译程序中加入供gdb使用的调试信息。方法是在
编译程序时使用:
-g gdb输出debug调试信息
-DDEBUG 传入宏DEBUG
-UDEBUG 取消宏DEBUG
举例如下
利用gcc的 -DDEBUG选项。
(1).源文件DEBUG.c中有：
#include <stdio.h>
int main(int argc, char *argv[])
{
#ifdef DEBUG
    printf("DEBUG is definded \n");
#else
    printf("DEBUG is not definded \n");
#endif
}
(2). 输入：$gcc -g -o debug DEBUG.c
        $./debug
   out: DEBUG is not definded
(3). 输入：$gcc -DDEBUG -g -o debug DEBUG.c
        $./debug
   out: DEBUG is definded
release版本编译
除去 -g -DDEBUG，并且加上 -o3 ： 最大优化代码
2. 一个简单的例子
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)//test.c
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)#include <stdio.h>
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)
void
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)func()
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif){
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)int *p = NULL;
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)     printf("%d\n", *p);
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)}
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)
int
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)main()
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif){
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)        func();
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)return 0;
![](http://debugo.blog.51cto.com/images/editer/InBlock.gif)}
$gcc -g -o test test.c
$./test.c
out:段错误(核心已转储)
如果没有转储，是因为默认选项的问题。通过echo "ulimit -S -c unlimited > /dev/null 2>&1" >>~/.bashrc,然后应用该配置文件. ~/.bashrc即可。
3. 直接从core文件里获得出错信息，有  
$gdb -c core
$ gdb -c core-e3926-1311507364 
......
[New Thread 3926]
Core was generated by `./test'.
Program terminated with signal 11, Segmentation fault.
(gdb) bt
#0  0x0000000000400508 in ?? ()
#1  0x0000000000000000 in ?? ()
(gdb) q
有时候并不能获得有效信息。
4. 用gdb查看core文件:
下面我们可以在发生运行时信号引起的错误时发生core dump了.
发生core dump之后, 用gdb进行查看core文件的内容, 以定位文件中引发core dump的行.
gdb [exec file] [core file]
或者gdb --core=[core file] [exec file]或者gdb -c [core file] [exec file] 
如:
gdb ./test test.core
在进入gdb后, 用bt(backtrace: display the program stack)命令查看backtrace以检查发生程序运行到哪里, 来定位core dump的文件->行.
Core was generated by `./test.o'.
Program terminated with signal 11, Segmentation fault.
#0  0x0000000000400508 in func () at test.c:7
7   printf("%d\n", *p);
(gdb) bt
#0  0x0000000000400508 in func () at test.c:7
#1  0x000000000040052e in main () at test.c:13
(gdb) q
Reference:
APUE
man gdb
Linux core dump file
 http://apps.hi.baidu.com/share/detail/30247650
Linux下使用core文件调试程序
 http://www.docin.com/p-235044288.html
gdb简介 http://www.360doc.com/content/07/0119/10/4910_336656.shtml

