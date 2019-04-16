# FFmpeg 2.1 试用（新版支持HEVC，VP9） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月01日 12:37:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：118









前两天帮一位老师转码图像的时候，无意间发现新版FFmpeg竟然支持了下一代编码标准HEVC，以及Google提出的下一代编码标准VP9。真心没想到FFmpeg对下一代的编码标准支持的是如此之快。我还以为这两种编码标准还要在实验室呆上1年呢。看来这两种编码标准离大规模应用已经不远了。



下面是其更新列表，红色部分为其新增加的HEVC和VP9的支持：

- aecho filter
 - perspective filter ported from libmpcodecs
 - ffprobe -show_programs option
 - compand filter
 - RTMP seek support
 - when transcoding with ffmpeg (i.e. not streamcopying), -ss is now accurate
 even when used as an input option. Previous behavior can be restored with
 the -noaccurate_seek option.
 - ffmpeg -t option can now be used for inputs, to limit the duration of
 data read from an input file
 - incomplete Voxware MetaSound decoder
 - read EXIF metadata from JPEG
 - DVB teletext decoder
 - phase filter ported from libmpcodecs
 - w3fdif filter
 - Opus support in Matroska
 - FFV1 version 1.3 is stable and no longer experimental
 - FFV1: YUVA(444,422,420) 9, 10 and 16 bit support
 - changed DTS stream id in lavf mpeg ps muxer from 0x8a to 0x88, to be
 more consistent with other muxers.
 - adelay filter
 - pullup filter ported from libmpcodecs
 - ffprobe -read_intervals option
 - Lossless and alpha support for WebP decoder
 - Error Resilient AAC syntax (ER AAC LC) decoding
 - Low Delay AAC (ER AAC LD) decoding
 - mux chapters in ASF files
 - SFTP protocol (via libssh)
 - libx264: add ability to encode in YUVJ422P and YUVJ444P
 - Fraps: use BT.709 colorspace by default for yuv, as reference fraps decoder does
 - make decoding alpha optional for prores, ffv1 and vp6 by setting
 the skip_alpha flag.
 - ladspa wrapper filter
- native VP9 decoder
 - dpx parser
 - max_error_rate parameter in ffmpeg
 - PulseAudio output device
 - ReplayGain scanner
 - Enhanced Low Delay AAC (ER AAC ELD) decoding (no LD SBR support)
 - Linux framebuffer output device
- HEVC decoder, raw HEVC demuxer, HEVC demuxing in TS, Matroska and MP4
 - mergeplanes filter



于是果断试了一下，用ffplay播放了一下实验室里的HEVC序列以及VP9序列，发现播放高清序列很流畅，不卡。

播放序列截图：

1920x1080，逐行扫描

![](https://img-blog.csdn.net/20131101123354156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

ffplay播放VP9时候控制台输出：

![](https://img-blog.csdn.net/20131101123522015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

ffplay播放hevc时候控制台输出：

![](https://img-blog.csdn.net/20131101123527062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



上传了一份FFmpeg 2.1：[http://download.csdn.net/detail/leixiaohua1020/6486599](http://download.csdn.net/detail/leixiaohua1020/6486599)

========补充==============

HEVC测试序列合集：[http://download.csdn.net/detail/leixiaohua1020/6843803](http://download.csdn.net/detail/leixiaohua1020/6843803)

VP9测试序列合集：[http://download.csdn.net/detail/leixiaohua1020/6843921](http://download.csdn.net/detail/leixiaohua1020/6843921)




