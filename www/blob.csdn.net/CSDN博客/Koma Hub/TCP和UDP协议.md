# TCP和UDP协议 - Koma Hub - CSDN博客
2018年07月15日 20:50:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：84
个人分类：[TCP/IP](https://blog.csdn.net/Rong_Toa/article/category/7792859)
## `TCP`
### [](https://github.com/Rtoax/Knowledge/blob/master/TCP-IP/TCP%E5%92%8CUDP%E5%8D%8F%E8%AE%AE.md#1%E6%A6%82%E8%BF%B0)(1)概述
同其他任何协议栈一样,`TCP` 向相邻的高层提供服务。因为`TCP`的上一层就是应用层,因此,`TCP`数据传输实现了从一个应用程序到另一个应用程序的数据传递。应用程序通过编程调用 TCP 并使用 TCP 服务,提供需要准备发送的数据,用来区分接收数据应用的目的地址和端口号。通常应用程序通过打开一个`socket`来使用 TCP 服务,TCP 管理到其他 socket 的数据传递。可以说,通过 IP 的源/目的可以惟一地区分网络中两个设备的关联,通过 socket 的源/目的可以惟一地区分网络中两个应用程序的关联。
### [](https://github.com/Rtoax/Knowledge/blob/master/TCP-IP/TCP%E5%92%8CUDP%E5%8D%8F%E8%AE%AE.md#2%E4%B8%89%E6%AC%A1%E6%8F%A1%E6%89%8B%E5%8D%8F%E8%AE%AE)(2)`三次握手协议`
TCP 对话通过三次握手来初始化的。三次握手的目的是使数据段的发送和接收同步,告诉其他主机其一次可接收的数据量,并建立虚连接。
下面描述了这三次握手的简单过程。
- 初始化主机通过一个同步标志置位的数据段发出会话请求。
- 接收主机通过发回具有以下项目的数据段表示回复: 同步标志置位、 即将发送的数据段的起始字节的顺序号、应答并带有将收到的下一个数据段的字节顺序号。
- 请求主机再回送一个数据段,并带有确认顺序号和确认号。
下图就是这个流程的简单示意图。
![](https://img-blog.csdn.net/20180715204957302?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
TCP 实体所采用的基本协议是滑动窗口协议。当发送方传送一个数据报时,它将启动计时器。当该数据报到达目的地后,接收方的 TCP 实体向回发送一个数据报,其中包含有一个确认序号,它意思是希望收到的下一个数据报的顺序号。如果发送方的定时器在确认信息到达之前超时,那么发送方会重发该数据报。
### [](https://github.com/Rtoax/Knowledge/blob/master/TCP-IP/TCP%E5%92%8CUDP%E5%8D%8F%E8%AE%AE.md#3tcp-%E6%95%B0%E6%8D%AE%E6%8A%A5%E5%A4%B4)(3)TCP 数据报头
下图给出了 TCP 数据报头的格式。
TCP 数据报头的含义如下所示。
![](https://img-blog.csdn.net/20180715205005904?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 源端口、目的端口:16 位长。标识出远端和本地的端口号。
- 序号:32 位长。标识发送的数据报的顺序。
- 确认号:32 位长。希望收到的下一个数据报的序列号。
- TCP 头长:4 位长。表明 TCP 头中包含多少个 32 位字。
- 6 位未用。
- ACK:ACK 位置 1 表明确认号是合法的。如果 ACK 为 0,那么数据报不包含确认信息,确认字段被省略。
- PSH:表示是带有 PUSH 标志的数据。接收方因此请求数据报一到便可送往应用程序而不必等到缓冲区装满时才传送。
- RST:用于复位由于主机崩溃或其他原因而出现的错误的连接。还可以用于拒绝非法的数据报或拒绝连接请求。
- SYN:用于建立连接。
- FIN:用于释放连接。
- 窗口大小:16 位长。窗口大小字段表示在确认了字节之后还可以发送多少个字节。
- 校验和:16 位长。是为了确保高可靠性而设置的。它校验头部、数据和伪 TCP 头部之和。
- 可选项:0 个或多个 32 位字。包括最大 TCP 载荷,窗口比例、选择重发数据报等选项。
## [](https://github.com/Rtoax/Knowledge/blob/master/TCP-IP/TCP%E5%92%8CUDP%E5%8D%8F%E8%AE%AE.md#udp)UDP
### [](https://github.com/Rtoax/Knowledge/blob/master/TCP-IP/TCP%E5%92%8CUDP%E5%8D%8F%E8%AE%AE.md#1%E6%A6%82%E8%BF%B0-1)(1)概述
UDP 即用户数据报协议,它是一种无连接协议,因此不需要像 TCP 那样通过三次握手来建立一个连接。同时,一个 UDP 应用可同时作为应用的客户或服务器方。由于 UDP 协议并不需要建立一个明确的连接,因此建立 UDP 应用要比建立 TCP 应用简单得多。
UDP 协议从问世至今已经被使用了很多年, 虽然其最初的光彩已经被一些类似协议所掩盖,但是在网络质量越来越高的今天,UDP 的应用得到了大大的增强。它比 TCP 协议更为高效,也能更好地解决实时性的问题。如今,包括网络视频会议系统在内的众多的客户/服务器模式的网络应用都使用 UDP 协议。
### [](https://github.com/Rtoax/Knowledge/blob/master/TCP-IP/TCP%E5%92%8CUDP%E5%8D%8F%E8%AE%AE.md#2udp-%E6%95%B0%E6%8D%AE%E5%8C%85%E5%A4%B4)(2)UDP 数据包头
UDP 数据包头如下图所示。
![](https://img-blog.csdn.net/20180715205016412?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 源地址、目的地址:16 位长。标识出远端和本地的端口号。
- 数据报的长度是指包括报头和数据部分在内的总的字节数。因为报头的长度是固定的,所以该域主要用来计算可变长度的数据部分(又称为数据负载)。
## [](https://github.com/Rtoax/Knowledge/blob/master/TCP-IP/TCP%E5%92%8CUDP%E5%8D%8F%E8%AE%AE.md#%E5%8D%8F%E8%AE%AE%E7%9A%84%E9%80%89%E6%8B%A9)协议的选择
协议的选择应该考虑到以下 3 个方面。
### [](https://github.com/Rtoax/Knowledge/blob/master/TCP-IP/TCP%E5%92%8CUDP%E5%8D%8F%E8%AE%AE.md#1%E5%AF%B9%E6%95%B0%E6%8D%AE%E5%8F%AF%E9%9D%A0%E6%80%A7%E7%9A%84%E8%A6%81%E6%B1%82)(1)对数据可靠性的要求
对数据要求高可靠性的应用需选择 TCP 协议,如验证、密码字段的传送都是不允许出错的,而对数据的可靠性要求不那么高的应用可选择 UDP 传送。
### [](https://github.com/Rtoax/Knowledge/blob/master/TCP-IP/TCP%E5%92%8CUDP%E5%8D%8F%E8%AE%AE.md#2%E5%BA%94%E7%94%A8%E7%9A%84%E5%AE%9E%E6%97%B6%E6%80%A7)(2)应用的实时性
由于 TCP 协议在传送过程中要进行三次握手、重传确认等手段来保证数据传输的可靠性。使用 TCP 协议会有较大的时延,因此不适合对实时性要求较高的应用,如 VOIP、视频监控等。相反,UDP 协议则在这些应用中能发挥很好的作用。
### [](https://github.com/Rtoax/Knowledge/blob/master/TCP-IP/TCP%E5%92%8CUDP%E5%8D%8F%E8%AE%AE.md#3%E7%BD%91%E7%BB%9C%E7%9A%84%E5%8F%AF%E9%9D%A0%E6%80%A7)(3)网络的可靠性
由于 TCP 协议的提出主要是解决网络的可靠性问题,它通过各种机制来减少错误发生的概率。因此,在网络状况不是很好的情况下需选用 TCP 协议(如在广域网等情况),但是若在网络状况很好的情况下(如局域网等)就不需要再采用 TCP 协议,选择 UDP 协议来减少网络负荷。
