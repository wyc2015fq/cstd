# 使用matplotlib绘制散点图 - 小灰笔记 - CSDN博客





2017年04月25日 00:42:49[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2593








使用matplotlib不仅能够绘制出曲线图，还能够绘制出散点图。从数据的分布上，两者的趋势应该是相同的，只是呈现的方式不同。从含以上来说，散点图应该是用于数据统计而曲线则应该是用于连续数据的采集描述。

简单写代码如下：

  1 import matplotlib.pyplot
as plt

  2 import numpy
as np

  3 from numpy.random
import randn

  4 

  5 x = range(1000)

  6 y = randn(1000)

  7 

  8 fig = plt.figure()

  9 ax1 = fig.add_subplot(2,1,1)

 10 ax2 = fig.add_subplot(2,1,2)

 11 ax1.plot(x,y,'k--')

 12 ax2.scatter(x,y)

 13 plt.show()




程序的执行结果如下：

![](https://img-blog.csdn.net/20170425004131240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




虽然只是一个简单的示范程序，但是这个功能还是停让人惊喜的。虽说这仅仅是一个随机数的绘制，但是图像或者曲线看着很漂亮，这让我觉得蛮新鲜的。




