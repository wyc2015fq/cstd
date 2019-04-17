# Python内置函数(54)——callable - lincappu - 博客园







# [Python内置函数(54)——callable](https://www.cnblogs.com/lincappu/p/8145188.html)





**英文文档：**

`callable`(*object*)

　　Return `True` if the *object* argument appears callable, `False` if not. If this returns true, it is still possible that a call fails, but if it is false, calling *object* will never succeed. Note that classes are callable (calling a class returns a new instance); instances are callable if their class has a `__call__()` method.

**　　检测对象是否可被调用**

**说明：**

　　1. 方法用来检测对象是否可被调用，可被调用指的是对象能否使用()括号的方法调用。

```
>>> callable(callable)
True
>>> callable(1)
False
>>> 1()
Traceback (most recent call last):
File "<pyshell#5>", line 1, in <module>
1()
TypeError: 'int' object is not callable
>>>
```

　　2. 可调用对象，在实际调用也可能调用失败；但是不可调用对象，调用肯定不成功。

　　3. 类对象都是可被调用对象，类的实例对象是否可调用对象，取决于类是否定义了__call__方法。

```
>>> class A: #定义类A
    pass

>>> callable(A) #类A是可调用对象
True
>>> a = A() #调用类A
>>> callable(a) #实例a不可调用
False
>>> a() #调用实例a失败
Traceback (most recent call last):
  File "<pyshell#31>", line 1, in <module>
    a()
TypeError: 'A' object is not callable


>>> class B: #定义类B
    def __call__(self):
        print('instances are callable now.')

        
>>> callable(B) #类B是可调用对象
True
>>> b = B() #调用类B
>>> callable(b) #实例b是可调用对象
True
>>> b() #调用实例b成功
instances are callable now.
```












