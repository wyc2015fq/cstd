# HEVC，VP9，x264性能对比 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年02月09日 20:35:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：46









Dan Grois等人在论文《Performance Comparison of H.265/MPEG-HEVC, VP9, andH.264/MPEG-AVC Encoders》中，比较了下一代编码标准HEVC，VP9的以及当前主流编码标准H.264之间的性能。在此记录一下。

本文中选用的编码器如下：

> 
HEVC：HM

VP9：libvpx

H.264：x264


下表显示了HM的参数配置。

![](https://img-blog.csdn.net/20140209200707734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下表显示了VP9和x264的参数配置。

![](https://img-blog.csdn.net/20140209200712078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

本文中PSNR取值取的是YUV的PSNR。是由Y，U，V三个分量的PSNR计算得到的。

![](https://img-blog.csdn.net/20140209200715250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

使用的测试序列如下表所示。

![](https://img-blog.csdn.net/20140209200719140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

实验结果如下图所示。这个实验结果还是让我有点意外的。HEVC性能最强并没有什么好奇怪的，奇怪的是x264的性能竟然好于VP9。要知道VP9可是Google推出的下一代编码标准。x264太强悍了！

PS：此外，x264的速度是远远高于HEVC和VP9的。

![](https://img-blog.csdn.net/20140209201030015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下表显示了HEVC在同等质量的前提下（以PSNR为依据），相对于VP9和x264节约的码率。下表显示了所有序列的情况。总体来说HEVC相对于VP9节约了41.9%，HEVC相对于x264节约了38.9%。

![](https://img-blog.csdn.net/20140209200726296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下表显示了三种编码器整体性能的比较。表中百分比数字的意义是：同等视频质量的前提下，该列所属的编码器相对于该行所属的编码器节约的码率，如果为负值，则代表反而消耗了更多的码率。例如，同等质量的前提下，x264相对于VP9节约了8.4%的码率。

![](https://img-blog.csdn.net/20140209200729437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

同等视频质量的前提条件下，编码消耗时间对比如下表所示。可以看出，VP9编码时间大约是x264的130倍。HEVC编码时间大约是VP9的7倍。

![](https://img-blog.csdn.net/20140209202623312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






