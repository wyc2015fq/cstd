# Numpy之asarray - qq_37385726的博客 - CSDN博客





2018年08月14日 14:28:07[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：138
所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)









# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)



# 代码

```python
import numpy as np


#asarray可以将元组，列表，元组列表，列表元组转化成ndarray对象

#像这种元组列表，如果每个元组的size都一样，会变成二维矩阵
a=[(1,2,3),(4,5,6)]
a=np.asarray(a)
print(a)


#像这种元组列表，由于每个元组的size不一样，所以只是一维array
b=[(1,2,3),(4,5)]
b=np.asarray(b)
print(b.shape)

#对于列表元组也是一个道理，尺寸决定论
c=([1,2,3],[4,5,6])
c=np.asarray(c)
print(c)


d=([1,2,3],[4,5])
d=np.asarray(d)
print(d.shape)

#但也可以通过reshape来修改形状
print(d.reshape(1,2))
```

# 输出

[[1 2 3]

 [4 5 6]]



(2,)



[[1 2 3]

 [4 5 6]]



(2,)



[[list([1, 2, 3]) list([4, 5])]]



