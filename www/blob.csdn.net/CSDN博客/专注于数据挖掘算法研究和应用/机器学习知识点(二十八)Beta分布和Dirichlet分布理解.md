# 机器学习知识点(二十八)Beta分布和Dirichlet分布理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月17日 10:30:23[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2104
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









1、二者关系：

     Dirichlet分布是Beta分布的多元推广。Beta分布是二项式分布的共轭分布，Dirichlet分布是多项式分布的共轭分布。

     通常情况下，我们说的分布都是关于某个参数的函数，把对应的参数换成一个函数（函数也可以理解成某分布的概率密度）就变成了关于函数的函数。

     于是，把Dirichlet分布里面的参数换成一个基分布就变成了一个关于分布的分布了。




2、Beta分布：




beta分布与二项分布的共轭先验性质二项分布

二项分布即重复n次独立的伯努利试验。在每次试验中只有两种可能的结果，而且两种结果发生与否互相对立，并且相互独立，与其它各次试验结果无关，事件发生与否的概率在每一次独立试验中都保持不变，则这一系列试验总称为n重伯努利实验，当试验次数为1时，二项分布服从0-1分布

**二项分布的似然函数：**


![P(data|\theta) \propto \theta^z(1-\theta)^{N-z} \\z=\sum_{i=1}^NX_i](https://www.zhihu.com/equation?tex=P%28data%7C%5Ctheta%29+%5Cpropto+%5Ctheta%5Ez%281-%5Ctheta%29%5E%7BN-z%7D+%5C%5C%0Az%3D%5Csum_%7Bi%3D1%7D%5ENX_i)

beta分布
![Beta(a,b)=\frac{\theta^{a-1}(1-\theta)^{b-1}}{B(a,b)}\propto \theta^{a-1}(1-\theta)^{b-1}](https://www.zhihu.com/equation?tex=Beta%28a%2Cb%29%3D%5Cfrac%7B%5Ctheta%5E%7Ba-1%7D%281-%5Ctheta%29%5E%7Bb-1%7D%7D%7BB%28a%2Cb%29%7D%5Cpropto+%5Ctheta%5E%7Ba-1%7D%281-%5Ctheta%29%5E%7Bb-1%7D)



在beta分布中，B函数是一个标准化函数，它只是为了使得这个分布的概率密度积分等于1才加上的。


参考：https://en.wikipedia.org/wiki/Beta_distribution


3、Dirichlet分布

狄利克雷分布是一组连续多变量概率分布，是多变量普遍化的Β分布。




K阶狄利克雷分布的概率密度函数表示为如下形式：


![](https://imgsa.baidu.com/baike/s%3D291/sign=80cf89edcf95d143de76e32a42f18296/267f9e2f07082838dd58b9fabf99a9014d08f183.jpg)

其中，

![](https://imgsa.baidu.com/baike/s%3D97/sign=c1a5fdb0dac451daf2f600ecb7fd7200/908fa0ec08fa513dfce709303a6d55fbb3fbd98b.jpg)

为参数。概率密度函数定义在（K-1）维单纯形上：


![](https://imgsa.baidu.com/baike/s%3D108/sign=b86a77c192cad1c8d4bbf827473f67c4/0824ab18972bd407bddeb4137c899e510eb309c6.jpg)



![](https://imgsa.baidu.com/baike/s%3D123/sign=8b9031f965d0f703e2b291de3bfb5148/c8ea15ce36d3d539b783682d3d87e950342ab0a5.jpg)



![](https://imgsa.baidu.com/baike/s%3D169/sign=b58911c86a224f4a5399771530f69044/3b87e950352ac65c174a019dfcf2b21192138a94.jpg)

归一化常数为对变量Beta函数，可以用Gamma函数来表示：


![](https://imgsa.baidu.com/baike/s%3D247/sign=3e1f049dfcf2b211e02e824afd816511/e61190ef76c6a7efe03ccac8fafaaf51f2de66c1.jpg)







参考：https://en.wikipedia.org/wiki/Dirichlet_distribution





