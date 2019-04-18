# gdb多线程调试 - nosmatch的专栏 - CSDN博客
2012年01月09日 10:50:39[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：722
先介绍一下GDB多线程调试的基本命令。 **info threads** 显示当前可调试的所有线程，每个线程会有一个GDB为其分配的ID，后面操作线程的时候会用到这个ID。 前面有*的是当前调试的线程。
**thread ID** 切换当前调试的线程为指定ID的线程。
**break thread_test.c:123 thread all**在所有线程中相应的行上设置断点**thread apply ID1 ID2 command**让一个或者多个线程执行GDB命令command。 **thread apply all command** 让所有被调试线程执行GDB命令command。 **set scheduler-locking off|on|step** 估计是实际使用过多线程调试的人都可以发现，在使用step或者continue命令调试当前被调试线程的时候，其他线程也是同时执行的，怎么只让被调试程序执行呢？通过这个命令就可以实现这个需求。off 不锁定任何线程，也就是所有线程都执行，这是默认值。 on 只有当前被调试程序会执行。 step 在单步的时候，除了next过一个函数的情况(熟悉情况的人可能知道，这其实是一个设置断点然后continue的行为)以外，只有当前线程会执行。
gdb对于多线程程序的调试有如下的支持：
- 线程产生通知：在产生新的线程时, gdb会给出提示信息
(gdb) r
Starting program: /root/thread 
[New Thread 1073951360 (LWP 12900)] 
[New Thread 1082342592 (LWP 12907)]---以下三个为新产生的线程
[New Thread 1090731072 (LWP 12908)]
[New Thread 1099119552 (LWP 12909)]
- 查看线程：使用可以查看运行的线程。info threads
(gdb) info threads
4 Thread 1099119552 (LWP
12940)   0xffffe002 in ?? ()
3 Thread 1090731072 (LWP 12939)   0xffffe002 in ?? ()
2 Thread 1082342592 (LWP 12938)   0xffffe002 in ?? ()
*
1 Thread 1073951360 (LWP 12931)   main (argc=1, argv=0xbfffda04) at thread.c:21
(gdb)
注意，行首的蓝色文字为gdb分配的线程号，对线程进行切换时，使用该该号码，而不是上文标出的绿色数字。
另外，行首的红色星号标识了当前活动的线程
- 切换线程：使用 thread THREADNUMBER
进行切换，THREADNUMBER
为上文提到的线程号。下例显示将活动线程从 1 切换至 4。
(gdb) info threads
   4 Thread 1099119552 (LWP 12940)   0xffffe002 in ?? ()
   3 Thread 1090731072 (LWP 12939)   0xffffe002 in ?? ()
   2 Thread 1082342592 (LWP 12938)   0xffffe002 in ?? ()
* 1 Thread 1073951360 (LWP 12931)   main (argc=1, argv=0xbfffda04) at thread.c:21
(gdb) thread 4
[Switching to thread 4 (Thread 1099119552 (LWP 12940))]#0   0xffffe002 in ?? ()
(gdb) info threads
* 4 Thread 1099119552 (LWP 12940)   0xffffe002 in ?? ()
   3 Thread 1090731072 (LWP 12939)   0xffffe002 in ?? ()
   2 Thread 1082342592 (LWP 12938)   0xffffe002 in ?? ()
   1 Thread 1073951360 (LWP 12931)   main (argc=1, argv=0xbfffda04) at thread.c:21
(gdb)
      以上即为使用gdb提供的对多线程进行调试的一些基本命令。另外，gdb也提供对线程的断点设置以及对指定或所有线程发布命令的命令。
      初次接触gdb下多线程的调试，往往会忽视gdb中活动线程的概念。一般来讲，在使用gdb调试的时候，只有一个线程为活动线程，如果希望得到其他的线程的输出结果，必须使用thread命令切换至指定的线程，才能对该线程进行调试或观察输出结果。
最后介绍一下我最近遇见的一个多线程调试和解决。 
基本问题是在一个Linux环境中，调试多线程程序不正常，info threads看不到多线程的信息。
我先用命令maintenance print target-stack看了一下target的装载情况，发现target"multi-thread"没有被装载，用GDB对GDB进行调试，发现在 函数check_for_thread_db在调用libthread_db中的函数td_ta_new的时候，返回了TD_NOLIBTHREAD，所 以没有装载target"multi-thread"。
在时候我就怀疑是不是libpthread有问题，于是检查了一下发现了问题，这个环境中的libpthread是被strip过的，我想可能 就是以为这个影响了td_ta_new对libpthread符号信息的获取。当我换了一个没有strip过的libpthread的时候，问题果然解决 了。
最终我的解决办法是拷贝了一个.debug版本的libpthread到lib目录中，问题解决了。 
多线程如果dump，多为段错误，一般都涉及内存非法读写。可以这样处理，使用下面的命令打开系统开关，让其可以在死掉的时候生成core文件。
ulimit -c unlimited
这样的话死掉的时候就可以在当前目录看到core.pid(pid为进程号)的文件。接着使用gdb:
gdb ./bin ./core.pid 
进去后，使用bt查看死掉时栈的情况，在使用frame命令。
还有就是里面某个线程停住，也没死，这种情况一般就是死锁或者涉及消息接受的超时问题(听人说的，没有遇到过)。遇到这种情况，可以使用：
gcore pid （调试进程的pid号）
手动生成core文件，在使用pstack(linux下好像不好使)查看堆栈的情况。如果都看不出来，就仔细查看代码，看看是不是在if，return，break，continue这种语句操作是忘记解锁，还有嵌套锁的问题，都需要分析清楚了。
原文地址 http://www.linuxforum.net/forum/gshowflat.php?Cat=&Board=program&Number=692404&page=0&view=collapsed
