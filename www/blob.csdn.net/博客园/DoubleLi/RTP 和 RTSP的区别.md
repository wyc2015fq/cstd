# RTP 和 RTSP的区别 - DoubleLi - 博客园






RTP(Real-time Transport Protocol)是用于Internet上针对多媒体数据流的一种传输协议。RTP被定义为在一对一或一对多的传输情况下工作。其目的是提供时间信息和实现流同步。但RTP通常使用UDP来传送数据。但RTP也可以在TCP或ATM等其他协议之上工作。当应用程序开始一个RTP会话时将使用两个端口:一个给RTP一个给 RTCP。RTP本身并不能为接顺序传送数据包提供可靠的传送机制。也不提供流量控制或拥塞控制。它依靠RTCP提供这些服务。通常RTP算法并不作为一个独立的网络层来实现。而是作为应用程序代码的一部分。实时传送控制协议RTCP.

    RTCP(Real-time Transport Control Protocol)和RTP提供流量控制和拥塞控制。在RTP会话期间,各参与者周期性地传送RTCP包.RTCP包中含有已发送的数据包的数量、丢失的数据包的数量等统计资料.因此,服务器可以利用这些信息动态地改变传输速率，甚至改变有效载荷类型。RTP和RTCP配合使用，它们能以有效的反馈和最小的开销使传输效率最佳化。因而特别适合传送网上的实时数据。 

    RTSP实时流协议RTSP(Real-time Streaming Protocol)是由Real Networks和Netscape共同中提出的。该协议定义了一对多应用程序如何有效地通过lP网络传送多媒体数据。RTSP在体系结构上位于RTP和RTCP之上。它使用TCP或RTP完成数据传输。HTTP与RTSP相比。HTTP传送HTML。而RTP传送是多媒体数据。HTTP请求由客户机发出，服务器作出响应；使用RTSP时，客户机和服务器都可以发出请求，即RTSP可以是双向的。



区别：

RTP是实时传输协议，一般不作为单独应用层协议处理；rtsp是实时流传输协议，它是与http等级的应用层网络协议，它是由realmedia开发，用来传输流媒体影像文件。   
   rtsp可基于rtp之上，比如常见的视频流传输过程：视频压缩文件－>rtp打包－>基于udp的rtsp网络传输；也可以不做成rtp包，直接基于udp传送，如视频压缩文件－>基于udp的rtsp网络传输。



具体协议内容可参看以下标准文档：

RTP/RTCP-------------------------RFC3550/RFC3551

RTSP        --------------------------RFC2326









