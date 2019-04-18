# 目前最好的DirectShow分离器和解码器： LAV Filter - 爱写代码的自由人 - CSDN博客





2018年02月24日 15:04:11[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：1356








之前搞Directshow开发经常用到各种Directshow插件，用的比较多的是FFDShow解码器，还有MPC开源项目下那一堆乱七八糟的Filter。对于开发一个播放器来说，用这两个开源项目的Filter还不是很够用，特别是要支持某些比较新的或罕见的多媒体格式时经常缺少相应的分离器，或上网找找不到免费的，而商用的Filter价格又比较高。还有另外一个问题：面对越来越多的多媒体封装格式和编码格式，要加入支持新格式的Filter越来越多，没有一种集成度高、统一的方式来处理，结果程序兼容性低、扩充功能麻烦。LAV Filter的出现为广大Directshow开发员带来福音，它的功能之强大是其他Filter望尘莫及的，它包括了分离器和解码器Filter，只要装上这些Filter基本上各种格式通吃，做Directshow开发就比以前轻松多了。毫不夸张的说，LAV Filter是目前功能最齐全、最好用的Directshow插件，没有之一。




LAV Filter 是一款开源的DirectShow视频分离和解码软件。它的分离器LAVSplitter封装了FFMPEG中的libavformat，解码器LAVAudio和LAVVideo则封装了FFMPEG中的libavcodec。它支持十分广泛的视音频格式。

支持状况：
封装格式：
MKV/WebM, AVI, MP4/MOV, MPEG-TS/PS (including basic EVO support), FLV, OGG, 以及其他ffmpeg支持的格式
视频编码：
H264, VC-1, MPEG-2, MPEG4-ASP (Divx/Xvid), VP8, MJPEG 等。
音频编码：
AAC, AC3, DTS(-HD), TrueHD, MP3/MP2, Vorbis, LPCM 等。
字幕：
VOB, DVB Subs, PGS, SRT, SSA/ASS, Text
蓝光：
支持打开蓝光播放列表 (.mpls), 或者 index.bdmv 文件
蓝光支持仍处于早期阶段，仍有许多改进和新功能计划！

反馈：
如果有问题请反馈至Google Code： [http://code.google.com/p/lavfilters/issues/list](http://code.google.com/p/lavfilters/issues/list)

下载：
源代码位于GitHub或Google Code：
[https://github.com/Nevcairiel/LAVFilters](https://github.com/Nevcairiel/LAVFilters)
[http://code.google.com/p/lavfilters/](http://code.google.com/p/lavfilters/)

这里有一个为了更适用于 LAV Filters而稍微修改过的ffmpeg：

[http://git.1f0.de/gitweb?p=ffmpeg.git;a=summary](http://git.1f0.de/gitweb?p=ffmpeg.git;a=summary)




英文原帖：[http://forum.doom9.org/showthread.php?t=156191](http://forum.doom9.org/showthread.php?t=156191)



