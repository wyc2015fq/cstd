# Web服务中延时对QoE（体验质量）的影响 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月24日 14:31:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：119
个人分类：[视频质量评价](https://blog.csdn.net/leixiaohua1020/article/category/1446693)










S. Egger等人在论文《WAITING TIMES IN QUALITY OF EXPERIENCE FOR WEB BASED SERVICES》中，研究了Web服务中延时对主观感受的影响。在此记录一下其重点内容。

注：这篇文章很有参考价值。

首先需要明确的是，该论文所讨论的是关于web服务的（而不是针对视频的）。

web服务的原理如下图所示。

![](https://img-blog.csdn.net/20131108173001765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



人的主观感受和客观的时间有的时候是不一致的。如下图所示。

![](https://img-blog.csdn.net/20131108173006187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



有关人类主观响应时间有如下研究成果：

0.1 s： 是用户感觉不到系统不连贯性的上限
 1.0 s： 是用户的思路不被打扰的上限，尽管用户会注意到延迟
 10 s： 是用户保持对对话框的注意力的上限

（怎么感觉翻译得怪怪的...）

**冲浪VS. 等待： WEB QOE**



**页面加载时间 vs带宽**

下图反映了页面加载时间和主观感受之间的关系。可以看出相同的页面加载时间的前提下，用户对不同服务的主观感受是不一样的。对于搜索，照片服务，用户可以忍受更长时间的等待。

![](https://img-blog.csdn.net/20131108173011890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图反映了带宽和主观感受之间的关系。可以看出，当带宽达到512kbps之后，用户主观感受的提升已经不明显了。

![](https://img-blog.csdn.net/20131108173014843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这张图很有意思。纵坐标是实际页面加载时间与主观页面加载时间之间的比值。横坐标是不同的页面：一共6家服务提供商，每家选择了2-3个页面（例如对Amazon来说，就是首页, 搜索结果以及商品详细信息页面）。可以看出，不同的页面，这个比值是不同的，范围从1.3到3（按照常理说，这个值是1应该是最客观的）。

注：PTL —— Page Load Time

![](https://img-blog.csdn.net/20131108173019437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图显示了下载文件时间与主观感受之间的关系。这个实验在2009和2011年被做过两次。可以看出，在下载时间相同的情况下，小文件下载给用户的主观感受要差一些。其实这个结果是众所周知的，但是很少有人量化研究。

![](https://img-blog.csdn.net/20131108173022953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图显示了等待时间与主观感受之间的关系。横坐标是等待时间（单位是秒）。可以看出对于不同的Web服务，相同的等待时间可能给人的主观感受不一样。比如说同样等待8秒，Email附件的MOS值比较高，而网页服务的MOS值就比较低。

![](https://img-blog.csdn.net/20131108173025984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图显示了影响Youtube的QoE最主要的一些因素。

![](https://img-blog.csdn.net/20131108173030203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图显示了Youtube视频播放过程中暂停（stalling）次数和主观感受之间的关系。黑色的线指的是暂停时长为1s的情况。黄色的线指的是暂停时长为3s的情况。

可以看出，用户对于暂停2次以上的情况都会比较不满意（MOS低于3.5），每次暂停的时间较长的话（3s）则会更加不满。

![](https://img-blog.csdn.net/20131108173034250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
















