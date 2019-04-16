# 开源安卓播放器：Dolphin Player 简单分析 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月25日 16:54:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：31
个人分类：[开源多媒体系统](https://blog.csdn.net/leixiaohua1020/article/category/1362938)










Dolphin播放器(Dolphin Player)是一款开源的音频和视频播放器，它支持大多数的音频和视频文件模式，也支持大部分的字幕文件格式。它是基于ffmpeg的。

项目主页：[http://code.google.com/p/dolphin-player/](http://code.google.com/p/dolphin-player/)

在自己手机上装了一下，感觉不错，绝大部分视频和音频都能播放，同时可以设置ffmpeg内核的很多参数。

下载了一份源代码，发现代码其实并不复杂：

![](https://img-blog.csdn.net/20130925164422890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

该播放器主要通过JNI调用ffmpeg完成视音频的播放。






