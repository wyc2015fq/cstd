# kmeans算法spark实战 - miner_zhu的博客 - CSDN博客





2018年07月23日 16:28:01[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：81








## 1.标准kmeans算法

kmeans算法是实际中最常用的聚类算法，没有之一。kmeans算法的原理简单，实现起来不是很复杂，实际中使用的效果一般也不错，所以深受广大人民群众的喜爱。 

kmeans算法的原理介绍方面的paper多如牛毛，而且理解起来确实也不是很复杂，这里使用wiki上的版本： 

已知观测集(x1,x2,⋯,xn)(x1,x2,⋯,xn)，其中每个观测都是一个dd维实矢量，kmeans聚类要把这nn个观测值划分到kk个集合中(k≤nk≤n)，使得组内平方和（WCSS within-cluster sum of squares）最小。换句话说，它的目标是找到使得下式满足的聚类SiSi， 

argminS∑i=1k∑x∈Si∥x−μi∥2argminS∑i=1k∑x∈Si‖x−μi‖2



其中μiμi是SiSi中所有点的均值。



标准kmeans算法的步骤一般如下： 

1.先随机挑选k个初始聚类中心。 

2.计算数据集中每个点到每个聚类中心的距离，然后将这个点分配到离该点最近的聚类中心。 

3.重新计算每个类中所有点的坐标的平均值，并将得到的这个新的点作为新的聚类中心。 

重复上面第2、3步，知道聚类中心点不再大范围移动(精度自己定义)或者迭代的总次数达到最大。

## 2.标准kmeans算法的优缺点

标准的kmeans算法的优缺点都很突出。这里挑几个最重要的点总结一下。

### 主要优点：

1.原理简单，易于理解。 

2.实现简单 

3.计算速度较快 

4.聚类效果还不错。

### 主要缺点：

1.需要确定k值。 

2.对初始中心点的选择敏感。 

3.对异常值敏感，因为异常值很很大程度影响聚类中心的位置。 

4.无法增量计算。这点在数据量大的时候尤为突出。

## 3.spark中对kmeans的优化

作为经典的聚类算法，一般的机器学习框架里都实现由kmeans，spark自然也不例外。前面我们已经讲了标准kmeans的流程以及优缺点，那么针对标准kmeans中的不足，spark里主要做了如下的优化：

### 1.选择合适的K值。

k的选择是kmeans算法的关键。Spark MLlib在KMeansModel里实现了computeCost方法，这个方法通过计算数据集中所有的点到最近中心点的平方和来衡量聚类的效果。一般来说，同样的迭代次数，这个cost值越小，说明聚类的效果越好。但在实际使用过程中，必须还要考虑聚类结果的可解释性，不能一味地选择cost值最小的那个k。比如我们如果考虑极限情况，如果数据集有n个点，如果令k=n，每个点都是聚类中心，每个类都只有一个点，此时cost值最小为0。但是这样的聚类结果显然是没有实际意义的。

### 2.选择合适的初始中心点

大部分迭代算法都对初始值很敏感，kmeans也是如此。spark MLlib在初始中心点的选择上，使用了k-means++的算法。想要详细了解k-means++的同学们，可以参考k-means++在wiki上的介绍：[https://en.wikipedia.org/wiki/K-means%2B%2B](https://en.wikipedia.org/wiki/K-means%2B%2B)。 

kmeans++的基本思想是是初始中心店的相互距离尽可能远。为了实现这个初衷，采取如下步骤： 

1.从初始数据集中随机选择一个点作为第一个聚类中心点。 

2.计算数据集中所有点到最近一个中心点的距离D(x)并存在一个数组里，然后将所有这些距离加起来得到Sum(D(x))。 

3.然后再取一个随机值，用权重的方式计算下一个中心点。具体的实现方法：先取一个在Sum(D(x))范围内的随机值，然后领Random -= D(x)，直至Random <= 0，此时这个D(x)对应的点为下一个中心点。 

4.重复2、3步直到k个聚类中心点被找出。 

5.利用找出的k个聚类中心点，执行标准的kmeans算法。

算法的关键是在第三步。有两个小点需要说明： 

1.不能直接取距离最大的那个点当中心店。因为这个点很可能是离群点。 

2.这种取随机值的方法能保证距离最大的那个点被选中的概率最大。给大家举个很简单的例子:假设有四个点A、B、C、D，分别离最近中心的距离D(x)为1、2、3、4，那么Sum(D(x))=10。然后在[0,10]之间取一随机数假设为random，然后用random与D(x)依次相减，直至random<0为止。应该不难发现，D被选中的概率最大。

## 4.spark实战kmeans算法

前面讲了这么多理论，照例咱们需要实践一把。talk is cheap,show me the code!

### 1.准备数据

首先准备数据集。这里采用的数据集是UCI的一个数据集。数据地址[http://archive.ics.uci.edu/ml/datasets/Wholesale+customers?cm_mc_uid=70889544450214522232748&cm_mc_sid_50200000=1469871598](http://archive.ics.uci.edu/ml/datasets/Wholesale+customers?cm_mc_uid=70889544450214522232748&cm_mc_sid_50200000=1469871598)。UCI是一个常用的标准测试数据集，是搞ML与DM同学经常使用的数据集。关于该数据集的介绍，同学们可以去网页上查看。

将数据下载下来以后查看一把，第一行相当于是表头，是对数据的相关说明。将此行去掉，还剩440行。将前400行作为训练集，后40行作为测试集。

### 2.将代码run起来

```java
import org.apache.spark.{SparkContext, SparkConf}
import org.apache.spark.mllib.clustering.{KMeans, KMeansModel}
import org.apache.spark.mllib.linalg.Vectors

object KmeansTest {
  def main(args: Array[String]) {

    val conf = new
        SparkConf().setAppName("K-Means Clustering").setMaster("spark://your host:7077").setJars(List("your jar file"))
    val sc = new SparkContext(conf)

    val rawTrainingData = sc.textFile("file:///Users/lei.wang/data/data_training")
    val parsedTrainingData =
      rawTrainingData.filter(!isColumnNameLine(_)).map(line => {
        Vectors.dense(line.split(",").map(_.trim).filter(!"".equals(_)).map(_.toDouble))
      }).cache()

    // Cluster the data into two classes using KMeans

    val numClusters = 8
    val numIterations = 30
    val runTimes = 3
    var clusterIndex: Int = 0
    val clusters: KMeansModel =
      KMeans.train(parsedTrainingData, numClusters, numIterations, runTimes)

    println("Cluster Number:" + clusters.clusterCenters.length)

    println("Cluster Centers Information Overview:")
    clusters.clusterCenters.foreach(
      x => {
        println("Center Point of Cluster " + clusterIndex + ":")
        println(x)
        clusterIndex += 1
      })

    //begin to check which cluster each test data belongs to based on the clustering result

    val rawTestData = sc.textFile("file:///Users/lei.wang/data/data_test")
    val parsedTestData = rawTestData.map(line => {
      Vectors.dense(line.split(",").map(_.trim).filter(!"".equals(_)).map(_.toDouble))

    })
    parsedTestData.collect().foreach(testDataLine => {
      val predictedClusterIndex:
      Int = clusters.predict(testDataLine)
      println("The data " + testDataLine.toString + " belongs to cluster " +
        predictedClusterIndex)
    })

    println("Spark MLlib K-means clustering test finished.")
  }

  private def isColumnNameLine(line: String): Boolean = {
    if (line != null && line.contains("Channel")) true
    else false
  }
}
```

## 5.后续工作

本次测试是在单机上做的demo测试，数据集比较小，运算过程也比较快。其实当数据量增大以后，基本过程跟这是类似的，只需要将input改为集群的数据路径，然后再写个简单的shell脚本，调用spark-submit，将任务提交到集群即可。



原文网址：[https://blog.csdn.net/bitcarmanlee/article/details/52092288](https://blog.csdn.net/bitcarmanlee/article/details/52092288)



