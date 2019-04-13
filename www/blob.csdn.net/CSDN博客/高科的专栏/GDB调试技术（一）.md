
# GDB调试技术（一） - 高科的专栏 - CSDN博客

2012年08月22日 21:00:49[高科](https://me.csdn.net/pbymw8iwm)阅读数：2314


**启动GDB**的方法有以下几种：
1、gdb <program>
program也就是你的执行文件，一般在当然目录下。
2、gdb <program> core
用gdb同时调试一个运行程序和core文件，core是程序非法执行后core
 dump后产生的文件。
3、gdb <program> <PID>
如果你的程序是一个服务程序，那么你可以指定这个服务程序运行时的进程ID。gdb会自动attach上去，并调试他。
4、在gdb中，运行程序使用r或是run命令。程序的运行，你有可能需要设置下面四方面的事。
1、程序运行参数。
set args可指定运行时参数。（如：set args 10 20 30 40 50）
show args命令可以查看设置好的运行参数。
2、运行环境。
path <dir>可设定程序的运行路径。
show paths查看程序的运行路径。
set environment varname [=value]设置环境变量。如：set env USER=edward
show environment [varname]查看环境变量。
3、工作目录。
cd <dir>相当于shell的cd命令。
pwd显示当前的所在目录。
4、程序的输入输出。
info terminal显示你程序用到的终端的模式。
使用重定向控制程序输出。如：run > outfile
tty命令可以指写输入输出的终端设备。如：tty /dev/ttyb
5、过程控制
（1）设置断点（BreakPoint）
我们用break命令来设置断点。有几种设置断点的方法：
break <function>
在进入指定函数时停住。C++中可以使用class::function或function(type,type)格式来指定函数名。
break <linenum>
在指定行号停住。
break +offset
break -offset
在当前行号的前面或后面的offset行停住。offiset为自然数。
break filename:linenum
在源文件filename的linenum行处停住。
break filename:function
在源文件filename的function函数的入口处停住。
break *address
在程序运行的内存地址处停住。
break
break命令没有参数时，表示在下一条指令处停住。
break ... if <condition>
可以是上述的参数，condition表示条件，在条件成立时停住。比如在循环境体中，可以设置break if i=100，表示当i为100时停住程序。
查看断点时，可使用info命令，如下所示：（注：n表示断点号）
info breakpoints [n]
info break [n]
（2）设置观察点（WatchPoint）
观察点一般来观察某个表达式（变量也是一种表达式）的值是否有变化了，如果有变化，马上停住程序。我们有下面的几种方法来设置观察点：
watch <expr>
为表达式（变量）expr设置一个观察点。一量表达式值有变化时，马上停住程序。
rwatch <expr>
当表达式（变量）expr被读时，停住程序。
awatch <expr>
当表达式（变量）的值被读或被写时，停住程序。
info watchpoints
列出当前所设置了的所有观察点。
（3）设置捕捉点（CatchPoint）
你可设置捕捉点来捕捉程序运行时的一些事件。如：载入共享库（动态链接库）或是C++的异常。设置捕捉点的格式为：
catch <event>
当event发生时，停住程序。event可以是下面的内容：
1、throw一个C++抛出的异常。（throw为关键字）
2、catch一个C++捕捉到的异常。（catch为关键字）
3、exec调用系统调用exec时。（exec为关键字，目前此功能只在HP-UX下有用）
4、fork调用系统调用fork时。（fork为关键字，目前此功能只在HP-UX下有用）
5、vfork调用系统调用vfork时。（vfork为关键字，目前此功能只在HP-UX下有用）
6、load或load <libname>载入共享库（动态链接库）时。（load为关键字，目前只在HP-UX下有用）
7、unload或unload <libname>卸载共享库（动态链接库）时。（unload为关键字，只在HP-UX下有用）
tcatch <event>
只设置一次捕捉点，当程序停住以后，应点被自动删除。

