# Python通过属性手段实现只允许调用一次的方法 - 小灰笔记 - CSDN博客





2017年08月20日 22:27:52[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3051








       如果希望一个对象的某个方法只能够调用一次，按照我之前的惯性思维，我肯定是定义一个状态量然后每次调用的时候修改它的值。通过查看状态量的数值，我可以决定采取执行不同的处理。其实，除此之外还有一种方法，不仅仅能够实现这样的处理，还能够顺便处理对象的属性。

       先看一下如下的代码：

class DemoClass:

       def __init__(self):

              pass

       def AttrCheck(self):

              try:

                     self.value

                     print("already hasvalue")

                     raise ValueAttrError

              except AttributeError:

                     self.value = 0

                     print(self.value)



obj = DemoClass()

obj.AttrCheck()

obj.AttrCheck()

       程序执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python attr1.py

0

already has value

Traceback (mostrecent call last):

  File "attr1.py", line 15, in<module>

    obj.AttrCheck()

  File "attr1.py", line 8, inAttrCheck

    raiseRuntimeError("multi-excued!")

RuntimeError:multi-excued!       从上面的结果看，我们所描述到的功能已经这样实现了！

       上面的属性是给了默认的赋值，我们当然也可以改成带有赋值数值的形式：

class DemoClass:

       def __init__(self):

              pass

       def AttrCheck(self,value):

              try:

                     self.value

                     print("already hasvalue")

                     raiseRuntimeError("multi-excued!")

              except AttributeError:

                     self.value = value

                     print(self.value)



obj = DemoClass()

obj.AttrCheck(123)

obj.AttrCheck(123)



       程序的执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python attr1.py

123

already has value

Traceback (mostrecent call last):

  File "attr1.py", line 15, in<module>

    obj.AttrCheck(123)

  File "attr1.py", line 8, in AttrCheck

    raiseRuntimeError("multi-excued!")

RuntimeError:multi-excued!



