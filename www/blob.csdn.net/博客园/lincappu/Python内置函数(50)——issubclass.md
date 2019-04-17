# Python内置函数(50)——issubclass - lincappu - 博客园







# [Python内置函数(50)——issubclass](https://www.cnblogs.com/lincappu/p/8145116.html)






** 英文文档：**
- `issubclass`(*class*, *classinfo*)
- 
- Return true if *class* is a subclass (direct, indirect or virtual) of *classinfo*. A class is considered a subclass of itself. *classinfo* may be a tuple of class objects, in which case every entry in *classinfo* will be checked. In any other case, a `TypeError` exception is raised.
- 
- 
- 
- 
- 
- **　　判断类是或否是另一个类或者类型元组中任意类元素的子类**
- **说明：**
- 
- 　　1. 函数功能用于判断一个类型对象是否是另一个类型对象的子类，class参数表示需要检查的类型对象，calssinfo参数表示需要对比类型对象。
- 　　2. 如果class参数是classinfo类型对象(或者classinfo类对象的直接、间接、虚拟子类)的实例，返回True。

```
>>> issubclass(bool,int)
True
>>> issubclass(bool,(str))
False

>>> class A:
    pass
>>> class B(A):
    pass
>>> issubclass(B,A)
True
```

　　3. 任何一个类都是自己类的子类，即class和calssinfo传入相同类型时，返回True。

```
>>> class A:
    pass

>>> issubclass(A,A)
True
```

　　4. 如果classinfo类型对象，是多个类型对象组成的元组，如果class类型对象是元组的任一类型对象的子类，则返回True，否则返回False。

```
>>> issubclass(bool,int)
True
>>> issubclass(bool,str)
False

>>> issubclass(bool,(str,int))
True
```

　　5. 如果classinfo类型对象，不是一个类型对象或者由多个类型对象组成的元组，则会报错(TypeError)。

```
>>> issubclass(bool,[str,int])
Traceback (most recent call last):
  File "<pyshell#14>", line 1, in <module>
    issubclass(bool,[str,int])
TypeError: issubclass() arg 2 must be a class or tuple of classes
```















