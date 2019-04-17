# NeurIPS 2018 | 腾讯AI Lab&北大提出基于随机路径积分的差分估计子非凸优化方法 - 知乎
# 



机器之心发布，作者： Cong Fang, Chris Junchi Li, Zhouchen Lin, Tong Zhang。

> 最近北京大学 ZERO 实验室与腾讯 AI Lab 提出一种新的技术：基于随机路径积分的差分估计子（SPIDER），该技术能够以更低的计算复杂度追踪许多我们感兴趣的量。该研究工作被接收为NeurIPS 2018 SPOTLIGHT（4.08%） 论文。

本文利用 SPIDER 技术求解大规模的随机非凸优化问题，在理论上本文的算法上取得的更快并在一定程度上最优的收敛速度！

**论文：Near-Optimal Non-Convex Optimization via Stochastic Path Integrated Differential Estimator**
![](https://pic4.zhimg.com/v2-336efae37ea229ea95cdea9625bc3d2b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='992' height='280'></svg>)
论文地址：[https://arxiv.org/pdf/1807.01695.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1807.01695.pdf)

具体地，我们研究如下的随机优化问题：
![](https://pic3.zhimg.com/v2-8a4e51e84c971d602640cbffe202994a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='634' height='164'></svg>)
其中当 n 有限时，我们把该问题称为有限和问题，当 n 趋于无穷时，我们把该问题称为在线问题。

由于上述问题可以是一个非凸问题，一般情况下人们很难求出该问题的全局最优解，所以往往会考虑寻求一个松弛解，例如寻求一个ɛ精度的一阶稳定点，即满足：
![](https://pic1.zhimg.com/v2-2190f2f151d1981cb9c4337820b62c0c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='251' height='62'></svg>)
对于传统的随机梯度下降法 (SGD), 理论上对于上述问题，只能获得ɛ负 4 次幂的收敛速度。当使用方差缩减技巧 (variance reduction) [1] 之后，速度可以提升到ɛ的负 3 分之 10 次幂。而本文提出的 SPIDER 技术，可以进一步将收敛速度在理论上提升到ɛ的负 3 次幂！我们将算法展示在下图算法 1 中。可以看出算法的核心在于使用随机梯度的差分的累和估计真实梯度，与使用了归一化的步长。
![](https://pic4.zhimg.com/v2-b586d151fd3cfcbb54125259e4ea17c3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1025' height='772'></svg>)
当得到了上述算法之后，我们进一步考虑是否存在理论上比该算法更快的算法。本文给出很好的回答：对于广义的该问题，在一定情况下 (n 有限) 不存在在数量级上比 SPIDER 更快的算法。具体地，本文扩展了文献 [2] 中的反例，说明了存在某个函数理论上至少需要ɛ负 3 次幂随机梯度访问才可能获得一个一阶稳定点。这即证明了 SPIDER 在一定条件下的最优性！

本文还有许多的重要扩展，读者可以在长文中 [https://arxiv.org/pdf/1807.01695.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1807.01695.pdf) 中看到，例如：

1. 对于一个更难的收敛准则，即要求算法能够逃离较明显的鞍点，找到一个二阶稳定点，本文提出了 SPIDER-SFO 算法，其收敛速度仍为ɛ的负 3 次幂。而目前所有非凸方法对于寻求二阶稳定点只能达到ɛ的负 3.5 次幂的收敛速度！下图为给算法之间的收敛速度比较：
![](https://pic4.zhimg.com/v2-04db0faab7549f383fe581c8e95a7def_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='782' height='610'></svg>)
2. 本文提出的 SPIDER 技巧非常灵活，不仅可以用于更好的追踪梯度，也可以帮助我们更好的追踪许多其他感兴趣的量，例如对于 0 阶算法，使用 SPIDER 技术，可以得到满足 d 乘以ɛ负 3 次幂收敛速度的算法（SPIDER-SZO）。而目前最快的方法收敛速度为 d 乘以ɛ负 4 次幂！

3. 本文的证明方法相对简单且易懂。证明技巧很容易被推广，例如很容易使用该文的证明技巧证明 SVRG [1] 在该问题的收敛速度。

[1] Johnson, Rie & Zhang, Tong (2013). Accelerating stochastic gradient descent using predictive variance reduction. In Advances in Neural Information Processing Systems (pp. 315–323).

[2] Carmon, Yair, Duchi, John C., Hinder, Oliver, & Sidford, Aaron (2017b). Lower bounds for finding stationary points i. arXiv preprint arXiv:1710.11606.


