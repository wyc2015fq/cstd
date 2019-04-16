# ITU-T Technical Paper： 测量QoS的基本网络模型 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月30日 18:44:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：36
个人分类：[网络](https://blog.csdn.net/leixiaohua1020/article/category/8433201)










本文翻译自ITU-T的Technical Paper：《How to increase QoS/QoE of IP-based platform(s) to regionally agreed standards》(2013/3/1)。这是其第六章的一部分，给出了测量QoS的基本网络模型。PS：在此感谢一位师弟的翻译。



理想情况下，为每个客户的流量进行的测试都应该在相同的端点之间进行。无论这些终端是客户终端（TE）、客户边缘路由（CE）还是供应商边缘路由（PE），测量的数目都会大到难以实现。因此，对于一个实际的解决方案，需要将网络分割成一个测量网络模型。一个区段探针的单一测量结果的杠杆作用越强，需要的探针数就越少。如果在成千上万的评估计算中用到的区段测量越少，总体的探头开销就会越低。

供应商在不同的端点之间提供的保证交付服务有如下几种情况：
***边到边（Edge-Edge）**: 扩展到一个供应商网络的边缘；
***站点到站点（Site-Site）**:扩展到客户端的边缘；
***终端到终端（TE-TE）**:扩展到一个客户的终端。

用于测量的网络模型被分为多个网段，每个网段被独立的监控并且应该支持这三种服务情况。一般来说，网络被认为是由入口和出口接入段和一个传输段组成的。人们认为一个区域服务供应商会提供同时支持一个特定站点的入口和出口段的接入网络。在区域服务供应商之间会有一个骨干服务供应商提供传输服务。一个具体的服务供应商可以同时扮演流量接入供应商和流量传输供应商的角色，也可以扮演其中之一。考虑到所有这些因素，下面的图12展示了QoS测量的基本网络模型。



![](https://img-blog.csdn.net/20131029155415281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图12 – 基本网络模型



此外, 图 13展示了网络模型的IP网络接入部分。在从核心网到客户端的下游方向，连接有一系列的网络元件和线缆：边缘路由器、DSLAM （或 GPON OLT）、 DSL 调制解调器（或 GPON ONT ）、防火墙和路由器。这一模型是双向的，所以上游方向的元件顺序与下游相反。



![](https://img-blog.csdn.net/20131029155419265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图 13 – 接入网部分






