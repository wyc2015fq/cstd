# live555_RTSP连接建立以及请求消息处理过程 - 建建的博客 - CSDN博客
2017年02月19日 18:09:32[纪建](https://me.csdn.net/u013898698)阅读数：356
1，RTSP连接的建立过程
    RTSPServer类用于构建一个RTSP服务器，该类同时在其内部定义了一个RTSPClientSession类，用于处理单独的客户会话。
    首先创建RTSP服务器(具体实现类是DynamicRTSPServer)，在创建过程中，先建立Socket(ourSocket)在TCP的554端口进行监听，然后把连接处理函数句柄
(RTSPServer:: incomingConnectionHandler)和socket句柄传给任务调度器(taskScheduler)。
    任务调度器把socket句柄放入后面select调用中用到的socket句柄集(fReadSet)中，同时将socket句柄和incomingConnectionHandler句柄关联起来。接着，主程序开始进入任务调度器的主循环（doEventLoop），在主循环中调用系统函数select阻塞，等待网络连接。
    当RTSP客户端输入(rtsp://192.168.1.109/1.mpg)连接服务器时，select返回对应的scoket，进而根据前面保存的对应关系，可找到对应处理函数句柄，这里就是前面提到的incomingConnectionHandler了。在incomingConnectionHandler中创建了RTSPClientSession，开始对这个客户端的会话进行处理。
具体分析如下：
DynamicRTSPServer::creatnew()：
   1.调用继承自RTPSever::setUpOurSocket：
       1.调用GroupsockHelper 的setupStreamSocket创建一个socket连接，并绑定，
       2.设置socket的发送缓存大小，
       3.调用listen开始监听端口，设置同时最大能处理连接数LISTEN_BACKLOG_SIZE=20，如果达到这个上限则client端将收到ECONNERREFUSED的错误
       4.测试绑定端口是否为0，为0的话重新绑定断口，并返回系统自己选择的新的端口。
       5.返回建立的socket文件描述符
2.调用自己和RTPSever的构造函数：
   RTPSever构造函数:
       1.用一个UsageEnvironment对象的引用构造其父类Medium
       2.设置最大等待回收连接时间reclamationTestSeconds，超过这个时间从客户端没有RTSP命令或者RTSP的RR包则收回其RTSPClientSession
       3.建立一个HashTable（实际上是一个BasicHashTable）,fServerMediaSessions指向这个表。
       4.调用参数UsageEnvironment对象env的成员,一个TaskScheduler指针所指对象（实际就是一个BasicTaskScheduler对象）的成员函数
turnOnBackgroundReadHandling()：
               1.调用一个HandlerSet::assignHandler()创建一个Handler，把socketNum【此处为服务器监听的socket描述符】和处理函数RTSPServer::incomingConnectionHandler()，还有指向RTSPSever的指针绑定在一起。
   incomingConnectionHandler作用：
                       1.调用accept返回服务器与客户端连接的socket描述符
                       2.设置客户端描述符为非阻塞
                       3.增加客户端socket描述符的发送缓存为50*1024
                       4.为此客户端随机分配一个sessionId
                       5.用客户端socket描述符clientSocket,sessionId,和客户端地址clientAddr调用creatNewClientSession创建一个clientSession。
2，请求消息处理过程
    上节我们谈到RTSP服务器收到客户端的连接请求，建立了RTSPClientSession类，处理单独的客户会话。在建立 RTSPClientSession的过程中，将新建立的socket句柄（clientSocket）和RTSP请求处理函数句柄RTSPClientSession::incomingRequestHandler传给任务调度器，由任务调度器对两者进行一对一关联。当客户端发出 RTSP请求后，服务器主循环中的select调用返回，根据socket句柄找到对应的incomingRequestHandler，开始消息处理。先进行消息的解析。
RTSPClientSession::RTSPClientSession()构造函数：
   1.重置请求缓存
   2.调用envir().taskScheduler().turnOnBackgroundReadHandling()，这次socketnumber为客户端socket描述符这次的处理函数是RTSPServer::RTSPClientSession::incomingRequestHandler()
  RTSPServer::RTSPClientSession::incomingRequestHandler():
           调用handleAlternativeRequestByte1(uint8_t requestByte)：
               1.fRequestBuffer[fRequestBytesAlreadySeen] =requestByte;把请求字符放入请求缓存fRequestBuffer
               2.调用handleRequestBytes(1) 处理请求缓存
handleRequestBytes(int newBytesRead):
                       1.调用noteLiveness()查看请求是否到期,如果服务器的reclamationTestSeconds>
 0，调用taskScheduler对象的rescheduleDelayedTask函数: 参数为
( fLivenessCheckTask,fOurServer.fReclamationTestSeconds*1000000,(TaskFunc*)livenessTimeoutTask, this )
其中livenessTimeoutTask()函数作用是删除new出来的clientSession.
                           1.调用unscheduleDelayedTask(TaskToken&prevTask)：
                               从DelayQueue中删除prevTask项， prevTask置空.
                           2.调用scheduleDelayedTask(int64_t microseconds, 
                                                     TaskFunc* proc, void*clientData)： 
                               1.创建一个DelayInterval对象timeToDelay，用microseconds初始化。
                               2.创建一个AlarmHandler对象，用proc, clientData, timeToDelay初始化
                               3.调用fDelayQueue.addEntry(),把这个AlarmHandler对象加入到延迟队列中
                               4.返回AlarmHandler对象的token[long类型]的指针
                     2.如果请求的的长度超过请求缓存可读长度fRequestBufferBytesLeft，结束这个连接。
               3.找到请求消息的结尾：。
               4.如果找到消息结尾，调用RTSPServer::RTSPClientSession::handleRequestBytes()[值得关注此函数]把请求字符串转换成命令各部分包括：cmdName[方法]，urlPreSuffix[url地址]，urlSuffix[要读取的文件名]，sceq[消息的Cseq]，否则函数返回需要继续从连接中读取请求。分别存入对应的数组。
               5.如果转换成功，调用handleCmd_xxx()处理对应的cmdName: xxx[此处实现了：OPTIONS，DESCRIBE，SETUP，TEARDOWN，PLAY，PAUSE，GET_PARAMETER，SET_PARAMETER]
               其中PLAY，PAUSE，GET_PARAMETER，SET_PARAMETER调用handleCmd_withinSession
 (cmdName,urlPreSuffix, urlSuffix,cseq,(char const*)fRequestBuffer);
               6.清空 RequestBuffer
比如：消息解析后，如果发现客户端的请求是DESCRIBE则进入handleCmd_DESCRIBE函数。RTSP服务器收到客户端的DESCRIBE请求后，根据请求URL(rtsp://192.168.1.109/1.mpg)，找到对应的流媒体资源，返回响应消息。live555中的ServerMediaSession类用来处理会话中描述，它包含多个（音频或视频）的子会话描述(ServerMediaSubsession)。根据客户端请求URL的后缀(例如是1.mpg), 调用成员函数
                  DynamicRTSPServer::lookupServerMediaSession查找对应的流媒体信息 ServerMediaSession。（根据urlSuffix查找）。
如果ServerMediaSession不存在，查找文件是否存在，若文件不存在，则判断ServerMediaSession         （即smsExists）是否存在，如果存在则将其remove（调用removeServerMediaSession方法）。但是如果本地存在1.mpg文件，则根据文件名创建一个新的 ServerMediaSession（调用createNewSMS方法，若在该方法中找不到对应的文件扩展名，则将返回NULL）。
如果通过lookupServerMediaSession返回的是NULL，则向客户端发送响应消息并将fSessionIsActive置为FALSE；否则，为该session组装一个SDP描述信息（调用generateSDPDescription方法，该方法在ServerMediaSession类中），组装完成后，生成一个RTSP
 URL（调用rtspURL方法，该方法在RTSPServer类中）。
在创建ServerMediaSession过程中，根据文件后缀.mpg，创建媒体MPEG-1or2的解复用器
                   (MPEG1or2FileServerDemux)。再由MPEG1or2FileServerDemux创建一个子会话描述 MPEG1or2DemuxedServerMediaSubsession。最后由ServerMediaSession完成组装响应消息中的SDP信息（SDP组装过程见下面的描述），然后将响应消息发给客户端，完成一次消息交互。
===================================================================================================================================
RTSP服务器处理客户端点播的基本流程
处理连接请求的基本流程：
l  Step 1：与客户端建立RTSP连接（调用incomingConnectionHandler方法），创建ClientSession并关联fClientSocket与incomingRequestHandler（调用incomingConnectionHandler1）。
l  Step 2：接收客户端请求（调用incomingRequestHandler方法）。
l  Step 3：从客户端Socket读取数据，并对请求数据（即the request string）进行转换（调用parseRTSPRequestString方法，该方法在RTSPCommon类中）。
l  Step 4：根据分离出来的指令进行分别处理：
n  OPTIONS→handleCmd_OPTIONS
n  DESCRIBE→handleCmd_DESCRIBE
handleCmd_DESCRIBE这一个方法比较重要，首先根据urlSuffix查找ServerMediaSession是否存在（调用lookupServerMediaSession方法，该方法中通过HashTable来查找）。
在testOnDemandRTSPServer项目工程中，仅仅是通过streamName来确认session是否为NULL。而在完整的live555MediaServer项目工程中，则是通过DynamicRTSPServer类来处理，其首先是查找文件是否存在，若文件不存在，则判断ServerMediaSession（即smsExists）是否存在，如果存在则将其remove（调用removeServerMediaSession方法）；若文件存在，则根据文件名创建一个ServerMediaSession（调用createNewSMS方法，若在该方法中找不到对应的文件扩展名，则将返回NULL）。
如果通过lookupServerMediaSession返回的是NULL，则向客户端发送响应消息并将fSessionIsActive置为FALSE；否则，为该session组装一个SDP描述信息（调用generateSDPDescription方法，该方法在ServerMediaSession类中），组装完成后，生成一个RTSP URL（调用rtspURL方法，该方法在RTSPServer类中）。
n  SETUP→handleCmd_SETUP
handleCmd_SETUP方法中，有两个关键的名词，一个是urlPreSuffix，代表了session name（即stream name）；一个是urlSuffix，代表了subsession name（即track name），后面经常用到的streamName和trackId分别与这两个名词有关。
接下来会创建session's state，包括incrementReferenceCount等。紧接着，会针对确定的subsession（track）查找相应的信息。接着，在request string查找一个"Transport:" header，目的是为了从中提取客户端请求的一些参数（调用parseTransportHeader方法，该方法在RTSPServer类中），如clientsDestinationAddressStr、ClientRTPPortNum等。
再接着是getStreamParameters（该方法在ServerMediaSession类中被定义为纯虚函数并在OnDemandServerMediaSubsession类中被重定义），然后通过fIsMulticast和streamingMode来组装不同的响应消息。
n  PLAY→handleCmd_PLAY：处理播放请求，具体的实现流程请参见后面的步骤。
n  PAUSE→handleCmd_PAUSE：处理暂停请求，在执行了该请求后，最终会调用StopPlaying方法，并将fAreCurrentlyPlaying置为FALSE。
n  TEARDOWN→handleCmd_TEARDOWN：处理停止请求，将fSessionIsActive置为FALSE。
n  GET_PARAMETER→handleCmd_GET_PARAMETER：该方法主要是维持客户端与服务器通信的生存状态，just for keep alive。
n  SET_PARAMETER→handleCmd_SET_PARAMETER：该方法未针对SET_PARAMETER作实现，使用该方法会调用handleCmd_notSupported方法，并将最终引发与客户端断开连接。
l  Step 5：根据Step 4的不同指令进行消息响应（调用send方法），该消息响应是即时的。
l  Step 6：处理客户端发送“SETUP”指令后即开始播放的特殊情况。
l  Step 7：将RequestBuffer进行重置，以便于为之后到来的请求做好准备。
l  Step 8：检查fSessionIsActive是否为FALSE，如果是则删除当前的ClientSession。
处理PLAY的基本流程：
l  Step 1：对rtspURL及相关header的处理，涉及较多的细节。
l  Step 2：根据不同的header对流进行缩放比例或定位的处理。
如果为sawScaleHeader，则进行缩放比例的处理（调用setStreamScale方法，该方法在OnDemandServerMediaSubsession类中实现）。
如果为sawRangeHeader，则进行寻找流的处理（即是对流进行定位，调用seekStream方法，该方法在OnDemandServerMediaSubsession类中实现；同时，该方法的调用是在初始播放前及播放过程中由于用户拖动播放进度条而产生的系列请求）。
在OnDemandServerMediaSubsession类中，seekStream方法中调用了seekStreamSource方法，该方法在对应的媒体格式文件的FileServerMediaSubsession类中实现（如针对WAV格式，则在WAVAudioFileServerMediaSubsession类中实现；针对MP3格式，则在MP3AudioFileServerMediaSubsession类中实现）。
同理，OnDemandServerMediaSubsession类中的setStreamScale方法中所调用的setStreamSourceScale方法亦是类似的实现机制。
l  Step 3：开始进行流式播放（调用startStream方法，该方法在OnDemandServerMediaSubsession类中实现）。
n  Step 3.1：根据clientSessionId从fDestinationsHashTable中查找到destinations（包括了客户端的IP地址、RTP端口号、RTCP端口号等信息）。
n  Step 3.2：调用startPlaying方法，在该方法中根据RTPSink或UDPSink分别调用startPlaying方法。
如果是调用RTPSink的startPlaying方法，则接着会调用MediaSink类中的startPlaying方法，并在该方法中调用MultiFramedRTPSink类中的continuePlaying方法，之后便是buildAndSendPacket了。这里已经来到重点了，即是关于不断读取Frame并Send的要点。在MultiFramedRTPSink类中，通过buildAndSendPacket、packFrame、afterGettingFrame、afterGettingFrame1、sendPacketIfNecessary和sendNext构成了一个循环圈，数据包的读取和发送在这里循环进行着。特别注意的是sendPacketIfNecessary方法中的后面代码（nextTask()
 = envir().taskScheduler().scheduleDelayedTask(uSecondsToGo, (TaskFunc*)sendNext, this);），通过Delay amount of time后，继续下一个Task，并回过来继续调用buildAndSendPacket方法。
在packFrame方法中，正常情况下，需要调用getNextFrame方法（该方法在FramedSource类中，并且对不同媒体格式的Frame的获取出现在FramedSource类的getNextFrame方法中，通过调用doGetNextFrame方法来实现）来获取新的Frame。
如果是调用UDPSink的startPlaying方法，则接着会调用MediaSink类中的startPlaying方法，并在该方法中调用BasicUDPSink类中的continuePlaying方法。在这之后由若干个方法构成了一个循环圈：continuePlaying1、afterGettingFrame、afterGettingFrame1、sendNext。并在afterGettingFrame1方法中实现了packet的发送（fGS->output(envir(), fGS->ttl(),fOutputBuffer, frameSize);）。
Step 3.3：针对RTPSink创建RTCP instance（RTP与RTCP的配套使用决定了其必须这么做，否则可能就跟直接使用UDP发送数据包没什么两样了^_^），创建RTCP instance时，将incomingReportHandler句柄作为BackgroundHandlerProc，以便于处理RTCP的报告，并开始startNetworkReading。这里RTP/RTCP的使用方式有两种，一种建立在TCP之上，一种建立在UDP之上。
![](http://images2015.cnblogs.com/blog/713723/201607/713723-20160720155831451-498405145.png)
