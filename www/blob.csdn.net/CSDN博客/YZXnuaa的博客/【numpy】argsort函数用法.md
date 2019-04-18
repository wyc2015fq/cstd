# 【numpy】argsort函数用法 - YZXnuaa的博客 - CSDN博客
2018年03月01日 15:55:26[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：161
在Python中使用help帮助
>>> import numpy
>>> help(numpy.argsort)
Help on function argsort in module numpy.core.fromnumeric:
argsort(a, axis=-1, kind='quicksort', order=None)
    Returns the indices that would sort an array.
    Perform an indirect sort along the given axis using the algorithm specified
    by the `kind` keyword. It returns an array of indices of the same shape as
    `a` that index data along the given axis in sorted order.
从中可以看出argsort函数返回的是数组值从小到大的索引值
Examples
    --------
    One dimensional array:一维数组
    >>> x = np.array([3, 1, 2])
    >>> np.argsort(x)
    array([1, 2, 0])
    Two-dimensional array:二维数组
    >>> x = np.array([[0, 3], [2, 2]])
    >>> x
    array([[0, 3],
           [2, 2]])
    >>> np.argsort(x, axis=0) #按列排序
    array([[0, 1],
           [1, 0]])
    >>> np.argsort(x, axis=1) #按行排序
    array([[0, 1],
           [0, 1]])
#######################################
例1：
>>> x = np.array([3, 1, 2])
>>> np.argsort(x) #按升序排列
array([1, 2, 0])
>>> np.argsort(-x) #按降序排列
array([0, 2, 1])
>>> x[np.argsort(x)] #通过索引值排序后的数组
array([1, 2, 3])
>>> x[np.argsort(-x)]
array([3, 2, 1])
另一种方式实现按降序排序：
>>> a = x[np.argsort(x)]
>>> a
array([1, 2, 3])
>>> a[::-1]
array([3, 2, 1])   
