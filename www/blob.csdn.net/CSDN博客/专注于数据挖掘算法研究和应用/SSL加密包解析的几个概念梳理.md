# SSL加密包解析的几个概念梳理 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月17日 10:25:09[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4254








1、DPI技术初识

DPI(Deep PacketInspection)深度包检测技术是在传统IP数据包检测技术(OSI L2-L4之间包含的数据包元素的检测分析)之上增加了对应用层数据的应用协议识别，数据包内容检测与深度解码。

![](https://img-blog.csdn.net/20160317102453542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




深度包检测技术即DPI技术是一种基于应用层的流量检测和控制技术，当IP数据包、TCP或UDP数据流通过基于DPI技术的带宽管理系统时，该系统通过深入读取IP包载荷的内容来对OSI七层协议中的应用层信息进行重组，从而得到整个应用程序的内容，然后按照系统定义的管理策略对流量进行整形操作。

通过对网络通讯的原始数据包捕获，DPI技术可使用其三大类的检测手段：基于应用数据的“特征值”检测、基于应用层协议的识别检测、基于行为模式的数据检测。根据不同的检测方法对通信数据包可能含有的异常数据做逐一的拆包分析，深度挖据出宏观数据流中存在的细微数据变化。

2、SSL/TLS协议

SSL（SecureSockets Layer，安全套接层），及其继任者 TLS（Transport Layer Security，传输层安全）是为网络通信提供安全及数据完整性的一种安全协议。TLS与SSL在传输层对网络连接进行加密。

SSL介于应用层和TCP层之间。应用层数据不再直接传递给传输层，而是传递给SSL层，SSL层对从应用层收到的数据进行加密，并增加自己的SSL头。

https是以安全为目标的HTTP通道，简单讲是HTTP的安全版。即HTTP下加入SSL层，https的安全基础是SSL

3、wireshark安装

Wireshark（前称Ethereal）是一个网络封包分析软件。网络封包分析软件的功能是撷取网络封包，并尽可能显示出最为详细的网络封包资料。Wireshark使用PCAP作为接口，直接与网卡进行数据报文交换。

在Ubuntu环境下安装，可参考：

http://blog.csdn.net/fjssharpsword/article/details/50739192

4、Wireshark解析加密包

wireshark配置

Edit->preferences->Protocols->SSL：RSA keyslist配置服务器密钥文件即可解析。

对于更高版本（如2.0.1版本），TSL包看ReassembledTCP页。

5、SSL会话劫持

实际，对于端到端的会话，密钥文件可获取情况下，解析加密包是可以的。但对于非端到端的加密包解析却需要通过数据转发机制捕获其会话过程中的密钥才能解析后续会话数据包。

从端口镜像获取客户端和服务端的会话数据包，可通过pcap先获取密钥，而后在用密钥解析。应用数据流重定向和SSL会话劫持技术，典型工具如sslsniff和sslstrip。



