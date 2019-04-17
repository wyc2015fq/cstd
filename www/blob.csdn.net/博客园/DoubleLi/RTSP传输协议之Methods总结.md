# RTSP传输协议之Methods总结 - DoubleLi - 博客园









- RTSP/1.0 200 OK  
- Server: DSS/5.5.5 (Build/489.16; Platform/Linux; Release/Darwin; state/beta; )  
- Cseq: 1  
- Last-Modified: Mon, 07 May 2007 20:03:31 GMT  
- Cache-Control: must-revalidate  
- Content-length: 1207  
- Date: Mon, 12 Aug 2013 08:08:55 GMT  
- Expires: Mon, 12 Aug 2013 08:08:55 GMT  
- Content-Type: application/sdp  
- x-Accept-Retransmit: our-retransmit  
- x-Accept-Dynamic-Rate: 1  
- Content-Base: rtsp://192.168.1.100/sample_100kbit.mp4/  
- 
- v=0  
- o=StreamingServer 3585283734 1178568211000 IN IP4 192.168.1.100  
- s=/sample_100kbit.mp4  
- u=http:///  
- e=admin@  
- c=IN IP4 0.0.0.0  
- b=AS:96  
- t=0 0  
- a=control:*  
- a=mpeg4-iod:"data:application/mpeg4-iod;  
- a=isma-compliance:1,1.0,1  
- a=range:npt=0-  70.00000  
- m=video 0 RTP/AVP 96  
- b=AS:76  
- a=rtpmap:96 MP4V-ES/90000  
- a=control:trackID=3  
- a=cliprect:0,0,242,192  
- a=framesize:96 192-242  
- a=fmtp:96 profile-level-id=1;config=000001B0F3000001B50EE040C0CF0000010000000120008440FA283020F2A21F  
- a=mpeg4-esid:201  
- m=audio 0 RTP/AVP 97  
- b=AS:20  
- a=rtpmap:97 mpeg4-generic/8000/2  
- a=control:trackID=4  
- a=fmtp:97 profile-level-id=15;mode=AAC-hbr;sizelength=13;indexlength=3;indexdeltalength=3;config=1590  
- a=mpeg4-esid:101  



如果媒体客户端从一个数据源获得表示描述，而非通过 DESCRIBE，并且该描述包含了一个媒体初始化参数的全集，那么客户端就应该使用这些参数,而不是再通过 RTSP 请求相同媒体的描述。再有，服务器不应该使用DESCRIBE Response作为media indirection的方法。

需要建立基本的规则,使得客户端有明确的方法了解何时通过 DESCRIBE 请求媒体初始化信息,何时不请求。强制 DESCRIBE 响应包含它所描述媒体流集合的所有初始化信息,不鼓励将 DESCRIBE 用作 media indirection 的方法,通过这两点避免了使用其他方法可能会引起的循环问题(looping problems).媒体初始化是任何基于RTSP系统的必要条件,但RTSP规范并没有规定它必须通过DESCRIBE方法完成。RTSP客户端可以通过3种方法来接收媒体初始化信息：

（1）利用DESCRIBE方法

（2）利用其它一些协议(HTTP,email 附件,等)

（3）利用命令行或标准输入(同一个 SDP 或其它媒体初始化格式的文件一起启动,工作方式类似于浏览器的帮助程序)

为了实际协同工作，对于最精简的服务器也推荐支持 DESCRIBE 方法，最精简的客户端也支持从标准输入,命令行和/或其它对于客户端操作环境合适的方法来接收媒体初始化文件的能力。

2、SETUP

SETUP 请求为 URI 指定流式媒体的传输机制。客户端能够发出一个 SETUP 请求为正在播放的媒体流改变传输参数,服务器可能同意这些参数的改变。若是不同意,它必须响应错误"455 Method Not Valid In This State"。 为了尽量绕开防火墙干涉,即使它不会影响参数,客户端也必须指出传输参数,例如,指出服务器向外发布的固定的广播地址。

由于 SETUP 包括了所有传输初始化信息,防火墙和其他中间的网络设备(它们需要这些信息)分让了解析 DESCRIBE 响应的繁琐任务,这些任务留给了媒体初始化。Transport 首部域指定了客户端数据传输时可接受的传输参数；响应包含了由服务器选出的传输参数。

