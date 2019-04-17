# 如何直观理解拉格朗日乘子法与KKT条件 - Machine Learning with Peppa - CSDN博客





2018年05月02日 16:37:13[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1735








以前学习SVM时曾草草了解到过KKT条件，当然，关于KKT条件的前身也就是拉格朗日乘子法，是高数就接触过的。如今学习最优化理论，又碰到了他俩，不得不说是一种缘分，当然这两个概念也是机器学习算法中非常重要的，因为机器学习归根到底就是一种优化。接下来让我们来一起看看。

### 拉格朗日乘子法

首先来了解拉格朗日乘子法，那么为什么需要拉格朗日乘子法？记住，有拉格朗日乘子法的地方，必然是一个组合优化问题。那么带约束的优化问题很好说，就比如说下面这个：

![](https://img-blog.csdn.net/20180502160539118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


也就是说，自变量![x](https://www.zhihu.com/equation?tex=x)是一个n维向量，要最大化一个目标函数![f](https://www.zhihu.com/equation?tex=f)，满足若干等式和不等式约束。KKT条件宣称，如果有一个点 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A)是满足所有约束的极值点，则


![](https://img-blog.csdn.net/20180502160539118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


这是一个带等式约束的优化问题，有目标值，有约束条件。那么想想假设没有约束条件这个问题是怎么求解的呢？是不是直接f对各个x求导等于0,，解x就可以了，可以看到没有约束的话，求导为0，那么各个x均为0，这样f=0最小。也就是说，自变量 x 是一个n维向量，要最大化一个目标函数 f ，满足若干等式和不等式约束。

那么问题就来了，这里在说一点的是，为什么上面说求导为0就可以呢？理论上多数问题是可以的，但是有的问题不可以。如果求导为0一定可以的话，那么f一定是个凸优化问题，什么是凸呢？
- 凸函数

严格说，我在泛函分析里看到的定义是这样的：设f为定义在区间I上的函数，![](https://gss1.bdstatic.com/9vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D37/sign=538640061f178a82ca3c79a7f403a8d3/e850352ac65c103882e37408b3119313b17e895f.jpg)和任意的实数 ![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D62/sign=98d866f9728da9774a2f8529b151a370/c75c10385343fbf2605903e9b17eca8064388f87.jpg)


![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D292/sign=af32d1ce960a304e5622a7f3e3c9a7c3/962bd40735fae6cd29a40eab0eb30f2443a70fe3.jpg)


则f称为I上的凸函数，当且仅当其上境图（在函数图像上方的点集）为一个凸集。如果通俗点说，我们在数学分析中学到的函数图像，如果函数任意一点的一阶导数（切线）值总是位于函数曲线的下方，那么就是凸函数。

![](https://img-blog.csdn.net/20180502163009723?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



- 凸优化

![](https://gss3.bdstatic.com/7Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D36/sign=dc9d3e2ecafc1e17f9bf8a374a90a972/3b87e950352ac65c8f0cef4bfef2b21193138af9.jpg)为目标函数，![](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D65/sign=6032b1df942397ddd2799b015982df46/bba1cd11728b4710bc1875f4c6cec3fdfc0323d6.jpg)为限制条件。 如果此处目标函数和限制条件为凸函数，即满足下式时优化问题即为一个凸优化问题

![](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D202/sign=d9e1d389850a19d8cf03830501fa82c9/ac6eddc451da81cb2c8509f55766d016092431a2.jpg)





想象一下，目标函数![f(x,y)](https://www.zhihu.com/equation?tex=f%28x%2Cy%29)是一座山的高度，约束![g(x,y)=C](https://www.zhihu.com/equation?tex=g%28x%2Cy%29%3DC)是镶嵌在山上的一条曲线如下图。
![](https://pic3.zhimg.com/80/ecf06d062b493f1674bd81d34d3446a8_hd.jpg)

你为了找到曲线上的最低点，就从最低的等高线（0那条）开始网上数。数到第三条，等高线终于和曲线有交点了（如上图所示）。因为比这条等高线低的地方都不在约束范围内，所以这肯定是这条约束曲线的最低点了。

而且约束曲线在这里不可能和等高线相交，一定是相切。因为如果是相交的话，如下图所示，那么曲线一定会有一部分在B区域，但是B区域比等高线低，这是不可能的。

![](https://img-blog.csdn.net/20180502161842546?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

两条曲线相切，意味着他们在这点的法线平行，也就是法向量只差一个任意的常数乘子（取为 -lambda ）：




![\nabla f(x,y)=-\lambda \nabla g(x,y)](https://www.zhihu.com/equation?tex=%5Cnabla+f%28x%2Cy%29%3D-%5Clambda+%5Cnabla+g%28x%2Cy%29)

我们把这个式子的右边移到左边，并把常数移进微分算子，就得到



![\nabla (f(x,y)+\lambda g(x,y))=0](https://www.zhihu.com/equation?tex=%5Cnabla+%28f%28x%2Cy%29%2B%5Clambda+g%28x%2Cy%29%29%3D0)

把这个式子重新解释一下，这个就是函数

![f(x,y)+\lambda g(x,y)](https://www.zhihu.com/equation?tex=f%28x%2Cy%29%2B%5Clambda+g%28x%2Cy%29)

无约束情况下极值点的充分条件。







![1](https://images2015.cnblogs.com/blog/743682/201607/743682-20160730165822138-1361790136.png)





如绿线所示，目标函数 f(x,y)f(x,y) 与约束 g(x,y)g(x,y) 只有三种情况，相交、相切或者没有交集，没交集肯定不是解,只有相交或者相切可能是解，但相交得到的一定不是最优值，因为相交意味着肯定还存在其它的等高线在该条等高线的内部或者外部，使得新的等高线与目标函数的交点的值更大或者更小，这就意味着只有等高线与目标函数的曲线相切的时候，才可能得到可行解。因此给出结论：拉格朗日乘子法取得极值的必要条件是目标函数与约束函数相切，这时两者的法向量是平行的。（关于法向量平行，推荐大家去看看MIT公开课：[多变量微积分：拉格朗日乘数法](http://open.163.com/movie/2010/8/P/C/M6TUC9K75_M6TUI4FPC.html)，老师讲的很好）




## KKT条件

在求取有约束条件的优化问题时，拉格朗日乘子法（Lagrange Multiplier) 和KKT条件是非常重要的两个求取方法，对于等式约束的优化问题，可以应用拉格朗日乘子法去求取最优值；如果含有不等式约束，可以应用KKT条件去求取。当然，这两个方法求得的结果只是必要条件，只有当是凸函数的情况下，才能保证是充分必要条件。

其实我们都知道，KKT条件是拉格朗日乘子法的泛化，在最优化理论中，KKT条件又叫做一阶（最优性）条件，其实还有一个二阶最优条件，但是这里我们暂且不讨论，想要了解的朋友欢迎去翻看教材，任何一本优化书里都有。

对于含有不等式约束的优化问题，如何求取最优值呢？常用的方法是KKT条件，同样地，把所有的不等式约束、等式约束和目标函数全部写为一个式子L(a, b, x)= f(x) + a*g(x)+b*h(x)，KKT条件是说最优值必须满足以下条件：



![](https://img-blog.csdn.net/20180502162527288?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



满足 KKT 条件后极小化 Lagrangian 即可得到在不等式约束条件下的可行解。 KKT 条件看起来很多，其实很好理解:
- (1) ：拉格朗日取得可行解的必要条件；
- (2) ：这就是以上分析的一个比较有意思的约束，称作松弛互补条件；
- (3) ∼∼ (4) ：初始的约束条件；
- (5) ：不等式约束的 Lagrange Multiplier 需满足的条件。

求取这三个等式之后就能得到候选最优值。其中第2个式子非常有趣，因为g(x)<=0，如果要满足这个等式，必须B=0或者g(x)=0. 这是SVM的很多重要性质的来源，如支持向量的概念。

为什么要这么求能得到最优值？先说拉格朗日乘子法，设想我们的目标函数z = f(x), x是向量, z取不同的值，相当于可以投影在x构成的平面（曲面）上，即成为等高线，如下图，目标函数是f(x, y)，这里x是标量，虚线是等高线，现在假设我们的约束g(x)=0，x是向量，在x构成的平面或者曲面上是一条曲线，假设g(x)与等高线相交，交点就是同时满足等式约束条件和目标函数的可行域的值，但肯定不是最优值，因为相交意味着肯定还存在其它的等高线在该条等高线的内部或者外部，使得新的等高线与目标函数的交点的值更大或者更小，只有到等高线与目标函数的曲线相切的时候，可能取得最优值，如下图所示，即等高线和目标函数的曲线在该点的法向量必须有相同方向，所以最优值必须满足：f(x)的梯度 = a* g(x)的梯度，a是常数，表示左右两边同向。这个等式就是L(a,x)对参数求导的结果。




## 参考文章：

[约束优化方法之KKT条件](https://www.cnblogs.com/ooon/p/5721119.html)


[优化算法—凸优化](https://blog.csdn.net/google19890102/article/details/44018135)


[SVM-拉格朗日乘数法与KKT条件](https://blog.csdn.net/on2way/article/details/47729419)



