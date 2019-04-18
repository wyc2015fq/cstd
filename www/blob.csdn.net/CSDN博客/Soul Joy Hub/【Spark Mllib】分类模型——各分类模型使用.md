# 【Spark Mllib】分类模型——各分类模型使用 - Soul Joy Hub - CSDN博客

2016年06月16日 13:37:30[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：3494
所属专栏：[Spark机器学习](https://blog.csdn.net/column/details/15014.html)



# http://blog.csdn.net/u011239443/article/details/51672245

# [一. 数据集](http://download.csdn.net/detail/u011239443/9550190)

这个数据集源自 Kaggle 比赛,由 StumbleUpon 提供。比赛的问题涉及网页中推荐的页面是短暂(短暂存在,很快就不流行了)还是长久(长时间流行)。

可以查看上面的数据集页面中的简介得知可用的字段。开始四列分别包含 URL 、页面的 ID 、原始的文本内容和分配给页面的类别。接下来 22 列包含各种各样的数值或者类属特征。最后一列为目标值, 1 为长久, 0 为短暂。

除去数据集的第一行，然后进行解析：

```java
// sed 1d train.tsv > train_noheader.tsv
// load raw data
val rawData = sc.textFile("/PATH/train_noheader.tsv")
val records = rawData.map(line => line.split("\t"))
records.first
// Array[String] = Array("http://www.bloomberg.com/news/2010-12-23/ibm-predicts-holographic-calls-air-breathing-batteries-by-2015.html", "4042", ...
```


# [二. 线性模型](https://zh.wikipedia.org/wiki/%E4%B8%80%E8%88%AC%E7%BA%BF%E6%80%A7%E6%A8%A1%E5%9E%8B)


## 1. 提取特征

由于数据格式的问题,我们做一些数据清理的工作,在处理过程中把额外的( " )去掉。数据集中还有一些用 "?" 代替的缺失数据,本例中,我们直接用 0 替换那些缺失数据。在清理和处理缺失数据后,我们提取最后一列的标记变量以及第 5 列到第 25 列的特征矩阵。我们也对数据进行缓存并且统计数据样本的数目。

```java
import org.apache.spark.mllib.regression.LabeledPoint

import org.apache.spark.mllib.linalg.Vectors

val data = records.map { r => val trimmed = r.map(_.replaceAll("\"", "")) val label = trimmed(r.size - 1).toInt val features = trimmed.slice(4, r.size - 1).map(d => if (d == "?") 0.0 else d.toDouble) LabeledPoint(label, Vectors.dense(features)) } data.cache

val numData = data.count

// numData: Long = 7395

// note that some of our data contains negative feature vaues. For naive Bayes we convert these to zeros
```



### [2. 逻辑回归](https://zh.wikipedia.org/wiki/%E9%82%8F%E8%BC%AF%E8%BF%B4%E6%AD%B8)


训练模型

```java
import org.apache.spark.mllib.classification.LogisticRegressionWithSGD
```

迭代次数设置：

```java
val numIterations = 10
```

建立模型：

```java
val lrModel = LogisticRegressionWithSGD.train(data, numIterations)
```

预测：

```java
// make prediction on a single data point
val dataPoint = data.first
// dataPoint: org.apache.spark.mllib.regression.LabeledPoint = LabeledPoint(0.0, [0.789131,2.055555556,0.676470588, ...
val prediction = lrModel.predict(dataPoint.features)
// prediction: Double = 1.0
val trueLabel = dataPoint.label
// trueLabel: Double = 0.0
val predictions = lrModel.predict(data.map(lp => lp.features))
predictions.take(5)
// res1: Array[Double] = Array(1.0, 1.0, 1.0, 1.0, 1.0)
```
更多可参阅《[【Spark Mllib】逻辑回归——垃圾邮件分类器与maven构建独立项目](http://blog.csdn.net/u011239443/article/details/51655469)》

### [3. 线性支持向量机](https://zh.wikipedia.org/wiki/%E6%94%AF%E6%8C%81%E5%90%91%E9%87%8F%E6%9C%BA)


训练模型

```java
import org.apache.spark.mllib.classification.SVMWithSGD
```

建立模型：

val svmModel = SVMWithSGD.train(data, numIterations)

# [三. 朴素贝叶斯模型](https://zh.wikipedia.org/wiki/%E6%9C%B4%E7%B4%A0%E8%B4%9D%E5%8F%B6%E6%96%AF%E5%88%86%E7%B1%BB%E5%99%A8)

提取特征：

在对数据集做进一步处理之前,我们发现数值数据中包含负的特征值。我们知道,朴素贝叶斯模型要求特征值非负,否则碰到负的特征值程序会抛出错误。因此,需要为朴素贝叶斯模型构建一份输入特征向量的数据,将负特征值设为 0 :

```java
val nbData = records.map { r =>
	val trimmed = r.map(_.replaceAll("\"", ""))
	val label = trimmed(r.size - 1).toInt
	val features = trimmed.slice(4, r.size - 1).map(d => if (d == "?") 0.0 else d.toDouble).map(d => if (d < 0) 0.0 else d)
	LabeledPoint(label, Vectors.dense(features))
}
```

训练模型:

```java
import org.apache.spark.mllib.classification.NaiveBayes
```


```java
// note we use nbData here for the NaiveBayes model training
val nbModel = NaiveBayes.train(nbData)
```


# [四. 决策树](https://zh.wikipedia.org/wiki/%E5%86%B3%E7%AD%96%E6%A0%91)

```java
import org.apache.spark.mllib.tree.DecisionTree
import org.apache.spark.mllib.tree.configuration.Algo
import org.apache.spark.mllib.tree.impurity.Entropy
```

设置决策树深度：

```java
val maxTreeDepth = 5
```


```java
val dtModel = DecisionTree.train(data, Algo.Classification, Entropy, maxTreeDepth)
```

可参阅 ：[《【Spark Mllib】决策树，随机森林——预测森林植被类型](http://blog.csdn.net/u011239443/article/details/51858825)[》](http://blog.csdn.net/u011239443/article/details/51858825)

[](http://blog.csdn.net/u011239443/article/details/51858825)

[](http://blog.csdn.net/u011239443/article/details/51858825)![](https://img-blog.csdn.net/20170513093407300?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


