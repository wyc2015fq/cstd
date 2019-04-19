# JVM运行时内存使用情况监控 - 零度的博客专栏 - CSDN博客
2017年10月30日 14:30:39[零度anngle](https://me.csdn.net/zmx729618)阅读数：2852
java 语言, 开发者不能直接控制程序运行内存, 对象的创建都是由类加载器一步步解析, 执行与生成与内存区域中的; 并且jvm有自己的垃圾回收器对内存区域管理, 回收; 但是我们已经可以通过一些工具来在程序运行时查看对应的jvm内存使用情况, 帮助更好的分析与优化我们的代码;
注: 查看系统里java进程信息
```
// 查看当前机器上所有运行的java进程名称与pid(进程编号)
jps -l 
// 显示指定的jvm进程所有的属性设置和配置参数
jinfo pid
```
1 . jmap : 内存占用情况查询 (位于”jdk_home/bin”目录下)
```
// 查询某个pid进程对应的应用程序内存占用情况
jmap -heap pid
// 示例
jmap -heap 5940
Attaching to process ID 5940, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.92-b14
using thread-local object allocation.
Parallel GC with 4 thread(s)
Heap Configuration:
   MinHeapFreeRatio         = 0
   MaxHeapFreeRatio         = 100
   MaxHeapSize              = 734003200 (700.0MB)
   NewSize                  = 44040192 (42.0MB)
   MaxNewSize               = 244318208 (233.0MB)
   OldSize                  = 88080384 (84.0MB)
   NewRatio                 = 2
   SurvivorRatio            = 8
   MetaspaceSize            = 21807104 (20.796875MB)
   CompressedClassSpaceSize = 1073741824 (1024.0MB)
   MaxMetaspaceSize         = 17592186044415 MB
   G1HeapRegionSize         = 0 (0.0MB)
Heap Usage:
PS Young Generation
Eden Space:
   capacity = 32505856 (31.0MB)
   used     = 13906760 (13.262519836425781MB)
   free     = 18599096 (17.73748016357422MB)
   42.782322052986395% used
From Space:
   capacity = 6291456 (6.0MB)
   used     = 294912 (0.28125MB)
   free     = 5996544 (5.71875MB)
   4.6875% used
To Space:
   capacity = 7340032 (7.0MB)
   used     = 0 (0.0MB)
   free     = 7340032 (7.0MB)
   0.0% used
PS Old Generation
   capacity = 41943040 (40.0MB)
   used     = 6127536 (5.8436737060546875MB)
   free     = 35815504 (34.15632629394531MB)
   14.609184265136719% used
8535 interned Strings occupying 710344 bytes.
```
2 . jstack : 进程所包含线程情况查询 (位于”jdk_home/bin”目录下)
```
// 命令帮助
jstach -h 
// 显示进程详情
jstack pid
// 示例
jstach 5940
Full thread dump Java HotSpot(TM) 64-Bit Server VM (25.92-b14 mixed mode):
"RMI TCP Connection(10)-10.2.13.162" #32 daemon prio=5 os_prio=0 tid=0x00000000179dc000 nid=0x1f60 in Object.wait() [0x000000001d7dd000]
   java.lang.Thread.State: TIMED_WAITING (on object monitor)
        at java.lang.Object.wait(Native Method)
        at com.sun.jmx.remote.internal.ArrayNotificationBuffer.fetchNotifications(ArrayNotificationBuffer.java:449)
        - locked <0x00000000d462ec18> (a com.sun.jmx.remote.internal.ArrayNotificationBuffer)
        at com.sun.jmx.remote.internal.ArrayNotificationBuffer$ShareBuffer.fetchNotifications(ArrayNotificationBuffer.java:227)
        at com.sun.jmx.remote.internal.ServerNotifForwarder.fetchNotifs(ServerNotifForwarder.java:274)
        at javax.management.remote.rmi.RMIConnectionImpl$4.run(RMIConnectionImpl.java:1270)
        at javax.management.remote.rmi.RMIConnectionImpl$4.run(RMIConnectionImpl.java:1268)
        at javax.management.remote.rmi.RMIConnectionImpl.fetchNotifications(RMIConnectionImpl.java:1274)
        at sun.reflect.GeneratedMethodAccessor59.invoke(Unknown Source)
        at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
        at java.lang.reflect.Method.invoke(Method.java:498)
        at sun.rmi.server.UnicastServerRef.dispatch(UnicastServerRef.java:324)
..............
"GC task thread#1 (ParallelGC)" os_prio=0 tid=0x0000000002d10000 nid=0x27dc runnable
"GC task thread#2 (ParallelGC)" os_prio=0 tid=0x0000000002d11800 nid=0x2d84 runnable
"GC task thread#3 (ParallelGC)" os_prio=0 tid=0x0000000002d13800 nid=0x118 runnable
"VM Periodic Task Thread" os_prio=2 tid=0x0000000015ccb000 nid=0x2fd4 waiting on condition
JNI global references: 239
```
3 . jstat: 可以实时监测系统资源占用与jvm运行情况 (位于”jdk_home/bin”目录下)
```
// 命令语法结构：
Usage: jstat -help|-options
       jstat -<option> [-t] [-h<lines>] <vmid> [<interval> [<count>]]
// 参数解释：
Options — 选项，我们一般使用 -gcutil 查看gc情况
vmid    — VM的进程号，即当前运行的java进程号
interval– 间隔时间，单位为秒或者毫秒
count   — 打印次数，如果缺省则打印无数次
S0  — Heap上的 Survivor space 0 区已使用空间的百分比
S1  — Heap上的 Survivor space 1 区已使用空间的百分比
E   — Heap上的 Eden space 区已使用空间的百分比
O   — Heap上的 Old space 区已使用空间的百分比
P   — Perm space 区已使用空间的百分比
YGC — 从应用程序启动到采样时发生 Young GC 的次数
YGCT– 从应用程序启动到采样时 Young GC 所用的时间(单位秒)
FGC — 从应用程序启动到采样时发生 Full GC 的次数
FGCT– 从应用程序启动到采样时 Full GC 所用的时间(单位秒)
GCT — 从应用程序启动到采样时用于垃圾回收的总时间(单位秒)
//示例
jstat -options
    -class
    -compiler
    -gc
    -gccapacity
    -gccause
    -gcmetacapacity
    -gcnew
    -gcnewcapacity
    -gcold
    -gcoldcapacity
    -gcutil
    -printcompilation
jstat -class -t 5940
Timestamp Loaded  Bytes  Unloaded  Bytes     Time
6188.4   3898  7178.4       40    58.3       1.78
jstat -gcutil 5940 1000 5
S0     S1     E      O      M     CCS    YGC     YGCT    FGC    FGCT     GCT
0.00  25.00  98.55  15.37  96.94  94.88     21    0.069     7    0.237    0.306
0.00  25.00  99.59  15.37  96.94  94.88     21    0.069     7    0.237    0.306
0.00  25.00  99.59  15.37  96.94  94.88     21    0.069     7    0.237    0.306
0.00  25.00 100.00  15.37  96.94  94.88     21    0.069     7    0.237    0.306
0.00  25.00 100.00  15.37  96.94  94.88     21    0.069     7    0.237    0.306
```
4 . jconsole 以GUI的方式更直观化呈现jvm进程的实时情况, 比如内存占用, 线程执行情况等; 
在jdk_home/bin目录下执行 jconsole.exe 打开图形化界面, 然后选择要检查的进程就可以查看所有相关jvm情况的信息了.
![概览](https://img-blog.csdn.net/20170110105226749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGluc3RlbnlIaXNva2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
5 . jprofiler 一个付费的商业jvm分析监控工具, 可查看概况, 内存, GC活动, class 状况, 线程信息, CPU 占用情况, 内存对象信息, 还有数据库连接等, 是一个非常优秀的分析工具;
![概览](https://img-blog.csdn.net/20170110111419756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGluc3RlbnlIaXNva2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