下面代码示例创建了一个SETUP请求。





**[cpp]**[view plain](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)[copy](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)





- string msg;  
- msg.append("SETUP ");  
- msg.append(trackurl);  
- msg.append(" RTSP/1.0\r\n");  
- msg.append("Transport: RTP/AVP/UDP;unicast;client_port=");  
- msg.append(rtpPort);  
- msg.append("-");  
- msg.append(rtpPort + 1);  
- msg.append("\r\n");  
- if (trackindex > 0) {  
-     msg->append("Session: ");  
-     msg->append(mSessionID);  
-     msg->append("\r\n");  
- }  
- msg.append("CSeq: ");  
- msg.append(CSeq);  
- msg.append("\r\n");  
- msg.append("\r\n");  




作为对 SETUP 请求的响应,服务器产生了会话标志符。如果对服务器的请求中包含了会话标志符,服务器必须将此 setup 请求捆绑到一个存在的会话,或者返回"459Aggregate Operation Not Allowed"。

如果包含多个Stream，比如既有Audio也有Video，则需要发送多次SETUP请求，同样也会有两个Response。下面是利用DarwinStreamingSrvrlinux-Linux建立的服务器的SETUP的Response的内容示例：





**[plain]**[view plain](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)[copy](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)





- RTSP/1.0 200 OK    
- Server: DSS/5.5.5 (Build/489.16; Platform/Linux; Release/Darwin; state/beta; )    
- Cseq: 2    
- Last-Modified: Mon, 07 May 2007 20:03:31 GMT    
- Cache-Control: must-revalidate    
- Session: 3135849742811878224    
- Date: Mon, 12 Aug 2013 08:08:55 GMT    
- Expires: Mon, 12 Aug 2013 08:08:55 GMT    
- Transport: RTP/AVP/UDP;unicast;source=192.168.1.100;client_port=16390-16391;server_port=6970-6971;ssrc=0B0533AE   




这里需要注意的一点是：在上面创建SETUP请码的代码中有一个if语句，这里为什么需要一个if语句呢？在RTSP的Spec中有这样一句话，如下：

Once a client receives a Session identifier, it MUST return it for any request related to that session.

就是说，如果客户端收到的Response中有Session ID，那么，在之后的请求中都会用这个Session ID，相应的Response也会用同一个Session ID，而不会重新创建一个。如上面的代码，mSessionID就是从第一个SETUP的Response中获得的Session ID。如果这里每次都不设置Session ID，那么服务器都会针对SETUP创建一个新的Session ID，从而就会影响后面的PLAY请求，从而影响整个播放过程。当然了，如果只要求播放某个Stream，比如Audio或者Video，那么在创建SETUP请求的时候就可以不用设置Session ID，服务器会针对每个SETUP创建一个新的Session ID。后面发送PLAY请求的时候，设置哪个Session ID，就会播放相应的Stream。

3、PLAY

PLAY 方法告知服务器通过 SETUP 中指定的机制开始发送数据 。在尚未收到SETUP 请求的成功应答之前,客户端不可以发出 PLAY 请求。PLAY 请求将正常播放时间定位到指定范围的起始处，并且传输数据流直到播放范围结束。

下面代码示例创建了一个PLAY请求。





**[cpp]**[view plain](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)[copy](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)





- string msg;  
- msg.append("PLAY ");  
- msg.append(mSessionURL);  
- msg.append(" RTSP/1.0\r\n");  
- msg.append("Session: ");  
- msg.append(mSessionID);  
- msg.append("\r\n");  
- msg.append("Range: npt=");  
- msg.append(start);  
- msg.append("-");  
- msg.append(end);  
- msg.append("\r\n");  
- msg.append("CSeq: ");  
- msg.append(CSeq);  
- msg.append("\r\n");  
- msg.append("\r\n");  




PLAY 请求会被放入队列中，服务器必须将 PLAY 请求放到队列中有序执行。也就是说，后一个 PLAY 请求需要等待前一个PLAY 请求完成才能得到执行。比如，Client连续创建了三个PLAY请求，Range域的设置如下所示：

