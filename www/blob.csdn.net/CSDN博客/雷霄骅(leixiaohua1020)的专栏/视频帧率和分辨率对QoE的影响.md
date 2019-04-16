# 视频帧率和分辨率对QoE的影响 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月18日 13:06:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：31
个人分类：[视频质量评价](https://blog.csdn.net/leixiaohua1020/article/category/1446693)










Thomas Zinner等人在《Impact of Frame Rate and Resolution on ObjectiveQoE Metrics》（QoMEX 2010）论文中，研究了视频帧率和分辨率对QoE的影响。在此摘录出其中重要的数据，很有参考价值，以作备忘。

首先，该论文提出了影响视频QoE的三个因素：分辨率，帧率，图像质量。如图所示。



![](https://img-blog.csdn.net/20131018121215593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这三个因素是相互独立的，而且其中的任意一个都不能太低，否则就会影响到视频的QoE。

该论文中使用到了H.264/SVC（H.264可分级编码）的编码方式。H.264/SVC可以实现编码视频时间上（帧率）、空间上（分辨率）可扩展，以及图像质量方面的可扩展，可以产生不同帧速率、分辨率或质量等级的视频。如下图所示：



![](https://img-blog.csdn.net/20131018121809656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

该论文指出了视频客观质量算法（SSIM，VQM）和视频主观QoE（MOS）之间的之间的关系。SSIM和VQM取值都在0-1之间。SSIM值越高，视频质量越好，VQM取值越高，则视频质量越差。

> 
这里可以参考文章：


> 
1.[全参考视频质量评价方法（PSNR，SSIM）以及相关数据库](http://blog.csdn.net/leixiaohua1020/article/details/11694369)

2.[视频质量评价方法：VQM](http://blog.csdn.net/leixiaohua1020/article/details/12685297)


举个例子一个视频如果它的SSIM取值在0.96，我们就可以认为它的质量达到了4（good）的程度。



![](https://img-blog.csdn.net/20131018122122312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

该论文使用的视频测试序列（blue sky，crowd run，park joy）以及其特性如下表所示：





![](https://img-blog.csdn.net/20131018123101953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

测试序列选自xiph.org。JSVM版本是9.15。基本层（base layer）分辨率为480x270，帧率为1.875fps。增强层（extension layers）包含以下几种帧率：3.75, 7.5, 15 30；以及以下几种分辨率：640x360， 960x540，1216x684。

**不同分辨率对视频QoE的影响如图所示。**左边的图是使用SSIM计算出来的结果，右边的图是使用VQM计算出来的结果。

图中横坐标Scale的意思是当前分辨率（像素数）除以最大分辨率（像素数）的值。例如Scale为0.5的时候，当前的分别率为860x484，其像素数416240为最大分辨率1216x684的像素数831744的一半。由图可见随着分辨率的下降，视频质量也在下降，在分辨率较低的时候，下降尤为明显。

分辨率变换的时候有两种插值方法：邻域像素插值，双三次差值。由图可见，双三次差值的方法要明显好于邻域像素插值的方法。



![](https://img-blog.csdn.net/20131018123344218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**不同帧率对视频QoE的影响如图所示。**由图可见，随着帧率的下降，视频质量逐渐下降（注：这一块还没弄明白是怎么回事）



![](https://img-blog.csdn.net/20131018125744000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**带宽，帧率，分辨率之间的权衡结果如下图所示。**横轴（x轴）代表带宽相对于视频最高质量节约的程度。

从这张图可以明显得出一个结论：如果想节约带宽，一定要优先考虑降低分辨率而不是帧率，否则就会严重影响到视频质量。



![](https://img-blog.csdn.net/20131018125530843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

论文地址：[http://ieeexplore.ieee.org/xpl/articleDetails.jsp?reload=true&tp=&arnumber=5518277&contentType=Conference+Publications&sortType%3Dasc_p_Sequence%26filter%3DAND(p_IS_Number%3A5514883)](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?reload=true&tp=&arnumber=5518277&contentType=Conference+Publications&sortType%3Dasc_p_Sequence%26filter%3DAND(p_IS_Number%3A5514883))




