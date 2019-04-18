# python类属性的访问、设置和删除 - Orisun - 博客园







# [python类属性的访问、设置和删除](https://www.cnblogs.com/zhangchaoyang/articles/4984074.html)





## 类属性和对象属性

我们把定义在类中的属性称为类属性，该类的所有对象共享类属性，类属性具有继承性，可以为类动态地添加类属性。

对象在创建完成后还可以为它添加额外的属性，我们把这部分属性称为对象属性，对象属性仅属于该对象，不具有继承性。

类属性和对象属性都会被包含在dir()中，而vars()是仅包含对象属性。vars()跟__dict__是等同的。

类属性和对象属性可类比于Java中的static成员和非static成员，只不python中的类属性和对象属性都是可以动态添加（和删除）的。

```
class A(object):

    name='orisun'

    def __init__(self):
        self.age=10

class B(A):

    city='bei jing'

    def __init__(self):
        self.tempurature=20

if __name__ == '__main__':
    a=A()
    print dir(A)
    print dir(a)
    print a.__dict__
    print vars(a)

    print 
    b=B()
    print dir(B)
    print dir(b)
    print b.__dict__
    print vars(b)
```

输出

```
['__class__', '__delattr__', '__dict__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'name']
['__class__', '__delattr__', '__dict__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'age', 'name']
{'age': 10}
{'age': 10}

['__class__', '__delattr__', '__dict__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'city', 'name']
['__class__', '__delattr__', '__dict__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'city', 'name', 'tempurature']
{'tempurature': 20}
{'tempurature': 20}
```

动态地为类添加类属性后，该类的所有对象也都添加了该属性（即使是动态添加类属性之前创建的对象）。通过实例修改属性，并不会影响其他实例的同名属性和类上的同名属性。

```
class A(object):

    name='orisun'

    def __init__(self):
        self.age=10

if __name__ == '__main__':
    a=A()
    print dir(a)
    A.city='BeiJing'    #动态添加类属性，会反应到所有对象上
    b=A()
    A.name='zcy'        #动态修改类属性，会反应到所有对象上
    print dir(b)
    print dir(a)
    print a.name        
    b.name='tom'        #通过实例修改属性，并不会影响其他实例的同名属性和类上的同名属性
    print a.name
    print A.name
    print b.name
```

输出

```
['__class__', '__delattr__', '__dict__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'age', 'name']
['__class__', '__delattr__', '__dict__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'age', 'city', 'name']
['__class__', '__delattr__', '__dict__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'age', 'city', 'name']
zcy
zcy
zcy
tom
```

下文中讨论的全部是类属性，不涉及对象属性。



对属性的访问、设置和删除又分为2种情况：
- 通过对象（实例）访问、设置和删除属性，即obj.attr、obj.attr=val、del obj.attr
- 通过类访问、设置和删除属性，即Cls.attr、Cls.attr=val、del Cls.attr

本文将针对这2种情况分别讨论。

## Descriptor

一个Descriptor是指实现了__get__的类，实现__set__和__delete__是可选的。同时实现了__get__和__set__则称为Data Descriptor，如果只实现了__get__则称为Non-data Descriptor。

```
class Descriptor(object):
    
    def __get__(self,instance,owner):
        return 'Descriptor in '+owner.__name__
    def __set__(self,obj,val):
        pass
    def __delete__(self,obj):
        pass
```

先给一个Descriptor的示例，__get__、__set__、__delete__的作用后文再细讲。

## 通过实例访问属性

__getattribute__、__getattr__、__get__和__dict__[attr]都是跟属性访问相关的方法，它们的优先级:
- 当类中定义了__getattribute__方法时，则调用__getattribute__。
- 如果访问的属性存在，且

2.1  属性是个Descriptor，是调用这个属性的__get__

2.2 属性不是Descriptor，则调用__dict__[attr]

- 如果类中没有定义该属性，则调用__getattr__
- 否则，抛出异常AttributeError　

### 验证4

```
class A(object):
    pass

if __name__ == '__main__':
    a=A()
    print a.d
```

输出：

```
AttributeError: 'A' object has no attribute 'd'
```

### 验证3

