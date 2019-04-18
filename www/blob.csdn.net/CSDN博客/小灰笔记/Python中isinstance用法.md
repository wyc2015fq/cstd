# Python中isinstance用法 - 小灰笔记 - CSDN博客





2017年08月12日 14:31:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：17505








       Isinstance的用法是用来判断一个量是否是相应的类型，接受的参数一个是对象加一种类型。示范代码如下：

a = 1

print(isinstance(a,int))

print(isinstance(a,float))

       程序的执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python instance.py

True

False

       从上面的结果看，对于基础的数据类型实际上是能够很好地进行判断的。

       其实，说的更为通用一些，isinstance是用于判断一个对象是否是一个类的实例的函数。为了说明其通用性，接下来再进行一个类以及对象的判断。写代码如下：

class Demo:

       pass



instDemo = Demo()

print(isinstance(instDemo,Demo))

       程序执行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python instance.py

True

       从上面的结果可以看出，程序执行按照预期进行。



