# 开源实时视频码流分析软件：VideoEye - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年06月26日 00:38:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：73
个人分类：[我的开源项目																[FFMPEG](https://blog.csdn.net/leixiaohua1020/article/category/1360795)](https://blog.csdn.net/leixiaohua1020/article/category/1843731)









本文介绍一个自己做的码流分析软件：VideoEye。为什么要起这个名字呢？感觉这个软件的主要功能就是对“视频”进行“分析”。而分析是要用眼睛来看的，因此取了“视频”和“分析”这两个词的英文，名之曰：VideoEye。这个软件是在自己毕业设计软件的基础上改的。可以对本地文件或者互联网视频流进行实时的码流分析。由于这个软件是自己一边学习视音频技术一边制作的，所以涵盖了比较全面的功能。在编写这个软件的过程中，自己也学会了很多的视音频编解码方面的知识，以及MFC的知识。后来想想，与其自己保存在电脑里，不如开源出来与大家分享，也许能帮助正在学习视音频技术的人学习这方面的知识。软件源代码中有大量的注释，都是自己边学边记的笔记，十分方便理解和学习有关的知识。

开发环境为VC2010，软件界面使用以下类库：

界面：MFC

视音频编解码类库：FFMPEG

视频非压缩域数据分析：OpenCV

播放列表解析/导出：TinyXML

视音频播放：SDL

目前还在完善过程中，估计还要不少的修改。



项目主页：

[https://sourceforge.net/projects/videoeye](https://sourceforge.net/projects/videoeye)



0.1测试版=================================

CSDN源代码下载

[http://download.csdn.net/detail/leixiaohua1020/7552669](http://download.csdn.net/detail/leixiaohua1020/7552669)

CSDN编译好的可执行程序下载（目前只在本机上测过）

[http://download.csdn.net/detail/leixiaohua1020/7552687](http://download.csdn.net/detail/leixiaohua1020/7552687)

注：晕了，上面2个上传至CSDN的文件都缺失了3个OpenCV的Dll，编译或者运行的时候会提示找不到Dll。由于CSDN上传的资源没有提供删除功能，只能再上传一个压缩包补齐相关的Dll。下面的Dll和其他Dll放到一起就可以了：

[http://download.csdn.net/detail/leixiaohua1020/7555063](http://download.csdn.net/detail/leixiaohua1020/7555063)



0.2测试版（2014.7.12）======================



相比于0.1测试版，做了以下几个部分的完善：

* 源代码添加了对Unicode的支持

* 添加了两个宏定义"INT64_MIN INT64_MAX"，在没有安装 Win7SDK的情况下，可能会出现找不到定义的情况。

* 改变了“收藏夹”的外观，修改了其展现视频地址的树形结构，使其美观一些。

* “单帧详细分析”界面增加了数据输出功能。可以将一帧视频码流的量化参数（Quantization Parameter），宏块类型（MacroBlock Type），运动矢量（Motion Vector），参考帧（Reference Frame）列表输出出来并存储为“.csv”格式的文件。

* 修正了“单帧详细分析”中运动矢量分析功能在分析MPEG4视频码流时候的一个BUG。

* 修正了“视频播放窗口”调整窗口大小的时候，会残留视频帧画面的BUG。



SourceForge上已经更新至0.2版。



CSDN源代码下载

[http://download.csdn.net/detail/leixiaohua1020/7624137](http://download.csdn.net/detail/leixiaohua1020/7624137)

CSDN编译好的可执行程序下载

[http://download.csdn.net/detail/leixiaohua1020/7624119](http://download.csdn.net/detail/leixiaohua1020/7624119)

注：需要VC2010的运行环境。如果出现找不到msvcp100.dll等文件的话，可以选择以下之一：

1.安装[Microsoft Visual C++ 2010 Redistributable Package](http://www.microsoft.com/en-us/download/details.aspx?id=5555)

2.下载压缩包“[VC2010编译的MFC程序需要的dll](http://download.csdn.net/detail/leixiaohua1020/7625627)”，并且将里面的Dll拷贝到程序的目录中



下面将自己总结的软件的文档贴出来。

![](https://img-blog.csdn.net/20140625231629937)



## 1简述

VideoEye是一个开源的视频分析的软件。本软件可以播放和分析视频数据。它支持多种视频流输入方式：HTTP，RTMP，RTSP以及文件等等。该软件可以实时分析视频流并能以图形化的方式呈现其分析结果。目前该软件还处于完善阶段。

### 1.1视频播放

视频播放是本软件最基本的功能。

### 1.2压缩域码流分析

压缩域码流分析主要用于分析视频和音频压缩码流的参数。

### 1.3非压缩域数据分析

非压缩域数据分析主要用于分析视频解码后的像素数据。

## 2主界面

本章简单介绍系统主界面的操作和使用。

软件运行后欢迎画面如图所示：



![](https://img-blog.csdn.net/20140625232045203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图2-1.欢迎画面



进入主界面以后，如下图所示。



![](https://img-blog.csdn.net/20140625232837734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图2-2.主界面



如果想让系统开始运行的话，首先需要选择一个视频（音频）文件。将视频所在的URL粘贴到“输入路径”里面即可。
注1：还可以通过以下几种方式添加输入路径
1.单击“文件”，打开文件对话框，选择一个视频文件
2.拖拽一个视频文件到主界面
3.单击“收藏夹”，打开收藏夹列表，选择一个视频的URL
注2：本系统不但支持本地文件的分析，也支持网络流的分析。





在这里我们选择一个视频文件，URL是“F:\movie\cuc_ieschool.flv”。

单击位于主界面左下角的“播放”大按钮（一个圆圈里面有一个三角形），即可让系统开始运行。系统运行后的截图如下图所示。



![](https://img-blog.csdn.net/20140625235920390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图2-3.主界面（播放中）



由图可见，在“输入参数”部分，系统解析出了输入协议类型为file，封装格式为FLV。比特率是394.94kbps，时长是34s。 “视频”部分，系统解析出了输出像素格式为YUV420P，视频编码方式为H.264，帧率为15fps，画面大小为512x288。“音频”部分，系统解析出了采样率为44100Hz，音频编码方式为MP3，声道数为2。
单击主界面下方的“播放控制”面板上的按钮，可以控制视频的播放。具体的功能包含快退，暂停，快进，停止，逐帧播放，全屏播放。拖动视频播放的进度条，则可以控制视频播放的进度（对于直播信号，是不能调整播放进度的）。

系统开始运行之后，会弹出视频播放窗口。该窗口类似于视频播放器，可以显示解码后的视频数据，并播放音频数据。



![](https://img-blog.csdn.net/20140625233132593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图2-4.播放窗口

## 3播放

本章主要介绍和视频播放相关的功能。播放是本系统最基本的功能。系统其它功能都是建立在播放的基础之上的。

### 3.1视频URL

如果想让系统开始播放的话，首先需要选择一个视频（音频）文件。将视频所在的URL粘贴到“输入路径”里面即可。
注1：还可以通过以下几种方式添加输入路径
1.单击“文件”，打开文件对话框，选择一个视频文件
2.拖拽一个视频文件到主界面
3.单击“收藏夹”，打开收藏夹列表，选择一个视频的URL
4.单击“地址解析”，可以使用网络上现有的地址解析引擎，解析得到像优酷，土豆，乐视这些网站上视频的URL。
注2：本系统不但支持本地文件的分析，也支持网络流的分析。

### 3.2收藏夹

收藏夹功能目前还处于调整中。支持导入m3u格式的播放列表，以及XSPF格式的播放列表。双击收藏夹中的条目，可以将该条目对应的地址传给主界面的“输入路径”。



![](https://img-blog.csdn.net/20140625233156937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图3-1.播放列表对话框



### 3.3播放控制

在“输入路径”里添加视频的URL之后，单击系统左下角的“播放”大按钮（一个圆圈里面有一个三角形），就可以开始播放了。
在“播放”按钮的旁边，排列着其它控制播放的按钮。依次是：“后退”，“暂停”，“前进”，“停止”，“逐帧播放”，“全屏”。通过这些按钮，基本上可以完成对播放的各种控制。此外，在这些按钮的上方，还有一个播放的进度条。可以通过拖拽进度条的方式，调整视频播放的进度。

在系统的右下角，有一个按钮：“关于”。



![](https://img-blog.csdn.net/20140625233215375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图3-2.播放控制按钮

### 3.4播放设置

选择菜单的“播放”->“播放器首选项”。打开播放设置对话框如图所示。

注：此处的设置只有在下一次视频播放开始后才会生效。



![](https://img-blog.csdn.net/20140625233235562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图3-3.播放器首选项对话框



### 3.5播放画面

选择菜单的“视频”->“大小”。可以调整视频播放窗口的大小。
选择菜单的“视频”->“纵横比”。可以调整视频播放窗口的纵横比。
选择菜单的“视频画面”，可以调整视频播放窗口显示的内容，有以下3种：
*视频画面
*音频波形图
*音频离散傅里叶变换图

### 3.6数据输出

选择菜单的“数据”。可以输出视频播放过程中的中间数据。该选项卡可以用于输出视音频码流数据，视频解码后的像素数据，或者音频解码后的采样数据。数据输出选项卡如下图所示。

注：特殊容器（mp4,mkv等）封装的H.264直接输出的话，会缺少SPS和PPS，因而导致码流无法被识别。为此专门添加了特殊容器输出H.264的选项。



![](https://img-blog.csdn.net/20140625233300203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图3-4.数据输出对话框



## 4视频分析

本章主要介绍和视频分析相关的功能。

### 4.1视频解码分析

在视频播放的过程中，单击主界面视频部分编码参数部分的按钮“视频解码分析”，打开视频解码分析对话框，如图4-1所示。
对话框中包含了一个帧列表。每个帧对应列表中的一条记录。不同类型的帧有着不同的背景色。列表显示了以下信息：
*帧数
*帧类型
*关键帧
*码流顺序
*PTS

![](https://img-blog.csdn.net/20140625233550421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4-1.视频解码分析对话框



### 4.2视频帧解码分析

在视频播放的过程中，单击主界面视频部分编码参数部分的按钮“单帧详细分析”，打开视频帧解码分析对话框，如图4-2所示。
该部分主要用于对当前播放的视频帧进行详细的分析。可以列表显示视频一帧的详细参数，包括：
*帧数
*帧类型
*PTS
*显示时间
*参考帧数量
并可以对视频进行宏块级的分析，包含以下几种参数的分析：
*量化参数分析
*宏块类型分析
*运动矢量list[0]分析
*运动矢量list[1]分析
*参考帧list[0]分析
*参考帧list[1]分析

对话框上方的下拉框用于设置希望分析的内容。对话框左边的“选项”部分可以设置分析结果的属性。具体包含以下几项：

表4-1.视频帧详细分析选项

|通用选项| |
|----|----|
|显示宏块边界|显示视频帧中宏块的边界。|
|字体|设置分析结果中文字的字体。|
|量化分析| |
|显示QP值|显示宏块的QP值。|
|显示背景颜色|根据QP值的不同，不同宏块显示不同灰度的背景颜色。|
|宏块类型分析| |
|显示子宏块|显示子宏块的划分方式。|
|显示背景颜色|根据划分方式的不同，不同宏块显示不同的背景颜色。|
|显示跳过宏块|在跳过宏块的上方标记以“s”。|
|显示参考列表|在使用参考帧的宏块上标记List0和List1。|
|运动矢量[0]分析| |
|颜色|显示的运动矢量[0]颜色。|
|样式|运动矢量[0]外观。|
|运动矢量[1]分析| |
|颜色|显示的运动矢量[1]颜色。|
|样式|运动矢量[1]外观。|





对话框中间的“宏块类型”部分包含了各种类型的宏块的数量的统计信息。例如帧内4x4，帧内16x16，16x16，16x8，8x16，8x8等类型的宏块的个数。以及每行宏块数，宏块行数，总计宏块数，每个宏块包含的运动矢量个数等信息。
对话框右边上方的“帧参数”部分包含了该视频帧的一些其他信息。例如帧数，帧类型，大小，PTS，显示时间，参考帧数量等信息。
对话框右边下方的“量化”部分包含了QP的统计信息。包括QP的最大值，最小值以及平均值。

此外，如果勾选了“随播放自动分析”选项，可以随着系统对视频的解码播放，实时的分析视频的码流参数。



![](https://img-blog.csdn.net/20140625233649265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4-2.视频帧解码分析对话框



量化参数分析结果如图所示。视频帧被划分成以宏块为单位的网格状。图中每个小方块代表视频码流中的一个宏块。其中的数字代表了该宏块的量化参数。为了使分析结果更加直观，每个宏块被标记以不同灰度的背景色，如图4-3（a）所示。量化系数越大，相应的背景色的灰度越浅。此外，也可以以视频帧的内容为背景显示分析结果，如图4-3（b）所示。也可以去掉量化系数的显示，如图4-3（c）所示。



![](https://img-blog.csdn.net/20140625233712171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（a）背景为灰度图，前景为量化参数

![](https://img-blog.csdn.net/20140625233728343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（b）背景为帧内容，前景为量化参数

![](https://img-blog.csdn.net/20140625233745218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（c）背景为灰度，前景不包含数字

图4-3.量化参数分析结果



宏块类型分析的结果如图4-4所示。视频帧同样被划分成以宏块为单位的网格状。根据子宏块划分方式的不同（16x16，16x8，8x16，8x8，4x4），这些宏块被划分成了不同样式。不同的宏块可以被标记以不同的背景颜色，如图4-4（a）所示。此外，如果该宏块属于跳过宏块，还可以在宏块上面标记以“s”字样。此外，也可以以视频帧的内容为背景显示分析结果，如图4-4（b）所示。

![](https://img-blog.csdn.net/20140625233803218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（a）背景为纯色图，前景为宏块类型

![](https://img-blog.csdn.net/20140625233821359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（b）背景为帧内容，前景为宏块划分方式

图4-4.宏块类型分析结果



运动矢量分析的结果如图4-5所示。运动矢量分为List0和List1两种。由图可见，运动较剧烈的地方，包含了较多的长度较长的运动矢量。画面相对静止的地方，运动矢量的长度相对比较短甚至取值为0。

![](https://img-blog.csdn.net/20140625233837593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（a）List0

![](https://img-blog.csdn.net/20140625233849484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（b）List1

图4-5. 运动矢量分析结果



参考帧的分析结果如图4-6所示。参考帧分为List0和List1两种。由图可见，不同的宏块参考了不同的视频帧作为其参考帧。

![](https://img-blog.csdn.net/20140625233901656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（a）List0

![](https://img-blog.csdn.net/20140625233914968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（b）List1

图4-6. 参考帧分析结果

### 4.3视频非压缩域分析

在视频播放的过程中，单击主界面视频参数部分的按钮“非压缩域数据分析”，打开非压缩域数据分析对话框，如图4-7所示。由图可见，可以从“分析方法”里面选择想要分析的内容，包含了颜色直方图，Canny边缘检测，轮廓检测，离散傅里叶变换，人脸识别。此外，还可以查看视频的R,G,B分量以及Y,U,V分量。如果勾选了“随着播放自动分析” 选项的话，可以随着系统对视频的解码播放，实时的分析视频的非压缩域数据。

![](https://img-blog.csdn.net/20140625234115000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4-7.非压缩域分析

颜色直方图的分析结果如图4-8所示。由图可见，颜色直方图列出了不同色彩在整幅视频帧中所占的比例。对于该视频帧来说，红色和黄色分量取值较大，代表该种色彩所占比重较大。

![](https://img-blog.csdn.net/20140625234136593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4-8.颜色直方图



边缘检测的分析结果如图4-9所示。通过该分析功能可以获得视频帧的边缘信息。

![](https://img-blog.csdn.net/20140625234153062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4-9.边缘检测



轮廓检测的结果如图4-10所示。通过该分析功能可以获得视频帧的轮廓信息。

![](https://img-blog.csdn.net/20140625234210500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4-10.轮廓检测



离散傅里叶变换的分析结果如图4-11所示。左边的图像为相位谱，中间的图像为亮度图，右边的图像为幅度谱。

![](https://img-blog.csdn.net/20140625234230343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4-11.离散傅里叶变换



人脸识别的分析结果如图4-12所示。通过该功能可以分析出视频帧中的人脸信息。

![](https://img-blog.csdn.net/20140625234245312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4-12.人脸识别



R,G,B分量的分析结果如图4-13所示。由图可以查看R,G,B三个分量的取值情况。

![](https://img-blog.csdn.net/20140625234302437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4-13. R（左上），G（右上），B（下）分量



Y,U,V分量的分析结果如图4-14所示。由于Y:U:V取样格式是4：2：0的，所以U,V分量的分辨率是Y分量的一半。

![](https://img-blog.csdn.net/20140625234344375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图4-14. Y（左上），U（右上），V（下）分量



## 5音频分析

本章主要介绍和音频分析相关的功能。

### 5.1音频解码分析

在视频播放的过程中，单击主界面音频编码参数部分的按钮“音频解码分析”，打开音频解码分析对话框，如图5-1所示。
对话框左边是解码状态表，表中显示了音频码流的情况。每个音频帧对应一个柱状图。横坐标对应音频帧序号，纵坐标对应音频帧大小。
对话框右边对应的是帧列表。列表显示了以下信息：
*帧数
*帧大小
*PTS
*DTS




![](https://img-blog.csdn.net/20140625234405343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图5-1. 音频解码分析





## 6其他功能

本章主要介绍系统的一些其它的功能。

### 6.1多国语言的支持

本软件目前支持简体中文和英文两种界面。英文界面示例如图6-1, 图6-2, 图6-3, 图6-4所示。

![](https://img-blog.csdn.net/20140625234425843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图6-1. 主界面（英文）



![](https://img-blog.csdn.net/20140625234445578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图6-2. 视频解码分析（英文）



![](https://img-blog.csdn.net/20140625234502875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图6-3.视频帧解码分析（英文）



![](https://img-blog.csdn.net/20140625234520109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图6-4.视频非压缩域分析（英文）







### 6.2专用分析工具

专用分析工具还未加入。

### 6.3辅助工具

目前支持的辅助工具是MediaInfo，用于查看视频信息，如图6-5所示。




![](https://img-blog.csdn.net/20140625234557484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图6-5.MediaInfo界面







