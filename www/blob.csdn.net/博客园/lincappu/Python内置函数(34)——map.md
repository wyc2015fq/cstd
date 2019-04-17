# Python内置函数(34)——map - lincappu - 博客园







# [Python内置函数(34)——map](https://www.cnblogs.com/lincappu/p/8144930.html)





**英文文档：**
- `map`(*function*, *iterable*, *...*)
- 
- Return an iterator that applies *function* to every item of *iterable*, yielding the results. If additional *iterable* arguments are passed, *function* must take that many arguments and is applied to the items from all iterables in parallel. With multiple iterables, the iterator stops when the shortest iterable is exhausted. For cases where the function inputs are already arranged into argument tuples, see `itertools.starmap()`.
- 
- 
- 
- 
- **　　使用指定的方法去作用传入的每个可迭代对象的元素，生成新的可迭代对象　　**
- 
- **说明：**
- 　　1. 函数接受一个函数类型参数、一个或者多个可迭代对象参数，返回一个可迭代器，此迭代器中每个元素，均是函数参数实例调用可迭代对象后的结果。
- 

```
>>> a = map(ord,'abcd')
>>> a
<map object at 0x03994E50>
>>> list(a)
[97, 98, 99, 100]
```

　　2. 当传入多个可迭代对象时，函数的参数必须提供足够多的参数，保证每个可迭代对象同一索引的值均能正确传入函数。

```
>>> a = map(ord,'abcd')
>>> list(a)
[97, 98, 99, 100]
>>> a = map(ord,'abcd','efg') # 传入两个可迭代对象，所以传入的函数必须能接收2个参数，ord不能接收2个参数，所以报错
>>> list(a)
Traceback (most recent call last):
  File "<pyshell#22>", line 1, in <module>
    list(a)
TypeError: ord() takes exactly one argument (2 given)

>>> def f(a,b):
    return a + b
>>> a = map(f,'abcd','efg') # f函数可以接受2个参数
>>> list(a)
['ae', 'bf', 'cg']
```

　　3. 当传入多个可迭代对象时，且它们元素长度不一致时，生成的迭代器只到最短长度。

```
>>> def f(a,b):
    return a + b

>>> a = map(f,'abcd','efg') # 选取最短长度为3
>>> list(a)
['ae', 'bf', 'cg']
```

　　4. map函数是一个典型的函数式编程例子。












