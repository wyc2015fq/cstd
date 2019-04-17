# Numpy之shape - qq_37385726的博客 - CSDN博客





2018年08月14日 12:59:01[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：90
所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)











# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)

# 代码

```
import numpy as np


#1X4的矩阵
a=np.array([[1,1,1,0]])
print(a.shape)

#4X1的矩阵
b=np.array([[1],[2],[3],[4]])
print(b.shape)

#几乎不能叫矩阵，因为.shape输出的是元素的个数
c=np.array([1,2,3,4])
print(c.shape)

#单个数字,.shape输出空
d=2
print(np.shape(d))
# print(d.shape)  报错，因为单个数字不存在shape函数
```

# 输出

(1, 4)

(4, 1)

(4,)

()

()





