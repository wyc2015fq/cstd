# 限制码率的视频编码标准比较（包括MPEG-2，H.263， MPEG-4，以及 H.264） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月18日 14:40:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：106









Thomas Wiegand等人在论文《Rate-Constrained Coder Control and Comparisonof Video Coding Standards》中对比了几种编码器的效率，包括MPEG-2，H.263， MPEG-4，以及H.264/AVC。在此记录一下关键实验数据。

**总体结论：**

同等码率的前提下，各种标准的视频质量如下所示：

**MPEG-2<H.263<MPEG-4<H.264/AVC**

**流媒体方面的应用的比较**

实验数据如表所示（貌似这样看比较抽象...，实验的数据量实在太大了）。一共四种编码标准，每种标准都测了PSNR-Y，PSNR-U，PSNR-V三种参数。

![](https://img-blog.csdn.net/20131018141315984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



实验数据如图所示（这样看还是比较形象的）。左边的图是比特率和PSNR关系图。右边的图是其他三种编码标准相对于MPEG2节约的码率的关系图。

![](https://img-blog.csdn.net/20131018141358375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

平均节约的码率。例如H.264相对于MPEG4-ASP节约了37.44%的码率，相对于H.263-HLP节约了47.58%的码率，相对于MPEG2节约了63.57%的码率。

![](https://img-blog.csdn.net/20131018141423625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**视频会议**方面的**应用的比较**

实验数据如表所示。一共四种编码标准，每种标准都测了PSNR-Y，PSNR-U，PSNR-V三种参数。

![](https://img-blog.csdn.net/20131018141507843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



和第一组实验结果图类似。左边的图是比特率和PSNR关系图。右边的图是其他三种编码标准相对于MPEG2节约的码率的关系图。

![](https://img-blog.csdn.net/20131018141448812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

平均节约的码率。例如H.264相对于MPEG4-ASP节约了27.69%的码率，相对于H.263-HLP节约了29.37%的码率，相对于MPEG2节约了40.59%的码率。

![](https://img-blog.csdn.net/20131018141458000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**娱乐方面的应用的比较**

结果如图所示，在这一环节只有两种标准：MPEG2和H.264。

![](https://img-blog.csdn.net/20131018141520734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



论文地址：[http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=1218200](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=1218200)












