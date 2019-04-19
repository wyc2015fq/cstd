# Java  基本结构 - Simple 专栏 - CSDN博客
2012年02月06日 17:42:58[Simple_Zz](https://me.csdn.net/love284969214)阅读数：2110标签：[java																[object																[Java基础																[基本结构																[架构图](https://so.csdn.net/so/search/s.do?q=架构图&t=blog)](https://so.csdn.net/so/search/s.do?q=基本结构&t=blog)](https://so.csdn.net/so/search/s.do?q=Java基础&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java																[JVM](https://blog.csdn.net/love284969214/article/category/8120996)](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
从Java平台的逻辑结构上来看，我们可以从下图来了解JVM：
![](http://hi.csdn.net/attachment/201009/25/0_1285381196kc7N.gif)
从上图能清晰看到Java平台包含的各个逻辑模块，也能了解到JDK与JRE的区别
对于JVM自身的物理结构，我们可以从下图鸟瞰一下：
![](http://hi.csdn.net/attachment/201009/25/0_1285381395C6iW.gif)
对于JVM的学习，在我看来这么几个部分最重要：
- Java代码编译和执行的整个过程
- JVM内存管理及垃圾回收机制
下面将这两个部分进行详细学习
