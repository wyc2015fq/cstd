# ffdshow神奇的功能：视频播放时显示运动矢量和QP - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月23日 22:12:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：47









FFDShow可以称得上是全能的解码、编码器.最初FFDShow只是mpeg视频解码器,不过现在他能做到的远不止于此.它能够解码的视频格式已经远远超出了mpeg4的范围,包括indeo video,WMV,mpeg2等等.同时,它也提供了丰富的加工处理选项,可以锐化画面,调节画面的亮度等等.不止是视频,FFDShow现在同样可以解码音频,AC3、MP3等音频格式都可支持.并且可以外挂winamp 的DSP插件,来改善听觉效果.可以说现在的FFDShow已经是windows平台多媒体播放的非常出色的工具了。



FFDshow功能十分强大，我们只要打开它的设置界面就会发现，它有大量的视频滤镜等，配置页面如图所示。

我发现它竟然可以在视频播放时显示运动矢量和QP！很神奇

![](https://img-blog.csdn.net/20130923220326359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在左边栏中有一个是"可视化效果"（英文版的是"Visualization"）

注意：我修改过FFDshow源代码，所以”可视化效果“里面多了两项，一般情况下为3项

![](https://img-blog.csdn.net/20130923220311390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

勾选”运动矢量“之后的效果。可以发现视频激烈运动的地方有大量的箭头

![](https://img-blog.csdn.net/20130923220710671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

勾选”量化“后的效果。可见每个宏块的量化值都被显示了出来

![](https://img-blog.csdn.net/20130923220739484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

勾选”图示“后的效果。可见视频每一帧的帧大小都被画在视频的最下面

![](https://img-blog.csdn.net/20130923220921546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

FFDshow直接让播放器变成码流分析软件了！




