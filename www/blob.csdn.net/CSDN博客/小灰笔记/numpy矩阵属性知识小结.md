# numpy矩阵属性知识小结 - 小灰笔记 - CSDN博客





2017年02月09日 22:35:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：829








简单浏览了一下numpy中array属性的清单，挨个输入看了一下

>>> a = np.array([[1,2,3],[4,5,6],[7,8,9]])

>>> a

array([[1, 2, 3],

       [4, 5, 6],

       [7, 8, 9]])

>>> a.T

array([[1, 4, 7],

       [2, 5, 8],

       [3, 6, 9]])







属性T应该是矩阵的转置，再创建一个量做一下测试：

>>> b = np.array([[1,2,3],[4,5,6]])

>>> b

array([[1, 2, 3],

       [4, 5, 6]])

>>> b.T

array([[1, 4],

       [2, 5],

       [3, 6]])







>>> a.data

<read-write buffer for 0x7ff3b8800710, size 72, offset 0 at 0x106b288b0>

>>> a.dtype

dtype('int64')

>>> a.flags

  C_CONTIGUOUS : True

  F_CONTIGUOUS : False

  OWNDATA : True

  WRITEABLE : True

  ALIGNED : True

  UPDATEIFCOPY : False

>>> a.flat

<numpy.flatiter object at 0x7ff3b9002200>

>>> a.imag

array([[0, 0, 0],

       [0, 0, 0],

       [0, 0, 0]])

>>> a.real

array([[1, 2, 3],

       [4, 5, 6],

       [7, 8, 9]])

>>> a.size

9

>>> a.itemsize

8

>>> a.nbytes

72

nbytes应该数a所占的存储空间。




>>> a.ndim

2

>>> a.shape

(3, 3)

>>> a.strides

(24, 8)

>>> a.ctypes

<numpy.core._internal._ctypes object at 0x106b288d0>

>>> a.base

>>> a.base is a

False







关于base属性，参考手册中的说明做测试：

>>> a

array([[1, 2, 3],

       [4, 5, 6],

       [7, 8, 9]])

>>> c = a[:2]

>>> c

array([[1, 2, 3],

       [4, 5, 6]])

>>> c.base is a

True




由上面的测试，似乎可以把这个属性理解为是子集判断数据共享的一种方法。



