# 你真知道如何高效用mapPartitions吗？ - Spark高级玩法 - CSDN博客
2018年08月13日 00:05:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1454
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
做过一段时间spark的应用开发的小伙伴都会渐渐发现，很没趣，因为都是调API。那么，真的是没趣吗，还是说你本身没有去深入研究呢？通过本文你就会发现自己没成长是哪的问题了。
浪尖会花一段时间在spark的算子原理分析和高性能使用对比方面的分析，并将这些知识放到浪尖的**知识星球**里。有兴趣的同学扫描底部**二维码**或者点击**阅读原文**加入星球。昨天将spark1.6源码阅读视频已经上传到星球里。
顺便打个广告，浪尖开了知乎，有兴趣的可以关注一下，搜索**浪尖**即可。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)1. mappartition粗介
本问主要想讲如何高效的使用mappartition。
首先，说到mappartition大家肯定想到的是map和MapPartition的对比。网上这类教程很多了，以前浪尖也发过类似的，比如
[对比foreach和foreachpartition](https://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483766&idx=1&sn=bed469000c668cc65a7329a278dd5c41&scene=21#wechat_redirect)
主要是map和foreach这类的是针对一个元素调用一次我们的函数，也即是我们的函数参数是单个元素，假如函数内部存在数据库链接、文件等的创建及关闭，那么会导致处理每个元素时创建一次链接或者句柄，导致性能底下，很多初学者犯过这种毛病。
而foreachpartition是针对每个分区调用一次我们的函数，也即是我们函数传入的参数是整个分区数据的迭代器，这样避免了创建过多的临时链接等，提升了性能。
下面的例子都是1-20这20个数字,经过map或者MapPartition然后返回a*3。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)2. map栗子
val a = sc.parallelize(1 to 20, 2)def mapTerFunc(a : Int) : Int = {
    a*3}val mapResult = a.map(mapTerFunc)
println(mapResult.collect().mkString(","))
结果
3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,51,54,57,60
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)3. mappartitions低效用法
大家通常的做法都是申请一个迭代器buffer，将处理后的数据加入迭代器buffer，然后返回迭代器。如下面的demo。
val a = sc.parallelize(1 to 20, 2)  def terFunc(iter: Iterator[Int]) : Iterator[Int] = {    var res = List[Int]()    while (iter.hasNext)
    {      val cur = iter.next;
      res.::= (cur*3) ;
    }
    res.iterator
  }val result = a.mapPartitions(terFunc)
println(result.collect().mkString(","))
结果
30,27,24,21,18,15,12,9,6,3,60,57,54,51,48,45,42,39,36,33
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)4. mappartitions高效用法
注意，3中的例子，会在mappartition执行期间，在内存中定义一个数组并且将缓存所有的数据。假如数据集比较大，内存不足，会导致内存溢出，任务失败。 对于这样的案例，Spark的RDD不支持像mapreduce那些有上下文的写方法。其实，浪尖有个方法是无需缓存数据的，那就是自定义一个迭代器类。如下例：
class CustomIterator(iter: Iterator[Int]) extends Iterator[Int] {    def hasNext : Boolean = {
      iter.hasNext
    }    def next : Int= {    
    val cur = iter.next
    
     cur*3
    }
  }  
  val result = a.mapPartitions(v => new CustomIterator(v))
  println(result.collect().mkString(","))
结果：
3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,51,54,57,60
是不是脑洞大开，要多学习的，同志们。
【完】
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)
