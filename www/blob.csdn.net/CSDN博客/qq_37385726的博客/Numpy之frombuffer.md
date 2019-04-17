# Numpy之frombuffer - qq_37385726的博客 - CSDN博客





2018年08月14日 15:40:29[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：2878
所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)









# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)



# 代码

```
import numpy as np



#frombuffer将data以流的形式读入转化成ndarray对象
#第一参数为stream,第二参数为返回值的数据类型，第三参数指定从stream的第几位开始读入

#data是字符串的时候，Python3默认str是Unicode类型，所以要转成bytestring在原str前加上b
s =  b'abc'
a = np.frombuffer(s,dtype='S1',offset=1)
print(a)
```

# 输出

[b'b' b'c']



