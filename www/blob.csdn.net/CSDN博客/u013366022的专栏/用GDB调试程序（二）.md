# 用GDB调试程序（二） - u013366022的专栏 - CSDN博客
2015年04月10日 14:53:06[slitaz](https://me.csdn.net/u013366022)阅读数：309
个人分类：[GDB调试](https://blog.csdn.net/u013366022/article/category/5798993)
**GDB的命令概貌———————**
启动gdb后，就你被带入gdb的调试环境中，就可以使用gdb的命令开始调试程序了，gdb的命令可以使用help命令来查看，如下所示：
    /home/hchen> gdb
    GNU gdb 5.1.1
    Copyright 2002 Free Software Foundation, Inc.
    GDB is free software, covered by the GNU General Public License, and you are
    welcome to change it and/or distribute copies of it under certain conditions.
    Type "show copying" to see the conditions.
    There is absolutely no warranty for GDB.  Type "show warranty" for details.
    This GDB was configured as "i386-suse-linux".
    (gdb) help
    List of classes of commands:
    aliases -- Aliases of other commands
    breakpoints -- Making program stop at certain points
    data -- Examining data
    files -- Specifying and examining files
    internals -- Maintenance commands
    obscure -- Obscure features
    running -- Running the program
    stack -- Examining the stack
    status -- Status inquiries
    support -- Support facilities
    tracepoints -- Tracing of program execution without stopping the program
    user-defined -- User-defined commands
    Type "help" followed by a class name for a list of commands in that class.
    Type "help" followed by command name for full documentation.
    Command name abbreviations are allowed if unambiguous.
    (gdb)
gdb的命令很多，gdb把之分成许多个种类。help命令只是例出gdb的命令种类，如果要看种类中的命令，可以使用help <class> 命令，如：help breakpoints，查看设置断点的所有命令。也可以直接help <command>来查看命令的帮助。
gdb中，输入命令时，可以不用打全命令，只用打命令的前几个字符就可以了，当然，命令的前几个字符应该要标志着一个唯一的命令，在Linux下，你可以敲击两次TAB键来补齐命令的全称，如果有重复的，那么gdb会把其例出来。
    示例一：在进入函数func时，设置一个断点。可以敲入break func，或是直接就是b func
    (gdb) b func
    Breakpoint 1 at 0x8048458: file hello.c, line 10.
    示例二：敲入b按两次TAB键，你会看到所有b打头的命令：
    (gdb) b
    backtrace  break      bt
    (gdb)
    示例三：只记得函数的前缀，可以这样：
    (gdb) b make_ <按TAB键>
    （再按下一次TAB键，你会看到:）
    make_a_section_from_file     make_environ
    make_abs_section             make_function_type
    make_blockvector             make_pointer_type
    make_cleanup                 make_reference_type
    make_command                 make_symbol_completion_list
    (gdb) b make_
    GDB把所有make开头的函数全部例出来给你查看。
    示例四：调试C++的程序时，有可以函数名一样。如：
    (gdb) b 'bubble( M-? 
    bubble(double,double)    bubble(int,int)
    (gdb) b 'bubble(
    你可以查看到C++中的所有的重载函数及参数。（注：M-?和“按两次TAB键”是一个意思）
要退出gdb时，只用发quit或命令简称q就行了。
**GDB中运行UNIX的shell程序————————————**
在gdb环境中，你可以执行UNIX的shell的命令，使用gdb的shell命令来完成：
    shell <command string>
    调用UNIX的shell来执行<command string>，环境变量SHELL中定义的UNIX的shell将会被用来执行<command string>，如果SHELL没有定义，那就使用UNIX的标准shell：/bin/sh。（在Windows中使用Command.com或cmd.exe）
还有一个gdb命令是make：
    make <make-args> 
    可以在gdb中执行make命令来重新build自己的程序。这个命令等价于“shell make <make-args>”。
**在GDB中运行程序————————**
当以gdb <program>方式启动gdb后，gdb会在PATH路径和当前目录中搜索<program>的源文件。如要确认gdb是否读到源文件，可使用l或list命令，看看gdb是否能列出源代码。
在gdb中，运行程序使用r或是run命令。程序的运行，你有可能需要设置下面四方面的事。
1、程序运行参数。
    set args 可指定运行时参数。（如：set args 10 20 30 40 50）
    show args 命令可以查看设置好的运行参数。
2、运行环境。
    path <dir> 可设定程序的运行路径。
    show paths 查看程序的运行路径。
    set environment varname [=value] 设置环境变量。如：set env USER=hchen
    show environment [varname] 查看环境变量。
3、工作目录。
    cd <dir> 相当于shell的cd命令。
    pwd 显示当前的所在目录。
4、程序的输入输出。
    info terminal 显示你程序用到的终端的模式。
    使用重定向控制程序输出。如：run > outfile
    tty命令可以指写输入输出的终端设备。如：tty /dev/ttyb
**调试已运行的程序————————**
两种方法：
1、在UNIX下用ps查看正在运行的程序的PID（进程ID），然后用gdb <program> PID格式挂接正在运行的程序。
2、先用gdb <program>关联上源代码，并进行gdb，在gdb中用attach命令来挂接进程的PID。并用detach来取消挂接的进程。
**暂停 / 恢复程序运行—————————**
调试程序中，暂停程序运行是必须的，GDB可以方便地暂停程序的运行。你可以设置程序的在哪行停住，在什么条件下停住，在收到什么信号时停往等等。以便于你查看运行时的变量，以及运行时的流程。
当进程被gdb停住时，你可以使用info program 来查看程序的是否在运行，进程号，被暂停的原因。
在gdb中，我们可以有以下几种暂停方式：断点（BreakPoint）、观察点（WatchPoint）、捕捉点（CatchPoint）、信号（Signals）、线程停止（Thread Stops）。如果要恢复程序运行，可以使用c或是continue命令。
**一、设置断点（BreakPoint）**
    我们用break命令来设置断点。正面有几点设置断点的方法：
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
        ...可以是上述的参数，condition表示条件，在条件成立时停住。比如在循环境体中，可以设置break if i=100，表示当i为100时停住程序。
    查看断点时，可使用info命令，如下所示：（注：n表示断点号）
    info breakpoints [n] 
    info break [n] 
**二、设置观察点（WatchPoint）**
    观察点一般来观察某个表达式（变量也是一种表达式）的值是否有变化了，如果有变化，马上停住程序。我们有下面的几种方法来设置观察点：
    watch <expr>
        为表达式（变量）expr设置一个观察点。一量表达式值有变化时，马上停住程序。
    rwatch <expr>
        当表达式（变量）expr被读时，停住程序。
    awatch <expr>
        当表达式（变量）的值被读或被写时，停住程序。
    info watchpoints
        列出当前所设置了的所有观察点。
**三、设置捕捉点（CatchPoint）**
    你可设置捕捉点来补捉程序运行时的一些事件。如：载入共享库（动态链接库）或是C++的异常。设置捕捉点的格式为：
    catch <event>
        当event发生时，停住程序。event可以是下面的内容：
        1、throw 一个C++抛出的异常。（throw为关键字）
        2、catch 一个C++捕捉到的异常。（catch为关键字）
        3、exec 调用系统调用exec时。（exec为关键字，目前此功能只在HP-UX下有用）
        4、fork 调用系统调用fork时。（fork为关键字，目前此功能只在HP-UX下有用）
        5、vfork 调用系统调用vfork时。（vfork为关键字，目前此功能只在HP-UX下有用）
        6、load 或 load <libname> 载入共享库（动态链接库）时。（load为关键字，目前此功能只在HP-UX下有用）
        7、unload 或 unload <libname> 卸载共享库（动态链接库）时。（unload为关键字，目前此功能只在HP-UX下有用）
    tcatch <event> 
        只设置一次捕捉点，当程序停住以后，应点被自动删除。
