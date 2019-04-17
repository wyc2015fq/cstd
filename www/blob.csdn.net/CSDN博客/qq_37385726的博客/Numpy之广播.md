# Numpy之广播 - qq_37385726的博客 - CSDN博客





2018年08月15日 11:03:08[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：69
个人分类：[Numpy](https://blog.csdn.net/qq_37385726/article/category/7935606)

所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)









> 
**很多博客上提到广播，都会说到可以广播的原则：一个是为1，一个是相等**

**但这个话就很困扰，到底什么叫为1，什么叫相等。**




转载自  [讲的很清楚的广播](https://blog.csdn.net/lanchunhui/article/details/50158975)



# **目录**

[1. 举例](#1-%E4%B8%BE%E4%BE%8B)

[2. 补充](#2-%E8%A1%A5%E5%85%85)

> 
broadcasting，广播，传递，赋值，拷贝；


**一定要注意，执行 broadcast 的前提在于，两个 ndarray 执行的是 element-wise（按位加，按位减） 的运算，而不是矩阵乘法的运算，矩阵乘法运算时需要维度之间严格匹配。**（且矩阵乘法，np.dot(A, B) 如果维度不匹配，提示的错误不会是 **broadcast**，而是 **aligned**）

我们常常会看到 python 编译器会提示如下类型的错误：
- ValueError: operands could not be broadcast together with shapes (8,4,3) (2,1)

那么如何理解这里的`broadcast`呢，matlab中并无对等的概念？

`broadcasting`机制的功能是为了方便不同`shape`的array（numpy库的核心数据结构）进行数学运算。

举一个简单的例子，实现对一个1-d array的每一个元素乘以2：

```
>>> a = np.array([1., 2., 3.])
>>> b = np.array([2., 2., 2.])
>>> a*b
array([2., 4., 6.])
```

broadcast的做法是：

```
>>> a = np.array([1., 2., 3.])
>>> b = 2.
>>> a*b
array([2., 4., 6.])
```

这也就解释了之前常常令人困惑的`(3, )`不同于`(3, 1)`（表shape）。

我们来看更为一般的`broadcasting rules`：

当操作两个`array`时，numpy会逐个比较它们的shape（构成的元组tuple），只有在下述情况下，两`arrays`才算兼容：
- 相等
- 其中一个为1，（进而可进行拷贝拓展已至，shape匹配）

## **1. 举例**

举例说明：

```
Image (3d array):  256 x 256 x 3
Scale (1d array):              3
Result (3d array): 256 x 256 x 3

A      (4d array):  8 x 1 x 6 x 1
B      (3d array):      7 x 1 x 5
Result (4d array):  8 x 7 x 6 x 5

A      (2d array):  5 x 4
B      (1d array):      1
Result (2d array):  5 x 4

A      (2d array):  15 x 3 x 5
B      (1d array):  15 x 1 x 5
Result (2d array):  15 x 3 x 5
```

再来看一些不能进行`broadcast`的例子：

```
A  (1d array): 3
B  (1d array): 4        # 最后一维（trailing dimension）不匹配

A  (2d array):      2 x 1
B  (3d array):  8 x 4 x 3（倒数第二维不匹配）
```

我们再来看一些具体的应用：

```
>>> x = np.arange(4)
>> xx = x.reshape(4, 1)
>> y = np.ones(5)
>> z = np.ones((3, 4))

>>> x.shape
(4,)
>>> y.shape
(5,)
>>> x+y
ValueError: operands could not be broadcast together with shapes (4,) (5,) 

>>> xx.shape
(4, 1)
>>> y.shape
(5,)
>>> (xx+y).shape
(4, 5)
>>> xx + y
array([[ 1.,  1.,  1.,  1.,  1.],
       [ 2.,  2.,  2.,  2.,  2.],
       [ 3.,  3.,  3.,  3.,  3.],
       [ 4.,  4.,  4.,  4.,  4.]])
```

当执行xx+y时，numpy是如何进行copy扩展的呢？

```
xx     (2d array):      4 x 1
y      (1d array):          5
Result (2d array):      4 x 5
```

也即对xx重复5列，对y重复4行

```
# 对xx重复5列
# 等价于np.dot(xx, np.ones((1, 4)))
array([[ 0.,  0.,  0.,  0.],
       [ 1.,  1.,  1.,  1.],
       [ 2.,  2.,  2.,  2.],
       [ 3.,  3.,  3.,  3.]])
# 对y重复4行，
array([[ 1.,  1.,  1.,  1.,  1.],
       [ 1.,  1.,  1.,  1.,  1.],
       [ 1.,  1.,  1.,  1.,  1.],
       [ 1.,  1.,  1.,  1.,  1.]])
```

## **2. 补充**

还有一点，注意和矩阵乘法的区别，当有一维数组参与运算时：
- **一维数组置于矩阵乘法的左部，被视为一个行向量**；
- **一维数组置于矩阵乘法的右部，被视为一个列向量；**
- （这样和一个一维数组作用无论在左还是在右）**矩阵乘法运算结束得到的向量仍是一维数组。**

```
x   (1d array)  ->  5       ->  1, 5
W   (2d array)  ->  5, 3    ->  5, 3
                            ->  3 (1d array)
```

```
>>> import numpy
>>> x = numpy.random.randn(5)
>>> x.shape
(5,)
>>> x.T.shape
(5,)                # 一维数组x并非严格意义上的行向量

>>> W = numpy.random.randn(5, 3)
>>> numpy.dot(x, W)
array([ 0.06489021, -3.08729591,  1.52169767])
>>> numpy.dot(x, W).shape
(3, )

>>> y = numpy.ones(3)
>>> y.shape
(3,)
>>> W.dot(y).shape
(5,)
```

# **references**

[1][broadcasting](http://docs.scipy.org/doc/numpy/user/basics.broadcasting.html)



