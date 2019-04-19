# wp8开发基础(1)-打印调试信息(Debug) - xqhrs232的专栏 - CSDN博客
2014年05月13日 10:40:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：711
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/skyhh/article/details/17417059](http://blog.csdn.net/skyhh/article/details/17417059)
写程序时，调试很重要
打印调试信息又是重中之重
在wp8开发时，怎么打印调试信息呢?
用
Debug.WriteLine("内容");
如：
![](https://img-blog.csdn.net/20131219160248718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2t5aGg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意：需要引用System.Diagnostics
但有点坑爹的是，在调试模式时，默认是没有输出窗口的。
Debug.WriteLine输出的内容无法显示![生气](http://static.blog.csdn.net/xheditor/xheditor_emot/default/mad.gif)
需要自己打开
(环境：Microsoft Visual Studio 2013)
步骤如下：
菜单-->调试(D)-->窗口(W)-->输出(O)
如图：
![](https://img-blog.csdn.net/20131219160218234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2t5aGg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
没有打开前
![](https://img-blog.csdn.net/20131219160125734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2t5aGg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
打开后
![](https://img-blog.csdn.net/20131219160144859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2t5aGg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
希望对你有用处
