
# stackoverflow上一个最会举例子的专家 - 无界 - CSDN博客

2019年03月26日 16:24:52[21aspnet](https://me.csdn.net/21aspnet)阅读数：494



## [https://stackoverflow.com/](https://stackoverflow.com/)
[Premraj](https://stackoverflow.com/users/1697099/premraj)是stackoverflow上一个一个最会举例子的专家，我特意收集了他的一些有趣的举例：
[Java弱引用最精彩的解释](https://linuxstyle.blog.csdn.net/article/details/88816279)
[https://stackoverflow.com/questions/299659/whats-the-difference-between-softreference-and-weakreference-in-java/46291143\#46291143](https://stackoverflow.com/questions/299659/whats-the-difference-between-softreference-and-weakreference-in-java/46291143#46291143)
![](https://img-blog.csdnimg.cn/2019032611441697.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[https://stackoverflow.com/questions/12646404/concurrenthashmap-and-hashtable-in-java/31579480\#31579480](https://stackoverflow.com/questions/12646404/concurrenthashmap-and-hashtable-in-java/31579480#31579480)
![](https://img-blog.csdnimg.cn/2019032616232112.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[https://stackoverflow.com/questions/510632/whats-the-difference-between-concurrenthashmap-and-collections-synchronizedmap](https://stackoverflow.com/questions/510632/whats-the-difference-between-concurrenthashmap-and-collections-synchronizedmap)
![](https://img-blog.csdnimg.cn/20190326162434430.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[https://stackoverflow.com/questions/2889777/difference-between-hashmap-linkedhashmap-and-treemap/50551555\#50551555](https://stackoverflow.com/questions/2889777/difference-between-hashmap-linkedhashmap-and-treemap/50551555#50551555)
# [HashMap，LinkedHashMap和TreeMap之间的区别](https://stackoverflow.com/questions/2889777/difference-between-hashmap-linkedhashmap-and-treemap)
这三个类都实现了`Map`接口，并提供了大部分相同的功能。最重要的区别是条目的迭代顺序：
HashMap绝对不保证迭代顺序。当添加新元素时，它甚至可以（并且将）完全改变。
TreeMap将根据其compareTo()方法（或外部提供的Comparator）按键的“自然排序”进行迭代。此外，它实现了SortedMap接口，该接口包含依赖于此排序顺序的方法。
LinkedHashMap将按照条目放入地图的顺序进行迭代
[“Hashtable”](http://en.wikipedia.org/wiki/Hashtable)是基于散列的映射的通用名称。在Java API的上下文中，`Hashtable`是一个过时的类，它来自Java 1.1之前的集合框架。它不应再被使用，因为它的API混杂着复制功能的过时方法，并且它的方法是同步的（这会降低性能并且通常是无用的）。使用[ConcurrentHashMap](http://docs.oracle.com/javase/7/docs/api/java/util/concurrent/ConcurrentHashMap.html)而不是Hashtable。
![](https://img-blog.csdnimg.cn/20190326163004585.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
这三个都表示从唯一键到值的映射，因此实现了[Map](http://java.sun.com/javase/6/docs/api/java/util/Map.html)接口。
HashMap是基于键[散列](http://en.wikipedia.org/wiki/Hash_function)的映射。它支持O（1）get / put操作。密钥必须具有[一致的实现](http://java.sun.com/javase/6/docs/api/java/lang/Object.html#hashCode())hashCode()[并且](http://java.sun.com/javase/6/docs/api/java/lang/Object.html#hashCode())equals()为此工作。
LinkedHashMap与HashMap非常相似，但它增加了对添加（或访问）项目的顺序的认知，因此迭代顺序与插入顺序（或访问顺序，取决于构造参数）相同。
TreeMap是基于树的映射。其put / get操作需要O（log n）时间。它要求项目具有一些比较机制，可以使用Comparable或Comparator。迭代顺序由此机制确定。
![](https://img-blog.csdnimg.cn/2019032616335217.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326163827494.PNG)
![](https://img-blog.csdnimg.cn/20190326163834737.PNG)
其实就是LinkedHashMap是插入顺序，TreeMap是排序的顺序。
[https://stackoverflow.com/questions/1216380/what-is-a-stream/33914070\#33914070](https://stackoverflow.com/questions/1216380/what-is-a-stream/33914070#33914070)
# [什么是流？](https://stackoverflow.com/questions/1216380/what-is-a-stream)
![](https://img-blog.csdnimg.cn/20190326164123778.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[https://stackoverflow.com/questions/3085285/difference-between-cohesion-and-coupling/48472013\#48472013](https://stackoverflow.com/questions/3085285/difference-between-cohesion-and-coupling/48472013#48472013)
# [内聚与耦合的区别](https://stackoverflow.com/questions/3085285/difference-between-cohesion-and-coupling)
----
[https://enterprisecraftsmanship.com/2015/09/02/cohesion-coupling-difference/](https://enterprisecraftsmanship.com/2015/09/02/cohesion-coupling-difference/)
## 高内聚，低耦合指南
实质上，高内聚意味着**将代码库的一部分保存在一个地方**。同时，低耦合是**尽可能地分离代码库的不相关部分**。
理论上，指南看起来很简单。但是，在实践中，您需要深入了解软件的域模型，以便了解代码库的哪些部分实际相关。
----
**内聚**是指类（或模块）可以做什么。低凝聚力意味着班级做了各种各样的行动 - 它是广泛的，没有关注它应该做什么。高凝聚力意味着课程专注于应该做的事情，即只关注课堂意图的方法。
低内聚的例子：
`-------------------
| Staff           |
-------------------
| checkEmail()    |
| sendEmail()     |
| emailValidate() |
| PrintLetter()   |
-------------------`高内聚的例子：
`----------------------------
| Staff                   |
----------------------------
| -salary                 |
| -emailAddr              |
----------------------------
| setSalary(newSalary)    |
| getSalary()             |
| setEmailAddr(newEmail)  |
| getEmailAddr()          |
----------------------------`至于**耦合**，它指的是两个类/模块相互依赖或相互依赖的方式。对于低耦合类，更改一个类中的主要内容不应该影响另一个类。高耦合会使您难以更改和维护代码; 由于课程紧密相连，因此进行更改可能需要对整个系统进行改造。
良好的软件设计具有**高内聚力**和**低耦合性**。
![](https://img-blog.csdnimg.cn/20190326165050183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[Java中的ThreadFactory用法](https://stackoverflow.com/questions/3179733/threadfactory-usage-in-java)
有人可以简要解释如何使用ThreadFactory？使用和不使用ThreadFactory的示例可能对理解差异非常有帮助。
![](https://img-blog.csdnimg.cn/20190326165426897.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326165511658.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326165603208.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326165626305.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
他也是引用了这里：
[https://docs.oracle.com/javase/7/docs/api/java/util/concurrent/ThreadFactory.html](https://docs.oracle.com/javase/7/docs/api/java/util/concurrent/ThreadFactory.html)
![](https://img-blog.csdnimg.cn/20190326165728918.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
# [如何在非线程的对象上调用wait（）和notify（）方法？](https://stackoverflow.com/questions/16197135/how-can-the-wait-and-notify-methods-be-called-on-objects-that-are-not-thread)
如何在不是线程的对象上调用`wait()`和`notify()`方法？那真的没有意义，是吗？
当然，它必须有意义，因为这两种方法可用于所有Java对象。有人可以提供解释吗？我无法理解如何使用`wait()`和之间的线程之间进行通信`notify()`。
![](https://img-blog.csdnimg.cn/2019032617023074.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326170015145.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
其实`wait`，`notify`成员函数不应该属于线程，它应该属于命名为东西**条件变量**它来自[POSIX线程](https://computing.llnl.gov/tutorials/pthreads/#ConditionVariables)。你可以看看cpp如何包装这个概念，它将它包装成一个专用的类名[std :: condition_variable](https://en.cppreference.com/w/cpp/thread/condition_variable)。
我认为cpp比java更好地封装，java做得太多了，它把概念直接放到了Object类中，好吧，这让人们在开始时感到困惑。

# [Spring @Transactional - 隔离，传播](https://stackoverflow.com/questions/8490852/spring-transactional-isolation-propagation)
有人可以通过现实世界的例子解释注释中的**隔离**和**传播**参数`@Transactional`。基本上何时以及为什么我应该选择更改其默认值。
![](https://img-blog.csdnimg.cn/20190326170414514.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326170620445.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326170630638.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[什么是回调函数？](https://stackoverflow.com/questions/824234/what-is-a-callback-function)
![](https://img-blog.csdnimg.cn/20190326171010235.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

# [如何用简单的英语解释回调？它们与从另一个函数调用一个函数有什么不同？](https://stackoverflow.com/questions/9596276/how-to-explain-callbacks-in-plain-english-how-are-they-different-from-calling-o)
![](https://img-blog.csdnimg.cn/2019032617092257.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

# [JDK动态代理和CGLib有什么区别？](https://stackoverflow.com/questions/10664182/what-is-the-difference-between-jdk-dynamic-proxy-and-cglib)
![](https://img-blog.csdnimg.cn/2019032617121126.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326171331496.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[http://bytebuddy.net/\#/](http://bytebuddy.net/#/)
[Java字节码3-使用ByteBuddy实现一个Java-Agent](http://blog.csdn.net/f59130/article/details/78494572)
[基于 ByteBuddy 运行时动态修改字节码](https://lzxz1234.cn/archives/168)
![](https://img-blog.csdnimg.cn/20190326172638665.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[https://gist.github.com/premraj10/3a3eac42a72c32de3a41ec13ef3d56ad](https://gist.github.com/premraj10/3a3eac42a72c32de3a41ec13ef3d56ad)
# [什么是枚举，为什么它们有用？](https://stackoverflow.com/questions/4709175/what-are-enums-and-why-are-they-useful)
![](https://img-blog.csdnimg.cn/20190326172813221.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[https://www.javatpoint.com/enum-in-java](https://www.javatpoint.com/enum-in-java)
![](https://img-blog.csdnimg.cn/20190326173112467.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

