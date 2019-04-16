# JDK8中JVM堆内存划分 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年02月20日 11:05:22[gloomyfish](https://me.csdn.net/jia20003)阅读数：20739标签：[JAVA																[内存																[jvm																[Matedata space](https://so.csdn.net/so/search/s.do?q=Matedata space&t=blog)
个人分类：[Swing桌面开发](https://blog.csdn.net/jia20003/article/category/871637)





 一：JVM中内存

JVM中内存通常划分为两个部分，分别为堆内存与栈内存，栈内存主要用执行线程方法

存放本地临时变量与线程中方法执行时候需要的引用对象地址。JVM所有的对象信息都

存放在堆内存中，相比栈内存，堆内存可以所大的多，所以JVM一直通过对堆内存划分

不同的功能区块实现对堆内存中对象管理。

堆内存不够最常见的错误就是**OOM(OutOfMemoryError)**

栈内存溢出最常见的错误就是**StackOverflowError**，程序有递归调用时候最容易发生



二：堆内存划分

在JDK7以及其前期的JDK版本中，堆内存通常被分为三块区域Nursery内存(young 

generation)、长时内存(old generation)、永久内存(Permanent Generation for 

VM Matedata)，显示如下图：

![](https://img-blog.csdn.net/20160220105923300)




其中最上一层是Nursery内存，一个对象被创建以后首先被放到Nursery中的Eden内

存中，如果存活期超两个Survivor之后就会被转移到长时内存中(Old Generation)中

永久内存中存放着对象的方法、变量等元数据信息。通过如果永久内存不够，我们

就会得到如下错误：

java.lang.OutOfMemoryError: PermGen

而在JDK8中情况发生了明显的变化，就是一般情况下你都不会得到这个错误，原因

在于JDK8中把存放元数据中的永久内存从堆内存中移到了本地内存(native memory)

中，JDK8中JVM堆内存结构就变成了如下：

![](https://img-blog.csdn.net/20160220110058880)




这样永久内存就不再占用堆内存，它可以通过自动增长来避免JDK7以及前期版本中

常见的永久内存错误(java.lang.OutOfMemoryError: PermGen)，也许这个就是你的

JDK升级到JDK8的理由之一吧。当然JDK8也提供了一个新的设置Matespace内存

大小的参数，通过这个参数可以设置Matespace内存大小，这样我们可以根据自己

项目的实际情况，避免过度浪费本地内存，达到有效利用。




-XX:MaxMetaspaceSize=128m 设置最大的元内存空间128兆




**注意：**如果不设置JVM将会根据一定的策略自动增加本地元内存空间。

如果你设置的元内存空间过小，你的应用程序可能得到以下错误：

**java.lang.OutOfMemoryError: Metadata space**](https://so.csdn.net/so/search/s.do?q=jvm&t=blog)](https://so.csdn.net/so/search/s.do?q=内存&t=blog)](https://so.csdn.net/so/search/s.do?q=JAVA&t=blog)




