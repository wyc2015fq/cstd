# Numpy之flatten和ravel - qq_37385726的博客 - CSDN博客





2018年08月15日 11:02:52[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：57
个人分类：[Numpy](https://blog.csdn.net/qq_37385726/article/category/7935606)

所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)









 转载自[此处](https://blog.csdn.net/lanchunhui/article/details/50354978)



# **目录**

[1. 两者的功能](#1-%E4%B8%A4%E8%80%85%E7%9A%84%E5%8A%9F%E8%83%BD)

[2. 两者的区别](#2-%E4%B8%A4%E8%80%85%E7%9A%84%E5%8C%BA%E5%88%AB)

[References](#references)



> 
首先声明两者所要实现的功能是一致的（将多维数组降位一维），**两者的区别在于返回拷贝（copy）还是返回视图（view），numpy.flatten()返回一份拷贝，对拷贝所做的修改不会影响（reflects）原始矩阵，而numpy.ravel()返回的是视图（view，也颇有几分C/C++引用reference的意味），会影响（reflects）原始矩阵。**


## 1. 两者的功能

```
>>> x = np.array([[1, 2], [3, 4]])
>>> x
array([[1, 2],
       [3, 4]])
>>> x.flatten()
array([1, 2, 3, 4])
>>> x.ravel()
array([1, 2, 3, 4])
                    两者默认均是行序优先
>>> x.flatten('F')
array([1, 3, 2, 4])
>>> x.ravel('F')
array([1, 3, 2, 4])
```







## 2. 两者的区别

```
>>> x = np.array([[1, 2], [3, 4]])
>>> x.flatten()[1] = 100
>>> x
array([[1, 2],
       [3, 4]])            # flatten：返回的是拷贝
>>> x.ravel()[1] = 100
>>> x
array([[  1, 100],
       [  3,   4]])
```







## References

[1][What is the difference between flatten and ravel functions in numpy?](http://stackoverflow.com/questions/28930465/what-is-the-difference-between-flatten-and-ravel-functions-in-numpy)



