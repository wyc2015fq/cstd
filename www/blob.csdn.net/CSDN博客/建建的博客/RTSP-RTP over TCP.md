# RTSP - RTP over TCP - 建建的博客 - CSDN博客
2017年09月07日 14:04:11[纪建](https://me.csdn.net/u013898698)阅读数：125
个人分类：[视频编解码学习](https://blog.csdn.net/u013898698/article/category/6703370)

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
**SETUP**
To use TCP communication, you need to request TCP connection during RTSP SETUP. You have to sent SETUP command with
要使用TCP连接，RTSP客户端需要在SETUP阶段请求TCP连接。SETUP命令中应该包括如下格式的Transport：
> 
Transport: RTP/AVP/TCP;interleaved=0-1
This will tell the server to send media data with TCP and interleave the data in channel 0 and 1. Given in the specification, data channel is even
上述Transport将告诉服务端使用TCP协议发送媒体数据，并且使用信道 0 和 1 对流数据以及控制信息进行交织。详细说来，使用偶数信道作为数据
number and control channel is odd (data_ch_num + 1). So, if you data channel is 0, your control channel will be 0 + 1 = 1.
传输信道，使用奇数信道作为控制信道（数据信道 + 1）。所以，如果你设定数据信道为 0 ，那控制信道应该是 0 + 1 = 1。
Below is an example of TCP SETUP
![](https://img-blog.csdn.net/20130920160700812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xvdW1l/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**RTP Data**
After the setup, RTP data will be sent through the TCP socket that is used for RTSP commands. The RTP data will be encapsulate in the following format
SETUP之后，RTP数据将通过用来发送RTSP命令的TCP Socket进行发送。RTP数据将以如下格式进行封装：
> 
> 
| magic number | channel number | embedded data length | data |
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
> 
Session = "Session" ":" session-id [ ";" "timeout" "=" delta-seconds ]
If Session is given, each subsequence RTSP command must be sent with the session so that the server can identify the stream. Also, please note
一旦会话（SESSION）被创建，接下来的每一个RTSP命令都必须加上session_id，否则服务端无法识别命令对应的流。另外，"timeout"的值是可选
that timeout is an optional value. The default value for timeout is 60 seconds. So, it is advisable to send a RTSP command to the server every
的。"timeout"的默认值是60秒。所以，建议每隔60秒向服务端发送一个RTSP命令以保持TCP连接的存活。
60 second to keep the TCP connection alive.
Read RFC 2326 Section 10.12 Embedded (Interleaved) Binary Data for more details
RFC 2326 第10.12节说明了插入二进制数据的细节。
Posted by [Thompson
 Ng](http://www.blogger.com/profile/14008404474958460857)at 11:59
 PM[](http://www.blogger.com/post-edit.g?blogID=181484357747627654&postID=485341598789567181&from=pencil)
Labels: [RTSP](http://www.thompsonng.blogspot.com/search/label/RTSP)
以下是一些问答，觉得对我们理解 RTP over TCP 是很有帮助的，也摘抄给大家。
#### 15 comments:
- 
[Jeff Chen](http://www.blogger.com/profile/05232608446719882826)[March
 28, 2011 at 8:01 AM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1301324515175#c1772801908428389441)
Does it still need "keep-alive" in TCP mode?
I found this:
http://msdn.microsoft.com/en-us/library/cc245366%28v=prot.10%29.aspx
However, I cannot find a clear statement in RFC2326.
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=1772801908428389441)
- 
[Thompson Ng](http://www.blogger.com/profile/14008404474958460857)[March
 28, 2011 at 7:09 PM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1301364580110#c22783158742320479)
My experience is Yes.
All media server/camera I have encounter so far requires me to send a RTSP message to keepalive the RTSP session while getting RTP stream from the same socket.
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=22783158742320479)
- 
[Andy](http://www.blogger.com/profile/06839420264148245451)[June
 5, 2011 at 1:39 AM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1307263153646#c3817791318103784703)
Hello, in the pic, there is a "$\000", I know the $ is the magic char, but, what does the "\00" mean?
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=3817791318103784703)
- 
[Thompson Ng](http://www.blogger.com/profile/14008404474958460857)[June
 5, 2011 at 2:32 AM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1307266363054#c1873902387214502537)
\0 is just a null character in C. 00 is ASCII representation of channel 0.
So just imagine $\000 is a printf from C++ for 1 byte of magic and 1 byte of channel
Just remember that you need 1 byte of magic 0x24, and 1 byte of channel number will do.
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=1873902387214502537)
- 
[Andy](http://www.blogger.com/profile/06839420264148245451)[June
 6, 2011 at 5:35 AM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1307363744485#c2585080785321455715)
Thanks for the response, Thompson.
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=2585080785321455715)
- 
[_vl_](http://www.blogger.com/profile/08392362344359230376)[June
 24, 2011 at 9:11 AM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1308931870059#c6176359866037563239)
What would be the RTCP metric for RTP over TCP to monitor for bandwidth congestion: delay, or jitter ? And would that monitoring prevent against delay accumulation? Anyone tried to research that?
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=6176359866037563239)
- 
[Thompson Ng](http://www.blogger.com/profile/14008404474958460857)[June
 25, 2011 at 1:38 AM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1308991095178#c4799105381643435832)
Normally, it should use the jitter on RTCP report and the server should reduce the sending rate.
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=4799105381643435832)
- 
[lauren](http://www.blogger.com/profile/06835407443449137284)[September
 21, 2011 at 11:42 AM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1316630539301#c4881551118792581951)
You are right that UDP is a better choice as it provides robust streaming capability for media. But I never knew about its issues that you listed above.Thanks.
[pdf digital signature](http://www.arx.com/information/pdf-signature)
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=4881551118792581951)
- 
[Thompson Ng](http://www.blogger.com/profile/14008404474958460857)[September
 21, 2011 at 11:53 PM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1316674389159#c4990875342291023189)
In fact, RTP/UDP does not really work in the public Internet most of the time. The UDP packets will be filtered by the ISP usually.
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=4990875342291023189)
- 
[psychesnet](http://www.blogger.com/profile/17296984081040889267)[January
 31, 2012 at 6:58 PM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1328065105581#c5745611147518045009)
Dear Thompson Ng,
What open source do you use for pack RTP over TCP ? 
ccRTP or ....
I try to use oRTP, but it seems only support UDP streaming.
Thanks a lot.
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=5745611147518045009)
- 
[raghava](http://www.blogger.com/profile/10796675258141169342)[May
 17, 2012 at 10:27 PM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1337318821841#c3226328017259849812)
Hi Thompson Ng,
i am writting an rtsp application in c.After sending PLAY command to server(ipcam) it responding with success code.
but am not able to receive the data buffers.
Do i need to create socket again for rtp port. which sent by SETUP command. can you give some info How i can receive data buffer?
Thanks in advance
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=3226328017259849812)
- 
[Thompson Ng](http://www.blogger.com/profile/14008404474958460857)[May
 17, 2012 at 11:15 PM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1337321736995#c9110901668893440268)
For RTSP tunneling, you need to create 2 TCP sockets. One POST socket to send commands and one GET socket to receive data and response. You need to set up these 2 socket with a session ID so that the server can bind these 2 socket together.
Then, you send your command via POST socket and read your response from GET socket. 
Note, both command response and data are received via GET socket. Thus, you need to check for magic byte 0x24 to identify if you are getting command response or data response
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=9110901668893440268)
- 
[Tarun](http://www.blogger.com/profile/13301685463889577139)[February
 11, 2013 at 3:32 AM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1360582336422#c7871431230425084304)
A very useful information to stream RTSP - RTP over TCP.
Can you tell me for this what changes do we have to make in the live 555 libraries as they also stream RTSP - RTP over UDP 
and you also have the knowledge of live 555 as seen in your article http://thompsonng.blogspot.in/search/label/Live555.
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=7871431230425084304)
- 
[Thompson Ng](http://www.blogger.com/profile/14008404474958460857)[February
 11, 2013 at 6:59 PM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1360637959009#c1257748542899376263)
Live555 comes with openRTSP that allow RTSP/RTP over TCP. http://www.live555.com/openRTSP/
openRTSP will switch to RTSP/RTP over TCP with switch command -t
I did not really work with it. But my advise is... Put debug point at the main.c command -t potion. Step through the code and you will able to find the relevant code
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=1257748542899376263)
- 
[Tarun](http://www.blogger.com/profile/13301685463889577139)[February
 12, 2013 at 10:52 PM](http://thompsonng.blogspot.com/2011/03/rtsp-rtp-over-tcp.html?showComment=1360738350707#c319220665905104423)
I am using Live 555 to stream the live video from camera so according to there documentationenter link description here i have implemented the createNewStreamSource() and createNewRTPSink() but the problem now i am facing is what should be the function call.
Reply[Delete](http://www.blogger.com/delete-comment.g?blogID=181484357747627654&postID=319220665905104423)

