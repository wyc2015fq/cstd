# Python浅复制中对象的生存周期 - 小灰笔记 - CSDN博客





2017年08月27日 22:35:06[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：192








       Python的浅复制以及深复制我了解的并不多，我不清楚shutil中的copy1和copy2是不是算是浅复制和深复制？这两个操作，一个是文件属性重建，还有一个是文件属性不变。不知道对象的复制是否可以以此类比？

       在复制之前，为了能够观看一下GC的执行，定义一个特殊的类和对象。测试代码如下：

class Demo:

       def __del__(self):

              print("Removing{0}".format(id(self)))



x = Demo()

del x

       程序执行结果如下：

E:\01_workspace\02_programme_language\03_python\03_OOP\2017\08>pythoncopy1.py

Removing1772226785520

       接下来构造浅复制的例子：

>>>obj_list1 = [Demo(),Demo()]

>>>obj_list2 = obj_list1[:]

>>> delobj_list1

>>> delobj_list2

Removing1902510644416

Removing1902510644360

       从上面的结果看，第一次del并没有触发对象销毁的方法执行。这是在交互界面中进行调试出现的结果，如果代码是写到一个具体的文件中，那么程序执行的结果有可能不同。当所有的语句都写到同一个文件中的时候，文件中的对象作用域会有轻微的调整。这是值得我们注意的。

       另一种常见的浅复制方法：

>>> o1 =o2 = Demo()

>>> delo1

>>> delo2

Removing1902510136568