```
class A(object):
    def __getattr__(self,name):
        return name+" not found in "+self.__class__.__name__+" object"


if __name__ == '__main__':
    a=A()
    print a.d
```

输出：

```
d not found in A object
```

### 验证2.1

```
class Descriptor(object):
    
    def __get__(self,instance,owner):
        return 'Descriptor in '+owner.__name__
    

class A(object):
    d=Descriptor()
    def __getattr__(self,name):
        return name+" not found in "+self.__class__.__name__+" object"


if __name__ == '__main__':
    a=A()
    print a.d
```

输出：

```
Descriptor in A
```

__getattr__并没有被调用。

### 验证2.2

```
class A(object):
    d=10
    def __getattr__(self,name):
        return name+" not found in "+self.__class__.__name__+" object"


if __name__ == '__main__':
    a=A()
    print a.d
```

输出：

```
10
```

__getattr__并没有被调用。

### 验证1

```
class Descriptor(object):
    
    def __get__(self,instance,owner):
        return 'Descriptor in '+owner.__name__
    

class A(object):
    d=Descriptor()
    def __getattribute__(self,name):
        return '__getattribute__ '
    def __getattr__(self,name):
        return name+" not found in "+self.__class__.__name__+" object"


if __name__ == '__main__':
    a=A()
```

输出：

```
__getattribute__
```

__get__和__getattr__并没有被调用。

## 通过实例设置属性

跟属性设置相关的方法有3个：__setattr__、__set__和__dict__[attr]=val。它们的优先级跟get正好反过来：
- 如果类中定义了__setattr__方法，则直接调用__setattr__
- 如果赋值的属性是个Descriptor，且
2.1  该Descriptor中定义了__set__，则直接调用__set__
2.2  该Descriptor中没有定义__set__，则调用__dict__[attr]=val
- 
如果赋值的属性不是Descriptor，则直接调用__dict__[attr]=val

- 
如果该属性不存在，则动态地添加该属性，然后调用__dict__[attr]=val进行赋值




### 验证4

```
class A(object):
    pass

if __name__ == '__main__':
    a=A()
    a.d='hello'
    print a.d
```

输出：

```
hello
```

### 验证3

```
class A(object):
    d=10

if __name__ == '__main__':
    a=A()
    a.d=30
    print a.d
```

输出：

```
30
```

### 验证2.2

```
class Descriptor(object):
    
    def __get__(self,instance,owner):
        return 'Descriptor in '+owner.__name__

class A(object):
    d=Descriptor()

if __name__ == '__main__':
    a=A()
    a.d=30
    print a.d
```

输出：

```
30
```

### 验证2.1

```
class Descriptor(object):
    
    def __get__(self,instance,owner):
        return 'Descriptor in '+owner.__name__

    def __set__(self,instance,value):
        pass

class A(object):
    d=Descriptor()

if __name__ == '__main__':
    a=A()
    a.d=30
    print a.d
```

输出

```
Descriptor in A
```

因为代码“a.d=30”调用了__set__，而__set__又什么都没做，所以属性d还是Descriptor对象（而非30），那么在执行"print a.d"时自然就调到了__get__

### 验证1

```
class Descriptor(object):
    
    def __get__(self,instance,owner):
        return 'Descriptor in '+owner.__name__

    def __set__(self,instance,value):
        print '__set__'

class A(object):
    d=Descriptor()

    def __setattr__(self,name,value):
        print '__setattr__'

if __name__ == '__main__':
    a=A()
    a.d=30
    print a.d
```

输出

```
__setattr__
Descriptor in A
```

调用了__setattr__，而__set__并没有被调到。

## 通过实例删除属性

调用del instance.attr进行属性删除时可能会调到__delattr__或__delete__，它们的优先级跟set雷同。
- 如果类中定义了__delattr__方法，则直接调用__delattr__
- 如果赋值的属性是个Descriptor，且该Descriptor中定义了__delete__，则直接调用__delete__
- 如果赋值的属性是个Descriptor，且该Descriptor中没有定义__delete__，则会报异常AttributeError：属性是只读的
- 如果赋值的属性不是Descriptor，也会报异常AttributeError：属性是只读的
- 如果该属性不存在，则报异常AttributeError