（1）Range: npt=10-15

（2）Range: npt=20-25

（3）Range: npt=30-

不管到达的两个 PLAY 请求之间有多紧凑,服务器首先 play 第10 到 15 秒,然后立即第 20 到 25 秒,最后是第 30 秒直到结束。

下面是利用DarwinStreamingSrvrlinux-Linux建立的服务器的PLAY的Response的内容示例：





**[plain]**[view plain](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)[copy](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)





- RTSP/1.0 200 OK  
- Server: DSS/5.5.5 (Build/489.16; Platform/Linux; Release/Darwin; state/beta; )  
- Cseq: 4  
- Session: 3135849742811878224  
- Range: npt=0.00000-70.00000  
- RTP-Info: url=rtsp://192.168.1.100/sample_100kbit.mp4/trackID=3;seq=54996;rtptime=310659698,url=rtsp://192.168.1.100/sample_100kbit.mp4/trackID=4;seq=6084;rtptime=702565368  




不含 Range 首部域的 PLAY 请求也是合法的。它从媒体流开头开始播放,直到媒体流被暂停。如果媒体流通过 PAUSE 暂停,媒体流传输将在暂停点重新开始。如果媒体流正在播放,那么这样一个 PLAY 请求将不起更多的作用,只是客户端可以用此来测试服务器是否存活。

Range 首部域可能包含一个时间参数。该参数以UTC格式指定了播放开始的时间。如果在这个指定时间后收到消息,那么播放立即开始。时间参数可能用来帮助同步从不同数据源获取的数据流。

对于一个点播媒体流，服务器用播放的实际范围答复请求。This may differ from the requested range if alignment of the requested range to valid frame boundaries is required for the media source.如果在请求中没有指定范围,当前位置将在答复中返回。答复中播放范围的单位与请求中相同。在播放完被要求的范围后,表示将自动暂停,就好像发出了一个 PAUSE 请求。

4、PAUSE

PAUSE 请求引起媒体流传输的暂时中断。如果请求 URL 中指定了具体的媒体流，那么只有该媒体流的播放和记录被暂停。比如，指定暂停音频,播放将会无声。如果请求 URL 指定了一个表示或者媒体流已成组,那么在该表示或组中的所有当前活动流的传输将被暂停。在重启播放或记录后,必须维护不同track的同步。尽管服务器可能在暂停后,在timeout 的时间内关闭会话,释放资源，但是任何资源都必须保存,其中 timeout 参数位于 SETUP 消息的Session Header中。

Session Header的格式如下所示：

Session = "Session" ":" session-id [ ";" "timeout" "=" delta-seconds ]

下面代码示例创建了一个PAUSE请求。





**[cpp]**[view plain](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)[copy](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)





- string msg;  
- msg.append("PAUSE ");  
- msg.append(mSessionURL);  
- msg.append(" RTSP/1.0\r\n");  
- msg.append("Session: ");  
- msg.append(mSessionID);  
- msg.append("\r\n");  
- msg.append("CSeq: ");  
- msg.append(CSeq);  
- msg.append("\r\n");  
- msg.append("\r\n");  




PAUSE 请求中可能包含一个 Range 首部域用来指定何时Stream或Presentation暂停，我们称这个时刻为暂停点。该首部域必须包含一个精确的值,而不是一个时间范围。媒体流的正常播放时间设置成暂停点。当服务器遇到在任何当前挂起的 PLAY 请求中指定的时间点后,暂停请求生效。如果 Range 首部域指定了一个时间超出了任何一个当前挂起的 PLAY 请求,将返回错误"457 InvalidRange" 。如果一个媒体单元(比如一个音频或视频禎)正好在一个暂停点开始，那么表示将不会被播放或记录。如果 Range 首部域缺失，那么在收到暂停消息后媒体流传输立即中断，并且暂停点设置成当前正常播放时间。

