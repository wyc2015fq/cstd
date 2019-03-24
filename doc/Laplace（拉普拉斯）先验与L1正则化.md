# Laplace（拉普拉斯）先验与L1正则化

在之前的一篇博客中[L1正则化及其推导](http://www.cnblogs.com/heguanyou/p/7582578.html)推导证明了L1正则化是如何使参数稀疏化人，并且提到过L1正则化如果从贝叶斯的观点看来是Laplace先验，事实上如果从贝叶斯的观点，所有的正则化都是来自于对参数分布的先验。现在来看一下为什么Laplace先验会导出L1正则化，也顺便证明Gauss（高斯）先验会导出L2正则化。



## 最大似然估计

很多人对最大似然估计不明白，用最简单的线性回归的例子来说：如果有数据集$(X,Y)$，并且$Y$是有白噪声（就是与测量得到的$Y$与真实的$Y_{real}$有均值为零的高斯分布误差），目的是用新产生的$X$来得到$Y$。如果用线性模型来测量，那么有：
$$
f(X) = \sum_i(x_i\theta_i) + \epsilon = X\theta^T + \epsilon \tag{1.1}
$$
其中$X=(x_1,x_2...x_n)$，$ϵ$是白噪声，即$\epsilon \sim N(0, \delta^2)$。那么于一对数据集$(X_i, Y_i)$来用，在这个模型中用$X_i$得到$Y_i$的概率是$Y_i \sim N(f(X_i), \delta^2)$：
$$
P(Y_i|X_i, \theta) = \frac{1}{\delta\sqrt{2\pi}} \exp(-\frac{\|f(X_i) - Y_i\|^2}{2\delta^2}) \tag{1.2}
$$
假设数据集中每一对数据都是独立的，那么对于数据集来说由X得到Y的概率是：
$$
P(Y|X,\theta)= \prod_i\frac{1}{\delta\sqrt{2\pi}} \exp(-\frac{\|f(X_i) - Y_i\|^2}{2\delta^2}) \tag{1.3}
$$
根据决策论，就可以知道可以使概率$P(Y|X,θ)$最大的参数$θ$就是最好的参数。那么我们可以直接得到最大似然估计的最直观理解：**对于一个模型，调整参数$θ$，使得用X得到Y的概率最大**。那么参数$θ$就可以由下式得到:
$$
\begin {split}
\theta^* &= argmax_{\theta} \left(\prod_i\frac{1}{\epsilon\sqrt{2\pi}} \exp(-\frac{\|f(X_i) - Y_i\|^2}{2\delta^2})\right) \cr
&=argmax_{\theta} \left( -\frac{1}{2\delta^2} \sum_i \|f(X_i) - Y_i\|^2 +　\sum_i ln(\delta\sqrt{2\pi}) \right) \cr
&=argmin_{\theta} \left(\sum_i \|f(X_i) - Y_i\|^2 \right)
\end {split} \tag{1.4}
$$
这个就是最小二乘计算公式。



## Laplace分布

Laplace概率密度函数分布为：
$$
f(x|\mu, b) = \frac{1}{2b} \exp(-\frac{|x-\mu|}{b}) \tag{2.1}
$$
分布的图像如下所示：

![img](https://images2017.cnblogs.com/blog/1191997/201710/1191997-20171018191512974-1322748219.jpg)

图1 Laplace分布

可以看到Laplace分布集中在$μ$附近，而且b越小，数据的分布就越集中。



## Laplace先验导出L1正则化

先验的意思是对一种未知的东西的假设，比如说我们看到一个正方体的骰子，那么我们会假设他的各个面朝上的概率都是1/6，这个就是先验。但事实上骰子的材质可能是密度不均的，所以还要从数据集中学习到更接近现实情况的概率。同样，在机器学习中，我们会根据一些已知的知识对参数的分布进行一定的假设，这个就是先验。有先验的好处就是可以在较小的数据集中有良好的泛化性能，当然这是在先验分布是接近真实分布的情况下得到的了，从信息论的角度看，向系统加入了正确先验这个信息，肯定会提高系统的性能。我们假设参数θ是如下的Laplace分布的，这就是Laplace先验：
$$
P(\theta_i) = \frac{\lambda}{2} \exp(-\lambda|\theta_i|) \tag{3.1}
$$
其中λ是控制参数θ集中情况的超参数，λ越大那么参数的分布就越集中在0附近。

在前面所说的最大似然估计事实上是假设了θ是均匀分布的，也就是$P(\theta)=Constant$，我们最大化的要后验估计，即是：
$$
\begin {split}
\theta^* &= argmax_{\theta} \left(\prod_i P(Y_i|X_i, \theta) \prod_i P(\theta_i)\right) \cr
&=argmin_{\theta} \left(\sum_i \|f(X_i) - Y_i\|^2  + \sum_i \ln(P(\theta_i))\right)
\end {split} \tag{3.2}
$$
如果是Laplace先验，将式(3.1)代入到式(3.2)中可得：
$$
\theta^* =argmin_{\theta} \left(\sum_i \|f(X_i) - Y_i\|^2  + \lambda \sum_i |\theta_i|)\right) \tag{3.3}
$$
这就是由Laplace导出L1正则化，我在之前的一篇博客中[L1正则化及其推导](http://www.cnblogs.com/heguanyou/p/7582578.html)分析过λ越大，那么参数的分布就越集中在0附近，这个与Laplace先验的分析是一致的。

### Gauss先验导出L2正则化

到这里，我们可以很轻易地导出L2正则化，假设参数θ的分布是符合以下的高斯分布：
$$
P(\theta_i) = \frac{\lambda}{\sqrt{\pi}} \exp(-\lambda\|\theta_i\|^2) \tag{3.4}
$$
代入式(3.2)可以直接得到L2正则化：
$$
\theta^* =argmin_{\theta} \left(\sum_i \|f(X_i) - Y_i\|^2  + \lambda \sum_i \|\theta_i\|^2)\right) \tag{3.5}
$$
