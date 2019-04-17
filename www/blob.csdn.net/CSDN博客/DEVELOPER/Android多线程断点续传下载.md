# Android多线程断点续传下载 - DEVELOPER - CSDN博客





2014年05月25日 11:51:44[学术袁](https://me.csdn.net/u012827205)阅读数：1448标签：[android																[多线程断点续传下载																[多线程下载																[断点下载](https://so.csdn.net/so/search/s.do?q=断点下载&t=blog)
个人分类：[网络下多线程断点续传下载](https://blog.csdn.net/u012827205/article/category/2288269)





这个月接到一个项目，要写一个像360助手一样的对于软件管理的APP；其中，遇到了一个问题：多线程断点下载

这个 ，由于之前没有写过这方面的应用功能。所以，不免要自学了。然后就在各个昂站上收索并整理了一下。跟大家分享一下，也能加深我在这方面的理解。



*什么是多线程下载？*

多线程下载其实就是迅雷,BT一些下载原理,通过多个线程同时和服务器连接,那么你就可以榨取到较高的带宽了,大致做法是将文件切割成N块,每块交给单独一个线程去下载,各自下载完成后将文件块组合成一个文件,程序上要完成做切割和组装的小算法

*什么是断点续传?*

断点续传,就是当我们下载未结束时候,退出保存下载进度,当下次打开继续下载的时接着上次的进度继续下载,不用每次下载都重新开始,那么有关断点续传的原理和实现手段。


*怎么完成多线程断点续传?*

将两者合二为一需要程序记住每个文件块的下载进度,并保存入库,当下载程序启动时候你需要判断程序是否已经下载过该文件,并取出各个文件块的保存记录,换算出下载进度继续下载,在这里你需要掌握java多线程的基本知识,handler的使用,以及集合,算法,文件操作等基本技能,同时还要解决sqlite数据库的同步问题,因为它是不太怎么支持多线程操作的,控制不好经常会出现库被锁定的异常,同时在android2.3以后就不能activity中直接操作http,否则你将收到系统送上的NetworkOnMainThreadException异常,在UI体验上一定记住要使用异步完成。

那么，了解过这些之后来看一下源码就会拉结了。。。。。。

先看一下，效果图：




![](https://img-blog.csdn.net/20140525114434312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140525114131718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20140525114729906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




至于那些部分代码，我就不在这里贴了；我觉得还是直接看源码的方便。

[这里是源码的下载网址，点击即可下载](http://download.csdn.net/detail/u012827205/7397501)](https://so.csdn.net/so/search/s.do?q=多线程下载&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程断点续传下载&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




