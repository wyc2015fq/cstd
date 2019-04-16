# Online Anomaly Prediction for Robust Cluster System - 一个跳popping的quant的博客 - CSDN博客





2018年05月17日 14:36:47[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1777标签：[论文																[异常点检测																[马尔科夫																[贝叶斯](https://so.csdn.net/so/search/s.do?q=贝叶斯&t=blog)
个人分类：[quantitative trading](https://blog.csdn.net/FrankieHello/article/category/7166871)

所属专栏：[一个用人工智能做量化的小白的进阶之路](https://blog.csdn.net/column/details/20644.html)](https://so.csdn.net/so/search/s.do?q=马尔科夫&t=blog)




题目：《Online Anomaly Prediction for Robust Cluster System》

时间：2009

会议：IEEE International Conference on Data Engineering

### 简介：

这是一篇发表在一个顶级会议上的会议论文。主要的工作是通过将stream-based的数据进行异常点的预测，文中也提到了这是首篇对stream-based data进行预测的paper。另外，当前对异常点研究的成果并不多，大多数也都是基于statistics的，而这篇则是用到了 Markov chain和navie Bayesian，毕竟是09年的paper，所以技术并不是很新，所以放在现在可以借鉴和改进的地方很有很多。

### 应用技术：

Markov chain和navie Bayesian classification

检测的是IBM的集群中的数据

### 模型框架：

![](https://img-blog.csdn.net/20180515210607873?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

从图中可以很直观的看出整个模型的框架：

1、将当前的特征点（多维）映射到坐标中，通过Markov Chain可以得到这个样本点中每个特征的概率分布，从而得到一个分布的region。其中，由于每个样本点可能有多个特征，而每个特征的范围可能是连续的，而Markov的状态空间则是要求有限的状态空间，所以文中使用了一个equ-depth的方式来划分这个范围，从而将连续的数据转换为离散的。

2、通过Markov Chain得到每个样本点的特征的概率分布后，通过下面的公式对特征的概率分布进行Bayesian计算。

![](https://img-blog.csdn.net/20180515211500959)

然后比较在anmoly和nomaly之间哪个confidence更高。

![](https://img-blog.csdn.net/20180515211718326)

算法的流程如下：

![](https://img-blog.csdn.net/20180515211822265)

首先是对bayesian模型的训练。

通过对labeled的数据进行navi bayesian的训练，可以计算得到每个特征点中的各个metric对分类结果（anomaly、normaly）的概率。


![](https://img-blog.csdn.net/20180515211839913)

后面的实验部分通过了三个指标进行了验证：

detection rate（检测率）

false alarm rate（错误警报率）

distance statistics（警报时隔）




下图就是对future data和current data的detection。

![](https://img-blog.csdn.net/20180517143203742)

Ntp是在指定的时间周期中出现正确的警报的数量。

Nfp是在指定的时间周期中出现错误的警报的数量。




time prediction diff的意思是异常出现和发出警报的时间差。





![](https://img-blog.csdn.net/20180517142244348?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### ![](https://img-blog.csdn.net/20180517142256938?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 总结：

1、文中的一个创新之处是在通过Bayesian对future data分类的时候，没有是对data进行分类，而是对data中的特征分布进行分类的，这样更能发现一些潜在的特征。

2、由于发表的时间较早，所以放在现在来看的话，可以用一些更先进的方法例如DL和Mate Learning等技术来对内部进行一些改进。](https://so.csdn.net/so/search/s.do?q=异常点检测&t=blog)](https://so.csdn.net/so/search/s.do?q=论文&t=blog)




