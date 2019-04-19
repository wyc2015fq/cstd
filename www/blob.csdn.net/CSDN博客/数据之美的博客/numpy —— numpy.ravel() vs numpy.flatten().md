# numpy —— numpy.ravel() vs numpy.flatten() - 数据之美的博客 - CSDN博客
2017年09月09日 13:45:43[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：109
目录[(?)](http://blog.csdn.net/lanchunhui/article/details/50354978#)[[-]](http://blog.csdn.net/lanchunhui/article/details/50354978#)
- [两者的功能](http://blog.csdn.net/lanchunhui/article/details/50354978#t0)
- [两者的区别](http://blog.csdn.net/lanchunhui/article/details/50354978#t1)
- [References](http://blog.csdn.net/lanchunhui/article/details/50354978#t2)
首先声明两者所要实现的功能是一致的（将多维数组降位一维），两者的区别在于返回拷贝（copy）还是返回视图（view），numpy.flatten()返回一份拷贝，对拷贝所做的修改不会影响（reflects）原始矩阵，而numpy.ravel()返回的是视图（view，也颇有几分C/C++引用reference的意味），会影响（reflects）原始矩阵。
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
>>> x.reshape(-1)
array([1, 2, 3, 4])
>>> x.T.reshape(-1)
array([1, 3, 2, 4])
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
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
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
## References
[1][What is the difference between flatten and ravel functions in numpy?](http://stackoverflow.com/questions/28930465/what-is-the-difference-between-flatten-and-ravel-functions-in-numpy)
