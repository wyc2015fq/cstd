# 【Python-ML】最小二乘法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月27日 12:18:36[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：184
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)









![](https://img-blog.csdn.net/20180127121738930?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180127121747335?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3、python代码参考：



```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月27日
@author: Jason.F
@summary: 最小二乘法Least Square Method，分类回归算法的基础，通过最小化误差的平方和寻找数据的最佳函数匹配。
利用最小二乘法可以简便地求得未知的数据，并使得这些求得的数据与实际数据之间误差的平方和为最小。
最小二乘法还可用于曲线拟合。其他一些优化问题也可通过最小化能量或最大化熵用最小二乘法来表达。
'''
     
import numpy as np  
import pylab as pl
from scipy.optimize import leastsq  # 引入最小二乘函数

n = 9  # 多项式次数
# 目标函数
def real_func(x):
    return np.sin(2 * np.pi * x)
# 多项式函数
def fit_func(p, x):
    f = np.poly1d(p)
    return f(x)
# 残差函数
def residuals_func(p, y, x):
    ret = fit_func(p, x) - y
    return ret

x = np.linspace(0, 1, 9)  # 随机选择9个点作为x
x_points = np.linspace(0, 1, 1000)  # 画图时需要的连续点

y0 = real_func(x)  # 目标函数
y1 = [np.random.normal(0, 0.1) + y for y in y0]  # 添加正太分布噪声后的函数

p_init = np.random.randn(n)  # 随机初始化多项式参数

plsq = leastsq(residuals_func, p_init, args=(y1, x))

print 'Fitting Parameters: ', plsq[0]  # 输出拟合参数

pl.plot(x_points, real_func(x_points), label='real')
pl.plot(x_points, fit_func(plsq[0], x_points), label='fitted curve')
pl.plot(x, y1, 'bo', label='with noise')
pl.legend()
pl.show()
```
结果：



```
Fitting Parameters:  [ -1.42410177e+03   6.58200072e+03  -1.23529559e+04   1.20605212e+04
  -6.50274314e+03   1.90476748e+03  -2.88678344e+02   2.10107377e+01
   1.92561591e-01]
```
![](https://img-blog.csdn.net/20180127121826718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




