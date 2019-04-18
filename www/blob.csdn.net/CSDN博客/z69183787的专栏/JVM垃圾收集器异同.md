# JVM垃圾收集器异同 - z69183787的专栏 - CSDN博客
2017年04月17日 10:56:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：355
个人分类：[Jvm-Gc&垃圾回收](https://blog.csdn.net/z69183787/article/category/2917447)
JVM 垃圾收集器有3类，这里主要介绍我们常用的 并行和并发收集器：
**The Throughput Collector** （也叫并行收集器）
串行收集器在GC时会停止其他所有工作线程（stop-the-world），CPU利用率是最高的，所以适用于要求高吞吐量（throughput）的应用，但停顿时间（pause time）会比较长，所以对web应用来说就不适合，因为这意味着用户等待时间会加长。而并行收集器可以理解是多线程串行收集，在串行收集基础上采用多线程方式进行GC，很好的弥补了串行收集的不足，可以大幅缩短停顿时间（如下图表示的停顿时长高度，并发比并行要短），因此对于空间不大的区域（如young
 generation），采用并行收集器停顿时间很短，回收效率高，适合高频率执行。
![](http://hi.csdn.net/attachment/201109/29/0_1317310991zK18.gif)
**The Concurrent Low Pause Collector**（也叫并发收集器）
并发收集器GC时GC线程和应用线程大部分时间是并发执行，只是在初始标记（initial mark）和二次标记（remark）时需要stop-the-world，这可以大大缩短停顿时间（pause time），所以适用于响应时间优先的应用，减少用户等待时间。由于GC是和应用线程并发执行，只有在多CPU场景下才能发挥其价值，在执行过程中还会产生新的垃圾floating garbage，如果等空间满了再开始GC，那这些新产生的垃圾就没地方放了，这时就会启动一次串行GC，等待时间将会很长，所以要在空间还未满时就要启动GC。mark和sweep操作会引起很多碎片，所以间隔一段时间需要整理整个空间，否则遇到大对象，没有连续空间也会启动一次串行GC。采用此收集器（如tenured
 generation），收集频率不能大，否则会影响到cpu的利用率，进而影响吞吐量。
![](http://hi.csdn.net/attachment/201109/29/0_13173111006etA.gif)
针对年轻代和年老代，收集器的设置参数也是有区别的，如下：
the following collectors operate on the young generation:
-XX:+UseSerialGC
-XX:+UseParallelGC
-XX:+UseParNewGC
the following collectors operate on the old generation:
-XX:+UseParallelOldGC
-XX:+UseConcMarkSweepGC
同一代的参数只能设置一个，-XX:+UseParNewGC和-XX:+UseConcMarkSweepGC是推荐的一个组合。
参考资料：[Memory Management
 in the Java HotSpot™ Virtual Machine](http://java.sun.com/j2se/reference/whitepapers/memorymanagement_whitepaper.pdf)
