# 【Java】—— JVM篇 - csdn_baotai的博客 - CSDN博客

2018年08月28日 09:09:50[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：192


**JVM知识**
- 
什么情况下会发生栈内存溢出。

- 
JVM的内存结构，Eden和Survivor比例。

- 
JVM内存为什么要分成新生代，老年代，持久代。新生代中为什么要分为Eden和Survivor。

- 
JVM中一次完整的GC流程是怎样的，对象如何晋升到老年代，说说你知道的几种主要的JVM参

	数。

- 
你知道哪几种垃圾收集器，各自的优缺点，重点讲下cms和G1，包括原理，流程，优缺点。

- 
垃圾回收算法的实现原理。

- 
当出现了内存溢出，你怎么排错。

- 
JVM内存模型的相关知识了解多少，比如重排序，内存屏障，happen-before，主内存，工作

	内存等。

- 
简单说说你了解的类加载器，可以打破双亲委派么，怎么打破。

- 
讲讲JAVA的反射机制。

- 
你们线上应用的JVM参数有哪些。

- 
g1和cms区别,吞吐量优先和响应优先的垃圾收集器选择。

- 
怎么打出线程栈信息。

- 
请解释如下jvm参数的含义：

	-server -Xms512m -Xmx512m -Xss1024K

	-XX:PermSize=256m -XX:MaxPermSize=512m -

	XX:MaxTenuringThreshold=20XX:CMSInitiatingOccupancyFraction=80 -

	XX:+UseCMSInitiatingOccupancyOnly。


