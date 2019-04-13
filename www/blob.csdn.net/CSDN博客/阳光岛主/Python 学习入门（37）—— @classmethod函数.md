
# Python 学习入门（37）—— @classmethod函数 - 阳光岛主 - CSDN博客

2014年02月21日 23:33:05[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：10815个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



@classmethod : 类方法
@staticmethod : 静态方法
类方法和静态方法的调用一样，都是通过类就可以直接调用。
区别：类方法，需要传入该类，定义类方法的时候要传一个默认的参数cls。静态方法则不用。

**示例：**

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# blog.ithomer.net
class Test(object):
    x = 11
    def __init__(self, _x):
        self._x = _x
        print("Test.__init__")
 
    @classmethod
    def class_method(cls):
        print("class_method")
 
    @staticmethod
    def static_method():
        print("static_method")
 
    @classmethod
    def getPt(cls):
        cls.class_method()
        cls.static_method()
 
if "__main__" == __name__:
    Test.class_method()         # class_method
    Test.static_method()        # static_method
    Test.getPt()                # class_method  static_method
    t = Test(22)                # Test.__init__
    t.class_method()            # class_method
    t.static_method()           # static_method
    
    print Test.x                # 11
#     print Test._x
    
    print t.x                   # 11
    print t._x                  # 22
    
#     t.getPr()   # 'Test' object has no attribute 'getPr'
```
运行结果：

```python
class_method
static_method
class_method
static_method
Test.__init__
class_method
static_method
11
11
22
Traceback (most recent call last):
  File "/home/homer/workspace/myPython/com/connmiliao.py", line 40, in <module>
    t.getPr() 
AttributeError: 'Test' object has no attribute 'getPr'
```

**示例：@property,@staticmethod,@classmethod**

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# blog.ithomer.net
class MyClass(object):
    def __init__(self):
        print '__init__'
        self._name = 'blog.ithomer.net'
    @staticmethod
    def static_method():
        print 'This is a static method!'
    def test(self):
        print 'call test'
    @classmethod
    def class_method(cls):
        print 'cls: ',cls
        print 'cls.name: ',cls.name
        print 'cls.static_method(): ',cls.static_method()
        instance = cls()
        print 'instance.test(): ',instance.test()
    @property
    def name(self):
        return self._name
    
    @name.setter
    def name(self, value):
        self._name = value
if __name__ == '__main__':
    MyClass.static_method()
    MyClass.class_method()
    
    mc = MyClass()
    print mc.name
    mc.name = 'forum.ithomer.net'  
    print mc.name
```
运行结果：
This is a static method!
cls:  <class '__main__.MyClass'>
cls.name:  <property object at 0x150d680>
cls.static_method():  This is a static method!
None
__init__
instance.test():  call test
None
__init__
blog.ithomer.net
forum.ithomer.net


