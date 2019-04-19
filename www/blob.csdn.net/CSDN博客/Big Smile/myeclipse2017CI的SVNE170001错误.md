# myeclipse2017CI的SVNE170001错误 - Big Smile - CSDN博客
2017年11月26日 22:46:33[王啸tr1912](https://me.csdn.net/tr1912)阅读数：632
         最近在公司使用myeclipse来开发项目，但是在安装SVN的时候却出现了一些小小的波折，现在和大家分享一下。
**一、安装svn插件**
        想必大家对于svn插件的安装已经非常熟悉了，就是把下载好的svn插件包中的
![](https://img-blog.csdn.net/20171126214339704?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个两个文件夹和里面所有的东西复制，然后到myeclipse中的plugins这个文件夹中，并建立一个叫SVN的文件夹，然后把复制的东西粘贴到这个SVN文件夹下，这样就安装成功了，我们在myeclipse的菜单中可以看到这个插件了。首先我们打开视图管理：
![](https://img-blog.csdn.net/20171126221849721?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后找到svn文件夹，找到资源库，点击OK即可
![](https://img-blog.csdn.net/20171126221926526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个时候我们就打开了SVN的资源库，可以进行添加资源库位置，检出等一系列操作了。
# 二、出现的问题
        但是在添加完成插件之后就出现了问题，首先是：Subversion Native Library Not Available这个问题，这个问题通常是由于Subclipse版本SVN和JavaHL版本不兼容的问题，我们首先要找到对应的合适的版本，然后去安装，问题一般都能解决，版本的对应我参考了这篇博客：[http://blog.csdn.net/shenhonglei1234/article/details/51270868](http://blog.csdn.net/shenhonglei1234/article/details/51270868)
        然后我查到了另外一篇博客，里面直接讲的是如何在2017上装svn的，试了一下果然好用，主要是用的myeclipse自带的安装插件的功能进行安装：
      首先在help中找到从网站安装：
![](https://img-blog.csdn.net/20171126223407490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后在work with后面写上svn - http://subeclipse.tigris.org/update_1.10.x
然后选择出现的两项，就可以了点击安装了。
博客链接：[https://www.cnblogs.com/liuyk-code/p/7519886.html](https://www.cnblogs.com/liuyk-code/p/7519886.html)
