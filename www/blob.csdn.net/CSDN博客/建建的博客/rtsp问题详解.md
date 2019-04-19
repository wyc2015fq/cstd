# rtsp问题详解 - 建建的博客 - CSDN博客
2019年01月29日 14:18:24[纪建](https://me.csdn.net/u013898698)阅读数：37
## 关于 RTSP.
RTSP协议是一个非常类似HTTP协议的流控制协议。它们都使用纯文本来发送信息，而且rtsp协议的语法也和HTTP类似。Rtsp一开始这样设计，也是为了能够兼容使用以前写的HTTP协议分析代码 。这是个好消息。
它们主要的区别是HTTP协议是没有状态的， http协议在发送一个命令后，连接会断开，而且命令之间没有依赖性。不同的是RTSP的命令需要知道现在正处于一个什么状态，也就是说rtsp的命令总是按照顺序来发送，某个命令总在另外一个命令之前要发送。Rtsp不管处于什么状态都不会去断掉连接。
HTTP 协议默认使用80端口，而RTSP 默认使用554端口。如果一些服务器因为某些安全的原因而封掉了这个端口，那代理和防火墙可能不让RTSP消息通过，需要管理员去放开554端口，而使得rtsp协议能通过。
## RTSP 并非只是微软在用！
这是一个公开的规范，在这个规范上开发了很多的流服务器。甚至Linux服务提供者和苹果的程序员也使用rtsp协议以及Real Networks流媒体。似乎整个世界的网络流传输都用这个协议。然而，微软并不只在rtsp上有所作为。
## 微软和RTSP.
在写这个文档的时候，微软正处于从首选MMS协议转换到首选采用RTSP协议的过程中。这个说明在Media Player9.0版本和流媒体服务器2003版本之后，我们会看到微软将rtsp协议作为流媒体传输的主要协议 。
随着时间慢慢的流逝，我们会发现mms协议正逐步走出人们的视野。It is only assumed that this is so MS can say they are being open with their protocols (rtsp is an open standard) while at the same time disregarding the need to publicise their own MMS protocol once its gone from media player. 然而，mms还没有真的死亡，至少在接下来的几年中我们依然可以看到它在流媒体传输中的身影。
## 是否微软的RTSP协议和标准的开放式RTSP不同？
是的。跟在RFC2326（1998年四月）中定义的原始RTSP协议相比，微软的rtsp协议有一些轻微的改动。我们网站上有本文档（还有后续版本）和一个简单的研究，它们可以帮助你了解这些信息。
## 区别在哪儿？
微软的rstp规范与标准rtsp协议相比最主要的改动是发送包payloads到客户端的方式，另外还有一些请求命令有一些改动。传输单个媒体包的机制并没有文档（就 我目前所知），这可能是微软要保留的信息。
就像MMS和HTTP 1.0 流协议使用一个媒体数据包头一样，RTSP也有。但是微软的数据包头格式没有在任何的rtsp文档中注明。在企图连接微软的rtsp服务器时，这是主要的问题。
微软RTSP协议的命令采用的语法和标准rtsp协议的命令语法一样，只有一些小的修改和添加，可以通过阅读网上的一些文档，就可以知道怎么去组成这些命令。微软rtsp命令部分已经有文档了。
## 一次典型的RTSP协议传输过程
这个例子为了简略，没有把发送接收的包放上来
## To Server =>                                 NETWORK                         <= To Client
客户端连服务器的554端口
客户端发送“DESCRIBE”命令
                                               服务器返回标准rtsp头
                                               这个rtsp头和数据实体包含ASF文件头信息
以及所有的和媒体文件相关的流bit rate data
客户端发送“SETUP” 音频流媒体建立命令(stream 1)
服务器返回标准rtsp头
客户端发送“SETUP” 视频流媒体建立命令(stream 2)
                                               服务器返回标准rtsp头
客户端发送“PLAY” 命令
服务器返回标准rtsp头
客户端发送 “SET_PARAMETER” 命令
这个命令还包含了一些客户端发送给服务器的信息，比如客户端的操作系统，CPU类型，播放器版本，日期时间等信息。消息格式是tagged XML.
服务器返回标准rtsp头
#### 服务器即将开始一个流，发送媒体数据包（包含媒体数据包头），请看接下来的 ####
当要断开这个流的时候，服务器会向客户端发送一个EOF指示
                                               服务器断开socket连接


## 一个典型的发给服务器的RTSP命令
DESCRIBE rtsp://wm.microsoft.com/ms/video/0001-hi.wmv RTSP/1.0
User-Agent: WMPlayer/9.0.0.2980 guid/3300AD50-2C39-46C0-AE0A-81D88F547805
Accept: application/sdp
Accept-Charset: UTF-8, *;q=0.1
X-Accept-Authentication: NTLM, Digest, Basic
Accept-Language: en-GB, *;q=0.1
CSeq: 1
Supported: com.microsoft.wm.srvppair, com.microsoft.wm.sswitch, com.microsoft.wm.eosmsg, com.microsoft.wm.predstrm
注意：
DESCRIBE rtsp://wm.microsoft.com/ms/video/0001-hi.wmv RTSP/1.0
这是要连接的url（服务器域名和流路径），后面跟着RTSP的版本。
User-Agent: WMPlayer/9.0.0.2980 guid/3300AD50-2C39-46C0-AE0A-81D88F547805
这条表示了客户端使用的是什么播放器，以及播放器的版本，再跟着一个独特的GUID。
X-Accept-Authentication: NTLM, Digest, Basic
客户端可以接受的authentication 类型
注意CSeq 要从1开始。服务器针对请求命令的应答也应该有Cseq: 加上数字 ，这样可以知道是针对哪条请求发的应答。
在客户端发送一个请求命令，得到成功的应答后，再发送下一条命令，CSeq的值要加1。
## 一个典型的RTSP应答消息，它们跟HTTP的消息非常的相似
一个针对客户端发的DESCRIBE命令，服务器的应答例子如下所示：
<<<HEADER START>>>
RTSP/1.0 200 OK
Content-Type: application/sdp
Vary: Accept
X-Playlist-Gen-Id: 27006
X-Broadcast-Id: 0
Content-Length: 3324
Date: Tue, 18 Nov 2003 15:57:05 GMT
CSeq: 1
Server: WMServer/9.0.0.3372
Supported: com.microsoft.wm.srvppair, com.microsoft.wm.sswitch, com.microsoft.wm.eosmsg, com.microsoft.wm.fastcache, com.microsoft.wm.packetpairssrc
Last-Modified: Tue, 18 Jun 2002 21:05:39 GMT
Cache-Control: x-wms-content-size=23180160, max-age=86399, must-revalidate, proxy-revalidate
Etag: "23180160"
<<<BODY START>>> ( 两个“/r/n”指示本SDP数据的开始 ).
<<< 这是Session Description Protocol 协议>>>
v=0
o=- 200311171721060249 200311171721060249 IN IP4 127.0.0.1
s=<No Title>
c=IN IP4 0.0.0.0
b=AS:1091
a=maxps:13632
t=0 0
a=control:rtsp://wm.microsoft.com/ms/video/0001-hi.wmv/
a=etag:{9D7121C3-1A1B-8ED6-6675-CB15D19D1FB7}
a=range:npt=3.100-173.903
a=recvonly
a=pgmpu:data:application/x-wms-contentdesc,8,language,31,0,,5,title,31,0,,6,author,31,0,,9,copyright,31,0,,35,WMS_CONTENT_DESCRIPTION_DESCRIPTION,31,0,,30,WMS_CONTENT_DESCRIPTION_RATING,31,0,,44,WMS_CONTENT_DESCRIPTION_SERVER_BRANDING_INFO,31,12,WMServer/9.0,51,WMS_CONTENT_DESCRIPTION_PLAYLIST_ENTRY_START_OFFSET,3,4,3100,47,WMS_CONTENT_DESCRIPTION_PLAYLIST_ENTRY_DURATION,3,6,170803,58,WMS_CONTENT_DESCRIPTION_COPIED_METADATA_FROM_PLAYLIST_FILE,3,1,1,42,WMS_CONTENT_DESCRIPTION_PLAYLIST_ENTRY_URL,31,17,0001-hi.wmv%0D%0A
a=pgmpu:data:application/vnd.ms.wms-hdr.asfv1;base64,Mcay...dY5mm2QCAQ==
m=audio 0 RTP/AVP 96
b=AS:35
b=RS:0
b=RR:0
a=rtpmap:96 x-asf-pf/1000
a=control:audio
a=stream:1
m=application 0 RTP/AVP 96
b=RS:0
b=RR:0
a=rtpmap:96 x-wms-rtx/1000
a=control:rtx
a=stream:65536
m=video 0 RTP/AVP 96
b=AS:471
b=RS:0
b=RR:0
a=rtpmap:96 x-asf-pf/1000
a=control:stream=2
a=stream:2
以/r/n/r/n结束
看下面这行：
a=pgmpu:data:application/vnd.ms.wms-hdr.asfv1;base64, Mcay..…==
这是真实的ASF文件头数据（被Base64编码过）。在将这部分头数据写到文件之前需要Base64解码成标准的ascii码。
SDP实体数据也告诉我们在媒体数据中都有些什么流
这个文件有2个媒体流：
ID = 1 有一个“audio”标志的音频流
ID = 2 有一个“stream=2”标志的视频流
还有第三个以“rtx”标志的流，这是一个控制流，并不携带任何的媒体内容。这个控制流使用stream ID = 65536.
看下面这行：
a=pgmpu:data:application/x-wms-contentdesc, …..
这表示了紧跟的数据是一个内容描述对象。
## 标准RTSP 消息的错误代码– 在应答消息的第一行表示
 ”100” ; Continue (all 100 range)
 “200” ; OK
 ”201” ; Created
 ”250” ; Low on Storage Space
 ”300” ; Multiple Choices
 ”301” ; Moved Permanently
 ”302” ; Moved Temporarily
 ”303” ; See Other
 ”304” ; Not Modified
 ”305” ; Use Proxy
 ”350” ; Going Away
 ”351” ; Load Balancing
 ”400” ; Bad Request
 ”401” ; Unauthorized
 ”402” ; Payment Required
 ”403” ; Forbidden
 ”404” ; Not Found
 ”405” ; Method Not Allowed
 ”406” ; Not Acceptable
 ”407” ; Proxy Authentication Required
 ”408” ; Request Time-out
 ”410” ; Gone
 ”411” ; Length Required
 ”412” ; Precondition Failed
 ”413” ; Request Entity Too Large
 ”414” ; Request-URI Too Large
 ”415” ; Unsupported Media Type
 ”451” ; Parameter Not Understood
 ”452” ; reserved
 ”453” ; Not Enough Bandwidth
 ”454” ; Session Not Found
 ”455” ; Method Not Valid in This State
 ”456” ; Header Field Not Valid for Resource
 ”457” ; Invalid Range
 ”458” ; Parameter Is Read-Only
 ”459” ; Aggregate operation not allowed
 ”460” ; Only aggregate operation allowed
 ”461” ; Unsupported transport
 ”462” ; Destination unreachable
 ”500” ; Internal Server Error
 ”501” ; Not Implemented
 ”502” ; Bad Gateway
 ”503” ; Service Unavailable
 ”504” ; Gateway Time-out
 ”505” ; RTSP Version not supported
 ”551” ; Option not supported

## RTSP数据包头(每一个媒体数据包的开头)
二进制包头数据后面紧跟着媒体数据包（asf/wmv 等等）。
Values are in Big Endian order. (not little endian as in mms and http protocols).
--------这些数据位在公开的RTSP 文档中也指示了-------
BYTE                 “$” or 0x24
BYTE                    Channel id ，一般= 0x00, 你可以看到“P” 或者 0x50 for packet pairs data, 看下面的描述.
WORD               数据包的长度（从接下来的区域算起）
-------- 接下来的数据位表示的含义还没有确定，微软也许会指出-----------
WORD      ???    填充0x80E0
WORD                   序列号。这个值被初始化成一个随即的值，然后在每一个数据包发送时加1。  在这个值达到0xffff时，它将设置成0。跟其他协议相比，这个值得范围比较小（16位），使得它不能在写媒体数据到文件中时做为有指导性的序列号使用。大的流媒体文件有很多的数据包，包的数量超过了他的范围，所以可能会发生相同序列号的包。只使用这个序列号来作错误检验——缺少了某一个序列号的包意味着发生了丢包错误。
DWORD                发送时间。This is stated in milliseconds and shows the time that this media packet should be presented. 预先录制的文件设置T = 0 ，实况流设置一个不同于0的值。
DWORD                ??? 固定的一个值. 它是一个随即值，但是一旦一个rtsp会话建立，这个值就一直保持不变，所有的数据包都是这个值。可以使用这个值作为每次会话的一个ID，新的rtsp会话会有新的值。
BYTE                     标志位， 看下面：
                                Bit 0 (lsb)      ? = 0
                                Bit 1               ? = 0
                                Bit 2               ? = 0
                                Bit 3               ? = 0
                                Bit 4               Duration field present in pre header if set (see below)
                                Bit 5               ? = 0
                                Bit 6               ? = 1 (总是1)
                                Bit 7 (msb)    被设置成1，表示数据包含有一个关键桢
BYTE                     ??? 总是设置成0
WORD                   包长度。此16位表示从上面标志位域开始的包长度。
DWORD                [可选的] 持续时间。只有上面的标志位的Bit4 被设置了才会有这个域，否则就没有这个域。表示为多少毫秒，指出了传输的媒体包的总持续时间。
<<< 下面跟着媒体数据, like 82 00 00 ….. >>>

## 在RTSP请求和应答中使用的有用的标签值：
CSeq:                                    命令的序列号，逐1增加。
                                               所有的请求和应答都用得到。
Content-Length:                                  这个标记的存在说明后面有实体数据，而且给出了这个数据块的大小，单位是byte
X-Playlist-Gen-Id:                               用来检查播放列表是否有效。这个标记最初在客户端发送DESCRIBE命令后使用。
                                               客户端在发送“SETUP”命令给服务器时必须回应一样的值
X-Playlist-Seek-Id:                              值必须和X-Playlist-Gen-Id 域的值相同，在PLAY请求命令中使用.
Blocksize:                                             媒体包的总长度，单位是byte
Session:                                                 Session ID是用作客户端和服务器之间是否是正确的连接。在客户端发送SETUP命令，服务器会在应答消息头里面发送这个值给客户端。 We only see the Session value on the first stream selected (usually this is the audio stream)。 Session值相当的长，一共有20个阿拉伯数字。
                                                                紧跟着Session值， 你可以看到一个值：       “timeout= xxxx”。. 这是服务器需要得到回应或者ACK回应（为了保持连接）的时间。客户端必须在这个时段内发送一个ACK ，要不然连接就要被强制中断。一个ACK就是发送一条GET_PARAMETER命令到服务器。
X-Accept-Authentication:                 允许的authentication 方法
                                                                NTLM, Digest 和 Basic 是标准的
X-Broadcast-Id:                                  是否是实况或者是先期录制的流。
                                                                0 表示先期录制，其他的值表示是实况。
Range:                                                   Range is the offset and end time positions to stream the media. For a zero start and full file stream, this value is set to:   npt=0.000-
                                                                where 0.000 is the offset and –0.000 (optional) is the ending time. Values are stated in seconds.
Speed:                                                    用来调整传输到客户端的流得速度。假如你的带宽可以接受更高速的数据传送，这个域的值可以设置大于1来加速下载数据
普通情况  Speed: 1.0       i.e. x1 rate
                                                                Media player 使用 :     Speed: 1.294
这个主要取决于你的网络连接速度。
Server:                                                   服务器类型和软件版本
EOF:                                                       文件结束标记，也是流的结束标记
Date:                                                      日期时间，下面举个例子：
Tue, 18 Nov 2003 15:57:07 GMT
Bandwidth:                                           流需要的最大带宽，bits/秒
Transport:                                             使用什么协议来传输数据，比如TCP或者UDP等
Etag:                                                      实体标记Entity tag，是一个分配给会话的值，就像"23180160"
Supported:                                            支持的COM modules , 有的是可选的.
com.microsoft.wm.srvppair       - packet pairs at server
com.microsoft.wm.sswitch         - stream ID selection com.microsoft.wm.eosmsg       - end of stream message com.microsoft.wm.fastcache       - fast cache for buffering com.microsoft.wm.packetpairssrc.  - packet pairs
Content-Type:                                     此域用来表示命令或者应答的用意
                                                                下面是常用的几种类型：
                                                                application/x-wms-Logconnectstats
                                                                这个在SET_PARAMETER命令中用到，表示将客户端的信息登记到服务器上。
                                                                application/sdp
                                                                这个表示接下来数据包里面的是sdp数据，它是在服务器对DESCRIBE命令的应答包中。
                                                                application/x-wms-contentdesc
                                                                表示紧跟的数据是一个内容描述对象，它设置the layout of the dialog.
                                                                application/vnd.ms.wms-hdr.asfv1
                                                                表示跟着一个流媒体头信息(ASF header)，
                                                                可以用BASIC 或者DIGEST来解码。
                                                                application/x-rtsp-packetpair
                                                                Packet Pair data is random non-compressible data and is sent to the client and timed for response times. 它被用来确定连接的可用带宽。Packet pair data 是可选的，你不必经常去请求这个数据。 这个是在发送GET_PARAMATER命令到服务器时，用到的。.
