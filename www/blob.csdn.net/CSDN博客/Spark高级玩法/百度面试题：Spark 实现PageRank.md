# 百度面试题：Spark 实现PageRank - Spark高级玩法 - CSDN博客
2018年09月04日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：307
**PageRank算法简介**
PageRank是执行多次连接的一个迭代算法，因此它是RDD分区操作的一个很好的用例。算法会维护两个数据集：一个由（pageID，linkList）的元素组成，包含每个页面的相邻页面的列表；另一个由（pageID，rank）元素组成，包含每个页面的当前排序值。它按如下步骤进行计算。
- 
将每个页面的排序值初始化为1.0。
- 
在每次迭代中，对页面p，向其每个相邻页面（有直接链接的页面）发送一个值为rank(p)/numNeighbors(p)的贡献值。
- 
将每个页面的排序值设为0.15 + 0.85 * contributionsReceived。
        最后两个步骤会重复几个循环，在此过程中，算法会逐渐收敛于每个页面的实际PageRank值。在实际操作中，收敛通常需要大约10轮迭代。
**模拟数据**
假设一个由4个页面组成的小团体：A，B，C和D。相邻页面如下所示：
A：B C 
B：A C 
C：A B D 
D：C 
```
object SparkPageRank {
  def showWarning() {
    System.err.println(
      """WARN: This is a naive implementation of PageRank and is given as an example!
        |Please use the PageRank implementation found in org.apache.spark.graphx.lib.PageRank
        |for more conventional use.
      """.stripMargin)
  }
  def main(args: Array[String]) {
    if (args.length < 1) {
      System.err.println("Usage: SparkPageRank <file> <iter>")
      System.exit(1)
    }
    showWarning()
    val spark = SparkSession
      .builder
      .appName("SparkPageRank")
      .getOrCreate()
    val iters = if (args.length > 1) args(1).toInt else 10
    val lines = spark.read.textFile(args(0)).rdd
    val links = lines.map{ s =>
      val parts = s.split("\\s+")
      (parts(0), parts(1))
    }.distinct().groupByKey().cache()
    var ranks = links.mapValues(v => 1.0)
    for (i <- 1 to iters) {
      val contribs = links.join(ranks).values.flatMap{ case (urls, rank) =>
        val size = urls.size
        urls.map(url => (url, rank / size))
      }
      ranks = contribs.reduceByKey(_ + _).mapValues(0.15 + 0.85 * _)
    }
    val output = ranks.collect()
    output.foreach(tup => println(s"${tup._1} has rank:  ${tup._2} ."))
    spark.stop()
  }
}
```
推荐阅读：
[hive的join优化](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485321&idx=1&sn=af0c370727d60ce6e83d263394767447&chksm=9f38e4a1a84f6db7ddf1cac3c29e33e76ec84d9d454749f151dedc86400e6a525aa40a675d19&scene=21#wechat_redirect)
[经验|如何设置Spark资源](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485250&idx=1&sn=f9a8a27d6b966fb53725505072f50f8f&chksm=9f38e46aa84f6d7ca50bec7c593f73ddc064af6e598cde4ea9ed85b91aef89e0f55065c4bad2&scene=21#wechat_redirect)
[干货:Flink+Kafka 0.11端到端精确一次处理语义实现](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485150&idx=1&sn=ba0570a9f631501a189d8184807d518b&chksm=9f38e5f6a84f6ce0dd805eae87d6c62e2c96670f14c2bac1607961614ba49f310a4630e31a3b&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
