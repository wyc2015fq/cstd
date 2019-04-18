# python学习笔记(五)--列表生成式、生成器(generator)、yield关键字 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月10日 15:56:17[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：297
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## 列表生成式

能够快速的生成一个list

配合 for 循环使用，for 循环 遍历出的结果 无需像如下方式生成

```
>>> for x in range(1,11):
	Li.append(x)

	
>>> Li
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```

而是用列表生成式让这个操作变得更为便捷

```
>>> [x for x in range(1,11)]
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```

就是 将 for遍历出来的变量 x  放到 for 前面，并在 外侧加上一个中括号，这样就可以直接生成一个 list。

同样，因为 for循环支持遍历以后提取出多个变量，所以列表生成器 也支持多个变量的组装

如下我们将一个dict 通过便利 组装成 一个 list

```
>>> L = {'a':'1','b':'2','c':'3'}
>>> [k +'='+ v for k,v in L.items()]
['a=1', 'b=2', 'c=3']
```

列表生成式 同样支持对 遍历出来的结果进行转换

```
>>> L = {'AaAa':'1','BbBb':'2','CcCc':'3'}
>>> [k.lower() +'='+ v for k,v in L.items()]
['aaaa=1', 'bbbb=2', 'cccc=3']
```

列表生成式 还可以配合 if 来使用

如下提取出 1-10 中 偶数数字

```
>>> [x for x in range(1,11) if x % 2 == 0]
[2, 4, 6, 8, 10]
```

列表生成式 还支持 多重循环

```
>>> [x+y for x in "abc" for y in "xyz"]
['ax', 'ay', 'az', 'bx', 'by', 'bz', 'cx', 'cy', 'cz']
```

## 生成器（generator） 

由于list有一个致命的缺点，如果元素个数过多，会导致极大的消耗内存容量，而且有时我们只需要list中的前几个元素。

generator 就可以为我们解决这个问题。

上面说了 列表生成式，我们只需要把列表生成式 外侧的中括号换成小括号 返回的结果就成 list变成了generator。

然后通过 next(generator) 就可以每调用一次 这个方法，就读取出一个值，类似于java中Iterator的next()方法，其实在python中也有Iterator这个对象，只要是Iterator对象的都可以通过next方法进行迭代。

```
>>> g = (x for x in "123456789")
>>> next(g)
'1'
>>> next(g)
'2'
>>> next(g)
'3'
>>> next(g)
'4'
>>> next(g)
'5'
>>> next(g)
'6'
>>> next(g)
'7'
>>> next(g)
'8'
>>> next(g)
'9'
>>> next(g)
Traceback (most recent call last):          后面没有值了会报异常
  File "<pyshell#36>", line 1, in <module>
    next(g)
StopIteration
>>>
```

generator 只有在我们想要使用 到某个值得时候，它才会根据逻辑去为我们生成这个值，比如我第一次调用next(g)，它只为我生成了字符串'1',后面的'23456789'都不存在，再调用next(g)才会生成 ‘2’,所以它 相对于 列表来说 非常节省内存。

但是，显然这种方式在实际开发中并不适用，一直next()。

我们判断一下 generator是否可以用for循环来遍历

```
>>> from collections import Iterator
>>> 
>>> isinstance(g,Iterator)
True
```

发现是可以的

```python
>>> g = (x+'='+y for x,y in {'a':'1','b':'2','c':'3'}.items())
>>> for x in g:
	print (x)
	if x.find('2'):
		break

	
a=1             

>>> next(g)          
'b=2'
```

再说说 generator的另外一种表现形式  （yield关键字）

当我们在一个方法中执行到return，如果 return后面还需要计算，那么计算完以后方法就会执行完毕。

但是如果方法中包含yield关键字，那么它就不再是一个方法了，而是一个generator对象。

即我们需要使用next()才能让它运行，并且每当遇到yield关键字，会打印出yield关键字后面的内容，并暂停于当前位置。

当我们再次调用next()方法，会从当前位置继续向下执行，再配到yield关键字还是输出、暂停。

```python
>>> def odd():
	print('1')
	yield 1
	print('2')
	yield 2
	print('3')
	yield 3

	
>>> odd()
<generator object odd at 0x00000000031844F8>
>>> a = odd()
>>> next(a)
1
1
>>> next(a)
2
2
>>> next(a)
3
3
>>> next(a)
Traceback (most recent call last):
  File "<pyshell#76>", line 1, in <module>
    next(a)
StopIteration
```

再看另外一个案例

```python
>>> def func():
	n = 0
	while True:
		yield (n)
		if n == 10:
			return "done"
		n = n+1

		
>>> func()
<generator object func at 0x00000000031AB390>
>>> a = func()
>>> next(a)
0
>>> next(a)
1
>>> next(a)
2
>>> next(a)
3
>>> next(a)
4
>>> next(a)
5
>>> next(a)
6
>>> next(a)
7
>>> next(a)
8
>>> next(a)
9
>>> next(a)
10
>>> next(a)
Traceback (most recent call last):
  File "<pyshell#103>", line 1, in <module>
    next(a)
StopIteration: done
>>> def func():
	n = 0
	while True:
		yield (n)
		if n == 10:
			return "done"
		n = n+1

>>> g = func()
>>> while True:
	try:
		x = next(g)
		print('g:',x)
	except StopIteration as e:
		print('Generator return value:',e.value)
		break

	
g: 0
g: 1
g: 2
g: 3
g: 4
g: 5
g: 6
g: 7
g: 8
g: 9
g: 10
Generator return value: done
```

可以看出如果通过调用next()方法，最后没有值了，报出StopIteration异常，且我们的方法有返回值，python会将我们的返回值放入这个StopIteration这个对象的value属性中。

