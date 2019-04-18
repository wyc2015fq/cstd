# 《机器学习实战（Scala实现）》（二）——k-邻近算法 - Soul Joy Hub - CSDN博客

2017年03月18日 14:41:08[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：3238
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



# 算法流程

1.计算中的set中每一个点与Xt的距离。 

2.按距离增序排。 

3.选择距离最小的前k个点。 

4.确定前k个点所在的`label`的出现频率。 

5.返回频率最高的`label`作为测试的结果。

# 实现

## python

```python
# -*- coding: utf-8 -*-  
'''
Created on 2017年3月18日

@author: soso
'''
from numpy import *
import operator

def createDataSet():
    group = array([[1.0, 1.1], [1.0, 1.0], [0, 0], [0, 0.1]])
    labels = ['A', 'A', 'B', 'B']
    return group, labels

def classify0(inX, dataSet, labels, k):
    dataSetSize = dataSet.shape[0]
    # 函数形式： tile(A，rep)
    # 功能：重复A的各个维度
    # 参数类型：
    # - A: Array类的都可以
    # - rep：A沿着各个维度重复的次数
    diffMat = tile(inX, (dataSetSize, 1)) - dataSet
    sqDiffMat = diffMat ** 2
    # 当加入axis=1以后就是将一个矩阵的每一行向量相加
    sqDistances = sqDiffMat.sum(axis=1)
    distance = sqDistances * 0.5
    # argsort函数返回的是数组值从小到大的索引值
    sortedDistIndicies = distance.argsort()
    classCount = {}
    for i in range(k):
        votelabel = labels[sortedDistIndicies[i]]
        classCount[votelabel] = classCount.get(votelabel, 0) + 1
    sortedClassCount = sorted(classCount.iteritems(), key=operator.itemgetter(1), reverse=True)
    return sortedClassCount[0][0]
```

## Scala

```
import scala.collection.mutable.Map

object kNN {

  def getGroup(): Array[Array[Double]] = {
    return Array(Array(1.0, 1.1), Array(1.0, 1.0), Array(0, 0), Array(0, 0.1))
  }
  def getLabels(): Array[Char] = {
    return Array('A', 'A', 'B', 'B')
  }

  def classify0(inX: Array[Double], dataSet: Array[Array[Double]], labels: Array[Char], k: Int): Char = {
    val dataSetSize = dataSet.length
    val sortedDisIndicies = dataSet.map { x =>
      val v1 = x(0) - inX(0)
      val v2 = x(1) - inX(1)
      v1 * v1 + v2 * v2
    }.zipWithIndex.sortBy(f => f._1).map(f => f._2)
    var classsCount: Map[Char, Int] = Map.empty
    for (i <- 0 to k - 1) {
      val voteIlabel = labels(sortedDisIndicies(i))
      classsCount(voteIlabel) = classsCount.getOrElse(voteIlabel, 0) + 1
    }
    classsCount.toArray.sortBy(f => -f._2).head._1
  }
  def main(args: Array[String]) {
    println(classify0(Array(0, 0), getGroup(), getLabels(), 3))
  }
}
```

![这里写图片描述](https://img-blog.csdn.net/20170604203345047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

