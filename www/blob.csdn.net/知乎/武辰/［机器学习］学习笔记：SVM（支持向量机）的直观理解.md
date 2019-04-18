# ［机器学习］学习笔记：SVM（支持向量机）的直观理解 - 知乎
# 

我最近在学习机器学习，打算抽部分时间把我的学习心得写下来。以下内容属个人见解，欢迎大家一起交流。

在机器学习系列文章里，为了节省时间，一些图画可能画得比较粗糙。以后有时间会回头修改我的文章。

支持向量机是一个二分类模型。在这篇文章里，我以最简单的**两特征**（即自变量只有两个）模型为例。

如下图所示，每一个数据带有两个特征：x和z。给定两个特征的值后，输出y=1或者-1。比如x代表年收入，z代表年消费，给定一个人的年收入和年消费情况，判断他是否会使用蚂蚁花呗，y=1代表他会使用蚂蚁花呗。

1代表正类，是圆形的点。-1代表负类，是小叉叉。
![](https://pic3.zhimg.com/v2-5d95b837ef73ca51d8267d181da1a2b6_b.jpg)
比如：A的x值为3，z值为3，即A点的坐标是（3,3），而A属于正类，于是就有这样的映射：（3,3） ![\rightarrow](https://www.zhihu.com/equation?tex=%5Crightarrow) 1.反映在上面具体的例子中就是：有一个人的年收入和年收入都是3万元，且他会使用蚂蚁花呗。

类似地，B点反映的映射是：（4,0.5） ![\rightarrow](https://www.zhihu.com/equation?tex=%5Crightarrow) -1.

我们要找一条直线，把正类和负类分隔开来。事实上，我们可以找到无穷条这样的线。但是我们要选取一条最好的线。那什么样的线是最好的线呢？支持向量机认为，拥有最大间隔的线是最好的线。那什么是最大间隔呢？

先来一个直观的感受，如下图所示，黑线和红线都能够正确分离正类和负类。在我们的标准里，黑线是更优的。红线虽然也分对了所有的点，但是离样本点太近了。
![](https://pic1.zhimg.com/v2-a7357c62a53fba3dfd34126baad2e200_b.jpg)
应用到新的样本（或应用到实际预测）中，假设有一个新的点D（如下图所示）。D本来是属于正类的，但是按照红线的标准，D会被划分到负类中。
![](https://pic3.zhimg.com/v2-a9603d5372f77a5b53e53f80984324f6_b.jpg)
接下来就是具体的数学论述。

我们在高中学过，点（ ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) , ![z_{i}](https://www.zhihu.com/equation?tex=z_%7Bi%7D) ）到直线 ![ax+bz+c=0](https://www.zhihu.com/equation?tex=ax%2Bbz%2Bc%3D0) 的距离 ![d](https://www.zhihu.com/equation?tex=d) 为：

![\frac{ax_{i}+bz_{i}+c}{\sqrt{a^{2}+b^{2}}}](https://www.zhihu.com/equation?tex=%5Cfrac%7Bax_%7Bi%7D%2Bbz_%7Bi%7D%2Bc%7D%7B%5Csqrt%7Ba%5E%7B2%7D%2Bb%5E%7B2%7D%7D%7D) 。注意这个距离是带有正负号的。

在支持向量机中，我们定义“几何间隔”为： ![y_{i}\frac{ax_{i}+bz_{i}+c}{\sqrt{a^{2}+b^{2}}}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5Cfrac%7Bax_%7Bi%7D%2Bbz_%7Bi%7D%2Bc%7D%7B%5Csqrt%7Ba%5E%7B2%7D%2Bb%5E%7B2%7D%7D%7D) .

为什么要这样定义：由于当 ![ax_{i}+bz_{i}+c>0](https://www.zhihu.com/equation?tex=ax_%7Bi%7D%2Bbz_%7Bi%7D%2Bc%3E0) 时，经过模型计算， ![y](https://www.zhihu.com/equation?tex=y) 的输出结果是1（事实上样本中的 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 也就是1）；同理，当 ![ax_{i}+bz_{i}+c<0](https://www.zhihu.com/equation?tex=ax_%7Bi%7D%2Bbz_%7Bi%7D%2Bc%3C0) 时， ![y](https://www.zhihu.com/equation?tex=y) 的输出结果是-1。那么几何间隔将会是一个恒为正的标量。且它还蕴含着“样本点被正确分类”。因为如果没有被正确分类，比如当 ![ax_{i}+bz_{i}+c>0](https://www.zhihu.com/equation?tex=ax_%7Bi%7D%2Bbz_%7Bi%7D%2Bc%3E0) 时，![y](https://www.zhihu.com/equation?tex=y) 的输出结果是1，但是 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 的真实值其实是-1，那么这个几何间隔就变成负数了，就不满足我们的要求了。

如下图所示，每个点到直线都有一个距离。首先找出距离直线距离最短的那个点（可能同时有几个点到直线的距离最短，这没有关系），假设这个点就是A。我们的目标是：找出使得点A到直线距离最大的直线。
![](https://pic3.zhimg.com/v2-ed693d6774439aa731652c83b9068246_b.jpg)
记第i个点到直线 ![ax+bz+c=0](https://www.zhihu.com/equation?tex=ax%2Bbz%2Bc%3D0) 的几何间隔是 ![d_{i}](https://www.zhihu.com/equation?tex=d_%7Bi%7D) ，

再记 ![d_{0}=min](https://www.zhihu.com/equation?tex=d_%7B0%7D%3Dmin) { ![d_{i}](https://www.zhihu.com/equation?tex=d_%7Bi%7D) }，即 ![d_{0}](https://www.zhihu.com/equation?tex=d_%7B0%7D) 是所有点中距离直线最短的距离。

我们的目标就是：

在 ![d_{i}\geq d_{0}](https://www.zhihu.com/equation?tex=d_%7Bi%7D%5Cgeq+d_%7B0%7D) 的约束下，我们需要使 ![d_{0}](https://www.zhihu.com/equation?tex=d_%7B0%7D) 最大化.

接下来分析 ![d_{i}\geq d_{0}](https://www.zhihu.com/equation?tex=d_%7Bi%7D%5Cgeq+d_%7B0%7D)这个约束条件，即：

![y_{i}\frac{ax_{i}+bz_{i}+c}{\sqrt{a^{2}+b^{2}}}\geq y_{0}\frac{ax_{0}+bz_{0}+c}{\sqrt{a^{2}+b^{2}}}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5Cfrac%7Bax_%7Bi%7D%2Bbz_%7Bi%7D%2Bc%7D%7B%5Csqrt%7Ba%5E%7B2%7D%2Bb%5E%7B2%7D%7D%7D%5Cgeq+y_%7B0%7D%5Cfrac%7Bax_%7B0%7D%2Bbz_%7B0%7D%2Bc%7D%7B%5Csqrt%7Ba%5E%7B2%7D%2Bb%5E%7B2%7D%7D%7D)

由于直线 ![ax+bz+c=0](https://www.zhihu.com/equation?tex=ax%2Bbz%2Bc%3D0) 与直线 ![\lambda ax+\lambda bz+\lambda c=0](https://www.zhihu.com/equation?tex=%5Clambda+ax%2B%5Clambda+bz%2B%5Clambda+c%3D0) 是同一条直线（ ![\lambda](https://www.zhihu.com/equation?tex=%5Clambda) 不为0），

所以约束条件等价于：

![y_{i}\frac{\lambda ax_{i}+\lambda bz_{i}+\lambda c}{\sqrt{(\lambda a)^{2}+(\lambda b)^{2}}}\geq y_{0}\frac{\lambda ax_{0}+\lambda bz_{0}+\lambda c}{\sqrt{(\lambda a)^{2}+(\lambda b)^{2}}}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%5Cfrac%7B%5Clambda+ax_%7Bi%7D%2B%5Clambda+bz_%7Bi%7D%2B%5Clambda+c%7D%7B%5Csqrt%7B%28%5Clambda+a%29%5E%7B2%7D%2B%28%5Clambda+b%29%5E%7B2%7D%7D%7D%5Cgeq+y_%7B0%7D%5Cfrac%7B%5Clambda+ax_%7B0%7D%2B%5Clambda+bz_%7B0%7D%2B%5Clambda+c%7D%7B%5Csqrt%7B%28%5Clambda+a%29%5E%7B2%7D%2B%28%5Clambda+b%29%5E%7B2%7D%7D%7D)

也就是说，直线的系数同时放缩，对我们的约束没有影响。

因此我们可以干脆令 ![ax_{0}+bz_{0}+c=1](https://www.zhihu.com/equation?tex=ax_%7B0%7D%2Bbz_%7B0%7D%2Bc%3D1) 。

所以问题转化为：在 ![y_{i}({ax_{i}+bz_{i}+c})\geq1](https://www.zhihu.com/equation?tex=y_%7Bi%7D%28%7Bax_%7Bi%7D%2Bbz_%7Bi%7D%2Bc%7D%29%5Cgeq1) 的约束下，

使 ![\frac{1}{\sqrt{a^{2}+b^{2}}}](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7B%5Csqrt%7Ba%5E%7B2%7D%2Bb%5E%7B2%7D%7D%7D) 最大化。

即使 ![\sqrt{a^{2}+b^{2}}](https://www.zhihu.com/equation?tex=%5Csqrt%7Ba%5E%7B2%7D%2Bb%5E%7B2%7D%7D) 最小化。

以上是本篇文章的主要内容。这篇文章讲的是最基本的模型。事实上，实战过程中可能有远远不止两个特征。那么直线 ![ax+bz+c=0](https://www.zhihu.com/equation?tex=ax%2Bbz%2Bc%3D0) 应该升级为超平面 ![wx+b=0](https://www.zhihu.com/equation?tex=wx%2Bb%3D0) 。其中 ![w,x](https://www.zhihu.com/equation?tex=w%2Cx) 都是n维向量，n是特征的维数（在上面的例子中n为2）， ![b](https://www.zhihu.com/equation?tex=b) 是截距。

另外，本篇文章只考虑距离直线最近的点。这个点起主要作用，因而命名为“支持向量”。我们可以放宽要求，允许超平面误分类，这样就能找到多个距离直线比较近的点。

支持向量机还能和感知机作比较。在这里我想先和一元线性回归作比较。尽管它和一元线性回归是完全两码事，但是有一个值得比较的点：如图所示，一元线性回归要找的直线是使得九条绿色线段长度的平方和最小，而支持向量机要找的直线是，在正确分类的前提下，使得距离直线最短距离的点到直线的距离最大。
![](https://pic3.zhimg.com/v2-cfa87aa444107c7496658d8a1e535482_b.jpg)
如果有时间的话我会更新《数据挖掘学习笔记：支持向量机（2）》。

