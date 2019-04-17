# 使用matplotlib绘制图像并设置标题以及坐标轴等信息 - 小灰笔记 - CSDN博客





2017年04月25日 01:14:07[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：21744








进行图像绘制有时候需要设定坐标轴以及图像标题等信息，示例代码如下：

  1 #!/usr/bin/python

  2 

  3 import matplotlib.pyplot
as plt

  4 from numpy.random
import randn

  5 

  6 x = range(100)

  7 y = randn(100)

  8 

  9 fig = plt.figure()

 10 ax = fig.add_subplot(1,1,1)

 11 ax.plot(x,y,'k--')

 12 ax.set_xticks([0,25,50,75,100])

 13 ax.set_xticklabels(['one','two','three','four','five'],rotation=45,fontsize='small')

 14 ax.set_title('Demo Figure')

 15 ax.set_xlabel('Time')

 16 plt.show()

 17




程序的执行结果如下：

![](https://img-blog.csdn.net/20170425011249548?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




总结如下：

1，12行实现设置坐标标记点；

2，13行，在设置的标记点写标记词；

3，13行其他的参数分别是设置字体的倾斜度以及字体的大小；

4，14行设置图片的标题；

5，15行设置横轴的名称。




