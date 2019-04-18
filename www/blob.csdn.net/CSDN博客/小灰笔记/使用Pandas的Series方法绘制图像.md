# 使用Pandas的Series方法绘制图像 - 小灰笔记 - CSDN博客





2017年04月25日 23:23:52[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3653








通常绘制二维曲线的时候可以使用matplotlib，不过如果电脑上安装了pandas的话可以直接使用Series的绘图方法进行图像的绘制。pandas绘制图像其实也是给予matplotlib的绘图功能处理相应的数据，最终绘制出相应的曲线。在图形对象创建并操作之后还需要调用matplotlib的图像显示方法才能够最终显示出绘制的图像。

编写代码如下：

  1 import pandas
as pd

  2 from pandas
import Series,DataFrame

  3 import numpy
as py

  4 import matplotlib.pyplot
as plt

  5 from numpy.random
import randn

  6 

  7 s_data = Series(randn(100))

  8 s_data.plot()

  9 plt.show()

 10 




程序的执行结果如下：

![](https://img-blog.csdn.net/20170425232257876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




其实，整个程序的操作过程跟直接使用matplotlib的操作过程差不多。不过，在数据处理的时候数据很多时候是以Series的数据对象存在，这样在操作上便捷了不少。而且，从pandas的操作来看，pandas本身的操作功能还是要便捷一点的。




