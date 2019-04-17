# 神经网络-pnorm - xmdxcsj的专栏 - CSDN博客





2016年12月02日 21:12:08[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1885








## 发展历史

### 1.bagging算法

Leo Breiman[1]在1994年提出了bagging算法，给定一个大小为 n的训练集D，Bagging算法从中均匀、有放回地选出 m个大小为 n’的子集$D_{i}$，作为新的训练集。在这m个训练集上使用分类、回归等算法，则可得到m个模型，再通过取平均值、取多数票等方法，即可得到Bagging的结果。

### 2.dropout

Hinton[2]在2012年提出了dropout的训练方法来缓解模型训练中的过拟合的问题，和bagging思想类似，都是使用训练集的不同子集训练多个model。不同之处在于dropout每一步训练针对同一个模型的不同的参数变量。 

当在参数空间使用较大步长的时候，dropout会比较有效；另外一个问题是dropout的模型平均在应用到deep models的时候，只是一种近似。 

一篇对dropout[理解比较好的博客](http://blog.csdn.net/stdcoutzyx/article/details/49022443)
### 3.maxout

dropout本质上是在模型训练过程中使用的一种方法，而Ian J Goodfellow[3]在2013年提出了maxout networks，将这种方法体现在了网络结构上面。 

论文[3]中的公式比较模糊，详细的公式含义以及网络图形变化[这篇博客](http://www.cnblogs.com/tornadomeet/p/3428843.html). 

maxout作为激活函数，也是一种非线性变换，在一定条件下可以拟合所有的凸函数。但是参数量也因为k个affine feature maps的引入而成倍增长。
### 4.pnorm

2014年Xiaohui Zhang[4]提出了两种新的maxout units，分别是p-norm和soft-maxout，发现p-norm在语音识别任务上面取得了较好的效果。

## pnorm和soft-maxout

假设maxout的group size为G=5，maxout units的个数为K=500，那么maxout units的输入将会是2500个units。 

使用maxout： 


$y=max_{i=1}^G x_i$

使用soft-maxout： 


$y=log\sum_{i=1}^Gexp(x_i)$

使用p-norm(一般取p=2)： 


$y=||x||_p=(\sum_i |x_i|^p)^{1/p}$

使用ReLU： 


$y=max(x,0)$

上面的非线性函数没有对输出进行限制，从而会导致训练的不稳定性。为了解决这个问题，引入了Normalization layer: 

对于所有maxout units的值进行规整，对于第i个maxout unit值$y_i,1\le i \le K$，首先计算方差： 


$\delta=\sqrt{\frac{1}{K}\sum_i y_i^2}$

然后对每一个$y_i$进行规整： 


$z_i =  \begin{cases}    y_i       & \quad \delta \le 1\\    y_i/\delta  & \quad \delta \gt 1\\  \end{cases}$
## 参考文献

[1]. Bagging Predictors 

[2]. Improving neural networks by preventing co-adaptation of feature detectors 

[3]. maxout networks 

[4]. IMPROVING DEEP NEURAL NETWORK ACOUSTIC MODELS USING GENERALIZED MAXOUT NETWORKS






