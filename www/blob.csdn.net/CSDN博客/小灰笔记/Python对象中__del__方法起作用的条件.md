# Python对象中__del__方法起作用的条件 - 小灰笔记 - CSDN博客





2017年08月27日 21:07:11[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2167








       对象的__del__是对象在被gc消除回收的时候起作用的一个方法，它的执行一般也就意味着对象不能够继续引用。

       示范代码如下：

class Demo:

       def __del__(self):

              print("calling __del__")



obj = Demo()

del obj

       程序执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python del_method.py

calling __del__

       但是，这并不是让__del__执行的唯一方式。其实，这个方法也可以直接调用。测试代码如下：

class Demo:

       def __init__(self):

              print("calling __del__")



obj = Demo()

obj.__del__()

       程序执行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python del_method.py

calling __del__       

       但是，这样的执行很多时候并不能够保证垃圾回收的正常执行。如下代码：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python del_method.py

calling __del__

calling __del__

       执行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python del_method.py

calling __del__

calling __del__

       推测：上面的删除方法触发了两次删除，但是由于引用关系，删除销毁其实没有实现。修改代码验证如下：

class Demo:

       def __del__(self):

              print("calling __del__")

              del self



obj = Demo()

obj.__del__()

print(id(obj))



执行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python del_method.py

calling __del__

140726800222040

calling __del__

       从上面看来，其实主要还有对对象的引用，这个销毁的动作还是需要等待对象引用没有了以后才能够完成。进一步验证代码：

class Demo:

       def __del__(self):

              print("calling __del__")

              del self



obj = Demo()

obj.__del__()

print(id(obj))

print(id(obj))

       执行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python del_method.py

calling __del__

140568015406936

140568015406936

calling __del__

       从上面结果看，猜测还是准确的。



