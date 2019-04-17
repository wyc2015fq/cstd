# 深入分析java web技术内幕----读书笔记（四） - 程序员江湖 - CSDN博客





2017年03月18日 12:59:45[黄小斜](https://me.csdn.net/a724888)阅读数：560标签：[nio																[jvm																[java																[GC																[java内存管理](https://so.csdn.net/so/search/s.do?q=java内存管理&t=blog)
个人分类：[读书笔记](https://blog.csdn.net/a724888/article/category/6784971)





JVM内存管理




1物理内存和虚拟内存。
物理内存不足时，会使用虚拟内存，让线程共享虚拟内存（实际上还是逻辑独立的），只是共享空间。




2内核空间和用户空间

内核空间主要用于程序调度，虚拟内存使用、连接硬件等逻辑。

用户程序不允许直接访问硬件资源，而需要调用操作系统的接口来实现。

每次系统调用都需要切换两个内存空间。保证安全但是效率较低。有一些程序可以减少这样的的数据复制方式，如linux的sendfile。




3java 内存分配




3-1java堆，-XMx与-Xms控制大小，分配后大小不会再改变，由jvm控制其空间，垃圾收集器会管理堆内存。




3-2线程：线程私有的虚拟机栈、程序计数器（用来标记线程）、本地方法栈。

线程共享的有方法区（包括运行时常量池）、堆。

另外还有一部分是直接内存




3-3类和类加载器存储在堆中，这个区成为永久代，hotspot虚拟机采用分代收集的gc算法。jvm按需加载类，只加载必要的类以及你程序中用到的类。

![](https://img-blog.csdn.net/20170320184458709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


3-4 NIO：NIO使用bytebuffer。allocatedirect（）方法可以直接分配本机内存而非java堆内存。

NIO的交互可以都在内核空间完成，效率高得多，但是数据比较多的时候。




3-5 bytebuffer对象会自动清理缓存，但是它是java堆gc的一部分内容，它本身不会知道java堆上的压力。




gc尽在java堆填满时产生，所以需要调用system。gc显示释放nio的内存。




-XX:+DisableExplicitGC，这个参数作用是禁止代码中显示调用GC。代码如何显示调用GC呢，通过System.gc()函数调用。如果加上了这个JVM启动参数，那么代码中调用System.gc()没有任何效果




如果这样设置会导致nio的直接内存出现内存泄漏的问题（system。gc无法使用）




3-6JNI 使




得本机代码（如c）可以调用java方法，就是指native memory。










3-7垃圾回收：

分代垃圾算法

![](https://img-blog.csdn.net/20170320184502467?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20170320184557820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注意：perm区的垃圾回收也是由full gc触发的。

fullgc在老年代满或者system。gc时触发。




hotspot的三类回收算法：




serial collector 是JVM client默认方式，是单线程的，会停止所有进程。




paraller collector（内部有三种略有不同）是JVM server的默认方式，是多线程的，也会暂停应用程序




cms collector 是折中的一种方式，触发规则是检测perm或old区的使用率，到达一定比例会触发。它只回收old区和perm区的垃圾。他也是多线程的。





文章为原创，请尊重作者劳动成果。

转载请注明文章地址：http://blog.csdn.net/a724888/article/details/http://blog.csdn.net/a724888/article/details/63253036](https://so.csdn.net/so/search/s.do?q=GC&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=jvm&t=blog)](https://so.csdn.net/so/search/s.do?q=nio&t=blog)




