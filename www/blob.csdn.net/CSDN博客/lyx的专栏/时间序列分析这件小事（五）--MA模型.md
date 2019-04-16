# 时间序列分析这件小事（五）--MA模型 - lyx的专栏 - CSDN博客





2016年12月04日 20:50:05[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：10113标签：[时间序列分析																[R](https://so.csdn.net/so/search/s.do?q=R&t=blog)](https://so.csdn.net/so/search/s.do?q=时间序列分析&t=blog)
个人分类：[R语言与时间序列分析](https://blog.csdn.net/qtlyx/article/category/6551781)

所属专栏：[R语言的时间序列分析](https://blog.csdn.net/column/details/15558.html)








1.MA

之前讲了AR模型，与之对应的是MA模型，也就是移动平均模型。与AR模型类似，只不过，之前是由不同阶滞后的序列拟合出yt，而现在是不同阶滞后的白噪音拟合。说白了，就是我们认为yt是白噪音的线性加权。同样的，我们利用R语言自带的函数来实现MA的学习。



```
#example 7
y3 = arima.sim(n=100,list(ma = 0.8))
plot(y3,type = 'o')
```
![](https://img-blog.csdn.net/20161203194957397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




同样的，acf与模型辨识函数都可以使用。

![](https://img-blog.csdn.net/20161204204906846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20161204204923502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2.ARMA

ARMA。看名字就知道，把AR和MA模型合起来就是了。



```
set.seed(12345)
y4 = arima.sim(n=100,list(ar = 0.8,ma = 0.6))
plot(y4,type = 'o')
```
套路还是一样的，大家自己看一下图。ARMA的模型辨识，通常我们认为，acf在q步后截尾，则MA是q阶的；pacf在p阶截尾，则AR是p阶的。但是，往往会不太准确，只能说是一个参考。例如下面





```
acf(y4)#acf在q步后截尾，那么可以认为是MA（q）
pacf(y4)#pacf在p阶截尾，那么可以认为是AR（p）
```
![](https://img-blog.csdn.net/20161204205520802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](https://img-blog.csdn.net/20161204205526942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

我们发现，现实如此残酷，和我们生成的完全不同。

R中forecast包中有一个auto函数，可以给出自动的模型参数辨识。


`auto.arima(y4)`![](https://img-blog.csdn.net/20161204205712224?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




这个方法还是很靠谱的吧。当然了，别忘了先加载forecast包。









