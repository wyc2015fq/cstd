# Python内置函数(22)——list - lincappu - 博客园







# [Python内置函数(22)——list](https://www.cnblogs.com/lincappu/p/8144794.html)





**英文文档：**

*class *`list`([*iterable*])

Rather than being a function, `list` is actually a mutable sequence type, as documented in Lists and Sequence Types — list, tuple, range

 　　根据传入的参数创建一个新的列表

**说明：**

　　1. list函数，实际是上列表类型的构造函数。

　　2. 可以不传入任何参数，结果返回一个空列表。

```
>>> a = list()
>>> a
[]
```

　　3. 可以传入一个可迭代对象，如字符串，字节数组、元组、列表、range对象，结果将返回可迭代对象中元素组成的列表。

```
>>> list('abcd') # 字符串
['a', 'b', 'c', 'd']
>>> list(bytes('abcd','utf-8')) # 字节数组
[97, 98, 99, 100]
>>> list(('a','b','c','d')) # 元组
['a', 'b', 'c', 'd']
>>> list(['a','b','c','d']) # 列表
['a', 'b', 'c', 'd']
>>> list(range(1,5)) # range对象
[1, 2, 3, 4]
```












