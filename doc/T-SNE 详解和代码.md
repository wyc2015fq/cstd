# T-SNE 详解和代码

2018年11月16日 09:49:42 [caymant](https://me.csdn.net/cayman_2015) 阅读数：368



## 1.T-SNE的背景及介绍：

任何事物产生的背景大多来自于现实的需求和现状的不满足：
一：高维数据的可视化是许多不同领域的重要问题，例如与乳腺癌相关的细胞核由约30个变量描述。
二：过去几十年中用于可视化高维数据的技术中很多非降维技术例如基于像素的技术等仅提供了两个以上数据维度的工具，并将数据的解释留给人类观察者。降维方法将高维数据集转换为可以在散点图中显示的二维或三维数据。降维的目的在于在低维地图中尽可能多地保留高维数据的信号结构。出现了几种用于降维的不同技术，他们保留的结构类型不同。论文中提到了七种不同技术，但是它们在可视化真实的高维数据方面不是很成功。t-SNE能够非常好地捕获高维数据的大部分局部结构，同时还揭示全局结构，例如在几个尺度上存在聚类.
三.说到降维，有一张图总结的非常好，这里没有T-sne是因为他比较新，t-SNE是深度学习大牛Hinton和lvdmaaten（他的弟子？）在2008年提出的，lvdmaaten对t-SNE有个主页介绍：tsne,包括论文以及各种编程语言的实现。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116134145731.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NheW1hbl8yMDE1,size_16,color_FFFFFF,t_70)

## 2.T-SNE可以实现的理论基础是基于假设的：

尽管现实世界中的许多数据集是嵌入在高维空间中,但是都具有很低的内在维度.也就是说高维数据经过降维后,在低维状态下更能显示出其本质特性.这就是流行学习的基本思想,也称为非线性降维。

## 3.T-sne基础 Sne

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116094311358.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NheW1hbl8yMDE1,size_16,color_FFFFFF,t_70)

## 5.T-sne较Sne的改进：

①使用对称版的SNE，简化梯度公式。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116094445911.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NheW1hbl8yMDE1,size_16,color_FFFFFF,t_70)
②低维空间下,使用t分布代替高斯分布表达两点之间的相似度（解决拥挤问题）

拥挤问题就是说各个簇聚集在一起，无法区分。比如有一种情况，高维度数据在降维到10维下，可以有很好的表达，但是降维到两维后无法得到可信映射，比如降维如10维中有11个点之间两两等距离的，在二维下就无法得到可信的映射结果(最多3个点)。 进一步的说明，假设一个以数据点xi为中心，半径为r的m维球(三维空间就是球)，其体积是按增长 的，假设数据点是在m维球中均匀分布的，下图表示其他数据点与xi的距离随维度增大而产生的变化。

## 6.算法过程：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116094751355.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NheW1hbl8yMDE1,size_16,color_FFFFFF,t_70)

## 7.不足：

🌂主要用于可视化，很难用于其他目的。比如测试集合降维，因为他没有显式的预估部分，不能在测试集合直接降维；比如降维到10维，因为t分布偏重长尾，1个自由度的t分布很难保存好局部特征，可能需要设置成更高的自由度。

🌂t-SNE倾向于保存局部特征，对于本征维数(intrinsic dimensionality)本身就很高的数据集，是不可能完整的映射到2-3维的空间。

🌂 t-SNE没有唯一最优解，且没有预估部分。如果想要做预估，可以考虑降维之后，再构建一个回归方程之类的模型去做。但是要注意，t-sne中距离本身是没有意义，都是概率分布问题。

🌂训练太慢。有很多基于树的算法在t-sne上做一些改进。
（计算量大耗时长占用内存大，2-3维，局部次优解的影响需要尝试不同的初始化点）

## 8.变种：

multiple maps of t-SNE

parametric t-SNE

Visualizing Large-scale and High-dimensional Data