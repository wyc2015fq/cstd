# Numpy之reshape - qq_37385726的博客 - CSDN博客





2018年08月14日 13:21:34[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：107
所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)











# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)

# 代码

```
import numpy as np


#reshape
a=np.array([1,2,3,4])
b=a.reshape(2,2)  #a调用reshape并不会改变a的形状，是其返回值为变形后的结果
print(a)
print(b)
a.shape=(2,2)   #但对于a.shape进行赋值,a的形状就变了
print(a)
```

# 输出

[1 2 3 4]

[[1 2]

 [3 4]]

[[1 2]

 [3 4]]



