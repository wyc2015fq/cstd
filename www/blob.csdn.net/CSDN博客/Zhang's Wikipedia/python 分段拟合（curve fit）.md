
# python 分段拟合（curve fit） - Zhang's Wikipedia - CSDN博客


2019年03月06日 22:43:35[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：196


[https://stackoverflow.com/questions/29382903/how-to-apply-piecewise-linear-fit-in-python](https://stackoverflow.com/questions/29382903/how-to-apply-piecewise-linear-fit-in-python)
使用 np.piecewise 创建分段函数，使用 scipy.optimize.curve_fit 进行拟合：
`from scipy import optimize
import matplotlib.pyplot as plt
import numpy as np
%matplotlib inline
x = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13, 14, 15], dtype=float)
y = np.array([5, 7, 9, 11, 13, 15, 28.92, 42.81, 56.7, 70.59, 
	                 84.47, 98.36, 112.25, 126.14, 140.03])
# 一个输入序列，4个未知参数，2个分段函数
def piecewise_linear(x, x0, y0, k1, k2):
	# x<x0 ⇒ lambda x: k1*x + y0 - k1*x0
	# x>=x0 ⇒ lambda x: k2*x + y0 - k2*x0
    return np.piecewise(x, [x < x0, x >= x0], [lambda x:k1*x + y0-k1*x0, 
                                   lambda x:k2*x + y0-k2*x0])
# 用已有的 (x, y) 去拟合 piecewise_linear 分段函数
p , e = optimize.curve_fit(piecewise_linear, x, y)
xd = np.linspace(0, 15, 100)
plt.plot(x, y, "o")
plt.plot(xd, piecewise_linear(xd, *p))`np.piecewise(x, condlist, funclist, *args, **kw)condlist[i] 和 funclist[i] 一一对应，
如果 len(funclist) == len(condlist)+1，则多出来的 funclist 表示默认的情况；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190306224153661.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=,size_16,color_FFFFFF,t_70)

