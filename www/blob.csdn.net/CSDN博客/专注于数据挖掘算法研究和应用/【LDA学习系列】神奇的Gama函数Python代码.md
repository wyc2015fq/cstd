# 【LDA学习系列】神奇的Gama函数Python代码 - 专注于数据挖掘算法研究和应用 - CSDN博客
2018年05月17日 09:40:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：214
![](https://img-blog.csdn.net/20180517094003771)
代码：
```python
# -*- coding: utf-8 -*-
'''
Created on 2018年5月15日
@author: user
@attention:  gama function
'''
import numpy as np
import matplotlib.pyplot as plt
from scipy.special import gamma, factorial 
def test_gamma_funciton():
    #整数集阶乘拓展到实数集、复数集
    #整数
    print (gamma([0, 1, 5, 10]))
    #实数
    print (gamma([0.5, 1.8, 5.9, 10.2]))
    #复数
    z = 2.5 + 1j
    print (gamma(z))
    print (gamma(z+1))
    print (z*gamma(z)) # Recurrence property)
    #计算π值
    print (gamma(0.5)**2)  # gamma(0.5) = sqrt(pi)
    #gama和阶乘示意图
    x = np.linspace(-3.5, 5.5, 2251)
    y = gamma(x)
    plt.plot(x, y, 'b', alpha=0.6, label='gamma(x)')
    k = np.arange(1, 7)
    plt.plot(k, factorial(k-1), 'k*', alpha=0.6,label='(x-1)!, x = 1, 2, ...')
    plt.xlim(-3.5, 5.5)
    plt.ylim(-10, 25)
    plt.grid()
    plt.xlabel('x')
    plt.legend(loc='lower right')
    plt.show()
    
if __name__ == '__main__': 
    
    test_gamma_funciton()#gama函数
```
结果：
```
[       inf 1.0000e+00 2.4000e+01 3.6288e+05]
[1.77245385e+00 9.31383771e-01 1.01270191e+02 5.70499028e+05]
(0.7747621045510835+0.7076312043795929j)
(1.2292740569981173+2.5438401155000685j)
(1.2292740569981158+2.543840115500066j)
3.1415926535897927
```
![](https://img-blog.csdn.net/20180517094039246)