利用 PAUSE 请求可忽视所有排队的 PLAY 请求，但必须维护媒体流中的暂停点。不带 Range 首部域的后继 PLAY 请求从暂停点重启播放。比如，如果服务器有两个挂起的播放请求，播放范围分别是 10 到 15 和 20 到 29，这时收到一个暂停请求,暂停点是 NPT21,那么它将会开始播放第二个范围,并且在 NPT21 处停止。如果服务器正在服务第一个请求播放到 NPT13 位置，收到暂停请求,暂停点 NPT12，那么它将立即停止。如果请求在 NPT16 暂停，那么服务器在完成第一个播放请求后停止,放弃了第二个播放请求。再如，服务器收到播放请求，播放范围从 10 到 15 和 13 到 20(即之间有重叠)，PAUSE 暂停点是 NPT14，则当服务器播放第一段范围时,PAUSE 请求将生效,而第二个 PLAY 请求会被忽略重叠部分,就好像服务器在开始播放第二段前收到 PAUSE 请求。不管 PAUSE 请求何时到达,它总是设置 NPT 到 14。

如果服务器已经在 Range 首部域指定的时间外发送了数据，后继的 PLAY 仍会在暂停点及时重启，因为它认为客户端会丢弃在暂停点后收到的数据。这就确保了连续的、无隙的暂停/播放循环。

5、ANNOUNCE

ANNOUNCE 方法有两个用途:

（1）当客户端向服务器发送时，ANNOUNCE 将通过请求 URL 识别的表示描述或者媒体对象提交给服务器;

（2）当服务器相客户端发送时，ANNOUNCE 实时更新会话描述。

如果有新的媒体流加到表示中(比如在一个现场表示中)，整个表示描述应该重发；而不只是增加组件,如果这样做的话，组件也可以被删除了。

ANNOUNCE请求包括两个部分，一部分是RTSP标准请求内容，另一部分是按照SDP格式描述的内容。下面代码示例创建了一个从Client到Server的ANNOUNCE请求。





**[cpp]**[view plain](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)[copy](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)





- string msg;  
- msg.append("ANNOUNCE ");  
- msg.append(mSessionURL);  
- msg.append(" RTSP/1.0\r\n");  
- msg.append("Session: ");  
- msg.append(mSessionID);  
- msg.append("\r\n");  
- msg.append("CSeq: ");  
- msg.append(CSeq);  
- msg.append("\r\n");  
- msg.append("Content-Type: application/sdp\r\n");  
- msg.append("Content-Length: ");  
- msg.append(SDPSize);  
- msg.append("\r\n");  
- msg.append("\r\n");  
- msg.append(SDPContent);  
- msg.append("\r\n");  
- msg.append("\r\n");  




其中，SDPContent是按照SDP格式描述的一段内容，SDPSize是SDPContent的内容的大小。一个简单的SDPContent如下所示：





**[plain]**[view plain](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)[copy](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)





- v=0  
- o=mhandley 2890844526 2890845468 IN IP4 126.16.64.4  
- s=SDP Seminar  
- i=A Seminar on the session description protocol  
- u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps  
- e=mjh@isi.edu (Mark Handley)  
- c=IN IP4 224.2.17.12/127  
- t=2873397496 2873404696  
- a=recvonly  
- m=audio 3456 RTP/AVP 0  
- m=video 2232 RTP/AVP 31  




6、OPTIONS

7、RECORD

8、GER_PARAMETER

9、SER_PARAMETER

10、REDIRECT

11、TEARDOWN

TEARDOWN 请求终止了给定 URI 的媒体流传输，并释放了与该媒体流相关的资源。如果该 URI 是对此Presentation的Presentation URI，那么任何与此会话相关的任何 RTSP 会话标志符将不再有效。除非所有传输参数由会话描述符定义，否则 SETUP 请求必须在会话能被再次播放之前发出。

下面代码示例创建了一个TEARDOWN请求。





**[cpp]**[view plain](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)[copy](http://blog.csdn.net/yu_yuan_1314/article/details/9896785#)





- string msg;  
- msg.append("TEARDOWN ");  
- msg.append(mSessionURL);  
- msg.append(" RTSP/1.0\r\n");  
- msg.append("Session: ");  
- msg.append(mSessionID);  
- msg.append("\r\n");  
- msg.append("CSeq: ");  
- msg.append(CSeq);  
- msg.append("\r\n");  
- msg.append("\r\n");  










