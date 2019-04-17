# Linux下快速查看CPU使用情况的相关命令 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月31日 11:31:33[boonya](https://me.csdn.net/boonya)阅读数：14246








Linux下快速查看CPU使用情况比较常用的命令是free、top、ps。这篇文章来看下如何在Linux下检查服务器的CPU使用情况。我的Linux是Linux-Ubuntu-server-15.04，如果是图形界面，有些统计会看起来更直观。


## 1.free

free命令是一个快速查看内存使用情况的方法，它是对 /proc/meminfo 收集到的信息的一个概述。

![](https://img-blog.csdn.net/20170331111208058)

## 2.top

top命令提供了实时的运行中的程序的资源使用统计。你可以根据内存的使用和大小来进行排序。

![](https://img-blog.csdn.net/20170331111521403)


## 3.atop

atop命令是一个终端环境的监控命令。它显示的是各种系统资源（CPU, memory, network, I/O, kernel）的综合，并且在高负载的情况下进行了彩色标注。

![](https://img-blog.csdn.net/20170331111350887)


## 4.htop

htop命令显示了每个进程的内存实时使用率。它提供了所有进程的常驻内存大小、程序总内存大小、共享库大小等的报告。列表可以水平及垂直滚动。

![](https://img-blog.csdn.net/20170331111444169)

## 5.ps

ps命令可以实时的显示各个进程的内存使用情况。Reported memory usage information includes %MEM (percent of physical memory used), VSZ (total amount of virtual memory used), and RSS (total amount of physical memory used)。你可以使用 “–sort”选项对进程进行排序，例如按RSS进行排序：ps aux --sort -rss.

![](https://img-blog.csdn.net/20170331112127780)


## 6.smem

smem命令允许你统计基于/proc信息的不同进程和用户的内存使用情况。内存使用情况的分析可以导出图表（如条形图和饼图--图形界面窗口才提供）。

![](https://img-blog.csdn.net/20170331112302437)

## 7.vmstat

vmstat命令显示实时的和平均的统计，覆盖CPU、内存、I/O等内容。例如内存情况，不仅显示物理内存，也统计虚拟内存。

![](https://img-blog.csdn.net/20170331112352875)

## 8.nmon

nmon是一个基于ncurses的系统基准测试工具，它可以监控CPU、内存、I/O、文件系统及网络资源等的互动模式。对于内存的使用，它可以实时的显示 总/剩余内存、交换空间等信息。


![](https://img-blog.csdn.net/20170331112634657)

## 9.memstat

memstat是一个有效识别executable(s), process(es) and shared libraries使用虚拟内存情况的命令。给定一个进程ID，memstat可以列出这个进程相关的可执行文件、数据和共享库。


![](https://img-blog.csdn.net/20170331112810219)

## 10.图形化支持GNOME System Monitor和KDE System Monitor



GNOME System Monitor 是一个显示最近一段时间内的CPU、内存、交换区及网络的使用情况的视图工具。它还提供了一种查看CPU及内存使用情况的方法。
|1|$ gnome-system-monitor|
|----|----|

![gnome-system-monitor](http://img.kuqin.com/upimg/allimg/150110/1102296409-3.jpg)

**KDE System Monitor**


$ ksysguard





![$ ksysguard](http://img.kuqin.com/upimg/allimg/150110/1102295B3-5.jpg)





参考地址：[http://www.kuqin.com/shuoit/20150110/344314.html](http://www.kuqin.com/shuoit/20150110/344314.html)




