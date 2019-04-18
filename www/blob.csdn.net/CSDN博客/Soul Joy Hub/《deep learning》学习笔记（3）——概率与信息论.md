# 《deep learning》学习笔记（3）——概率与信息论 - Soul Joy Hub - CSDN博客

2017年09月20日 14:11:10[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1488
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/78040210](http://blog.csdn.net/u011239443/article/details/78040210)

# 3.1 为什么要使用概率？

概率论是用来描述不确定性的数学工具，很多机器学习算都是通过描述样本的概率相关信息或推断来构建模型；信息论最初是用来描述一个信号中包含信息的多少进行量化，在机器学习中通常利用信息论中的一些概念和结论来描述不同概率分布之间的关系。

# 3.2 随机变量
- 随机变量: 可以随机取不同值的变量，在机器学习算法中，每个样本的特征取值，标签值都可以看作是一个随机变量，包括离散型随机变量和连续型随机变量。

# 3.3 概率分布
- 概率分布: 表示随机变量在每一个可能取到状态下的可能性大小，对于离散型的概率分布，称为概率质量函数(Probability Mass Function, PMF)，对于连续性的变量，其概率分布叫做概率密度函数(Probability Density Function, PDF)
- 联合概率分布:　两个或两个以上随机随机变量联合地概率分布情况。

# 3.4 边缘概率
- 边缘概率: 在知道联合概率分布后，其中某几个变量子集的概率分布被称为边缘概率分布。

# 3.5 条件概率
- 条件概率: 在给定某个事件后，其他事件发生的概率，例如在已知今天多云的条件下下雨的概率就是条件概率有，在随机变量X=x的条件下 Y=y的条件概率为：

![](http://upload-images.jianshu.io/upload_images/1621805-5ce05dc01a3ab3df.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.6 条件概率的链式法则

![](http://upload-images.jianshu.io/upload_images/1621805-e4b240956e6f03a1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.7 独立性和条件独立性

![](http://upload-images.jianshu.io/upload_images/1621805-73d9fbd0a166a5d9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-2e1fcfd50f8a9810.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.8 期望、方差和协方差

![](http://upload-images.jianshu.io/upload_images/1621805-c39b0588c88994e5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-a969fc444cb0dbe9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-4cf6ae429af976ed.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

协方差矩阵（covariance matrix）是一个 n × n 的矩阵，并且满足：

![](http://upload-images.jianshu.io/upload_images/1621805-687b603549cf2510.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.9 常用概率分布

![](http://upload-images.jianshu.io/upload_images/1621805-02e3551df4e46c4c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- 指数分布

![](http://upload-images.jianshu.io/upload_images/1621805-279d31d94cfbfafd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-53fec712ade8d617.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 在一些情况下，我们希望概率分布中的所有质量都集中在一个点上。这可以通 

过Dirac delta 函数（Dirac delta function）δ(x) 定义概率密度函数来实现：

![](http://upload-images.jianshu.io/upload_images/1621805-278a071e81f52f38.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

Dirac delta 函数被定义成在除了 0 以外的所有点的值都为 0，但是积分为 1。Dirac 

delta 函数不像普通函数一样对 x 的每一个值都有一个实数值的输出，它是一种不同 

类型的数学对象，被称为广义函数（generalized function），广义函数是依据积分性质定义的数学对象。我们可以把 Dirac delta 函数想成一系列函数的极限点，这一系列函数把除 0 以外的所有点的概率密度越变越小。 

- Dirac 分布经常作为经验分布（empirical distribution）的一个组成部分出现：

![](http://upload-images.jianshu.io/upload_images/1621805-c5ba5ade8328dd15.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

经验分布将概率密度1/m赋给 m 个点 x (1) ,…,x (m) 中的每一个，这些点是给定的数据集或者采样的集合。
- 分布的混合 

通过组合一些简单的概率分布来定义新的概率分布也是很常见的。一种通用的组 

合方法是构造混合分布（mixture distribution）。混合分布由一些组件 (component) 

分布构成。样本是由哪个组件分布产生的取决于从一个 Multinoulli 分布中采样的结果：

![](http://upload-images.jianshu.io/upload_images/1621805-9fc65dd4b9ce6eec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

这里 P(c) 是对各组件的一个 Multinoulli 分布。 

混合模型使我们能够一瞥以后会用到的一个非常重要的概念——潜变量（latent variable）。潜变量是我们不能直接观测到的随机变量。混合模型的组件标识变量 c 就是其中一个例子。潜变量在联合分布中可能和 x 有关，在这种情况下，P(x,c) = P(x | c)P(c)。潜变量的分布 P(c) 以及关联潜变量和观测变量的条件分布P(x | c)，共同决定了分布 P(x) 的形状，尽管描述 P(x) 时可能并不需要潜变量。 

一个非常强大且常见的混合模型是高斯混合模型（Gaussian Mixture Model），它的组件 p(x | c = i) 是高斯分布。每个组件都有各自的参数，均值 µ (i) 和协方差矩阵 Σ (i) 。

![](http://upload-images.jianshu.io/upload_images/1621805-bf6b48124c2bf8d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.10 常用函数的有用性质
- logistic sigmoid函数

![](http://upload-images.jianshu.io/upload_images/1621805-cb820a62ab8da20d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-85c76d1a277b5c4d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 线性整流函数(Rectified Linear Unit, ReLU)

![](http://upload-images.jianshu.io/upload_images/1621805-9c4f013fdb882732.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- softplus函数

![](http://upload-images.jianshu.io/upload_images/1621805-5d7f81f7bb48a150.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-cbbc7c8a81390ea7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.11 贝叶斯规则

![](http://upload-images.jianshu.io/upload_images/1621805-e9eeca540db5390c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.12 连续型变量的技术细节

某个性质如果是几乎处处都成立的，那么它在整个空间中除了一个测度为零的集合以外都是成立的。

# 3.13 信息论

![](http://upload-images.jianshu.io/upload_images/1621805-c55c5c05ba2d5c7b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.14 结构化概率模型

概率图模型: 通过图的概念来表示随机变量之间的概率依赖关系：

![](http://upload-images.jianshu.io/upload_images/1621805-c10e840778c847c5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![这里写图片描述](https://img-blog.csdn.net/20170920141024731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

