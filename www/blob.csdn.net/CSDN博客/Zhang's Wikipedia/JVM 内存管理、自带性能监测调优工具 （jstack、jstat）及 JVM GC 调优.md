
# JVM 内存管理、自带性能监测调优工具 （jstack、jstat）及 JVM GC 调优 - Zhang's Wikipedia - CSDN博客


2018年07月15日 18:52:57[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1221



## 0. 内存查看
获取可用内存大小：
public static long getAvaliableMemory()
{
    Runtime runtime = Runtime.getRuntime();
    return runtime.totalMemory() - runtime.freeMemory();
}totalMemory()这个方法返回的是java虚拟机现在已经从操纵系统那里挖过来的内存大小maxMemory()这个方法返回的是java虚拟机（这个进程）能构从操纵系统那里挖到的最大的内存，以字节为单位，假如在运行java程 序的时 候，没有添加-Xmx参数，那么就是64兆，也就是说maxMemory()返回的大约是6410241024字节
-Xmx参数，将以这个参数后面的值为准
在java程序运行的过程的，内存总是慢慢的 从操 作系统那里挖的，基本上是用多少挖多少，但是java虚拟机100％的情况下是会稍微多挖一点的，这些挖过来而又没有用上的内存，实际上就是 freeMemory()

## 1. jstack：占用最多资源（CPU 内存）的Java代码
[https://www.cnblogs.com/chengJAVA/p/5821218.html](https://www.cnblogs.com/chengJAVA/p/5821218.html)
[https://blog.csdn.net/u012448083/article/details/76855340](https://blog.csdn.net/u012448083/article/details/76855340)
jstack可以定位到线程堆栈，根据堆栈信息我们可以定位到具体代码（代码行号）。
第一步，先找到 java进程ID
ps -ef | grep java | grep -v grep：grep java 能够搜索到 java -jar xxx.jar

根据进程ID，找到该进程内最耗费CPU的线程：
top -Hp pid
将十进制的线程 ID，转换为十六进制的线程 ID
printf(’%x\n’, thread_id)
使用 jstack 查看该线程 ID 的堆栈信息：
jstack pid | grep thread_id当然还可以输出更为详尽的信息：jstack pid | grep -A 10 thread_id

## 2. JVM GC 堆
[https://blog.csdn.net/ls5718/article/details/51777195](https://blog.csdn.net/ls5718/article/details/51777195)
Java 中的堆是 JVM 所管理的最大的一块内存空间，主要用于存放各种类的实例对象。
在 Java 中，堆被划分成两个不同的区域：新生代 ( Young )、老年代 ( Old )。新生代 ( Young ) 又被划分为三个区域：Eden、From Survivor、To Survivor。
这样划分的目的是为了使 JVM 能够更好的管理堆内存中的对象，包括内存的分配以及回收。
![这里写图片描述](https://img-blog.csdn.net/2018071518493737?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)堆大小 = 新生代 + 老年代。
堆的大小可以通过参数 –Xms、-Xmx 来指定。
JVM 每次只会使用 Eden 和其中的一块 Survivor （from survivor/to survivor）区域来为对象服务，所以无论什么时候，总是有一块 Survivor 区域是空闲着的。因此，新生代实际可用的内存空间为 9/10 ( 即90% )的新生代空间。

