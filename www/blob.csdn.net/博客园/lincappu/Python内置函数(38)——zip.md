# Python内置函数(38)——zip - lincappu - 博客园







# [Python内置函数(38)——zip](https://www.cnblogs.com/lincappu/p/8144979.html)





**英文文档：**

`zip`(**iterables*)

Make an iterator that aggregates elements from each of the iterables.

Returns an iterator of tuples, where the *i*-th tuple contains the *i*-th element from each of the argument sequences or iterables. The iterator stops when the shortest input iterable is exhausted. With a single iterable argument, it returns an iterator of 1-tuples. With no arguments, it returns an empty iterator.

The left-to-right evaluation order of the iterables is guaranteed. This makes possible an idiom for clustering a data series into n-length groups using `zip(*[iter(s)]*n)`. This repeats the *same* iterator `n` times so that each output tuple has the result of `n` calls to the iterator. This has the effect of dividing the input into n-length chunks.

 　　聚合传入的每个迭代器对象中相同位置的元素，返回一个新的元组类型迭代器

**说明：**

　　1. 函数功能是聚合传入的每个迭代器中相同位置的元素，返回一个新的元组类型迭代器。

```
>>> x = [1,2,3]
>>> y = [4,5,6]
>>> xy = zip(x,y)
>>> xy #xy的类型是zip类型
<zip object at 0x0429C828>
#导入Iterable
>>> from collections import Iterable
>>> isinstance(xy,Iterable) #判断是否可迭代对象
True
>>> list(xy) #结果
[(1, 4), (2, 5), (3, 6)]
```

　　2. 如果传入的迭代器长度不一致，最短长度的迭代器迭代结束后停止聚合。

```
>>> x = [1,2,3] #长度3
>>> y = [4,5,6,7,8] #长度5
>>> list(zip(x,y)) # 取最小长度3
[(1, 4), (2, 5), (3, 6)]
```

　　3. 如果只传入一个迭代器，则返回的单个元素元组的迭代器。

```
>>> list(zip([1,2,3]))
[(1,), (2,), (3,)]
```

　　4. 如果不传入参数，则返回空的迭代器。

```
>>> list(zip())
[]
```

　　5. zip(*[iter(s)]*n)等效于调用zip(iter(s),iter(s),...,iter(s))。

```
>>> x = [1,2,3]

>>> list(zip(*[x]*3))
[(1, 1, 1), (2, 2, 2), (3, 3, 3)]

>>> list(zip(x,x,x))
[(1, 1, 1), (2, 2, 2), (3, 3, 3)]
```

　 6.反zip。
x = [1, 2, 3]
y = [4, 5, 6]
z = [7, 8, 9]
xyz = zip(x, y, z)
u = zip(*xyz)
print u
运行的结果是：

[(1, 2, 3), (4, 5, 6), (7, 8, 9)]

一般认为这是一个unzip的过程，它的运行机制是这样的：

在运行zip(*xyz)之前，xyz的值是：[(1, 4, 7), (2, 5, 8), (3, 6, 9)]

那么，zip(*xyz) 等价于 zip((1, 4, 7), (2, 5, 8), (3, 6, 9))

所以，运行结果是：[(1, 2, 3), (4, 5, 6), (7, 8, 9)]

注：在函数调用中使用*list/tuple的方式表示将list/tuple分开，作为位置参数传递给对应函数（前提是对应函数支持不定个数的位置参数）





注：python3中已经将zip 后的结果换成了迭代器对象。














