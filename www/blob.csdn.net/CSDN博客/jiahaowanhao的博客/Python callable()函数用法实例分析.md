
# Python callable()函数用法实例分析 - jiahaowanhao的博客 - CSDN博客


2018年05月07日 21:21:32[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：359


[Python callable()函数用法实例分析](http://cda.pinggu.org/view/25478.html)
本文实例讲述了Python callable()函数用法。分享给大家供大家参考，具体如下：
python中的内建函数callable( ) ，可以检查一个对象是否是可调用的 。
对于函数, 方法, lambda 函数式, 类, 以及实现了 _ _call_ _ 方法的类实例, 它都返回 True.
>>> help(callable)
Help on built-in function callable in module __builtin__:
callable(...)
callable(object) -> bool
Return whether the object is callable (i.e., some kind of function).
Note that classes are callable, as are instances with a __call__() method.
1》函数是可调用的
>>> def add(x,y):
...  return x+y
...
>>> callable(add)
True
2》类和类内的方法是可调用的
>>> class C:
...  def printf(self):
...   print 'This is class C!'
...
>>> objC=C()
>>> callable(C)\#类是可调用的，调用它们, 就产生对应的类实例.
True
>>> callable(C.printf)
True
>>> callable(objC.printf)
True
3》实现了__call__()方法的类实例是可调用的
>>> class A:
...  def printf(self):
...   print 'This is class A!'
...
>>> objA=A()
>>> callable(A) \#类是可调用的，调用它们, 就产生对应的类实例.
True
>>> callable(objA) \#类A没有实现__call__()方法，因此，类A的实例是不可调用的
False
>>> class B:
...  def __call__(self):
...   print 'This is class B!'
...
>>> objB=B()
>>> callable(B) \#类是可调用的，调用它们, 就产生对应的类实例.
True
>>> callable(objB) \#类B实现了__call__()方法，因此，类B实例是可调用的
True
4》lambda表达式是可调用的
>>> f=lambda x,y:x+y
>>> f(2,3)
5
>>> callable(f)
True
其它的，像整数，字符串，列表，元组，字典等等，都是不可调用的，如下：
>>> callable(2)
False
>>> callable('python')
False
>>> l=[1,2,3]
>>> callable(l)
False
>>> t=(4,5,6)
>>> callable(t)
False
>>> d={'a':1,'b':2}
>>> callable(d)
False

