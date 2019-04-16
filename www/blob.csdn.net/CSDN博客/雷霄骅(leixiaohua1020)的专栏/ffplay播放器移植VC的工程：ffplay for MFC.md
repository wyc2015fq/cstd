# ffplay播放器移植VC的工程：ffplay for MFC - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年06月06日 00:07:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：49









本文介绍一个自己做的FFPLAY移植到VC下的开源工程：ffplayfor MFC。本工程将ffmpeg项目中的ffplay播放器（ffplay.c）移植到了VC的环境下。并且使用MFC做了一套简单的界面。它可以完成一个播放器播放视频的基本流程：解协议，解封装，视频/音频解码，视音频同步，视音频输出。此外还包含一些控制功能：播放，暂停/继续，前进，后退，停止，逐帧播放，全屏等；以及一些简单的视频码流分析功能：视频解码分析和音频解码分析。通过本程序可以学习视频播放器原理，以及SDL和Windows消息机制。

![](https://img-blog.csdn.net/20140605211334250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

平台：VC2010

类库：FFmpeg，MFC



项目下载：

ffplay for MFC1.0.0(stable)——本版本采用FFmpeg的2012年的类库，稳定

[http://download.csdn.net/detail/leixiaohua1020/7452437](http://download.csdn.net/detail/leixiaohua1020/7452437)

ffplay for MFC1.0.1——本版本采用FFmpeg的2014年5月的类库，支持HEVC和VP9，Debug下稳定，Release下目前还有问题。

[http://download.csdn.net/detail/leixiaohua1020/7452493](http://download.csdn.net/detail/leixiaohua1020/7452493)

注：Release下的问题已经得到网友建议并且解决。解决方法如下：

VC工程属性里，linker->Optimization->References 选项，改成No(/OPT:NOREF)。



注：以上两个项目由于失误，少了一个SDL.dll文件，去SDL官网

[http://www.libsdl.org/download-1.2.php](http://www.libsdl.org/download-1.2.php)

下载一个Runtime Libraries即可



SourceForge项目主页：

[https://sourceforge.net/projects/ffplayformfc/](https://sourceforge.net/projects/ffplayformfc/)

补充：

该项目原代码简介可以参考：[ffplay for mfc 代码备忘](http://blog.csdn.net/leixiaohua1020/article/details/33450345)



更新记录==============================
1.1版（2014.7.10）
* 更换了部分界面

* 原工程支持Unicode编码

* 修复了Release下的Bug

* 添加了两个宏定义"INT64_MIN INT64_MAX"，在没有安装 Win7SDK的情况下，可能会出现找不到定义的情况。



CSDN上又上传了一份：

ffplay for MFC1.1

[http://download.csdn.net/detail/leixiaohua1020/7764509](http://download.csdn.net/detail/leixiaohua1020/7764509)





## 一. 主界面

程序的主界面如图所示。最上方是输入的URL。中间是视频的参数列表。下方是视频的控制按钮。

![](https://img-blog.csdn.net/20140605211530343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 1.1输入URL

输入URL框用于输入视频的URL。视频源可以是本地视频或者是网络流。单击输入框右侧的“文件”按钮可以方便地选择本地的文件并获取他们的URL（在这里是路径信息）。此外，也可以将本地视频文件直接拖拽到对话框中，也可以获取到该视频的URL。



### 1.2参数列表

**封装格式参数**

输入类型：输入视音频采用的传输协议。举例：RTP，FILE（文件），HTTP，RTMP。

封装格式：视音频采用的封装格式。举例：AVI，FLV，MKV，RMVB。

比特率：视音频的码率。举例1Mbps。

时长：视音频的时间长度。

MetaData：视音频元数据信息。

**视频参数**

输出像素格式：解码后像素数据格式。举例：YUV420P，RGB24，UYVY。

编码方式：视频采用的压缩编码标准。举例：H.264，MPEG2。

帧率：每秒钟视频画面数。举例：25fps。

画面大小：画面的分辨率。举例：1920x1080，720x576。

解码分析：以列表的形式显示每个视频帧的详细信息。

**音频参数**

采样率：每秒钟采样点个数。举例：44.1kHz，48kHz。

编码方式：音频采用的压缩编码标准。举例：AAC，MP3，WMA。

声道数：声道数量。

解码分析：以列表的形式显示每个音频帧的详细信息。



### 1.3控制按钮

控制按钮包含了：开始，后退，暂停/继续，前进，停止，逐帧播放，全屏。

此外，这一排控制按钮的上方，包含了视频播放的进度条。可以通过拖动进度条控制视频的播放。

## **二. 视频播放窗口**

视频播放采用SDL显示。

![](https://img-blog.csdn.net/20140605211953984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## **三. 视频解码分析窗口**

视频解码分析窗口列表显示了每一个视频帧的一些信息：序号，帧类型，关键帧，码流序号，PTS。其中不同的帧类型使用不同的背景色标识出来：I帧使用红色，P帧使用蓝色，B帧使用绿色。如图所示。

![](https://img-blog.csdn.net/20140605212012843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## **四. 音频解码分析窗口**

音频解码分析窗口列表显示了每个音频帧的一些信息：序号，大小，PTS等。如图所示。

![](https://img-blog.csdn.net/20140605212026750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **五. 系统支持信息窗口**

系统支持信息窗口列表显示了系统的支持信息。包括系统支持的：协议，封装格式，视频解码器，音频解码器，以及其他解码器（字幕解码器）。例如协议列表如图所示，从中可以看出本程序支持的协议类型包含http，file，mms，rtp等等。

![](https://img-blog.csdn.net/20140605212050218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

例如从封装格式列表中，可以看出系统支持flv等封装格式。每个列表包含了序号，简称，全称，扩展名，私有数据大小等信息。

![](https://img-blog.csdn.net/20140605212106906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## **六. 菜单**

程序上端的菜单中包含了更丰富的选项。一共分成6部分的功能：**文件，播放控制，播放窗口，视图，语言，帮助**。下面分别介绍。

**文件**：文件的打开。

**播放控制**：视频播放过程中的控制功能。包含：播放，暂停/继续，停止，逐帧播放，全屏。此外包含了多种快进快退操作：快进10秒，1分，10分；快退10秒，1分，10分。

![](https://img-blog.csdn.net/20140605212124343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**播放窗口**：与播放窗口有关的各种选项。可以设置播放窗口显示的内容，以及窗口的大小，纵横比。播放窗口默认是显示视频画面。此外还可以图形化的显示音频的信息，包括音频的波形图，以及音频的RDFT变换图。

![](https://img-blog.csdn.net/20140605212139953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



音频波形图显示效果如图所示：

![](https://img-blog.csdn.net/20140605212248125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

音频的RDFT变换图显示效果如图所示：

![](https://img-blog.csdn.net/20140605212257562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**视图**：包含程序中的几个窗口。包含视频解码分析窗口，音频解码分析窗口，系统支持信息窗口。

**语言**：包含程序支持的几种语言。

**帮助**：包含程序的支持信息。



## 七. 英文界面

系统的英文界面如图所示。以后有机会可以增加其他语言。

主界面

![](https://img-blog.csdn.net/20140605212317625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

视频解码分析窗口

![](https://img-blog.csdn.net/20140605212337390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

音频解码分析窗口

![](https://img-blog.csdn.net/20140605212348281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

系统支持信息窗口

![](https://img-blog.csdn.net/20140605212401750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






