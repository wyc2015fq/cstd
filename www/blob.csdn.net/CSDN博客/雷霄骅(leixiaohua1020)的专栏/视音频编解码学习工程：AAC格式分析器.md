# 视音频编解码学习工程：AAC格式分析器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年01月13日 00:42:15[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：20922










=====================================================
视音频编解码学习工程系列文章列表：

[视音频编解码学习工程：H.264分析器](http://blog.csdn.net/leixiaohua1020/article/details/17933821)

[视音频编解码学习工程：AAC格式分析器](http://blog.csdn.net/leixiaohua1020/article/details/18155549)

[视音频编解码学习工程：FLV封装格式分析器](http://blog.csdn.net/leixiaohua1020/article/details/17934487)

[视音频编解码学习工程：TS封装格式分析器](http://blog.csdn.net/leixiaohua1020/article/details/17973587)

[视音频编解码学习工程：JPEG分析器](http://blog.csdn.net/leixiaohua1020/article/details/18280253)


=====================================================

本文介绍一个自己的开源小项目：AAC格式分析器。AAC全称是Advanced Audio Coding，是互联网上使用极为广泛的音频编码格式，也是当前性能差不多最好的音频编码格式。现有的互联网上的视频很大一部分都是使用AAC进行音频编码的。使用VC 2010的MFC开发完成。开发的时候使用了一个AAC的开源解码器faad2。目前软件的的分析功能还不是很强，提取了AAC解码过程中的一些参数。以后有时间进一步完善相关的功能。

软件的exe以及源代码已经上传到了SourceForge上。

软件SourceForge地址：[https://sourceforge.net/projects/aacstreamanalysis/](https://sourceforge.net/projects/aacstreamanalysis/)

CSDN下载地址（程序+源代码）：[http://download.csdn.net/detail/leixiaohua1020/6841555](http://download.csdn.net/detail/leixiaohua1020/6841555)


![](https://img-blog.csdn.net/20140708232847162)




更新记录==============================

1.1版（2014.7.8）

 * 更换了界面

 * 原工程支持Unicode编码

 * 支持中英文切换

 * 增加了一个解码进度条


CSDN源代码：[http://download.csdn.net/detail/leixiaohua1020/7767701](http://download.csdn.net/detail/leixiaohua1020/7767701)
PUDN源代码：[http://www.pudn.com/downloads644/sourcecode/multimedia/audio/detail2605184.html](http://www.pudn.com/downloads644/sourcecode/multimedia/audio/detail2605184.html)




新版（2016.1.1）

 * 修正少量界面UI问题

 * 去掉了faad库，使用纯粹的C语言代码解码

 * 添加了AAC音频帧列表

已经更新至SourceForge上





**软件使用介绍**

软件的使用相当简单。

软件运行后，首先打开一个AAC文件（或者是M4A文件）。

注：AAC文件就是不经封装的音频流。M4A文件就是以MPEG-4标准的封装格式进行封装的AAC。

单击“开始”，软件就会开始解码AAC音频文件，右边显示了解码的进程以及音频帧信息，左边显示了解码过程中提取出来的一些信息。

在软件的左下角，可以设置输出音频原始采样数据的格式（PCM或者WAV）。

![](https://img-blog.csdn.net/20160313003527127)





**软件源代码简析**

源代码方面和普通的MFC程序差不太多，懂得MFC的人应该很快就能看懂。唯一比较特殊的地方，就在于对开源项目faad2进行了一些改动，在此就不细说了。很多地方都做了注释。






