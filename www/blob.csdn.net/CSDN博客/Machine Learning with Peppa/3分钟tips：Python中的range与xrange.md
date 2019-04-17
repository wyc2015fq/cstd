# 3分钟tips：Python中的range与xrange - Machine Learning with Peppa - CSDN博客





2018年04月03日 21:53:01[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：142







range返回的是一个包含所有元素的列表，xrange返回的是一个生成器，生成器是一个可迭代对象，在对生成器进行迭代时，元素是逐个被创建的。一般来看，在对大序列进行迭代的时候，因为xrange的特性，所以它会比较节约内存。
我们来仔细看看这两者的不同：
>>> Range = range(10)
>>> xRange = xrange(10)
>>> type(Range)
<type 'list'>
>>> type(xRange)
<type 'xrange'>
这里就是很明显的类型上的不同了，range直接生成一个列表，xrange生成的是一个xrange对象

另外一个很明显的区别在与xrange不支持列表切片
>>> xRange[:1]

Traceback (most recent call last):
  File "<pyshell#39>", line 1, in <module>
    xRange[:1]
TypeError: sequence index must be integer, not 'slice'

在其它场合，我觉得xrange完全可以取代range，这样你可以无需担心列表长度的问题。
另外，xange也和普通的生成器略有不同，对于同一个xrange对象，对它进行多次迭代，每次都会从头开始，这里我自己也试了一下
首先写个常规生成器

>>> def test(x):
	for y in x:
		yield y
>>> t = test(range(10))

>>> t.next()
0
>>> t.next()
1
>>> t.next()
2
>>>list(t)
[3, 4, 5, 6, 7, 8, 9]
可以看到元素被生成出来之后就从生成器中剔除了，但是xrange有所不同
>>> xRange = xrange(10)
>>> for x in xRange:
	print x
	break

0
>>> for x in xRange:
	print x
	break

0
迭代两次，每次都是从第一个元素开始....


