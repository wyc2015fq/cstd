# Python中的字典与C语言中的switch结构类比 - 小灰笔记 - CSDN博客





2017年07月10日 23:39:31[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：574










       题目中用了类比而不是对比，因为我觉得我想要的并不是他们两者之间的差异，而是想在Python中找到一种方式能够完全代替C语言中的switch结构。

       之前使用字典功能模拟switch结构的时候总觉得有点缺憾，因为自己还要构造switch的default分支的模拟逻辑。

       其实，之前真是走了弯路了。在Python中，这种方式完全没有必要。字典中的get方法中可以使用默认的参数实现对switch语句的default分支模拟。这样，当想在Python中实现一段C语言中实现过的代码就简单多了。

       下面实现一个简单的switch结构的完整模拟：

#!/usr/bin/python



def Func1():

       print("function 1")



def Func2():

       print("function 2")



def Func3():

       print("function 3")



dict_data ={1:Func1,2:Func2}





defSwitchDemo(num):

       dict_data.get(num,Func3)()



for i inrange(1,10):

       SwitchDemo(i

       运行的结果如下：

E:\WorkSpace\01_programme_language\03_Python\OOP>pythonswitch.py

function 1

function 2

function 3

function 3

function 3

function 3

function 3

function 3

function 3

       从上面的运行结果可以看出，成功的通过前面叙述的方式实现了把Func3作为一个默认的分支。

       写代码久了觉得最初接触的编程语言教程内容的划分还是很合理的。最初接触的教程一般是从语法、类型、条件以及循环结构、数据结构等几个方面进行内容的安排。其实，我们用程序描述现实世界问题的时候用到的也不过就是这些，在加上合适的算法，整个程序的实现方式不管用什么语言都有一个相似的套路。这样，如果在一种语言平台上能够做到纯熟，那么其他的语言或许也能够触类旁通、一通百通。





