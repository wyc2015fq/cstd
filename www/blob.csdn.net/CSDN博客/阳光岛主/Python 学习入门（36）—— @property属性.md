
# Python 学习入门（36）—— @property属性 - 阳光岛主 - CSDN博客

2014年02月15日 01:01:00[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：30994所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



@property 可以将python定义的函数“当做”属性访问，从而提供更加友好访问方式，但是有时候setter/getter也是需要的
假设定义了一个类Cls，该类必须继承自object类，有一私有变量__x
**1. 第一种使用属性的方法：**

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# blog.ithomer.net
class Cls(object):
    def __init__(self):
        self.__x = None
    
    def getx(self):
        return self.__x
    
    def setx(self, value):
        self.__x = value
        
    def delx(self):
        del self.__x
        
    x = property(getx, setx, delx, 'set x property')
if __name__ == '__main__':
    c = Cls()
    c.x = 100
    y = c.x
    print("set & get y: %d" % y)
    
    del c.x
    print("del c.x & y: %d" % y)
```
运行结果：
set & get y: 100
del c.x & y: 100
在该类中定义三个函数，分别用作赋值、取值、删除变量
property函数原型为property(fget=None,fset=None,fdel=None,doc=None)，上例根据自己定义相应的函数赋值即可。

**2. 第二种方法（在2.6中新增）**
同方法一，首先定义一个类Cls，该类必须继承自object类，有一私有变量__x

```python
class Cls(object):
    def __init__(self):
        self.__x = None
        
    @property
    def x(self):
        return self.__x
    
    @x.setter
    def x(self, value):
        self.__x = value
        
    @x.deleter
    def x(self):
        del self.__x
if __name__ == '__main__':
    c = Cls()
    c.x = 100
    y = c.x
    print("set & get y: %d" % y)
    
    del c.x
    print("del c.x & y: %d" % y)
```
运行结果：
set & get y: 100
del c.x & y: 100
说明： 同一属性__x的三个函数名要相同。


**参考推荐：**
[python Property属性用法](http://www.cnblogs.com/lovemo1314/archive/2011/05/03/2035600.html)
[python学习笔记 - @property](http://joy2everyone.iteye.com/blog/910950)



