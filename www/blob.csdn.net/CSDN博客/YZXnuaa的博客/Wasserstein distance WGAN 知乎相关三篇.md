# Wasserstein distance WGAN 知乎相关三篇 - YZXnuaa的博客 - CSDN博客
2018年03月28日 11:26:43[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：2138
作者：子元
链接：https://www.zhihu.com/question/39872326/answer/83688277
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
用什么距离取决于你关心什么类型的差别。举几个例子。
1. [Kullback-Leibler divergence](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Kullback%25E2%2580%2593Leibler_divergence)
对于两个分布![p(x)](https://www.zhihu.com/equation?tex=p%28x%29)和![q(x)](https://www.zhihu.com/equation?tex=q%28x%29)，KL散度定义为![D_{KL}(p\|q)=\int_xp(x)\ln\frac{p(x)}{q(x)}\,\mathrm{d}x](https://www.zhihu.com/equation?tex=D_%7BKL%7D%28p%5C%7Cq%29%3D%5Cint_xp%28x%29%5Cln%5Cfrac%7Bp%28x%29%7D%7Bq%28x%29%7D%5C%2C%5Cmathrm%7Bd%7Dx)。可以看出，如果要![D_{KL}(p\|q)](https://www.zhihu.com/equation?tex=D_%7BKL%7D%28p%5C%7Cq%29)小，那么![p(x)](https://www.zhihu.com/equation?tex=p%28x%29)大的地方![q(x)](https://www.zhihu.com/equation?tex=q%28x%29)必须要大（否则![p(x)/q(x)](https://www.zhihu.com/equation?tex=p%28x%29%2Fq%28x%29)会很大）；而在![p(x)](https://www.zhihu.com/equation?tex=p%28x%29)小的地方，KL-divergence 的值对![q(x)](https://www.zhihu.com/equation?tex=q%28x%29)的大小就没那么敏感。相应地，如果要![D_{KL}(q\|p)](https://www.zhihu.com/equation?tex=D_%7BKL%7D%28q%5C%7Cp%29)小，那么![p(x)](https://www.zhihu.com/equation?tex=p%28x%29)小的地方![q(x)](https://www.zhihu.com/equation?tex=q%28x%29)必须也要小；而在![p(x)](https://www.zhihu.com/equation?tex=p%28x%29)大的地方，同样地，KL-divergence 的值对![q(x)](https://www.zhihu.com/equation?tex=q%28x%29)的大小也没那么敏感。
下图来自 [Machine Learning: A Probabilistic Perspective](https://link.zhihu.com/?target=https%3A//mitpress.mit.edu/books/machine-learning-0) p734，演示了上述两种情况。图中![p](https://www.zhihu.com/equation?tex=p)（蓝色）是一个二分量的高斯混合分布，![q](https://www.zhihu.com/equation?tex=q)（红色）是最小化![D_{KL}(p\|q)](https://www.zhihu.com/equation?tex=D_%7BKL%7D%28p%5C%7Cq%29)（图a）或![D_{KL}(q\|p)](https://www.zhihu.com/equation?tex=D_%7BKL%7D%28q%5C%7Cp%29)（图b-c）的高斯分布。感受一下区别。
![](https://pic1.zhimg.com/50/d18d8d8ff1150dbff8cb25febf676fdc_hd.jpg)![](https://img-blog.csdn.net/20180328112355795)
下图来自 [Information Retrieval Perspective to Nonlinear Dimensionality Reduction for Data Visualization](https://link.zhihu.com/?target=http%3A//www.jmlr.org/papers/volume11/venna10a/venna10a.pdf) 的图2。作者在里面提出了一种非线性降维算法，目标函数（差不多）是两个方向的 KL-divergence 的加权平均。调整这个权值的话，三维球面上的点在二维平面上的降维结果会从A变化到B。这也是类似的原理。
![](https://pic3.zhimg.com/50/6108bd93be4514021d61592125673dd6_hd.jpg)![](https://img-blog.csdn.net/20180328112417158)
2. 其他 [f-divergence](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/F-divergence)
KL-divergence 的坏处在于它是无界的。事实上KL-divergence 属于更广泛的 f-divergence 中的一种。一般性的 f-divergence 定义为 ![D_{f}(p\|q)=\int_xq(x)f\left(\frac{p(x)}{q(x)}\right)\,\mathrm{d}x](https://www.zhihu.com/equation?tex=D_%7Bf%7D%28p%5C%7Cq%29%3D%5Cint_xq%28x%29f%5Cleft%28%5Cfrac%7Bp%28x%29%7D%7Bq%28x%29%7D%5Cright%29%5C%2C%5Cmathrm%7Bd%7Dx)，如果取![f(t)=t\log t](https://www.zhihu.com/equation?tex=f%28t%29%3Dt%5Clog+t)或者![f(t)=-\log t](https://www.zhihu.com/equation?tex=f%28t%29%3D-%5Clog+t)就能得到KL-divergence。除了KL-divergence，常用的 f-divergence 有 [Hellinger distance](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Hellinger_distance)、[total variation distance](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Total_variation_distance_of_probability_measures) 等等。这两种 f-divergence 都是有界并且对称的。
3. [Wasserstein distance](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Wasserstein_metric)
只讨论最简单的一种情形，一般情形见维基链接。定义![W_2(p,q)=\sqrt{\min_{P_{XY}}E_{P_{XY}}\left[\|x-y\|_2^2\right]}\ \mathrm{s.t.}\ P_X\sim p,P_Y\sim q](https://www.zhihu.com/equation?tex=W_2%28p%2Cq%29%3D%5Csqrt%7B%5Cmin_%7BP_%7BXY%7D%7DE_%7BP_%7BXY%7D%7D%5Cleft%5B%5C%7Cx-y%5C%7C_2%5E2%5Cright%5D%7D%5C+%5Cmathrm%7Bs.t.%7D%5C+P_X%5Csim+p%2CP_Y%5Csim+q)，也就是说，对任意边缘分布为![p](https://www.zhihu.com/equation?tex=p)和![q](https://www.zhihu.com/equation?tex=q)的联合分布![P_{XY}](https://www.zhihu.com/equation?tex=P_%7BXY%7D)，我们可以求出![E_{P_{XY}}\left[\|x-y\|_2^2\right]](https://www.zhihu.com/equation?tex=E_%7BP_%7BXY%7D%7D%5Cleft%5B%5C%7Cx-y%5C%7C_2%5E2%5Cright%5D)，而 ![p](https://www.zhihu.com/equation?tex=p)和![q](https://www.zhihu.com/equation?tex=q)的 Wasserstein distance 则定义为当![P_{XY}](https://www.zhihu.com/equation?tex=P_%7BXY%7D)取遍可能的分布时，这个期望的最小值的平方根。
Wasserstein distance 衡量了把数据从分布![p](https://www.zhihu.com/equation?tex=p)“移动成”分布![q](https://www.zhihu.com/equation?tex=q)时所需要移动的平均距离的最小值（类似于把一堆土从一个形状移动到另一个形状所需要做的功的最小值）。下图出自 [Principal Differences Analysis: Interpretable Characterization of Differences between Distributions](https://link.zhihu.com/?target=http%3A//arxiv.org/pdf/1510.08956.pdf) 的图S2。文章目的是找出能解释两个维数一样的总体分布不同的最小特征集。他们找了个例子，说明有时候使用 Wasserstein distance 来刻画分布之间的不同是必要的：右边的那个 gene expression 的分布，看上去十分像是把左边那个分布往上挤了之后的结果。所以如果要刻画这种“原因”导致的区别，用 Wasserstein distance 比其他要更合理。
![](https://pic1.zhimg.com/50/5e38b0909d9305504163b1dfb759b3ad_hd.jpg)![](https://img-blog.csdn.net/20180328112429649)
4. 其他。见 [Statistical distance](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Statistical_distance)。还是那句话：先问自己关心分布之间怎样的不同，有没有什么特殊约束或要求，再据此作相应的选择。至于题主的例子……我不太清楚“分布里的横坐标有着很实际的意义，比如分贝或者转速”这句话对“衡量两个分布之间的相似度（或者距离）”有着什么样的具体约束。题主可以先厘清一下。
## 文章2
Wasserstein GAN可以算是GAN界的一大突破了，有关它的介绍和使用心得的文章也已经满天飞了，感兴趣的童鞋随便一搜就能好多，今天就不说太多大家说过的内容，我们从一个十分通俗的角度来看看这个目标函数究竟做了些什么。
## 一个简单的例子
如果直接去看Wasserstein metric的定义，相信对实变函数、泛函分析、测度论等数学学科不熟悉的人来说简直是云里雾里：
![W(\mu, \upsilon)=\inf_{\gamma \in \Gamma (\mu, \upsilon)} \int_{M \times M}d(x,y) d \gamma(x,y)](https://www.zhihu.com/equation?tex=W%28%5Cmu%2C+%5Cupsilon%29%3D%5Cinf_%7B%5Cgamma+%5Cin+%5CGamma+%28%5Cmu%2C+%5Cupsilon%29%7D+%5Cint_%7BM+%5Ctimes+M%7Dd%28x%2Cy%29+d+%5Cgamma%28x%2Cy%29)
这公式都说了些什么，看到就头疼，看看旁边的解释，稍微明白一点，但还是晕晕乎乎的。下面我们就从一个简单的例子开始说起，然后慢慢过渡到这个公式上来。
我们使用最优运输里面的一个小例子，假设我国的某市要修建立交桥，修高速公路需要石头，现在已经查明城市附近有几个山头有石头，我们希望把石头采集来并运输到立交桥的几个建造地点，如图所示：
我们假设其他的工作都没有花费，只有运输这一步花费，那么从这些石头产生地把石头运到建桥处，怎么最省钱呢？
为了计算方便，我们需要再做一些设定：
- 石头a处有100单位石头
- 石头b处有50单位石头
- 石头c处有100单位石头
- 桥A处需要100单位石头
- 桥B处需要50单位石头
- 桥C处需要100单位石头
各个点的距离如下所示：
```
d(a,A)=10, d(a,B)=20, d(a,C)=30
d(b,A)=25, d(b,B)=25, d(b,C)=25
d(c,A)=20, d(c,B)=30, d(c,C)=10
```
我们假设每搬运1单位石头行走1单位距离，我们要花费1块钱，那么最优的搬运路径是什么，怎么最省钱？
这个问题其实也不算很复杂，我们令x[s][t]表示把s石头处运往t桥处的石头数量，那么就有下面的公式：
```
min 10*x[a][A]+20*x[a][B]+30*x[a][C]
   +25*x[b][A]+25*x[b][B]+25*x[b][C]
   +20*x[c][A]+30*x[c][B]+10*x[c][C]
s.t.
# 石头的约束
x[a][A]+x[a][B]+x[a][C]=100
x[b][A]+x[b][B]+x[b][C]=50
x[c][A]+x[c][B]+x[c][C]=100
# 桥的约束
x[a][A]+x[b][A]+x[c][A]=100
x[a][B]+x[b][B]+x[c][B]=50
x[a][C]+x[b][C]+x[c][C]=100
```
那么这个问题能不能求出最优解呢？当然可以，这道线性规划的问题就交给感兴趣的童鞋自行研究答案了。总之我们列出了公式，并给明了对应的约束条件，大家对这个问题应该已经十分清晰了。
不过话说回来，这个问题和Wasserstein有关系么？当然有。我们需要对上面的公式做一些抽象了。
首先是最小化的函数，我们用![d(s,t)](https://www.zhihu.com/equation?tex=d%28s%2Ct%29)表示石头到桥的距离，用![m(s,t)](https://www.zhihu.com/equation?tex=m%28s%2Ct%29)表示运输石头的数量，这样目标函数就变成了：
![min \sum_{\pi(s,t)}d(s,t)*m(s,t)](https://www.zhihu.com/equation?tex=min+%5Csum_%7B%5Cpi%28s%2Ct%29%7Dd%28s%2Ct%29%2Am%28s%2Ct%29)
其中的![\pi](https://www.zhihu.com/equation?tex=%5Cpi)表示了s,t的所有组合形式，这个问题下，我们有9种组合形式。
然后就是下面的约束内容了，我们再定义![s(x)](https://www.zhihu.com/equation?tex=s%28x%29)表示石头处拥有石头的数量，![t(x)](https://www.zhihu.com/equation?tex=t%28x%29)表示桥需要的石头数量，那么就有
![\sum_t m(s,t)=s(s), \forall s](https://www.zhihu.com/equation?tex=%5Csum_t+m%28s%2Ct%29%3Ds%28s%29%2C+%5Cforall+s)
![\sum_s m(s,t)=t(t),\forall t](https://www.zhihu.com/equation?tex=%5Csum_s+m%28s%2Ct%29%3Dt%28t%29%2C%5Cforall+t)
怎么样，和上面的公式是不是靠近了不少？
## 升级版问题
下面我们要把问题升级一下，前面的数字都是几百几百的，和我们想要的公式不太一样，我们能不能把它们归一化一下，也就是说，把每个数字都除以它们的总和？
这样就有：
- 石头a有0.4的石头（100/250）
- 石头b有0.2的石头
……
- 桥A需要0.4的石头（100/250）
……
这样带来一个好处，我们发现它从数值上和概率值很像了：
- 首先这些值非0
- 其次它们的加和为1
- 当然还满足一些我们一般不关注的性质（次可数可加性）
嗯，专业来说这个就可以想象成概率测度函数了，我们完成了一个关键的变化，此时我们需要变换一下符号：
s(s)变成![\mu(s)](https://www.zhihu.com/equation?tex=%5Cmu%28s%29)，表示对所有石头的“概率测度”，它的空间是a,b,c三处的石头
t(t)变成![\upsilon(t)](https://www.zhihu.com/equation?tex=%5Cupsilon%28t%29)，表示对所头桥的“概率测度”，它的空间是A,B,C三处的桥
那么m(s,t)呢？看上去和我们学过的联合概率很像嘛，当然这里我们学术一点，管它叫积空间![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)，积空间上的“概率测度”函数就是之前的m(s,t)，我们现在管它叫![\gamma(s,t)](https://www.zhihu.com/equation?tex=%5Cgamma%28s%2Ct%29)
好了，到这里，我们可以再看看我们对问题的描述了：
![min_{\gamma(s,t)} \sum_{(s,t) \in \Gamma} \gamma(s,t) * d(s,t)](https://www.zhihu.com/equation?tex=min_%7B%5Cgamma%28s%2Ct%29%7D+%5Csum_%7B%28s%2Ct%29+%5Cin+%5CGamma%7D+%5Cgamma%28s%2Ct%29+%2A+d%28s%2Ct%29)
s.t.
![\sum_t\gamma(s,t)=\mu(s),\forall s](https://www.zhihu.com/equation?tex=%5Csum_t%5Cgamma%28s%2Ct%29%3D%5Cmu%28s%29%2C%5Cforall+s)
![\sum_s \gamma(s,t)=\upsilon(t),\forall t](https://www.zhihu.com/equation?tex=%5Csum_s+%5Cgamma%28s%2Ct%29%3D%5Cupsilon%28t%29%2C%5Cforall+t)
嗯，感觉离最终想要的目标不远了。我们还发现了一个现象，如果把![\gamma](https://www.zhihu.com/equation?tex=%5Cgamma)想象成一个联合分布，那么它的两个边缘分布分别就是![\mu(s)](https://www.zhihu.com/equation?tex=%5Cmu%28s%29)和![\upsilon(t)](https://www.zhihu.com/equation?tex=%5Cupsilon%28t%29)，这样两个约束条件也可以用一句话来描述了。
## 升级2.0的问题
下面完成问题解释的最后一步，前面看到的问题都是离散问题，我们能不能把它表示成连续的问题？我们有一片石头上，每一处都有若干的石头，我们有一片区域都要建桥，每个点上都可能需要石头，于是前面提到的![\mu(s)](https://www.zhihu.com/equation?tex=%5Cmu%28s%29)、![\upsilon(t)](https://www.zhihu.com/equation?tex=%5Cupsilon%28t%29)和![\gamma(s,t)](https://www.zhihu.com/equation?tex=%5Cgamma%28s%2Ct%29)全部变成了连续分布，于是问题又变成了：
![min_{\gamma(s,t)} \int_{\Gamma}d(s,t)d\gamma(s,t)](https://www.zhihu.com/equation?tex=min_%7B%5Cgamma%28s%2Ct%29%7D+%5Cint_%7B%5CGamma%7Dd%28s%2Ct%29d%5Cgamma%28s%2Ct%29)
![s.t. \gamma(s,t)](https://www.zhihu.com/equation?tex=s.t.+%5Cgamma%28s%2Ct%29)要满足边缘分布的约束
嗯，到目前为止，两边的形状可以说是几乎一样了。但是似乎还差一点，剩下的那点实际上就是“高级数学”的抽象与归纳了。关于剩下的问题，我们下次再说。
## 文章3
Wasserstein GAN可以算是GAN界的一大突破了，有关它的介绍和使用心得的文章也已经满天飞了，感兴趣的童鞋随便一搜就能好多，今天就不说太多大家说过的内容，我们从一个十分通俗的角度来看看这个目标函数究竟做了些什么。
## 一个简单的例子
如果直接去看Wasserstein metric的定义，相信对实变函数、泛函分析、测度论等数学学科不熟悉的人来说简直是云里雾里：
![W(\mu, \upsilon)=\inf_{\gamma \in \Gamma (\mu, \upsilon)} \int_{M \times M}d(x,y) d \gamma(x,y)](https://www.zhihu.com/equation?tex=W%28%5Cmu%2C+%5Cupsilon%29%3D%5Cinf_%7B%5Cgamma+%5Cin+%5CGamma+%28%5Cmu%2C+%5Cupsilon%29%7D+%5Cint_%7BM+%5Ctimes+M%7Dd%28x%2Cy%29+d+%5Cgamma%28x%2Cy%29)
这公式都说了些什么，看到就头疼，看看旁边的解释，稍微明白一点，但还是晕晕乎乎的。下面我们就从一个简单的例子开始说起，然后慢慢过渡到这个公式上来。
我们使用最优运输里面的一个小例子，假设我国的某市要修建立交桥，修高速公路需要石头，现在已经查明城市附近有几个山头有石头，我们希望把石头采集来并运输到立交桥的几个建造地点，如图所示：
![](https://pic3.zhimg.com/80/v2-5bee1722e976c9267fb7ba16ca64ad42_hd.jpg)
我们假设其他的工作都没有花费，只有运输这一步花费，那么从这些石头产生地把石头运到建桥处，怎么最省钱呢？
为了计算方便，我们需要再做一些设定：
- 石头a处有100单位石头
- 石头b处有50单位石头
- 石头c处有100单位石头
- 桥A处需要100单位石头
- 桥B处需要50单位石头
- 桥C处需要100单位石头
各个点的距离如下所示：
```
d(a,A)=10, d(a,B)=20, d(a,C)=30
d(b,A)=25, d(b,B)=25, d(b,C)=25
d(c,A)=20, d(c,B)=30, d(c,C)=10
```
我们假设每搬运1单位石头行走1单位距离，我们要花费1块钱，那么最优的搬运路径是什么，怎么最省钱？
这个问题其实也不算很复杂，我们令x[s][t]表示把s石头处运往t桥处的石头数量，那么就有下面的公式：
```
min 10*x[a][A]+20*x[a][B]+30*x[a][C]
   +25*x[b][A]+25*x[b][B]+25*x[b][C]
   +20*x[c][A]+30*x[c][B]+10*x[c][C]
s.t.
# 石头的约束
x[a][A]+x[a][B]+x[a][C]=100
x[b][A]+x[b][B]+x[b][C]=50
x[c][A]+x[c][B]+x[c][C]=100
# 桥的约束
x[a][A]+x[b][A]+x[c][A]=100
x[a][B]+x[b][B]+x[c][B]=50
x[a][C]+x[b][C]+x[c][C]=100
```
那么这个问题能不能求出最优解呢？当然可以，这道线性规划的问题就交给感兴趣的童鞋自行研究答案了。总之我们列出了公式，并给明了对应的约束条件，大家对这个问题应该已经十分清晰了。
不过话说回来，这个问题和Wasserstein有关系么？当然有。我们需要对上面的公式做一些抽象了。
首先是最小化的函数，我们用![d(s,t)](https://www.zhihu.com/equation?tex=d%28s%2Ct%29)表示石头到桥的距离，用![m(s,t)](https://www.zhihu.com/equation?tex=m%28s%2Ct%29)表示运输石头的数量，这样目标函数就变成了：
![min \sum_{\pi(s,t)}d(s,t)*m(s,t)](https://www.zhihu.com/equation?tex=min+%5Csum_%7B%5Cpi%28s%2Ct%29%7Dd%28s%2Ct%29%2Am%28s%2Ct%29)
其中的![\pi](https://www.zhihu.com/equation?tex=%5Cpi)表示了s,t的所有组合形式，这个问题下，我们有9种组合形式。
然后就是下面的约束内容了，我们再定义![s(x)](https://www.zhihu.com/equation?tex=s%28x%29)表示石头处拥有石头的数量，![t(x)](https://www.zhihu.com/equation?tex=t%28x%29)表示桥需要的石头数量，那么就有
![\sum_t m(s,t)=s(s), \forall s](https://www.zhihu.com/equation?tex=%5Csum_t+m%28s%2Ct%29%3Ds%28s%29%2C+%5Cforall+s)
![\sum_s m(s,t)=t(t),\forall t](https://www.zhihu.com/equation?tex=%5Csum_s+m%28s%2Ct%29%3Dt%28t%29%2C%5Cforall+t)
怎么样，和上面的公式是不是靠近了不少？
## 升级版问题
下面我们要把问题升级一下，前面的数字都是几百几百的，和我们想要的公式不太一样，我们能不能把它们归一化一下，也就是说，把每个数字都除以它们的总和？
这样就有：
- 石头a有0.4的石头（100/250）
- 石头b有0.2的石头
……
- 桥A需要0.4的石头（100/250）
……
这样带来一个好处，我们发现它从数值上和概率值很像了：
- 首先这些值非0
- 其次它们的加和为1
- 当然还满足一些我们一般不关注的性质（次可数可加性）
嗯，专业来说这个就可以想象成概率测度函数了，我们完成了一个关键的变化，此时我们需要变换一下符号：
s(s)变成![\mu(s)](https://www.zhihu.com/equation?tex=%5Cmu%28s%29)，表示对所有石头的“概率测度”，它的空间是a,b,c三处的石头
t(t)变成![\upsilon(t)](https://www.zhihu.com/equation?tex=%5Cupsilon%28t%29)，表示对所头桥的“概率测度”，它的空间是A,B,C三处的桥
那么m(s,t)呢？看上去和我们学过的联合概率很像嘛，当然这里我们学术一点，管它叫积空间![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)，积空间上的“概率测度”函数就是之前的m(s,t)，我们现在管它叫![\gamma(s,t)](https://www.zhihu.com/equation?tex=%5Cgamma%28s%2Ct%29)
好了，到这里，我们可以再看看我们对问题的描述了：
![min_{\gamma(s,t)} \sum_{(s,t) \in \Gamma} \gamma(s,t) * d(s,t)](https://www.zhihu.com/equation?tex=min_%7B%5Cgamma%28s%2Ct%29%7D+%5Csum_%7B%28s%2Ct%29+%5Cin+%5CGamma%7D+%5Cgamma%28s%2Ct%29+%2A+d%28s%2Ct%29)
s.t.
![\sum_t\gamma(s,t)=\mu(s),\forall s](https://www.zhihu.com/equation?tex=%5Csum_t%5Cgamma%28s%2Ct%29%3D%5Cmu%28s%29%2C%5Cforall+s)
![\sum_s \gamma(s,t)=\upsilon(t),\forall t](https://www.zhihu.com/equation?tex=%5Csum_s+%5Cgamma%28s%2Ct%29%3D%5Cupsilon%28t%29%2C%5Cforall+t)
嗯，感觉离最终想要的目标不远了。我们还发现了一个现象，如果把![\gamma](https://www.zhihu.com/equation?tex=%5Cgamma)想象成一个联合分布，那么它的两个边缘分布分别就是![\mu(s)](https://www.zhihu.com/equation?tex=%5Cmu%28s%29)和![\upsilon(t)](https://www.zhihu.com/equation?tex=%5Cupsilon%28t%29)，这样两个约束条件也可以用一句话来描述了。
## 升级2.0的问题
下面完成问题解释的最后一步，前面看到的问题都是离散问题，我们能不能把它表示成连续的问题？我们有一片石头上，每一处都有若干的石头，我们有一片区域都要建桥，每个点上都可能需要石头，于是前面提到的![\mu(s)](https://www.zhihu.com/equation?tex=%5Cmu%28s%29)、![\upsilon(t)](https://www.zhihu.com/equation?tex=%5Cupsilon%28t%29)和![\gamma(s,t)](https://www.zhihu.com/equation?tex=%5Cgamma%28s%2Ct%29)全部变成了连续分布，于是问题又变成了：
![min_{\gamma(s,t)} \int_{\Gamma}d(s,t)d\gamma(s,t)](https://www.zhihu.com/equation?tex=min_%7B%5Cgamma%28s%2Ct%29%7D+%5Cint_%7B%5CGamma%7Dd%28s%2Ct%29d%5Cgamma%28s%2Ct%29)
![s.t. \gamma(s,t)](https://www.zhihu.com/equation?tex=s.t.+%5Cgamma%28s%2Ct%29)要满足边缘分布的约束
嗯，到目前为止，两边的形状可以说是几乎一样了。但是似乎还差一点，剩下的那点实际上就是“高级数学”的抽象与归纳了。关于剩下的问题，我们下次再说。
