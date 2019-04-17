# 64位Linux下JVM内存调设遇到GC问题的备忘 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月22日 14:32:21[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2363








问题：jar提交到64位的linux环境中执行，由于文件过大，5G左右，发现数据一致导入不了，于是设置了JVM内存，执行命令如下：



```java
java  -Xmx10240m -jar vip.jar
```


结果爆出GC错误：




`java.lang.OutOfMemoryError: GC overhead limit exceeded`

分析：



官网上对该错误的说明：


`The concurrent collector will throw an OutOfMemoryError if too much time is being spent in garbage collection: if more than 98% of the total time is spent in garbage collection and less than 2% of the heap is recovered, an OutOfMemoryError will be thrown. This feature is designed to prevent applications from running for an extended period of time while making little or no progress because the heap is too small. If necessary, this feature can be disabled by adding the option-XX:-UseGCOverheadLimitto the command line.`

在JDK_1.7.0_60版本中，JVM默认启动的时候-XX:+UseGCOverheadLimit，即启用了该特性。这其实是JVM的一种推断，如果垃圾回收耗费了98%的时间，但是回收的内存还不到2%，那么JVM会认为即将发生OOM，让程序提前结束。可以使用-XX:-UseGCOverheadLimit，关掉这个特性。






执行程序加了这个设置，如下：


`java -XX:-UseGCOverheadLimit -Xmx10240m -jar vip.jar`

有必要总结下JVM内存调优和JVM参数设置：



-server//服务器模式

-Xmx2g //JVM最大允许分配的堆内存，按需分配

-Xms2g //JVM初始分配的堆内存，一般和Xmx配置成一样以避免每次gc后JVM重新分配内存。

-Xmn256m //年轻代内存大小，整个JVM内存=年轻代 + 年老代 + 持久代

-XX:PermSize=128m //持久代内存大小

-Xss256k //设置每个线程的堆栈大小

-XX:+DisableExplicitGC //忽略手动调用GC, System.gc()的调用就会变成一个空调用，完全不触发GC

-XX:+UseConcMarkSweepGC //并发标记清除（CMS）收集器

-XX:+CMSParallelRemarkEnabled //降低标记停顿

-XX:+UseCMSCompactAtFullCollection //在FULL GC的时候对年老代的压缩

-XX:LargePageSizeInBytes=128m //内存页的大小

-XX:+UseFastAccessorMethods //原始类型的快速优化

-XX:+UseCMSInitiatingOccupancyOnly //使用手动定义初始化定义开始CMS收集

-XX:CMSInitiatingOccupancyFraction=70 //使用cms作为垃圾回收使用70％后开始CMS收集


JVM参数大全参考：http://www.blogjava.net/bitbit/archive/2009/11/30/304247.html




后续又爆出错误：


`java.lang.OutOfMemoryError: Java heap space `

增加JVM参数设置：




`java -XX:-UseGCOverheadLimit -Xmx10240m -Xms10240m -jar vip.jar`

最好先执行下：`ulimit -c unlimited`






