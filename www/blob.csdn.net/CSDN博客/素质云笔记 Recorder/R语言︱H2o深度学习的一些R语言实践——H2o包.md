
# R语言︱H2o深度学习的一些R语言实践——H2o包 - 素质云笔记-Recorder... - CSDN博客

2016年04月22日 12:39:04[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：11241




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
R语言H2o包的几个应用案例

笔者寄语：受启发想了解H2o平台的一些R语言实现，网上已有一篇H2o的demo文件。笔者在这多贴一些案例，并且把自己实践的一些小例子贴出来。
关于H2o平台长啥样，可以看[H2o的官网](http://open.h2o.ai/?_ga=1.197886758.886141007.1461291835)，关于深度学习长啥样，可以看一些教程，比如[ParallelR博客之中的解析](http://www.parallelr.com/r-deep-neural-network-from-scratch/)。
下面主要是贴几个案例，让大家看看。

————————————————————————————————————————————————————————————

**Matt︱R语言调用深度学习架构系列引文**

[R语言︱H2o深度学习的一些R语言实践——H2o包](http://blog.csdn.net/sinat_26917383/article/details/51219025)

# R用户的福音︱TensorFlow：TensorFlow的R接口


# [mxnet：结合R与GPU加速深度学习](http://blog.csdn.net/sinat_26917383/article/details/52737901)



# 碎片︱R语言与深度学习

[sparklyr包：实现Spark与R的接口，会用dplyr就能玩Spark](http://blog.csdn.net/sinat_26917383/article/details/52749138)
—————————————————————————————————————


本文中介绍的**H2o包在调用的过程主要有以下简要步骤**：
连接、搭建H2o环境（heo.init()）——数据转换成h2o格式（as.h2o）——模型拟合（h2o.deeplearning）——预测（h2o.predict）——数据呈现（h2o.performance）。


# 一、H2o包的demo(glm)
网上已经有了，博客笔者看了并做了简单的中文注释。详情可以见[std1984博客](http://blog.itpub.net/16582684/viewspace-1255976/)。



# 二、来
# [自ParallelR博客的案例](http://www.parallelr.com/r-dnn-parallel-acceleration/)
博客中主要是为了说明深度学习要比其他普通学习方法的精度高。数据是H2o网站数据，笔者windows系统下没有能够下载到该数据，所以后续的分析都没有办法继续进行了。
`library(h2o)
# single thread
h2o.init()
#连接h2o平台

train_file <- "https://h2o-public-test-data.s3.amazonaws.com/bigdata/laptop/mnist/train.csv.gz"
test_file <- "https://h2o-public-test-data.s3.amazonaws.com/bigdata/laptop/mnist/test.csv.gz"
train <- h2o.importFile(train_file)
test  <- h2o.importFile(test_file)
# To see a brief summary of the data, run the following command
summary(train)
summary(test)
y <- "C785"
x <- setdiff(names(train), y)
# We encode the response column as categorical for multinomial
#classification
train[,y] <- as.factor(train[,y])
test[,y]  <- as.factor(test[,y])
# Train a Deep Learning model and valid
system.time(
  model_cv <- h2o.deeplearning(x = x,
                               y = y,
                               training_frame = train,
                               distribution = "multinomial",
                               activation = "Rectifier",
                               hidden = c(32),
                               l1 = 1e-5,
                               epochs = 200)
)`

# 三、最简单的案例——基于iris数据集的深度学习

本案例主要来自h2o官方手册中，h2o.deeplearning包的示例，比较简单易懂。如果你想看预测的数据可以用as.data.frame来变成R能识别的数据框格式。
`##参考来自：h2o官方手册,h2o.deeplearning函数的示例
library(h2o)
h2o.init()
iris.hex <- as.h2o(iris)
iris.dl <- h2o.deeplearning(x = 1:4, y = 6, training_frame = iris.hex)  #模型拟合
# now make a prediction
predictions <- h2o.predict(iris.dl, iris.hex)          #预测
as.data.frame(predictions)                             #预测数据变成数据框
performance = h2o.performance(model = iris.dl)
print(performance)`

输出的结果长成下面这个样子。
大概构成是：模型评价指标+混淆矩阵+一些指标的阈值（这个是啥？？）
看到混淆矩阵，你就差不多懂了~
`> print(performance)
H2OBinomialMetrics: deeplearning
** Reported on training data. **
Description: Metrics reported on full training frame
MSE:  0.01030833
R^2:  0.9536125
LogLoss:  0.05097025
AUC:  1
Gini:  1
Confusion Matrix for F1-optimal threshold:
         0  1    Error    Rate
0      100  0 0.000000  =0/100
1        0 50 0.000000   =0/50
Totals 100 50 0.000000  =0/150
Maximum Metrics: Maximum metrics at their respective thresholds
                      metric threshold    value idx
1                     max f1  0.983179 1.000000  49
2                     max f2  0.983179 1.000000  49
3               max f0point5  0.983179 1.000000  49
4               max accuracy  0.983179 1.000000  49
5              max precision  0.999915 1.000000   0
6                 max recall  0.983179 1.000000  49
7            max specificity  0.999915 1.000000   0
8           max absolute_MCC  0.983179 1.000000  49
9 max min_per_class_accuracy  0.983179 1.000000  49
Gains/Lift Table: Extract with `h2o.gainsLift(<model>, <data>)` or `h2o.gainsLift(<model>, valid=<T/F>, xval=<T/F>)``

**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


