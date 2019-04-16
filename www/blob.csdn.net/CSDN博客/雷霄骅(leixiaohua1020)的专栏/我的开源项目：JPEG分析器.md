# 我的开源项目：JPEG分析器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年01月15日 00:04:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：54
个人分类：[我的开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1843731)










本文介绍一个自己的开源小项目：JPEG分析器。JPEG全称是 Joint Photographic Experts Group（联合图像专家小组），是当今使用最为广泛的图像编码标准。我这个项目规模不大，主要可以用来学习JPEG编码标准。结合着源代码看的话效果比较好。它支持将JPEG解码为像素数据（YUV或者RGB）。同时支持输出DCT系数等参数。使用VC 2010的MFC开发完成。用到了开源工程：tinyjpeg。

软件的exe以及源代码已经上传到了SourceForge上。支持中英双语，紧跟国际潮流~

项目地址：[http://sourceforge.net/projects/jpeganalysis/](http://sourceforge.net/projects/jpeganalysis/)

CSDN下载地址（程序+源代码）：[http://download.csdn.net/detail/leixiaohua1020/6849669](http://download.csdn.net/detail/leixiaohua1020/6849669)

![](https://img-blog.csdn.net/20140708233330100)



更新记录==============================

1.1版（2014.7.8）

* 更换了界面

* 原工程支持Unicode编码

* 支持中英文切换（英文界面还未完成）





**软件使用介绍**

软件的使用相当简单。

软件运行后，首先打开一个JPEG文件。

单击“开始”，可以解析出JPEG文件头的各种标记，以及这些标记中的内容，显示在左侧。

软件的右侧，显示了解码的进程。

![](https://img-blog.csdn.net/20140708233136343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

时间仓促，英文的界面还没做完。

![](https://img-blog.csdn.net/20140708233435729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以选择输出每个8x8亮度块的DCT系数，以及每个8x8亮度块的亮度值。它们会被保存在TXT文本文件中。

**软件源代码简析**

源代码方面和普通的MFC程序差不太多，懂得MFC的人应该很快就能看懂。唯一比较特殊的地方，就在于对开源项目tinyjpeg进行了一个改动，在此就不细说了。注释很充分。






