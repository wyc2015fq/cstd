# FFMPEG源码下载和对应windows下DLL库下载使用 - zhuyinglong的博客 - CSDN博客





2017年09月12日 11:17:32[初见时的微笑](https://me.csdn.net/zhuyinglong2010)阅读数：486








官方地址

[http://ffmpeg.org/download.html](http://ffmpeg.org/download.html)

1、下载源码

![](https://img-blog.csdn.net/20170912105503851?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2、下载windows版本对应库

使用下载的源码可通过configure选项自行编译出所需的相应版本的库。如不想自己编译可使用如下方法下载已经编译好的对应版本的ffmpeg库。

1）点击页面如下按钮


![](https://img-blog.csdn.net/20170912105548278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2）下载对应版本的shared库与Dev库

![](https://img-blog.csdn.net/20170912105604756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下载完成如下

![](https://img-blog.csdn.net/20170912105623753?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3、使用

ffmpeg的dev版本中并没有包含dll，只有lib，需要在相同版本中取对应的dll配合使用。

1）取dev版本的lib及头文件

![](https://img-blog.csdn.net/20170912105641240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20170912110142285?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


2）取对应shared版本的dll

![](https://img-blog.csdn.net/20170912105725161?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3）最终所需三个文件如下图所示。在vs中配置使用方法与其他dll的使用方法相同，不予说明。

![](https://img-blog.csdn.net/20170912111622457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







