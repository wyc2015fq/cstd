# FreeSWITCH第三方库（其他）的简单介绍（三） - DoubleLi - 博客园







FreeSWITCH使用了大量的第三方库，本文档主要介绍关联相关库的信息：

音频相关库的信息介绍参考：[http://www.cnblogs.com/yoyotl/p/5486753.html](http://www.cnblogs.com/yoyotl/p/5486753.html)

视频相关库的信息介绍参考：[http://www.cnblogs.com/yoyotl/p/5488890.html](http://www.cnblogs.com/yoyotl/p/5488890.html)



```
① ldns
```

dns相关的库。



```
② libre
```

libre 是一个利用异步 I/O 实现的实时通讯库。采用可移植的 POSIX 源码编写，符合 ANSI C89 和 ISO C99 标准。

这是一个可靠、快速、低内存占用的库，兼容 RFC ，支持 IPv4 和 IPv6，协议实现包括 SIP, SDP, RTP/RTCP, BFCP, DNS, 和 STUN/TURN/ICE.



```
③ udt
```

基于UDP的数据传输协议（UDP-based Data Transfer Protocol，简称UDT）是一种互联网数据传输协议。UDT的主要目的是支持高速广域网上的海量数据传输，而互联网上的标准数据传输协议TCP在高带宽长距离网络上性能很差。

顾名思义，UDT建于UDP之上，并引入新的拥塞控制和数据可靠性控制机制。UDT是面向连接的双向的应用层协议。它同时支持可靠的数据流传输和部分可靠的数据报传输。

由于UDT完全在UDP上实现，它也可以应用在除了高速数据传输之外的其它应用领域，例如点到点技术（P2P），防火墙穿透，多媒体数据传输等。



```
④ VAST
```

VAST是一个开放源代码的软件库（LGPL许可证），允许基于P2P技术构建可扩展的虚拟世界应用。

它主要支持空间发布订阅（SPS）和基于Voronoi图的覆盖网络（VON）。此外，Voronoi图的自组织覆盖（VSO）有助于执行负载均衡，VoroCast能帮助信息转发到大网络域。



```
⑤ sngtc
```

sangoma板卡的一个媒体转换库。



```
⑥ libmongoc
```

mongo-c-driver是MongoDB数据库的客户端库。



```
⑦ freetype
```

FreeType库是一个完全免费（开源）的、高质量的且可移植的字体引擎，它提供统一的接口来访问多种字体格式文件，包括TrueType, OpenType, Type1, CID, CFF, Windows FON/FNT, X11 PCF等。



```
⑧ libsmpp34
```

SMPP协议相关的库，具体项目地址：[git://git.osmocom.org/libsmpp34](git://git.osmocom.org/libsmpp34)。



```
⑨ libtap
```

libtap库是一个简单易用的测试C程序的软件。（类比于Junit）依赖于Test::More Perl模块。



```
⑩ grapes
```

This is GRAPES, the Generic Resource-Aware P2P Environment for Streaming。




上善若水，水利万物而不争。









