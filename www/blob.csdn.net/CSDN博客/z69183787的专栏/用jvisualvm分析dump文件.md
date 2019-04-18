# 用jvisualvm分析dump文件 - z69183787的专栏 - CSDN博客
2018年06月26日 10:30:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1995
[https://blog.csdn.net/lkforce/article/details/60878295](https://blog.csdn.net/lkforce/article/details/60878295)
最近有一个java服务的堆内存溢出，然后僵死了，在重启服务之前用jmap命令生成了一份dump文件便于后面分析。
生成dump文件的命令：
jmap -dump:format=b,file=20170307.dump 16048
file后面的是自定义的文件名，最后的数字是进程的pid。
使用jvisualvm来分析dump文件：
jvisualvm是JDK自带的Java性能分析工具，在JDK的bin目录下，文件名就叫jvisualvm.exe。
jvisualvm可以监控本地、远程的java进程，实时查看进程的cpu、堆、线程等参数，对java进程生成dump文件，并对dump文件进行分析。
像我这种从服务器上dump下来文件也可以直接扔给jvisualvm来分析。
使用方式：直接双击打开jvisualvm.exe，点击文件->装入，在文件类型那一栏选择堆，选择要分析的dump文件，打开。
![](https://img-blog.csdn.net/20170308184242660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGtmb3JjZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170308184030048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGtmb3JjZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
装入之后在界面右侧的概要、类等选项卡可以看到生成dump文件当时的堆信息：
![](https://img-blog.csdn.net/20170308184430445?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGtmb3JjZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170308184739884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGtmb3JjZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到，dump文件里记录的堆中的实例，总大小大概300M左右，（用第一行的实例大小除以百分比就能算出来），和JVM默认的新生代的大小差不多，远小于JVM中设置的最大堆内存，也就是说，dump文件里记录的并不是实例大小达到最大堆内存时的状态。
为了验证一下，我自己在本地模拟了一下堆内存溢出的情形，并用jvisualvm监控
我本地设置的最大堆内存是800M左右，代码里面是往一个List里面疯狂加数据，直到撑爆堆内存，得到的监控内容是这样的：
![](https://img-blog.csdn.net/20170308185634788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGtmb3JjZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
分析：红框框出的部分是发生堆内存溢出时的情形，已使用的堆大小（蓝色部分）并没有增长特别明显，但是申请的堆的大小（黄色部分）从默认的400多兆急速上涨，涨到800M，然后内存溢出，然而使用的堆大小依然没怎么增长。
所以，dump文件中的实例列表其实是反映了使用的堆的情况，而使用的堆内存并没有达到预先设置的最大堆内存，只是在申请堆内存的过程中超出了预先设置的最大堆内存，然后内存溢出。
最后，jvisualvm确实挺好用的。
