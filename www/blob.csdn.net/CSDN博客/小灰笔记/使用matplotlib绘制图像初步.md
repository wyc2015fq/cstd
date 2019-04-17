# 使用matplotlib绘制图像初步 - 小灰笔记 - CSDN博客





2017年04月24日 23:52:42[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：477








绘制图形曲线，我想到的工具一般是MATLAB。不过，鉴于软件的昂贵价格个人折腾显然是买不起的。不过现在开源软件甚至是自由软件给了我们很多选择，python则为很多便捷的功能实现提供了基础。这样，图形绘制时matplotlib便成了我的首选。

在此之前，我确实是使用这工具绘制过图像，但是之前并没有好好研究总结过。一般是需要的时候直接去官网看一下相册中的图片集，选择跟我自己所需相似的图片直接拷贝代码进行修改。但是，为了能够用的顺手，学习总结一下还是很有必要的。先从最基本的功能开始。

  1 import matplotlib.pyplot
as plt

  2 from numpy.random
import randn

  3 

  4 fig = plt.figure()

  5 ax1 = fig.add_subplot(1,1,1)

  6 ax1.plot(range(100),randn(100),'k--')

  7 plt.show()




程序执行后，会生成一个图形窗口，具体的窗口如下：

![](https://img-blog.csdn.net/20170424235038762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







如果需要，可以点击上面界面中的保存按钮把图片保存到自己的磁盘。


![](https://img-blog.csdn.net/20170424235114773?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







从图像可以看出，这种图像的风格跟MATLAB十分相似。其实，这也是matlibplot的设计目的。从上面的代码中，大致总结一下：

1，第4行代码是创建figure对象，但是此时的图像板并不能直接进行图形绘制。必须通过add_subplot方法添加子图像。而它接受的前两个参数是图像子图像的行列数目，而第三个参数则是选定子图像的序号。

2，而plot的方法接受的三个参数中，前两个参数分别是图像的横纵坐标，第三个参数则是曲线的绘制线型。

3，在绘制结束之后通过show方法的调用才会实现图像的显示。

根据上面的总结，绘制一个3行1列的图像，修改代码如下：

  1 import matplotlib.pyplot
as plt

  2 from numpy.random
import randn

  3 

  4 fig = plt.figure()

  5 ax1 = fig.add_subplot(3,1,1)

  6 ax2 = fig.add_subplot(3,1,2)

  7 ax3 = fig.add_subplot(3,1,3)

  8 ax1.plot(range(100),randn(100),'k--')

  9 ax2.plot(range(100),randn(100),'k--')

 10 ax3.plot(range(100),randn(100),'k--')

 11 plt.show()




代码的执行结果如下：
![](https://img-blog.csdn.net/20170424235150070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



