# 必读|spark的重分区及排序 - Spark高级玩法 - CSDN博客
2018年08月14日 00:01:55[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1863
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
前几天，有人在星球里，问了一个有趣的算子，也即是RepartitionAndSortWithinPartitions。当时浪尖也在星球里讲了一下，整个关于分区排序的内容。今天，在这里给大家分享一下。
更多大数据小技巧及调优，spark的源码文章，原理文章及源码视频请加入**知识星球**。扫描，底部二维码，或者点击**阅读原文**。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
昨天说了，mapPartitions 的使用技巧。大家应该都知道mapPartitions值针对整个分区执行map操作。而且对于PairRDD的分区默认是基于hdfs的物理块，当然不可分割的话就是hdfs的文件个数。但是我们也可以给partitionBy 算子传入HashPartitioner，来给RDD进行重新分区，而且会使得key的hashcode相同的数据落到同一个分区。
spark 1.2之后引入了一个高质量的算子repartitionAndSortWithinPartitions 。该算子为spark的Shuffle增加了sort。假如，后面再跟mapPartitions算子的话，其算子就是针对已经按照key排序的分区，这就有点像mr的意思了。与groupbykey不同的是，数据不会一次装入内存，而是使用迭代器一次一条记录从磁盘加载。这种方式最小化了内存压力。
repartitionAndSortWithinPartitions 也可以用于二次排序。
下面举个简单的例子。
```
import org.apache.spark.Partitioner
  class KeyBasePartitioner(partitions: Int) extends Partitioner {
    override def numPartitions: Int = partitions
    override def getPartition(key: Any): Int = {
      val k = key.asInstanceOf[Int]
      Math.abs(k.hashCode() % numPartitions)
    }
  }
  
  import org.apache.spark.SparkContext._
      sc.textFile("file:///opt/hadoop/spark-2.3.1/README.md").flatMap(_.split("\\s+")).map((_,1)).reduceByKey(_+_).map(each=>(each._2,each._1))
      implicit val caseInsensitiveOrdering = new Ordering[Int] {
       override def compare(a: Int, b: Int) = b.compareTo(a)
      }
      // Sort by key, using 
  res7.repartitionAndSortWithinPartitions(new KeyBasePartitioner(3)).saveAsTextFile("file:///opt/output/")
```
结果,可以看到每个分区都是有效的。
```
mdhdeMacBook-Pro-3:output mdh$ pwd
/opt/output
mdhdeMacBook-Pro-3:output mdh$ ls
_SUCCESS        part-00000      part-00001      part-00002
mdhdeMacBook-Pro-3:output mdh$ head -n 10 part-00000 
(24,the)
(12,for)
(9,##)
(9,and)
(6,is)
(6,in)
(3,general)
(3,documentation)
(3,example)
(3,how)
mdhdeMacBook-Pro-3:output mdh$ head -n 10 part-00001
(16,Spark)
(7,can)
(7,run)
(7,on)
(4,build)
(4,Please)
(4,with)
(4,also)
(4,if)
(4,including)
mdhdeMacBook-Pro-3:output mdh$ head -n 10 part-00002
(47,)
(17,to)
(8,a)
(5,using)
(5,of)
(2,Python)
(2,locally)
(2,This)
(2,Hive)
(2,SparkPi)
mdhdeMacBook-Pro-3:output mdh$
```
上面只是一个简单的使用，关于二次排序及高效结合mapPartitions的例子，浪尖会在这两天更新到星球里。
【完】
**推荐阅读：**
[你真知道如何高效用mapPartitions吗？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485220&idx=1&sn=e4965b088e1d55914a235f84c2f01908&chksm=9f38e40ca84f6d1ae4b2e5fbac7f78612a40f0d948ca4104ac1bfbe91df021d63ee9dbeb14a6&scene=21#wechat_redirect)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)
