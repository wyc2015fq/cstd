# live555服务器端、客户端源代码分析总结 - 建建的博客 - CSDN博客
2017年03月01日 18:01:48[纪建](https://me.csdn.net/u013898698)阅读数：202
原文地址：http://www.cnblogs.com/lidabo/p/4103461.html
RTSP服务器端流程：
1. RTSP连接的建立过程
      RTSPServer类用于构建一个RTSP服务器，该类同时在其内部定义了一个RTSPClientSession类，用于处理单独的客户会话。
      首先创建RTSP服务器(具体实现类是DynamicRTSPServer)，在创建过程中，先建立Socket(ourSocket)在TCP的554 端口进行监听，然后把连接处理**函数**句柄(RTSPServer::incomingConnectionHandler)和socket句柄传给任务调度器(taskScheduler)。
      任务调度器把socket句柄放入后面select调用中用到的socket句柄集(fReadSet)中，同时将socket句柄和 incomingConnectionHandler句柄关联起来。 接着，主**程序**开始进入任务调度器的主循环（doEventLoop），在主循环中调用**系统**函数select阻塞，等待**网络**连接。
      当RTSP客户端输入(rtsp://192.168.0.1/1.mpg)连接**服务器**时，select返回对应的scoket，进而根据前面保存的对应关系，可找到对应处理函数句柄，这里就是前面提到的incomingConnectionHandler了。在
 incomingConnectionHandler中创建了RTSPClientSession，开始对这个客户端的会话进行处理。
2. DESCRIBE请求消息处理过程
      RTSP服务器收到客户端的DESCRIBE请求后，根据请求URL(rtsp://192.168.0.1/1.mpg)，找到对应的流媒体资源，返回响应消息。live555中的ServerMediaSession类用来处理会话中描述，它包含多个（音频或**视频**）的子会话描述
 (ServerMediaSubsession)。
      RTSP服务器收到客户端的连接请求，建立了RTSPClientSession类，处理单独的客户会话。在建立RTSPClientSession的过程中，将新建立的socket句柄（clientSocket）和RTSP请求处理函数句柄 RTSPClientSession::incomingRequestHandler传给任务调度器，由任务调度器对两者进行一对一关联。
      当客户端发出RTSP请求后，服务器主循环中的select调用返回，根据socket句柄找到对应的incomingRequestHandler，开始消息处理。先进行消息的**解析**，如果发现请求是DESCRIBE则进入handleCmd_DESCRIBE函数。根据客户端请求URL的后缀(如
 1.mpg),调用成员函数DynamicRTSPServer::lookupServerMediaSession查找对应的流媒体信息 ServerMediaSession。如果ServerMediaSession不存在，但是本地存在1.mpg文件，则创建一个新的 ServerMediaSession。在创建ServerMediaSession过程中，根据**文件**后缀.mpg，创建媒体MPEG-1or2的解复用器(MPEG1or2FileServerDemux)。再由MPEG1or2FileServerDemux创建一个子会话描述
 MPEG1or2DemuxedServerMediaSubsession。最后由ServerMediaSession完成组装响应消息中的SDP信息（SDP组装过程见下面的描述），然后将响应消息发给客户端，完成一次消息交互。
SDP消息组装过程：
      ServerMediaSession负责产生会话公共描述信息，子会话描述由 MPEG1or2DemuxedServerMediaSubsession产生。 MPEG1or2DemuxedServerMediaSubsession在其父类成员函数 OnDemandServerMediaSubsession::sdpLines()中生成会话描述信息。在sdpLines()实现里面，创建一个虚构(dummy)的FramedSource(具体实现类为MPEG1or2AudioStreamFramer和 MPEG1or2VideoStreamFramer)和RTPSink（具体实现类为MPEG1or2AudioRTPSink和
 MPEG1or2VideoRTPSink），最后调用setSDPLinesFromRTPSink(...)成员函数生成子会话描述。
3. SETUP请求消息处理过程
        RTSPClientSession类用于处理单独的客户会话。其类成员函数handleCmd_SETUP()处理客户端的SETUP请求。调用 parseTransportHeader()对SETUP请求的传输头解析，调用子会话(这里具体实现类为 OnDemandServerMediaSubsession)的getStreamParameters()函数获取流媒体发送传输参数。将这些参数组装成响应消息，返回给客户端。
        获取发送传输参数的过程：调用子会话(具体实现类MPEG1or2DemuxedServerMediaSubsession)的 createNewStreamSource(...)创建MPEG1or2VideoStreamFramer，选择发送传输参数，并调用子会话的 createNewRTPSink(...)创建MPEG1or2VideoRTPSink。同时将这些信息保存在StreamState类对象中，用于记录流的状态。
        客户端发送两个SETUP请求，分别用于建立音频和视频的RTP接收。
4. PLAY请求消息处理过程
      RTSPClientSession类成员函数handleCmd_PLAY()处理客户端的播放请求。首先调用子会话的startStream(),内部调用MediaSink::startPlaying(...)，然后是 MultiFramedRTPSink::continuePlaying()，接着调用 MultiFramedRTPSink::buildAndSendPacket(...)。buildAndSendPacke内部先设置RTP包头，内部再调用MultiFramedRTPSink::packFrame()填充编码帧**数据**。
      packFrame内部通过FramedSource::getNextFrame(), 接着MPEGVideoStreamFramer::doGetNextFrame()，再接着经过 MPEGVideoStreamFramer::continueReadProcessing()， FramedSource::afterGetting(...), MultiFramedRTPSink::afterGettingFrame(...), MultiFramedRTPSink::afterGettingFrame1(...)等一系列繁琐调用，最后到了
 MultiFramedRTPSink::sendPacketIfNecessary(), 这里才真正发送RTP数据包。然后是计算下一个数据包发送时间，把MultiFramedRTPSink::sendNext(...)函数句柄传给任务调度器，作为一个延时事件调度。在主循环中，当MultiFramedRTPSink::sendNext()被调度时，又开始调用 MultiFramedRTPSink::buildAndSendPacket(...)开始新的发送数据过程，这样客户端可以源源不断的收到服务器传来的RTP包了。
openRTSP客户端流程：
1、创建TaskScheduler和BasicUsageEnvironment类；
2、命令行解析，获取流媒体地址和其他选项；　　
3、创建RTSPClient对象；　　
4、如果需要，RTSPClient对象发送OPTIONS命令并解析服务端响应，获取可以使用命令集。　
5、RTSPClient对象发送DESCRIBE命令，并从获服务端反馈中获取流媒体相关描述SDP字串。　
6、创建MediaSession对象，解析SDP字串，创建了相应的子会话对象。在这个过程中还完成了RTP和RTCP通信使用的GroupSock对象的创建，包括协议和端口的选择。　
7、根据流媒体不同类型，实例化具体的RTP会话的Source和Sink对象。　
8、RTSPClient对象发送SETUP和PLAY命令，服务端开始传输流媒体数据。　　
9、TaskScheduler开始事件处理循环，通过select监听数据包到达并调用注册函数进行处理。
