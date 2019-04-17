# Numpy之itemsize - qq_37385726的博客 - CSDN博客





2018年08月14日 13:29:10[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：976
所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)









# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)



# 代码

```
import numpy as np


#itemsize输出array元素的字节数
a=np.array([1,2,3,4])
print(a.itemsize)

b=np.array([1.,2.,3.,4.])
print(b.itemsize)

c=np.array([1,2,3,4],dtype=float)
print(c.itemsize)
```

# 输出

4

8

8



