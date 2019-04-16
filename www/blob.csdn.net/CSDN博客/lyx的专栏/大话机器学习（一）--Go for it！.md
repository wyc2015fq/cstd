# 大话机器学习（一）--Go for it！ - lyx的专栏 - CSDN博客





2016年01月22日 11:25:26[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2061








本系列教程适合对机器学习一无所知，甚至文科森哦。反正不会有数学公式，I promise！教程基于sklearn的python机器学习库。

打开机器学习的面纱，就是这样。

首先要有利器。工欲善其事必先利其器。看我下面这个文章吧，下载一下这个软件，然后我们就可以开始了。

http://blog.csdn.net/qtlyx/article/details/49742483

### 1.什么是机器学习？

数据挖掘，深度学习，模式识别，大数据挖掘。一切听起来很深奥的样子。其实呢，就是一句话：分类和预测！说的那么好听就是在做这两件事。

### 2.线性回归

马上开始！有一种东西叫做最小二乘法，不知道也没有关系。

￼![](https://img-blog.csdn.net/20160122154826769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


话说有这样一张图，上面有这么多点点是已知的，现在问题来了，告诉你横轴的值是0.56的点，纵轴的值应该是多少？这就是预测喽。最最简单的方法就是最小二乘法，说白了就是找一条直线，能够很好的预测0.56的点对应的函数值。那怎么画出这个直线呢？接下来就是sklearn登场了。

http://scikit-learn.org/stable/


这个是sklearn的官网，都很详细的说明，不过当然不如我通俗啦~

Anaconda里面集成了这个包，直接import就可以了。



```python
import matplotlib.pyplot as plt#画图包
import numpy as np#sklearn的支持包
from sklearn import datasets, linear_model#导入sklearn中的数据集和线性模型

# 读取一组数据，好像是糖尿病的哦。这不是重点
diabetes = datasets.load_diabetes()
diabetes_X = diabetes.data[:, np.newaxis, 2]
diabetes_X = diabetes_X[:-20]
diabetes_y = diabetes.target[:-20]
# 重点来了，这里利用数据建立了一个线性回归的类
regr = linear_model.LinearRegression()
#把数据放进去，说好听点，就是让这个模型“学习”一下。数据就是我们上图的那些点。
regr.fit(diabetes_X, diabetes_y)
```

```python
regr.coef_#返回最小二乘法的那根直线的。分别是常数和一次项系数。
```


![](https://img-blog.csdn.net/20160122154843785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


把直线画出来的话的，就是这样的。然后就可以快乐的拿这条直线与预测了。

机器学习，其实就是这么简单。





