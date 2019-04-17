# Numpy之logspace - qq_37385726的博客 - CSDN博客





2018年08月14日 15:59:41[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：133
所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)









# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)

（很多都与linspace类似，可参考）

# 代码

```
#logspace返回值为ndarray对象，float型
#numpy.logspace(start, stop, num, endpoint, base, dtype)

#数据头一定是base^start，若endpoint=True，数据尾为base^end，其余数据为base^(start,end)

a=np.logspace(1,5,5,base=2)
print(a)
```

# 输出

[ 2.  4.  8. 16. 32.]



