# Linux 多线程调试（内存占用、死循环、CPU占用率高……） - DoubleLi - 博客园






文章出处：http://www.cnblogs.com/cy568searchx/archive/2013/10/28/3391790.html





你的软件在某个时刻停止服务，CPU占用达到100%+，这种问题一个可能的原因是产生了死循环，假设程序某处存在潜在的死循环，并在某种条件下会引发，本文以一个示例来定位出现死循环的位置。
当程序某处存在死循环，通常定位问题及缩小范围的方法是，在可疑的代码处加log，或者注释掉可疑代码，这对于容易重现问题的程序来说还好，但对于“偶尔”才会产生问题程序却很难调试，因为我们很难重现程序故障。本文所述的调试过程正是在这种情况下，假设问题已经出现，我们要求环境保护现场，即出问题的程序还在运行中。

1.我们首先要知道是哪个线程出了问题：
首先查一下出问题进程的pid，例如

[ovtsvn@ovtsvn:](mailto:ovtsvn@ovtsvn:~/MASS4/src/icdn/src$)[~/MASS4/src/icdn/src$](mailto:ovtsvn@ovtsvn:~/MASS4/src/icdn/src$) ps -ef | grep icdn ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

ovtsvn   11065     1 50 11:57 ?        00:00:07 ./icdn ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

ovtsvn   11076 10971  0 11:57 pts/2    00:00:00 grep

ovtsvn@ovtsvn:~/MASS4/src/icdn/src$


ovtsvn@ovtsvn:~/MASS4/src/icdn/src$ 

然后top命令查看线程信息：
top -H -p 11065

PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND                                                                 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

11073 ovtsvn    25   0  325m 3980 2236 R  100  0.4   1:40.84 icdn                                                                    ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

11065 ovtsvn    18   0  325m 3980 2236 S    0  0.4   0:00.01 icdn                                                                    ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

11066 ovtsvn    18   0  325m 3980 2236 S    0  0.4   0:00.00 icdn                                                                    ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

11067 ovtsvn    15   0  325m 3980 2236 S    0  0.4   0:00.00 icdn                                                                    ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

11068 ovtsvn    15   0  325m 3980 2236 S    0  0.4   0:00.00 icdn                                                                    ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
11069 ovtsvn 180 325m 39802236 S 00.40:00.00 icdn ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

11070 ovtsvn 180 325m 39802236 S 00.40:00.00 icdn ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

11071 ovtsvn 220 325m 39802236 S 00.40:00.00 icdn ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

11072 ovtsvn 150 325m 39802236 R 00.40:00.00 icdn




从上面可以看出，出问题线程PID为11073

2.接下来，我们用gdb来attach目标进程
执行: gdb icdn 11065
在gdb中，列出线程状态：

![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)(gdb) info threads ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

9 Thread 47056948181264 (LWP 11066)  0x00002acc4a3dec91 in nanosleep () from /lib/libc.so.6 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

8 Thread 47056956573968 (LWP 11067)  0x00002acc4a406fc2 in select () from /lib/libc.so.6 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

7 Thread 47056964966672 (LWP 11068)  0x00002acc4a3dec91 in nanosleep () from /lib/libc.so.6 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

 6 Thread 47056973359376 (LWP 11069)  0x00002acc4a3dec91 in nanosleep () from /lib/libc.so.6 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

5 Thread 47056981752080 (LWP 11070)  0x00002acc4a3dec91 in nanosleep () from /lib/libc.so.6 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

4 Thread 47056990144784 (LWP 11071)  0x00002acc4a40e63c in recvfrom () from /lib/libc.so.6 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

3 Thread 47057194060048 (LWP 11072)  0x00002acc4a406fc2 in select () from /lib/libc.so.6 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

2 Thread 47057226893584 (LWP 11073)  CSendFile::SendFile (this=0x2acc5d4aff40, pathname=@0x2acc5d4afee0) ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    at ../src/csendfile.cpp:101 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

1 Thread 47056939784832 (LWP 11065)  0x00002acc4a3dec91 in nanosleep () from /lib/libc.so.6 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)(gdb) 


gdb已经列出了各线程正在执行的函数，我们需要更多信息，记住11073对应的行首标号，这是gdb为线程分配的id，这里为2，然后执行切换：

(gdb) thread 2 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

[Switching to thread 2 (Thread 47057226893584 (LWP 11073))]#0  CSendFile::SendFile (this=0x2acc5d4aff40, pathname=@0x2acc5d4afee0) ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    at ../src/csendfile.cpp:101 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)101             while(1) ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

(gdb) 

bt一下：

![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)(gdb) bt ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

#0  CSendFile::SendFile (this=0x2acc5d4aff40, pathname=@0x2acc5d4afee0) at ../src/csendfile.cpp:101 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

#1  0x000000000040592e in CIcdn::TaskThread (pParam=0x7fff617eafe0) at ../src/cicdn.cpp:128 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

#2  0x00002acc4a90b73a in start_thread () from /lib/libpthread.so.0 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

#3  0x00002acc4a40d6dd in clone () from /lib/libc.so.6 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

#4  0x0000000000000000 in ?? ()


来看一下101行的代码：

![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)(gdb) l ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

96      } ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

97 ![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)

98      int CSendFile::SendFile(const string& pathname) ![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif)

99      {

100             int n;

101             while(1)

102             {

103                     n++;

104             }

105             //read file and send 

现在我们定位到了出问题的代码位置，这里的循环只用来演示的。 
最后别忘了detach（）

调试完指定进程后，可以运行detach命令来让GDB释放该进程，该进程得以继续运行。当回车时，detach不会重复。当执行完detach后，进程和GDB不再相关，GDB可以attach其他进程。









