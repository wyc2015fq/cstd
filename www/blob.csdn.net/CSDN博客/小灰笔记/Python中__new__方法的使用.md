# Python中__new__方法的使用 - 小灰笔记 - CSDN博客





2017年08月18日 02:23:10[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1590








       先看一段代码：

classdoubleFloat(float):

       def __new__(cls,arg = .6):

              return float.__new__(cls,arg * 2)



a = doubleFloat()

print(a)



b =doubleFloat(1.9)

print(b)

       程序执行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08$python new_demo.py

1.2

3.8

       通过上面的例子，结合学习的东西做一下总结：

在Python中__new__方法与__init__方法类似，但是如果两个都存在那么__new__闲执行。

       在基础类object中，__new__被定义成了一个静态方法，并且需要传递一个参数cls。Cls表示需要实例化的类，此参数在实例化时由Python解析器自动提供。

       接下来总结一下__new__与__init__的异同点：

       1，两个功能相似，但是如果都存在__new__先执行；

       2，__new__方法必须要返回一个实例化的对象；

       3，__init__方法没有返回值；

       4，__new__有一个参数cls，__init__有一个参数self即为__new__返回的实例对象。



