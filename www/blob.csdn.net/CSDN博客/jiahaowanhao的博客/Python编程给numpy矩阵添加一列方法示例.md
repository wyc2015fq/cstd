
# Python编程给numpy矩阵添加一列方法示例 - jiahaowanhao的博客 - CSDN博客


2018年04月25日 22:02:59[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：262


Python编程给numpy矩阵添加一列方法示例
首先我们有一个数据是一个mn的numpy矩阵现在我们希望能够进行给他加上一列变成一个m(n+1)的矩阵
import numpy as np
a = np.array([[1,2,3],[4,5,6],[7,8,9]])
b = np.ones(3)
c = np.array([[1,2,3,1],[4,5,6,1],[7,8,9,1]])
PRint(a)
print(b)
print(c)
[[1 2 3]
[4 5 6]
[7 8 9]]
[ 1. 1. 1.]
[[1 2 3 1]
[4 5 6 1]
[7 8 9 1]]
我们要做的就是把a，b合起来变成c
方法一
使用np.c_[]和np.r_[]分别添加行和列
np.c_[a,b]
array([[ 1., 2., 3., 1.],
[ 4., 5., 6., 1.],
[ 7., 8., 9., 1.]])
np.c_[a,a]
array([[1, 2, 3, 1, 2, 3],
[4, 5, 6, 4, 5, 6],
[7, 8, 9, 7, 8, 9]])
np.c_[b,a]
array([[ 1., 1., 2., 3.],
[ 1., 4., 5., 6.],
[ 1., 7., 8., 9.]])
方法二
使用np.insert
np.insert(a, 0, values=b, axis=1)
array([[1, 1, 2, 3],
[1, 4, 5, 6],
[1, 7, 8, 9]])
np.insert(a, 3, values=b, axis=1)
array([[1, 2, 3, 1],
[4, 5, 6, 1],
[7, 8, 9, 1]])
方法三
使用'column_stack'
np.column_stack((a,b))
array([[ 1., 2., 3., 1.],
[ 4., 5., 6., 1.],
[ 7., 8., 9., 1.]])
总结
以上就是本文关于Python编程给numpy矩阵添加一列方法示例的全部内容，希望对大家有所帮助。

