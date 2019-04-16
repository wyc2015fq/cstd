# 开源DirectShow分析器和解码器： LAV Filter - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月21日 10:43:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：46









LAV Filter是一款开源的DirectShow视频分离和解码软件，他的分离器LAVSplitter封装了FFMPEG中的libavformat，解码器LAVAudio和LAVVideo则封装了FFMPEG中的libavcodec。它支持十分广泛的视音频格式。

支持状况：
 封装格式：
 MKV/WebM, AVI, MP4/MOV, MPEG-TS/PS (including basic EVO support), FLV, OGG, 以及其他ffmpeg支持的格式！

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
 如果有问题请反馈至Google Code： [http://code.google.com/p/lavfilters/issues/list](http://code.google.com/p/lavfilters/issues/list)

 下载：
 源代码位于GitHub或Google Code：
[https://github.com/Nevcairiel/LAVFilters](https://github.com/Nevcairiel/LAVFilters)
[http://code.google.com/p/lavfilters/](http://code.google.com/p/lavfilters/)

 这里有一个为了更适用于LAV Filters而稍微修改过的ffmpeg：

[http://git.1f0.de/gitweb?p=ffmpeg.git;a=summary](http://git.1f0.de/gitweb?p=ffmpeg.git;a=summary)



英文原帖：[http://forum.doom9.org/showthread.php?t=156191](http://forum.doom9.org/showthread.php?t=156191)




