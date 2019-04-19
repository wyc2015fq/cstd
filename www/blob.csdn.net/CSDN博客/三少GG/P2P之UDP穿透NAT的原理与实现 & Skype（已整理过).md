# P2P之UDP穿透NAT的原理与实现 & Skype（已整理过) - 三少GG - CSDN博客
2012年11月26日 20:37:31[三少GG](https://me.csdn.net/scut1135)阅读数：3425
P2P 之UDP穿透NAT的原理与实现（附源代码）
原创：shootingstars
参考：[http://midcom-p2p.sourceforge.net/draft-ford-midcom-p2p-01.txt](http://midcom-p2p.sourceforge.net/draft-ford-midcom-p2p-01.txt)
论坛上经常有对P2P原理的讨论，但是讨论归讨论，很少有实质的东西产生（源代码）。呵呵，在这里我就用自己实现的一个源代码来说明UDP穿越NAT的原理。
首先先介绍一些基本概念：
    NAT(NetworkAddressTranslators)，网络地址转换：网络地址转换是在IP地址日益缺乏的情况下产生的，它的主要目的就是为了能够地址重用。NAT分为两大类:
          * 基本的NAT,和
          * NAPT(Network Address/Port Translator)。
   最开始NAT是运行在路由器上的一个功能模块。
   最先提出的是基本的NAT，它的产生基于如下事实：一个私有网络（域）中的节点中只有很少的节点需要与外网连接（呵呵，这是在上世纪90年代中期提出的）。那么这个子网中其实只有少数的节点需要全球唯一的IP地址，其他的节点的IP地址应该是可以重用的。
   因此，基本的NAT实现的功能很简单，在子网内使用一个保留的IP子网段，这些IP对外是不可见的。子网内只有少数一些IP地址可以对应到真正全球唯一的IP地址。如果这些节点需要访问外部网络，那么基本NAT就负责将这个节点的子网内IP转化为一个全球唯一的IP然后发送出去。(基本的NAT会改变IP包中的原IP地址，但是不会改变IP包中的端口).
    关于基本的NAT,可以参看RFC 1631.
   另外一种NAT叫做NAPT，从名称上我们也可以看得出，NAPT不但会改变经过这个NAT设备的IP数据报的IP地址，还会改变IP数据报的TCP/UDP端口号。基本NAT的设备可能我们见的不多（呵呵，我没有见到过），NAPT才是我们真正讨论的主角。看下图：
                               ServerS1                        
                        18.181.0.31:1235                         
                                     |
         ^  Session 1 (A-S1) ^     | 
         |  18.181.0.31:1235 |     |  
         v 155.99.25.11:62000v     |   
                                     |
