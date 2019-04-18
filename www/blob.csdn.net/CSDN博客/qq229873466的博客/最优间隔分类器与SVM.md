# 最优间隔分类器与SVM - qq229873466的博客 - CSDN博客

2015年11月24日 23:20:02[qq229873466](https://me.csdn.net/qq229873466)阅读数：655


最优间隔分类器问题可以表述为

![](https://img-blog.csdn.net/20151126002147799?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其中，ε为到平面的距离，w=[w1,w2...wn,b]' ，x=[x1,x2...xn,1]'
 ，S1和S2表示两类

上式等价于

![](https://img-blog.csdn.net/20151126003629551?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

令K=[w ε]'得

![](https://img-blog.csdn.net/20151126004040210?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20151126010148768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20151126010423749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

