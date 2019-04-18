# 使用pandas中的DataFrame数据绘制柱状图 - 小灰笔记 - CSDN博客





2017年04月26日 00:20:48[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：15369








折线图是数据分析的一种手段，但是有时候我们也需要柱状图进行不同数据的可视化量化对比。使用pandas的DataFrame方法进行柱状图的绘制也是比较方便的。

把之前的折线图绘制代码修改一下如下：

  1 from pandas
import Series,DataFrame

  2 from numpy.random
import randn

  3 import numpy
as np

  4 import matplotlib.pyplot
as plt

  5 

  6 df = DataFrame(abs(randn(10,5)),columns=['A','B','C','D','E'],index
 = np.arange(0,100,10))

  7 df.plot(kind='bar')

  8 plt.show()

  9 




程序执行结果如下：

![](https://img-blog.csdn.net/20170426002014300?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




这种绘制方式确实是让人很欣喜，我觉得在一定程度上比Excel的数据处理要便捷很多。对于我这种对Excel一点都不熟悉的我来说，这种方法已经好多了。




