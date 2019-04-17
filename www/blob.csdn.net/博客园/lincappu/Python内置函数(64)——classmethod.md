# Python内置函数(64)——classmethod - lincappu - 博客园







# [Python内置函数(64)——classmethod](https://www.cnblogs.com/lincappu/p/8145372.html)





**英文文档：**

`classmethod`(*function*)

Return a class method for *function*.

A class method receives the class as implicit first argument, just like an instance method receives the instance. To declare a class method, use this idiom:


class C:
    @classmethod
    def f(cls, arg1, arg2, ...): ...



The `@classmethod` form is a function decorator – see the description of function definitions in Function definitions for details.

It can be called either on the class (such as `C.f()`) or on an instance (such as `C().f()`). The instance is ignored except for its class. If a class method is called for a derived class, the derived class object is passed as the implied first argument.

Class methods are different than C++ or Java static methods. If you want those, see `staticmethod()` in this section.

　 标记方法为类方法的装饰器 

**说明：**

　　1. `classmethod 是一个装饰器函数，用来标示一个方法为类方法`

　　2. 类方法的第一个参数是类对象参数，在方法被调用的时候自动将类对象传入，参数名称约定为cls

　　3. 如果一个方法被标示为类方法，则该方法可被类对象调用(如 C.f())，也可以被类的实例对象调用(如 C().f())

```
>>> class C:
    @classmethod
    def f(cls,arg1):
        print(cls)
        print(arg1)

        
>>> C.f('类对象调用类方法')
<class '__main__.C'>
类对象调用类方法

>>> c = C()
>>> c.f('类实例对象调用类方法')
<class '__main__.C'>
类实例对象调用类方法
```

　　4. 类被继承后，子类也可以调用父类的类方法，但是第一个参数传入的是子类的类对象

```
>>> class D(C):
    pass

>>> D.f("子类的类对象调用父类的类方法")
<class '__main__.D'>
子类的类对象调用父类的类方法
```












