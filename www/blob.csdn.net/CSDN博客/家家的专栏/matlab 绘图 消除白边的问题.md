# matlab 绘图 消除白边的问题 - 家家的专栏 - CSDN博客





2013年08月22日 23:07:27[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2085








![](https://img-blog.csdn.net/20130822230244250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



在画出的barh上，上面有很大一块白边。。。



捉摸了很久发现一个问题。

在Figures的面板上，点击View-〉Property Editor。  点击窗口中的图。

发现最下面的面板。

![](https://img-blog.csdn.net/20130822230619406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

把YLimits 0 to 12， 改为0 to 11 即可。



![](https://img-blog.csdn.net/20130822230826578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



