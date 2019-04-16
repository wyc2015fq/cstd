# RTMPDump源代码分析 0： 主要函数调用分析 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月19日 01:00:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：32









rtmpdump 是一个用来处理 RTMP 流媒体的开源工具包，支持 rtmp://, rtmpt://, rtmpe://, rtmpte://, and rtmps://.也提供 Android 版本。

最近研究了一下它内部函数调用的关系。

下面列出几个主要的函数的调用关系。

RTMPDump用于下载RTMP流媒体的函数Download：

![](https://img-blog.csdn.net/20130919005536296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

用于建立网络连接（NetConnect）的函数Connect：

![](https://img-blog.csdn.net/20130919005553859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

用于建立网络流（NetStream）的函数

![](https://img-blog.csdn.net/20130919005800968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



rtmpdump源代码（Linux）：[http://download.csdn.net/detail/leixiaohua1020/6376561](http://download.csdn.net/detail/leixiaohua1020/6376561)

rtmpdump源代码（VC 2005 工程）：[http://download.csdn.net/detail/leixiaohua1020/6563163](http://download.csdn.net/detail/leixiaohua1020/6563163)




