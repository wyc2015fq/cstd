# Python中partial的使用 - 小灰笔记 - CSDN博客





2017年06月28日 08:13:45[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：713








Parrtial的功能是可以提前向函数传递一部分参数，传递的原则是按照顺序传入。举个简单的例子，假如有以下代码：

from functoolsimport partial



def Demo1(a,b):

       return a + b



Demo1_p =partial(Demo1,5)

r_Demo1 =Demo1_p(7)

print(r_Demo1)

       执行结果如下：

E:\WorkSpace\01_编程语言\03_Python\OOP>python partial_t.py

12

       实际上，通过partial的功能实现了函数中参数5的预先传递。同样的功能也能够用于更多参数的函数，假如有以下代码：

from functoolsimport partial



def Demo2(a,b,c):

       return a + b + c



Demo2_p =partial(Demo2,7)

print(Demo2_p(3,8))

Demo2_p_p =partial(Demo2_p,3)

r_Demo2 =Demo2_p_p(8)

print(r_Demo2)

       执行的结果如下：

E:\WorkSpace\01_编程语言\03_Python\OOP>python partial_t.py

18

18

       从上面的演示可以看出，使用partial不仅能够实现一个参数的预先传递，也能够分次实现过个函数参数的传递。不管在哪个阶段，都能够预先应用已经传递了的参数。上述代码再做一下修改：

from functoolsimport partial



def Demo2(a,b,c):

       return a + b + c



Demo2_p =partial(Demo2,7)

print(Demo2_p(3,8))

Demo2_p_p =partial(Demo2_p,3)

r_Demo2 =Demo2_p_p(8)

print(r_Demo2)

print(Demo2_p(3,8))

       执行结果如下：

E:\WorkSpace\01_编程语言\03_Python\OOP>python partial_t.py

18

18

18

       从上面的结果可以看出，实际上预先传输参数的行为没有被后面的再次传入覆盖。从这一点来看，这种操作的行为结果类似于产生了新的函数。



