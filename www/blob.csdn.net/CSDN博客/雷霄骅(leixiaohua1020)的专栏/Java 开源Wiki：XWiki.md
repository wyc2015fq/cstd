# Java 开源Wiki：XWiki - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月30日 00:29:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：40
个人分类：[开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1652801)










XWiki是一个由Java编写的基于LGPL协议发布的开源wiki和应用平台。之前只接触过MediaWiki，但是MediaWiki是用PHP写的，一直想找找看有没有熟悉的JAVA语言的Wiki系统。于是乎就找到了XWiki。

![](https://img-blog.csdn.net/20131220213618296)

从XWiki首页上（[http://www.xwiki.org/](http://www.xwiki.org/)）可以下载到源代码，以及可以运行的程序。

PS：首页挺漂亮的，特意截图一张~

![](https://img-blog.csdn.net/20131220213627609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

首先简单介绍一下安装步骤：

1.首先要安装JAVA，这个就不多说了。

2.XWiki有独立的Standalone distribution，也有可以部署于服务器上的WAR distribution。我自己测试的时候使用的是Standalone distribution。Standalone distribution还分成好几种：直接安装的exe，或者是压缩包zip。我选择了压缩包zip。

3.运行解压后根目录里的start_xwiki.bat，就可以启动服务器了。

4.使用浏览器访问http://localhost:8080/即可打开XWiki首页。



以下几张截图展示了界面。

DashBoard：

![](https://img-blog.csdn.net/20131220213635281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

内容：

![](https://img-blog.csdn.net/20131220213703171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

博客页面：

![](https://img-blog.csdn.net/20131220213714484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

点击页面右上角的Log in，可以登录系统的后台：

![](https://img-blog.csdn.net/20131220213727921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在后台中可以对Wiki的内容进行管理（添加，修改，删除等等）

![](https://img-blog.csdn.net/20131220213737859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



XWiki首页：[http://www.xwiki.org/](http://www.xwiki.org/)




