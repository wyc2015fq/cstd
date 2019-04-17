# Python内置函数(21)——tuple - lincappu - 博客园







# [Python内置函数(21)——tuple](https://www.cnblogs.com/lincappu/p/8144790.html)





**英文文档：**

　　The constructor builds a tuple whose items are the same and in the same order as *iterable*‘s items. *iterable* may be either a sequence, a container that supports iteration, or an iterator object. If *iterable* is already a tuple, it is returned unchanged. For example, `tuple('abc')` returns `('a', 'b', 'c')` and `tuple( [1, 2, 3] )` returns `(1, 2, 3)`. If no argument is given, the constructor creates a new empty tuple, `()`.

 　　根据传入的参数创建一个新的元组

**说明：**

　　1. 函数功能创建一个新的元组。

　　2. 不传入任何参数函数将创建一个空的元组。

```
#不传入参数，创建空元组
>>> tuple() 
()
```

　　3. 函数可以接收1个可迭代对象作为参数，将使用可迭代对象的每个元素创建一个新的元组。

```
#传入不可迭代对象，不能创建新的元组
>>> tuple(121)
Traceback (most recent call last):
  File "<pyshell#17>", line 1, in <module>
    tuple(121)
TypeError: 'int' object is not iterable

#传入可迭代对象。使用其元素创建新的元组
>>> tuple('121')
('1', '2', '1')
>>> tuple([1,2,1])
(1, 2, 1)
>>> tuple((1,2,1))
(1, 2, 1)
```

　　4. 创建新的元组还可以使用一对括号的方式：

　　4.1 使用一对括号来创建空的元组。

```
>>> a= ()
>>> a
()
```

　　4.2 创建单个元素的元组时必须尾随逗号。

```
>>> a = (1,)
>>> a #a是元组
(1,)
>>> a = (1)
>>> a #a是数值
1
```

　　4.3 创建多个元素的元组，依次用逗号隔开。　　

```
>>> a = (1,2,3)
>>> a
(1, 2, 3)
```












