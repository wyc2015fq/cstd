# VirtualBox创建虚拟电脑、运行Genymotion模拟器报错 - DEVELOPER - CSDN博客





2014年08月18日 10:32:26[学术袁](https://me.csdn.net/u012827205)阅读数：2395标签：[virtualbox																[创建虚拟电脑失败																[运行Genymotion模拟器报错](https://so.csdn.net/so/search/s.do?q=运行Genymotion模拟器报错&t=blog)
个人分类：[错误总结值Virtualbox](https://blog.csdn.net/u012827205/article/category/2493727)





当安装完Genynition关于Android应用的调试模拟器之后，在Genymotion运行的平台virtualBox：VirtualBox创建虚拟电脑、运行Genymotion模拟器报错；

错误卖相是这样的<这张图是截取的别人的，但是我敢说我的解释才够让你一目了然>：


![](https://img-blog.csdn.net/20140818101519656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


































对于这类错误的解决办法，我亲自运行了下并解决了自己的问题：


首先：window+R<快捷键> 进入运行页面  输入： CMD        打开运行窗口，首先找到自己安装VirtualBox模拟器的文件安装写入位置。我的是默认安装，所以目录是：

```java
C:\Program Files\Oracle VM VirtualBox
```


具体操作：第一步就是要找到你的安装VirturalBox的安装目录。语言难以表达清楚，有图才有真相<途中分别对应的具体操作步骤>：

![](https://img-blog.csdn.net/20140818102804994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




















































然后，重启电脑 运行自己的VirtualBox，一切都会OK了。

待续。。。](https://so.csdn.net/so/search/s.do?q=创建虚拟电脑失败&t=blog)](https://so.csdn.net/so/search/s.do?q=virtualbox&t=blog)




