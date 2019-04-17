# Python中的@classmethod用法 - 小灰笔记 - CSDN博客





2017年08月19日 11:53:58[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：9406








       在Python面向对象编程中的类构建中，有时候会遇到@classmethod的用法。总感觉有这种特殊性说明的用法都是高级用法，在我这个层级的水平中一般是用不到的。不过还是好奇去查了一下。

       大致可以理解为：使用了@classmethod修饰的方法是类专属的，而且是可以通过类名进行调用的。为了能够展示其与一般方法的差异，写一段简单的代码如下：

class DemoClass:

       @classmethod

       def classPrint(self):

              print("class method")

       def objPrint(self):

              print("obj method")



obj = DemoClass()

obj.objPrint()

obj.classPrint()



DemoClass.classPrint()

DemoClass.objPrint()

       程序的执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python classmethod.py

obj method

class method

class method

Traceback (mostrecent call last):

  File "classmethod.py", line 13, in<module>

    DemoClass.objPrint()

TypeError: unboundmethod objPrint() must be called with DemoClass instance as first argument (gotnothing instead)

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$exit

exit



E:\01_workspace\02_programme_language\03_python\03_OOP\2017\08>pythonclassmethod.py

obj method

class method

class method

Traceback (mostrecent call last):

  File "classmethod.py", line 13, in<module>

    DemoClass.objPrint()

TypeError:objPrint() missing 1 required positional argument: 'self'

       上面的程序执行，我是在两个操作系统中的两个Python版本环境中进行的。不管是Py2还是Py3，这方面的设计都是差不多的。总体来说，这种用法还是很微妙的。由于没有足够的实战历练，暂时还说不好这个东西有什么更好的优势。



