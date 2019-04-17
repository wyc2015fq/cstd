# Numpy之ndarray - qq_37385726的博客 - CSDN博客





2018年08月14日 13:14:16[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：101
所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)









# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)



# 代码

```
import numpy as np


#ndarray类型


#创建一个含4个元素的数组，不进行矩阵操作
arr=np.array([1,2,3,4])
print(arr)

#创建1X4的矩阵
a=np.array([[1,2,3,4]])
print(a)

#创建1X4的矩阵，指定dtype为float，没有double...
b=np.array([[1,2,3,4]],dtype=float)
print(b)

#创建2X4的矩阵，一个[]是一行
c=np.array([[1,2,3,4],[5,6,7,8]])
print(c)

#ndmin指定data的最小维数,原来本来是数组的，由于最小维数变成矩阵
d=np.array([1,2,3,4],ndmin=2)
print(d)
```

# 输出

[1 2 3 4]

[[1 2 3 4]]

[[1. 2. 3. 4.]]

[[1 2 3 4]

 [5 6 7 8]]

[[1 2 3 4]]



