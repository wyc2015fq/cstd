# python builtin keyword: property - Arthur的随笔 - CSDN博客
2012年07月11日 22:50:57[largetalk](https://me.csdn.net/largetalk)阅读数：1093
个人分类：[python](https://blog.csdn.net/largetalk/article/category/715661)
该文章有误， 请google python descriptor
##################################################################################
property 这个东西用起来还是很简单得，见 [http://docs.python.org/library/functions.html#property](http://docs.python.org/library/functions.html#property)， 两种用法 ：
```python
class C(object):
    def __init__(self):
        self._x = None
    def getx(self):
        return self._x
    def setx(self, value):
        self._x = value
    def delx(self):
        del self._x
    x = property(getx, setx, delx, "I'm the 'x' property.")
```
和
```python
class Parrot(object):
    def __init__(self):
        self._voltage = 100000
    @property
    def voltage(self):
        """Get the current voltage."""
        return self._voltage
```
这里我就奇怪了，一般我们写decorator都是用function，那这个property到底是class还是function？在ipython里help(property) 显示 property 定义是
```python
class property(object)
 |  property(fget=None, fset=None, fdel=None, doc=None) -> property attribute
```
想了下，property功能得实现应该如下：
```python
class MyProperty(object):
    def __init__(self, fget):
        print 'my property init'
        self._fget = fget
    def __call__(self):
        print 'my property call'
        return self._fget()
@MyProperty
def foo():
    print 'foo'
print '===', foo
foo()
```
真正property得实现应该比这复杂多了，但基本做法是这样。
还有一个疑问是怎么将property宿主得函数变成属性，代码如下：
```python
class MyProperty(object):
    def __init__(self, func):
        print 'my property init'
        self._func = func
    def __get__(self, obj, cls=None):
        return self._func(obj)
class A(object):
    def __init__(self):
        self._x = 10
    @MyProperty
    def x(self):
        return self._x
a = A()
print a.x
```
其他a.x = 1, del a.x 这样得功能就是给MyProperty添加 __set__, __del__ 方法。
============================================================================
classproperty [http://pypi.python.org/pypi/classproperty](http://pypi.python.org/pypi/classproperty)
这个代码太奇怪了，类继承于实例，看了很久都没看出所以然来，后来经人指点才知道了。先看个小例子
```python
class A(object):
    def __init__(self, *args):
        print 'A init'
        print self, args
        super(A, self).__init__()
    def __call__(self):
        print 'A callable'
        return self
a = A()
print a()
print '============================='
class B(a):
    def __init__(self):
        super(B, self).__init__()
b = B()
输出是：
A init
<__main__.A object at 0xb72bc38c> ()
A callable
<__main__.A object at 0xb72bc38c>
=============================
A init
<__main__.A object at 0xb72bc3ec> ('B', (<__main__.A object at 0xb72bc38c>,), {'__module__': '__main__', '__init__': <function __init__ at 0xb72adc6c>})
A callable
```
这样应该就可以明白  classpropertytype 为什么这么写， 为什么类可以继承于实例了。A.__call__ 很重要，没有它b就不会被创建，我猜想对象实例化得时候对父类应该是 有__new__属性就会去执行new， 不然就去执行__call__。
ps. 以上说的有误，pls google python descriptor -- 20121126.
