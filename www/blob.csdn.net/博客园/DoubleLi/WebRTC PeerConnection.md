# WebRTC PeerConnection - DoubleLi - 博客园






   PeerConnection 是WebRtc native interface 最外层的模块,  通常来说, 用户使用Peerconection就可以构建最简单的p2p 应用.     下面的UML里是PeerConnection的最重要的属性和方法.   

      PortAllocator 用来为媒体连接分配可用的端口

      LocalStreams 存放本端的Video/Audio Streams.  

      RemoteStreams 存放远端Video/Audio Streams,      streams 里保存了y一个或者多个Audio/Video Tracks.    

      MediaController 是Channel的管理者,  channel 可以是Video, Audio 或者 data Channel.  

      Stats 是统计信息 

       WebRtcSession 是PeerConnection内部最重要的成员, 它负责SDP创建, 解析,  ICE Client 等等功能,   而且核心的Voice,  Video,  Data Channel 也在其内部工作.  

      CreateOffer,  CreateAnswer 用来创建本端的SDP,  

      SetLocalDescription和SetRemoteDescription 是完成媒体协商和 ICE 过程的必要步骤,  通过这两步, 就可以建立peer to peer的媒体连接.   



![](http://img.blog.csdn.net/20161023125840527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

from:http://blog.csdn.net/volvet/article/details/52901133









