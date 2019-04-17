# Sigar简介 - DoubleLi - 博客园






大家好，我是Sigar。也许好多人还不认识我。下面就介绍一下我自己，好让大家对我有一个大致的了解。


我的全名是System Information Gatherer And Reporter，中文名是系统信息收集和报表工具。我是一个开源的工具，提供了跨平台的系统信息收集的API，我可以收集的信息包括：

1.操作系统的信息，包括：dataModel、cpuEndian、name、version、arch、machine、description、patchLevel、vendor、vendorVersion、vendorName、vendorCodeName

2.CPU信息，包括：基本信息（vendor、model、mhz、cacheSize）和统计信息（user、sys、idle、nice、wait）

3.内存信息，物理内存和交换内存的总数、使用数、剩余数；RAM的大小

4.进程信息，包括每个进程的内存、CPU占用数、状态、参数、句柄等。

5.文件系统信息，包括名称、容量、剩余数、使用数、分区类型等

6.网络接口信息，包括基本信息和统计信息。

7.网络路由和链接表信息。

哈哈，够全了吧。另外我的性格比较外向，可以和绝大多数操作系统和大多数版本打交道。
当然了我的底层接口是用C语言写的。



好了，如果大家还想了较更多的信息可以去我的网站[http://sigar.hyperic.com/](http://sigar.hyperic.com/)



相关链接：[](http://sigar.hyperic.com/)



### [Sigar获取CPU的基本信息](http://blog.csdn.net/aoxida/article/details/8080538)

[Sigar获取内存的信息](http://blog.csdn.net/aoxida/article/details/8080623)

[Sigar获取磁盘信息](http://blog.csdn.net/aoxida/article/details/8080633)

[Sigar获取网卡流量信息](http://blog.csdn.net/aoxida/article/details/8080967)

[Sigar获取系统信息](http://blog.csdn.net/aoxida/article/details/8080978)



sigar所需要的文件 [下载地址](http://download.csdn.net/detail/aoxida/5040075)

![](https://blog.csdn.net/aoxida/article/details/8080492)





Sigar还可以结合[amcharts](http://blog.csdn.net/aoxida/article/details/8109335)插件在页面展现服务器相关信息，详细点击[amcharts _ 2.7.6 实现动态数据展现](http://blog.csdn.net/aoxida/article/details/8109335)











其他帖子介绍：





Hyperic-Sigar是一个收集系统各项底层信息的工具集.他有如下特点:
1. 收集信息全面
收集CPU,MEM,NETWORK,PROCESS,IOSTAT等
使用Sigar,你完全可以模仿出cpuinfo,meminfo,top,free,ifconfig,ipconfig,netstat,route,df,du,ps,ls等多种unix平台和windows平台的指令.
2.跨平台,支持多数平台
支持的平台包括:windows系列(32系列,IA64系列,AMD64系列),linux系列,freeBsd系列,HPUnix系列,Sun solaris/Sparc/Sparc64系列,macOs系列,AIX系列等

3.提供的API接口全面
sigar本身由C语言开发而成,提供了丰富的API接口,包括:JAVA,.NET,PERL,PHP,PYTHON,RUBY.Sigar.jar包是通过本地方法来调用操作系统API来获取系统相关数据。Windows操作系统下Sigar.jar依赖sigar-amd64-winnt.dll或sigar-x86-winnt.dll，linux操作系统下则依赖libsigar-amd64-linux.so或libsigar-x86-linux.so……而Sigar.jar还依赖于jug-asl-2.0.0.jar、log4j-1.2.14.jar、Junit.jar，

Hyperic-hq官方网站：[http://www.hyperic.com](http://www.hyperic.com/)

Sigar.jar下载地址：[http://sigar.hyperic.com](http://sigar.hyperic.com/)









