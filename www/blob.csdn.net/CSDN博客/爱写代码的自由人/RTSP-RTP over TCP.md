# RTSP - RTP over TCP - 爱写代码的自由人 - CSDN博客





2018年02月26日 18:50:23[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：98








原文链接：http://www.cnblogs.com/lidabo/p/4483497.html




Normally, RTSP provide streaming over UDP. By nature, UDP is a better choice as it provides robust streaming capability for media. However, it is unlikely to use UDP for streaming over the Internet.

通常来说，RTSP提供UDP方式发送RTP流。当然，发送流媒体时，UDP往往是更好的选择。但是，在互联网上使用UDP方式发送流是不可能的。



Some issues with UDP are

下面是使用UDP时的一些问题：



1. RTSP/RTP over UDP requires many UDP ports to be opened (each media stream requires 2 UDP port for data and control).

1. UDP协议上的RTSP/RTP需要打开许多UDP端口（每一路流媒体都需要2个UDP端口，一个用于接收数据，一个用于接收控制信息）；

2. Point 1 is a real problem as routers in the Internet may not open these ports.

2.当因特网上的路由器没有打开这些端口的时候，上述第一点将会存在问题；

3. It is normal for intermediate Internet router to filter and ignore UDP packets.

3. 中间网络路由器很容易就过滤或者忽略掉UDP数据包；

4. UDP is unreliable. Media packets may be lost when travelling along the Internet

4. UDP是不可靠传输协议，媒体包在因特网上传输时会面临着丢包。



RTSP/RTP over TCP may resolve this issue.

但是我们如果在TCP传输协议上承载RTSP/RTP将解决这些问题。




1. RTSP/RTP will communicate via 1 port for command and data. That is the RTSP port.

1. RTSP/RTP的控制命令和数据都通过一个端口，即RTSP的端口（默认为554），进行交互。

2. TCP provide reliable streaming

2. TCP协议提供可靠的流传输。

3. It is more likely that the intermediate Internet router allow these TCP packets to go through.

3. TCP包更容易穿透中间网络路由器。



Using RTSP/RTP over TCP come with a price.

但是，使用TCP传输协议承载RTSP/RTP需要花更多的功夫。



1. It complicated the packetization and depacketization process due to binary interleave.

1. 由于二元交织，会使得RTP包封包和解包的过程变得更加复杂。

2. TCP is reliable but have overhead. It may cause delay in real time media.

2. TCP是可靠的传输协议，但正是因为如此，会导致在实时流媒体中的延时。

So, now I will talk about how to setup RTSP/RTP over TCP

接下来让我们来了解一下怎么使用TCP承载RTSP/RTP。



**RTSP/RTP over TCP**

**TCP承载RTSP/RTP**



When you use RTSP/RTP over TCP, all command and media data will be sent through the RTSP port, normally, port 554. Also, when using

当使用TCP协议承载RTSP/RTP时，所有的命令和媒体数据都将通过RTSP端口，通常是554，进行发送。同时，数据将经过二元交织格式化之后才能发

RTSP/RTP over TCP, the data will be sent via binary interleave format.

送。



Below will describe the essential for using RTSP/RTP over TCP

接下来我们将描述使用TCP承载RTSP/RTP的主要要素：



***SETUP***



To use TCP communication, you need to request TCP connection during RTSP SETUP. You have to sent SETUP command with

要使用TCP连接，RTSP客户端需要在SETUP阶段请求TCP连接。SETUP命令中应该包括如下格式的Transport：


> Transport: RTP/AVP/TCP;interleaved=0-1


This will tell the server to send media data with TCP and interleave the data in channel 0 and 1. Given in the specification, data channel is even

上述Transport将告诉服务端使用TCP协议发送媒体数据，并且使用信道 0 和 1 对流数据以及控制信息进行交织。详细说来，使用偶数信道作为数据

number and control channel is odd (data_ch_num + 1). So, if you data channel is 0, your control channel will be 0 + 1 = 1.

传输信道，使用奇数信道作为控制信道（数据信道 + 1）。所以，如果你设定数据信道为 0 ，那控制信道应该是 0 + 1 = 1。



Below is an example of TCP SETUP

![](https://img-blog.csdn.net/20130920160700812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xvdW1l/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



***RTP Data***



After the setup, RTP data will be sent through the TCP socket that is used for RTSP commands. The RTP data will be encapsulate in the following format

SETUP之后，RTP数据将通过用来发送RTSP命令的TCP Socket进行发送。RTP数据将以如下格式进行封装：


> | magic number | channel number | embedded data length | data |


magic number - 1 byte value of hex 0x24

RTP数据标识符，"$"

channel number - 1 byte value to denote the channel

信道数字 - 1个字节，用来指示信道

embedded data length - 2 bytes to denote the embedded data length

数据长度 - 2个字节，用来指示插入数据长度

data - data packet, ie RTP packet, with the total length of the embedded data length

数据 - 数据包，比如说RTP包，总长度与上面的数据长度相同

Below is a full example of the communication exchanged

下面是交互过程的一个完整示例：

![](https://img-blog.csdn.net/20130917144427437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xvdW1l/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



Also, as RTSP is a application protocol, it has no way to control how TCP timeout the connection. Thus, during the RTSP SETUP, a Session is 

然而，由于RTSP是应用协议，没有办法控制TCP连接的超时。所以，在RTSP的SETUP阶段，新建一个会话（SESSION）用于标识已连接的流。

given to identify the connected stream


> Session = "Session" ":" session-id [ ";" "timeout" "=" delta-seconds ]
If Session is given, each subsequence RTSP command must be sent with the session so that the server can identify the stream. Also, please note

一旦会话（SESSION）被创建，接下来的每一个RTSP命令都必须加上session_id，否则服务端无法识别命令对应的流。另外，"timeout"的值是可选

that timeout is an optional value. The default value for timeout is 60 seconds. So, it is advisable to send a RTSP command to the server every

的。"timeout"的默认值是60秒。所以，建议每隔60秒向服务端发送一个RTSP命令以保持TCP连接的存活。


60 second to keep the TCP connection alive.



Read RFC 2326 Section 10.12 Embedded (Interleaved) Binary Data for more details

RFC 2326 第10.12节说明了插入二进制数据的细节。



