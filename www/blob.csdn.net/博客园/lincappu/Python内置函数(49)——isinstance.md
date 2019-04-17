# Python内置函数(49)——isinstance - lincappu - 博客园







# [Python内置函数(49)——isinstance](https://www.cnblogs.com/lincappu/p/8145108.html)





**英文文档：**

`isinstance`(*object*, *classinfo*)

Return true if the *object* argument is an instance of the *classinfo* argument, or of a (direct, indirect or virtual) subclass thereof. If *object* is not an object of the given type, the function always returns false. If *classinfo* is a tuple of type objects (or recursively, other such tuples), return true if *object* is an instance of any of the types. If *classinfo* is not a type or tuple of types and such tuples, a `TypeError` exception is raised.

 　　 判断对象是否是类或者类型元组中任意类元素的实例

**说明：**

o　　1. 函数功能用于判断对象是否是类型对象的实例，object参数表示需要检查的对象，calssinfo参数表示类型对象。

　　2. 如果object参数是classinfo类型对象(或者classinfo类对象的直接、间接、虚拟子类)的实例，返回True。

```
>>> isinstance(1,int)
True
>>> isinstance(1,str)
False

# 定义3各类：C继承B，B继承A
>>> class A:
    pass

>>> class B(A):
    pass

>>> class C(B):
    pass

>>> a = A()
>>> b = B()
>>> c = C()
>>> isinstance(a,A) #直接实例
True
>>> isinstance(a,B)
False
>>> isinstance(b,A) #子类实例 
True
>>> isinstance(c,A) #孙子类实例
True
```

　　3. 如果object参数传入的是类型对象，则始终返回False。

```
>>> isinstance(str,str)
False
>>> isinstance(bool,int)
False
```

　　4. 如果classinfo类型对象，是多个类型对象组成的元组，如果object对象是元组的任一类型对象中实例，则返回True，否则返回False。

```
>>> isinstance(a,(B,C))
False
>>> isinstance(a,(A,B,C))
True
```

　　5. 如果classinfo类型对象，不是一个类型对象或者由多个类型对象组成的元组，则会报错(TypeError)。

```
>>> isinstance(a,[A,B,C])
Traceback (most recent call last):
  File "<pyshell#23>", line 1, in <module>
    isinstance(a,[A,B,C])
TypeError: isinstance() arg 2 must be a type or tuple of types
```












