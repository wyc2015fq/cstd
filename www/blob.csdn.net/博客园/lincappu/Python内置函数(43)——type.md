# Python内置函数(43)——type - lincappu - 博客园







# [Python内置函数(43)——type](https://www.cnblogs.com/lincappu/p/8145050.html)





**英文文档：**

*class *`type`(*object*)

*class *`type`(*name*, *bases*, *dict*)

With one argument, return the type of an *object*. The return value is a type object and generally the same object as returned by `object.__class__`.

The `isinstance()` built-in function is recommended for testing the type of an object, because it takes subclasses into account.

With three arguments, return a new type object. This is essentially a dynamic form of the `class` statement. The *name*string is the class name and becomes the `__name__` attribute; the *bases* tuple itemizes the base classes and becomes the `__bases__` attribute; and the *dict* dictionary is the namespace containing definitions for class body and is copied to a standard dictionary to become the `__dict__` attribute. 

 　　返回对象的类型，或者根据传入的参数创建一个新的类型

**说明：**

　　1. 函数只传入一个参数时，返回参数对象的类型。 返回值是一个类型对象，通常与对象.__ class__返回的对象相同。

```
#定义类型A
>>> class A:
    name = 'defined in A'

#创建类型A实例a
>>> a = A()

#a.__class__属性
>>> a.__class__
<class '__main__.A'>

#type(a)返回a的类型
>>> type(a)
<class '__main__.A'>

#测试类型
>>> type(a) == A
True
```

　　2. 虽然可以通过type函数来检测一个对象是否是某个类型的实例，但是更推荐使用isinstance函数，因为isinstance函数考虑了父类子类间继承关系。

```
#定义类型B，继承A
>>> class B(A):
    age = 2

#创建类型B的实例b
>>> b = B()

#使用type函数测试b是否是类型A，返回False
>>> type(b) == A
False

#使用isinstance函数测试b是否类型A，返回True
>>> isinstance(b,A)
True
```

　　3. 函数另一种使用方式是传入3个参数，函数将使用3个参数来创建一个新的类型。其中第一个参数name将用作新的类型的类名称，即类型的__name__属性；第二个参数是一个元组类型，其元素的类型均为类类型，将用作新创建类型的基类，即类型的__bases__属性；第三个参数dict是一个字典，包含了新创建类的主体定义，即其值将复制到类型的__dict__属性中。

```
#定义类型A，含有属性InfoA
>>> class A(object):
    InfoA = 'some thing defined in A'

#定义类型B，含有属性InfoB
>>> class B(object):
    InfoB = 'some thing defined in B'

#定义类型C，含有属性InfoC
>>> class C(A,B):
    InfoC = 'some thing defined in C'

#使用type函数创建类型D，含有属性InfoD
>>> D = type('D',(A,B),dict(InfoD='some thing defined in D'))

#C、D的类型
>>> C
<class '__main__.C'>
>>> D
<class '__main__.D'>

#分别创建类型C、类型D的实例
>>> c = C()
>>> d = D()

#分别输出实例c、实例b的属性
>>> (c.InfoA,c.InfoB,c.InfoC)
('some thing defined in A', 'some thing defined in B', 'some thing defined in C')
>>> (d.InfoA,d.InfoB,d.InfoD)
('some thing defined in A', 'some thing defined in B', 'some thing defined in D')
```












