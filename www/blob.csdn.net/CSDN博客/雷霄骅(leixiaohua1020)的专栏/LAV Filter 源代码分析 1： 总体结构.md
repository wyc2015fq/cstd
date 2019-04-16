# LAV Filter 源代码分析 1： 总体结构 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月14日 17:12:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：30
个人分类：[DirectShow](https://blog.csdn.net/leixiaohua1020/article/category/1645657)










LAV Filter 是一款视频分离和解码软件，他的分离器封装了FFMPEG中的libavformat，解码器则封装了FFMPEG中的libavcodec。它支持十分广泛的视音频格式。

源代码位于GitHub或Google Code：
[https://github.com/Nevcairiel/LAVFilters](https://github.com/Nevcairiel/LAVFilters)
[http://code.google.com/p/lavfilters/](http://code.google.com/p/lavfilters/)


本文分析了LAV Filter源代码的总体架构。

使用git获取LAV filter源代码之后，使用VC 2010 打开源代码，发现代码目录结构如图所示：

![](https://img-blog.csdn.net/20131014170118062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

整个解决方案由8个工程组成，介绍一下我目前所知的几个工程：

> 
baseclasses：DirectShow基类，在DirectShow的SDK中也有，是微软为了简化DirectShow开发而提供的。

Demuxers：解封装的基类，LAVSplitter需要调用其中的方法完成解封装操作。

LAVAudio：音频解码Filter。封装了libavcodec。

LAVSplitter：解封装Filter。封装了libavformat。

LAVVideo：视频解码Filter。封装了libavcodec。

libbluray：蓝光的支持。


以上标为咖啡色字体的是要重点分析的，也是最重要的工程。










