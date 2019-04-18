# python学习笔记(六) -- Iterator和Iterable - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月13日 09:48:36[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：73
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



通过 isinstance可以查看对象的类型

```python
>>> from collections import Iterable
>>> from collections import Iterator
>>> isinstance("123",Iterable)
True
>>> isinstance([1,2,3],Iterable)
True
>>> isinstance((1,2,3),Iterable)
True
>>> isinstance(set([1,2,3]),Iterable)
True
>>> isinstance(set((1,2,3)),Iterable)
True
>>> isinstance({'1':'a','2':'b'},Iterable)
True
>>> isinstance("123",Iterator)
False
>>> isinstance([1,2,3],Iterator)
False
>>> isinstance((1,2,3),Iterator)
False
>>> isinstance(set([1,2,3]),Iterator)
False
>>> isinstance(set((1,2,3)),Iterator)
False
>>> isinstance({'1':'a','2':'b'},Iterator)
False

>>> g = (x * x for x in range(10))  #generator 既是Iterable又是Iterator
>>> isinstance(g,Iterable)
True
>>> isinstance(g,Iterator)
True
>>> type(g)
<class 'generator'>
```

凡是Iterator的就可以通过for循环遍历以及通过next()迭代

凡是Iterable的就可以通过for循环遍历，不可以通过next()迭代

```python
>>> a = [1,2,3,4,5]
>>> for n in a:
	print (n)

	
1
2
3
4
5
>>> next(a)
Traceback (most recent call last):
  File "<pyshell#96>", line 1, in <module>
    next(a)
TypeError: 'list' object is not an iterator

>>> g = (x * x for x in range(10))
>>> while True:
	try:	
		print (next(g))
	except StopIteration:
		break

	
0
1
4
9
16
25
36
49
64
81
```

Iterable对象 可以通过 iter(Iterable) 转换成 iterator对象

```python
>>> a = [1,2,3,4,5]
>>> b = iter(a)
>>> isinstance(b,Iterator)
True
>>> isinstance(a,Iterator)
False
>>> while True:
	try:	
		print (next(b))
	except StopIteration:
		break

	
1
2
3
4
5
```

iterator对象的特点就是每次调用next()内存中才会存在下一个数据，相比于Iterable对象是非常节省内存空间的

