# Python中使用property实现类的特性 - 小灰笔记 - CSDN博客





2017年08月21日 00:52:56[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：189








       Python中的特性是一个函数，但是在使用的形式上看起来更像是一个属性。针对一个对象来说，与属性相比，特性是不能够随意添加的。而对象的属性，默认情况下添加是十分简单的。

       下面通过代码展示如何使用property实现特性：

class MyClass:

       def __init__(self,val1,val2):

              self.val1 = val1

              self.val2 = val2

       @property

       def Sum(self):

              return self.val1 + self.val2



obj =MyClass(34,45)

print(obj.Sum)

       程序的执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python property.py

79

       从上面的结果来看，特性在调用的时候跟属性是差不多的。如果通过类似方法的调用则会报错，下面修改代码如下：

class MyClass:

       def __init__(self,val1,val2):

              self.val1 = val1

              self.val2 = val2

       @property

       def Sum(self):

              return self.val1 + self.val2



obj =MyClass(34,45)

print(obj.Sum)

# wrong way tocall the Sum property

print(obj.Sum())

       程序执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python property.py

79

Traceback (mostrecent call last):

  File "property.py", line 12, in<module>

    print(obj.Sum())

TypeError: 'int'object is not callable



