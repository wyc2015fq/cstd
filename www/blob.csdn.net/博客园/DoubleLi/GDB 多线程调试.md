# GDB 多线程调试 - DoubleLi - 博客园






线程有自己的寄存器，运行时堆栈或许还会有私有内存。 
gdb提供了以下供调试多线程的进程的功能： 
* 自动通告新线程。 
* \ "thread THREADNO\ "，一个用来在线程之间切换的命令。 
* \ "info threads\ "，一个用来查询现存线程的命令。 
* \ "thread apply [THREADNO] [ALL] ARGS\ ",一个用来向线程提供命令的命令。 
* 线程有关的断点设置。 
注意：这些特性不是在所有gdb版本都能使用，归根结底要看操作系统是否支持。 
如果你的gdb不支持这些命令，会显示出错信息： 
    (gdb) info threads 
    (gdb) thread 1 
    Thread ID 1 not known. Use the \ "info threads\ " command to 
    see the IDs of currently known threads. 
gdb的线程级调试功能允许你观察你程序运行中所有的线程，但无论什么时候 
gdb控制，总有一个“当前”线程。调试命令对“当前”进程起作用。 
一旦gdb发现了你程序中的一个新的线程，它会自动显示有关此线程的系统信 
息。比如： 
    [New process 35 thread 27] 
不过格式和操作系统有关。 
为了调试的目的，gdb自己设置线程号。 
`info threads\ " 
显示进程中所有的线程的概要信息。gdb按顺序显示： 
1.线程号(gdb设置) 
2.目标系统的线程标识。 
3.此线程的当前堆栈。 
一前面打\ "*\ "的线程表示是当前线程。 
例如： 
(gdb) info threads 
3 process 35 thread 27 0x34e5 in sigpause () 
2 process 35 thread 23 0x34e5 in sigpause () 
* 1 process 35 thread 13 main (argc=1, argv=0x7ffffff8) 
at threadtest.c:68 
`thread THREADNO\ " 
把线程号为THREADNO的线程设为当前线程。命令行参数THREADNO是gdb内定的 
线程号。你可以用\ "info threads\ "命令来查看gdb内设置的线程号。gdb显示该线程 
的系统定义的标识号和线程对应的堆栈。比如： 
(gdb) thread 2 
[Switching to process 35 thread 23] 
0x34e5 in sigpause () 
\ "Switching后的内容取决于你的操作系统对线程标识的定义。 
`thread apply [THREADNO] [ALL] ARGS\ " 
此命令让你对一个以上的线程发出相同的命令\ "ARGS\ ",[THREADNO]的含义同上。 
如果你要向你进程中的所有的线程发出命令使用[ALL]选项。 
无论gdb何时中断了你的程序(因为一个断点或是一个信号)，它自动选择信号或 
断点发生的线程为当前线程。gdb将用一个格式为\ "[Switching to SYSTAG]\ "的消息 
来向你报告。

对于多线程程序,你可以定义你的断点是否在所有的线程上，或是在某个特定的线程。GDB很容易帮你完成这一工作。 
break <linespec> thread <threadno> 
break <linespec> thread <threadno> if ... 
linespec指定了断点设置在的源程序的行号。threadno指定了线程的ID，注意，这个ID是GDB分配的，你可以通过“info threads”命令来查看正在运行程序中的线程信息。如果你不指定thread <threadno>则表示你的断点设在所有线程上面。你还可以为某线程指定断点条件。如： 
(gdb) break frik.c:13 thread 28 if bartab > lim 
当你的程序被GDB停住时，所有的运行线程都会被停住。这方便你你查看运行程序的总体情况。而在你恢复程序运行时，所有的线程也会被恢复运行。那怕是主进程在被单步调试时。









