# An Artificial Neural Network-based Stock Trading Sysytem Using Technical Analysis and Big Data Frame - 一个跳popping的quant的博客 - CSDN博客





2018年03月21日 15:48:00[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1841标签：[量化交易																[论文																[智能交易																[算法交易																[机器学习](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[machine learning](https://blog.csdn.net/FrankieHello/article/category/7223906)

所属专栏：[一个用人工智能做量化的小白的进阶之路](https://blog.csdn.net/column/details/20644.html)](https://so.csdn.net/so/search/s.do?q=算法交易&t=blog)




不经意看到的一篇paper，整理一下：


题目：

An Artificial Neural Network-based Stock Trading Sysytem Using Technical Analysis and Big Data Frame work

发表时间：

2017/12




### 摘要：

这篇论文所做的工作主要就是利用股票中的一些技术指标，基于神经网络做的一套交易系统，训练和测试用的是Dow30stocks(道琼斯30只股票 1997-2007 for test, 2007-2017 for train)。另外在训练和创建模型的时候，用到的是Apache Spark框架下的MLlib的library。模型的目的是用来预测股市中的买点和卖点。




### 应用技术：

MLP（Multilayer Perceptron）多层感知机。

NN神经网络。

stock market technical技术指标。

常用来分析股市的技术指标有很多，该paper用到了三个较为常用的指标：RSI、MACD、Williams%R。

RSI是一种相对强弱指标，具体计算方式是：

N日内上涨的幅度平均值/N日内上涨的幅度平均值+N日内下跌总幅度的平均值

MACD是指数平滑移动平均线。

Williams%R是威廉指标，原理是用当日收盘价在最近一段时间股价分布的相对位置来描述超买和超卖程度。




### 模型的结构：

![](https://img-blog.csdn.net/20180321133929740)


### 模型的训练和测试的流程是：

![](https://img-blog.csdn.net/20180321152226114)


梳理一下就是：

1、先处理数据集，将每行数据集通过常用的一些技术指标打上标签。

2、再计算每行数据的RSI、WilliamR和MACD指标，最后将数据集分成测试集和训练集。

3、将打好标签的数据集输入到模型中进行训练，训练好后用测试集进行测试。




使用时，就是根据输出预测的标签进行判断是买点还是卖点或是持仓，其中当两个连续的信号是相同时，也就是说第一个点是个买点信号，第二个点也是买点信号，那么第二点就没有作用了。


最后的测试效果（加入了止损）：

![](https://img-blog.csdn.net/20180321153130983)





### 总结：

整体上其实就是一个监督性的学习过程，整篇论文并没有什么创新点，应用的技术也不是很多。

个人为什么整理这篇论文，主要是一下几个方面：

1、觉得这篇paper首先结构框架很简单，道理说的也很明白，模型实现起来也没有什么困难。

2、就是它在应用时结合股市或者说是金融市场的一些特性（技术指标），没有单纯只是把价格塞到模型中去，虽然一切的信息都包含在这个价格当中，但是通过这种的指标分析可以更加具体些，所以整个问题就转换为了通过指标来预测买点和卖点，而不是说通过价格来预测价格。

3、它通过将数据打标签的形式，将整个问题又转换为了一个监督性学习中的分类问题，往常的很多做预测的都是偏向价格回归的。](https://so.csdn.net/so/search/s.do?q=智能交易&t=blog)](https://so.csdn.net/so/search/s.do?q=论文&t=blog)](https://so.csdn.net/so/search/s.do?q=量化交易&t=blog)




