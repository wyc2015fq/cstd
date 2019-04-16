# 探索JVM 内存分配 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年07月15日 14:55:10[gloomyfish](https://me.csdn.net/jia20003)阅读数：6120








> 
> 
> 
> 
> 






Java中的堆内存与栈内存：










栈内存主要用于Java 代码的加载和执行，一个典型的例子是递归次数过多，JVM会抛出一个

Runtime错误StackOverflowError，栈内存的分配与管理完全依赖于JVM本身。



内存可以分为大致分为: 永久Generation（Perm Gen）， 长时Generation (Old Gen)，短时

Generation (New/Young Gen)

![](http://hi.csdn.net/attachment/201107/15/0_1310712618NwZ2.gif)


**永久Generation(permanent generation):**主要用来存储VM的对象信息，理论上来说应用

程序需要加载的class越多，需要的Perm Gen也会越大，所有的类和方法对象信息(MetaData)

将会存储在永久Generation中，对象将会在长时Generation或者短时Generation中，JVM中提

供调整永久Generation的参数为-XX:PermSize=32m 与-XX:MaxPermSize, 永久内存主要是

non-heap内存



**长时Generation (Old Generation)：**JVM默认的Old Generation是 Tenured Generation, 应

用程序大多数的堆内存分配都发生在短时Generation(YoungGeneration)上，如果对象在经过几

个垃圾回收周期以后还存在于YoungGeneration, JVM将会自动把对象移到长时Generation

（TenuredGeneration）,长时Generation中垃圾回收发生的频率将大大低于短时Generation.



**短时Generation(Young Generation)**:由两个部分组成Eden Space 和 Survivor Space对应

的JVM参数为-XX:NewRatio和-XX:SurvivorRatio, 任何新创建的对象操作都会发生在Eden 

Space. 短时Generation的堆内存中，JVM的垃圾回收会经常发生用来清除无用对象。

![](http://hi.csdn.net/attachment/201107/15/0_1310712797tq9z.gif)


上图是JDK6在实时运行过程中堆内存的活动情况。



**堆内存回收JVM参数详解：**

参数**-XX:+UseParallelGC**表示并行GC收集者（Collector）是多线程环境下使用的，默认情况下主

机有多少个CPU垃圾回收器就创建多少个垃圾回收者线程，垃圾回收者线程的数目可以通过JVM

参数-XX:ParallelGCThreads=n (n desirednumber) 来控制，图示是一个使用并行GC的例子

![](http://hi.csdn.net/attachment/201107/15/0_1310712863Lhzb.gif)


参数**-XX:+UseParNewGC**表示并行Young GC,它与并发低中断收集者(concurrent low pause


collector)集成在一起使用，而且并发收集者参数-XX:+UseParallelGC不能与-XX:+UseParNewGC参

数同时使用。一个使用-XX:+UseParNewGC参数的效果如下：

![](http://hi.csdn.net/attachment/201107/15/0_131071288939GA.gif)


参数**-XX:+UseConcMarkSweepGC**表示使用并发低中断收集者作为GC Collector,为了使CMS GC更

好的工作，推荐设置相同的你永久内存最小和最大size，在本例中我的设置值如下：

-XX:PermSize=32m

-XX:MaxPermSize=128m

-XX:+UseConcMarkSweepGC

效果图示如下：

![](http://hi.csdn.net/attachment/201107/15/0_13107129305B5T.gif)


**内存分配参数详解：**

**-XX:PermSize **表示请求初始分配永久性内存大小

**-XX:MaxPermSize**表示分配的最大的永久性内存大小



-Xmx512m表示JVM最大的可用内存为512MB

-Xms128m表示初始启动JVM分配的为128MB

-Xmn64m表示初始分配给Eden Space内存大小，理论上不要分配太大的内存块给它。

XX:+UseLargePages -在JDK5之后新加了对大分页内存的支持参数

-Xss表示分配线程栈的内存大小，如果过小可能导致抛出StackOverflowError



在一个1GB物理内存中，Java App进程启动参数配置例子：

-Xmx512m 

-Xms512m 

-XX:MaxNewSize=24m 

-XX:NewSize=24m 

-XX:+UseConcMarkSweepGC



JDK6中默认值PermSize为16MB， MaxPermSize为64MB。默认的CodeCache的大小为32MB

在32位的JDK中JVM支持的最大内存值大概在2GB左右，在64位的JDK版本中，理论上没有

限制。



参考引用：

[http://www.oracle.com/technetwork/java/javase/tech/vmoptions-jsp-140102.html](http://www.oracle.com/technetwork/java/javase/tech/vmoptions-jsp-140102.html)

[http://java.sun.com/docs/hotspot/gc1.4.2/faq.html](http://java.sun.com/docs/hotspot/gc1.4.2/faq.html)

PDF document - 《INSIDE THE JAVA VIRTUAL MACHINE》



研究工具： 

Java YourKit 9.0




