# 【强化学习】策略梯度policy gradient原理 - zkq_1986的博客 - CSDN博客





2018年05月22日 19:41:45[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1014








![](https://img-blog.csdn.net/20180522193533942)

eta为学习率。

R_theta的梯度为什么是R(t^n)grad(p(a_t|s_t,theta)？

首先，我们来解释下grad(p(a_t|s_t,theta)是怎么来的。先看下面的一个分类问题。

![](https://img-blog.csdn.net/20180522193541546)


从上可知，分类的损失函数采用交叉熵，最小化交叉熵相当于最大化log(y_i)

再来看为什么要乘上一个R(t^n)。从下面可知，乘上一个R(t^n)，说明回报越大的状态动作对将被训练的次数越多，即对策略theta的影响更多。

![](https://img-blog.csdn.net/20180522193551619)




