# Spring MVC 中发布jws webservice 请求HTTP404错误处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年12月26日 17:17:28[boonya](https://me.csdn.net/boonya)阅读数：4475








Spring MVC 中发布jws webservice 请求HTTP404错误处理。

正确的地址在这里：[http://blog.csdn.net/boonya/article/details/17577521](http://blog.csdn.net/boonya/article/details/17577521)

**1、现象描述：服务器正常启动，但是webservice和服务器网页均不能访问，如图：**

![](https://img-blog.csdn.net/20131226171516875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**2、解决方案：将webservice与服务器的端口设为不同就行。**

![](https://img-blog.csdn.net/20131226171618453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


结果截图：

![](https://img-blog.csdn.net/20131226172042937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







