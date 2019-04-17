# Python内置函数(37)——sorted - lincappu - 博客园







# [Python内置函数(37)——sorted](https://www.cnblogs.com/lincappu/p/8144969.html)





**英文文档：**

`sorted`(*iterable[, key][, reverse]*)

Return a new sorted list from the items in *iterable*.

Has two optional arguments which must be specified as keyword arguments.

*key* specifies a function of one argument that is used to extract a comparison key from each list element: `key=str.lower`. The default value is `None` (compare the elements directly).

*reverse* is a boolean value. If set to `True`, then the list elements are sorted as if each comparison were reversed.

Use `functools.cmp_to_key()` to convert an old-style *cmp* function to a *key* function.

The built-in `sorted()` function is guaranteed to be stable. A sort is stable if it guarantees not to change the relative order of elements that compare equal — this is helpful for sorting in multiple passes (for example, sort by department, then by salary grade).

 　　对可迭代对象进行排序，生成一个新的可迭代对象

**说明：**

　　1. 函数功能对一个可迭代对象进行排序，返回一个排序后列表，原来的列表不变。

```
>>> a = sorted('dcabegf')
>>> a # 返回结果是列表
['a', 'b', 'c', 'd', 'e', 'f', 'g']
```

　　2. 函数调用时可以提供一个可选的命名参数key，它是一个方法，默认值是None，用来指定具体排序的算法；函数对可迭代对象每个元素使用key算法后再排序，返回的任然是可迭代对象中的元素。

```
>>> a = ['a','b','d','c','B','A']
>>> a
['a', 'b', 'd', 'c', 'B', 'A']

>>> sorted(a) # 默认按字符ascii码排序
['A', 'B', 'a', 'b', 'c', 'd']

>>> sorted(a,key = str.lower) # 转换成小写后再排序，'a'和'A'值一样，'b'和'B'值一样
['a', 'A', 'b', 'B', 'c', 'd']
```

　　3. 函数调用时可以提供一个可选的命名参数reverse，它的默认值是False，用来排序结果是否倒转。

```
>>> a = sorted('dcabegf')
>>> a
['a', 'b', 'c', 'd', 'e', 'f', 'g']

>>> a = sorted('dcabegf',reverse = True) # 排序结果倒置
>>> a
['g', 'f', 'e', 'd', 'c', 'b', 'a']
```














