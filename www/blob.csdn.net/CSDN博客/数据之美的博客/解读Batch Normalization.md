# 解读Batch Normalization - 数据之美的博客 - CSDN博客
2017年08月29日 01:12:42[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：181
个人分类：[深度学习/机器学习](https://blog.csdn.net/oppo62258801/article/category/6739087)

目录[(?)](http://blog.csdn.net/elaine_bao/article/details/50890491#)[[-]](http://blog.csdn.net/elaine_bao/article/details/50890491#)
- [1-Motivation](http://blog.csdn.net/elaine_bao/article/details/50890491#t0)
- [2-Normalization via Mini-Batch Statistics](http://blog.csdn.net/elaine_bao/article/details/50890491#t1)- [测试](http://blog.csdn.net/elaine_bao/article/details/50890491#t2)
- [BN before or after Activation](http://blog.csdn.net/elaine_bao/article/details/50890491#t3)
- [3-Experiments](http://blog.csdn.net/elaine_bao/article/details/50890491#t4)
本文转载自：[http://blog.csdn.net/shuzfan/article/details/50723877](http://blog.csdn.net/shuzfan/article/details/50723877)
本次所讲的内容为Batch Normalization，简称BN，来源于[《Batch Normalization: Accelerating Deep Network Training by Reducing Internal Covariate Shift》](http://arxiv.org/pdf/1502.03167.pdf)，是一篇很好的paper。
### 1-Motivation
作者认为：网络训练过程中参数不断改变导致后续每一层输入的分布也发生变化，而学习的过程又要使每一层适应输入的分布，因此我们不得不降低学习率、小心地初始化。作者将分布发生变化称之为 internal covariate shift。
大家应该都知道，我们一般在训练网络的时会将输入减去均值，还有些人甚至会对输入做白化等操作，目的是为了加快训练。为什么减均值、白化可以加快训练呢，这里做一个简单地说明：
首先，图像数据是高度相关的，假设其分布如下图a所示(简化为2维)。由于初始化的时候，我们的参数一般都是0均值的，因此开始的拟合y=Wx+b，基本过原点附近，如图b红色虚线。因此，网络需要经过多次学习才能逐步达到如紫色实线的拟合，即收敛的比较慢。如果我们对输入数据先作减均值操作，如图c，显然可以加快学习。更进一步的，我们对数据再进行去相关操作，使得数据更加容易区分，这样又会加快训练，如图d。 
![这里写图片描述](https://img-blog.csdn.net/20160223160020989)
白化的方式有好几种，常用的有PCA白化：即对数据进行PCA操作之后，在进行方差归一化。这样数据基本满足0均值、单位方差、弱相关性。作者首先考虑，对每一层数据都使用白化操作，但分析认为这是不可取的。因为白化需要计算协方差矩阵、求逆等操作，计算量很大，此外，反向传播时，白化操作不一定可导。于是，作者采用下面的Normalization方法。
### 2-Normalization via Mini-Batch Statistics
数据归一化方法很简单，就是要让数据具有0均值和单位方差，如下式： 
![这里写图片描述](https://img-blog.csdn.net/20160223160039062)
但是作者又说如果简单的这么干，会降低层的表达能力。比如下图，在使用sigmoid激活函数的时候，如果把数据限制到0均值单位方差，那么相当于只使用了激活函数中近似线性的部分，这显然会降低模型表达能力。 
![这里写图片描述](https://img-blog.csdn.net/20160223160053859)
为此，作者又为BN增加了2个参数，用来保持模型的表达能力。 
于是最后的输出为： 
![这里写图片描述](https://img-blog.csdn.net/20160223160123115)
上述公式中用到了均值E和方差Var，需要注意的是理想情况下E和Var应该是针对整个数据集的，但显然这是不现实的。因此，作者做了简化，用一个Batch的均值和方差作为对整个数据集均值和方差的估计。 
整个BN的[算法](http://lib.csdn.net/base/datastructure)如下： 
![这里写图片描述](https://img-blog.csdn.net/20160223160132599)
求导的过程也非常简单，有兴趣地可以自己再推导一遍或者直接参见原文。
#### 测试
实际[测试](http://lib.csdn.net/base/softwaretest)网络的时候，我们依然会应用下面的式子： 
![这里写图片描述](https://img-blog.csdn.net/20160223160145084)
特别注意： 这里的均值和方差已经不是针对某一个Batch了，而是针对整个数据集而言。因此，在训练过程中除了正常的前向传播和反向求导之外，我们还要记录每一个Batch的均值和方差，以便训练完成之后按照下式计算整体的均值和方差： 
![这里写图片描述](https://img-blog.csdn.net/20160223160200068)
#### BN before or after Activation
作者在文章中说应该把BN放在激活函数之前，这是因为Wx+b具有更加一致和非稀疏的分布。但是也有人做实验表明放在激活函数后面效果更好。这是实验链接，里面有很多有意思的对比实验：[https://github.com/ducha-aiki/caffenet-benchmark](https://github.com/ducha-aiki/caffenet-benchmark)
### 3-Experiments
作者在文章中也做了很多实验对比，我这里就简单说明2个。 
下图a说明，BN可以加速训练。图b和c则分别展示了训练过程中输入数据分布的变化情况。 
![这里写图片描述](https://img-blog.csdn.net/20160223170548936)
下表是一个实验结果的对比，需要注意的是在使用BN的过程中，作者发现Sigmoid激活函数比Relu效果要好。 
![这里写图片描述](https://img-blog.csdn.net/20160223170557093)
