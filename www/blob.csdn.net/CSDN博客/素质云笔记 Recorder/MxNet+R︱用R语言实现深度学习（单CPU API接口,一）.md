
# MxNet+R︱用R语言实现深度学习（单CPU-API接口,一） - 素质云笔记-Recorder... - CSDN博客

2016年12月20日 14:32:23[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4828


MxNet有了亚马逊站台之后，声势大涨，加之接口多样化，又支持R语言所以一定要学一下。而且作为R语言的fans，为啥咱们R语言就不能上深度学习嘞~

——————————————————————————————

# 一、MxNet对R的API接口

MxNet提供给了R一个API接口，但是这个API接口也不是万能的，不同的操作系统有着不同的使用功能。

**1、****Windows/Mac用户——只能CPU训练**

可以通过下面的代码安装预编译的版本。这个版本会每周进行预编译，不过为了保证兼容性，只能使用CPU训练模型。
**这个接口其不想pycaffe一样，调用本地的caffe，而是一个远端MxNet社团在维护的一个版本，不能算是正式的MxNet**
`install.packages("drat", repos="https://cran.rstudio.com")
drat:::addRepo("dmlc")
install.packages("mxnet")`

**2、****Linux用户——兼GPU版本**

详情可参考官网链接：http://mxnet.io/get_started/setup.html

# 二、官方案例一：多层感知器MLP
参考博文：[mxnet：结合R与GPU加速深度学习](http://cos.name/2016/04/mxnet-r/?utm_source=tuicool&utm_medium=referral)
案例数据：mlbench包自带数据集
使用package：mxnet（之前博主在想，要用mxnet是否需要下载MxNet，但是这个API还是很给力的...）

## 1、准备数据
`require(mlbench)
require(mxnet)
data(Sonar, package="mlbench")
Sonar[,61] = as.numeric(Sonar[,61])-1
train.ind = c(1:50, 100:150)
train.x = data.matrix(Sonar[train.ind, 1:60])
train.y = Sonar[train.ind, 61]
test.x = data.matrix(Sonar[-train.ind, 1:60])
test.y = Sonar[-train.ind, 61]`
## 2、mx.mlp函数模型训练
`mx.set.seed(0)
model <- mx.mlp(train.x, train.y, hidden_node=10, out_node=2,out_activation="softmax", num.round=20, array.batch.size=15, learning.rate=0.07, momentum=0.9, eval.metric=mx.metric.accuracy)`其中：
1、mx.set.seed(0)，随机数设置不是之前的set.seed，因为如果要并行、分布式的话，需要一个更快、更效率的随机数生成器，于是重写了一个更好的
2、函数mx.mlp：mx.mlp(data训练自变量x,label训练因变量y,每个隐藏层的大小hidden_node，输出层的结点数out_node,激活函数类型out_activation,num.round,学习率learning.rate,动量momentum)
激活函数类型out_activation类型：softmax/tanh

其他函数类型跟mx.model.FeedForward.create，前馈模型一致：
num.round，迭代次数，默认10
array.batch.size，默认128，输入数组个数
eval.metric：评估函数

## 3、模型预测
`preds = predict(model, test.x)
pred.label = max.col(t(preds))-1
table(pred.label, test.y)`
predict返回的两个概率：成为0的概率，成为1的概率，max.col找到了成为0/1，哪个概率值最大，就是哪个

——————————————————————————————


# 三、官方案例二：利用Symbol系统自定义节点——构造简单回归

参考：http://mxnet.io/tutorials/r/symbol.html
一般情况下，不同的深度学习架构都需要自己构建节点，而Tensorflow对节点十分看重，把tensor张量作为数据输入。


## 1、数据载入

`data(BostonHousing, package="mlbench")
train.ind = seq(1, 506, 3)
train.x = data.matrix(BostonHousing[train.ind, -14])
train.y = BostonHousing[train.ind, 14]
test.x = data.matrix(BostonHousing[-train.ind, -14])
test.y = BostonHousing[-train.ind, 14]`

## 2、利用Symbol系统自定义节点

mxnet提供了一个叫做“Symbol”的系统，从而使我们可以定义结点之间的连接方式与激活函数等参数。
下面是一个定义没有隐藏层神经网络，模拟回归的简单例子：

`# 定义输入数据
data <- mx.symbol.Variable("data")
# 完整连接的隐藏层
# data: 输入源
# num_hidden: 该层的节点数
fc1 <- mx.symbol.FullyConnected(data, num_hidden=1)
# 针对回归任务，定义损失函数
lro <- mx.symbol.LinearRegressionOutput(fc1)`
输入数据mx.symbol.Variable，然后设置了一个节点mx.symbol.FullyConnected，设置了节点损失函数mx.symbol.LinearRegressionOutput
回归与分类的差别主要在于输出层的损失函数。这里我们使用了平方误差fc1（L1损失）来训练模型。


## 3、模型训练

`mx.set.seed(0)
model <- mx.model.FeedForward.create(lro, X=train.x, y=train.y, ctx=mx.cpu(), num.round=50, array.batch.size=20, learning.rate=2e-6, momentum=0.9, eval.metric=mx.metric.rmse)`ctx控制使用CPU还是GPU，ctx=mx.cpu()，ctx=mx.gpu()
eval.metric评估函数，包括”accuracy”，”rmse”，”mae” 和 “rmsle”

## 4、如何写新的评估函数
`#定义一个函数
demo.metric.mae <- mx.metric.custom("mae", function(label, pred) {
  res <- mean(abs(label-pred))
  return(res)
})
#直接在eval.metric中体现
mx.set.seed(0)
model <- mx.model.FeedForward.create(lro, X=train.x, y=train.y, ctx=mx.cpu(), num.round=50, array.batch.size=20, learning.rate=2e-6, momentum=0.9, eval.metric=demo.metric.mae)`


