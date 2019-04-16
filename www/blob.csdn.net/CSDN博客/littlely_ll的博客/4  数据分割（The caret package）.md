# 4. 数据分割（The caret package） - littlely_ll的博客 - CSDN博客





2017年03月05日 14:41:11[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1148
所属专栏：[R语言caret包应用](https://blog.csdn.net/column/details/29369.html)









[1. 简介(The caret package )](http://blog.csdn.net/littlely_ll/article/details/58592298)
[2. 可视化(The caret package)](http://blog.csdn.net/littlely_ll/article/details/58596562)
[3. 预处理(The caret package)](http://blog.csdn.net/littlely_ll/article/details/59106802)
# 4. 数据分割

内容： 

 * 基于输出结果的简单分割 

 * 基于特征变量的分割 

 * 时间序列的数据分割
## 4.1 基于输出结果的简单分割

`createDataPartition`函数用于创建平衡数据的分割。如果函数中的参数y是一个因子向量，则对每一类随机抽样，并且保持数据整体类别的分类。例如：对iris数据按照80/20%的比例分割：

```
library(caret)
set.seed(3456)
trainIndex <- createDataPartition(iris$Species, p = .8, 
                                  list = FALSE, 
                                  times = 1)
head(trainIndex)

##      Resample1
## [1,]         1
## [2,]         2
## [3,]         4
## [4,]         5
## [5,]         6
## [6,]         8

irisTrain <- iris[ trainIndex,]
irisTest  <- iris[-trainIndex,]
```

`list=FALSE`避免返回的数据是一个列表。这个函数含有一个参数`times`，能够一次创建多个分割；数据分割按照整数向量列表的形式返回。同样的，`createResample`函数用于生成boostrap样本，`createFolds`能够从数据集中生成平衡交叉验证小组。

## 4.2 基于特征变量的分割

函数`maxDissim`应用最大相异方法（maximum dissimiarity approach）创建子样本。假设有一个m个样本的数据集A和具有n个样本的一个大数据集B。我们希望从B中抽取和A不同的子样本。为了这样做，对于B中的每一个样本，函数计算与A中每一样本点的相异性。把B中最大相异性的点加到A并继续。在R中有许多方法计算相异性。caret包使用的是`proxy`包计算的。可以从包的手册中查看可用的测量方法。也有很多方法计算哪个样本具有最大相异性。参数`obj`用来设定能够返回标量的函数。caret包含两个函数`minDiss`和`sumDiss`,可以分别用于计算最小最大化和总的相异性。 

作为一个例子，下图展示了Cox2数据中两个化学因素的散点图。从5种混合物进行初始随机抽样，我们能从数据中选择20多种混合物，并且新的混合物与初始的5种具有最大相异性。图中的面板展示了用距离测量和得分函数的多种组合得到的结果。对于这些数据，距离测量相比于能够决定哪种混合物具有最大相异性的得分方法来说有更小的影响。

```
library(mlbench)
data(BostonHousing)

testing <- scale(BostonHousing[, c("age", "nox")])
set.seed(5)
## A random sample of 5 data points
startSet <- sample(1:dim(testing)[1], 5)
samplePool <- testing[-startSet,]
start <- testing[startSet,]
newSamp <- maxDissim(start, samplePool, n = 20)
head(newSamp)
```

下面的可视化展示了数据集（小点），starting样本（大绿点）和增加的另20个样本点的排序。 
![这里写图片描述](https://img-blog.csdn.net/20170305142306611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.3 时间序列的数据分割

时间序列简单随机抽样并不是对时间序列抽样的最好的方法。Hyndman和Athanasopoulos（2013）讨论了rolling forecasting origin技术。caret包 包含了`createTimeSeries`函数，它能创建这种类型的切片。 

这种类型分割的三个参数为： 

 * `initialWindow`：在每一训练样本的连续值的初始数量。 

 * `horizon`：检测样本中连续值的的数目。 

 * `fixedWindow`：逻辑值；如果FALSE，训练集开始于第一个样本，并且训练样本量随数据分割的不同而不同。
作为一个例子，假设我们有20个数据点的时间序列数据。我们固定`initialWindow=5`并且对其他两个参数尝试不同的设置。在下面的这张图中，在每块的行对应不同的数据分割，红色表示样本在训练集中而绿色在检测集中。 
![这里写图片描述](https://img-blog.csdn.net/20170305144025832?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







