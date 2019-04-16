# XBMC源代码分析 3：核心部分（core）-综述 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年01月02日 13:07:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：31
个人分类：[XBMC](https://blog.csdn.net/leixiaohua1020/article/category/2286583)










前文分析了XBMC的整体结构以及皮肤部分：

[XBMC源代码分析 1：整体结构以及编译方法](http://blog.csdn.net/leixiaohua1020/article/details/17454977)

[XBMC源代码分析 2：Addons（皮肤Skin）](http://blog.csdn.net/leixiaohua1020/article/details/17511993)

本文以及以后的文章主要分析XBMC的VC工程中的源代码。XBMC源代码体积庞大，想要完全分析所有代码是比较困难的。在这里我们选择它和音视频编解码有关的部分进行分析。在本文里，我们主要分析其核心部分（core）代码。

核心部分（core）源代码结构如图所示：

![](https://img-blog.csdn.net/20140102130422828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我目前理解的有以下3个，其他的有时间研究后再补上：

> 
AudioEngine：音频引擎。其封装了所有不同的媒体类型的混音、采样率转换、格式转换、编码、上混、缩混等。

dvdplayer：视频播放器。其中封装了FFMPEG等一些库，是我们分析的重点。

paplayer：XBMC自行开发出来的音频播放器。


本系列文章将会重点分析dvdplayer这个播放器。



下面我们先来看看dvdplayer的代码结构：

![](https://img-blog.csdn.net/20140102130443468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

先不说一大堆cpp文件。dvdplayer包含以下5个文件夹，我们分析以下3个文件夹中的内容

> 
DVDCodecs：封装各种解码器

DVDDemuxers：封装各种解复用器

DVDHeaders：封装各种Dll的头文件




DVDCodecs里面包含各种解码器的封装，下图列出了封装视频解码器的文件。

![](https://img-blog.csdn.net/20140102130522031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

DVDDemuxers里面包含了各种解复用器（视音频分离器）的封装，如下图所示。

![](https://img-blog.csdn.net/20140102130536250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

DVDHeaders里面包含了封装各种Dll的头文件，如下图所示。

![](https://img-blog.csdn.net/20140102130613531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

详细的分析会在后续文章中完成。








