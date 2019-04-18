# Python实现接受任意个数参数的函数 - 小灰笔记 - CSDN博客





2017年08月10日 20:02:18[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3755
个人分类：[Python																[OOP](https://blog.csdn.net/grey_csdn/article/category/6926551)](https://blog.csdn.net/grey_csdn/article/category/6639459)








       这个功能倒也不是我多么急需的功能，只是恰好看到了，觉得或许以后会用的到。功能就是实现函数能够接受不同数目的参数。

       其实，在C语言中这个功能是熟悉的，虽说实现的形式不太一样。C语言中的main函数是可以实现类似的功能的，可以通过这种方式实现一个支持命令行参数的程序。

       先写一段python实现相应功能的示范代码：

defFuncDemo(*par):

       print("number of pars: %d" %len(par))

       print("type of par: %s" %type(par))

       i = 0

       if len(par) != 0:

              for p in par:

                     i = i + 1

                     print("%d par is:%s" %(i,p))

       加载后运行测试交互记录：

>>>FuncDemo()

number of pars: 0

type of par:<class 'tuple'>

>>>FuncDemo(1,2,3)

number of pars: 3

type of par:<class 'tuple'>

1 par is: 1

2 par is: 2

3 par is: 3

>>>FuncDemo(1,2,3,'abc')

number of pars: 4

type of par:<class 'tuple'>

1 par is: 1

2 par is: 2

3 par is: 3

4 par is: abc

       这基本上就是Python实现接受任意参数函数的方法以及应用，接下来小结一下相应的知识。

       实现Python接受任意个数参数的函数，在形式上比较简单。就是在参数前面加上一个星号，这样相应的参数位置就能够接受任意个参数。相应的参数在函数中是一个元组，从上面交互的结果也能够看得出。

       其实，这个功能还能能够支持字典的传入。如果是字典的传入，那么就需要传入成对儿的参数。

       暂时看来，这个功能在我的工作以及生活中用处并不是很大，作为一个后备的功能再观其效吧！




