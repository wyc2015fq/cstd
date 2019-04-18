# Python中@符号的用法-1 - 小灰笔记 - CSDN博客





2017年08月11日 00:15:07[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：14428








       首先参考了几篇网络上的文章或者教程，看完之后也没有弄明白这个功能是什么意思。于是查了一下Python的文档，相关的描述如下：

A function definition may be wrapped by one or more [decorator]() expressions. Decorator expressions areevaluated when the function is defined, in the scope that contains the functiondefinition. The result must be a callable, which is invoked with the functionobject as the only
 argument. The returned value is bound to the function nameinstead of the function object. Multiple decorators are applied in nestedfashion. For example, the following code

@f1(arg)

@f2

def func(): pass

is roughly equivalent to

def func(): pass

func = f1(arg)(f2(func))

       其实，太多的东西也没必要去理解了。直接按照这个例子来理解一下就好了。这样，假如有以下代码：

def Func1(par):

       print("Func1")

       print(par)



def Func2(par):

       print("Func2")

       print(par)

@Func1

@Func2

def Func3():

       print("Func3")

       return 9

       相应的修饰部分等效结果应该如下：

Func3 =Func1(Func2(Func3))

       先执行这段代码看一下结果：

E:\01_workspace\02_programme_language\03_python\OOP\2017\08\10>pythondemo.py

Func2

<function Func3at 0x00000201330A3510>

Func1

None

       然后修改代码如下：

def Func1(par):

       print("Func1")

       print(par)



def Func2(par):

       print("Func2")

       print(par)

#@Func1

#@Func2

def Func3():

       print("Func3")

       return 9



Func3 =Func1(Func2(Func3))

程序运行结果：

E:\01_workspace\02_programme_language\03_python\OOP\2017\08\10>pythondemo.py

Func2

<function Func3at 0x000002240ABE3510>

Func1

None

       从上面的结果看，两个版本的代码执行效果是相同的。而修饰符号的作用其实是直接运行了代码中被修饰的函数，而且按照修饰的层级进行了参数传递。好奇调用了一下Fun3，出现一下提示：

Traceback (mostrecent call last):

  File "demo.py", line 20, in<module>

    Func3()

TypeError:'NoneType' object is not callable

       由此看来，是不是可以理解为被修饰的函数最终的函数体都等同于pass，而相应的函数还是一个不可被调用的函数。这样看来的话，这样的功能作用也不是很大。

       通过查找资料，这样的功能还能够用于修饰类，具体的用法以后再做一下小结。





