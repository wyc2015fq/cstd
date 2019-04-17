# 支持向量机（SVM）算法推导 - 快来学习鸭～～～ - CSDN博客





2017年09月29日 20:41:56[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：587








**Author:** DivinerShi

线性回归可以简单理解为去找到一条线，使得这条线可以区分不同类的数据。那么SVM就是去找到所有可行的线中，最优的那条。什么叫最优？就是距离两类数据都最远的那条分割线。

**优点：**

可用于线性和非线性分类，也可以用于回归

低泛化误差，容易解释

计算复杂度较低
**缺点：**

对参数和核函数的选择比较敏感

原始SVM只擅长处理二分类问题。
## 线性可分支持向量机

给定线性可分训练数据集，通过间隔最大化或者等价得求解相应的凸二次规划问题学习得到的分离超平面为
![这里写图片描述](https://img-blog.csdn.net/20170929203928880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

以及相应的分类决策函数
![这里写图片描述](https://img-blog.csdn.net/20170929203953813?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

称为线性可分支持向量机。
**假设**超平面为wx+b=0，那么|wx+b|可以用于表示点x距离超平面的距离，而我们要做就是最大化|wx+b|。为了去掉绝对值，可以用y(wx+b)替代，因为y和wx+b是同正负号的，所以y(wx+b)一定是正值。因此可以利用y(wx+b)来表示分类的正确性和确信度，这就是**函数间隔**。

但是我们知道，等比例的改变函数的系数，函数是不变的，但是函数的输出值变了。如将函数间隔中的w，b等比例的变成2w,2b那么函数间隔将扩大两倍。所以我们需要对函数间隔加一定的约束，这里我们对函数系数统一除以w的L2模，||W||。那么我们就可以得到**几何间隔**：
![这里写图片描述](https://img-blog.csdn.net/20170929204044371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因为我们要求的是系数w和b，因为最优解是唯一的，唯一要确定的就是w和b的比例，所以当||w||确定后，w和b就可以确定了。因此我们可以将||w||定为1，即||w||=1.

那么就可以唯一确定w和b了。
## 间隔最大化

现在有了几何间隔后，我们要做得就是去使得间隔能够最大化。支持向量机的基本思想就是求解正确划分训练集并且几何间隔最大的分离超平面。线性可分的超平面可能有无数个，但是几何间隔最大的分离超平面是唯一的。

所以我们要做得就是最大化几何间隔，而且所有的样本点到最优超平面的距离都需大于几何间隔。因此我们可以得到如下的约束优化函数：
![这里写图片描述](https://img-blog.csdn.net/20170930130852321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后可以进一步改写为
![这里写图片描述](https://img-blog.csdn.net/20170930130922566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上面我们也提到过，可以将函数间隔直接定为一个常数，这里选择为1，这样w和b就是唯一确定的值了。带入上式中，得到![这里写图片描述](https://img-blog.csdn.net/20170930131000370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。为求解方便，将最大化![这里写图片描述](https://img-blog.csdn.net/20170930131000370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)转化为最小化![这里写图片描述](https://img-blog.csdn.net/20170930131058698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，所以我们可以得到如下最优化问题
![这里写图片描述](https://img-blog.csdn.net/20170930131121780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这是一个凸二次规划问题。直接求解就可以。但是为了更容易的求解，并且能自然的引入核函数，所以这里选择使用对偶算法来进行求解。
## 对偶：

定义拉格朗日函数：
![这里写图片描述](https://img-blog.csdn.net/20170930131207085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

原始问题为：![这里写图片描述](https://img-blog.csdn.net/20170930131235271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因为先求α，需要去固定w,b两个参数，难以求解，所以利用对偶来解，转化为![这里写图片描述](https://img-blog.csdn.net/20170930131334841?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

1.先固定α，求解w,b，分别对w,b求偏导数并令其等于0；

2.求α的极大；
## 软间隔最大化

但是真实数据中，其实很难做到能完全线性可分的数据。所以这里假设所有的数据都存在一定的偏移。为了解决这个问题，对每个样本点![这里写图片描述](https://img-blog.csdn.net/20170930131444025?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)引入一个松弛变量![这里写图片描述](https://img-blog.csdn.net/20170930131513342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，使得函数间隔加上松弛变量大于等于1，即约束条件变为![这里写图片描述](https://img-blog.csdn.net/20170930131550014?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，目标函数由原来的![这里写图片描述](https://img-blog.csdn.net/20170930131632019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)变成![这里写图片描述](https://img-blog.csdn.net/20170930131656654?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

所以现在的线性支持向量机的学习问题变成如下凸二次规划问题：
![这里写图片描述](https://img-blog.csdn.net/20170930132301070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 非线性支持向量机与核函数

什么是核函数？

最开始的支持向量只能支持线性可分数据，对于那些非线性的数据集无能为力。如果可以将非线性的数据通过一个函数映射到线性可分的空间，映射后的特征空间叫希尔伯特空间。

核函数的作用就是在不知道该函数是什么的情况下来进行映射；

设X输入空间，H为希尔伯特空间，如果存在一个从X到H的映射
![这里写图片描述](https://img-blog.csdn.net/20170930141149058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

使得对所有来自X的x,z，函数K(x,z)满足条件
![这里写图片描述](https://img-blog.csdn.net/20170930141218767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

则称K(x,z)为核函数，![这里写图片描述](https://img-blog.csdn.net/20170930141247454?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)为映射函数，式中![这里写图片描述](https://img-blog.csdn.net/20170930141310084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)为![这里写图片描述](https://img-blog.csdn.net/20170930141334366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)和![这里写图片描述](https://img-blog.csdn.net/20170930141359207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)的内积。

其实说白了就是找不到确定映射函数，然后找个可以做非线性映射的函数替代下，并且这个函数只需要去计算它的内积就可以了。

我们之前的解是可以解出来的，然后解里面是存在一个内积的，核技巧就是把这个内积改成核函数的内积，在外面套了一层函数，这就把原始数据做了一个映射了。

常见的一些核函数也就是

线性核（就是和之前的一样，线性的）

高斯核（用高斯函数做核函数），也叫径向基核

等等

整体的流程就是这样还有一点要注意的就是，在求解参数α的时候，因为参数很多，一般会用SMO算法启发式的去求解这些α的值。
## 参考

统计学习方法-李航










