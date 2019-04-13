
# Monte Carlo笔记-2：MCMC，附简易python代码 - simplex - 博客园|[simplex](https://www.cnblogs.com/simplex/)
|
|[博客园](https://www.cnblogs.com/)|::|[首页](https://www.cnblogs.com/simplex/)|::|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|::|[联系](https://msg.cnblogs.com/send/simplex)|::|[订阅](https://www.cnblogs.com/simplex/rss)![订阅](//www.cnblogs.com/images/xml.gif)|::|[管理](https://i.cnblogs.com/)|17 				Posts ::				0 Stories				::				5 Comments				::				0 Trackbacks|
|

|公告


|Markov chain
|基本概念
|当随机变量序列\(\{\mathbf{x}_{1},\mathbf{x}_{2},\cdots,\mathbf{x}_{n}\}\)的联合分布满足：
|\(p(\mathbf{x}_{1},\mathbf{x}_{2},\cdots,\mathbf{x}_{n})=p(\mathbf{x}_{1})\prod_{t=2}^{n}p(\mathbf{x}_{t}|\mathbf{x}_{t-1})\)
|时，称其为Markov model或者Markov chain。特点是\(\mathbf{x}_{t}\)包含了所有过去的信息，下一随机变量
|\(\mathbf{x}_{t+1}\)的分布完全由\(\mathbf{x}_{t}\)决定，\(p(\mathbf{x}_{t}|\mathbf{x}_{1:t-1})=p(\mathbf{x}_{t}|\mathbf{x}_{t-1})\)。
|\(\mathbf{x}_{t}\)的边缘分布为：
|\(\mathbf{x}_{t}=\int_{\mathbf{x}_{1:t-1}}p(\mathbf{x}_{1})\prod_{t=2}^{n}p(\mathbf{x}_{t}|\mathbf{x}_{t-1})d\mathbf{x}_{1}d\mathbf{x}_{2}...d\mathbf{x}_{t-1} \)
|当\(\mathbf{x}_{t}\)的值域为有限集合\(\{1,2,\cdots,K\}\)时，这种离散的马尔可夫链可以用\(K\times K\)跃迁矩阵\(A\)表示，其中\(A_{ij}=p(\mathbf{x}_{t}=j|\mathbf{x}_{t-1}=i)\)表示从状态\(i\)跃迁到状态\(j\)的概率。
|另外，用\(A(n)\)表示\(n\)步跃迁，即\(A_{ij(n)}=p(\mathbf{x}_{t+n}=j|\mathbf{x}_{t}=i)\)。

|对于离散情况，利用跃迁矩阵\(A\)，可以将\(\mathbf{x}_{t}\)的边缘分布表示：
|\(\mathbf{x}_{t}=\int_{\mathbf{x}_{1:t-1}}p(\mathbf{x}_{1})\prod_{t=2}^{n}p(\mathbf{x}_{t}|\mathbf{x}_{t-1})d\mathbf{x}_{1}d\mathbf{x}_{2}...d\mathbf{x}_{t-1}=p(\mathbf{X}_{1})A\times A...\times A=p(\mathbf{X}_{1})A^{n-1}\)
|上式利用了关系\(p(\mathbf{x}_{t})=\int p(\mathbf{x}_{t}|\mathbf{x}_{t-1})d\mathbf{x}_{t-1}=p(\mathbf{x}_{t-1})A\)
|Markov chain的性质
|MCMC主要利用的是满足某些条件的Markov chain具有stationary distribution的性质。这一部分可以参考文献 [4] 的17.2.3节（简单易懂）以及文献[3]的第6章（更详细）。
|MCMC方法用到的一个定理（文献[4], Theorem 17.2.3）：
|If a Markov chain with transition matrix \(A\) is /regular/  and satisfies /detailed balance/ wrt distribution \(\pi\), then \(\pi\) is a stationary distribution of the chain.
|MCMC
|MCMC相比 Reject Sampling and Importance Sampling的主要优点是MCMC在高维的情况下的效果更好。MCMC的基本思路是，已知待采样的分布\(\pi(\mathbf{x})\)，通过构造跃迁核\(A(\mathbf{x},\mathbf{x}')\)，得到一个马尔可夫链\(M\)。\(M\)具有规则性(regular)并且对\(\pi(\mathbf{x})\)满足detailed balance条件。这样，\(M\)具有稳态分布\(\pi(\mathbf{x})\)。对\(M\)依次进行采样，得到采样点序列\(\{\mathbf{x}_{1},\mathbf{x}_{2},\cdots,\mathbf{x}_{n},\cdots\}\)。当采样点数\(n\)足够大时，可以认为\(p（\mathbf{x}_{m>n}）\)已经进入了稳态分布，从而\(\mathbf{x}_{m>n}\)就是从稳态分布\(\pi(\mathbf{x})\)得到的采样。注意这样得到的采样点是关联的（通常非常相关），这一点和reject sampling不同。
|上述“对\(M\)进行采样”就是对分布\(A(\mathbf{x}=\mathbf{x}_{0},\mathbf{x}')\)进行采样，其中\(\mathbf{x}_{0}\)是当前已经得到的采样点。
|在离散情况下，\(A(\mathbf{x},\mathbf{x}')\)退化成一个矩阵，分布\(A(\mathbf{x}=\mathbf{x}_{0}=j,x')\)就是矩阵\(A\)的第\(j\)行定义的概率分布\(p(\mathbf{x}_{t+1}|\mathbf{x}_{t}=j)\)
|在具体实现的时候，关于\(A\)的构造和采样可能很难实现。我们可以采用类似于Reject Sampling的方法：选择一个很简单的分布（proposal distribution）\(T=q(\mathbf{x}_{t+1}|\mathbf{x}_{t})\)进行采样，然后再对采样点进行判断，以概率\(r\)接受该采样。这样一来，\(A=Tr\)由两部分组成，可以设计\(r\)的形式（类似Reject Sampling，\(r\)一般由\(\pi\)和\(T\)同时决定），使\(A\)满足上文所述的两个条件。典型的此类算法有Metropolis算法和Metropolis-Hastings算法。
|Metropolis algorithm的步骤如下：
|1选择proposal distribution \(T=q(\mathbf{x}_{t+1}|\mathbf{x}_{t})\)。\(T\)的常见选择可以参考random-walk Metropolis、Metropolized independent sampler等等
|令\(t=0\)，任选一个初始采样点\(\mathbf{x}_{0}\)，比如从均匀分布中采样得到一个样本
|对分布\(T\)进行采样，得到候选采样点\(\bar{\mathbf{x}}\)
|计算\(r=min\{1,\frac{p(\bar{\mathbf{x}})}{p(\mathbf{x}_{t})}\}\)
|以概率\(r\)接受候选采样点\(\bar{\mathbf{x}}\)。如果接受，则\(\mathbf{x}_{t+1}=\bar{\mathbf{x}}\);如果拒绝，则\(\mathbf{x}_{t+1}=\bar{\mathbf{x}}\)
|重复3-5直到得到所需采样点数
|Metropolis算法要求proposal distribution满足对称性，从而增加了proposal distribution的选取难度。Hastings对Metropolis算法做了改进，取消了proposal distribution必须对称的要求，该算法即Metropolis-Hastings，具体细节可以参见[3]或[1]。
|Gibbs算法
|Gibbs算法可以认为是一类特殊的MCMC算法，在统计物理中被广泛应用。从形式上来看，Gibbs算法把对一个\(N\)维变量的采样分解为\(N\)个一维的采样，即每次对变量的一个坐标进行采样，得到一个采样点需要进行\(N\)次采样过程。可以证明Gibbs算法每次采样的接受率都是1，即从来不会拒绝采样点。
|Gibbs算法的突出优点是它的每一步都是在进行一维采样，这一般是非常简单的。比如在概率图模型中的使用partical based inference方法计算某些概率分布时，就可以利用Gibb算法方便的获得采样点。因为进行采样时除了待采样变量，其它变量的值都是固定的，可以很方便的求出待采样变量的一维分布，然后进行采样。

|下面是Metropolis算法和Gibbs采样算法的一个简单例子，来自|[Gibbs sampler](https://people.duke.edu/~ccc14/sta-663/MCMC.html#gibbs-sampler)
|1|\#|-*- coding: utf-8 -*-|2|3|"""|4|利用MCMC算法对两维分布采样|5|问题描述：|6|给出两个硬币A、B，进行抛硬币实验，其中A硬币共抛掷N_1次，正面向上次数z_1次，B硬币共抛掷N_2次，正面向上次数z_2次。|7|现在估计两个硬币正面向上的概率θ_1和θ_2，即p(θ|X)=p(θ_1, θ_2|N_1, z_1, N_2, z_2)。|8|已知：|9|先验分布p(θ)是Beta分布，|10|似然p(X|θ)是Bernoulli分布|11|后验分布p(θ|X)是Beta分布|12|两个硬币独立进行实验，所以概率可以表示成两个硬币各自分布乘积的形式|13|14|source: https://people.duke.edu/~ccc14/sta-663/MCMC.html\#gibbs-sampler|15|16|"""|17|import|sys|18|import|glob|19|import|numpy as np|20|import|pandas as pd|21|import|scipy.stats as stats|22|import|matplotlib.pyplot as plt|23|from|mpl_toolkits.mplot3d|import|Axes3D|24|from|functools|import|partial|25|26|27|def|bern(theta, z, N):|28|"""|Bernoulli likelihood with N trials and z successes.|"""|29|return|np.clip(theta**z * (1-theta)**(N-z), 0, 1|)|30|31|32|def|bern2(theta1, theta2, z1, z2, N1, N2):|33|"""|Bernoulli likelihood with N trials and z successes.|"""|34|return|bern(theta1, z1, N1) *|bern(theta2, z2, N2)|35|36|37|def|make_thetas(xmin, xmax, n):|38|xs =|np.linspace(xmin, xmax, n)|39|widths = (xs[1:] - xs[:-1])/2.0|40|thetas = xs[:-1] +|widths|41|return|thetas|42|43|44|def|make_plots(X, Y, prior, likelihood, posterior, projection=|None):|45|fig, ax = plt.subplots(1, 3, subplot_kw=|dict(|46|projection=projection, aspect=|'|equal|'|), figsize=(12, 3|))|47|if|projection ==|'|3d|'|:|48|ax[0].plot_surface(X, Y, prior, alpha=0.3, cmap=|plt.cm.jet)|49|ax[1].plot_surface(X, Y, likelihood, alpha=0.3, cmap=|plt.cm.jet)|50|ax[2].plot_surface(X, Y, posterior, alpha=0.3, cmap=|plt.cm.jet)|51|else|:|52|ax[0].contour(X, Y, prior)|53|ax[1|].contour(X, Y, likelihood)|54|ax[2|].contour(X, Y, posterior)|55|ax[0].set_title(|'|Prior|'|)|56|ax[1].set_title(|'|Likelihood|'|)|57|ax[2].set_title(|'|Posteior|'|)|58|plt.tight_layout()|59|60|61|thetas1 = make_thetas(0, 1, 101|)|62|thetas2 = make_thetas(0, 1, 101|)|63|X, Y =|np.meshgrid(thetas1, thetas2)|64|65|a = 2|66|b = 3|67|68|z1 = 11|69|N1 = 14|70|z2 = 7|71|N2 = 14|72|73|74|\#|Analytical|75|76|prior = stats.beta(a, b).pdf(X) *|stats.beta(a, b).pdf(Y)|77|likelihood =|bern2(X, Y, z1, z2, N1, N2)|78|posterior = stats.beta(a + z1, b + N1 - z1).pdf(X) *|\|79|stats.beta(a + z2, b + N2 -|z2).pdf(Y)|80|make_plots(X, Y, prior, likelihood, posterior)|81|make_plots(X, Y, prior, likelihood, posterior, projection=|'|3d|'|)|82|83|84|\#|采样算法|85|def|prior(theta1, theta2):|86|return|stats.beta(a, b).pdf(theta1) *|stats.beta(a, b).pdf(theta2)|87|88|89|lik = partial(bern2, z1=z1, z2=z2, N1=N1, N2=|N2)|90|91|92|def|target(theta1, theta2):|93|return|prior(theta1, theta2) *|lik(theta1, theta2)|94|95|96|theta = np.array([0.5, 0.5|])|97|niters = 10000|98|burnin = 500|99|sigma = np.diag([0.2, 0.2|])|100|101|\#|Metropolis采样计算p(θ|X)|102|103|thetas = np.zeros((niters-burnin, 2|), np.float)|104|for|i|in|range(niters):|105|new_theta =|stats.multivariate_normal(theta, sigma).rvs()|106|p = min(target(*new_theta)/target(*theta), 1|)|107|if|np.random.rand() <|p:|108|theta =|new_theta|109|if|i >=|burnin:|110|thetas[i-burnin] =|theta|111|112|113|kde =|stats.gaussian_kde(thetas.T)|114|XY =|np.vstack([X.ravel(), Y.ravel()])|115|posterior_metroplis =|kde(XY).reshape(X.shape)|116|make_plots(X, Y, prior(X, Y), lik(X, Y), posterior_metroplis)|117|make_plots(X, Y, prior(X, Y), lik(X, Y), posterior_metroplis, projection=|'|3d|'|)|118|119|120|\#|Gibbs采样计算p(θ|X)|121|122|thetas = np.zeros((niters-burnin, 2|), np.float)|123|for|i|in|range(niters):|124|\#|θ是二维的，所以要进行两次采样，由于两个参数互相独立，所以采样实际上是各自进行的|125|\#|接受率为1|126|theta = [stats.beta(a + z1, b + N1 - z1).rvs(), theta[1|]]|127|theta = [theta[0], stats.beta(a + z2, b + N2 -|z2).rvs()]|128|129|if|i >=|burnin:|130|thetas[i-burnin] = theta

|MCMC和Importance Sampling思路的区别
|这两者都是为了解决针对复杂分布的积分问题。由于目标分布太过于复杂，不但解析形式的积分没法计算，连对其采样都很困难。MCMC的思路是通过构造马尔可夫链的方式，得到 *目标分布* 的 *关联* 的样本，而Importance Sampling的思路则是转而对一个简单的分布（ *测试分布* ）进行采样，得到 *独立* 的样本，然后在计算积分的时候对采样点分配适当的权重，从而得到对原始积分的近似解。
|MCMC的收敛性讨论
|使用MCMC时要注意的重要一点就是要等到MC收敛到稳态分布之后再进行真正的采样，之前的采样点都弃之不用，这个过程叫做 burn in。但是在实际使用中，MCMC到底何时进入稳态是难以判定的，文献[4]第24.4.3节有一些这方面的讨论，介绍了定性的trace plot方法和定量的Estimated potential scale reduction (EPSR)方法，可以做参考。
|另外，在使用MCMC时，究竟是选择一条很长的MC进行采样以减少burn in时间还是选择多条短的MC（优点？），文献[4]也给出了一个经验选择：用中等数量的中等长度的MC（比如3条MC，每条长度为\(10^{6}\)）。
|参考文献
|[1] Monte Carlo Strategies in Scientific Computing
|[2] Numerical recipes
|[3] Monte Carlo Statistical Methods
|[4] Machine learning: a probabilistic perspective





|posted on|2017-04-27 23:48|[simplex](https://www.cnblogs.com/simplex/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=6777957)|[收藏](#)


|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
Copyright @
	simplex
Powered by:[.Text](http://scottwater.com/blog)and[ASP.NET](http://asp.net)
Theme by:[.NET Monster](http://www.DotNetMonster.com)
