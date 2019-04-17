# 【LDA学习系列】Gibbs采样python代码 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年05月18日 15:41:48[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1656








Gibbs采样算法流程：从已知分布采样，前提是预知条件分布

![](https://img-blog.csdn.net/20180518152732331)

![](https://img-blog.csdn.net/20180518152833403)

代码流程：


![](https://img-blog.csdn.net/20180518152825963)

代码：

```python
# -*- coding: utf-8 -*-
'''
Created on 2018年5月15日
@author: user
@attention:  Gibbs Sampling利用条件概率产生符合分布的样本，用于估计分布的期望，边缘分布；是一种在无法精确计算情况下，用计算机模拟的方法。
'''

import random
import math
import matplotlib.pyplot as plt

def xrange(x):
    return iter(range(x))

def p_ygivenx(x, m1, m2, s1, s2):
    return (random.normalvariate(m2 + rho * s2 / s1 * (x - m1), math.sqrt(1 - rho ** 2) * s2))

def p_xgiveny(y, m1, m2, s1, s2):
    return (random.normalvariate(m1 + rho * s1 / s2 * (y - m2), math.sqrt(1 - rho ** 2) * s1))

N = 5000
K = 20
x_res = []
y_res = []
m1 = 10
m2 = -5
s1 = 5
s2 = 2

rho = 0.5
y = m2

for i in xrange(N):
    for j in xrange(K):
        x = p_xgiveny(y, m1, m2, s1, s2)
        y = p_ygivenx(x, m1, m2, s1, s2)
        x_res.append(x)
        y_res.append(y)

num_bins = 50
plt.hist(x_res, num_bins, normed=1, facecolor='green', alpha=0.5)
plt.hist(y_res, num_bins, normed=1, facecolor='red', alpha=0.5)
plt.title('Histogram')
plt.show()
```

结果：

![](https://img-blog.csdn.net/20180518152925751)

总结下MCMC的理解：


MCMC采样的思想是：p(x)直接采样存在困难，借助已知可采样的分布 q(x) ，如高斯分布，然后按照一定的方法拒绝q(x)采样的样本，达到接近 p(x) 分布的目的。而基于可采样分布q(x)是利用马氏链细致平稳性来满足的，收敛后的样本即接近来自p(x)产生的样本。

MCMC算法：

![](https://img-blog.csdn.net/20180518153810526)


由于可能非常的小，比如0.1，导致大部分的采样值都被拒绝转移，采样效率很低，因此提出M-H算法。

![](https://img-blog.csdn.net/20180518153942398)

主要改造在接受率上，这个时候如果知道条件分布，那么接受率可以直接设置为1，即为Gibbs采样算法。

![](https://img-blog.csdn.net/20180518154138350)




