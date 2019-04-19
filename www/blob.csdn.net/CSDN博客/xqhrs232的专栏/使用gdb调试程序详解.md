# 使用gdb调试程序详解 - xqhrs232的专栏 - CSDN博客
2012年04月24日 11:30:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：791标签：[file																[汇编																[bt																[function																[list																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=bt&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.vimer.cn/2009/11/%E4%BD%BF%E7%94%A8gdb%E8%B0%83%E8%AF%95%E7%A8%8B%E5%BA%8F%E8%AF%A6%E8%A7%A3.html](http://www.vimer.cn/2009/11/%E4%BD%BF%E7%94%A8gdb%E8%B0%83%E8%AF%95%E7%A8%8B%E5%BA%8F%E8%AF%A6%E8%A7%A3.html)
相关网帖
1.用GDB调试程序----[http://wiki.ubuntu.org.cn/index.php?title=%E7%94%A8GDB%E8%B0%83%E8%AF%95%E7%A8%8B%E5%BA%8F&variant=zh-cn](http://wiki.ubuntu.org.cn/index.php?title=%E7%94%A8GDB%E8%B0%83%E8%AF%95%E7%A8%8B%E5%BA%8F&variant=zh-cn)
熟练使用gdb是一个linux下开发人员必备的一项技能，我们由浅入深的学习一下gdb的强大功能。
一.gdb简单介绍
名称
    gdb - GNU 调试器
提要
    gdb [-help] [-nx] [-q] [-batch] [-cd=dir]
 [-f] [-b bps]
        [-tty=dev] [-s symfile] [-e prog] [-se prog] [-c
        core] [-x cmds] [-d dir] [prog[core|procID]]
描述
    调试器（如GDB）的目的是允许你在程序运行时进入到某个程序内部去看看该程序在做什么，或者在该程序崩溃时它在做什么。
GDB主要可以做4大类事（加上一些其他的辅助工作），以帮助用户在程序运行过程中发现bug。
    o  启动您的程序，并列出可能会影响它运行的一些信息
    o  使您的程序在特定条件下停止下来
    o  当程序停下来的时候，检查发生了什么（简单来说，就是当在打开了core文件限制后，gdb能够调试core文件来定位程序出现问题的地方）
    o  对程序做出相应的调整，这样您就能尝试纠正一个错误并继续发现其它错误
二.gdb的启动
一般来说GDB主要调试的是C/C++的程序。要调试C/C++的程序，首先 在编译时，我们必须要把调试信息加到可执行文件中。使用编译器（ cc/gcc/g++）的 -g 参数可以做到这一点。如：
gcc -g hello.c 
-o hello 
g++-g hello.cpp
-o hello 
如果没有-g，你将看不见程序的函数名、变量名，所代替的全是运行 时的内存地址。
启动GDB的方法有以下几种：
1、gdb 
<program> program也就是你的执行文件，一般在当然目录下。
2、gdb <program> core 用gdb同时调试一个运行程序和core文件，core是程序非法执行后core dump后产生的文件。
3、gdb <program><PID> 如果你的程序是一个服务程序，那么你可以指定这个服务程序运行时 的进程ID。gdb会自动attach上去，并调试他。program应该在PATH环 境变量中搜索得到。
GDB启动时，可以加上一些GDB的启动开关，详细的开关可以用gdb -help查看。我在下面只例举一些比较常用的参数：
-help (-h)
      列出所有选项，并附简要说明。
-symbols=file (-s file)
      读出文件（file）中的符号表。
-write
      开通(enable)往可执行文件和核心文件写的权限。
-exec=file (-e file)
      在适当时候把File作为可执行的文件执行，来检测与core dump结合的数据。
－se File
      从File读取符号表并把它作为可执行文件。
－core File (-c File)
      把File作为core dump来执行。
－directory=Directory (-d Directory)
      把Dicrctory加入源文件搜索的路径中。
-cd=Directory
      运行GDB,使用Directory作为它的工作目录,取代当前工作目录.
三.gdb的命令字介绍
我们简要介绍一下在gdb调试中需要用到的命令字
file [filename]
      装入想要调试的可执行文件
kill [filename]
      终止正在调试的程序
**break** [file:]function
      在(file文件的)function函数中设置一个断点
clear
      删除一个断点，这个命令需要指定代码行或者函数名作为参数
run [arglist]
      运行您的程序 (如果指定了arglist,则将arglist作为参数运行程序)
bt Backtrace
      显示程序堆栈信息
print expr
      打印表达式的值
**continue**
      继续运行您的程序 (在停止之后，比如在一个断点之后)
list
      列出产生执行文件的源代码的一部分
next
      单步执行 (在停止之后); 跳过函数调用（与step相对应，step会进入函数内部）
nexti
      执行下一行的源代码中的一条汇编指令
set
      设置变量的值。例如：set nval=54 将把54保存到nval变量中；设置输入参数也可以通过这个命令(例如当三个入参分别为a、b、c的话，set args a b c)
step
      单步执行 (在停止之后); 进入函数调用
stepi
      继续执行程序下一行源代码中的汇编指令。如果是函数调用，这个命令将进入函数的内部，单步执行函数中的汇编代码
watch
      使你能监视一个变量的值而不管它何时被改变
rwatch
      指定一个变量，如果这个变量被读，则暂停程序运行，在调试器中显示信息，并等待下一个调试命令。参考rwatch和watch命令
awatch
      指定一个变量，如果这个变量被读或者被写，则暂停程序运行，在调试器中显示信息，并等待下一个调试命令。参考rwatch和watch命令
Ctrl-C
      在当前位置停止执行正在执行的程序，断点在当前行
disable
      禁止断点功能，这个命令需要禁止的断点在断点列表索引值作为参数
display
      在断点的停止的地方，显示指定的表达式的值。(显示变量)
undisplay
      删除一个display设置的变量显示。这个命令需要将display list中的索引做参数
enable
      允许断点功能，这个命令需要允许的断点在断点列表索引值作为参数
finish
      继续执行，直到当前函数返回
ignore
      忽略某个断点制定的次数。例：ignore 4
23 忽略断点4的23次运行，在第24次的时候中断
info [name]
      查看name信息
load
      动态载入一个可执行文件到调试器
xbreak
      在当前函数的退出的点上设置一个断点
whatis
      显示变量的值和类型
ptype
      显示变量的类型
**return**
      强制从当前函数返回
txbreak
      在当前函数的退出的点上设置一个临时的断点(只可使用一次)
make
      使你能不退出 gdb 就可以重新产生可执行文件
shell
      使你能不离开 gdb 就执行 UNIX shell 命令
help [name]
      显示GDB命令的信息，或者显示如何使用GDB的总体信息
quit
      退出gdb.
四.gdb调试程序实例
源程序：tst.c
#**include**<stdio.h>
**int** func(**int** n)
{
**int** sum=0,i;
**for**(i=0; i<n; i++)
    {
        sum+=i;
    }
**return** sum;
}
**void** main()
{
**int** i; **long** result =
0;
**for**(i=1; i<=100; i++)
    {
        result += i;
    }
    printf("result[1-100] = %d \n", result );
    printf("result[1-250] = %d \n", func(250) );
}
编译生成执行文件：（Linux下）
gcc -g tst.c 
-o tst
使用GDB调试：
hchen/test>gdb tst
<---------- 启动GDB
GNU gdb 5.1.1
Copyright 2002 Free Software Foundation, Inc.
GDB is free software, covered by the GNU General Public License, and you are
welcome to change it and/or distribute copies of it under certain conditions.
Type "show copying" to see the conditions.
There is absolutely no warranty **for** GDB. Type
"show warranty"**for** details.
This GDB was configured as "i386-suse-linux"...
(gdb) l <--------------------
 l命令相当于list，从第一行开始例出原码。
1 #**include**<stdio.h>
2
3**int** func(**int** n)
4 {
5**int** sum=0,i;
6**for**(i=0; i<n; i++)
7 {
8 sum+=i;
9 }
10**return** sum;
(gdb) <--------------------
 直接回车表示，重复上一次命令
11 }
12
13
14 main()
15 {
16**int** i;
17**long** result
=0;
18**for**(i=1; i<=100;
 i++)
19 {
20 result += i;
(gdb) **break**
16<--------------------
 设置断点，在源程序第16行处。
Breakpoint 1 at 0x8048496: file tst.c, line
16.
(gdb) **break** func 
<--------------------
 设置断点，在函数func()入口处。
Breakpoint 2 at 0x8048456: file tst.c, line
5.
(gdb) info **break**
<--------------------
 查看断点信息。
Num Type Disp Enb Address What
1 breakpoint keep y 0x08048496 in main at tst.c:16
2 breakpoint keep y 0x08048456 in func at tst.c:5
(gdb) r <---------------------
 运行程序，run命令简写
Starting program:
/home/hchen/test/tst
Breakpoint 1, main () at tst.c:17<---------- 在断点处停住。
17**long** result
=0;
(gdb) n <---------------------
 单条语句执行，next命令简写。
18**for**(i=1; i<=100;
 i++)
(gdb) n
20 result += i;
(gdb) n
18**for**(i=1; i<=100;
 i++)
(gdb) n
20 result += i;
(gdb) c <---------------------
 继续运行程序，**continue**命令简写。
Continuing.
result[1-100]
=5050
<----------程序输出。
Breakpoint 2, func (n=250) at tst.c:5
5**int** sum=0,i;
(gdb) n
6**for**(i=1; i<=n; i++)
(gdb) p i <---------------------
 打印变量i的值，print命令简写。
$1=
134513808
(gdb) n
8 sum+=i;
(gdb) n
6**for**(i=1; i<=n; i++)
(gdb) p sum
$2=
1
(gdb) n
8 sum+=i;
(gdb) p i
$3=
2
(gdb) n
6**for**(i=1; i<=n; i++)
(gdb) p sum
$4=
3
(gdb) bt <---------------------
 查看函数堆栈。
#0 func (n=250) at tst.c:5
#1 0x080484e4 in main () at tst.c:24
#2 0x400409ed in __libc_start_main () from 
/lib/libc.so.6
(gdb) finish <---------------------
 退出函数。
Run till exit from #0 func (n=250) at tst.c:5
0x080484e4 in main () at tst.c:24
24 printf("result[1-250] = %d \n", func(250) );
Value returned is $6
=31375
(gdb) c <---------------------
 继续运行。
Continuing.
result[1-250]
=31375
<----------程序输出。
Program exited with code 027. 
<--------程序退出，调试结束。
(gdb) q <---------------------
 退出gdb。
五.gdb调试core文件实例
假设一个程序a.out 生成了一个 core 文件，那么可以通过如下命令来进入调试：
gdb a.out core
当然如果core文件和a.out没有在同一个目录，也可以直接
gdb -c core
这种情况下gdb是找不到符号表的，所以需要在进入gdb之后通过
file a.out （a.out代表路径）
来载入符号表，不管是哪一种方式，进入调试界面之后，按下where或者bt就可以看见出错的堆栈情况。
转载请注明出处.[http://www.vimer.cn](http://www.vimer.cn/)