###  验证5

```
class A(object):
    pass

if __name__ == '__main__':
    a=A()
    del a.d
```

输出

```
AttributeError: 'A' object has no attribute 'd'
```

### 验证4

```
class A(object):
    d=10

if __name__ == '__main__':
    a=A()
    del a.d
```

输出

```
AttributeError: 'A' object attribute 'd' is read-only
```

### 验证3

```
class Descriptor(object):
    
    def __get__(self,instance,owner):
        return 'Descriptor in '+owner.__name__


class A(object):
    d=Descriptor()

if __name__ == '__main__':
    a=A()
    del a.d
```

输出

```
AttributeError: 'A' object attribute 'd' is read-only
```

### 验证2

```
class Descriptor(object):
    
    def __get__(self,instance,owner):
        return 'Descriptor in '+owner.__name__

    def __delete__(self,instance):
        print '__delete__'

class A(object):
    d=Descriptor()

if __name__ == '__main__':
    a=A()
    del a.d
```

输出

```
__delete__
```

### 验证1

```
class Descriptor(object):
    
    def __get__(self,instance,owner):
        return 'Descriptor in '+owner.__name__

    def __delete__(self,instance):
        print '__delete__'

class A(object):
    d=Descriptor()

    def __delattr__(self,name):
        print '__delattr__'

if __name__ == '__main__':
    a=A()
    del a.d
```

输出

```
__delattr__
```

__delete__并没有被调用。

## __get__  __set__  __delete__参数说明

```
class Descriptor(object):
    
    def __get__(self,obj,owner):
        return '__get__',self,obj,owner

    def __set__(self,obj,val):
        print '__set__',self,obj,val

    def __delete__(self,obj):
        print '__delete__',self,obj
    

class A(object):
    d=Descriptor()

if __name__ == '__main__':
    a=A()
    print a.d
    a.d=3
    del a.d
```

输出

```
('__get__', <__main__.Descriptor object at 0x100481c10>, <__main__.A object at 0x1004a0fd0>, <class '__main__.A'>)
__set__ <__main__.Descriptor object at 0x100481c10> <__main__.A object at 0x1004a0fd0> 3
__delete__ <__main__.Descriptor object at 0x100481c10> <__main__.A object at 0x1004a0fd0>
```

可见，3个方法参数中的obj是Descriptor属性所在的对象，而owner参数（__get__中的owner参数）是该对象所属的类。



在上面的讨论中我们是通过实例操作属性，如果你作一下对应转换："实例转换到类，类转换到MetaClass"，那就是通过类操作属性的规则。这种对应转换也是容易理解的，应该类是用于创建对象的，而[MetaClass](http://www.cnblogs.com/zhangchaoyang/articles/4984833.html)是用于创建类的。

```
class MetaClass(object):
    pass  

class A(object):
    __metaclass__=MetaClass
```

通过类访问属性

通过A.attr访问属性的规则为：
- 如果MetaClass中有__getattribute__，则直接返回该__getattribute__的结果。
- 如果attr是个Descriptor，则直接返回Descriptor的__get__的结果。
- 如果attr是通过属性，则直接返回attr的值
- 如果类中没有attr，且MetaClass中定义了__getattr__，则调用MetaClass中的__getattr__
- 如果类中没有attr，且MetaClass中没有定义__getattr__，则抛出异常AttributeError

## 通过类设置属性

通过A.attr=val给属性赋值时：
- 如果MetaClass中定义了__setattr__，则执行该__setattr__
- 如果该属性是Descriptor，且定义了__set__，则执行Descriptor的__set__
- 如果是普通属性或None-data Descriptor，则直接令attr=val
- 如果属性不存在，则动态给类添加该属性，然后进行赋值

## 通过类删除属性

通过del A.attr删除属性时：
- 如果MetaClass中定义了__delattr__，则执行该__delattr__
- 如果该属性是Descriptor，且定义了__delete__，则执行Descriptor的__delete__
- 如果是普通属性，或虽是Descriptor但是没有定义__delete__，则直接从A.__dict__中删除该属性
- 如果属性不存在，则抛出异常AttributeError












