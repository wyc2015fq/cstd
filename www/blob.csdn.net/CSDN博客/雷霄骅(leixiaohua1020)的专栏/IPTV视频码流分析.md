# IPTV视频码流分析 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月20日 14:39:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：39
个人分类：[IPTV](https://blog.csdn.net/leixiaohua1020/article/category/1638143)










去上海出差的时候，去同学家中采集了一部分百视通的IPTV码流。

具体的方法是通过在机顶盒和电视信号接口ONU之间增加一个镜像路由器，截取数据，再通过Wireshark软件或者自行开发的软件分析数据。

![](https://img-blog.csdn.net/20130920142939218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

WireShark抓取数据包：

![](https://img-blog.csdn.net/20130920143158640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

分析结果：

机顶盒

机顶盒实质上是一个网页浏览器，每次开机之后会访问固定的网页，而该网页就是机顶盒的开机界面。

传输方式：UDP组播



rtsp://124.75.34.37/PLTV/88888888/224/0/3221225489/000000000114.smil

rtsp://124.75.34.37/PLTV/88888888/224/0/3221225558/00000000015n.smil

...

数据格式：UDP/RTP/TS



![](https://img-blog.csdn.net/20130920143550812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

压缩编码参数：

比特率：



8 000 Kbps

视频 

 Format/Info: Advanced Video Codec

 Format profile :Main@L4.0

 Format settings, CABAC :Yes

 Format settings, ReFrames : 4 frames

 Bit rate : 7 411 Kbps

 Width : 1 920 pixels

 Height : 1 080 pixels

 Display aspect ratio :16:9

 Frame rate : 25.000 fps

 Color space : YUV

 Chroma subsampling :4:2:0

 Bit depth : 8 bits

 Scan type : Interlaced

音频 

Format : MPEG Audio

 Format version : Version1

 Format profile : Layer 2

 Bit rate mode : Constant

 Bit rate : 192 Kbps

 Channel(s) : 1 channel

 Sampling rate : 48.0 KHz

 Compression mode : Lossy


















