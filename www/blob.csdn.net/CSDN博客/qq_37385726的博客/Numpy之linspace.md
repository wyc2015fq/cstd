# Numpy之linspace - qq_37385726的博客 - CSDN博客





2018年08月14日 15:54:13[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：100
所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)









# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)



# 代码

```
#linspace返回值为ndarray,float型

#numpy.linspace(start, stop, num, endpoint=true, retstep=false, dtype=float)

#num为从start到stop的等间隔的数的个数
a=np.linspace(1,5,5)
print(a)

b=np.linspace(1,5,9,retstep=True)
print(b)

#endpoint=False表示不包括end点
c=np.linspace(1,5,9,endpoint=False,retstep=True)
print(c)

#dtype改变默认为float的数据类型
d=np.linspace(1,5,9,endpoint=False,retstep=True,dtype=int)
print(d)
```

# 输出

[1. 2. 3. 4. 5.]



(array([1. , 1.5, 2. , 2.5, 3. , 3.5, 4. , 4.5, 5. ]), 0.5)



(array([1.        , 1.44444444, 1.88888889, 2.33333333, 2.77777778,

       3.22222222, 3.66666667, 4.11111111, 4.55555556]), 0.4444444444444444)



(array([1, 1, 1, 2, 2, 3, 3, 4, 4]), 0.4444444444444444)



