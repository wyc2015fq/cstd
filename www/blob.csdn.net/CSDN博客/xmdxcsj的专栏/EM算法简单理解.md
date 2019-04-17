# EM算法简单理解 - xmdxcsj的专栏 - CSDN博客





2015年09月29日 18:05:29[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1704








## EM算法是为了解决什么问题？

在求解概率模型的时候，如果需要的变量都是观测变量，不涉及到隐藏变量的话，可以使用极大似然或者贝叶斯估计来求解模型的参数。比如：对于单高斯模型来讲，如果知道观测变量，那么就可以使用极大似然或者最小均方误差来估计高斯模型的均值和方差。

如果模型同时包含观察变量和隐藏变量的话，传统的方法不能完成模型的估计，此时就需要引入EM算法。比如：对于混合高斯模型来讲，除了需要估计高斯模型的均值和方差，还涉及到每个高斯模型的权重信息，这个权重信息就可以认为是隐变量，所以一般使用EM来求解GMM的参数。

## EM算法的目标函数

![](https://img-blog.csdn.net/20150929180439353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其中Y表示观测变量，Z表示隐含变量。

由于包含隐含变量Z，所以无法正常求解。

## EM算法的思想

使用迭代的方法，一步步的求模型参数，使其逐渐逼近于最优，即保证本次的模型参数qi+1相比上一次的模型参数qi，使得似然函数的值变大。

为了实现似然函数的递增，基于观察数据Y和本次的模型参数qi，构造基于未知变量q的Q(q,qi)函数，作为似然函数L的下限，Q函数最大值对应的q即为本次迭代的模型参数qi+1。

**为什么可以保证Q函数的最大值一定会使得似然函数L的值变大呢？**

因为Q有两个性质：一个是作为L的下限，同时还满足在qi这个点L和Q函数的值是相等的，如果qi+1满足Q(qi+1)>=Q(qi),同时

Q(qi)==L(qi)

L(qi)>=Q(qi), L(qi+1)>=Q(qi+1)

不难推出L(qi+1)>=L(qi)，但是无法保证求出的模型参数是全局最优，所以初值的选择非常重要。

## EM算法的具体执行

![](https://img-blog.csdn.net/20150929180512807?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

EM算法是expectationmaximization的简称，每次迭代分为两步：

**E步，求期望：**

即为Q函数

![](https://img-blog.csdn.net/20150929180459529?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**M步，求极大：**

求Q函数最大时对应的q作为qi+1

如果q对应于多组未知变量，分别求导等于0求解

## EM算法的应用

1.      非监督学习

对于监督学习，训练数据(x,y)都是已知的，对于非监督学习，只知道输入x，不知道输出y。

可以将输出y作为隐藏变量（未观测数据），EM算法可以学习非监督学习的生成模型。

2.  GMM训练



