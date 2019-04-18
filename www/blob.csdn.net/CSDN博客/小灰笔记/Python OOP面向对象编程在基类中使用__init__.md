# Python OOP面向对象编程在基类中使用__init__ - 小灰笔记 - CSDN博客





2017年05月24日 20:33:14[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：281
个人分类：[Python																[OOP](https://blog.csdn.net/grey_csdn/article/category/6926551)](https://blog.csdn.net/grey_csdn/article/category/6639459)








      __init__()方法在每次对象创建的时候就会调用，这个方法因此比较适合对一些对象的内部变量或者其他一些一次性过程进行初始化。

      构造如下类：

#!/usr/bin/python



class MyBase:

      def__init__(self,par1,par2):

            self.par1= par1

            self.par2= par2

            self.sum= _sum()



class ClassSum(MyBase):

      def_sum(self):

            returnself.par1 + self.par2



class Class10Sum(MyBase):

      def_sum(self):

            return10 * (self.par1 + self.par2)

      利用上面的类构造对象并进行简单的属性调用：

sum1 = ClassSum(15,20)

print("sum of sum1: %d" % sum1.sum)

sum2 = Class10Sum(15,20)

print("sum of sum2: %d" % sum2.sum)

      从上面的运行结果可以得出以下结论：

            1，创建对象时，传入的参数传给__init__()

            2，基类中的方法可以在派生类中进行实现

            3，__init__()方法在对象创建的过程中被调用了，而且在这个方法中可以调用其他的方法。




