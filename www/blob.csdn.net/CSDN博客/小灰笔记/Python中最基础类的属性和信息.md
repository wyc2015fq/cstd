# Python中最基础类的属性和信息 - 小灰笔记 - CSDN博客





2017年08月18日 22:30:09[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：279








       查看Python中基础类的属性和信息可以看一下Python面向对象编程所接触到的最基本的东西。今天我想探索的是两个不同的大版本之间差异，也就是Python2和Python3之间的差异。

       通常，我查看一个类或者对象的属性的时候我会使用dir。使用dir函数可以查看到类或者对象的大多数属性或者方法，但是也不一定全面，有一部分特殊的属性或者方法显示不出来。如果想要显示的完整一些，可以直接查看__dict__属性。

       示范代码如下：

class baseClass:

       pass



print(dir(baseClass))

print(baseClass.__dict__)

print(baseClass.__bases__)

       程序在两个版本的Python中执行结果分别如下（Linux中是Python2，Windows中是Python3）：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python base_class.py

['__doc__','__module__']

{'__module__':'__main__', '__doc__': None}

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python base_class.py

['__doc__','__module__']

{'__module__':'__main__', '__doc__': None}

()

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$exit

exit



E:\01_workspace\02_programme_language\03_python\03_OOP\2017\08>pythonbase_class.py

['__class__','__delattr__', '__dict__', '__dir__', '__doc__', '__eq__', '__format__','__ge__', '__getattribute__', '__gt__', '__hash__', '__init__','__init_subclass__', '__le__', '__lt__', '__module__', '__ne__', '__new__','__reduce__', '__reduce_ex__',
 '__repr__', '__setattr__', '__sizeof__','__str__', '__subclasshook__', '__weakref__']

{'__module__':'__main__', '__dict__': <attribute '__dict__' of 'baseClass' objects>,'__weakref__': <attribute '__weakref__' of 'baseClass' objects>,'__doc__': None}

(<class'object'>,)



     从上面的结果看得出，两个查看属性或者方法的手段再执行效果上的差异。也看得出Python2和Python3在默认继承上的差异。如果不进行显式的继承指定，在Python2中找不到类的父类。但是Python3中则不一样，在Python3中默认继承基础的类object。从上面的结果我们也能够看得到Python3中基础类所具有的最基础的属性和方法。



