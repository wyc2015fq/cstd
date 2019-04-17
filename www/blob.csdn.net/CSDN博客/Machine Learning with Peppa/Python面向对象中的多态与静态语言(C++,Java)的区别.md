# Python面向对象中的多态与静态语言(C++,Java)的区别 - Machine Learning with Peppa - CSDN博客





2018年03月31日 21:44:09[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：293
所属专栏：[C语言入门](https://blog.csdn.net/column/details/18792.html)









                动态语言的多态和静态语言c++等多态含义不太一样，c++中的多态就是参数数量和类型不同的方法就是不同方法，而动态语言中的多态其实值的是方法的寻找过程，即向右找到类（或者单件类），在类中找不到的话再找父类，一直在祖先链中找到或者找不到为止，先找到的就被调用：


class MyClass(object):

    def fn(x):

        print "This is the first fn"

    def fn(x,y):

        print "This is the second fn"

obj = MyClass()

obj.fn(1)

结果fn被复写，而没有实现多态



