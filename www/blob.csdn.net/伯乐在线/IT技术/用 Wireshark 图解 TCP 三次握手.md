# 用 Wireshark 图解 TCP 三次握手 - 文章 - 伯乐在线
原文出处： [BYSocket （@BYSocket）](http://www.bysocket.com/?p=1404)
## **一、什么是 Wireshark ？**
简单地说，Wireshark 是抓包工具。官网说，“Wireshark 是一个网络包分析工具。 网络数据包分析仪将尝试捕获网络数据包并试图尽可能详尽显示该数据包。”
推荐一本书《Wireshark 网络分析就这么简单》，Wireshark 工具下载地址 https://www.wireshark.org/download.html。
Wireshark 不单单能抓包，主要的 Features 是对数据包进行各种的分析。下载打开界面非常简单直观：
![tcp01](http://jbcdn2.b0.upaiyun.com/2016/11/8e288a206523382facec9f8f0bca8c59.jpeg)
最上面是 Packet List 窗口，它列出了所有网络包。
在 Packet List 中选定的网络包会详细地显示在中间的 Packet Details 窗口中。
最底下是 Packet Bytes Details 窗口（16 进制数据），我们一般不会用到它。
## **二、我们抓什么包呢？如题 TCP**
TCP 一脸懵逼。记得上次在 《[图解Http协议](http://blog.jobbole.com/108188/)》得知，HTTP 是一个客户端和服务器端请求和响应的标准TCP。其实建立在 TCP 之上的。那我们就抓 HTTP，这很简单。
## **三、实操 Wireshark**
选择 Capture – Options，选择当前网络，点击 Start 。如图：
![tcp02-1024x846](http://jbcdn2.b0.upaiyun.com/2016/11/3850de701d9e1db2ce399130b3944e50.jpeg)
浏览器打开我的博客 www.bysocket.com ，然后在 Wireshark 中的过滤器输入博客 IP 过滤：ip.addr == 106.14.40.18
选择第一行，如第一张图，简单介绍下 Packet Details 窗口参数详情：
> 
Frame – 物理层
Ethernet – 数据链路层
Internet Protocol Version – 网络层
Transmission Control Protocol – 传输层
大家知道 网络层次划分为 标准的OSI七层模型，还有 TCP/IP四层协议 以及 TCP/IP五层协议。如图：
![705728-20160424234827195-1493107425](http://jbcdn2.b0.upaiyun.com/2016/11/7f82213e27bc3ecbd3e9148307f0e396.png)
其中，Transmission Control Protocol – 传输层是我们最为关心的。如图已经展开了:
![tcp04-1024x564](http://jbcdn2.b0.upaiyun.com/2016/11/6574a80d667c5ae686f691a090abf81e.jpg)
具体我们先学习 TCP 报文段格式：
![tcp05](http://jbcdn2.b0.upaiyun.com/2016/11/32cb075f99050fc03c76cceeea4d23eb.jpg)
> 
那么第二个红框，由上至下，一一大家可以对应下，重点的是：
Sequence number – 序号
Acknowledgment number – 确认号
Flags – 标志位
— Acknowledgment 确认位
— Push 急迫位
— Reset 重置位
— Syn 同步位
— Fin 终止位
**a. 第一次握手标志位**
localhost Seq=0 -> 博客地址
从标志位看出，同步位有值，在做请求（SYN）：Syn 同步位为1
![tcp06-1024x387](http://jbcdn2.b0.upaiyun.com/2016/11/93197d6dc5882e6649d6ce5aeaf99b62.jpg)
**b. 第二次握手标志位**
博客地址 Seq=0 Ack=1 -> localhost
从标志位看出，确认位、同步位有值，在做应答（SYN+ACK）：Syn 同步位为 1 、Acknowledgment 确认位为 1
![tcp07](http://jbcdn2.b0.upaiyun.com/2016/11/f588326573e9a3e5de82f1017e49d738.jpg)
**c. 第三次握手标志位**
localhost Seq=1 Ack=1 -> 博客地址 （注: Seq=Seq+1）
从标志位看出，只有确认位有值，在做再次确认（SYN）：Acknowledgment 确认位为 1
![tcp08](http://jbcdn2.b0.upaiyun.com/2016/11/e19122099aaad07369098246bb6c9eb6.jpg)
综上所述，一个完整的三次握手就是：请求（SYN） — 应答（SYN+ACK） — 再次确认（SYN）
![tcp09](http://jbcdn2.b0.upaiyun.com/2016/11/dfa7d99525e6dee05ba21af550424a11.jpg)
