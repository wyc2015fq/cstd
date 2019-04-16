# 基于RTMP的实时流媒体的QoE分析 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月11日 19:16:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：49









Holly French等人在论文《Real Time Video QoE Analysis of RTMP Streams》中，研究了基于RTMP的实时视频的QoE。在此记录一下。



他们的研究结果表明，码率（bitrate）与帧率或者带宽结合，可以相对准确的反映RTMP视频流的QoE。

他们的实验设计如下图所示。分析服务器包含质量分析器以及相应的数据库。web服务器提供了显示视频的页面。Flash流媒体服务器是提供视频源。Flash流媒体服务器和客户端之间有一个网络模拟器，可以模拟网络上的丢包和延时。

![](https://img-blog.csdn.net/20131107152822812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

实验一共有10人参加，平均每人观看10个视频。测试序列如下表所示：

![](https://img-blog.csdn.net/20131111185606250)

引入的丢包率在0-15%，时延在0-100ms。

实验的结果如下图所示。横坐标为3个测试序列，其中每个序列都通过不同的指标预测RTMP流的QoE。纵坐标为精确度。

从实验的结果来看，对于高清晰度的视频，使用带宽+码率（BW+BR）预测QoE的精确度能达到80%。

对于标准清晰度的视频，使用码率+帧率（BR+FR）或者单独使用码率预测QoE的精确度能达到70%。

最终可以得出结论：码率（bitrate）与帧率或者带宽结合，可以相对准确的反映RTMP视频流的QoE。

![](https://img-blog.csdn.net/20131107152828500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)














































