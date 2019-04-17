# Python内置函数(31)——object - lincappu - 博客园







# [Python内置函数(31)——object](https://www.cnblogs.com/lincappu/p/8144883.html)





**英文文档：**

class object
Return a new featureless object. object is a base for all classes. It has the methods that are common to all instances of Python classes. This function does not accept any arguments.
Note：object does not have a __dict__, so you can’t assign arbitrary attributes to an instance of the object class.

 　　创建一个新的 object 对象

** 说明：**

　　1. object类是Python中所有类的基类，如果定义一个类时没有指定继承哪个类，则默认继承object类。

```
>>> class A:
    pass

>>> issubclass(A,object)
True
```

　　2. object类定义了所有类的一些公共方法。

```
>>> dir(object)
['__class__', '__delattr__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__le__', '__lt__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__']
```

　　3. object类没有定义 __dict__，所以不能对object类实例对象尝试设置属性值。

```
>>> a = object()
>>> a.name = 'kim' # 不能设置属性
Traceback (most recent call last):
  File "<pyshell#9>", line 1, in <module>
    a.name = 'kim'
AttributeError: 'object' object has no attribute 'name'

#定义一个类A
>>> class A:
    pass

>>> a = A()
>>> 
>>> a.name = 'kim' # 能设置属性
```












