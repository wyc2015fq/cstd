# 未能加载文件或程序集“System.Web.Helpers, Version=2.0.0.0 - 左直拳的马桶_日用桶 - CSDN博客
2014年01月03日 16:08:39[左直拳](https://me.csdn.net/leftfist)阅读数：20263
在本地终于用上了ASP.NET MVC4自带的认证功能，但放到生产服务器上就出问题了：打开注册页面没问题，但一点下注册按钮就报错了：
*未能加载文件或程序集“System.Web.Helpers, Version=2.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35”或它的某一个依赖项。系统找不到指定的文件。*
生产服务器是个阿里云，WINDOWS2008，装了.NET4.5.。
网上搜了下，说应该安装MVC4。
我到微软站点下了个MVC4，发现还是BETA版。beta版就beta版，但装了之后，结果又报另外一个错。
后来，受网上一篇文章的启示，在项目里引用 System.Web.Helpers，将属性设为“复制本地”，发布之后得到System.Web.Helpers.dll，拷贝到生产服务器对应的BIN目录，问题解决。
![](https://img-blog.csdn.net/20140103160805421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140103160826250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考文章：
[http://www.tuicool.com/articles/Ajq2Un](http://www.tuicool.com/articles/Ajq2Un)
