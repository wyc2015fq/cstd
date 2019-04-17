# B样条曲线 - 工作笔记 - CSDN博客





2015年11月25日 09:38:19[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10705
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









**相关内容可以网上了解，这里只记录核心定义。**

**B样条曲线的定义**

       给定n+1个控制点Pi(i=0,1,2,...,n)的坐标Pi，n次B样条曲线段的参数表达式为

![](https://img-my.csdn.net/uploads/201206/30/1341009706_2340.png)

式中为n次B样条基函数，其形式为

![](https://img-my.csdn.net/uploads/201206/30/1341009770_7461.png)



其中

![](https://img-my.csdn.net/uploads/201206/30/1341009852_1043.png)

取n=3,则3次B样条基为：


![](https://img-blog.csdn.net/20151125094802044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中G函数与F函数一样，则P(t)第0个点的值，写成矩阵形式为：

![](https://img-blog.csdn.net/20151125095014318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




再给个三次样条插值的理解内容

[http://www.cnblogs.com/xpvincent/archive/2013/01/26/2878092.html](http://www.cnblogs.com/xpvincent/archive/2013/01/26/2878092.html)




