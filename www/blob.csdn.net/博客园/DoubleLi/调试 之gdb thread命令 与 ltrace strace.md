# 调试 之gdb thread命令 与 ltrace/strace - DoubleLi - 博客园






我们可以通过  1)  gdb prog_name -> r               用在逐步调试自己的程序时

                    2)  gdb -> attach process_id       正在运行中的后台程序突然卡在了某个地方，先ps再gdb/attach

                    3)  gdb prog_name core              程序core掉了

三种方式对一个程序进行调试；



1. thread——gdb 多线程调试命令:



     info threads:           显示当前进程中的线程；

     thread thread_no:  进入线程xx，通常紧接而来的是 bt/f 命令；



2. strace/ltrace:

     前者关注系统调用和程序所接收的信号；后者关注库函数调用；

     strace的应用在 我们没有程序的源码，或者不方便从头开始运行程序时；可以方便查看一个应用程序进行了哪些系统调用。

     而在希望知道程序都调用了动态库中的哪些函数时，我们使用 ltrace。ltrace有个-S选项，类似于strace功能。



3.检查内存泄漏的工具：

   valgrind (in linux, free) 
   visual leak detector (windows , free)
   boundschecker(windows, free)

   profile工具：
   oprofile
   vtune









