# python对象的创建和销毁 - Orisun - 博客园







# [python对象的创建和销毁](https://www.cnblogs.com/zhangchaoyang/articles/4984768.html)





## 对象的创建

pyhton用__new__来创建对象(__new__相当于Java中的构建函数)，对象创建好之后会立即调用__init__方法，__init__方法有个参数self就是刚才__new__创建好的那个对象。通过我们有__init__方法中给对象的属性进行赋值，或者动态线对象添加属性并赋值。

```
class Student(object):

    cnt = 0  # 用于记录该类的实例创建了多少个

    def __new__(cls, *args, **kwargs):
        print '__new__'
        cls.cnt += 1
        return super(Student, cls).__new__(cls, *args, **kwargs)

    def __init__(self):
        print '__init__'
        self.name = 'ELE' + str(self.cnt)


if __name__ == '__main__':
    inst1 = Student()
    print 
    inst2 = Student()
    print inst2.name
    print Student.cnt
```

输出

```
__new__
__init__

__new__
__init__
ELE2
2
```

对象被当作函数来使用时，会调用对象的__call__方法，相当于是__call__重载了小括号运算符。

```
class Student(object):

    def __init__(self):
        self.name='orisun'

    def __call__(self):
        print '__call__'
        self.name = 'SOS'


if __name__ == '__main__':
    inst = Student()
    print inst.name
    print
    inst()
    print inst.name
```

输出

```
orisun

__call__
SOS
```

## 用__new__实现单例模式

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# coding:utf-8


class Singleton1(object):

    name = ''

    def __new__(cls, *args, **kwargs):
        if not '_instance' in vars(cls):
            print 'creating instance of Singleton1'
            cls._instance = super(Singleton1, cls).__new__(cls)
            if len(args) > 0:
                cls.name = args[0]
            elif 'name' in kwargs:
                cls.name = kwargs[name]
        return cls._instance


class Singleton2(object):

    name = ''

    def __new__(cls, *args, **kwargs):
        if not '_instance' in vars(cls):
            print 'creating instance of Singleton2'
            cls._instance = super(Singleton2, cls).__new__(cls)
            if len(args) > 0:
                cls.name = args[0]
            elif 'name' in kwargs:
                cls.name = kwargs['name']
        return cls._instance

if __name__ == '__main__':
    inst1 = Singleton1('a')
    inst2 = Singleton1(name='b')
    print inst1 is inst2
    print inst1.name
    print inst2.name
    print '*' * 10
    inst3 = Singleton2(name='b')
    inst4 = Singleton2('a')
    print inst3 is inst4
    print inst3.name
    print inst4.name

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

输出：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

creating instance of Singleton1
True
a
a
**********
creating instance of Singleton2
True
b
b

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

判断2个对象是否为同一个对象我们用了关键字is。

上例中我们充分利用了__new__()函数的两个参数：*args和**kwargs。

## 用__new__实现对实例的缓存

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# coding:utf-8

import weakref


class Cached(object):

    _cache = weakref.WeakValueDictionary()

    def __new__(cls, *args, **kwargs):
        name = None
        if len(args) > 0:
            name = args[0]
        elif 'name' in kwargs:
            name = kwargs['name']
        else:
            name = ''
        if not name in cls._cache:
            _instance = super(Cached, cls).__new__(cls)
            cls._cache[name] = _instance
        return cls._cache[name]

    def __init__(self, *args, **kwargs):
        if len(args) > 0:
            self.name = args[0]
        elif 'name' in kwargs:
            self.name = kwargs['name']


if __name__ == '__main__':
    inst1 = Cached('a')
    inst2 = Cached(name='b')
    inst3 = Cached(name='a')
    print 'inst1 is inst2' if (inst1 is inst2) else 'inst1 is not inst2'
    print 'inst1 is inst3' if (inst1 is inst3) else 'inst1 is not inst3'
    print 'inst1.name =', inst1.name
    print 'inst2.name =', inst2.name
    print 'inst3.name =', inst3.name

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

