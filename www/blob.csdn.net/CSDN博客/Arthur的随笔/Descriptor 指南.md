# Descriptor 指南 - Arthur的随笔 - CSDN博客
2012年12月07日 00:36:22[largetalk](https://me.csdn.net/largetalk)阅读数：1044
[http://docs.python.org/2/howto/descriptor.html](http://docs.python.org/2/howto/descriptor.html)
**Abstract**
**Definition and Introduction**
通常来说， descriptor 是一种绑定着特殊行为属性的对象， 在访问它时行为被descriptor协议定义的方法所重载。这些方法是__get__, __set__ 和__delete__。 如果对象定义了任一方法，这个对象就被叫做descriptor.
访问对象的属性默认行为是get, set或delete对象字典中的属性。例如， a.x查找路径是从a.__dict__['x']开始，然后是type(a).__dict__['x'],并继续查找type(a)的祖先类（不包括metaclass).如果查找的值是定义了descriptor方法的对象，python可能会重载默认行为转而调用descriptor方法。这个行为在哪里发生取决于定义了哪些descriptor方法。注意，descriptor仅在新型对象或类中有效（新型类是继承于object或type的类）
descriptor是个功能强大，通用的协议。它是实现properties, method, static methods, class methods 和 super()功能背后的机制。它被python自己用来实现在2.2版本中引入的新型类。descriptors简化底层c代码并为python程序提供一套灵活的工具。
**Descriptor Protocol**
```python
descr.__get__(self, obj, type=None) --> value
descr.__set__(self, obj, value) --> None
descr.__delete__(self, obj) --> None
```
所有东西就这些。定义其中任一方法的对象都被认为是descriptor并重载作为属性被查找时默认行为。
如果对象同时定义了__get__和__set__， 它就被认为是一个data descriptor. 如果仅定义__get__则被称为non-data descriptor（通常用于methods，但也可做其他用途）
data和non-data descriptor不同之处在于，对于实例的字典中的实体，关心的是如何重载。如果实例字典中有一个与data decriptor同名的实体， data descriptor优先.如果是与non-data descriptor同名， 字典实体优先。
如要创建只读的data descriptor， 同时定义__get__和__set__, 然后在__set__里抛出一个AttributeError异常。定义会抛出异常的__set__方法就能保证它是data descriptor.
**Invoking Descriptors**
descriptor可以直接用它的方法名调用， 例如d.__get__(obj)。
另外，更常见的调用方式是通过访问属性后自动被调用。例如， obj.d看起来是在obj的字典中查找d。如果d定义了__get__方法， 则d.__get__(obj)会根据下面的优先规则被调用。
调用的细节取决于obj是对象还是类。无论怎样， descriptor只在新型类中起作用。如果一个类是object的子类，它就是新型类。
对于对象，机制在于object.__getattribute__， 它转变b.x成type(b).__dict__['x'].__get__(b, type(b)).执行顺序沿着一个优先级链： data descriptor优于实例变量，实例变量优于non-datadescriptor，如有__getattr__则给予其最低的优先级。C实现可以在Object/object.c的PyObject_GeneriGetAttr()中找到。
对于类， 机制在于type.__getattribute__, 它转变B.x成B.__dict__['x'].__get__(None, B)。 在python中，看起来类似：
```python
def __getattribute__(self, key):
    "Emulate type_getattro() in Objects/typeobject.c"
    v = object.__getattribute__(self, key)
    if hasattr(v, '__get__'):
       return v.__get__(None, self)
    return v
```
需要记住的要点是：
-   descriptors 是由__getattribute__调用的
-   重载__getattribute__会阻止了descriptor的自动调用
-   __getattribute__是只有新型类和对象才有的
-   object.__getattribute__ 和 type.__getattribute__调用__get__是不一样的
-   data descriptor会重载实例字典
-   non-data descriptor 会被实例字典重载
由super()返回的对象为调用descriptors,也有个自定义的__getattribute__。 调用super(B, obj).m() 会搜索obj.__class__.__mro__， 先是基类A，马上是B然后返回A.__dict__['m'].__get(obj, A)。如果m不是descriptor, 返回的m不变.如果不在字典中， m再使用object.__getattribute__重新查找。
请注意， 在python2.2中， 如果仅当m是data descriptor，super(B,obj).m()才会调用__get__()。而在python2.3中，是non-data descriptor也会被调用，除非涉及到旧型（经典）类。实现细节在objects/typeobject.c的super_getattro()，相等python版本可在guido的指南中找到。
上述详细展示了descriptor机制和细节都是内嵌到object, type和super（）的__getattribute__方法中。类会继承这些机制当类继承于object或他们的meta-class能提供相似功能时。同样，类可以通过重载__getattribute__关闭descriptor调用。
**Descriptor Example**
下面的代码创建了一个是data descriptor的类，它在每次get或set时都打印一条消息。如想更改所有属性行为，重载__getattribute__是一个另一种方法。然而，如只想监控几个选定的属性，descriptor非常有用。
```python
class RevealAccess(object):
    """A data descriptor that sets and returns values
       normally and prints a message logging their access.
    """
    def __init__(self, initval=None, name='var'):
        self.val = initval
        self.name = name
    def __get__(self, obj, objtype):
        print 'Retrieving', self.name
        return self.val
    def __set__(self, obj, val):
        print 'Updating' , self.name
        self.val = val
>>> class MyClass(object):
    x = RevealAccess(10, 'var "x"')
    y = 5
>>> m = MyClass()
>>> m.x
Retrieving var "x"
10
>>> m.x = 20
Updating var "x"
>>> m.x
Retrieving var "x"
20
>>> m.y
5
```
descriptor 协议非常简单并且提供令人兴奋的各种可能性。几个用例是如此通用以至于他们都被打包成单独的涵数调用。properties, bound 和 unbound 方法，static方法， class方法都是基于descriptor 协议。
**Properties**
调用property()是构建data descriptor（存取属性会触发函数）的一个简洁方式。它的标志是：
```python
property(fget=None, fset=None, fdel=None, doc=None) -> property attribute
```
下面展示了一个典型应用：
```python
class C(object):
    def getx(self): return self.__x
    def setx(self, value): self.__x = value
    def delx(self): del self.__x
    x = property(getx, setx, delx, "I'm the 'x' property.")
```
property() python版实现：
```python
class Property(object):
    "Emulate PyProperty_Type() in Objects/descrobject.c"
    def __init__(self, fget=None, fset=None, fdel=None, doc=None):
        self.fget = fget
        self.fset = fset
        self.fdel = fdel
        self.__doc__ = doc
    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        if self.fget is None:
            raise AttributeError, "unreadable attribute"
        return self.fget(obj)
    def __set__(self, obj, value):
        if self.fset is None:
            raise AttributeError, "can't set attribute"
        self.fset(obj, value)
    def __delete__(self, obj):
        if self.fdel is None:
            raise AttributeError, "can't delete attribute"
        self.fdel(obj)
```
内建property()非常有用，当用户接口用来保证属性可用或在随后改变时需要干预。
比如， 一个spreadsheet类打算通过Cell('b10').value访问一个cell的值, 程序要求在每次访问之后随之重新计算cell的值；然而，程序员不希望客户端代码直接存取属性。解决方法是包装属性成一个property data descriptor.
```python
class Cell(object):
    . . .
    def getvalue(self, obj):
        "Recalculate cell before returning value"
        self.recalc()
        return obj._value
    value = property(getvalue)
```
Functions and Methods
python的面向对象特性是建立于基于函数的环境。因为有了non-data descriptor，函数和方法完美统一。
类字典中存储函数成方法。在类定义中， 定义方法使用def和lambda, 如同创建函数一样。唯一不一样的地方就是第一个参数是预留给对象实例的。按python惯例，实例引用被叫做self，但是也可以叫做this或其它变量名。
为了支持方法调用， 含有__get__()的函数绑定成方法。这意味着，所有函数都是non-data descriptor，它们会返回绑定方法还是非绑定方法取决于调用者是对象还是类。python版本如下：
```python
class Function(object):
    . . .
    def __get__(self, obj, objtype=None):
        "Simulate func_descr_get() in Objects/funcobject.c"
        return types.MethodType(self, obj, objtype)
```
在解释中展示function descriptor是如何实际工作的：
```python
>>> class D(object):
     def f(self, x):
          return x
>>> d = D()
>>> D.__dict__['f'] # Stored internally as a function
<function f at 0x00C45070>
>>> D.f             # Get from a class becomes an unbound method
<unbound method D.f>
>>> d.f             # Get from an instance becomes a bound method
<bound method D.f of <__main__.D object at 0x00B18C90>>
```
输出显示bound和unbound方法是两个不同类型。而它们可能就是如此被实现， 在实际的实现C代码（Objects/classobjec.c的PyMethod_Type）中，是一个对象，两种不同表述，取决于im_self字段被设置与否.
同样， 调用方法的效果也依赖于im_self. 如果im_self有值（绑定），原始函数（存在im_func)被调用且第一个参数设置为实例。如果未绑定， 所有参数不作更改传给原始函数。实际C实现代码instancemethod_call看起来仅稍微复杂，包括了一些类型检查。
**Static Methods and Class Methods**
non-data descriptor提供一种简单机制把函数绑定成方法
为了复用， 函数含有一个__get__(), 所以当被当作属性访问时，他们转化成了方法。non-data descriptor将obj.f(*args)转换成f(obj, *args), 而调用klass.f(*args)变成调用f(*args).
这个图表总结这两个变种：
|Transformation|Called from an Object|Called from a Class|
|----|----|----|
|function|f(obj, *args)|f(*args)|
|staticmethod|f(*args)|f(*args)|
|classmethod|f(type(obj), *args)|f(klass, *args)|
static method不做任何改变返回下面的函数。调用c.f或C.f都是相当于查找到object.__getattribute__(c, "f")或object.__getattribute__(C, "f"). 结果是， 函数无论从对象或类访问都变得一样。
static method的方法好处是不需要引用self变量
例如， 一个统计包可能包含一个处理实验数据的容器类。类提供正常方法来计算均值，中值和其他基于数据的统计公式。然而， 可能有些函数是概念相关去独立于数据的，如erf(x)在统计工作中是一个便利转换程式却不直接依赖于数据。它既能从对象调用s.erf(1.5)又能从类调用Sample.erf(1.5).
```python
>>> class E(object):
     def f(x):
          print x
     f = staticmethod(f)
>>> print E.f(3)
3
>>> print E().f(3)
3
```
python版本staticmethod如下：
```python
class StaticMethod(object):
 "Emulate PyStaticMethod_Type() in Objects/funcobject.c"
 def __init__(self, f):
      self.f = f
 def __get__(self, obj, objtype=None):
      return self.f
```
不同于static method， class method在调用函数前预先在参数列表中有类引用。格式是一样，无论调用者是对象还是类。
```python
>>> class E(object):
     def f(klass, x):
          return klass.__name__, x
     f = classmethod(f)
>>> print E.f(3)
('E', 3)
>>> print E().f(3)
('E', 3)
```
当函数仅需要类引用而不关心任何实例数据时这种行为非常有用。classmethod一个用处是创建可选类构造器。 在python2.3， 使用classmethod dict.fromkeys() 创建从keys列表中一个新字典， 如下：
```python
class Dict:
    . . .
    def fromkeys(klass, iterable, value=None):
        "Emulate dict_fromkeys() in Objects/dictobject.c"
        d = klass()
        for key in iterable:
            d[key] = value
        return d
    fromkeys = classmethod(fromkeys)
```
现在可以如下创建一个新dict：
```python
>>> Dict.fromkeys('abracadabra')
{'a': None, 'r': None, 'b': None, 'c': None, 'd': None}
```
python版本classmethod如下：
```python
class ClassMethod(object):
     "Emulate PyClassMethod_Type() in Objects/funcobject.c"
     def __init__(self, f):
          self.f = f
     def __get__(self, obj, klass=None):
          if klass is None:
               klass = type(obj)
          def newfunc(*args):
               return self.f(klass, *args)
          return newfunc
```
