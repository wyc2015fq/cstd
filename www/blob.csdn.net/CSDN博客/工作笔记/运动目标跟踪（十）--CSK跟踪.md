# 运动目标跟踪（十）--CSK跟踪 - 工作笔记 - CSDN博客





2016年08月17日 11:54:57[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8103
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：

[**http://blog.csdn.net/ikerpeng/article/details/44171159**](http://blog.csdn.net/ikerpeng/article/details/44171159)

首先，这篇文章使用的决策函数是一个结构风险最小化的函数：

![](https://img-blog.csdn.net/20141016105145979)


这个函数中：前面是一个损失函数，损失函数里面的f（x）就是最后要求的判别函数；后面是一个结构化的惩罚因子。对于SVM分类器来讲就是合页损失函数（Hinge loss）。但是实际上，采用核函数的最小二乘法（Regularized Least Squares（RLS） with Kernels）也可以同样的达到这样的效果。于是文章就采用了这样的方法来求解这个Function。得到的结果是：

![](https://img-blog.csdn.net/20141017095455769)

也就是说，实际这里是RLSC的结果，推导看：

http://blog.csdn.net/app_12062011/article/details/52229530




![](https://img-blog.csdn.net/20150310090410612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWtlcnBlbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](https://img-blog.csdn.net/20150310090309586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWtlcnBlbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20150310090343953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWtlcnBlbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](https://img-blog.csdn.net/20150310090532747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWtlcnBlbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](https://img-blog.csdn.net/20150310090407025?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWtlcnBlbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







![](https://img-blog.csdn.net/20150310091431666?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWtlcnBlbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







