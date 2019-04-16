# JVM内存泄漏检测与处理 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年08月16日 17:44:55[gloomyfish](https://me.csdn.net/jia20003)阅读数：8459








JVM内存泄漏检测与处理（JVM Memory Leak detection and handling）



**JVM垃圾回收机制的原则和方法**

JVM垃圾回收中一个基本原则是对象没有被引用或则引用其它对象，换句话说当一个对象在heap

中是隔离(isolation)状态的时候，垃圾回收器就会自动回收分配给这个对象的堆内存空间。区分

活动对象的两个基本方法是引用计数器(reference counter)和跟踪(trace), JVM中垃圾回收器（GC）

的种类主要有以下：

Ø        吞吐量优先收集器(Throughput Collector) 一个并行版本的短时(YoungGeneration)收

        集器，使用参数为：-XX:+UseParallelGC

Ø        并发低中断收集器(Concurrent Low Pause Collector),在垃圾回收过程中应用程序将被

        短时挂起，使用参数为：-XX:UseConcMarkSweepGC

Ø        增量低中断收集器(Incremental Low Pause Collector)，使用参数为：-Xincgc

Ø        此外，JDK 1.4以后支持自适应的垃圾回收器



怎么观察垃圾回收器的工作和详细信息，使用如下两个JVM参数即可：

`-verbose:gc`

`-XX:+PrintGCDetails`

另外一个很有用的JVM参数选项为： -Xprof打印CPU Profiling开销时间数据



**JVM内存泄漏特征：**

1.      程序在不断重复某个操作时，内存一直处于动态的增长之中，并且到达JVM声明的最大内

         存而不释放任何heap内存。

2.      某个流程已经处理完成，但是过程中增加的临时内存在介绍后却没有被垃圾回收

3.      应用程序运行一段时间以后就得到OOM(Out of Memory)异常




**一个典型JVM内存活动图如下：呈现锯齿状**

![](http://hi.csdn.net/attachment/201108/16/0_1313487860y7Pn.gif)


**一个内存泄漏的JVM内存活动图如下：呈现阶梯状上升**

![](http://hi.csdn.net/attachment/201108/16/0_1313487880fX3Z.gif)

**怎么避免JVM内存泄漏：**

1.      对象使用完以后记得将对象引用置为null， object_instance = null;

2.      退出或者关闭UI对象时，要remove注册的listeners

3.      对与swing的图形设备对象（Graphics）使用以后调用dispose() 方法释放资源

4.      对于HashMap对象要调用clearAll()方法在设置为null之前

5.      对于树形结构的数据对象， 一个通常的做法是定义一个通用的cleanup()/release()接口。

然后逐级调用cleanup()方法来完成对象引用的销毁（设置为Null）。

6.      避免创建重复信息的Data Model或者多个本地copy

7.      文件I/O与Hibernate的session用完以后要及时关闭释放资源

8.      对于socket的stream buffer空间，数据库连接要及时关闭

9.      回收和处理JNI分配的内存。



**关于JVM垃圾回收的认知误区：**

1.      当内存使用太多的时候，可以显式的调用System.gc()方法

答案是显式调用并不能解决此类问题，反而会因为反复调用System.gc()导致JVM性能问题，

更进一步的原因在于，即使显式调用JVM并不保证垃圾回收器立即运行。

推荐：无论什么时候，永远不要使用System.gc()来显式调用JVM垃圾回收器



2.      当对象不再被使用的时候，垃圾回收器就会自动回收它们

垃圾回收只会回收那些引用计数为零的对象instance，当一个对象的引用计数不为零或者

处于可以追溯状态是，垃圾回收器永远都不会回收这些堆内存，常见JVM内存泄漏都是由

这个原因引起的。



**什么才是正确的处理方法：**

1.      定义一个接口类 - MemHandler

2.      实现并完成cleanup()方法在你自己定义的对象classes中

3.      当应用程序不再使用该instance之前调用cleanup() 方法



参考引用：



[http://www.oracle.com/technetwork/java/gc-tuning-5-138395.html](http://www.oracle.com/technetwork/java/gc-tuning-5-138395.html)

[http://java.sun.com/docs/books/performance/1st_edition/html/JPAppGC.fm.html](http://java.sun.com/docs/books/performance/1st_edition/html/JPAppGC.fm.html)




