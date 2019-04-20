# 案例读懂Spark的闭包清理机制 - Spark高级玩法 - CSDN博客
2019年01月24日 22:44:09[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：102
今天，这个话题开启的原因是很多球友或者粉丝都私聊过浪尖，关于Spark任务运行时发生不可序列话的问题。所以，浪尖今天就统一讲解一下这块的内容。
首先，要先读懂scala的闭包是怎么回事儿。可以参考我前面的文章
[简单理解scala的闭包](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486351&idx=1&sn=547e825d363d5dafa5f84ef2ea098d36&chksm=9f38e8a7a84f61b17ed2f4925879f3498c79fe93101778f33ceb86d8e4c941b2705db3603861&scene=21#wechat_redirect)
接着就是要理解Spark 算子闭包生成及我们编写的闭包执行的原理。接下来我们就拿map和mapPartition两个算子来开启本文讲解：
def map[U: ClassTag](f: T => U): RDD[U] = withScope {
    val cleanF = sc.clean(f)
new MapPartitionsRDD[U, T](this, (context, pid, iter) => iter.map(cleanF))
  }
def mapPartitions[U: ClassTag](
      f: Iterator[T] => Iterator[U],
      preservesPartitioning: Boolean = false): RDD[U] = withScope {
    val cleanedF = sc.clean(f)
new MapPartitionsRDD(
this,
(context: TaskContext, index: Int, iter: Iterator[T]) => cleanedF(iter),
      preservesPartitioning)
  }
对于任务划分，调度，执行，结果返回的原理浪尖就不在这里扩展了，浪尖在知识星球里分享过一套Spark 源码的视频，可以参考阅读。
map和mapPartitions的区别面试常考的，对于两者的区别从源码里看很明显，一个是f被迭代器迭代调用，一个是f的参数是迭代器。浪尖很早以前发过一篇文章，彻底讲解过foreach和foreachPartition的区别。可以参考理解
[Spark源码系列之foreach和foreachPartition的区别](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483766&idx=1&sn=bed469000c668cc65a7329a278dd5c41&chksm=9f38e25ea84f6b48012580bd02227b70a1f6c1fa93ee57a8e6c569686553b22f388554fabb69&scene=21#wechat_redirect)
回到正题，之所以会发生不可序列化的错误，主要原因是传递给map的f函数不是在driver端执行的，所以会被序列化传输到executor节点，然后在executor节点反序列化然后执行。假如f函数里引用了map外部不可序列化的对象就会报不可序列化的异常。
但是，很多时候我们并没有直接去在闭包里使用不可序列化的对象，这个时候报异常就有点不合适了。比如下面的例子：
*   classSomethingNotSerializable {
   *     defsomeValue = 1
   *     defscope(name: String)(body: => Unit) = body
   *     defsomeMethod(): Unit = scope("one") {
   *       defx = someValue
   *       defy = 2
   *       scope("two") { println(y + 1) }
   *     }
   *   }
此示例中，scope(two) 不可序列化，因为它引用了scope(one)(通过y)，而scope(one)引用了SomethingNotSerializable（通过someValue）。但是，其实scope(two)并不直接依赖于SomethingNotSerializable。假如这种情况下抛出不可序列化异常就不科学了，所以Spark会对闭包进行一些清理操作，也即是本文中所要讲的。
主要工具类是ClosureCleaner。该工具的主要作用是遍历闭包的层次结构，并且将没有被闭包实际引用的链路设置为null，但是仍然包含在已经编译的匿名类中。请注意直接修改封闭中的闭包是不安全的，因为可能有其他代码路径会依赖于他们。所以，我们会克隆封闭中的闭包并且相应地设置父指针。
默认情况下，可以传递清除闭包。这就意味着，我们需要检测封闭对象是否由起始对象实际引用，（要么直接引用要么间接引用），如果没有被实际使用则从层次结构中切断这些闭包。换句话说，除了清空无用字段的引用之外，也会将没有被起始闭包引用的引用封闭对象的父指针清空。传递性的确定是通过遍历闭包所调用的
再回到前面的例子，scope(two) 不可序列化，因为它引用了scope(one)(通过y)，而scope(one)引用了SomethingNotSerializable（通过someValue）。但是，其实scope(two)并不直接依赖于SomethingNotSerializable。这就意味着我们可以安全的将其副本scope(one)的父指针清空，同时将其设置为scope（two）的父级，这样scope(two)就不再需要间接传递引用SomethingNotSerializable了。
***解决方法***
**实现序列化**是最直接的，假如不能的话。那就读下面的话：
那么为了不实现序列化还能尽量避免不可序列化错误，就不要在map等算子里引用外部变量，而是直接在算子中实例化，假如每次实例化代价高，那就使用mapPartitions。
***还不懂，下次就只能上源码了～***
***推荐阅读：***
[spark rdd的另类解读](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486098&idx=1&sn=ecc9682268acd51befc333929675c592&chksm=9f38e9baa84f60ac63c29f2de8cdb8fe6f30de92963128c2be61e23cff200670b567327ce9a6&scene=21#wechat_redirect)
[不懂watermark？来吧～](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486324&idx=1&sn=b1132367e26e6da1c1982de4af436e91&chksm=9f38e85ca84f614a201eb20c5b365cfd9e93b2c2b93f1906a12181e8720afc7d0541309c1dc4&scene=21#wechat_redirect)
[源码:Spark SQL 分区特性第一弹](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486061&idx=1&sn=3662feca085f66e7a686aa6e627d60ec&chksm=9f38e945a84f60538936a1ecfc462127cddb7cfc44bda4a8d1ed4f3300e6906f733917f83634&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
