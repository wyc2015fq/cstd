
# GDB调试技术（二） - 高科的专栏 - CSDN博客

2012年08月22日 21:09:34[高科](https://me.csdn.net/pbymw8iwm)阅读数：2463


1）恢复程序运行和单步调试当程序被停住了，你可以用continue命令恢复程序的运行直到程序结束，或下一个断点到来。也可以使用step或next命令单步跟踪程序。
continue [ignore-count]
c [ignore-count]
fg [ignore-count]
恢复程序运行，直到程序结束，或是下一个断点到来。ignore-count表示忽略其后的断点次数。continue，c，fg三个命令都是一样的意思。
step <count>
单步跟踪，如果有函数调用，他会进入该函数。进入函数的前提是，此函数被编译有debug信息。很像VC等工具中的stepin。后面可以加count也可以不加，不加表示一条条地执行，加表示执行后面的count条指令，然后再停住。
next <count>
同样单步跟踪，如果有函数调用，他不会进入该函数。很像VC等工具中的step
 over。后面可以加count也可以不加，不加表示一条条地执行，加表示执行后面的count条指令，然后再停住。
set step-mode
set step-mode on
打开step-mode模式，于是，在进行单步跟踪时，程序不会因为没有debug信息而不停住。这个参数有很利于查看机器码。
set step-mod off
关闭step-mode模式。
finish
运行程序，直到当前函数完成返回。并打印函数返回时的堆栈地址和返回值及参数值等信息。
until或u
当你厌倦了在一个循环体内单步跟踪时，这个命令可以运行程序直到退出循环体。
stepi或si
nexti或ni
单步跟踪一条机器指令！一条程序代码有可能由数条机器指令完成，stepi和nexti可以单步执行机器指令。与之一样有相同功能的命令是“display/i
 $pc”，当运行完这个命令后，单步跟踪会在打出程序代码的同时打出机器指令（也就是汇编代码）
2）信号（Signals）
信号是一种软中断，是一种处理异步事件的方法。一般来说，操作系统都支持许多信号。尤其是UNIX，比较重要应用程序一般都会处理信号。UNIX定义了许多信号，比如SIGINT表示中断字符信号，也就是Ctrl+C的信号，SIGBUS表示硬件故障的信号；SIGCHLD表示子进程状态改变信号；SIGKILL表示终止程序运行的信号，等等。信号量编程是UNIX下非常重要的一种技术。
GDB有能力在你调试程序的时候处理任何一种信号，你可以告诉GDB需要处理哪一种信号。你可以要求GDB收到你所指定的信号时，马上停住正在运行的程序，以供你进行调试。你可以用GDB的handle命令来完成这一功能。
handle <signal> <keywords...>
在GDB中定义一个信号处理。信号<signal>可以以SIG开头或不以SIG开头，可以用定义一个要处理信号的范围（如：SIGIO-SIGKILL，表示处理从SIGIO信号到SIGKILL的信号，其中包括SIGIO，SIGIOT，SIGKILL三个信号），也可以使用关键字all来标明要处理所有的信号。一旦被调试的程序接收到信号，运行程序马上会被GDB停住，以供调试。其<keywords>可以是以下几种关键字的一个或多个。
|nostop
|当被调试的程序收到信号时，GDB|不会停住程序的运行，但会打出消息告诉你收到这种信号。
|
|stop
|当被调试的程序收到信号时，GDB|会停住你的程序。
|
|print
|当被调试的程序收到信号时，GDB|会显示出一条信息。
|
|noprint
|当被调试的程序收到信号时，GDB|不会告诉你收到信号的信息。
|
|pass
|noignore
|当被调试的程序收到信号时，GDB|不处理信号。这表示，|GDB|会把这个信号交给被调试程序|来|处理。
|
|nopass
|ignore
|当被调试的程序收到信号时，GDB|不会让被调试程序来处理这个信号。
|
|info signals
|info handle
|查看有哪些信号在被GDB|检测中。
|
3）线程（Thread Stops）
如果你程序是多线程的话，你可以定义你的断点是否在所有的线程上，或是在某个特定的线程。GDB很容易帮你完成这一工作。
break <linespec> thread <threadno>
break <linespec> thread <threadno> if ...
linespec指定了断点设置在的源程序的行号。threadno指定了线程的ID，注意，这个ID是GDB分配的，你可以通过“infothreads”命令来查看正在运行程序中的线程信息。如果你不指定thread
 <threadno>则表示你的断点设在所有线程上面。你还可以为某线程指定断点条件。如：
(gdb) break frik.c:13 thread 28 if bartab > lim
当你的程序被GDB停住时，所有的运行线程都会被停住。这方便你你查看运行程序的总体情况。而在你恢复程序运行时，
所有的线程也会被恢复运行。那怕是主进程在被单步调试时。
**A、****查看栈信息**
当程序被停住了，你需要做的第一件事就是查看程序是在哪里停住的。当你的程序调用了一个函数，函数的地址，函数参数，函数内的局部变量都会被压入“栈”（Stack）中。你可以用GDB命令来查看当前的栈中的信息。
下面是一些查看函数调用栈信息的GDB命令：
backtrace
bt
打印当前的函数调用栈的所有信息。如：
(gdb) bt
\#0 func (n=250) at tst.c:6
\#1 0x08048524 in main (argc=1, argv=0xbffff674) at tst.c:30
\#2 0x400409ed in __libc_start_main () from /lib/libc.so.6
从上可以看出函数的调用栈信息：__libc_start_main --> main() --> func()
backtrace <n>
bt <n>
n是一个正整数，表示只打印栈顶上n层的栈信息。
backtrace <-n>
bt <-n>
-n表一个负整数，表示只打印栈底下n层的栈信息。
如果你要查看某一层的信息，你需要在切换当前的栈，一般来说，程序停止时，最顶层的栈就是当前栈，如果你要查看
栈下面层的详细信息，首先要做的是切换当前栈。
frame <n>
f <n>
n是一个从0开始的整数，是栈中的层编号。比如：frame
 0，表示栈顶，frame 1，表示栈的第二层。
up <n>
表示向栈的上面移动n层，可以不打n，表示向上移动一层。
down <n>
表示向栈的下面移动n层，可以不打n，表示向下移动一层。
上面的命令，都会打印出移动到的栈层的信息。如果你不想让其打出信息。你可以使用这三个命令：
select-frame <n>对应于frame命令。
up-silently <n>对应于up命令。
down-silently <n>对应于down命令。
查看当前栈层的信息，你可以用以下GDB命令：
frame或f
会打印出这些信息：栈的层编号，当前的函数名，函数参数值，函数所在文件及行号，函数执行到的语句。
info frame
info f
这个命令会打印出更为详细的当前栈层的信息，只不过，大多数都是运行时的内内地址。比如：函数地址，调用函数的地址，被调用函数的地址，目前的函数是由什么样的程序语言写成的、函数参数地址及值、局部变量的地址等等。如：
(gdb) info f
Stack level 0, frame at 0xbffff5d4:
eip = 0x804845d in func (tst.c:6); saved eip 0x8048524
called by frame at 0xbffff60c
source language c.
Arglist at 0xbffff5d4, args: n=250
Locals at 0xbffff5d4, Previous frame''s sp is 0x0
Saved registers:
ebp at 0xbffff5d4, eip at 0xbffff5d8
info args
打印出当前函数的参数名及其值。
info locals
打印出当前函数中所有局部变量及其值。
info catch
打印出当前的函数中的异常处理信息。
C、产生信号量
使用singal命令，可以产生一个信号量给被调试的程序。如：中断信号Ctrl+C。这非常方便于程序的调试，可以在程序运行的任意位置设置断点，并在该断点用GDB产生一个信号量，这种精确地在某处产生信号非常有利程序的调试。
语法是：signal <singal>，UNIX的系统信号量通常从1到15。所以<singal>取值也在这个范围。
single命令和shell的kill命令不同，系统的kill命令发信号给被调试程序时，是由GDB截获的，而single命令所发出一信号则是直接发给被调试程序的。
D、强制函数返回
如果你的调试断点在某个函数中，并还有语句没有执行完。你可以使用return命令强制函数忽略还没有执行的语句并返回。
return
return <expression>
使用return命令取消当前函数的执行，并立即返回，如果指定了<expression>，那么该表达式的值会被认作函数的返回值。
E、强制调用函数
call <expr>
表达式中可以一是函数，以此达到强制调用函数的目的。并显示函数的返回值，如果函数返回值是void，那么就不显示。
另一个相似的命令也可以完成这一功能——print，print后面可以跟表达式，所以也可以用他来调用函数，print和call的不同是，如果函数返回void，call则不显示，print则显示函数返回值，并把该值存入历史数据中。
F、在不同语言中使用GDB
GDB支持下列语言：C, C++, Fortran, PASCAL, Java, Chill, assembly,和Modula-2。一般说来，GDB会根据你所调试
的程序来确定当然的调试语言，比如：发现文件名后缀为“.c”的，GDB会认为是C程序。文件名后缀为“.C,
 .cc, .cp,.cpp, .cxx, .c++”的，GDB会认为是C++程序。而后缀是“.f, .F”的，GDB会认为是Fortran程序，还有，后缀为如果是“.s,
 .S”的会认为是汇编语言。
也就是说，GDB会根据你所调试的程序的语言，来设置自己的语言环境，并让GDB的命令跟着语言环境的改变而改变。比如一些GDB命令需要用到表达式或变量时，这些表达式或变量的语法，完全是根据当前的语言环境而改变的。例如C/C++中对指针的语法是*p，而在Modula-2中则是p^。并且，如果你当前的程序是由几种不同语言一同编译成的，那到在调试过程中，GDB也能根据不同的语言自动地切换语言环境。这种跟着语言环境而改变的功能，真是体贴开发人员的一种设计。
下面是几个相关于GDB语言环境的命令：
show language
查看当前的语言环境。如果GDB不能识为你所调试的编程语言，那么，C语言被认为是默认的环境。
info frame
查看当前函数的程序语言。
info source
查看当前文件的程序语言。
如果GDB没有检测出当前的程序语言，那么你也可以手动设置当前的程序语言。使用set language命令即可做到。
当set language命令后什么也不跟的话，你可以查看GDB所支持的语言种类：
(gdb) set language


