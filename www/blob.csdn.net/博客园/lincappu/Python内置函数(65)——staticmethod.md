# Python内置函数(65)——staticmethod - lincappu - 博客园







# [Python内置函数(65)——staticmethod](https://www.cnblogs.com/lincappu/p/8145385.html)





**英文文档：**

`staticmethod`(*function*)

Return a static method for *function*.

A static method does not receive an implicit first argument.

The `@staticmethod` form is a function decorator – see the description of function definitions in Function definitions for details.

It can be called either on the class (such as `C.f()`) or on an instance (such as `C().f()`). The instance is ignored except for its class.

 　　标示方法为静态方法的装饰器

**说明：**

　　1. 类中普通的方法，实际上既可以被类直接调用也可以被类的实例对象调用，但是被实例对象调用的时候，要求方法至少有一个参数，而且调用时会将实例对象本身传给第一个参数

```
>>> class Student(object):
    def __init__(self,name):
        self.name = name
    def sayHello(lang):
        print(lang)
        if lang == 'en':
            print('Welcome!')
        else:
            print('你好！')
  
    
>>> Student.sayHello
<function Student.sayHello at 0x02AC7810>
>>> a = Student('Bob')
>>> a.sayHello
<bound method Student.sayHello of <__main__.Student object at 0x02AD03F0>>


>>> Student.sayHello('en') # 类调用的时候，将'en'传给了lang参数
en
Welcome!

>>> a.sayHello() # 类实例对象调用的时候，将对象本身自动传给了lang参数，不能再接收参数
<__main__.Student object at 0x02AD03F0>
你好！



  >>> a.sayHello('en')
  Traceback (most recent call last):
  File "<pyshell#7>", line 1, in <module>
  a.sayHello('en')
  TypeError: sayHello() takes 1 positional argument but 2 were given
```

　　2. staticmethod函数功能就是将一个方法定义成类的静态方法，正确的方法是使用 `@staticmethod装饰器，这样在实例对象调用的时候，不会把实例对象本身传入静态方法的第一个参数了。`

```
# 使用装饰器定义静态方法
>>> class Student(object):
    def __init__(self,name):
        self.name = name
    @staticmethod
    def sayHello(lang):
        print(lang)
        if lang == 'en':
            print('Welcome!')
        else:
            print('你好！')

            
>>> Student.sayHello('en') #类调用,'en'传给了lang参数
en
Welcome!

>>> b = Student('Kim') #类实例对象调用,不再将类实例对象传入静态方法
>>> b.sayHello()
Traceback (most recent call last):
  File "<pyshell#71>", line 1, in <module>
    b.sayHello()
TypeError: sayHello() missing 1 required positional argument: 'lang'

>>> b.sayHello('zh')  #类实例对象调用,'zh'传给了lang参数
zh
你好！
```












