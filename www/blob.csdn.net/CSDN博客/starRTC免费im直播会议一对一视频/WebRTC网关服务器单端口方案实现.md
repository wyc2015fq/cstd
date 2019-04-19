# WebRTC网关服务器单端口方案实现 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年09月05日 17:20:33[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：143
所属专栏：[webRTC](https://blog.csdn.net/column/details/30214.html)
标准WebRTC连接建立流程
这里描述的是Trickle ICE过程，并且省略了通话发起与接受的信令部分。流程如下：
1） WebRTC A通过Signal Server转发SDP OFFER到WebRTC B。WebRTC B做完本地处理以后，通过 Signal Server转发SDP ANSWER到A。
2）A、B同时向STUN Server发送Binding request请求自身的外网地址，并从STUN Server回包的MAPPED-ADDRESS中得到各自的外网地址；
3）A、B收集完内外网ICE Candidate，并通过Signal Server发送给对方；
4）双方开始做NAT穿越，互相给对方的ICE Candidate发送STUN Binding Request；
5）NAT穿越成功，A、B之间的P2P连接建立，进入媒体互通阶段。在这个过程中，我们看到了有三个核心的部分，SDP协商、ICE Candidate交换、Stun Binding Req/Res的连通性检查。
WebRTC网关连接建立流程
在了解了标准WebRTC的建连流程以后，我们来看看WebRTC客户端如何与网关建连。
首先，我们网关的Media Server拥有公网IP，因此Server就不需要通过Stun Server收集自身的公网IP。WebRTC客户端先与网关Signal Server协商SDP，包括ICE Candidate，Media Server分配IP和端口作为网关的ice candidate发送给客户端。因为网关是公网IP，所以客户端向这个IP发送STUN Binding Request会被服务器收到， 并回复Response。接着客户端与网关媒体服务器进行DTLS握手与秘钥协商，在此基础上进一步进行SRTP的音视频通信。至此，WebRTC客户端与网关服务器建连成功。
WebRTC网关服务器媒体架构
最简的服务器端端口方案是我们可以为每个客户端分配一个端口，服务器上使用这个端口区分每个用户，就像图里描述的A、B、C、D四个人在WebRTC网关服务器上分别对应UDP端口10001~1004。这种方案逻辑上很简单，很多开源的服务器都采用这个方案，如janus。另外一个原因是使用了libnice库在服务器上来和客户端做ice建连，类似的做法都是采用多端口的架构。
那么多端口有什么不足呢？
1）很多的网络出口防火墙对能够通过的UDP端口是有限制的；
2）对于服务端来说开辟这么多端口，安全性本身也有一定的问题，特别是运维同学，更是拒绝；
3）开辟这么多的端口在Server端上，端口的开销和性能均有一定的影响。那能否用单端口？使用单端口前，核心要解决的一个问题是：如何区分每一个RTP/RTCP包是属于哪一个WebRTC客户端。
为了解决这个问题，我们需要使用一些小技巧。首先，有几个基础知识点我们先了解一下。如下图：
1）SDP offer和answer里配置的ice-ufrag字段里面内容，原来是用来作为stun数据包的鉴权的，因此STUN Binding Request里面的USERNAME字段就是由上Offer和answer的ice-ufrag内容拼接而成。
2）发送STUN Binding Request的客户端本地udp fd，与ice建连成功后发送媒体数据的udp fd是同一个，也就是说Server上看到的ip port是同一个。
有了上面的背景知识，你肯定已经大致有一个方案了。我们来看看实现细节是怎么样的：
1）在服务器给Web端的SDP Answer中设置 ice-ufrag为roomid/userid，其中RoomID和UserID是通话业务层分配的内容，用于区分每对通话以及参与者。接着做Ice candidate协商，Web端开始做连通性检测，也就是stun binding request里的USERNAME为SDP local和remote的ice-ufrag指定内容。
2）服务器收到stun binding request的客户端ip和端口，并正常回stun binding response。3）记录客户端地址与用户的信息的映射关系。
4）服务器收到一个rtp/rtcp媒体数据包，通过包的源ip和端口，查询映射表就可以识别这个包属于哪个用户。
WebRTC客户端使用PeerConnection来表示不同的媒体连接，接下来我们将介绍如何选择PeerConnection的方案。
在线体验单端口直播与一对一视频通话：[https://github.com/starrtc/android-demo](https://github.com/starrtc/android-demo)
