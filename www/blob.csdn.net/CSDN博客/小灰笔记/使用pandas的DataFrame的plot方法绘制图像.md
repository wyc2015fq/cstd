# 使用pandas的DataFrame的plot方法绘制图像 - 小灰笔记 - CSDN博客





2017年04月26日 00:03:12[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：13416
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









使用了pandas的Series方法绘制图像体验之后感觉直接用matplotlib的功能好用了不少，又试用了DataFrame的方法之后发现这个更加人性化。

写代码如下：

  1 from pandas
import Series,DataFrame

  2 from numpy.random
import randn

  3 import numpy
as np

  4 import matplotlib.pyplot
as plt

  5 

  6 df = DataFrame(randn(10,5),columns=['A','B','C','D','E'],index
 = np.arange(0,100,10))

  7 df.plot()

  8 plt.show()

  9 

 10 




程序运行结果如下：

![](https://img-blog.csdn.net/20170426000223164?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




使用DataFrame的plot方法绘制图像会按照数据的每一列绘制一条曲线，参数中的columns就是列的名称而index本来是DataFrame的行名称。图形绘制成功之后还会按照列的名称绘制图例，这个功能确实是比较赞的。如果使用matplotlib的基本绘制功能，图例的添加还需要自己额外处理。看来，数据的规整化不仅仅是为了向量化以及计算加速做准备，而且为数据的可视化提供了不少便捷的方法。




