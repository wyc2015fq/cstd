# 如何将RDD或者MLLib矩阵zhuanzhi - Spark高级玩法 - CSDN博客
2017年12月19日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：808

![?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUd4W8NmVrFrL5XAJscbr0NvFusnJoO714oTVHLmUm9WjBstHKDibFbJb4M3xrIOVQtcEibUgGFeOpQ/?wx_fmt=png&wxfrom=5&wx_lazy=1)
最近老有人在qq群或者公众号留言问浪尖如何将Spark Mllib的矩阵或者将一个RDD进行转置操作。Spark Mllib的矩阵有多种形式，分布式和非分布式，非分布式在这里浪尖就不讲了，很简单，因为他是基于数组的。而分布式存储是基于RDD的，那么问题就又变成了如何将一个RDD进行转置。
首先我们来介绍一下什么是转置操作：
百科上的定义，将一个矩阵的行列互换得到的矩阵就是该矩阵的转置。
要想把一个RDD的行列互换的话，主要思路如下：
1，先转化RDD，给每一行带上唯一的行号(row, rowIndex)。
2，针对RDD的每一行，转化为(value, colIndex)，并整理的到(colIndex.toLong, (rowIndex, value))
3，进行flatmap
4，步骤3完成后，我们只需要按照3key进行分组，并按照其key进行排序就可以得到转化后列式有序。
5，完成步骤4后，我们就可以按照每一行的(rowIndex, value)，使用下标和其值构建新的行，保证每一行转换后的顺序。
到此转换完成。
具体步骤如下：
def transposeRowMatrix(m: RowMatrix): RowMatrix = {
val transposedRowsRDD = m.rows.zipWithIndex.map{case (row, rowIndex) => rowToTransposedTriplet(row, rowIndex)}
    .flatMap(x => x) //  (newRowIndex, (newColIndex, value))
.groupByKey
    .sortByKey().map(_._2) // 对row进行排序，去除掉索引
.map(buildRow) // 利用索引和值，重新构建每一行，去掉索引
new RowMatrix(transposedRowsRDD)
}
//转换每一行
def rowToTransposedTriplet(row: Vector, rowIndex: Long): Array[(Long, (Long, Double))] = {
val indexedRow = row.toArray.zipWithIndex
      indexedRow.map{case (value, colIndex) => (colIndex.toLong, (rowIndex, value))}
    }
//构建新的行
def buildRow(rowWithIndexes: Iterable[(Long, Double)]): Vector = {
val resArr = new Array[Double](rowWithIndexes.size)
      rowWithIndexes.foreach{case (index, value) =>
        resArr(index.toInt) = value
      }
      Vectors.dense(resArr)
    }
测试
准备数据
val observations = sc.parallelize(
Seq(
    Vectors.dense(1.0, 10.0, 100.0,2.0),
Vectors.dense(2.0, 20.0, 200.0,2.0),
Vectors.dense(3.0, 30.0, 300.0,2.0)
  )
)
生成矩阵
val mat: RowMatrix = new RowMatrix(observations)
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUd4W8NmVrFrL5XAJscbr0NFwDTMENhhD2e3C10AcS2RL9o8DpyTM03NRXmDZwI5cv7qUVbnSxdqw/?wx_fmt=png)
会发现行列已经互换。
**推荐阅读：**
1，[HBase在滴滴出行的应用场景和最佳实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484343&idx=1&sn=cd0c5bd1fbd8d55dd9779d336880bb3d&chksm=9f38e09fa84f6989105d027e60beb463332313724813ea114ec3f4e29707c8da75fc09434269&scene=21#wechat_redirect)
2，[SparkML模型选择（超参数调整）与调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484330&idx=1&sn=49095988b3bb00259f79432622ad324f&chksm=9f38e082a84f69943044b515643ce051a4d88154fbe0574b24240aa85a501a5846d83c9bc714&scene=21#wechat_redirect)
3，[SparkMLLib中基于DataFrame的TF-IDF](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484310&idx=1&sn=acf0a573d24c112a4a20370f5430848f&chksm=9f38e0bea84f69a8d50db328b373fdd8a867e2caee3262236c1cf57188bf87cefdfaa456607a&scene=21#wechat_redirect)
4，[用java提交一个Spark应用程序](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484324&idx=1&sn=9d68b15a297c965c24ccece28fa6cc00&chksm=9f38e08ca84f699ab7f50d02bdfecc8da9c697c672e5764dd1333ca0cf028712b5d556006435&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

