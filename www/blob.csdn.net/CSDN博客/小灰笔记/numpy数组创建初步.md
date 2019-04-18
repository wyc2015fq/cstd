# numpy数组创建初步 - 小灰笔记 - CSDN博客





2017年02月07日 23:15:09[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3293








   看MATLAB的书籍看到使用矩阵运算可以在一定程度上提高软件的执行效率，我又想到了Python，想到了numpy。且不管两个是否一样能够加速软件的执行速度，学习一点基础的numpy似乎是很有必要了。




引用numpy库

>>> import numpy as np







创建一维矩阵，从下面的代码以及运行结果中可以看出，创建矩阵的时候使用列表和元组作用相同。

>>> a = np.array([1,2,3,4])

>>> a

array([1, 2, 3, 4])

>>> b = np.array((1,2,3,4))

>>> b

array([1, 2, 3, 4])







查看矩阵的数据类型

>>> a.dtype

dtype(‘int64')







创建多维矩阵，从下面的代码以及运行结果可以看出，array函数的参数最多只能有2个同类型参数。创建多维矩阵，元素可以放进一个列表中，不同的矩阵行存储到列表的元素中。

>>> c = np.array([1,2,3,4],[4,5,6,7],[7,8,9,10])

Traceback (most recent call last):

  File "<stdin>", line 1, in <module>

ValueError: only 2 non-keyword arguments accepted

>>> c = np.array([[1,2,3,4],[4,5,6,7],[7,8,9,10]])

>>> c

array([[ 1,  2,  3,  4],

       [ 4,  5,  6,  7],

       [ 7,  8,  9, 10]])







查看矩阵的维度

>>> a.shape

(4,)

>>> c.shape

(3, 4)







修改矩阵维度

>>> d = c.reshape((6,2))

>>> d

array([[ 1,  2],

       [ 3,  4],

       [ 4,  5],

       [ 6,  7],

       [ 7,  8],

       [ 9, 10]])

>>> d = c.reshape(4,3)

>>> d

array([[ 1,  2,  3],

       [ 4,  4,  5],

       [ 6,  7,  7],

       [ 8,  9, 10]])







根据已有的矩阵创建新矩阵，同时制定元素的数据类型，分别使用缺省参数和指明参数两种方式

>>> b = np.array(a,float)

>>> b

array([ 1.,  2.,  3.,  4.])

>>> b = np.array(a,dtype = np.complex)

>>> b

array([ 1.+0.j,  2.+0.j,  3.+0.j,  4.+0.j])



