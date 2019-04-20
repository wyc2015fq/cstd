# 理解Spark里的闭包 - Spark高级玩法 - CSDN博客
2018年06月11日 10:36:19[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：604
闭包的概念如下图：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUkfd862lO7r8vEVQuD2GhibP6s2CAwDp8Kicbv2OYDmbh4v6GQyccxZTSjtrnKZEvT0GfjkjR3Ow9g/640?wx_fmt=png)
在spark应用里，变量及函数的作用范围和声明周期在spark的集群运行模式下是比较难理解的，尤其是对初学者来说。RDD的操作，要修改其作用范围的变量，经常会出点叉子。下面，可以举个用foreach，修改一个计数器的例子。
**例子**
求和RDD元素的例子，该例子会根据该段代码是否执行在同一个jvm里面有不同的输出结果，比如local模式，运行于同一个jvm，输出是15；cluster模式运行于不同jvm输出是0。
val data = Array(1, 2, 3, 4, 5)
var counter =0
var rdd = sc.parallelize(data)
// Wrong: Don't do this!!
rdd.foreach(x => counter += x)
println("Counter value: "+ counter)
**本地或集群模式**
上述代码的行为是未定义的，并且不同模式下运行情况不同。为了执行作业，Spark将RDD操作的处理分解为tasks，每个task由Executor执行。在执行之前，Spark会计算task的闭包。闭包是Executor在RDD上进行计算的时候必须可见的那些变量和方法（在这种情况下是foreach()）。闭包会被序列化并发送给每个Executor。
发送给每个Executor的闭包中的变量是副本，因此，当foreach函数内引用计数器时，它不再是driver节点上的计数器。driver节点的内存中仍有一个计数器，但该变量是Executor不可见的！执行者只能看到序列化闭包的副本。因此，计数器的最终值仍然为零，因为计数器上的所有操作都引用了序列化闭包内的值。
在本地模式下，在某些情况下，该foreach函数实际上将在与driver相同的JVM内执行，并且会引用相同的原始计数器，并可能实际更新它。
为了确保在这些场景中明确定义的行为，应该使用一个Accumulator。Spark中的累加器专门用于提供一种机制，用于在集群中的工作节点之间执行拆分时安全地更新变量。
一般来说，closures - constructs像循环或本地定义的方法，不应该被用来改变一些全局状态。Spark并没有定义或保证从闭包外引用的对象的改变行为。这样做的一些代码可以在本地模式下工作，但这只是偶然，并且这种代码在分布式模式下的行为不会像你想的那样。如果需要某些全局聚合，请改用累加器。
**打印RDD的元素**
另一个常见的习惯用法是尝试使用rdd.foreach(println)或rdd.map(println)打印出RDD的元素。在单台机器上，这将产生预期的输出并打印所有RDD的元素。但是，在cluster模式下，由Executor执行输出写入的是Executor的stdout，而不是driver上的那个stdout，所以driver的stdout不会显示这些！要在driver中打印所有元素，可以使用该collect()方法首先将RDD数据带到driver节点：rdd.collect().foreach(println)。但这可能会导致driver程序内存不足，因为collect()会将整个RDD数据提取到driver端; 如果您只需要打印RDD的一些元素，则更安全的方法是使用take()：rdd.take(100).foreach(println)。
推荐阅读：
1，[必读：Spark与kafka010整合](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484604&idx=1&sn=c7fb09731502041562894ad9ef06a8ca&chksm=9f38e794a84f6e82dddf1b6c0b081f7f3e0512b85557e7165f87c443ca804f7af6e11846cffa&scene=21#wechat_redirect)
2，[论Spark Streaming的数据可靠性和一致性](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484247&idx=1&sn=f9a65c44ea7186012d72dee25526ec77&chksm=9f38e07fa84f69699c3599dbf06753b7b4b99148fc076b1ea6896aef466731c012a4831a4607&scene=21#wechat_redirect)
3，[Spark技术学院-进去能学到啥？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484919&idx=1&sn=c435fdd7d500285721649554f283fcd4&chksm=9f38e6dfa84f6fc96d8663250ba20fc792c94a4cfab8a8db2001fdc42a320c51bc327c28ed71&scene=21#wechat_redirect)
与bat大牛一起学习进步，请关注知识星球：spark技术学院
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFU7Oic5ibSSV5sJGB2RgzSeqQpuBicxOPgCBglwQPt9rnatXN0VblKhw9WicqSGsuTU9o0Sj12698QpuQ/640?wx_fmt=jpeg)
