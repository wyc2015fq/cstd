# 观察Linux 的性能 - 我相信...... - CSDN博客





2014年09月16日 12:39:35[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1689







分析linux的性能，可以帮助我们解决应用的问题，调整系统架构，优化产品。先看看Linux自身的系统结构



![](https://img-blog.csdn.net/20140916123814928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)











系统调用以上是用户域，以下为内核域。




Linux上基本的性能监测工具如下：

1）top 或Htop，读取CPU，DRAM，Virtual Memory的信息，能够观察每个进程的状况，主要是CPU的使用情况

2）iostat 读取Disk，IO控制器，块设备， 观察Disk 的IO统计数据

3）mpstat 读取CPU 信息，观察那些活动的线程信息

4）vmstat 读取虚拟内存，调度器，系统调用及CPU，观察虚拟内存的统计信息，dstat类似只是屏幕输出多了点色彩

5） free 读取DRAM和虚拟内存，观察内存的使用情况







进一步，linux的性能监测工具还有：

1）sar 观察系统活动例如分页读写，块设备统计，运行的队列统计，同时涵盖了了vmstat，iostat等的功能

2）netstat 各种网络协议的统计

3）pidstat 观察进程故障

4）strace 跟踪系统调用及所占资源

5）tcpdump 监听网络数据包，并提供文件输出，共wireshark做进一步的分析

6）blktrace 统计块设备的IO事件

7）iotop 观察每个进程的IO使用状况

8）slabtop 观察内核中slab分配器的使用

9）sysctl 系统内核参数调优的工具

10）/proc 呵呵， 可以直接读源数据了




更强大的inux的性能监测工具就是我最爱的性能指示器perf ，它读取CPU，内存，IO总线，网络控制机等信息，可以观察一个可执行程序的性能指标，抽样采集IO，cpu的活动数据，用一下，感觉。。。



