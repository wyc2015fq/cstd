# np.r_ np.c_ - YZXnuaa的博客 - CSDN博客
2018年03月13日 10:35:16[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：63
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)
1. 代码
import numpy as np
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])
c = np.c_[a, b]
print(np.r_[a, b])
print(np.c_[c, a])
2. 结果
np.r_: 
 [1 2 3 4 5 6]
np.c_:
 [[1 4 1]
 [2 5 2]
 [3 6 3]]
