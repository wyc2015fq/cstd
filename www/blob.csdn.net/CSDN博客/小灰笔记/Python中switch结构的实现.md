# Python中switch结构的实现 - 小灰笔记 - CSDN博客





2017年06月05日 23:38:31[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1262








              Python中switch条件分支结构的实现一般是通过字典来实现，或许这个基本上是最初接触过Python的人也能够掌握的技巧。不过在更深入模拟C语言中的switch语句功能上，还是有一点其他的小技巧需要注意一下。

       简单实现一个switch的分支结构，示范代码如下：

#!/usr/bin/python



def Func1():

       print("function 1")



def Func2():

       print("function 2")



def Func3():

       print("function 3")



dict_data ={1:Func1,2:Func2,3:Func3}



defSwitchDemo(num):

       dict_data[num]()



for i inrange(1,4):

       SwitchDemo(i)

       程序的运行结果如下：

E:\WorkSpace\01_编程语言\03_Python\OOP>python switch.py

function 1

function 2

function 3

       从上面看，基本上实现了一个switch语句的功能。而多重分支都走同样的逻辑时，只需要把多个分支的“标签”堆叠到同一个字典中即可。简单修改代码以示范，代码如下：

#!/usr/bin/python



def Func1():

       print("function 1")



def Func2():

       print("function 2")



def Func3():

       print("function 3")



dict_data ={1:Func1,2:Func2,3:Func3}





defSwitchDemo(num):

       dict_data[num]()

       if i < 3:

              {1:Func1,2:Func1}[num]()



for i inrange(1,4):

       SwitchDemo(i)

       程序的执行结果如下：

E:\WorkSpace\01_编程语言\03_Python\OOP>python switch.py

function 1

function 1

function 2

function 1

function 3



