# Python中的静态函数的用法 - 小灰笔记 - CSDN博客





2017年08月12日 19:16:08[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：403








       Python中也存在静态函数，查看文档之后，文档解释这个功能跟Java或者C++的功能相同的。

       总结Python中的静态函数，先看一段自己写的简单的代码：

class MyClass:

       @staticmethod

       def Func():

              print("static function inpython")



class MyClass1:

       def Func(self):

              print("non-static function inpython")



obj1 = MyClass()

obj2 = MyClass1()



obj1.Func()

obj2.Func()



MyClass.Func()

MyClass1.Func()

       上面的代码中定义了两个类，定义了两个相应类的实例化对象。两个类中的方法有一个是静态方法，另一个则是一般的方法。分别通过实例化对象和类对齐方法进行调用，程序的运行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python staticmethod.py

static function inpython

non-staticfunction in python

static function inpython

Traceback (mostrecent call last):

  File "staticmethod.py", line 17, in<module>

    MyClass1.Func()

TypeError: unboundmethod Func() must be called with MyClass1 instance as first argument (gotnothing instead)

       从上面得出结论：

       1，静态方法可以不带self参数

       2，静态方法可以通过类进行调用，但是一般方法不行

       一个更好地示范代码如下：

class MyClass:

       @staticmethod

       def F1():

              print("Functiom 1")

       def F2(self):

              MyClass.F1()

       def F3(self):

              MyClass.F2()



obj = MyClass()

obj.F1()

obj.F2()

obj.F3()

       程序的执行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python s1.py

Functiom 1

Functiom 1

Traceback (most recentcall last):

  File "s1.py", line 15, in<module>

    obj.F3()

  File "s1.py", line 10, in F3

    MyClass.F2()

TypeError: unboundmethod F2() must be called with MyClass instance as first argument (got nothinginstead)

       从上面看，静态方法比较便于类内部的调用，他与类的对象无关，仅仅与类有关。如果想要上面的代码执行通过，修改如下：

class MyClass:

       @staticmethod

       def F1():

              print("Functiom 1")

       def F2(self):

              MyClass.F1()

       def F3(self):

              self.F2()



obj = MyClass()

obj.F1()

obj.F2()

obj.F3()

       程序执行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python s1.py

Functiom 1

Functiom 1

Functiom 1