输出：

```
inst1 is not inst2
inst1 is inst3
inst1.name = a
inst2.name = b
inst3.name = a
```

## 对象的销毁

python和Java一样都是垃圾自动回收，不需要我们显示地销毁对象。执行del obj时会调用对象的__del__方法，这样对象的引用计数会减1，当对象的引用计数为0时，对象就会被销毁，内存就会被回收。

```
import gc

class A(object):

    def __init__(self):
        self.large_list=[i for i in xrange(10000000)]

    def __del__(self):
        print "__del__"

@profile
def foo():
    a=A()
    b=a
    del b
    del a
    gc.collect()
    print

if __name__ == '__main__':
    foo()
```

我们使用memory_profiler模块监控每行代码内存的变化。

```
$ python -m memory_profiler a.py

__del__

 

Filename: a.py

 

Line #    Mem usage    Increment   Line Contents

================================================

    13    9.066 MiB    0.000 MiB   @profile

    14                             def foo():

    15  251.051 MiB  241.984 MiB       a=A()

    16  251.051 MiB    0.000 MiB       b=a

    17  251.051 MiB    0.000 MiB       del b

    18  243.863 MiB   -7.188 MiB       del a

    19   20.863 MiB -223.000 MiB       gc.collect()

    20   20.863 MiB    0.000 MiB       print
```

可见del b时，对象的引用计数还没有变为0，故内存占用没有丝毫减少。当del a时，对象的引用计数变为0，内存有所减少，但是对象占用的内存并没有立即完全回收，直到显式调用gc.collect()。

## 通过MetaClass创建类

```
class MetaClass(type):
    def __new__(cls,class_name,base_classes,attr_dict):
        print '__new__ in MetaClass'
        return super(MetaClass,cls).__new__(cls,class_name,base_classes,attr_dict)

    def __init__(cls,class_name,base_classes,attr_dict): 
        print '__init__ in MetaClass'
        super(MetaClass, cls).__init__(class_name,base_classes,attr_dict) 

    def __call__(cls,*args): 
        print '__call__ in MetaClass'
        print 'AAAAAAAAAAAAAAA'
        super(MetaClass, cls).__call__(*args)
        print 'BBBBBBBBBBBBBBB'
   
class A(object):
    __metaclass__=MetaClass
    name='orisun'

    def __new__(cls, *args, **kwargs):
        print '__new__ in A'
        return super(A, cls).__new__(cls, *args, **kwargs)

    def __init__(self):
        print '__init__ in A'


if __name__ == '__main__':
    print '='*40
    a=A()
    print '='*40
```

输出

```
__new__ in MetaClass
__init__ in MetaClass
========================================
__call__ in MetaClass
AAAAAAAAAAAAAAA
__new__ in A
__init__ in A
BBBBBBBBBBBBBBB
========================================
```

我们看到，不需要执行类A之外的任何代码，单纯是在定义完类A之后，MetaClass中的__new__和__init__就被执行了。

当创建类实例时，MetaClass中的__call__会被执行，__call__中调用super.__call__时，类A的__new__和__init__才被调到。

## 用MetaClass实现单例模式

```
# coding:utf-8


class Singleton(type):

    def __init__(cls, class_name, base_classes, attr_dict):
        cls.__instance = None
        super(Singleton, cls).__init__(class_name, base_classes, attr_dict)

    def __call__(cls, *args, **kwargs):
        if cls.__instance is None:
            cls.__instance = super(Singleton, cls).__call__(*args, **kwargs)
            return cls.__instance
        else:
            return cls.__instance


class Segger(object):

    __metaclass__ = Singleton

    def __init__(self, name):
        self.name = name

if __name__ == '__main__':
    inst1 = Segger('a')
    inst2 = Segger('b')
    print inst1 is inst2
    print inst1.name
    print inst2.name
```

输出：

```
True
a
a
```














