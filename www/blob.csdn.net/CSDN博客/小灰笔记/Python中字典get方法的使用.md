# Python中字典get方法的使用 - 小灰笔记 - CSDN博客





2017年06月05日 23:49:14[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：8498








       说起来，这个功能是否需要学习还真是有待于讨论。其实，知道了字典这种数据结构以后，通过最基本的Python功能也能够实现一个类似的功能。不过，既然内置了这个功能那么我们就没有必要重复造车轮。

       在字典内置的方法中，想说的方法为get。这个方法是通过键来获取相应的值，但是如果相应的键不存在则返回None。其实，None只是一个默认的返回值但是并不是一个不能修改的返回值。其实，如果查询失败，我们可以指定一个返回值。

       上面提到的所有功能汇总在一个示范代码，具体如下：

#!/usr/bin/python



dict_data ={1:'one',2:'two',3:'three',4:'four'}



print(dict_data.get(1))

print(dict_data.get(3))

print(dict_data.get(5))

print(dict_data.get(5,'notfound'))



       程序的运行结果如下：

E:\WorkSpace\01_编程语言\03_Python\OOP>python dict_get.py

one

three

None

not found

       其实，不使用get方法，相应的功能实现也比较简单。只不过是在尝试得到字典的键值对信息的时候加上一个键是否存在于字典中的一个检测而已。



