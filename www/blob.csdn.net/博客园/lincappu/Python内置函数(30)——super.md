# Python内置函数(30)——super - lincappu - 博客园







# [Python内置函数(30)——super](https://www.cnblogs.com/lincappu/p/8144873.html)





**英文文档：**

`super`([*type*[, *object-or-type*]])

Return a proxy object that delegates method calls to a parent or sibling class of *type*. This is useful for accessing inherited methods that have been overridden in a class. The search order is same as that used by `getattr()` except that the *type* itself is skipped.

The `__mro__` attribute of the *type* lists the method resolution search order used by both `getattr()` and `super()`. The attribute is dynamic and can change whenever the inheritance hierarchy is updated.

If the second argument is omitted, the super object returned is unbound. If the second argument is an object, `isinstance(obj, type)` must be true. If the second argument is a type, `issubclass(type2, type)` must be true (this is useful for classmethods).

There are two typical use cases for *super*. In a class hierarchy with single inheritance, *super* can be used to refer to parent classes without naming them explicitly, thus making the code more maintainable. This use closely parallels the use of *super* in other programming languages.

The second use case is to support cooperative multiple inheritance in a dynamic execution environment. This use case is unique to Python and is not found in statically compiled languages or languages that only support single inheritance. This makes it possible to implement “diamond diagrams” where multiple base classes implement the same method. Good design dictates that this method have the same calling signature in every case (because the order of calls is determined at runtime, because that order adapts to changes in the class hierarchy, and because that order can include sibling classes that are unknown prior to runtime).

Note that `super()` is implemented as part of the binding process for explicit dotted attribute lookups such as `super().__getitem__(name)`. It does so by implementing its own `__getattribute__()` method for searching classes in a predictable order that supports cooperative multiple inheritance. Accordingly, `super()` is undefined for implicit lookups using statements or operators such as `super()[name]`.

Also note that, aside from the zero argument form, `super()` is not limited to use inside methods. The two argument form specifies the arguments exactly and makes the appropriate references. The zero argument form only works inside a class definition, as the compiler fills in the necessary details to correctly retrieve the class being defined, as well as accessing the current instance for ordinary methods.

 　　根据传入的参数生成一个新的子类和父类关系的代理对象

**说明：**

　　1. super函数返回的是一个代理对象，通过此对象可以调用所在类的父类或者兄弟类的方法，而不显示的指定父类或者兄弟类的类名。

　　2. 为什么要有super?

　　　　最早之前，在子类(B)中调用父类(A)的方法采用的方式如下： 

```
#定义父类A
>>> class A(object):
    def __init__(self):
        print('A.__init__')

#实例化A        
>>> a = A() 
A.__init__

# 定义子类B，继承A，在B的__init__ 方法中调用A的__init__方法
>>> class B(A): 
    def __init__(self):
        print('B.__init__')
        A.__init__(self)

 #实例化B
>>> b = B()
B.__init__
A.__init__
```

　　　　假设现在要更改新定义一个类(A1)，并更改继承关系(B->A改成B->A1)，则需要所有类中做如下修改：

```
#定义新的父类A1
>>> class A1(object):
    def __init__(self):
        print('A1.__init__')

#更改继承关系B->A改成B->A1
>>> class B(A1):
    def __init__(self):
        print('B.__init__')
        A1.__init__(self)

#能正确调用新的父类A1的__init__方法
>>> b = B()
B.__init__
A1.__init__

#假设忘了修改A.__init__(self)
>>> class B(A1):
    def __init__(self):
        print('B.__init__')
        A.__init__(self)
      
#则还是调用了A的__init__方法
>>> b = B()
B.__init__
A.__init__
```

　　　　引入super之后，不需要显示指定父类的类名，增强了程序的可维护性：

```
#B->A 改用super方式调用父类方法
>>> class B(A):
    def __init__(self):
        print('B.__init__')
        super().__init__()

#能正确调用父类方法
>>> b = B()
B.__init__
A.__init__

#更改继承关系B->A改成B->A1，调用父类方法方式不用修改
>>> class B(A1):
    def __init__(self):
        print('B.__init__')
        super().__init__()

#也能正确调用父类方法
>>> b = B()
B.__init__
A1.__init__
```

　　3. 不带任何参数的super等效于super(类名,self)，此种情况多用于单继承关系的子类中。

```
#super不带参数
>>> class B(A1):
    def __init__(self):
        print('B.__init__')
        super().__init__()

#能正确调用父类方法
>>> b = B()
B.__init__
A1.__init__

#super带两个参数(类名,self)
>>> class B(A1):
    def __init__(self):
        print('B.__init__')
        super(B,self).__init__()

#也能正确调用父类方法
>>> b = B()
B.__init__
A1.__init__
```

　　4. 如果第2个参数不传入，则表示代理对象不绑定继承关系。

