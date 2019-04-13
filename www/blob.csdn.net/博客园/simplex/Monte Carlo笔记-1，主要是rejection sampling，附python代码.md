
# Monte Carlo笔记-1，主要是rejection sampling，附python代码 - simplex - 博客园|[simplex](https://www.cnblogs.com/simplex/)
|
|[博客园](https://www.cnblogs.com/)|::|[首页](https://www.cnblogs.com/simplex/)|::|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|::|[联系](https://msg.cnblogs.com/send/simplex)|::|[订阅](https://www.cnblogs.com/simplex/rss)![订阅](//www.cnblogs.com/images/xml.gif)|::|[管理](https://i.cnblogs.com/)|17 				Posts ::				0 Stories				::				5 Comments				::				0 Trackbacks|
|

|公告


|前言
|MC方法的关键在于如何对想要的分布进行采样，获得采样点。换句话说就是如何生成满足指定分布的随机数。在该系列文章中，我们有一个默认的假设就是已经有了一个能产生均匀分布随机数的机制，不管它是硬件生成的真随机数，还是算法模拟的伪随机数。关于伪随机数的生成算法，如线性同余法或者移位寄存器法，请参考文献[2]相关章节。
|MC方法基本介绍
|在概率应用中，如果某个分布|\(p(\mathbf{x})\)|的解析表达式很复杂，导致涉及到该分布的计算(如计算某函数关于|\(p\)|的期望)难以得到解析结果，那么可以用|\(m\)|个关于|分布|\(p(\mathbf{x})\)|的采样点|\(\mathbf{x}_{1},\mathbf{x}_{2},\cdots,\mathbf{x}_{m}\)|来近似替代分布|\(p(\mathbf{x})\)|。比如在求期望|\(E_{p(\mathbf{x})}[f(\mathbf{x})]\)|时，可以用|\(\sum_{i=1}^{m}f(\mathbf{x}_{i})\)|近似替代，即：
|\[E_{p(\mathbf{x})}[f(\mathbf{x})]\approx \sum_{i=1}^{m}f(\mathbf{x}_{i})\]
|这种计算积分的方法就是MC。
|另外，在计算积分|\(\int_{V}f(\mathbf{x})d\mathbf(x)\)|时，也可以利用MC方法得到近似解。
|首先将积分式重写为|\(\int_{V}f(\mathbf{x})d\mathbf(x)=V\int_{V}f(\mathbf{x})\frac{1}{V}d\mathbf(x)=E_{p(\mathbf{x})}[f(\mathbf{x})]\)|。
|其中|\(p(\mathbf{x})=\frac{1}{V}\)|为在区域|\(V\)|中的均匀分布。
|所以计算积分|\(\int_{V}f(\mathbf{x})d\mathbf(x)\)|，首先在积分区域|\(V\)|中|均匀采样|，得到|\(m\)|个采样点|\(\mathbf{x}_{1},\mathbf{x}_{2},\cdots,\mathbf{x}_{m}\)|，然后用求和|\(V\sum_{i=1}^{m}f(\mathbf{x}_{i})/m\)|代替原始积分，即
|\[\int_{V}f(\mathbf{x})d\mathbf(x)\approx V\frac{\Sigma_{i=1}^{m}f(\mathbf{x}_{i})}{m}\]
|MC方法与固定网格法的优缺点
|文献
|在一维情况下，假设采样点数为|\(m\)|，固定网格法的误差为|\(m^{-1}\)|，而MC方法的误差为|\(m^{-1/2}\)|，固定网格法的精度更高。
|在|\(N\)|维情况下，固定网格法想获得|\(m^{-1}\)|的误差，需要采样点数为|\(m^{N}\)|。而MC方法的误差永远是|\(m^{-1/2}\)|，与问题维度无关。
|链式模型的exact method
|文献 2.4节
|链式模型概率分布的两个特点：|可以用动态规划的方式求极值
|可以用动态规划的方式计算边缘分布，进而对模型进行采样。
|对高维概率分布的采样可以通过依次求各个分量的边缘分布并采样得到。
|比如对概率分布|\(\pi(x_{1},x_{2},\cdots,x_{}{d})\)|采样，可以先求出边缘分布|\(\pi(x{d})=\int\pi(x_{1},x_{2},\cdots,x{d})dx_{1}d_{x2}\cdots,dx_{d-1}\)|，然后对|\(\pi(x{d})\)|采样，得到样本点|\(x_{d}^{0}\)|。然后把|\(x_{d}^{0}\)|带入到原分布|\(\pi(x_{1},x_{2},\cdots,x_{d})\)|，得到降了一维的分布|\(\pi(x_{1},x_{2},\cdots,x_{d}^{0})\)|。重复此过程直到对所有分量都进行采样，便得到高维分布的一个采样。
|如果分布|\(\pi(x_{1},x_{2},\cdots,x_{}{d})\)|刚好是链式结构的话，按照链式结构的反方向顺序对各分量进行采样可以充分利用动态规划算法计算各边缘分布的归一化常数(配分函数)，这种计算方法又叫 forward summation backward sampling。详细的内容可以参见文献 2.4.2节
|Rejection Method
|Rejection Method可以认为是从概率密度函数（Probability density function，pdf）的定义出发构造的一种比较高效的采样方法。
|假设|\(p(\mathbf{x})\)|为一pdf，由定义可知，|\(\mathbf{x}\)|落在|\([\mathbf{x}_{0},\mathbf{x}_{0}+\Delta \mathbf{x}]\)|区间的概率即为|\(p(\mathbf{x})\)|在该区间下覆盖的面积。所以，如果我们能得到一个在|\(p(\mathbf{x})\)|覆盖的面积（记做|\(P\)|）内的(二维)均匀分布的话，那么该分布的|\(\mathbf{x}\)|坐标的概率分布就满足|\(p(\mathbf{x})\)|。
|得到上述均匀分布的一个简单方法是在一个包围|\(P\)|的区域(记做|\(Q\)|)内产生二维的均匀随机样本，然后只接受落在|\(P\)|内部的样本，落在|\(P\)|外的样本就丢弃（拒绝）。然后取这些随机样本的|\(\mathbf{x}\)|坐标，就可以得到|\(p(\mathbf{x})\)|对应的随机抽样点。
|根据以上的讨论可以发现，Rejection Method的效率与|\(P\)|和|\(Q\)|的面积之比有关，|\(Q\)|越大，则随机样本点落在|\(P\)|外部的概率也就越大，即被拒绝的概率也就越大。一个极端的例子是在整个二维平面内生成均匀分布随机样本点作为|\(Q\)|，此时，虽然理论上也能得到想要的抽样点，但是任一采样点被拒绝的概率都为1。
|所以Rejection Method的关键点就在于选择合适的|\(Q\)|，使其满足以下要求：|\(Q\)|可以完全覆盖|\(P\)
|\(Q\)|不会太大，以免影响效率
|\(Q\)|内部的均匀分布容易产生
|一般可以取|\(Q\)|同样是一个函数|\(q(\mathbf{x})=Mg(\mathbf{x})\)|覆盖的面积，其中|\(g(\mathbf{x})\)|是一个概率分布的pdf。此时，获取|\(Q\)|内的二维均匀分布样本点|\((\mathbf{x},y)\)|可以分两步来进行：|得到|\(\mathbf{x}\)|分量的分布，根据前述讨论，这是关于|\(g(\mathbf{x})\)|的分布乘以一个常量|\(M\)|。
|得到|\(y\)|分量在|\(Q\)|内的均匀分布
|其中第一步也要求|\(g(\mathbf{x})\)|的采样可以较为容易的计算出来，比如通过cdf变换的方式。
|综上，利用Rejection Method对概率分布|\(p(\mathbf{x})\)|进行抽样的流程可以归纳如下：|寻找一个概率分布|\(g(\mathbf{x})\)|，使其满足|\(Mg(\mathbf{x})\leq p(\mathbf{x})\)|对所有|\(\mathbf{x}\)|成立(比如最简单的均匀分布)
|对|\(g(\mathbf{x})\)|进行采样，得到一个采样点|\(\mathbf{x}_{0}\)
|对[0,1]上的均匀分布进行采样，得到采样点|\(y_{0}\)
|如果|\(y_{0}\leq\frac{p(\mathbf{x})}{Mg(\mathbf{x})}\)|，则接受|\(\mathbf{x}_{0}\)|，否则拒绝并转向步骤2。
|其中步骤2&3相当于是得到|\(Q\)|的均匀分布采样点，步骤4是判断该采样点是否落在|\(P\)|内
|关于Rejection Method的数学证明，可以参考文献，非常简单。
|示例代码如下：
|%matplotlib inline
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
\# use uniform distrubituion as proposal distrubituion
m = 1000000
bandwidth = 10
x = np.random.rand(m) * bandwidth
x.sort()
\# define the target distribution
mu1 = bandwidth * 0.25
mu2 = bandwidth * 0.75
sigma1 = 0.5
sigma2 = 1
p = 0.4 / (np.sqrt(2 * np.pi) * sigma1**2) * np.exp(-0.5 * (
    x - mu1)**2 / sigma1**2) + 0.5 / (np.sqrt(2 * np.pi) * sigma2**2) * np.exp(
        -0.6 * (x - mu2)**2 / sigma2**2)
\# compute the reject propability
M = 10
r = p / M * bandwidth
\# generate the samples
x2 = np.random.rand(m)
accepted_samples_x = x[x2 < r]
\# plot
n, bins, patches = plt.hist(
    accepted_samples_x,
    bins=100,
    normed=True,
    color=sns.desaturate("indianred", .8),
    label="sample  distribution")
plt.plot(x, p / max(p) * max(n), 'blue', label="target distribution",linewidth=4)
plt.legend(fontsize=15)
plt.show()
|Importance Sampling
|importance sampling的目的是为了计算积分|\(E_{\pi}[h(\mathbf{x})]=\int h(\mathbf{x})\pi(\mathbf{x})\)|。和rejection method不同，importance sampling本身并不能也不期望得到分布|\(\pi(\mathbf{x})\)|的采样，而是转而去对一个很容易采样的分布|\(g(\mathbf{x})\)|进行采样，然后在计算积分的时候加考虑采样点各自的权重，以逼近真实的积分。这样做的好处是除了可以避免对复杂分布|\(\pi(\mathbf{x})\)|采样以外，还能通过设计|\(g(\mathbf{x})\)|的形式，使得采样点更多的出现在对积分贡献大的区域，以提高计算的精度和效率。
|基本的importance sampling原理可以表示如下：
|\[E_{\pi}[h(\mathbf{x})]=\int h(\mathbf{x})\pi(\mathbf{x})=\int h(\mathbf{x})g(\mathbf{x})\frac{\pi(\mathbf{x})}{g(\mathbf{x})}=E_{g}[\omega h(\mathbf{x})]\]
|其中|\(\omega=\frac{\pi(\mathbf{x})}{g(\mathbf{x})}\)|为权重，|\(g(\mathbf{x})\)|是用来近似|\(h(\mathbf{x})\pi(\mathbf{x})\)|的分布。
|结合Rejection Method和Importance Sampling
|利用Rejection Method得到采样分布并计算积分的缺点是要选择合适的|\(M\)|才能得到较高的效率，因为平均每|\(M\)|个采样点才会有一个被接受。但是|\(M\)|又不能选择的太小以避免在某些区域出现|\(Mg(\mathbf{x})< p(\mathbf{x})\)|的情况。
|而利用Importance Sampling计算积分时，虽然对测试分布没有什么要求（这点和Rejection Method不太一样，Rejection Method要求测试分布|\(g(\mathbf{x})\)|一定要满足|\(Mg(\mathbf{x})\leq p(\mathbf{x})\)|），但是如果测试分布与目标分布的差别非常大，那么在计算权重时就会出现大多数采样点的权重都非常小的情况，从而也会影响计算效率
|为了解决以上问题，发展出了一种综合Rejection Method和Importance Sampling的方法，即Rejection Control。
|Rejection Control可以认为是结合了Rejection Method的Importance Sampling方法。假设需要对分布|\(\pi(\mathbf{x})\)|进行采样，得到|\(m\)|个采样点|\(\mathbf{x}_{1},\mathbf{x}_{2},\cdots,\mathbf{x}_{m}\)|，并用它计算某个积分|\(E_{\pi}[h(\mathbf{x})]=\int h(\mathbf{x})\pi(\mathbf{x})\)|。根据Importance Sampling的思路，可以选取一个测试分布|\(g(\mathbf{x})\)|，对其进行采样，然后给每个采样点|\(\mathbf{x}_{j}\)|赋一个权重|\(w_{i}=\frac{\pi(\mathbf{x})}{g(\mathbf{x})}\)|用于计算积分|\(E_{\pi}[h(\mathbf{x})]\)|。
|与普通Importance Sampling不同的是，在采样的时候，增加以下步骤：|以概率|\(r_{i}=\textrm{min}\{1,\frac{w_{i}}{c}\}\)|接受该采样点，其中|\(c\)|是一固定常数
|如果某个采样点|\(\mathbf{x}_{i}\)|被接受，那么其权重|\(w_{i}\)|更新为|\(\bar{w}_{i}=q_{c}\frac{w_{i}}{r_{i}}\)|，其中|\(q_{c}=\int \textrm{min}\{1,\frac{w}{c}\}g(\mathbf{x})d\mathbf{x}\)|为对所有采样值都相同的常数。如果只是计算积分的话，该常数可以忽略，因为根据中(2.4)式，对所有采样点都相同的常量会被约掉。
|Rejection Control方法也可以认为是Rejection Method的改良版，利用采样点权重|\(w\)|来衡量测试分布与目标分布的差距，并据此更新测试分布。根据|\(w_{i}=\frac{\pi(\mathbf{x})}{g(\mathbf{x})}\)|可得
|\[\bar{g}(\mathbf{x})=\frac{\pi(\mathbf{x})}{\bar{w}}=\frac{\pi(\mathbf{x})}{q_{c}\frac{w}{r}}=\frac{\pi(\mathbf{x})r}{q_{c}w}=\frac{\textrm{min}\{\pi(\mathbf{x}),\frac{\pi(\mathbf{x})w}{C}\}}{q_{c}w}=q_{c}^{-1}\textrm{min}\{g(\mathbf{x}),\frac{\pi(\mathbf{x})}{c}\}\]
|可见，当|\(q_{c}=c=1\)|时，|\(g(\mathbf{x})\)|在某些地方会等于|\(\pi(\mathbf{x})\)|。
|参考文献
|[1] Monte Carlo Strategies in Scientific Computing
|[2] Numerical recipes
|[3] Monte Carlo Statistical Methods
|[4] Machine learning: a probabilistic perspective





|posted on|2017-04-06 00:14|[simplex](https://www.cnblogs.com/simplex/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=6671337)|[收藏](#)


|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
Copyright @
	simplex
Powered by:[.Text](http://scottwater.com/blog)and[ASP.NET](http://asp.net)
Theme by:[.NET Monster](http://www.DotNetMonster.com)
