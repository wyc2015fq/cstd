# gdb参数详解 - ljx0305的专栏 - CSDN博客
2008年07月27日 18:38:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：627标签：[file																[function																[command																[shell																[linux																[fortran](https://so.csdn.net/so/search/s.do?q=fortran&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[C++																[C](https://blog.csdn.net/ljx0305/article/category/394717)](https://blog.csdn.net/ljx0305/article/category/380566)
名称
        gdb - GNU 调试器
提要
        gdb    [-help] [-nx] [-q] [-batch] [-cd=dir] [-f] [-b bps]
              [-tty=dev] [-s symfile] [-e prog] [-se prog] [-c
              core] [-x cmds] [-d dir] [prog[core|procID]]
描述
        调试器（如GDB）的目的是允许你在程序运行时进入到某个程序内部去看看该程序在做什么，或者在该程序崩溃时它在做什么。
        GDB主要可以做4大类事（加上一些其他的辅助工作），以帮助用户在程序运行过程中发现bug。
          o  启动您的程序，并列出可能会影响它运行的一些信息
          o  使您的程序在特定条件下停止下来
          o  当程序停下来的时候，检查发生了什么
          o  对程序做出相应的调整，这样您就能尝试纠正一个错误并继续发现其它错误
        您能使用GDB调试用C、C++、Modula-2写的程序。等GNU Fortran编译器准备好过后，GDB将提供对Fortran的支持
        GDB通过在命令行方式下输入gdb来执行。启动过后，GDB会从终端读取命令，直到您输入GDB命令quit使GDB退出。您能通过GDB命令help获取在线帮助。
        您能以无参数无选项的形式运行GDB，不过通常的情况是以一到两个参数运行GDB，以待调试的可执行程序名为参数
        gdb 程序名
        您能用两个参数来运行GDB，可执行程序名与core文件(译注：不知道怎么翻译好，就不翻译了)。
        gdb 程序名 core
        您可以以进程ID作为第二个参数，以调式一个正在运行的进程
        gdb 程序名 1234
        将会把gdb附在进程1234之上(除非您正好有个文件叫1234，gdb总是先查找core文件)
       下面是一些最常用的GDB命令:
       file [filename]
              装入想要调试的可执行文件
       kill [filename]
              终止正在调试的程序
       break [file:]function
              在(file文件的)function函数中设置一个断点
       clear
              删除一个断点，这个命令需要指定代码行或者函数名作为参数
       run [arglist]
              运行您的程序 (如果指定了arglist,则将arglist作为参数运行程序)
       bt Backtrace: 显示程序堆栈信息
       print expr
              打印表达式的值
       continue
              继续运行您的程序 (在停止之后，比如在一个断点之后)
       list
              列出产生执行文件的源代码的一部分
       next
              单步执行 (在停止之后); 跳过函数调用
       nexti
              执行下一行的源代码中的一条汇编指令
       set
              设置变量的值。例如：set nval=54 将把54保存到nval变量中
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
              忽略某个断点制定的次数。例：ignore 4 23 忽略断点4的23次运行，在第24次的时候中断
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
       return
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
       要得到所有使用GDB的资料，请参考Using GDB: A Guide to the GNU
       Source-Level  Debugger,  by Richard M. Stallman and Roland
       H. Pesch.  当用info查看的时候，也能看到相同的文章
选项
        任何参数而非选项指明了一个可执行文件及core 文件(或者进程ID)；所
        遇到的第一个未关联选项标志的参数与 '-se' 选项等价，第二个，如果存
        在，且是一个文件的名字，则等价与 '-c' 选项。许多选项都有一个长格式
        与短格式；都会在这里表示出来。如果你把一个长格式截短，只要不引起歧
        义，那么它还是可以被识别。(如果你愿意，你可以使用 '+' 而非 '-' 标
        记选项参数，不过我们在例子中仍然遵从通常的惯例)
        -help
       -h     列出所有选项，并附简要说明。   
       -symbols=file
       -s file
              读出文件（file）中的符号表。
       -write 
              开通(enable)往可执行文件和核心文件写的权限。
       -exec=file
       -e file
              在适当时候把File作为可执行的文件执行，来检测与core dump结合的数据。
       －se File
              从File读取符号表并把它作为可执行文件。
       －core File
       -c File
              把File作为core dump来执行。
       －command=File
       -x File
              从File中执行GDB命令。
       －directory=Directory
       -d Directory
              把Dicrctory加入源文件搜索的路径中。
       -nx
       -n
              不从任何.gdbinit初始化文件中执行命令。通常情况下，这些文件中的命令是在所有命令选项和参数处理完后才执行。
       -quiet
       -q
              "Quiet".不输入介绍和版权信息。这些信息输出在batch模式下也被关闭。
       -batch
              运行batch模式。在处理完所有用'-x'选项指定的命令文件(还有'.gdbi-nit',如果没禁用)后退出，并返回状态码0.如果在命令文件中的命令被
执行时发生错误，则退出，并返回状态码非0.batch模式对于运行GDB作为过滤器也许很有用,比如要从另一台电脑上下载并运行一个程序;为了让这些更有用,当
在batch模式下运行时,消息:Program exited normally.(不论什么时候,一个程序在GDB控制下终止运行,这条消息都会正常发出.),将不会发出.
       -cd=Directory
              运行GDB,使用Directory作为它的工作目录,取代当前工作目录.
       -fullname
       -f
              当Emacs让GDB作为一个子进程运行时,设置这个选项.它告诉GDB每当一个堆栈结构(栈帧)显示出来(包括每次程序停止)就用标准的,认同的方式
输出文件全名和行号.这里,认同的格式看起来像两个' 32'字符,紧跟文件名,行号和字符位置(由冒号,换行符分隔).Emacs同GDB的接口程序使用这两个' 32'字
符作为一个符号为框架来显示源代码.
       -b Bps
              设置行速(波特率或bits/s).在远程调试中GDB在任何串行接口中使用的行速.
       -tty=Device
              使用Device作为你程序运行的标准输入输出.
gdb主要调试的是C/C++的程序。要调试C/C++的程序，首先在编译时，必须要把调试信息加到可执行文件中。使用编译器(cc/gcc/g++)的 -g 参数即可。如：
[david@DAVID david]$ gcc -g hello.c -o hello
[david@DAVID david]$ g++ -g hello.cpp -o hello
如果没有-g，将看不见程序的函数名和变量名，代替它们的全是运行时的内存地址。当用-g把调试信息加入，并成功编译目标代码以后，看看如何用gdb来调试。
启动gdb的方法有以下几种：
1. gdb <program> 
program也就是执行文件，一般在当前目录下。
2. gdb <program> core
用gdb同时调试一个运行程序和core文件，core是程序非法执行后，core dump后产生的文件。
3. gdb <program> <PID>
如果程序是一个服务程序，那么可以指定这个服务程序运行时的进程ID。gdb会自动attach上去，并调试它。program应该在PATH环境变量中搜索得到。 
gdb启动时，可以加上一些gdb的启动开关，详细的开关可以用gdb -help查看。下面只列举一些比较常用的参数：
-symbols <file>
-s <file> 
从指定文件中读取符号表。
-se file 
从指定文件中读取符号表信息，并把它用在可执行文件中。
-core <file>
-c <file> 
调试时core dump的core文件。
-directory <directory>
-d <directory>
加入一个源文件的搜索路径。默认搜索路径是环境变量中PATH所定义的路径。
4.1.1  gdb的命令概貌
启动gdb后，进入gdb的调试环境中，就可以使用gdb的命令开始调试程序了。gdb的命令可以使用help命令来查看，如下所示：   
[david@DAVID david]$ gdb
GNU gdb Red Hat Linux (5.3post-0.20021129.18rh)
Copyright 2003 Free Software Foundation, Inc.
gdb is free software, covered by the GNU General Public License, and you are
welcome to change it and/or distribute copies of it under certain conditions.
Type "show copying" to see the conditions.
There is absolutely no warranty for gdb.  Type "show warranty" for details.
This gdb was configured as "i386-redhat-Linux-gnu".
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
gdb的命令很多，gdb将之分成许多种类。help命令只是列出gdb的命令种类，如果要看其中的命令，可以使用help <class> 命令。如：
(gdb) help data
也可以直接用help [command]来查看命令的帮助。
gdb中，输入命令时，可以不用输入全部命令，只用输入命令的前几个字符就可以了。当然，命令的前几个字符应该标志着一个惟一的命令，在Linux下，可以按两次TAB键来补齐命令的全称，如果有重复的，那么gdb会把它全部列出来。
示例一：在进入函数func时，设置一个断点。可以输入break func，或是直接输入b func。
(gdb) b func
Breakpoint 1 at 0x804832e: file test.c, line 5.
(gdb)
示例二：输入b按两次TAB键，你会看到所有b开头的命令。
(gdb) b
backtrace  break  bt
要退出gdb时，只用输入quit或其简称q就行了。 
4.1.2  gdb中运行Linux的shell程序
在gdb环境中，可以执行Linux的shell命令：
shell <command string>
调用Linux的shell来执行<command string>，环境变量SHELL中定义的Linux的shell将会用来执行<command string>。如果SHELL没有定义，那就使用Linux的标准shell：/bin/sh(在Windows中使用Command.com或cmd.exe)。
还有一个gdb命令是make：
make <make-args> 
可以在gdb中执行make命令来重新build自己的程序。这个命令等价于shell make <make-args>。  
4.1.3  在gdb中运行程序
当以gdb <program>方式启动gdb后，gdb会在PATH路径和当前目录中搜索<program>的源文件。如要确认gdb是否读到源文件，可使用l或list命令，看看gdb是否能列出源代码。
在gdb中，运行程序使用r或是run命令。程序的运行，有可能需要设置下面四方面的事。
1. 程序运行参数
set args 可指定运行时参数。如：
set args 10 20 30 40 50
show args 命令可以查看设置好的运行参数。
2. 运行环境
path <dir> 可设定程序的运行路径。
show paths 查看程序的运行路径。
set environment varname [=value] 设置环境变量。如：
set env USER=hchen
show environment [varname] 查看环境变量。
3. 工作目录
cd <dir> 相当于shell的cd命令。
pwd 显示当前的所在目录。
4. 程序的输入输出
info terminal 显示程序用到的终端的模式。
使用重定向控制程序输出。如：
run > outfile
tty命令可以指写输入输出的终端设备。如：
tty /dev/ttyb
4.1.4  调试已运行的程序
调试已经运行的程序有两种方法：
●       在Linux下用ps(第一章已经对ps作了介绍)查看正在运行的程序的PID(进程ID)，然后用gdb <program> PID格式挂接正在运行的程序。
●       先用gdb <program>关联上源代码，并进行gdb，在gdb中用attach命令来挂接进程的PID，并用detach来取消挂接的进程。
4.1.5  暂停/恢复程序运行
调试程序中，暂停程序运行是必需的，gdb可以方便地暂停程序的运行。可以设置程序在哪行停住，在什么条件下停住，在收到什么信号时停往等，以便于用户查看运行时的变量，以及运行时的流程。
当进程被gdb停住时，可以使用info program 来查看程序是否在运行、进程号、被暂停的原因。
在gdb中，有以下几种暂停方式：断点(BreakPoint)、观察点(WatchPoint)、捕捉点(CatchPoint)、信号(Signals)及线程停止(Thread Stops)。
如果要恢复程序运行，可以使用c或是continue命令。
1. 设置断点(BreakPoint)   
用break命令来设置断点。有下面几种设置断点的方法：
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
该命令没有参数时，表示在下一条指令处停住。
break ... if <condition>
condition表示条件，在条件成立时停住。比如在循环体中，可以设置break if i=100，表示当i为100时停住程序。
查看断点时，可使用info命令，如下所示(注：n表示断点号)：
info breakpoints [n] 
info break [n]  
2. 设置观察点(WatchPoint)   
观察点一般用来观察某个表达式(变量也是一种表达式)的值是否变化了。如果有变化，马上停住程序。有下面的几种方法来设置观察点：  
watch <expr>
为表达式(变量)expr设置一个观察点。一旦表达式值有变化时，马上停住程序。     
rwatch <expr>
当表达式(变量)expr被读时，停住程序。    
awatch <expr>
当表达式(变量)的值被读或被写时，停住程序。 
info watchpoints
列出当前设置的所有观察点。
3. 设置捕捉点(CatchPoint)
可设置捕捉点来补捉程序运行时的一些事件。如载入共享库(动态链接库)或是C++的异常。设置捕捉点的格式为：
catch <event>
当event发生时，停住程序。event可以是下面的内容：
●       throw  一个C++抛出的异常 (throw为关键字)。
●       catch  一个C++捕捉到的异常 (catch为关键字)。
●       exec  调用系统调用exec时(exec为关键字，目前此功能只在HP-UX下有用)。
●       fork  调用系统调用fork时(fork为关键字，目前此功能只在HP-UX下有用)。
●       vfork  调用系统调用vfork时(vfork为关键字，目前此功能只在HP-UX下有)。
●       load 或 load <libname>  载入共享库(动态链接库)时 (load为关键字，目前此功能只在HP-UX下有用)。
●       unload 或 unload <libname>  卸载共享库(动态链接库)时 (unload为关键字，目前此功能只在HP-UX下有用)。
tcatch <event> 
只设置一次捕捉点，当程序停住以后，应点被自动删除。
4. 维护停止点
上面说了如何设置程序的停止点，gdb中的停止点也就是上述的三类。在gdb中，如果觉得已定义好的停止点没有用了，可以使用delete、clear、disable、enable这几个命令来进行维护。
Clear
清除所有的已定义的停止点。
clear <function>
clear <filename:function>
清除所有设置在函数上的停止点。
clear <linenum>
clear <filename:linenum>
清除所有设置在指定行上的停止点。
delete [breakpoints] [range...]
删除指定的断点，breakpoints为断点号。如果不指定断点号，则表示删除所有的断点。range 表示断点号的范围(如:3-7)。其简写命令为d。
比删除更好的一种方法是disable停止点。disable了的停止点，gdb不会删除，当还需要时，enable即可，就好像回收站一样。
disable [breakpoints] [range...]
disable所指定的停止点，breakpoints为停止点号。如果什么都不指定，表示disable所有的停止点。简写命令是dis.
enable [breakpoints] [range...]
enable所指定的停止点，breakpoints为停止点号。
enable [breakpoints] once range...
enable所指定的停止点一次，当程序停止后，该停止点马上被gdb自动disable。
enable [breakpoints] delete range...
enable所指定的停止点一次，当程序停止后，该停止点马上被gdb自动删除。
5. 停止条件维护
前面在介绍设置断点时，提到过可以设置一个条件，当条件成立时，程序自动停止。这是一个非常强大的功能，这里，专门介绍这个条件的相关维护命令。
一般来说，为断点设置一个条件，可使用if关键词，后面跟其断点条件。并且，条件设置好后，可以用condition命令来修改断点的条件(只有break和watch命令支持if，catch目前暂不支持if)。
condition <bnum> <expression>
修改断点号为bnum的停止条件为expression。
condition <bnum>
清除断点号为bnum的停止条件。
还有一个比较特殊的维护命令ignore，可以指定程序运行时，忽略停止条件几次。
ignore <bnum> <count>
表示忽略断点号为bnum的停止条件count次。
6. 为停止点设定运行命令
可以使用gdb提供的command命令来设置停止点的运行命令。也就是说，当运行的程序在被停住时，我们可以让其自动运行一些别的命令，这很有利行自动化调试。
commands [bnum]
... command-list ...
end
为断点号bnum指定一个命令列表。当程序被该断点停住时，gdb会依次运行命令列表中的命令。
例如：
break foo if x>0
commands
printf "x is %d/n",x
continue
end
断点设置在函数foo中，断点条件是x>0，如果程序被断住后，也就是一旦x的值在foo函数中大于0，gdb会自动打印出x的值，并继续运行程序。
如果要清除断点上的命令序列，那么只要简单地执行一下commands命令，并直接在输入end就行了。
7. 断点菜单
在C++中，可能会重复出现同一个名字的函数若干次(函数重载)。在这种情况下，break <function>不能告诉gdb要停在哪个函数的入口。当然，可以使用break <function(type)>，也就是把函数的参数类型告诉gdb，以指定一个函数。否则的话，gdb会列出一个断点菜单供用户选择所需要的断点。只要输入菜单列表中的编号就可以了。如：
(gdb) b String::after
[0] cancel
[1] all
[2] file:String.cc; line number:867
[3] file:String.cc; line number:860
[4] file:String.cc; line number:875
[5] file:String.cc; line number:853
[6] file:String.cc; line number:846
[7] file:String.cc; line number:735
> 2 4 6
Breakpoint 1 at 0xb26c: file String.cc, line 867.
Breakpoint 2 at 0xb344: file String.cc, line 875.
Breakpoint 3 at 0xafcc: file String.cc, line 846.
Multiple breakpoints were set.
Use the "delete" command to delete unwanted
breakpoints.
(gdb)    
可见，gdb列出了所有after的重载函数，选一下列表编号就行了。0表示放弃设置断点，1表示所有函数都设置断点。
8. 恢复程序运行和单步调试
当程序被停住后，可以用continue命令恢复程序的运行直到程序结束，或下一个断点到来。也可以使用step或next命令单步跟踪程序。
continue [ignore-count]
c [ignore-count]
fg [ignore-count]
恢复程序运行，直到程序结束，或是下一个断点到来。ignore-count表示忽略其后的断点次数。continue，c，fg三个命令都是一样的意思。
step <count>
单步跟踪，如果有函数调用，它会进入该函数。进入函数的前提是，此函数被编译有debug信息。很像VC等工具中的step in。后面可以加count也可以不加，不加表示一条条地执行，加表示执行后面的count条指令，然后再停住。
next <count>
同样单步跟踪，如果有函数调用，它不会进入该函数(很像VC等工具中的step over)。后面可以加count也可以不加，不加表示一条条地执行，加表示执行后面的count条指令，然后再停住。
set step-mode
set step-mode on
打开step-mode模式。在进行单步跟踪时，程序不会因为没有debug信息而不停住。这个参数有很利于查看机器码。
set step-mod off
关闭step-mode模式。
finish
运行程序，直到当前函数完成返回。并打印函数返回时的堆栈地址和返回值及参数值等信息。
until 或 u
当厌倦了在一个循环体内单步跟踪时，这个命令可以运行程序直到退出循环体。
stepi 或 si
nexti 或 ni
单步跟踪一条机器指令。一条程序代码有可能由数条机器指令完成，stepi和nexti可以单步执行机器指令。与之一样有相同功能的命令是display/i $pc，当运行完这个命令后，单步跟踪会在显示出程序代码的同时显示出机器指令(也就是汇编代码)。
9. 信号(Signals)
信号是一种软中断，是一种处理异步事件的方法。
一般来说，操作系统都支持许多信号，尤其是Linux，比较重要的应用程序一般都会处理信号。Linux定义了许多信号，比如SIGINT表示中断字符信号，也就是Ctrl+C的信号，SIGBUS表示硬件故障的信号；SIGCHLD表示子进程状态改变信号；SIGKILL表示终止程序运行的信号等。信号量编程是UNIX下非常重要的一种技术。
gdb有能力在调试程序的时候处理任何一种信号。可以告诉gdb需要处理哪一种信号；可以要求gdb收到所指定的信号时，马上停住正在运行的程序，以供用户进行调试。可用gdb的handle命令来完成这一功能。
handle <signal> <keywords...>
在gdb中定义一个信号处理。信号<signal>可以以SIG开头或不以SIG开头，可以定义一个要处理信号的范围(如：SIGIO-SIGKILL，表示处理从SIGIO信号到SIGKILL的信号，其中包括SIGIO，SIGIOT，SIGKILL三个信号)，也可以使用关键字all来标明要处理所有的信号。一旦被调试的程序接收到信号，运行程序马上会被gdb停住，以供调试。其<keywords>可以是以下几种关键字中的一个或多个。
nostop
当被调试的程序收到信号时，gdb不会停住程序的运行，但会显示出消息告诉用户收到这种信号。
stop
当被调试的程序收到信号时，gdb会停住程序。
print
当被调试的程序收到信号时，gdb会显示出一条信息。
noprint
当被调试的程序收到信号时，gdb不会告诉用户收到信号的信息。
Pass
noignore
当被调试的 
引用:[http://blog.csdn.net/samehai/archive/2007/09/27/1803521.aspx](http://blog.csdn.net/samehai/archive/2007/09/27/1803521.aspx)
