# Python内置函数(40)——dir - lincappu - 博客园







# [Python内置函数(40)——dir](https://www.cnblogs.com/lincappu/p/8144992.html)





**英文文档：**

`dir`([*object*])

Without arguments, return the list of names in the current local scope. With an argument, attempt to return a list of valid attributes for that object.

If the object has a method named `__dir__()`, this method will be called and must return the list of attributes. This allows objects that implement a custom `__getattr__()` or `__getattribute__()` function to customize the way `dir()`reports their attributes.

If the object does not provide `__dir__()`, the function tries its best to gather information from the object’s `__dict__`attribute, if defined, and from its type object. The resulting list is not necessarily complete, and may be inaccurate when the object has a custom `__getattr__()`.

The default `dir()` mechanism behaves differently with different types of objects, as it attempts to produce the most relevant, rather than complete, information:
- If the object is a module object, the list contains the names of the module’s attributes.
- If the object is a type or class object, the list contains the names of its attributes, and recursively of the attributes of its bases.
- Otherwise, the list contains the object’s attributes’ names, the names of its class’s attributes, and recursively of the attributes of its class’s base classes. 

 　　返回对象或者当前作用域内的属性列表

**说明：**

　　1. 当不传参数时，返回当前作用域内的变量、方法和定义的类型列表。

```
>>> dir()
['__builtins__', '__doc__', '__loader__', '__name__', '__package__', '__spec__']
>>> a = 10 #定义变量a
>>> dir() #多了一个a
['__builtins__', '__doc__', '__loader__', '__name__', '__package__', '__spec__', 'a']
```

　　2. 当参数对象是模块时，返回模块的属性、方法列表。

```
>>> import math
>>> math
<module 'math' (built-in)>
>>> dir(math)
['__doc__', '__loader__', '__name__', '__package__', '__spec__', 'acos', 'acosh', 'asin', 'asinh', 'atan', 'atan2', 'atanh', 'ceil', 'copysign', 'cos', 'cosh', 'degrees', 'e', 'erf', 'erfc', 'exp', 'expm1', 'fabs', 'factorial', 'floor', 'fmod', 'frexp', 'fsum', 'gamma', 'gcd', 'hypot', 'inf', 'isclose', 'isfinite', 'isinf', 'isnan', 'ldexp', 'lgamma', 'log', 'log10', 'log1p', 'log2', 'modf', 'nan', 'pi', 'pow', 'radians', 'sin', 'sinh', 'sqrt', 'tan', 'tanh', 'trunc']
```

　　3. 当参数对象是类时，返回类及其子类的属性、方法列表。

```
>>> class A:
    name = 'class'

    
>>> a = A()
>>> dir(a) #name是类A的属性，其他则是默认继承的object的属性、方法
['__class__', '__delattr__', '__dict__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__le__', '__lt__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'name']
```

　　4. 当对象定义了`__dir__方法，则返回__dir__方法的结果`

```
>>> class B:
    def __dir__(self):
        return ['name','age']

>>> b = B()
>>> dir(b) #调用 __dir__方法
['age', 'name']
```












