# Python abc模块的几个小知识点 - 小灰笔记 - CSDN博客





2017年08月12日 10:31:28[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：6644








       编程的思想应该在不断地实践中才能够体悟，这是我自己的感受。接触了好长一段时间的OOP，但是一直在实战中用到的很少。或许，平时我需要的仅仅是一些小脚本而已。今天总结一下学习Python OOP遇到的一个小模块，abc。

       首先需要了解的是一个基类，abc.ABCMeta。最初看到与之相关的代码，我还把这个理解为了一个参数。这让我在读代码的时候有点不理解。查了一下文档，恍然大悟。原来，这个是用于实现抽象类的一个基础类。

       接下来，需要总结的是关于抽象方法的用法。在相应的方法之前一行加上@abstractmethod之后，从新的一行开始定义相应的方法。实现的方法就是一个抽象的方法，有点像Java中的接口。子类继承之后，如果需要用到的这个方法则必须用新的方法将其实现。关于这个，写一段示范代码：

import abc

from abc import *



classSClass(metaclass = abc.ABCMeta):

       @abstractmethod

       def my_print(self):

              pass



classCClass(SClass):

       pass



cc = CClass()  

程序执行结果：

E:\01_workspace\02_programme_language\03_python\OOP\2017\08\12>pythonabstractmethod.py

Traceback (mostrecent call last):

  File "abstractmethod.py", line 12,in <module>

    cc = CClass()

TypeError: Can'tinstantiate abstract class CClass with abstract methods my_print

       这样，如果想要能够正确进行实例化就需要对相应的抽象接口进行实现。修改代码如下：

import abc

from abc import *



classSClass(metaclass = abc.ABCMeta):

       @abstractmethod

       def my_print(self):

              pass



classCClass(SClass):

       def my_print(self):

              print("my childmethod!")



cc = CClass()

       程序的执行结果如下：

E:\01_workspace\02_programme_language\03_python\OOP\2017\08\12>pythonabstractmethod.py



E:\01_workspace\02_programme_language\03_python\OOP\2017\08\12>

       从上面的结果至少可以看出，这个类是可以实现实例化的。继续修改代码：

import abc

from abc import *



classSClass(metaclass = abc.ABCMeta):

       @abstractmethod

       def my_print(self):

              pass



classCClass(SClass):

       def my_print(self):

              print("my childmethod!")



cc = CClass()

cc.my_print()

       程序的执行结果：

E:\01_workspace\02_programme_language\03_python\OOP\2017\08\12>pythonabstractmethod.py

my child method!

       从上面的结果就能够看出，程序运行已经正常。



