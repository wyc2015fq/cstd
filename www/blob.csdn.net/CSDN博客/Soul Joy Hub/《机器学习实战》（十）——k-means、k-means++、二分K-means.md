# 《机器学习实战》（十）——k-means、k-means++、二分K-means - Soul Joy Hub - CSDN博客

2017年03月30日 18:45:23[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：3449
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



# k-means

## 原理
- 创建K个点作为起始质点。每次迭代如下： 
- 将各个数据点分配到离它距离最近的质点的簇。
- 全部分配后，用各个簇中的数据点的位置均值来更新质点的位置。

- 直到达到迭代次数，或者所有的数据点所在的簇不再改变。

可参阅：[http://blog.csdn.net/u011239443/article/details/51707802#t0](http://blog.csdn.net/u011239443/article/details/51707802#t0)

## 实现

### Python
- 支持函数

```python
# 加载数据
def loadDataSet(fileName):      
    dataMat = []                
    fr = open(fileName)
    for line in fr.readlines():
        curLine = line.strip().split('\t')
        fltLine = map(float,curLine) 
        dataMat.append(fltLine)
    return dataMat
```

![](http://upload-images.jianshu.io/upload_images/1621805-15fbc0797ae369d4?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

```python
# 计算距离
def distEclud(vecA, vecB):
    return sqrt(sum(power(vecA - vecB, 2))) #la.norm(vecA-vecB)

# 随机创建质点
def randCent(dataSet, k):
    n = shape(dataSet)[1]
    centroids = mat(zeros((k,n)))
    for j in range(n):
        minJ = min(dataSet[:,j]) 
        rangeJ = float(max(dataSet[:,j]) - minJ)
        centroids[:,j] = mat(minJ + rangeJ * random.rand(k,1))
    return centroids
```
- 聚类算法 

```python
def kMeans(dataSet, k, distMeas=distEclud, createCent=randCent):
    m = shape(dataSet)[0]
    # 存储聚类结果 （质点，离该质点的距离）
    clusterAssment = mat(zeros((m,2)))
    # 随机创建k个质点
    centroids = createCent(dataSet, k)
    # 用于判断所有的数据点所在的簇是否不再改变
    clusterChanged = True
    while clusterChanged:
        clusterChanged = False
        # 对每个数据点
        for i in range(m):
            minDist = inf; minIndex = -1
            # 求与之最近的质点
            for j in range(k):
                distJI = distMeas(centroids[j,:],dataSet[i,:])
                if distJI < minDist:
                    minDist = distJI; minIndex = j
            if clusterAssment[i,0] != minIndex: clusterChanged = True
            clusterAssment[i,:] = minIndex,minDist**2
        print centroids
        # 更新质点
        for cent in range(k):
            ptsInClust = dataSet[nonzero(clusterAssment[:,0].A==cent)[0]]
            centroids[cent,:] = mean(ptsInClust, axis=0) 
    return centroids, clusterAssment
```

### Scala

```
package Kmeans

import scala.io.Source
import scala.collection.mutable.ArrayBuffer
import scala.util.Random

object Kmeans {

  def loadDataSet(fileName: String) = {
    var dataMat = ArrayBuffer[Array[Double]]()
    val fr = Source.fromFile(fileName)
    for (line <- fr.getLines()) {
      val fltLine = line.trim().split("\t").map(_.toDouble)
      dataMat.append(fltLine)
    }
    dataMat.toArray
  }

  def distEclud(vecA: Array[Double], vecB: Array[Double]) = {
    vecA.zip(vecB).map(x => math.pow(x._1 - x._2, 2)).sum
  }

  def randCent(dataSet: Array[Array[Double]], k: Int) = {
    val n = dataSet(0).length
    var centroids = Array.fill(k)(Array.fill(n)(0.0))
    for (j <- 0 to n - 1) {
      val minJ = dataSet.map(_(j)).min
      val rangeJ = dataSet.map(_(j) - minJ).max
      for (i <- 0 to k - 1) {
        Random.setSeed(i)
        centroids(i)(j) = minJ + rangeJ * Random.nextDouble()
      }
    }
    centroids
  }

  def MykMeans(dataSet: Array[Array[Double]],
               k: Int,
               distMeas: (Array[Double], Array[Double]) => Double = distEclud,
               createCent: (Array[Array[Double]], Int) => Array[Array[Double]] = randCent) = {
    val m = dataSet.length
    // 存储聚类结果 （质点，离该质点的距离）
    var clusterAssment = Array.fill(m)((0, 0.0))
    // 随机创建k个质点
    var centroids = createCent(dataSet, k)
    // 用于判断所有的数据点所在的簇是否不再改变
    var clusterChanged = true
    while (clusterChanged) {
      clusterChanged = false
      // 对每个数据点
      for (i <- 0 to m - 1) {
        var minDist = Double.MaxValue
        var minIndex = -1
        // 求与之最近的质点
        for (j <- 0 to k - 1) {
          val distJI = distMeas(centroids(j), dataSet(i))
          if (distJI < minDist) {
            minDist = distJI
            minIndex = j
          }
        }
        if (clusterAssment(i)._1 != minIndex)
          clusterChanged = true
        // 笔者认为这里应该的开方 而不不是平方
        clusterAssment(i) = (minIndex, math.sqrt(minDist))
      }
      centroids.foreach(x => println(x.mkString("|")))
      println("——————————————————————————————————————————————")
      // 更新质点
      for (cent <- 0 to k - 1) {
        val centDataSet = dataSet.zip(clusterAssment).filter(_._2._1 == cent)
        val len = centDataSet.length
        // 保证簇中有数据点，才更新质点
        if (len > 0)
          centroids(cent) = centDataSet.map(_._1).reduce((a, b) => Array(a(0) + b(0), a(1) + b(1))).map(_ / len)
      }
    }
    (centroids, clusterAssment)
  }

  def main(args: Array[String]) {
    val dataSet = loadDataSet("testSet.txt")
    MykMeans(dataSet, 4)
  }
}

/*
运行结果：

2.089206937214362 2.6553132269012165
2.0882914517558113 2.6544689587024406
2.0910374513579297 2.657001342058469
2.090122270415068 2.6561573546865596
——————————————————————————————————————————————
2.089206937214362 2.6553132269012165
-0.8102053124999998 -0.777700796875
2.7227551874999993 3.3823091875
2.090122270415068 2.6561573546865596
——————————————————————————————————————————————
1.3357000833333332 1.7167295833333334
-1.3054142499999999 -1.3533385769230764
2.6742446666666666 3.7408964166666667
2.86828675 2.3065474999999998
——————————————————————————————————————————————
-2.002083882352941 3.0127088823529418
-1.1042668249999996 -2.4955051999999998
2.4340693 3.9126998
3.5058729999999994 1.0631734615384618
——————————————————————————————————————————————
-2.46154315 2.7873755500000006
-1.2792010625 -3.04600065625
2.5212765 3.4946472500000003
3.461318 -0.8204791666666668
——————————————————————————————————————————————
-2.46154315 2.7873755500000006
-3.011694681818182 -3.012386727272727
2.54391447368421 3.2129961052631577
3.0981428421052626 -2.4304122631578946
——————————————————————————————————————————————
-2.46154315 2.7873755500000006
-3.38237045 -2.9473363000000004
2.6265298999999995 3.10868015
2.80293085 -2.7315145999999997
——————————————————————————————————————————————

*/
```

# k-means++

k-means++算法选择初始seeds的基本思想就是：初始的聚类中心之间的相互距离要尽可能的远。该算法的描述是如下:

> 
1.从输入的数据点集合中随机选择一个点作为第一个聚类中心 

  2.对于数据集中的每一个点x，计算它与最近聚类中心(指已选择的聚类中心)的距离D(x) 

  3.选择一个新的数据点作为新的聚类中心，选择的原则是：D(x)较大的点，被选取作为聚类中心的概率较大 

  4.重复2和3直到k个聚类中心被选出来 

  5.利用这k个初始的聚类中心来运行标准的k-means算法

从上面的算法描述上可以看到，算法的关键是第3步，如何将D(x)反映到点被选择的概率上，一种算法如下：

> 
1.先从我们的数据库随机挑个随机点当“种子点” 

  2.对于每个点，我们都计算其和最近的一个“种子点”的距离D(x)并保存在一个数组里，然后把这些距离加起来得到Sum(D(x))。 

  3.然后，再取一个随机值，用权重的方式来取计算下一个“种子点”。这个算法的实现是，先取一个能落在Sum(D(x))中的随机值Random，然后用Random -= D(x)，直到其<=0，此时的点就是下一个“种子点”。 

  4.重复2和3直到k个聚类中心被选出来 

  5.利用这k个初始的聚类中心来运行标准的k-means算法

可以看到算法的第三步选取新中心的方法，这样就能保证距离D(x)较大的点，会被选出来作为聚类中心了。至于为什么原因很简单，如下图 所示：  

![](http://upload-images.jianshu.io/upload_images/1621805-27d869f7cfbdc95e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# bisecting k-meas

为了克服K-Means算法收敛于局部最小值的问题，提出了一种二分K-均值(bisecting K-means) 

算法的伪代码如下：

```
将所有的点看成是一个簇
当簇小于数目k时
    对于每一个簇
        计算总误差
        在给定的簇上进行K-均值聚类,k值为2
        计算将该簇划分成两个簇后总误差
    选择使得误差最小的那个簇进行划分
```

## 实现

首先我们来讲解下代码中的这句：

```python
ptsInCurrCluster = dataSet[nonzero(clusterAssment[:,0].A == i)[0],:]
```

`clusterAssment` 存储的（所属的中心编号，距中心的距离）的列表，`clusterAssment[:,0].A`就是把`clusterAssment`各个`所属的中心编号`抽出来形成数组。`clusterAssment[:,0].A == i`就形成的布尔数组，如果编号等于i，则为true，反之为false。`nonzero(clusterAssment[:,0].A == i)`，会得到非零或者非false的索引列表，列表中第一个元素是按行的索引，第二个元素是按列的索引。所以，`dataSet[nonzero(clusterAssment[:,0].A == i)[0],:]`就得到了`dataSet`中行号与`clusterAssment`中`所属的中心编号`为`i`的行号对应的子集数据。

接下来，我们来分析全部的代码：

```python
def biKmeans(dataSet,k,distMeas=distEclud):
    m = shape(dataSet)[0]
    # 将所有的点看成是一个簇
    # clusterAssment 存储 （所属的中心编号，距中心的距离）的列表
    clusterAssment = mat(zeros((m,2)))
    # centList 存储聚类中心
    centroid0 = mean(dataSet,axis=0).tolist()[0]
    centList = [centroid0]
    for j in range(m):
        clusterAssment[j,1] = distMeas(mat(centroid0),dataSet[j,:]) ** 2
    # 当簇小于数目k时
    while len(centList) < k:
        lowestSSE = inf
        for i in range(len(centList)):
            # 得到dataSet中行号与clusterAssment中所属的中心编号为i的行号对应的子集数据。
            ptsInCurrCluster = dataSet[nonzero(clusterAssment[:,0].A == i)[0],:]
            # 在给定的簇上进行K-均值聚类,k值为2
            centroidMat,splitClustAss = kMeans(ptsInCurrCluster,2,distMeas)
           # 计算将该簇划分成两个簇后总误差
            sseSplit = sum(splitClustAss[:,1])
            sseNotSplit = sum(clusterAssment[nonzero(clusterAssment[:,0].A != i)[0],1])
           # 选择使得误差最小的那个簇进行划分
            if sseSplit + sseNotSplit < lowestSSE:
                bestCentToSplit = i
                bestNewCents = centroidMat.copy()
                bestClustAss = splitClustAss.copy()
                lowestSSE = sseSplit + sseNotSplit

       # 将需要分割的聚类中心下的点进行1划分
       # 新增的聚类中心编号为len(centList)
        bestClustAss[nonzero(bestClustAss[:,0].A == 1)[0],0] = len(centList)
        bestClustAss[nonzero(bestClustAss[:,0].A == 0)[0],0] = bestCentToSplit
        clusterAssment[nonzero(clusterAssment[:,0].A == bestCentToSplit)[0],:] = bestClustAss

        # 更新被分割的聚类中心的坐标
        centList[bestCentToSplit] = bestNewCents[0,:]
        # 增加聚类中心
        centList.append(bestNewCents[1,:])

    return centList,clusterAssment
```

## 测试

```python
import myKmeans
from numpy import *
dataMat = mat(myKmeans.loadDataSet('testSet2.txt'))
centList,myNewAssments = myKmeans.biKmeans(dataMat,3)

print(centList)

# [matrix([[ 2.93386365,  3.12782785]]), matrix([[-2.94737575,  3.3263781 ]]), matrix([[-0.45965615, -2.7782156 ]])]
```

![](http://upload-images.jianshu.io/upload_images/1621805-d92bb4bb6b5b3d84?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

