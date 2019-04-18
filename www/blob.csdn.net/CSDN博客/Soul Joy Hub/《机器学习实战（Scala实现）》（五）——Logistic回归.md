# 《机器学习实战（Scala实现）》（五）——Logistic回归 - Soul Joy Hub - CSDN博客

2017年04月01日 20:12:35[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：2369
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



# 原理

逻辑回归的一种二分类。我们先来看下以下公式：

$\large z = w_0x_0 + w_1x_1 + ... + w_nx_n$

`z`是`Sigmoid`函数的输入：

$\large \sigma(z) = \frac{1}{1+e^{-z}}$

我们可以看出上式子，当`z = 0`时，$\sigma(z) = 0.5$;当`z`不断的减小时，$\sigma(z) $会不断的趋于0;当`z`不断的增大时，$\sigma(z) $会不断的趋于1。

所以我们就可以利用`Sigmoid`函数，当$\sigma(z)  < 0.5 $，我们就将`label`判断为`0`;当$\sigma(z)  > 0.5 $，我们就将`label`判断为`1`。

`x`是特征的值，而逻辑回归模型训练的就是最优的权值`w`。

## 梯度向上

我们可以使用偏导来确定权值优化的最快的方向，例如二维：

`x`移动方向为$\large  \frac{\vartheta f(x,y)}{\vartheta x}$

`y`移动方向为$\large  \frac{\vartheta f(x,y)}{\vartheta y	}$

# 训练算法

## Python

```python
def loadDataSet():
    dataMat = []; labelMat = []
    fr = open('testSet.txt')
    for line in fr.readlines():
        lineArr = line.strip().split()
        dataMat.append([1.0, float(lineArr[0]), float(lineArr[1])])
        labelMat.append(int(lineArr[2]))
    return dataMat,labelMat

def sigmoid(inX):
    return 1.0/(1+exp(-inX))
```

### 随机梯度上升
- 所有的权值初始化为1
- 对数据集中的每个一个样本 
- 计算该样本的梯度
- 使用$alpha * gradient$更新权值

- 返回权值

这里我们给出的是优化后的代码：

```python
# numIter 迭代次数 默认150次
def stocGradAscent1(dataMatrix, classLabels, numIter=150):
    m,n = shape(dataMatrix)
    weights = ones(n)   
    for j in range(numIter):
        dataIndex = range(m)
        for i in range(m):
            # 每次迭代都更新alpha
            # 可以发现alpha值越来越小
            # 确保每次移动的步长越来越小
            alpha = 4/(1.0+j+i)+0.0001  
            # 随机得到某行  
            randIndex = int(random.uniform(0,len(dataIndex)))
            # 预测结果
            h = sigmoid(sum(dataMatrix[randIndex]*weights))
            # 得到误差
            error = classLabels[randIndex] - h
            # 更新权值
            weights = weights + alpha * error * dataMatrix[randIndex]
            # 移除该行
            del(dataIndex[randIndex])
    return weights
```

### 测试算法

```python
def classifyVector(inX, weights):
    prob = sigmoid(sum(inX*weights))
    if prob > 0.5: return 1.0
    else: return 0.0
```

# Scala

```
package Logistic

import scala.io.Source
import scala.collection.mutable.ArrayBuffer
import scala.util.Random

object Logistic {

  def loadDataSet() = {
    val dataMat = new ArrayBuffer[Array[Double]]
    val labelMat = new ArrayBuffer[Double]
    val fr = Source.fromFile("LogisticTestSet.txt")
    for (line <- fr.getLines()) {
      val lineArr = line.trim().split("\t").map(_.toDouble)
      dataMat.append(Array(1.0, lineArr(0), lineArr(1)))
      labelMat.append(lineArr(2))
    }
    (dataMat.toArray, labelMat.toArray)
  }

  def sigmoid(inX: Double) = {
    1.0 / (1 + math.exp(-inX))
  }

  def stocGradAscent1(dataMatrix: Array[Array[Double]], classLabels: Array[Double], numIter: Int = 150) = {
    val m = dataMatrix.length
    val n = dataMatrix(0).length
    var weights = Array.fill(n)(1.0)
    for (j <- 1 to numIter) {
      var dataIndex: ArrayBuffer[Int] = ArrayBuffer.empty
      for (loc <- 0 to m - 1) dataIndex.append(loc)
      for (i <- 0 to m - 1) {
        val alpha = 4 / (1.0 + j + i) + 0.0001
        val randIndex = Random.nextInt(dataIndex.length)
        //go to 0 because of the constant
        val rowZipWeight = dataMatrix(dataIndex(randIndex)).zip(weights)
        val h = sigmoid(rowZipWeight.map(x => x._1 * x._2).sum)
        val error = classLabels(randIndex) - h
        weights = rowZipWeight.map(x => x._1 + alpha * error * x._2)
        dataIndex.remove(randIndex)
      }
    }
    weights
  }

  def classifyVector(inX: Array[Double], weights: Array[Double]) = {
    val prob = sigmoid(inX.zip(weights).map(x => x._1 * x._2).sum)
    if (prob > 0.5) 1.0 else 0.0
  }

  def main(args: Array[String]): Unit = {
    val dataSet = loadDataSet()
    val dataMatrix = dataSet._1
    val classLabels = dataSet._2
    val weights = stocGradAscent1(dataMatrix, classLabels,500)
    val result = dataMatrix.map(x => classifyVector(x, weights))
    println(result.mkString("\n"))
    println(result.zip(classLabels).filter(x => x._1 == x._2).length.toDouble / classLabels.length)
  }
}

/*
运行结果：

0.0
1.0
0.0
0.0
0.0
0.0
0.0
1.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
1.0
0.0
0.0
1.0
0.0
1.0
1.0
0.0
0.0
1.0
0.0
0.0
0.0
0.0
0.0
1.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
1.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
0.0
1.0
0.0
0.0
0.0
0.0
1.0
0.0
0.0
0.0
0.0
0.0
1.0
0.0
0.0
0.0
0.0
1.0
0.0
0.0
0.0
1.0
1.0
0.0
0.0
1.0
1.0
1.0
0.0
0.0
1.0
1.0
0.0
0.0
1.0
1.0
0.0
0.0
1.0
0.0
0.0
0.9973131930172848 -1.6933548385975405 -0.5646293142100435
0.68
*/
```

![这里写图片描述](https://img-blog.csdn.net/20170609200625289?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

