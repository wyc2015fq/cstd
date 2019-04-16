# 我的开源项目：TS封装格式分析器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年01月14日 00:03:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：89









本文介绍一个自己的开源小项目：TS封装格式分析器。TS全称是 MPEG 2 Transport Stream（MPEG2 传输流），广泛用于广播电视系统，比如说数字电视，以及IPTV。我这个项目规模不大，主要可以用来学习TS封装格式结构。此外它还支持分离TS中的视频流和音频流。以及输出TS包的时间戳，PTS，DTS等。使用VC 2010的MFC开发完成。在对TS进行视音频分离的过程中，用到了一个Github开源小工程：tsdemux。

软件的exe以及源代码已经上传到了SourceForge上。支持中英双语，紧跟国际潮流~

项目地址：[http://sourceforge.net/projects/tsformatanalysis/](http://sourceforge.net/projects/tsformatanalysis/)

CSDN下载地址（程序+源代码）：[http://download.csdn.net/detail/leixiaohua1020/6845473](http://download.csdn.net/detail/leixiaohua1020/6845473)



![](https://img-blog.csdn.net/20140708233115113)



更新记录==============================

1.1版（2014.7.8）

* 更换了界面

* 原工程支持Unicode编码

* 支持中英文切换



**软件使用介绍**

软件的使用相当简单。

软件运行后，首先打开一个TS流文件。

单击“开始”，可以解析出一系列TS包，列表显示在软件右侧，不同种类的TS包被标记成了不同的颜色。

软件的左侧，显示了TS流的相关信息信息，还不是很完善，以后有机会继续升级。

![](https://img-blog.csdn.net/20140708232922062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

英文界面：

![](https://img-blog.csdn.net/20140708233223051?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果勾选上“输出视音频”的话，可以输出分离后的视频流和音频流。

如果勾选上“输出TS包信息至文件”的话，则可以将TS包的信息（Timestamp，PTS，DTS）输出至文本文件。

**软件源代码简析**

源代码方面和普通的MFC程序差不太多，懂得MFC的人应该很快就能看懂。唯一比较特殊的地方，就在于对开源项目tsdemux进行了一个改动，在此就不细说了。注释方面做的还是比较充分的。








