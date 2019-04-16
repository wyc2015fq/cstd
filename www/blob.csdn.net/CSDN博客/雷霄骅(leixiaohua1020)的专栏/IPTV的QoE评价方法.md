# IPTV的QoE评价方法 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月18日 00:19:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：54
个人分类：[视频质量评价](https://blog.csdn.net/leixiaohua1020/article/category/1446693)










NTT公司的Keishiro Watanabe等人在论文《Proposal of new QoE assessment approach for quality management of IPTV services》中，提出了IPTV的QoE评价方法。现在摘录一下重点内容，以作备忘。

1.评价IPTV的QoE的几个点如下图所示：

![](https://img-blog.csdn.net/20131017231040218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从图中，可以看出一共有五个点可以评价IPTV的QoE：PT1，PT2，PT3，PT4，PT5。它们分别位于内容提供商（Content provider），服务提供商（Service provider），网络提供商（Network provider），以及用户终端（End user）之间。

2.这五个点（PT1，PT2，PT3，PT4，PT5）对不同种类的IPTV的QoE评价方式的支持如下表所示：

![](https://img-blog.csdn.net/20131017231333921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中媒体层（Media Layer）模型有三种评测方式：全参考（Full Reference），缩减参考（Reduced Reference），无参考（No Reference）。

包层（Parametric packet Layer）模型只有一种。

3.几种IPTV的QoE评价模型（媒体层模型，包层模型，以及新提出来的比特流层模型）提取参数的位置如下图所示：

![](https://img-blog.csdn.net/20131017232415921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

包层模型主要提取TS层以及其以上层的参数，比如一些包头信息。可以计算丢包率等参数。

媒体层模型提取视频解码数据，也就是解码后的YUV或RGB数据。可以计算块效应等参数。

比特流层模型（提出的模型）提取一些码流的参数，比如说QP等。可以根据这些参数推测视频质量。

4.该论文给出的IPTV的视频质量评价的公式如下所示：

![](https://img-blog.csdn.net/20131017234204750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

该公式主要通过QPmin（即QP平均值）估测视频质量DMOS（该值越低，人眼主观感受越好）

5.传统模型预测的视频质量与实际主观视频质量之间的关系如下图所示（一共使用了8个不同场景的序列作为测试序列）。

![](https://img-blog.csdn.net/20131017234223562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

6.同时该论文给出了QPmin（QP平均值）和视频质量（DMOS）之间的关系式。由图可以明显看出，QPmin越低，视频质量越高。

![](https://img-blog.csdn.net/20131017234300781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

7.论文提出的新模型预测的视频质量与实际主观视频质量之间的关系如下图所示（一共使用了8个不同场景的序列作为测试序列）。



![](https://img-blog.csdn.net/20131017234306421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以看出，所有的点很明显集中在一条线上，比传统模型的均方误差要小很多。因而有比传统模型更好的性能。



论文地址：[http://ieeexplore.ieee.org/xpl/articleDetails.jsp?reload=true&tp=&arnumber=4712191&queryText%3D(iptv+quality+of+service)%26openedRefinements%3D*](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?reload=true&tp=&arnumber=4712191&queryText%3D(iptv+quality+of+service)%26openedRefinements%3D*)




