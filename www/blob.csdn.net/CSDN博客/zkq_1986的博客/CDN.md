# CDN - zkq_1986的博客 - CSDN博客





2016年07月18日 20:15:18[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：262
个人分类：[CDN](https://blog.csdn.net/zkq_1986/article/category/6317999)









1 IP网络中的协议

1） RSVP，资源预留协议（Resource ReserVation protocol）。为连接预留相应的资源，如CPU，内存。应用场景：视频会议。

2）RTP/RTCP，实时传输协议/实时传输控制协议(Real-time transport protocol)。位于TCP/UDP之上。RTCP为RTP传输文件时提供流量控制和拥塞控制。

3）RTSP，实时流协议(Real-time streaming protocol)。位于TCP/UDP之上。底层通过TCP/UDP或者RTP/RTCP实现文件传输。





http与RTSP比较

1）http是一个无状态协议，而RTSP是有状态的

2）http本质上是一个非对称协议，客户端提出请求而服务器响应（单向）；RTSP是对称的，服务器和客户端都可发送和响应请求（双向）。

RTSP主要应用于对实时性要求较高的流媒体服务，如IPTV点播、直播、回看业务，由于客户端和服务器需要一直保持连接状态进行信令交互和流传输，对服务器性能和网络质量也有较高要求。

渐进式http由于部署简单，客户端只要有支持http协议，如浏览器即可，所以目前主要被国内外众多主流的视频网站采用，如Youtube、优酷、土豆、爱奇艺等等。







