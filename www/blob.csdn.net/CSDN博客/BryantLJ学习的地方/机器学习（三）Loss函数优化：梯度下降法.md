# 机器学习（三）Loss函数优化：梯度下降法 - BryantLJ学习的地方 - CSDN博客





2016年08月09日 15:21:56[遍地流金](https://me.csdn.net/u012177034)阅读数：6365








线性回归，逻辑回归，Softmax分类器，LinearSVM等算法都能够简单的分解成scoreFunction的设计与LossFunction的求解两部分，具体分解如下表所示：
|ClassifierName|ScoreFunction|LossFunction|
|----|----|----|
|LinearSVM|$s=W^{T}X$|$L=max(0,1-s)$|
|LinearRegression|$s=W^{T}X$|$L=(s-label)^{2}$|
|LogisticRegression|$s=W^{T}X$|$p=\frac{1}{1+e^{-s}};L=label\bullet log(p)+(1-label)\bullet log(1-p)$|
|SoftmaxClassifier|$s=W^{T}X$|$p_{k}=\frac{e^{s_{k}}}{\sum_{i=1}^{K}e^{s_{i}}};L=log(p_{label})$|

本文主要讲述传统机器学习算法损失函数$L=f(W,X)$关于权值$W$的优化问题

## 梯度下降法思想

**梯度下降法及其各种变体为目前机器学习（包括神经网络）中使用最多的优化算法。其不仅能够有效处理凸函数优化问题，还能够对非常复杂的非凸函数:神经网络，进行优化。**

类似的优化算法还有共轭梯度法，牛顿法，拟牛顿法等一系列迭代优化算法，这些新的算法虽然原理上很快，但是都有一些适用范围，没有梯度下降法普适性。比如牛顿法在处理具有奇点的目标函数时可能无法收敛等。 

梯度下降法的核心思想：

$W=W-\alpha*\frac{\partial f(W,X)}{\partial W}|X_{i}$对每一个样本都执行一次梯度计算，然后朝下降最快的地方更新权值 
**奇点问题**

由于采用的Loss函数可能会有一些奇点，导致在某些点上$f(W,X)$的倒数不存在，梯度无法计算的情况，这时可以用次梯度(subgradient)来代替梯度，仍然可以进行迭代优化。 
![次梯度](https://img-blog.csdn.net/20160809152304491)
次梯度的概念如下： 

c称为$f(x)$在$x_{0}$处的次梯度，如果$\exists \delta$，对于$\forall x\in(x_{0}-\delta,x_{0}+\delta)$，都有$f(x)-f(x_{0})>=c(x-x_{0})$

由定义可知当该点可导时，次梯度即为该点的导数，否则则为一个闭区间$[f^{'}(x_{0-}),f^{'}(x_{0+})]$或$[f^{'}(x_{0+}),f^{'}(x_{0-})]$
根据梯度的计算方式不同，梯度下降法有多种变体： 
![这里写图片描述](https://img-blog.csdn.net/20160810111718107)
## 批梯度下降（Batch Gradient Descent）

每次迭代的梯度方向由所有样本共同决定。 

损失函数：

$J(\theta ) = \frac{1}{{2m}}\sum\limits_{i = 1}^m {{{({h_\theta }({x^{(i)}}) - {y^{(i)}})}^2}}$

训练算法为： 


$\begin{array}{l} repeate\{ \\ \theta : = \theta  - \alpha \frac{1}{m}\sum\limits_{i = 1}^m ( {h_\theta }({x^{(i)}}) - {y^{(i)}})x_j^{(i)}\\ \}  \end{array}$

即先计算损失函数在每个样本处的梯度，然后将所有样本的平均梯度作为整个训练集的梯度，进而来更新权值 
**好处**：全数据集的梯度平均值能够更好的代表样本总体 
**坏处**：数据集很大时一次处理完所有样本不切实际
## 迷你批梯度下降（Mini-batch Gradient Descent）

为了弥补Batch Gradient Descent法在处理大数据时的不足，mini-batch Gradient Descent被提了出来。其主要思想是：用训练集的较小子集来代表样本总体进行梯度更新。如果样本集分布充分的话，对其进行随机采样，子集的分布律与样本总体的差别不大，这样既能更快的更新。 

将样本总体$\Omega$分为k个随机采样的$mini-batch$，即$\Omega = \{ b_{1},b_{2},...b_{k} \}$，其中每个batch的样本数为m 

则一个epoch的训练算法如下：

$ for\ each\ b_{i}\ in\ \Omega \\ \theta := \theta -\alpha \frac{1}{m}\sum_{i=1}^{m}({h_\theta }({x^{(i)}}) - {y^{(i)}})x^{(i)}，\forall x^{(i)}\in b_{i}$
**好处**： 

（1）每次处理多个样本，将这种梯度计算和更新的操作采用向量式操作，计算更快。 

（2）内存利用率高 

（3）每次只利用小样本集合代替全体样本，收敛更快 

（4）对于一个epoch而言，采用多个batch而非整个总体进行迭代，能够一定程度避免局部极小值 
**坏处**： 

（1）样本子集并不能很正确的代表样本总体 

（2）**由于代表性不足，会造成整体梯度更新的效率降低，如8个batch更新的梯度方向为0.1，2个batch_更新的梯度为-0.4，则一个epoch下来，梯度相当于没有更新**
**对于目前的大数据集训练，一般batch_size的选取原则就是在内存的限制下越大越好，越大子集越能代表样本整体。一般选取32，64，128，256这几种参数。**选取为2的幂次原因是：向量化代码对于输入大小为2的幂次的数据加速更快 

但batch_size过大也可能会导致迭代完一个epoch，需要更少的迭代次数，进而导致要达到相同的精度需要的epoch个数更多～～

## 随机梯度下降（Stochastic Gradient Descent）

batch_size为1的mini-batch Gradient Descent即叫做SGD，也叫做**Online Learning**
**好处**：时效性非常好，能够在线学习 
**坏处**：单个样本不能代表样本总体的分布，利用不同样本进行的梯度更新可能会相互抵消，进而会造成损失函数在整个样本集上不能收敛








