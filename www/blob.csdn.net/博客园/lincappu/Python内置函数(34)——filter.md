# Python内置函数(34)——filter - lincappu - 博客园







# [Python内置函数(34)——filter](https://www.cnblogs.com/lincappu/p/8144917.html)





**英文文档：**

`filter`(*function*, *iterable*)

Construct an iterator from those elements of *iterable* for which *function* returns true. *iterable* may be either a sequence, a container which supports iteration, or an iterator. If *function* is `None`, the identity function is assumed, that is, all elements of *iterable* that are false are removed.

Note that `filter(function, iterable)` is equivalent to the generator expression `(item for item in iterable iffunction(item))` if function is not `None` and `(item for item in iterable if item)` if function is `None`.

See `itertools.filterfalse()` for the complementary function that returns elements of *iterable* for which *function*returns false.

　　使用指定的方法过滤可迭代对象的元素 

**说明：**

　　1. filter函数用于过滤序列。过滤的方式则是采用传入的函数，去循环序列的元素调用，如果函数计算的结果为True则保留元素，否则将舍弃该元素。

```
>>> a = list(range(1,10)) #定义序列
>>> a
[1, 2, 3, 4, 5, 6, 7, 8, 9]
>>> def if_odd(x): #定义奇数判断函数
    return x%2==1

>>> list(filter(if_odd,a)) #筛选序列中的奇数
[1, 3, 5, 7, 9]
```

　　2. 当function参数传入None时，序列中的元素值如果为False，也会自动舍弃。

```
>>> c = ['',False,'I',{}] #定义序列
>>> c
['', False, 'I', {}]

>>> list(filter(None,c)) #筛选函数为None，自动舍弃序列中的False值，空字符串、False值、空序列都是False值，所以丢弃
['I']
```












