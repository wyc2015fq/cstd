# 【LDA学习系列】Beta分布Python代码 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年05月17日 10:32:37[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1237








![](https://img-blog.csdn.net/20180517103130658)

代码：

```python
# -*- coding: utf-8 -*-
'''
Created on 2018年5月15日
@author: user
@attention:  beta distribution
'''
from scipy.stats import beta
import matplotlib.pyplot as plt
import numpy as np

def test_beta_distribution():
    
    fig, ax = plt.subplots(1, 1)
    a, b = 2.31, 0.627
    #Mean(‘m’), variance(‘v’), skew(‘s’), and/or kurtosis(‘k’).
    mean, var, skew, kurt = beta.stats(a, b, moments='mvsk')
    print (mean)
    print (var)
    print (skew)
    print (kurt)
    print (beta.pdf(0.333, a, b))
    x = np.linspace(beta.ppf(0.01, a, b),beta.ppf(0.99, a, b), 100)
    ax.plot(x, beta.pdf(x, a, b), 'r-', lw=5, alpha=0.6, label='beta pdf')
    rv = beta(a, b)
    ax.plot(x, rv.pdf(x), 'k-', lw=2, label='frozen pdf')
    vals = beta.ppf([0.001, 0.5, 0.999], a, b)
    np.allclose([0.001, 0.5, 0.999], beta.cdf(vals, a, b))
    r = beta.rvs(a, b, size=1000)
    ax.hist(r, density=True, histtype='stepfilled', alpha=0.2)
    ax.legend(loc='best', frameon=False)
    plt.show()
    
    
if __name__ == '__main__': 
    
    test_beta_distribution()#beta分布
```

结果：

```
0.7865168539325842
0.04264874077027537
-1.124071486322822
0.5654574834055228
0.30981296354477267
```
![](https://img-blog.csdn.net/2018051710322996)