```
#super第2个参数不传入，生成代理对象不绑定继承关系
>>> class B(A1):
    def __init__(self):
        print('B.__init__')
        super(B).__init__()

#super(B).__init__()方法执行时不会调用父类方法
>>> b = B()
B.__init__
```

　　5. 如果第2个参数是一个对象，则对象必须是第1个参数指定类型的实例，此种关系多用于多层继承关系的子类中。

```
#定义父类A
>>> class A(object):
    def __init__(self):
        print('A.__init__')

#定义子类B，继承A，__init__中调用父类的__init__方法
>>> class B(A):
    def __init__(self):
        print('B.__init__')
        super().__init__()

#定义子类C，继承B，__init__中调用父类的__init__方法        
>>> class C(B):
    def __init__(self):
        print('C.__init__')
        super().__init__()

#实例化C时，执行C的__init__方法，调用直接父类B的__init__方法，又进一步调用间接父类A的__init__方法
>>> c = C()
C.__init__
B.__init__
A.__init__

#重新定义子类C，继承关系不变，调用父类方法__init__时改用super(B,self)
>>> class C(B):
    def __init__(self):
        print('C.__init__')
        super(B,self).__init__()

#实例化C时，执行C的__init__方法，super(B,self)代理找到B的父类A，将self转换成B的实例，直接调用了A的__init__方法，跳过了调用B的__init__方法
>>> c = C()
C.__init__
A.__init__

#定义一个新类D
>>> class D(object):
    def __init__(self):
        print('D.__init__')

#重新定义C，继承关系不变，调用父类方法__init__时改用super(D,self)
>>> class C(B):
    def __init__(self):
        print('C.__init__')
        super(D,self).__init__()

#实例化C时，执行C的__init__方法，super(D,self)代理找到D的父类object，将self转换成D的实例，因为D和C无继承关系，self
无法转换成D的实例所以报错
>>> c= C()
C.__init__
Traceback (most recent call last):
  File "<pyshell#14>", line 1, in <module>
    c= C()
  File "<pyshell#13>", line 4, in __init__
    super(D,self).__init__()
TypeError: super(type, obj): obj must be an instance or subtype of type
```

　　6. 如果第2个参数时一个类型，则类型必须是第1个参数指定类型的子类，此种关系多用于多层继承关系的子类中，适用于类方法。

```
#定义父类A，并定义有一个类方法sayHello
>>> class A(object):
    @classmethod
    def sayHello(cls):
        print('A.sayHello')

# 定义子类B，继承A，重写类方法sayHello，在其中调用父类的sayHello方法
>>> class B(A):
    @classmethod
    def sayHello(cls):
        print('B.sayHello')
        super().sayHello()

# 定义子类C，继承B，重写类方法sayHello，在其中调用父类的sayHello方法
>>> class C(B):
    @classmethod
    def sayHello(cls):
        print('C.sayHello')
        super().sayHello()

#调用C的类方法sayHello，其调用C的直接父类B的类方法sayHello，调用时B的sayHello方法又调用B的直接父类A的类方法sayHello
>>> C.sayHello()
C.sayHello
B.sayHello
A.sayHello

#重新定义类C，继承关系不变，使用super(C,C)的方式调用父类方法
>>> class C(B):
    @classmethod
    def sayHello(cls):
        print('C.sayHello')
        super(C,C).sayHello()

#调用C的类方法sayHello，super(C,C)代理对象，找到C的直接父类B，然后调用C的直接父类B的类方法sayHello，调用时B的sayHello方法又调用B的直接父类A的类方法sayHello
>>> C.sayHello()
C.sayHello
B.sayHello
A.sayHello

#重新定义类C，继承关系不变，使用super(B,C)的方式调用父类方法
>>> class C(B):
    @classmethod
    def sayHello(cls):
        print('C.sayHello')
        super(B,C).sayHello()

#调用C的类方法sayHello，super(B,C)代理对象，找到B的直接父类A，然后调用B的直接父类A的类方法sayHello，中间不会调用B的sayHello方法
>>> C.sayHello()
C.sayHello
A.sayHello

#定义一个新类D，和A、B、C无继承关系
>>> class D(object):
    @classmethod
    def sayHello(cls):
        print('D.sayHello')

#重新定义类C，继承关系不变，使用super(D,C)的方式调用父类方法
>>> class C(B):
    @classmethod
    def sayHello(cls):
        print('C.sayHello')
        super(D,C).sayHello()

#调用C的类方法sayHello，super(D,C)代理对象，找到B的直接父类object，然后将C转换成D类，转换失败调用出错
>>> C.sayHello()
C.sayHello
Traceback (most recent call last):
  File "<pyshell#81>", line 1, in <module>
    C.sayHello()
  File "<pyshell#80>", line 5, in sayHello
    super(D,C).sayHello()
TypeError: super(type, obj): obj must be an instance or subtype of type
```












