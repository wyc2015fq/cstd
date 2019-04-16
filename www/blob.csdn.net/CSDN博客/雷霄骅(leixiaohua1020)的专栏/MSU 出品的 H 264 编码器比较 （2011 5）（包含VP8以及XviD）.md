# MSU 出品的 H.264 编码器比较 （2011.5）（包含VP8以及XviD） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月23日 23:10:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：41









这是著名的视频质量评价实验室MSU Graphics&Media Lab（是莫斯科国立大学的实验室）做的编码器比较的实验报告《Seventh MPEG-4 AVC/H.264 Video Codecs Comparison》。非常的详细，全面，在此翻译一下，以作备忘。

第一次发现编码器竟然还可以从这么多方面进行对比。

他们LOGO挺有特点的，是一只大眼睛。

![](https://img-blog.csdn.net/20130923024739921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



注：MSU出品的报告包含2种版本：免费版（Free version）和企业版（Enterprise version），免费版使用的评价方法相对较少，企业版使用的方法则比较多。详细的差别如下。

免费版包含的评价算法:
 SSIM (Y component)
 PSNR (Y component)
企业版此外还包含的评价算法:
 SSIM, Y-SSIM, U-SSIM, V-SSIM
 PSNR, Y-PSNR, U-PSNR, V-PSNR
 MSE
 3-SSIM
 MS-SSIM



参加测试的编码器包括：

• H.264
 DivX H.264
 Elecard H.264
 Intel SandyBridge Transcoder (GPU encoder)
 MainConcept H.264 (software)
 MainConcept H.264 (CUDA based encoder)
 Microsoft Expression Encoder
 DiscretePhoton
 x264
• 非 H.264
 VP8 (WebM project)
 XviD (MPEG-4 ASP codec)

这次评测不仅考虑了主流的H.264编码器，而且也考虑了基于其他编码标准的VP8和XviD。

在此只考虑分辨率为720P（1280x720）的视频会议（Video Conferences）情况，否则内容太多。。。

下表列出了参与评测的全部视频序列。全部参与评价的视频序列可以分成3类：视频会议（Video Conferences），电影（Movies），高清电视（HDTV）。

![](https://img-blog.csdn.net/20140810234748237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面例举视频会议序列720P条件下的实验结果。

### 一.RD曲线（RD Curves）

RD曲线（率失真曲线）是视频质量评价中最常见的一种曲线。横坐标是码率，纵坐标是视频客观算法的结果（例如PSNR或者SSIM）。下图的纵坐标取的是SSIM。可以看出X264的性能最为强大。XviD由于是基于上一代视频编码标准MPEG4，因此相同码率的情况下，视频质量最差。此外，可以发现VP8的性能也还不错，和很多H.264编码器的性能在伯仲之间。

![](https://img-blog.csdn.net/20130923230018640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 二.编码速度（Encoding Speed）

编码速度图表横坐标是编码视频的码率，纵坐标是编码速度（在这里是每秒编码的帧数）。所有的编码器随着编码码率的提高，编码速度都会有所下降。也可以看出X264和VP8的速度相对都是比较慢的。Discrete Photon速度最快。

![](https://img-blog.csdn.net/20130923230241593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 三.速度/质量权衡（Speed/Quality Trade-Off）

一般情况下，编码速度越快，视频的质量相对越差。反过来，如果要求编码出来的视频质量较高，那么通常编码速度也会相对较慢。下图列出了编码的速度和质量的关系图。这张表的横坐标和纵坐标都是一个相对值。在这里使用XviD作为参考编码器。即XviD在这张表的比啊那么速度和编码时间都是1，即位于（1,1）点。可以看出，X264是速度慢，质量好；而Discrete Photon是速度快，质量相对较差。VP8在速度和质量上都不占优势。

![](https://img-blog.csdn.net/20130923230429890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 四.码率控制（Bitrate Handling）

这张图表用于测试编码器的码率控制性能。码率控制性能好的编码器，编码后输出的视频码率和编码前设定的码率相同。码率控制性能差的编码器，编码后输出的视频码率和编码前设定的码率差别会很大。图表中纵坐标是实际码率与设定码率的比值。这个值为1，代表码率控制准确。从图中可以看出除了XviD在200kbps以下的低码率上码率控制非常不准之外。大部分编码器的码率控制都十分的准确。

![](https://img-blog.csdn.net/20130923230505296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 五.相对质量分析（Relative Quality Analysis）

下列这张表列出了在相同的视频质量的前提下，不同的编码器需要使用的码率。


![](https://img-blog.csdn.net/20130923230715421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们从中可以看出，VP8编码方式需要约213%的数据量才能编码获得和x264视频质量相同的视频。可见VP8和X264之间确实差距还是不小的。

下图以图形化的方式列出了上面那张表。不同颜色的线代表不同的编码器。纵坐标代表达到和横坐标编码器相同的视频质量需要的相对码率。

![](https://img-blog.csdn.net/20130923230758171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

最后列一下以上这些图表使用的测试视频序列的信息。


|名称|Conference 720p|
|----|----|
|分辨率|1280x720|
|帧数|1500|
|颜色空间|YV12|
|帧率|30|
|扫描方式|逐行扫描|



![](https://img-blog.csdn.net/20140811001717155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



原文地址：[http://www.compression.ru/video/codec_comparison/h264_2011/mpeg-4_avc_h264_video_codecs_comparison.pdf](http://www.compression.ru/video/codec_comparison/h264_2011/mpeg-4_avc_h264_video_codecs_comparison.pdf)




