# ffmpeg转码器移植VC的工程：ffmpeg for MFC - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年06月07日 23:59:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：46
个人分类：[FFMPEG																[我的开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1843731)](https://blog.csdn.net/leixiaohua1020/article/category/1360795)









本文介绍一个自己做的FFMPEG移植到VC下的开源工程：ffmpeg for MFC。本工程将ffmpeg工程中的ffmpeg转码器（ffmpeg.c）移植到了VC环境下。并且使用MFC做了一套简单的界面。它可以完成一个视频转码的基本功能：封装格式以及编码格式的转换。通过本程序可以学习视频转码器的原理，以及Windows的消息机制等知识。

![](https://img-blog.csdn.net/20140607235912406)

平台：VC2010

类库：FFmpeg，MFC

目前正在完善中，先不上传到CSDN下载了。在SourceForge上建立了一个项目。

SourceForge项目主页：

[https://sourceforge.net/projects/ffmpegformfc](https://sourceforge.net/projects/ffmpegformfc)



更新记录==============================

1.1版（2014.7.12）

* 更换了部分界面

* 原工程支持Unicode编码

* 修复了一部分内存泄露的问题

* 修正了ffmpeg.c中的exit_program()函数。保证在转码结束后可以释放资源。

* 修正了一部分“任务列表”的功能

* 修正了添加任务的时候“要用按钮去选输入文件，不能自己直接输入”的问题。



1.2版（2014.8.16）

*增加了“内部转码”和“外部转码”选项。“内部转码”即通过原先移植的ffmpeg.c进行转码。“外部转码”支持作为一个GUI直接调用ffmpeg.exe进行转码。



发现SourceForge速度很慢，有的时候几乎访问不了，因此还是打算在CSDN上上传了一下源代码和编译后的可执行程序。结果昨天传了一晚上竟然没有传上去。。。每次上传完在自己资源列表里面都看不到，很是郁闷。结果今天早上起来猛然发现资源列表里面竟然有好几份。。。这个延迟有点大啊，不过好歹传上去了。

地址：[http://download.csdn.net/detail/leixiaohua1020/7764529](http://download.csdn.net/detail/leixiaohua1020/7764529)





![](https://img-blog.csdn.net/20140816011535523)



## **一.主界面**

程序的主界面如图所示。主界面中包含一个任务列表，其中包含了需要转码的任务。下方是转码的控制按钮，并且包含了转码的进度信息。

![](https://img-blog.csdn.net/20140607234653500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **1.1.任务列表**

任务列表包含了需要转码的任务。列表的上方包含了4个按钮，包含了对任务列表的4种操作：添加，编辑，删除，清空。此外，在任务列表中选择一个任务然后单击右键，也可以在右键菜单中选择这4项操作。这4项操作如下：

添加：新增加一个任务。

编辑：修改现有的一个任务。

删除：删除一个任务。

清空：删除所有的任务。

列表显示了任务的信息。包含：序号，输入，大小，输出，状态。含义如下：

序号：编号。

输入：输入文件名称。

大小：输入文件大小。

输出：输出文件名称。

状态：该任务所处的状态（准备，处理中，已完成）。

## **1.2.控制按钮**

控制按钮完成了对转码过程的控制功能。包含：开始，暂停，停止。

开始：系统开始转码。

暂停：系统暂停转码。

停止：系统停止转码。







## **二.任务信息窗口**

任务信息窗口用于配置一条任务信息。其中包含了三个部分：输入和输出，输入文件信息，输出配置。

![](https://img-blog.csdn.net/20140606001450984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### **2.1.输入和输出**

输入和输出部分用于指定该任务的输入文件和输出文件。输入文件即准备进行转码的文件，输出文件即转码后生成的文件。可以直接在输入框中输入文件路径，也可以单击输入框右侧的“文件”按钮选择文件。此外还可以直接将文件拖拽至对话框的方式选择输入文件。

选择输入文件后，程序会检查输入文件的格式信息。如果输入文件不是视音频文件，程序会报出错误信息。如果输入文件为视音频文件，程序会检测该文件的参数信息，包括封装格式，视频编码，音频编码等信息。并将检测结果显示在“输入文件信息”部分。

为了方便起见，选择输入文件后，程序会在输入文件的目录下生成名为“{输入文件名称}_ffmfc.mkv”的输出文件。可以对该输出文件路径和名称进行更改。



### **2.2.输入文件信息**

包含：封装格式，视频比特率，时长，输出像素格式，视频编码方式，帧率，画面大小，采样率，音频编码方式，声道数。





### **2.3.输出配置**

**封装格式**

输入封装格式：强制指定输入文件的格式。默认情况下ffmpeg是通过输入文件的后缀判断文件格式的。如果指定了这个选项，则按照该指定的格式对文件进行解析。

输出封装格式：强制指定输出文件的格式。默认情况下ffmpeg是通过输出文件的后缀判断文件格式的。如果指定了这个选项，则按照该指定的格式对文件进行转码。

**视频**

比特率：视频的码率，默认800kbps。

编码器：视频编码使用的编码器。

帧率：视频编码使用的帧率，默认25fps。

**音频**

比特率：音频的码率，默认64kbps。

编码器：音频编码使用的编码器。

采样率：音频的采样率，默认44100Hz。

声道数：音频的声道数，默认是2。





输出的封装格式，视频编码格式，音频编码格式以列表的形式显示出来，可以自行选择。



![](https://img-blog.csdn.net/20140606001608218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140606001613125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20140606001617812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## **三.系统支持信息窗口**

系统支持信息窗口列表显示了系统的支持信息。包括系统支持的：协议，输入封装格式，输出封装格式，视频解码器，音频解码器，以及其他解码器（字幕解码器）。例如从视频解码器列表中可以看出本程序支持的视频编码器包含libx264，libvpx等等。

![](https://img-blog.csdn.net/20140606001719203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## **四.菜单**

程序上端的菜单中包含了更丰富的选项。一共分成5部分的功能：**任务，控制，视图，语言，帮助**。下面分别介绍。

**任务**：对任务列表进行添加，编辑，删除，清空操作。

![](https://img-blog.csdn.net/20140606001725078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





**控制**：控制转码的开始，暂停，停止。

![](https://img-blog.csdn.net/20140606001728625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**语言**：设置界面的语言。

![](https://img-blog.csdn.net/20140606001732078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## **五.英文界面**

英文界面截图如下所示。

主界面

![](https://img-blog.csdn.net/20140607234757953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



任务信息窗口

![](https://img-blog.csdn.net/20140606001903281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



系统支持信息窗口



![](https://img-blog.csdn.net/20140606001907656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)









