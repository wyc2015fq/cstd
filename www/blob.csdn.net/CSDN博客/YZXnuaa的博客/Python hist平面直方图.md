# Python hist平面直方图 - YZXnuaa的博客 - CSDN博客
2019年04月18日 21:49:02[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)
二、demos
from sklearn.datasets import load_iris
import matplotlib.pyplot as plt
import numpy as np
iris = load_iris()
data = iris.data
target = iris.target
print(data[:, 0])
print(target)
num_bins = 30
plt.hist(data[:, 0], num_bins,)
# plt.axis([-1, 1, -1, 1])
plt.show()
