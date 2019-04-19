# RTMP流媒体播放过程：握手，建立连接，建立流，播放 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月31日 09:41:25[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：268
本文讲述从打开一个RTMP流媒体到视音频数据开始播放的整个过程。
播放一个流媒体有两个前提步骤：
第一步，建立一个网络连接（NetConnection）；
第二步，建立一个网络流（NetStream）。
其中，网络连接代表服务器端应用程序和客户端之间的连通关系。
网络流代表了发送多媒体数据的通道。服务器和客户端之间只能建立一个网络连接，但是基于该连接可以创建很多网络流。他们的关系如图所示：
![](http://note.youdao.com/yws/res/52576/CEC66E6868BF4614911CD557E53D27A9)
1 介绍
播放一个RTMP协议的流媒体需要经过以下几个步骤：握手，建立连接，建立流，播放。RTMP连接都是以握手作为开始的。建立连接阶段用于建立客户端与服务器之间的“网络连接”；建立流阶段用于建立客户端与服务器之间的“网络流”；播放阶段用于传输视音频数据。
2 握手（HandShake）
一个RTMP连接以握手开始，双方分别发送大小固定的三个数据块
a)        握手开始于客户端,发送C0、C1块。服务器收到C0或C1后发送S0和S1。
b)        当客户端收齐S0和S1后，开始发送C2。当服务器收齐C0和C1后，开始发送S2。
c)        当客户端和服务器分别收到S2和C2后，握手完成。
![](http://note.youdao.com/yws/res/52578/D8D8338DCA28496B8CC08EB0471E7748)
握手
3，建立网络连接（NetConnection）
a)        客户端发送命令消息中的“连接”(connect)到服务器，请求与一个服务应用实例建立连接。
b)        服务器接收到连接命令消息后，发送确认窗口大小(Window Acknowledgement Size)协议消息到客户端，同时连接到连接命令中提到的应用程序。
c)        服务器发送设置带宽()协议消息到客户端。
d)        客户端处理设置带宽协议消息后，发送确认窗口大小(Window Acknowledgement Size)协议消息到服务器端。
e)        服务器发送用户控制消息中的“流开始”(Stream Begin)消息到客户端。
f)         服务器发送命令消息中的“结果”(_result)，通知客户端连接的状态。
![](http://note.youdao.com/yws/res/52580/8ABC88DB0EFA42689B04D447EC4A3EB2)
建立连接
4，建立网络流（NetStream）
a)      客户端发送命令消息中的“创建流”（createStream）命令到服务器端。
b)      服务器端接收到“创建流”命令后，发送命令消息中的“结果”(_result)，通知客户端流的状态。
![](http://note.youdao.com/yws/res/52581/5ADFB1493070424186D77936335812C9)
建立流
5 ，播放（Play）
a)        客户端发送命令消息中的“播放”（play）命令到服务器。
b)        接收到播放命令后，服务器发送设置块大小（ChunkSize）协议消息。
c)        服务器发送用户控制消息中的“streambegin”，告知客户端流ID。
d)        播放命令成功的话，服务器发送命令消息中的“响应状态” NetStream.Play.Start & NetStream.Play.reset，告知客户端“播放”命令执行成功。
e)        在此之后服务器发送客户端要播放的音频和视频数据。
![](http://note.youdao.com/yws/res/52579/E32D76B0676B45C89D039F00115D56E5)
播放流
RTMP协议规范（中文翻译）：[http://download.csdn.net/detail/leixiaohua1020/6563059](http://download.csdn.net/detail/leixiaohua1020/6563059)
[http://blog.csdn.net/leixiaohua1020/article/details/11704355](http://blog.csdn.net/leixiaohua1020/article/details/11704355)
