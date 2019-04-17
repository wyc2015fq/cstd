# Python内置函数(63)——property - lincappu - 博客园







# [Python内置函数(63)——property](https://www.cnblogs.com/lincappu/p/8145365.html)





**英文文档：**

*class *`property`(*fget=None*, *fset=None*, *fdel=None*, *doc=None*)

Return a property attribute.

*fget* is a function for getting an attribute value. *fset* is a function for setting an attribute value. *fdel* is a function for deleting an attribute value. And *doc* creates a docstring for the attribute.

If given, *doc* will be the docstring of the property attribute. Otherwise, the property will copy *fget*‘s docstring (if it exists). This makes it possible to create read-only properties easily using [`property()`](mk:@MSITStore:D:\Python\Python35-32\Doc\Python352.chm::/library/functions.html#property) as a decorator。

The `@property` decorator turns the `voltage()` method into a “getter” for a read-only attribute with the same name, and it sets the docstring for *voltage* to “Get the current voltage.”

A property object has `getter`, `setter`, and `deleter` methods usable as decorators that create a copy of the property with the corresponding accessor function set to the decorated function. 

 　　显示属性的装饰器

**说明：**

　　1. property是一个类，其作用是用来包装类的属性，这个属性可以根据实际需要，控制是否可读(设置fget参数)、可写(设置fset参数)、可删除(设置fdel参数)。

```
class C:
    def __init__(self):
        self._x = '_x in C'

    def getx(self):
        return self._x

    def setx(self, value):
        self._x = value

    def delx(self):
        del self._x

    x = property(getx, setx, delx, "I'm the 'x' property.")
```

```
>>> c = C()
>>> c.x # 调用 getx
'_x in C'

>>> c.x = 'x had changed' # 调用 setx
>>> c.x # 调用 getx
'x had changed'

>>> del c.x  # 调用 delx
>>> c.x # 调用 getx
Traceback (most recent call last):
  File "<pyshell#34>", line 1, in <module>
    c.x
  File "<pyshell#28>", line 6, in getx
    return self._x
AttributeError: 'C' object has no attribute '_x'
```

　　2. 参数doc表示的是属性的说明，如果没有指定，将从fget参数指定的方法中读取。

```
>>> help(c)

*Help on C in module __main__ object:class C(builtins.object) |  Methods defined here: |   |  __init__(self) |      Initialize self.  See help(type(self)) for accurate signature. |   |  delx(self) |   |  getx(self) |   |  setx(self, value) |   |  ---------------------------------------------------------------------- |  Data descriptors defined here: |   |  __dict__ |      dictionary for instance variables (if defined) |   |  __weakref__ |      list of weak references to the object (if defined) |   |  x |      I'm the 'x' property.# 最后一行中 文档字符串 为I'm the 'x' property.>>> class C:    def __init__(self):        self._x = '_x in C'    def getx(self):        """I'm the 'x' property. provide by getx"""        return self._x    def setx(self, value):        self._x = value    def delx(self):        del self._x    x = property(getx, setx, delx)    >>> help(C)Help on class C in module __main__:class C(builtins.object) |  Methods defined here: |   |  __init__(self) |      Initialize self.  See help(type(self)) for accurate signature. |   |  delx(self) |   |  getx(self) |      I'm the 'x' property. provide by getx |   |  setx(self, value) |   |  ---------------------------------------------------------------------- |  Data descriptors defined here: |   |  __dict__ |      dictionary for instance variables (if defined) |   |  __weakref__ |      list of weak references to the object (if defined) |   |  x |      I'm the 'x' property. provide by getx# 最后一行中 文档字符串 为 I'm the 'x' property. provide by getx*
```

　　3. property更优雅的做法是用作装饰器，装饰过的方法就可以以属性方式调用。同时将生成.setter和.deleter装饰器，用于指定可入方法，删除方法。

```
>>> class C:
    def __init__(self):
        self._x = '_x in C'

    @property
    def x(self):
        """I'm the 'x' property."""
        return self._x

    @x.setter
    def x(self, value):
        self._x = value

    @x.deleter
    def x(self):
        del self._x
```

```
>>> c = C()
>>> c.x # 调用 getx
'_x in C'

>>> c.x = 'x had changed' # 调用 setx
>>> c.x # 调用 getx
'x had changed'

>>> del c.x  # 调用 delx
>>> c.x # 调用 getx
Traceback (most recent call last):
  File "<pyshell#34>", line 1, in <module>
    c.x
  File "<pyshell#28>", line 6, in getx
    return self._x
AttributeError: 'C' object has no attribute '_x'
```

　　4. 不直接使用字段，而使用属性，主要因为可以控制外部对类字段的恶意修改和删除，而且可以再设置属性值的时候进行适当的验证。

```
>>> class C:
    def __init__(self):
        self._name = ''
    @property
    def name(self):
        """i'm the 'name' property."""
        return self._name
    @name.setter
    def name(self,value):
        if value is None:
            raise RuntimeError('name can not be None')
        else:
            self._name = value

            
>>> c = C()

>>> c.name # 访问属性
''
>>> c.name = None # 设置属性时进行验证
Traceback (most recent call last):
  File "<pyshell#84>", line 1, in <module>
    c.name = None
  File "<pyshell#81>", line 11, in name
    raise RuntimeError('name can not be None')
RuntimeError: name can not be None

>>> c.name = 'Kim' # 设置属性
>>> c.name # 访问属性
'Kim'

>>> del c.name # 删除属性，不提供deleter则不能删除
Traceback (most recent call last):
  File "<pyshell#87>", line 1, in <module>
    del c.name
AttributeError: can't delete attribute
>>> c.name
'Kim'
```












