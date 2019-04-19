# Qt常见设置背景图片的几种方法总结 - fanyun的博客 - CSDN博客
2017年10月29日 19:03:39[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4187
     常用的设置背景的方式有三种：
1、利用style sheet给窗体设置背景。
2、利用Qt UI设计器的改变样式表进行设置。如下：
![](https://img-blog.csdn.net/20171024104439676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、通过如下代码来实现。
QPixmap pixmap(":/Resources/Penguins.jpg");
