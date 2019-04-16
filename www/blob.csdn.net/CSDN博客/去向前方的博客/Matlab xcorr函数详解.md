# Matlab xcorr函数详解 - 去向前方的博客 - CSDN博客





2018年09月30日 21:06:18[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：2209








Matlab提供了计算互相关和自相关的函数xcorr函数

1.使用方法

c = xcorr(x,y)

c = xcorr(x)

c = xcorr(x,y,‘option’)

c = xcorr(x,‘option’)

c = xcorr(x,y,maxlags)

c = xcorr(x,maxlags)

c =

xcorr(x,y,maxlags,‘option’)

c =

xcorr(x,maxlags,‘option’)

[c,lags] = xcorr(…)

其中option为：

"biased"为有偏的互相关函数估计；

"unbiased"为无偏的互相关函数估计；

"coeff"为0延时的正规化序列的自相关计算；

"none"为原始的互相关计算

2.特别的：

c=

xcorr(x,‘option’)特指以上某个选项的自相关估计。

c = xcorr(x,y,maxlags)

返回一个延迟范围在[-maxlags,maxlags]的互相关函数序列，输出c的程度为2*maxlags+1.c = xcorr(x,maxlags)返回一个延迟范围在[-maxlags,maxlags]的自相关函数序列，输出c的程度为2maxlags+1.c = xcorr(x,y,maxlags,‘option’)同时指定maxlags和option的互相关计算.c = xcorr(x,maxlags,‘option’)同时指定maxlags和option的自相关计算.3.例子3.1计算自相关dt=.1;t=[0:dt:100];x=cos(t);[a,b]=xcorr(x,‘coeff’);plot(b*dt,a)![在这里插入图片描述](https://img-blog.csdn.net/20180930210211337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)3.2计算互相关dt=.1;t=[0:dt:1000];x=cos(t);y=sin(t);[a,b]=xcorr(x,y,200,‘coeff’);plot(b*dt,a)![在这里插入图片描述](https://img-blog.csdn.net/20180930210538318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)*



