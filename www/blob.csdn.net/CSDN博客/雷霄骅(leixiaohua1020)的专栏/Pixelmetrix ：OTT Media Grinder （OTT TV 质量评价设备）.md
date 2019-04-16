# Pixelmetrix ：OTT Media Grinder （OTT TV 质量评价设备） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月16日 14:09:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：29
个人分类：[视频质量评价](https://blog.csdn.net/leixiaohua1020/article/category/1446693)










有关OTT TV 质量评价方法方面的研究少之又少。国内貌似还几乎没有相关的研究。不过在国外已经找到相关的产品了，翻译了一下产品手册的部分内容，很有参考价值，尤其是其提出的8个指标。



概述

OTT Media Grinder这个设备其实主要是用于测试的。同时其中提供了一些算法进行质量评价。其使用方法如下图所示。

![](https://img-blog.csdn.net/20131107143039390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**传统的方法已经不再适用**

OTT TV是基于HTTP的，而HTTP是基于TCP的。TCP传输数据的时候不会发生丢包。

传统的IPTV等QoS测量方法都注重丢包这类的指标，因而他们的测量方法已经不适用于OTT TV。

下表给出了OTT TV相对于传统IPTV的测量指标的变化：


|无意义的|不足的|
|----|----|
|视频• 丢包• 抖动• 数据包到达间隔时间• RTP 统计• IGMP 统计|HTTP• HTTP 发送流量• HTTP 接收流量• HTTP 错误统计- 超时- 4xx 错误- 5xx 错误|



**缓存模型 & ‘The Margin’（限度）**

由于客户端接收到的OTT TV数据可能并不是连续和匀速的，因而在接收OTT TV传送过来的数据的时候，客户端会先将数据存入一个缓存。这个缓存的原理有点像一个漏斗。从互联网上接收的数据就像一桶一桶的水，流量有时候大有时候小。而缓存输出数据的地方就像一个漏斗留出水的那个嘴。输出的数据就是连续而匀速的了。

当缓存里面的数据不足的时候，就有可能出现问题。这就像漏斗里面的水不够了，可能就不能保证匀速的流出水来了。这时候客户端就会出现暂停现象（播放器上显示“缓冲中...”）。



![](https://img-blog.csdn.net/20131107143042484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图是一个指标测量的界面截图。

![](https://img-blog.csdn.net/20131107143045875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**VideoMargin™指标**

针对于OTT TV，一共提出了8个指标，这些指标既可以用于仿真模拟，又可以用于实时监控。

4个指标是关于传输网络的传输能力的。2个指标是关于自适应比特率的特性的。

这些指标的适用范围如下表所示。


|指标|源服务器|网络|客户端|全部QoE|
|----|----|----|----|----|
|Starvation Margin|X|X|X||
|Response Margin|X||||
|Delivery Margin||X|||
|Request Margin|||X||
|Quality Distribution||||X|
|Quality Stability||||X|
|License Requests|X||||


 下面详细介绍各种指标：

**Delivery Margin（传递）**

数据传输的速度有多快？

检测是否有足够的应用层的带宽。

**Request Margin（请求）**

检测是否客户端请求数据太晚了？

客户端算法决定了根据网络的特性应该何时发出新的请求。

**Response Margin（响应）**

服务器对视频请求的响应速度有多快？

较慢的响应速度可能会导致客户端收不到数据，从而“忍饥挨饿”。

**Starvation Margin（饥饿）**

客户端是不是“忍饥挨饿”了？

客户端缓存中可以播放的时长。越高越好。

**Quality Distribution（质量分布）**

高，中，低比特率的视频在全部视频中占的比例？

分别报告观看高，中，低比特率的客户端数。

**Quality Stability（质量稳定性）**

客户端改变比特率的次数？

监测平均每个用户变换比特率的次数。

**HTTP Response Codes（HTTP响应代码）**
 Web服务器是否正确的响应了HTTP请求？

监测Web服务器返回什么类型的错误。

**License Requests（许可证请求）**

测量新被授予的许可证与那些未授予的许可证之间的比例。（没懂）



**附上原文：**

![](https://img-blog.csdn.net/20131112161457781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20131112161503640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**中央面板（Central Dashboard）**

屏幕截图如下所示：

![](https://img-blog.csdn.net/20131107143049671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其他界面，不多介绍了：

![](https://img-blog.csdn.net/20131107143052703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**规格**


|**支持的协议**• Apple HTTP Live Streaming (HLS)**客户端最大数量**• 2000**最大负载**• Up to 990 Mbps**测量精度**• 1 minute**控制界面**• HTML Web Browser|**操作系统**• Linux**存储**• 250 GB SATA II HDD**系统接口**• Management Interface-1 Gbps Ethernet Interface-RJ-45• Copper Load Generating Interface-1 Gbps Ethernet Interface-RJ-45• System Recovery-USBMisc-USB 2.0|**机械特性**•1RU 19 inch rack-mountable•Operating Temperatures - +10˚C to +30˚C•Storage Temperature-0˚C to +50˚C**电气特性**• Power Input-90-240 V AC; 47-63 Hz-5A at 115 V AC-2.5A at 230 V AC max**法规遵守**• CE Mark s|
|----|----|----|










