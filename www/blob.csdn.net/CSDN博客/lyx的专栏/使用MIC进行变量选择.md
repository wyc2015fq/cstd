# 使用MIC进行变量选择 - lyx的专栏 - CSDN博客





2016年03月14日 20:45:05[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1891








如果有很多自变量，我们能在很多自变量中选出几个对因变量影响最大的吗?或许MIC可以解决这个问题哦。



```python
# -*- coding: utf-8 -*-
"""
Created on Mon Mar 14 19:52:57 2016

@author: Luyixiao
"""


import numpy as np
from minepy import MINE
import matplotlib.pyplot as plt

def print_stats(mine):
    print "MIC", mine.mic()
#    print "MAS", mine.mas()#detecting periodic relationships with unknown frequencies
#    print "MEV", mine.mev()
#    print "MCN (eps=0)", mine.mcn(0)
#    print "MCN (eps=1-MIC)", mine.mcn_general()

x1 = np.linspace(0, 1, 1000)
x2 = np.random.randn(x1.shape[0])+0.5
x3 = np.random.randn(x1.shape[0])#normal mean = 0,dev = 1
x4 = np.random.random_sample(x1.shape[0])#random
x5 = 2*np.random.randn(x1.shape[0])+10 #normal mean = 10,dev = 2
x6 = 4*np.random.random_sample(x1.shape[0])#random*4
y = np.sin(10 * np.pi * x1) + x2
#y = np.sin(10 * np.pi * x1) + x2*0.01
mine = MINE(alpha=0.6, c=15)

print "MIC between y and x1:"
mine.compute_score(x1, y)
print_stats(mine)
plt.scatter(x1, y,  color='red')
plt.show()
print "MIC between y and x2:"
mine.compute_score(x2, y)
print_stats(mine)
plt.scatter(x2, y,  color='blue')
plt.show()
print "MIC between y and x3(normal disturbtion):"
mine.compute_score(x3, y)
print_stats(mine)
print "MIC between y and x4(random disturbtion):"
mine.compute_score(x4, y)
print_stats(mine)
print "MIC between y and x3(normal disturbtion):"
mine.compute_score(x5, y)
print_stats(mine)
print "MIC between y and x4(random disturbtion):"
mine.compute_score(x6, y)
print_stats(mine)
```
这里，y只与x1，x2有关。函数表达式是y=sin（10*3.14*x1）+x2。

运行之后的效果是这样的。

首先是x1变量与y的MIC值

MIC between y and x1:

MIC 0.34578174965

散点图是

![](https://img-blog.csdn.net/20160314204223843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


然后是x2的：

MIC between y and x2:

MIC 0.583633971634

散点图是

![](https://img-blog.csdn.net/20160314204211718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


那么还有几个变量呢？

MIC between y and x3(normal disturbtion):

MIC 0.125493013005

MIC between y and x4(random disturbtion):

MIC 0.126849697109

MIC between y and x3(normal disturbtion):

MIC 0.134937129482

MIC between y and x4(random disturbtion):

MIC 0.147182587126

相比较而言，这几个无关变量与y的MIC值都是比较小的，不管这些无关系数的范围在什么区间。






