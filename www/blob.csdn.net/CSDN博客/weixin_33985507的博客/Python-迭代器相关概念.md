# Python-迭代器相关概念 - weixin_33985507的博客 - CSDN博客
2017年09月30日 00:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
![1845535-cff55cd2bb84e1b0.png](https://upload-images.jianshu.io/upload_images/1845535-cff55cd2bb84e1b0.png)
各概念关系图
# 一些基本概念
## 1 容器
可以询问某个元素是否包含其中，如list，set，tuples,dict等都是容器
## 2 迭代器（iterator）
1）迭代器是一个带状态的对象，任何实现了**iter**和next__方法的对象都是迭代器（python2:任何实现next（）方法的对象都是迭代器）。
2）其中**iter**返回迭代器本身，**next**返回容器中的下一个值。如果容器中没有更多元素了，则抛出Stopiteration异常。
因此，可以把迭代器理解成一个带有流水线的工程，我们每次询问他时，他就给我们返回下一个值。**迭代器会把所有的值都存储在内存中。**
> 
### 2.1 next（）
next（）函数 用来返回文件的下一行/下一个值，直到促发STopIteration。
[《Python File next() 方法》](https://link.jianshu.com?t=//www.runoob.com/python/file-next.html)
> 
### 2.2 iter（）
用处：把可迭代对象变为迭代器。
## 3 可迭代对象（iterable）
凡是可以返回一个迭代器的对象都可以称之为可迭代对象（除了上面提到的list,tuples,dict等容器外，还有很多其他对象也是可迭代对象。比如，打开状态的files.
我的理解是所有可以使用 for .. in .. 语法的对象都可以叫做一个迭代对象。
但是迭代器把所有的值都存储到了内存中，如果有大量数据的话，这个方式就会占用大量内存。
> 
注：很多容器都是可迭代对象，但并不是所有容器都是可迭代对象.
下面的例子可以帮助更好的理解可迭代对象。
```
>>> x = [1, 2, 3]
>>> y = iter(x)
>>> z = iter(x)
>>> next(y)
1
>>> next(y)
2
>>> next(z)
1
>>> type(x)
<class 'list'>
>>> type(y)
<class 'list_iterator>
```
上题中，x就是一个可迭代对象。可迭代对象和容器一样是一种通俗的叫法，并不是指某种具体的数据类型，***列表***是可迭代对象，***字典***是可迭代对象，***集合***也是可迭代对象。
我们用 ***liter()*** 函数可以把可迭代对象变为迭代器对象。
上面代码中，y和z是两个独立的迭代器。迭代器内部持有一个状态，该状态用于记录当前迭代所在的位置，以方便下次迭代的时候获取正确的元素。迭代器有一种具体的迭代器类型，比如list_iterator，set_iterator。可迭代对象实现了**iter**方法，该方法返回一个迭代器对象。
当运行以下代码时：
```
x = [1, 2, 3]
for elemments in x:
    ...
```
实际执行情况是：
![1845535-f29ffc28d22ac45a.png](https://upload-images.jianshu.io/upload_images/1845535-f29ffc28d22ac45a.png)
迭代对象--->迭代器
## 4 生成器(generator)
生成器其实是一种特殊的迭代器。它和一般迭代器不同的地方在于，我们 **只可以读取它一次**，因为它并不把所有的值放在内存中，它是实时地生成数据:
```
>>> mygenerator = (x*x for x in range(3))  #range后面会介绍
>>> for i in mygenerator :
...    print(i)
结果：
0
1
4
```
生成器只能读取一次是什么意思？举个例子：
```
# -*- coding: UTF-8 -*-
def mygenerator(n):   #建一个生成器
     for x in range(n):
         yield int(x)
y = mygenerator(5) 
z = sum(y) # 使用一次生成器。遍历mygenerator中所有数，并相加
for i in y:    #第二次使用生成器
    print(i)   #print不会有任何结果，因为已经使用过一次生成器。
```
本节中其他相关函数的意思：
> 
range（）
range（y,x,z); y表示起始范围，x表示终止范围，z表示间隔值
1.range(x) 表示0-x范围内的数（不包含x）
2.range(y,x)表示y-x范围内的数（不包含x）
3.range(y,x,z)表示y-x范围内(不包含x),间隔为z的数.
更具体一点的说明：
[《详细记录python的range()函数用法》](https://link.jianshu.com?t=http://www.cnblogs.com/buro79xxd/archive/2011/05/23/2054493.html)
和range（）函数相似的，还有个xrange（）函数，具体见下面说明：
> 
xrange()
参数与range()函数一样，不一样的地方在于，xrange（）生成的不是一个数组，而是一个生成器。
xrange() 和 range（）的区别可以参见以下详细资料：
[《Python的range和xrange》](https://link.jianshu.com?t=http://www.cnblogs.com/zhangjing0502/archive/2012/05/16/2503880.html)
举例：
```
>>> range(5) 
[0, 1, 2, 3, 4] 
>>> xrange(5)
xrange(5)
>>> list(xrange(5))
[0, 1, 2, 3, 4]
```
由上面可以，range（）会直接生成列表，而xrange()会生成一个生成器。因此，range相比于xrange会预先占用很多资源。
比如，如果是range(1000)，那个range会直接生成0-1000的列表，预先占用内存；但是xrange只会生成xrange生成器，需要用到具体函数时，再占用相应的内存。所以xrange做循环的性能比range好，尤其是返回很大的时候，尽量用xrange。
### 生成器与迭代器的代码区分
例子：
```
>>> mylist = [x*x for x in range(3)]
>>> for i in mylist :
...    print(i)
>>> mygenerator = (x*x for x in range(3))
>>> for i in mygenerator :
...    print(i)
```
前一个[ ] 迭代器，后一个（）生成器。
## 5 yield 关键词
`yield` 是一个类似 `return` 的关键字，只是这个函数返回的是个生成器。
```
>>> def createGenerator() :
...    mylist = range(3)
...    for i in mylist :
...        yield i*i
...
>>> mygenerator = createGenerator() # create a generator
>>> print(mygenerator) # mygenerator is an object!
<generator object createGenerator at 0xb7555c34>
>>> for i in mygenerator:
...     print(i)
0
1
4
```
当我们用print（mygenerator）调用createGenerator()这个函数的时候，函数内部的代码并不立马执行 ，这个函数只是返回一个生成器对象。
只有当我们使用for进行迭代的时候，函数内代码才会执行。
第一次迭代时,函数从开始一直执行到 yield这个 关键字，然后返回 yield 后的值（即i*i）作为第一次迭代的返回值（即将0作为返回值）.每次执行这个函数，都会继续执行你在函数内部定义的那个循环的下一次，再返回那个值。例如，第二次执行，迭代返回值为1*1=1。当我们不断调用，这个过程会一直持续，直到没有可以返回的值为止。
**生成器下一次迭代是从上一次结束的地方开始，而不会从头开始。比如第一次迭代后。第二次迭代是从i=1开始，而不是从i=0开始。**
如果生成器内部没有定义 yield 关键字，那么这个生成器被认为成空的。这种情况可能因为是循环进行没了，或者是没有满足 if/else 条件。
## 迭代器有关工具：itertools
itertools 是一个模块，集合了众多的迭代函数，功能非常强大，具体可以见以下内容
[《PYTHON-进阶-ITERTOOLS模块小结》](https://link.jianshu.com?t=http://www.wklken.me/posts/2013/08/20/python-extra-itertools.html)
### 参考资料
1[《(译)Python关键字yield的解释》](https://link.jianshu.com?t=http://pyzh.readthedocs.io/en/latest/the-python-yield-keyword-explained.html)
2.[《完全理解 Python 迭代对象、迭代器、生成器》](https://link.jianshu.com?t=http://python.jobbole.com/87805/)
3.[知乎：《如何更好地理解Python迭代器和生成器？》](https://link.jianshu.com?t=https://www.zhihu.com/question/20829330)
4[《PYTHON-进阶-ITERTOOLS模块小结》](https://link.jianshu.com?t=http://www.wklken.me/posts/2013/08/20/python-extra-itertools.html)
5[《Python的range和xrange》](https://link.jianshu.com?t=http://www.cnblogs.com/zhangjing0502/archive/2012/05/16/2503880.html)
6[《详细记录python的range()函数用法》](https://link.jianshu.com?t=http://www.cnblogs.com/buro79xxd/archive/2011/05/23/2054493.html)
7[《Python File next() 方法》](https://link.jianshu.com?t=//www.runoob.com/python/file-next.html)
