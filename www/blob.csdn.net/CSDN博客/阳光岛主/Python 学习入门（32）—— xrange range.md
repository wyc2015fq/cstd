
# Python 学习入门（32）—— xrange-range - 阳光岛主 - CSDN博客

2013年12月21日 22:25:05[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5329所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



在Python中range和xrange是不同的，两者的区别是微妙的。为了简单起见，这里省略两个函数的可选参数start和step。range返回值和想象的一样：一个从0开始指定长度的连续整数序列。然而，xrange返回一个"xrange对象"，该对象非常类似于迭代器。如果你曾经研究过迭代器，则两者的区别就很明显。
下面是个例子：
range(1000000)
xrange(1000000)
range(1000000)会返回一个100万的整数元素的序列，而xrange(1000000)在本质上会返回一个迭代序列。显然xrange支持迭代，而range不支持。这种收益是很小的，因为"当需要元素的时候，xrange仍然要创建它们"。但是无论需要的序列多大，每次遍历时xrange消耗相同量的内存。在遍历极端的大序列时，这是相对于range的很大优势。另一个优势也是明显的：当你的代码在遍历已经产生的序列时调用break的话，xrange显然比range要好，因为xrange会消耗更少的内存。

这两个基本上都是在循环的时候用。
**for**i**in**range(0,100):
**print**i
**for**i**in**xrange(0,100):
**print**i
这两个输出的结果都是一样的，实际上有很多不同，range会直接生成一个list对象：
a =range(0,100)
**print**type(a)
**print**a

**print**a[0], a[1]
而xrange则不会直接生成一个list，而是每次调用返回其中的一个值
a =xrange(0,100)
**print**type(a)
**print**a
**print**a[0], a[1]

示例：

```python
t1 = range(0, 100)
print type(t1)              # <type 'list'>
print t1[0],t1[1],t1[99]    # 0 1 99
print t1                    # [0, 1, 2, 3, 4, 5, 6, ..., 99]
t2 = xrange(0, 100)  
print type(t2)              # <type 'xrange'>
print t2[0],t2[1],t2[99]    # 0 1 99
print t2                    # xrange(100)
```
性能比较：

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
import time
def test_range(num):
    for i in range(0, num):
        #print i
        pass
        
def test_xrange(num):
    for i in xrange(0, num):
        #print i
        pass
    
    
NUM = 100000000
# test range
start = time.time()
test_range(NUM)
print("test_range() costTime: " + str(time.time() - start))
# test xrange
start = time.time()
test_xrange(NUM)
print("test_xrange() costTime: " + str(time.time() - start))
```
运行结果：
NUM = 100000000
test_range() costTime: 18.8114709854
test_xrange() costTime: 1.08277893066

NUM = 1000000
test_range() costTime: 0.0496039390564
test_xrange() costTime: 0.0162799358368

NUM = 10000
test_range() costTime: 0.000231027603149
test_xrange() costTime: 0.000118970870972

所以xrange做循环的性能比range好，尤其是返回很大的时候！
尽量用xrange吧，除非你是要返回一个列表。


