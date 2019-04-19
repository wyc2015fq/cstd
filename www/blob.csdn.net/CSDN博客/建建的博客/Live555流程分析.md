# Live555流程分析 - 建建的博客 - CSDN博客
2017年02月21日 15:29:13[纪建](https://me.csdn.net/u013898698)阅读数：2797
[http://www.cnblogs.com/jqctop1/p/4385557.html](http://www.cnblogs.com/jqctop1/p/4385557.html)
前一阵子，因为项目需要，研究了一下Live555开源框架，研究的不是很深入，基本上把Live555当做API用了一下，但是毕竟也是本人看的第一个开源框架，在此记录总结一下。
　　Live555是一个实现了RTSP协议的开源流媒体框架，Live555包含RTSP服务器端的实现以及RTSP客户端的实现。Live555可以将若干种格式的视频文件或者音频文件转换成视频流或者音频流在网络中通过RTSP协议分发传播，这便是流媒体服务器最核心的功能。Live555支持以下几种文件格式的流化：　　
- A MPEG Transport Stream file (with file name suffix ".ts")
- A Matroska or WebM file (with filename suffix ".mkv" or ".webm")
- An Ogg file (with filename suffix ".ogg", "ogv", or ".opus")
- A MPEG-1 or 2 Program Stream file (with file name suffix ".mpg")
- A MPEG-4 Video Elementary Stream file (with file name suffix ".m4e")
- A H.264 Video Elementary Stream file (with file name suffix ".264")
- A H.265 Video Elementary Stream file (with file name suffix ".265")
- A VOB video+audio file (with file name suffix ".vob")
- A DV video file (with file name suffix ".dv")
- A MPEG-1 or 2 (including layer III - i.e., 'MP3') audio file (with file name suffix ".mp3")
- A WAV (PCM) audio file (with file name suffix ".wav")
- An AMR audio file (with file name suffix ".amr")
- An AC-3 audio file (with file name suffix ".ac3")
- An AAC (ADTS format) audio file (with file name suffix ".aac")
　　经过Live555流化后的视频流或者音频流可以通过实现了标准RTSP协议的播放器（如VLC）来播放。
　　Live555的官网：[http://www.live555.com/](http://www.live555.com/)，下载Live555的源代码：[http://www.live555.com/liveMedia/public/](http://www.live555.com/liveMedia/public/)
      下载源码后解压得到live目录，目录结构如下，lib目录是编译后产生的目录：
![](http://images.cnitblog.com/blog2015/688670/201504/012155345298460.jpg)
      主要使用其中的四个目录，分别对应Live555的四个库：
　　　　UsageEnvironment目录，生成的静态库为libUsageEnvironment.lib，这个库主要包含一些基本[数据结构](http://lib.csdn.net/base/datastructure)以及工具类的定义
　　　　groupsock目录，生成的静态库为libgroupsock.lib，这个库主要包含网络相关类的定义和实现
　　　　liveMedia目录，生成的静态库为libliveMedia.lib，这个库包含了Live555核心功能的实现
　　　　BasicUsageEnvironment目录，生成的静态库为libBasicUsageEnvironment.lib，这个库主要包含对UsageEnvironment库中一些类的实现
　　mediaServer目录中包含Live555流媒体服务器的标准示例程序，运行live555MediaServer.exe后出现如下界面：
![](http://images.cnitblog.com/blog2015/688670/201504/012216550141765.jpg)
　　在mediaServer目录中放入你的媒体文件，如test.mp3，在VLC播放器中选择“媒体”-“打开网络串流”，然后输入 rtsp://127.0.0.1:8554/test.mp3 就可以播放刚才的mp3文件了。
　　proxyServer目录中是live555实现的代理服务器的例子程序，这个程序可以从其他的流媒体服务器（如支持RTSP的摄像机）取实时的视频流然后转发给多个RTSP客户端，这个程序很有用，可以转发摄像机的实时视频流。
　　testProgs目录中包含很多的[测试](http://lib.csdn.net/base/softwaretest)例子程序，我经常用的是testOnDemandRTSPServer.cpp，我是从这个例子程序开始学习Live555的。
首先，看看这个程序的说明：
　　// A test program that demonstrates how to stream - via unicast RTP
　　// - various kinds of file on demand, using a built-in RTSP server.
就是说这个程序演示了如何利用RTSPServer这个类来对媒体文件进行单播，OnDemand的意思是收到RTSP客户端请求时才进行流化和单播。
　　下面，首先看main函数，很简单，主要包含以下几个步骤：
```
1 // Begin by setting up our usage environmen
 2 // 创建工具类
 3 TaskScheduler* scheduler = BasicTaskScheduler::createNew();
 4 env = BasicUsageEnvironment::createNew(*scheduler);
 5 // Create the RTSP server:
 6 // 创建RTSPServer，指定端口为8554
 7 RTSPServer* rtspServer = RTSPServer::createNew(*env, 8554, authDB);
 8 if (rtspServer == NULL) {
 9 *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
10 exit(1);
11 }
12 
13 char const* descriptionString
14 = "Session streamed by \"testOnDemandRTSPServer\"";
15 
16 // Set up each of the possible streams that can be served by the
17 // RTSP server. Each such stream is implemented using a
18 // "ServerMediaSession" object, plus one or more
19 // "ServerMediaSubsession" objects for each audio/video substream.
20 
21 
22 /* 为每一种媒体文件创建会话，简单理解就是：一个ServerMediaSession对象对应一个媒体文件，一个媒体文件中可能同时包含音频和视频，对于每个视频或者音频，对应一个ServerMediaSubsession对象，
23 一个ServerMediaSession中可以包含多个ServerMediaSubsession对象 */
24 // 这里我们只看H.264文件
25 
26 // A H.264 video elementary stream:
27 {
28   char const* streamName = "h264ESVideoTest";                   //标识请求播放该媒体文件时使用的名称
29   char const* inputFileName = "test.264";                       //默认媒体文件名为test.264
30   ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName,descriptionString);      //为该媒体文件创建一个ServerMediaSession
31   /* .264文件只包含视频，创建一个ServerMediaSubsession对象并添加到ServerMediaSession
32    H264VideoFileServerMediaSubsession是ServerMediaSubsession的子类，针对不同格式有不同的实现类 */
33   sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(*env, inputFileName, reuseFirstSource)); 
34   rtspServer->addServerMediaSession(sms);                       //将刚才创建的ServerMediaSession添加到RTSPServer中
35   announceStream(rtspServer, sms, streamName, inputFileName);   //打印出播放该媒体文件的rtsp地址 
36 }
37 
38 // 程序从下面开始进入一个主循环，后面的return 0不会被执行。
39 env->taskScheduler().doEventLoop(); // does not return
40 return 0; // only to prevent compiler warning
 
　　Live555是单线程的，基于事件驱动模式，程序从doEventLoop函数出进入无限循环，在这个循环中不断地查看事件队列是否有事件需要去处理，有就去处理，没有则休眠一小会儿，看下doEventLoop函数，该函数在live/BasicUsageEnvironment/BasicTaskScheduler0.cpp文件中定义。
```
1 void BasicTaskScheduler0::doEventLoop(char* watchVariable) {
2   // Repeatedly loop, handling readble sockets and timed events:
3   while (1) {
4     if (watchVariable != NULL && *watchVariable != 0) break;
5     SingleStep();
6     //SingleStep函数中，对可读数据的socket进行读数据，对事件队列中的事件调用对应的处理函数处理
7   }
8 }
 　　主循环部分的代码比较简单，那我们就需要仔细看看创建RTSPServer，创建ServerMediaSession以及ServerMediaSubsession这部分的代码，看这部分代码之前，我们需要先对RTSP协议的建立连接过程有个大概的了解，在此我就不再详述，网上有很多讲解这个过程的博文，在此推荐一个：[http://www.cnblogs.com/qq78292959/archive/2010/08/12/2077039.html](http://www.cnblogs.com/qq78292959/archive/2010/08/12/2077039.html)
　　RTSPServer类即表示一个流媒体服务器实例，RTSPServer::createNew是一个简单工厂函数，使用指定的端口（8554）创建一个TCP的socket用于等待客户端的连接，然后new一个RTSPServer实例。
```
1 RTSPServer* RTSPServer::createNew(UsageEnvironment& env, Port ourPort,
2               UserAuthenticationDatabase* authDatabase,
3               unsigned reclamationTestSeconds) {
4   int ourSocket = setUpOurSocket(env, ourPort);
5   if (ourSocket == -1) return NULL;
6   
7   return new RTSPServer(env, ourSocket, ourPort, authDatabase, reclamationTestSeconds);
8 }
　　RTSPServer的构造函数：
```
1 RTSPServer::RTSPServer(UsageEnvironment& env,
 2                int ourSocket, Port ourPort,
 3                UserAuthenticationDatabase* authDatabase,
 4                unsigned reclamationTestSeconds)
 5   : Medium(env),
 6     fRTSPServerPort(ourPort), fRTSPServerSocket(ourSocket), fHTTPServerSocket(-1), fHTTPServerPort(0),
 7     fServerMediaSessions(HashTable::create(STRING_HASH_KEYS)),
 8     fClientConnections(HashTable::create(ONE_WORD_HASH_KEYS)),
 9     fClientConnectionsForHTTPTunneling(NULL), // will get created if needed
10     fClientSessions(HashTable::create(STRING_HASH_KEYS)),
11     fPendingRegisterRequests(HashTable::create(ONE_WORD_HASH_KEYS)), fRegisterRequestCounter(0),
12     fAuthDB(authDatabase), fReclamationTestSeconds(reclamationTestSeconds),
13     fAllowStreamingRTPOverTCP(True) {
14   ignoreSigPipeOnSocket(ourSocket); // so that clients on the same host that are killed don't also kill us
15   
16   // Arrange to handle connections from others:
17   env.taskScheduler().turnOnBackgroundReadHandling(fRTSPServerSocket,
18                            (TaskScheduler::BackgroundHandlerProc*)&incomingConnectionHandlerRTSP, this);
19 }
　　这里主要看一下turnOnBackgroundReadHandling函数，这个函数的作用即将某个socket加入SOCKET  SET（参见select模型），并指定相应的处理函数。这里的处理函数即收到RTSP客户端连接请求时的回调处理函数incomingConnectionHandlerRTSP，第三个参数作为回调函数的参数。
　　ServerMediaSession::createNew是一个简单工厂模式函数，在其中new了一个ServerMediaSession对象，看一下ServerMediaSession这个类的定义。
```
1 class ServerMediaSession: public Medium {
 2 public:
 3   static ServerMediaSession* createNew(UsageEnvironment& env,
 4                        char const* streamName = NULL,
 5                        char const* info = NULL,
 6                        char const* description = NULL,
 7                        Boolean isSSM = False,
 8                        char const* miscSDPLines = NULL);
 9 
10   static Boolean lookupByName(UsageEnvironment& env,
11                               char const* mediumName,
12                               ServerMediaSession*& resultSession);
13 
14   char* generateSDPDescription(); // based on the entire session　　　　　　　　　　//产生媒体描述信息（SDP），在收到DESCRIBE命令后回复给RTSP客户端
15       // Note: The caller is responsible for freeing the returned string
16   
17   char const* streamName() const { return fStreamName; }　　                     // 返回流的名称
18 
19   Boolean addSubsession(ServerMediaSubsession* subsession);　　　　　　　　　　　　 // 添加表示子会话的ServerMediaSubsession对象
20   unsigned numSubsessions() const { return fSubsessionCounter; }
21 
22   void testScaleFactor(float& scale); // sets "scale" to the actual supported scale
23   float duration() const;　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　// 返回流的持续时间
24     // a result == 0 means an unbounded session (the default)
25     // a result < 0 means: subsession durations differ; the result is -(the largest).
26     // a result > 0 means: this is the duration of a bounded session
27 
28   unsigned referenceCount() const { return fReferenceCount; }                      // 返回请求该流的RTSP客户端数目
29   void incrementReferenceCount() { ++fReferenceCount; }
30   void decrementReferenceCount() { if (fReferenceCount > 0) --fReferenceCount; }
31   Boolean& deleteWhenUnreferenced() { return fDeleteWhenUnreferenced; }            // fDeleteWhenUnreferenced表示在没有客户端请求该流时，是否从RTSPServer中删除该流
32 
33   void deleteAllSubsessions();
34     // Removes and deletes all subsessions added by "addSubsession()", returning us to an 'empty' state
35     // Note: If you have already added this "ServerMediaSession" to a "RTSPServer" then, before calling this function,
36     //   you must first close any client connections that use it,
37     //   by calling "RTSPServer::closeAllClientSessionsForServerMediaSession()".
38 
39 protected:
40   ServerMediaSession(UsageEnvironment& env, char const* streamName,
41              char const* info, char const* description,
42              Boolean isSSM, char const* miscSDPLines);
43   // called only by "createNew()"
44 
45   virtual ~ServerMediaSession();
46 
47 private: // redefined virtual functions
48   virtual Boolean isServerMediaSession() const;
49 
50 private:
51   Boolean fIsSSM;
52 
53   // Linkage fields:
54   friend class ServerMediaSubsessionIterator;                           //  ServerMediaSubsessionIterator是一个用于访问ServerMediaSubsession对象的迭代器
55   ServerMediaSubsession* fSubsessionsHead;
56   ServerMediaSubsession* fSubsessionsTail;
57   unsigned fSubsessionCounter;
58 
59   char* fStreamName;
60   char* fInfoSDPString;
61   char* fDescriptionSDPString;
62   char* fMiscSDPLines;
63   struct timeval fCreationTime;
64   unsigned fReferenceCount;
65   Boolean fDeleteWhenUnreferenced;
66 };
　　ServerMediaSession的构造函数比较简单，主要就是初始化一些成员变量，产生一些对该媒体流的描述信息，然后我们来看一下ServerMediaSubsession这个类。
```
1 class ServerMediaSubsession: public Medium {
 2 public:
 3   unsigned trackNumber() const { return fTrackNumber; }            //每个ServerMediaSubsession又叫一个track，有一个整型标识号trackNumber 4   char const* trackId(); 　　　　                                   // trackID函数返回trackNumber的字符串形式，用于填充SDP中的trackID字段
 5   virtual char const* sdpLines() = 0;                              // 产生关于该视频流或者音频流的描述信息(SDP)
 6   virtual void getStreamParameters(unsigned clientSessionId, // in
 7                    netAddressBits clientAddress, // in
 8                    Port const& clientRTPPort, // in
 9                    Port const& clientRTCPPort, // in
10                    int tcpSocketNum, // in (-1 means use UDP, not TCP)
11                    unsigned char rtpChannelId, // in (used if TCP)
12                    unsigned char rtcpChannelId, // in (used if TCP)
13                    netAddressBits& destinationAddress, // in out
14                    u_int8_t& destinationTTL, // in out
15                    Boolean& isMulticast, // out
16                    Port& serverRTPPort, // out
17                    Port& serverRTCPPort, // out
18                    void*& streamToken // out
19                    ) = 0;
20   virtual void startStream(unsigned clientSessionId, void* streamToken,                      // 开始流化
21                TaskFunc* rtcpRRHandler,
22                void* rtcpRRHandlerClientData,
23                unsigned short& rtpSeqNum,
24                unsigned& rtpTimestamp,
25                ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
26                void* serverRequestAlternativeByteHandlerClientData) = 0;
27   virtual void pauseStream(unsigned clientSessionId, void* streamToken);                     // 暂停流化
28   virtual void seekStream(unsigned clientSessionId, void* streamToken, double& seekNPT,      // 从指定位置处开始流化（对应的操作即客户端指定从进度条上的某一个点开始播放）
29               double streamDuration, u_int64_t& numBytes);
30      // This routine is used to seek by relative (i.e., NPT) time.
31      // "streamDuration", if >0.0, specifies how much data to stream, past "seekNPT".  (If <=0.0, all remaining data is streamed.)
32      // "numBytes" returns the size (in bytes) of the data to be streamed, or 0 if unknown or unlimited.
33   virtual void seekStream(unsigned clientSessionId, void* streamToken, char*& absStart, char*& absEnd);
34      // This routine is used to seek by 'absolute' time.
35      // "absStart" should be a string of the form "YYYYMMDDTHHMMSSZ" or "YYYYMMDDTHHMMSS.<frac>Z".
36      // "absEnd" should be either NULL (for no end time), or a string of the same form as "absStart".
37      // These strings may be modified in-place, or can be reassigned to a newly-allocated value (after delete[]ing the original).
38   virtual void nullSeekStream(unsigned clientSessionId, void* streamToken,
39                   double streamEndTime, u_int64_t& numBytes);
40      // Called whenever we're handling a "PLAY" command without a specified start time.
41   virtual void setStreamScale(unsigned clientSessionId, void* streamToken, float scale);
42   virtual float getCurrentNPT(void* streamToken);
43   virtual FramedSource* getStreamSource(void* streamToken);                                      // FramedSource从名字即可以看出它即每一帧视频流的来源（视频或者音频数据的来源）
44   virtual void deleteStream(unsigned clientSessionId, void*& streamToken);
45 
46   virtual void testScaleFactor(float& scale); // sets "scale" to the actual supported scale
47   virtual float duration() const;                                                                // 返回该子会话的持续时间
48     // returns 0 for an unbounded session (the default)
49     // returns > 0 for a bounded session
50   virtual void getAbsoluteTimeRange(char*& absStartTime, char*& absEndTime) const;               //  返回该子会话的时间范围
51     // Subclasses can reimplement this iff they support seeking by 'absolute' time.
52 
53   // The following may be called by (e.g.) SIP servers, for which the
54   // address and port number fields in SDP descriptions need to be non-zero:
55   void setServerAddressAndPortForSDP(netAddressBits addressBits,
56                      portNumBits portBits);
57 
58 protected: // we're a virtual base class
59   ServerMediaSubsession(UsageEnvironment& env);
60   virtual ~ServerMediaSubsession();
61 
62   char const* rangeSDPLine() const;                                  // 产生rangeLine信息用于填充SDP信息中的rangeLine字段
63       // returns a string to be delete[]
64 
65   ServerMediaSession* fParentSession;                                // 父会话
66   netAddressBits fServerAddressForSDP;
67   portNumBits fPortNumForSDP;
68 
69 private:
70   friend class ServerMediaSession;
71   friend class ServerMediaSubsessionIterator;
72   ServerMediaSubsession* fNext;
73 
74   unsigned fTrackNumber; // within an enclosing ServerMediaSession
75   char const* fTrackId;
76 };
 　　此处我们的媒体文件是.264文件，创建的ServerMediaSubsession对象是H264VideoFileServerMediaSubsession类的实例，该类是FileServerMediaSubsession的子类，FileServerMediaSubsession表示从媒体文件中获取数据的子会话，FileServerMediaSubsession又是OnDemandServerMediaSubsession的子类。
　　H264VideoFileServerMediaSubsession的构造函数：
```
1 H264VideoFileServerMediaSubsession::H264VideoFileServerMediaSubsession(UsageEnvironment& env,
2                                        char const* fileName, Boolean reuseFirstSource)
3   : FileServerMediaSubsession(env, fileName, reuseFirstSource),
4     fAuxSDPLine(NULL), fDoneFlag(0), fDummyRTPSink(NULL) {
5 }
```
　　FileServerMediaSubsession的构造函数：
```
1 FileServerMediaSubsession::FileServerMediaSubsession(UsageEnvironment& env, char const* fileName,
2                 Boolean reuseFirstSource)
3   : OnDemandServerMediaSubsession(env, reuseFirstSource),
4     fFileSize(0) {
5   fFileName = strDup(fileName);
6 }
```
　　OnDemandServerMediaSubsession的构造函数：
```
1 OnDemandServerMediaSubsession
 2 ::OnDemandServerMediaSubsession(UsageEnvironment& env,
 3                 Boolean reuseFirstSource,
 4                 portNumBits initialPortNum,
 5                 Boolean multiplexRTCPWithRTP)
 6   : ServerMediaSubsession(env),
 7     fSDPLines(NULL), fReuseFirstSource(reuseFirstSource),
 8     fMultiplexRTCPWithRTP(multiplexRTCPWithRTP), fLastStreamToken(NULL) {
 9   fDestinationsHashTable = HashTable::create(ONE_WORD_HASH_KEYS);
10   if (fMultiplexRTCPWithRTP) {
11     fInitialPortNum = initialPortNum;
12   } else {
13     // Make sure RTP ports are even-numbered:
14     fInitialPortNum = (initialPortNum+1)&~1;
15   }
16   gethostname(fCNAME, sizeof fCNAME);
17   fCNAME[sizeof fCNAME-1] = '\0'; // just in case
18 }
　　关于testOnDemandRTSPServer.cpp就先介绍到这里，后面详细分析RTSP客户端与RTSPServer建立RTSP连接的详细过程。
接下来我们分析一下Live555中建立RTSP连接的详细过程，首先我们需要简单了解一下RTSP协议建立连接的过程：
　　1.（可选） 
       RTSP客户端  —>   RTSP服务器端     OPTIONS命令             询问服务器端有哪些方法可使用
       RTSP服务器端 —>  RTSP客户端       回复OPTIONS命令        回复客户端服务器支持的方法
　　2. （可选）
　　 RTSP客户端  —>  RTSP服务器端      DESCRIBE命令　　　 请求对某个媒体资源（Live555中用ServerMediaSession表示）的描述信息
       RTSP服务器端 —>  RTSP客户端 　　回复DESCRIBE命令　 回复客户端某个媒体资源的描述信息（即SDP）
      3. （必选）
       RTSP客户端  —>   RTSP服务器端　 SETUP命令 　　　　　请求建立对某个媒体资源的连接
　　 RTSP服务器端 —>  RTSP客户端　　 回复SETUP命令　　　 回复建立连接的结果
      4. （必选）
       RTSP客户端  —>   RTSP服务器端　 PLAY命令　　　　　　请求播放媒体资源
　　 RTSP服务器端  —>  RTSP客户端　　回复PLAY命令　　　　回复播放的结果
      --------------------RTSP服务器端发送RTP包（封装了数据）给RTSP客户端-------------------------------
 
　　下面我们从RTSPServer::incomingConnectionHandlerRTSP函数开始，在incomingConnectionHandlerRTSP函数中又调用了RTSPServer::incomingConnectionHandler函数，在这个函数中accept客户端的TCP连接，然后调用RTSPServer::createNewClientConnection函数创建一个RTSPClientConnection实例，该实例表示一个与客户端的RTSP连接。
```
1 RTSPServer::RTSPClientConnection
 2 ::RTSPClientConnection(RTSPServer& ourServer, int clientSocket, struct sockaddr_in clientAddr)
 3   : fOurServer(ourServer), fIsActive(True),
 4     fClientInputSocket(clientSocket), fClientOutputSocket(clientSocket), fClientAddr(clientAddr),
 5     fRecursionCount(0), fOurSessionCookie(NULL) {
 6   // Add ourself to our 'client connections' table:  把这个RTSPClientConnection实例添加到RTSPServer的列表中
 7   fOurServer.fClientConnections->Add((char const*)this, this);
 8   
 9   // Arrange to handle incoming requests:
10   resetRequestBuffer();
11   envir().taskScheduler().setBackgroundHandling(fClientInputSocket, SOCKET_READABLE|SOCKET_EXCEPTION,
12                         (TaskScheduler::BackgroundHandlerProc*)&incomingRequestHandler, this);
13 }
　　RTSPClientConnection的构造函数中，将自己添加到RTSPServer的连接列表中，然后将客户端socket添加到SOCKET SET中，并且设置相应的回调处理函数incomingRequestHandler，然后就开始等待客户端发送命令。服务器端收到客户端的命令即回调RTSPClientConnection::incomingRequestHandler来处理。
　　在RTSPClientConnection::incomingRequestHandler函数中又调用RTSPClientConnection::incomingRequestHandler1函数，在这个函数中，从客户端socket中读取数据，读取的数据存储在RTSPClientConnection::fRequestBuffer这个数组中，然后调RTSPClientConnection::handleRequestBytes函数处理刚才读到的数据。handleRequestBytes函数的内容（比较多）主要是分析读取的数据，提取出命令名等数据，然后根据不同的命令调用不同的函数去处理，将处理后的结果保存在fResponseBuffer这个数组中，然后发送给客户端。在此，我们假设客户端跳过OPTINS命令，直接发送DESCRIBE命令请求建立连接，则在handleRequestBytes函数中会调用RTSPClientConnection::handleCmd_DESCRIBE函数来处理，下面来看一下handleCmd_DESCRIBE函数。先说一下urlPreSuffix和urlSuffix吧，假设客户端请求媒体资源的RTSP地址是rtsp://127.0.0.1:8554/test1/test2/test.264，urlPreSuffix表示的是ip:port之后（不含紧跟的“/”）到最后一个“/”之前的部分，即test1/test2，urlSuffix表示的是最后一个“/”之后（不含紧跟的“/”）的内容，即test.264。
```
1 void RTSPServer::RTSPClientConnection
 2 ::handleCmd_DESCRIBE(char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr) {
 3   char* sdpDescription = NULL;
 4   char* rtspURL = NULL;
 5   do {
 6     char urlTotalSuffix[RTSP_PARAM_STRING_MAX];
 7     if (strlen(urlPreSuffix) + strlen(urlSuffix) + 2 > sizeof urlTotalSuffix) {
 8       handleCmd_bad();
 9       break;
10     }
11     urlTotalSuffix[0] = '\0';                              // 拼接urlPreSuffix和urlSuffix，保存在urlTotalSuffix中
12     if (urlPreSuffix[0] != '\0') {
13       strcat(urlTotalSuffix, urlPreSuffix);
14       strcat(urlTotalSuffix, "/");
15     }
16     strcat(urlTotalSuffix, urlSuffix);
17     
18     if (!authenticationOK("DESCRIBE", urlTotalSuffix, fullRequestStr)) break;
19     
20     // We should really check that the request contains an "Accept:" #####
21     // for "application/sdp", because that's what we're sending back #####
22     
23     // Begin by looking up the "ServerMediaSession" object for the specified "urlTotalSuffix":
24     ServerMediaSession* session = fOurServer.lookupServerMediaSession(urlTotalSuffix);                 // 在RTSPServer中查找对应的ServerMediaSession
25     if (session == NULL) {
26       handleCmd_notFound();
27       break;
28     }
29     
30     // Then, assemble a SDP description for this session:
31     sdpDescription = session->generateSDPDescription();                                                // 产生SDP描述信息字符串
32     if (sdpDescription == NULL) {
33       // This usually means that a file name that was specified for a
34       // "ServerMediaSubsession" does not exist.
35       setRTSPResponse("404 File Not Found, Or In Incorrect Format");
36       break;
37     }
38     unsigned sdpDescriptionSize = strlen(sdpDescription);
39     
40     // Also, generate our RTSP URL, for the "Content-Base:" header
41     // (which is necessary to ensure that the correct URL gets used in subsequent "SETUP" requests).
42     rtspURL = fOurServer.rtspURL(session, fClientInputSocket);
43     
44     snprintf((char*)fResponseBuffer, sizeof fResponseBuffer,                              // 构造回复信息
45          "RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
46          "%s"
47          "Content-Base: %s/\r\n"
48          "Content-Type: application/sdp\r\n"
49          "Content-Length: %d\r\n\r\n"
50          "%s",
51          fCurrentCSeq,
52          dateHeader(),
53          rtspURL,
54          sdpDescriptionSize,
55          sdpDescription);
56   } while (0);
57   
58  59 
60   delete[] sdpDescription;
61   delete[] rtspURL;
62 }
 　　在handleCmd_DESCRIBE函数中，主要调用了ServerMediaSession::generateSDPDescription函数产生SDP信息，ServerMediaSession的SDP信息由每个ServerMediaSubsession的SDP信息构成，然后将产生的SDP回复给客户端。我们就来看一下generateSDPDescription函数。
```
1 char* ServerMediaSession::generateSDPDescription() {
  2   AddressString ipAddressStr(ourIPAddress(envir()));
  3   unsigned ipAddressStrSize = strlen(ipAddressStr.val());
  4 
  5   // For a SSM sessions, we need a "a=source-filter: incl ..." line also:
  6   char* sourceFilterLine;
  7   if (fIsSSM) {
  8     char const* const sourceFilterFmt =
  9       "a=source-filter: incl IN IP4 * %s\r\n"
 10       "a=rtcp-unicast: reflection\r\n";
 11     unsigned const sourceFilterFmtSize = strlen(sourceFilterFmt) + ipAddressStrSize + 1;
 12 
 13     sourceFilterLine = new char[sourceFilterFmtSize];
 14     sprintf(sourceFilterLine, sourceFilterFmt, ipAddressStr.val());
 15   } else {
 16     sourceFilterLine = strDup("");
 17   }
 18 
 19   char* rangeLine = NULL; // for now
 20   char* sdp = NULL; // for now
 21 
 22   do {
 23     // Count the lengths of each subsession's media-level SDP lines.
 24     // (We do this first, because the call to "subsession->sdpLines()"
 25     // causes correct subsession 'duration()'s to be calculated later.)
  
        //首先调用每个ServerMediaSubsession的sdpLines函数，用来计算sdp的长度
 26     unsigned sdpLength = 0;
 27     ServerMediaSubsession* subsession;
 28     for (subsession = fSubsessionsHead; subsession != NULL;
 29      subsession = subsession->fNext) {
 30       char const* sdpLines = subsession->sdpLines();
 31       if (sdpLines == NULL) continue; // the media's not available
 32       sdpLength += strlen(sdpLines);
 33     }
 34     if (sdpLength == 0) break; // the session has no usable subsessions
 35 
 36     // Unless subsessions have differing durations, we also have a "a=range:" line:
        // 计算ServerMediaSession的持续时间，该返回值影响a=range字段，该字段决定了该媒体资源是否可以执行快进、快退、任意进度点播，ServerMediaSession的duration由各个ServerMediaSubsession的duration决定。
        // ServerMediaSubsession的duration默认实现是返回0，Live555只对部分格式的媒体文件实现了duration函数，如MKV文件的MatroskaFileServerMediaSubsession分析了mkv文件的播放时长
 37     float dur = duration();　 38     if (dur == 0.0) {
 39       rangeLine = strDup("a=range:npt=0-\r\n");
 40     } else if (dur > 0.0) {
 41       char buf[100];
 42       sprintf(buf, "a=range:npt=0-%.3f\r\n", dur);
 43       rangeLine = strDup(buf);
 44     } else { // subsessions have differing durations, so "a=range:" lines go there
 45       rangeLine = strDup("");
 46     }
 47 
 48     char const* const sdpPrefixFmt =
 49       "v=0\r\n"
 50       "o=- %ld%06ld %d IN IP4 %s\r\n"
 51       "s=%s\r\n"
 52       "i=%s\r\n"
 53       "t=0 0\r\n"
 54       "a=tool:%s%s\r\n"
 55       "a=type:broadcast\r\n"
 56       "a=control:*\r\n"
 57       "%s"
 58       "%s"
 59       "a=x-qt-text-nam:%s\r\n"
 60       "a=x-qt-text-inf:%s\r\n"
 61       "%s";
 62     sdpLength += strlen(sdpPrefixFmt)
 63       + 20 + 6 + 20 + ipAddressStrSize
 64       + strlen(fDescriptionSDPString)
 65       + strlen(fInfoSDPString)
 66       + strlen(libNameStr) + strlen(libVersionStr)
 67       + strlen(sourceFilterLine)
 68       + strlen(rangeLine)
 69       + strlen(fDescriptionSDPString)
 70       + strlen(fInfoSDPString)
 71       + strlen(fMiscSDPLines);
 72     sdpLength += 1000; // in case the length of the "subsession->sdpLines()" calls below change
 73     sdp = new char[sdpLength];
 74     if (sdp == NULL) break;
 75 
 76     // Generate the SDP prefix (session-level lines):
 77     snprintf(sdp, sdpLength, sdpPrefixFmt,
 78          fCreationTime.tv_sec, fCreationTime.tv_usec, // o= <session id>
 79          1, // o= <version> // (needs to change if params are modified)
 80          ipAddressStr.val(), // o= <address>
 81          fDescriptionSDPString, // s= <description>
 82          fInfoSDPString, // i= <info>
 83          libNameStr, libVersionStr, // a=tool:
 84          sourceFilterLine, // a=source-filter: incl (if a SSM session)
 85          rangeLine, // a=range: line
 86          fDescriptionSDPString, // a=x-qt-text-nam: line
 87          fInfoSDPString, // a=x-qt-text-inf: line
 88          fMiscSDPLines); // miscellaneous session SDP lines (if any)
 89 
 90     // Then, add the (media-level) lines for each subsession:  
        // 再次调用每个ServerMediaSubsession的sdpLines函数，这次真正将每个ServerMediaSubsession的sdp信息添加到ServerMediaSession的SDP信息中
 91     char* mediaSDP = sdp;
 92     for (subsession = fSubsessionsHead; subsession != NULL;
 93      subsession = subsession->fNext) {
 94       unsigned mediaSDPLength = strlen(mediaSDP);
 95       mediaSDP += mediaSDPLength;　　　　　　　　　　　　　　　　　　　　　　// 指针后移
 96       sdpLength -= mediaSDPLength;
 97       if (sdpLength <= 1) break; // the SDP has somehow become too long
 98 
 99       char const* sdpLines = subsession->sdpLines();
100       if (sdpLines != NULL) snprintf(mediaSDP, sdpLength, "%s", sdpLines);
101     }
102   } while (0);
103 
104   delete[] rangeLine; delete[] sourceFilterLine;
105   return sdp;
106 }
　　到此，服务器端将客户端请求的SDP信息发送给客户端，然后等着客户端发送下一个命令（SETUP命令），在分析服务器端如何处理SETUP命令之前，我们继续深入看一下服务器端是如何获得SDP信息的。从generateSDPDescription函数中可以看到，主要是调用了每个ServerMediaSubsession的sdpLines函数，默认实现在OnDemandServerMediaSubsession这个类中，下面我们就来看看OnDemandServerMediaSubsession::sdpLines函数。
```
1 char const* OnDemandServerMediaSubsession::sdpLines() {
 2   if (fSDPLines == NULL) {
 3     // We need to construct a set of SDP lines that describe this
 4     // subsession (as a unicast stream).  To do so, we first create
 5     // dummy (unused) source and "RTPSink" objects,
 6     // whose parameters we use for the SDP lines:
       // 这几句话的意思是说，为了获得这个ServerMediaSubsession的sdp信息，我们先创建“虚设的”FramedSource和RTPSink来分析出sdp信息，并非正式的开始播放
 7     unsigned estBitrate;
       // 创建FramedSource对象，用来获取数据
       //（这里实际调用的是子类H264VideoFileServerMediaSubsession的createNewStreamSource函数，创建的是ByteStreamFileSource，ByteStreamFileSource是FramedSource的子类）
 8     FramedSource* inputSource = createNewStreamSource(0, estBitrate);              // 创建FramedSource对象，获取视频帧数据
 9     if (inputSource == NULL) return NULL; // file not found
10 
11     struct in_addr dummyAddr;
12     dummyAddr.s_addr = 0;
13     Groupsock dummyGroupsock(envir(), dummyAddr, 0, 0);
14     unsigned char rtpPayloadType = 96 + trackNumber()-1; // if dynamic
       // 创建RTPSink对象，用来保存RTP数据包（这里实际调用的是子类H264VideoFileServerMediaSubsession的createNewRTPSink函数，创建的是H264VideoRTPSink对象，H264VideoRTPSink是RTPSink的子类）
15     RTPSink* dummyRTPSink
16       = createNewRTPSink(&dummyGroupsock, rtpPayloadType, inputSource);   
17     if (dummyRTPSink != NULL && dummyRTPSink->estimatedBitrate() > 0) estBitrate = dummyRTPSink->estimatedBitrate();
18 
19     setSDPLinesFromRTPSink(dummyRTPSink, inputSource, estBitrate);           // 通过RTPSink对象获得ServerMediaSubsession的sdp信息
20     Medium::close(dummyRTPSink);
21     closeStreamSource(inputSource);
22   }
23 
24   return fSDPLines;
25 }
 　　我们再转到OnDemandServerMediaSubsession::setSDPLinesFromRTPSink函数，在这个函数中，我们通过创建的FramedSource对象和RTPSink对象将文件播放一段以便产生出sdp信息。在此，我要插一下Live555  RTSPServer播放媒体资源的一个大体流程：RTSPServer使用RTPSink获得和保存RTP包，RTPSink不断地向FramedSource请求帧数据，FramedSource取得帧数据后就调用回调函数把数据给RTPSink处理，RTPSink在回调函数中将数据发送给客户端（也可以保存在本地存成文件，即录像的功能）
```
1 void OnDemandServerMediaSubsession
 2 ::setSDPLinesFromRTPSink(RTPSink* rtpSink, FramedSource* inputSource, unsigned estBitrate) {
 3   if (rtpSink == NULL) return;
 4 
     //通过RTPSink获取各种关于该ServerMediaSubsession的信息，最主要的是获取auxSDPLine
 5   char const* mediaType = rtpSink->sdpMediaType();
 6   unsigned char rtpPayloadType = rtpSink->rtpPayloadType();
 7   AddressString ipAddressStr(fServerAddressForSDP);
 8   char* rtpmapLine = rtpSink->rtpmapLine();
 9   char const* rtcpmuxLine = fMultiplexRTCPWithRTP ? "a=rtcp-mux\r\n" : "";
10   char const* rangeLine = rangeSDPLine();
11   char const* auxSDPLine = getAuxSDPLine(rtpSink, inputSource);
12   if (auxSDPLine == NULL) auxSDPLine = "";
13 
14   char const* const sdpFmt =
15     "m=%s %u RTP/AVP %d\r\n"
16     "c=IN IP4 %s\r\n"
17     "b=AS:%u\r\n"
18     "%s"
19     "%s"
20     "%s"
21     "%s"
22     "a=control:%s\r\n";
23   unsigned sdpFmtSize = strlen(sdpFmt)
24     + strlen(mediaType) + 5 /* max short len */ + 3 /* max char len */
25     + strlen(ipAddressStr.val())
26     + 20 /* max int len */
27     + strlen(rtpmapLine)
28     + strlen(rtcpmuxLine)
29     + strlen(rangeLine)
30     + strlen(auxSDPLine)
31     + strlen(trackId());
32   char* sdpLines = new char[sdpFmtSize];
33   sprintf(sdpLines, sdpFmt,
34       mediaType, // m= <media>
35       fPortNumForSDP, // m= <port>
36       rtpPayloadType, // m= <fmt list>
37       ipAddressStr.val(), // c= address
38       estBitrate, // b=AS:<bandwidth>
39       rtpmapLine, // a=rtpmap:... (if present)
40       rtcpmuxLine, // a=rtcp-mux:... (if present)
41       rangeLine, // a=range:... (if present)
42       auxSDPLine, // optional extra SDP line
43       trackId()); // a=control:<track-id>
44   delete[] (char*)rangeLine; delete[] rtpmapLine;
45 
46   fSDPLines = strDup(sdpLines);
47   delete[] sdpLines;
48 }
 　　在setSDPLinesFromRTPSink函数中通过RTPSink对象获得各种信息，最复杂的是获取auxSDPLine的过程，这个函数在H264VideoFileServerMediaSubsession类中被重写了，由于我们现在分析的媒体资源是.264文件，所以我们来看一下H264VideoFileServerMediaSubsession::getAuxSDPLine函数:
```
1 char const* H264VideoFileServerMediaSubsession::getAuxSDPLine(RTPSink* rtpSink, FramedSource* inputSource) {
 2   if (fAuxSDPLine != NULL) return fAuxSDPLine; // it's already been set up (for a previous client)
 3 
 4   if (fDummyRTPSink == NULL) { // we're not already setting it up for another, concurrent stream
 5     // Note: For H264 video files, the 'config' information ("profile-level-id" and "sprop-parameter-sets") isn't known
 6     // until we start reading the file.  This means that "rtpSink"s "auxSDPLine()" will be NULL initially,
 7     // and we need to start reading data from our file until this changes.
 8     fDummyRTPSink = rtpSink;
 9 
10     // Start reading the file:   //调用RTPSink的startPlaying函数来播放，对于文件型的ServerMediaSubsession，Live555的做法是播放一段文件来获取sdp信息
11     fDummyRTPSink->startPlaying(*inputSource, afterPlayingDummy, this);
12 
13     // Check whether the sink's 'auxSDPLine()' is ready:
14     checkForAuxSDPLine(this);
15   }
16 
17   envir().taskScheduler().doEventLoop(&fDoneFlag);  // fDoneFlag初始值为NULL，让程序在此循环等待，直到成功分析出sdp信息
18 
19   return fAuxSDPLine;
20 }
 　　在这个函数中调用RTPSink的startPlaying函数开始读取数据，调用H264VideoFileServerMediaSubsession::checkForAuxSDPLine函数来检查是否已经从读取的数据中分析出sdp信息，看一下checkForAuxSDPLine函数:
```
1 static void checkForAuxSDPLine(void* clientData) {
 2   H264VideoFileServerMediaSubsession* subsess = (H264VideoFileServerMediaSubsession*)clientData;
 3   subsess->checkForAuxSDPLine1();
 4 }
 5 
 6 void H264VideoFileServerMediaSubsession::checkForAuxSDPLine1() {
 7   char const* dasl;
 8 
 9   if (fAuxSDPLine != NULL) {                       //说明已经分析出了sdp信息
10     // Signal the event loop that we're done:
11     setDoneFlag();                                 // 使程序退出循环等待         
12   }  // 还没分析出sdp信息，调用RTPSink的auxSDPLine函数分析sdp信息
     else if (fDummyRTPSink != NULL && (dasl = fDummyRTPSink->auxSDPLine()) != NULL) {
13     fAuxSDPLine = strDup(dasl);
14     fDummyRTPSink = NULL;
15 
16     // Signal the event loop that we're done:
17     setDoneFlag();　　　　　　　　　　　　　　　　　　　　// 分析出了sdp信息，使程序退出循环等待
18   } else if (!fDoneFlag)                       // 仍然没有分析出sdp信息，则稍后一会儿再执行checkForAuxSDPLine函数
19     // try again after a brief delay:
20     int uSecsToDelay = 100000; // 100 ms
21     nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecsToDelay,
22                   (TaskFunc*)checkForAuxSDPLine, this);
23   }
24 }
　　上面检查发现没有分析出sdp信息后，调用H264VideoRTPSink::auxSDPLine函数再次试图分析出sdp信息，看看auxSDPLine函数:
```
1 char const* H264VideoRTPSink::auxSDPLine() {
 2   // Generate a new "a=fmtp:" line each time, using our SPS and PPS (if we have them),
 3   // otherwise parameters from our framer source (in case they've changed since the last time that
 4   // we were called):
 5   H264or5VideoStreamFramer* framerSource = NULL;
 6   u_int8_t* vpsDummy = NULL; unsigned vpsDummySize = 0;
 7   u_int8_t* sps = fSPS; unsigned spsSize = fSPSSize;
 8   u_int8_t* pps = fPPS; unsigned ppsSize = fPPSSize;
 9   if (sps == NULL || pps == NULL) {
10     // We need to get SPS and PPS from our framer source:
       //  fOurFragmenter在调用startPlaying函数后被创建
11     if (fOurFragmenter == NULL) return NULL; // we don't yet have a fragmenter (and therefore not a source)
12     framerSource = (H264or5VideoStreamFramer*)(fOurFragmenter->inputSource());
13     if (framerSource == NULL) return NULL; // we don't yet have a source
14 
15     framerSource->getVPSandSPSandPPS(vpsDummy, vpsDummySize, sps, spsSize, pps, ppsSize);
16     if (sps == NULL || pps == NULL) return NULL; // our source isn't ready
17   }
18 
     // 已经从文件里面成功读出了数据，接下来就分析sdp信息
19   // Set up the "a=fmtp:" SDP line for this stream:
20   u_int8_t* spsWEB = new u_int8_t[spsSize]; // "WEB" means "Without Emulation Bytes"
21   unsigned spsWEBSize = removeH264or5EmulationBytes(spsWEB, spsSize, sps, spsSize);
22   if (spsWEBSize < 4) { // Bad SPS size => assume our source isn't ready
23     delete[] spsWEB;
24     return NULL;
25   }
26   u_int32_t profileLevelId = (spsWEB[1]<<16) | (spsWEB[2]<<8) | spsWEB[3];
27   delete[] spsWEB;
28 
29   char* sps_base64 = base64Encode((char*)sps, spsSize);
30   char* pps_base64 = base64Encode((char*)pps, ppsSize);
31 
32   char const* fmtpFmt =
33     "a=fmtp:%d packetization-mode=1"
34     ";profile-level-id=%06X"
35     ";sprop-parameter-sets=%s,%s\r\n";
36   unsigned fmtpFmtSize = strlen(fmtpFmt)
37     + 3 /* max char len */
38     + 6 /* 3 bytes in hex */
39     + strlen(sps_base64) + strlen(pps_base64);
40   char* fmtp = new char[fmtpFmtSize];
41   sprintf(fmtp, fmtpFmt,
42           rtpPayloadType(),
43       profileLevelId,
44           sps_base64, pps_base64);
45 
46   delete[] sps_base64;
47   delete[] pps_base64;
48 
49   delete[] fFmtpSDPLine; fFmtpSDPLine = fmtp;
50   return fFmtpSDPLine;
51 }
 　　至此，RTSPServer成功地处理了客户端发送来的DESCRIBE命令，将SDP信息回复客户端。然后，客户端对应每个ServerMediaSubsession发送一个SETUP命令请求建立与该ServerMediaSubsession的连接，服务器端收到后会调用RTSPClientSession::handleCmd_SETUP函数来处理SETUP命令。RTSPClientSession类是服务器端用来维护和客户端的一个会话，SETUP命令、PLAY命令、PAUSE命令、TEARDOWN命令等都是在RTSPClientSession中处理的，RTSPClientSession是RTSPClientConnection的内部类，来看一下这个类：
```
1  // The state of an individual client session (using one or more sequential TCP connections) handled by a RTSP server:
  2    class RTSPClientSession {
  3    protected:
  4      RTSPClientSession(RTSPServer& ourServer, u_int32_t sessionId);
  5      virtual ~RTSPClientSession();
  6  
  7       friend class RTSPServer;
  8       friend class RTSPClientConnection;
  9       // Make the handler functions for each command virtual, to allow subclasses to redefine them:
 10      virtual void handleCmd_SETUP(RTSPClientConnection* ourClientConnection,　　　　　　　　　　　　　　　　　　// 处理SETUP命令
 11                   char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr);
 12      virtual void handleCmd_withinSession(RTSPClientConnection* ourClientConnection,
 13                       char const* cmdName,
 14                       char const* urlPreSuffix, char const* urlSuffix,
 15                       char const* fullRequestStr);
 16      virtual void handleCmd_TEARDOWN(RTSPClientConnection* ourClientConnection,　　　　　　　　　　　　　　　　// 处理TEARDOWN命令（结束会话）
 17                      ServerMediaSubsession* subsession);
 18      virtual void handleCmd_PLAY(RTSPClientConnection* ourClientConnection,　　　　　　　　　　　　　　　　　　 // 处理PLAY命令
 19                  ServerMediaSubsession* subsession, char const* fullRequestStr);
 20      virtual void handleCmd_PAUSE(RTSPClientConnection* ourClientConnection,　　　　　　　　　　　　　　　　　　// 处理PAUSE命令
 21                   ServerMediaSubsession* subsession);
 22      virtual void handleCmd_GET_PARAMETER(RTSPClientConnection* ourClientConnection,
 23                       ServerMediaSubsession* subsession, char const* fullRequestStr);
 24      virtual void handleCmd_SET_PARAMETER(RTSPClientConnection* ourClientConnection,
 25                       ServerMediaSubsession* subsession, char const* fullRequestStr);
 26    protected:
 27      UsageEnvironment& envir() { return fOurServer.envir(); }
 28      void reclaimStreamStates();
 29      Boolean isMulticast() const { return fIsMulticast; }
 30      void noteLiveness();
 31      static void noteClientLiveness(RTSPClientSession* clientSession); 　　　　　　　　　　// 与客户端的心跳
 32      static void livenessTimeoutTask(RTSPClientSession* clientSession);
 33  
 34      // Shortcuts for setting up a RTSP response (prior to sending it):
 35      void setRTSPResponse(RTSPClientConnection* ourClientConnection, char const* responseStr) { ourClientConnection->setRTSPResponse(responseStr); }
 36      void setRTSPResponse(RTSPClientConnection* ourClientConnection, char const* responseStr, u_int32_t sessionId) { ourClientConnection->setRTSPResponse(responseStr, sessionId); }
 37      void setRTSPResponse(RTSPClientConnection* ourClientConnection, char const* responseStr, char const* contentStr) { ourClientConnection->setRTSPResponse(responseStr, contentStr); }
 38      void setRTSPResponse(RTSPClientConnection* ourClientConnection, char const* responseStr, u_int32_t sessionId, char const* contentStr) { ourClientConnection->setRTSPResponse(responseStr, sessionId, contentStr); }
 39  
 40    protected:
 41      RTSPServer& fOurServer;
 42      u_int32_t fOurSessionId;
 43      ServerMediaSession* fOurServerMediaSession;
 44      Boolean fIsMulticast, fStreamAfterSETUP;
 45      unsigned char fTCPStreamIdCount; // used for (optional) RTP/TCP
 46      Boolean usesTCPTransport() const { return fTCPStreamIdCount > 0; }
 47      TaskToken fLivenessCheckTask;
 48      unsigned fNumStreamStates;　　　　　　　　// streamState对象的数目
 49      struct streamState {　　　　　　　　　　　　　　　　　　　　　　　　// streamState结构体，保存一个请求的ServerMediaSubsession以及对应的StreamState对象
 50        ServerMediaSubsession* subsession;
 51        void* streamToken;　　　　　　　　　　　// streamToken指向一个StreamState对象
 52      } * fStreamStates;                     // fStreamStates是streamState数组
 53    };
 54  
      /* StreamState类从名字上就可以看出是服务器端用来保存对某个ServerMediaSubsession的流化的状态（包括serverRTPPort、serverRTCPPort、rtpSink、mediaSource等）
         当某个ServerMediaSubsession被客户端请求SETUP时，服务器端会创建一个StreamState对象，并创建相关的服务器端socket、RTPSink、FramedSource为后面的播放做好准备，
　　　　　在创建一个ServerMediaSubsession对象时（详情见testOnDemandRTSPServer.cpp的main函数），会传入reuseFirstSource这个参数。如果reuseFirstSource为true，
　　　　　则表示对于请求该ServerMediaSubsession的所有客户端都使用同一个StreamState对象，即服务器端使用同一个RTP端口、RTCP端口、RTPSink、FramedSource来为请求该
　　　　　ServerMediaSubsession的多个客户端服务（一对多，节省服务器端资源）；而如果reuseFirstSource为false，则服务器端为每个对ServerMediaSubsession的请求创建一个StreamState对象（多对多，需要占用服务器端较多资源） */
 55 class StreamState {                                        // StreamState类，表示服务器端对一个ServerMediaSubsession的一次流化，并保存相关状态
 56 public:
 57   StreamState(OnDemandServerMediaSubsession& master,
 58               Port const& serverRTPPort, Port const& serverRTCPPort,
 59           RTPSink* rtpSink, BasicUDPSink* udpSink,
 60           unsigned totalBW, FramedSource* mediaSource,
 61           Groupsock* rtpGS, Groupsock* rtcpGS);
 62   virtual ~StreamState();
 63 
 64   void startPlaying(Destinations* destinations,　　　　　　　　　　　// 开始播放，服务器端在收到PLAY命令后，就是调用各个StreamState的startPlaying函数来开始播放一个ServerMediaSubsession
 65             TaskFunc* rtcpRRHandler, void* rtcpRRHandlerClientData,
 66             ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
 67                     void* serverRequestAlternativeByteHandlerClientData);
 68   void pause();
 69   void endPlaying(Destinations* destinations);　　　　　　　　　　　// 结束播放
 70   void reclaim();
 71 
 72   unsigned& referenceCount() { return fReferenceCount; }         // 引用该路流的客户端数目
 73 
 74   Port const& serverRTPPort() const { return fServerRTPPort; }
 75   Port const& serverRTCPPort() const { return fServerRTCPPort; }
 76 
 77   RTPSink* rtpSink() const { return fRTPSink; }
 78 
 79   float streamDuration() const { return fStreamDuration; }
 80 
 81   FramedSource* mediaSource() const { return fMediaSource; }
 82   float& startNPT() { return fStartNPT; }
 83 
 84 private:
 85   OnDemandServerMediaSubsession& fMaster;
 86   Boolean fAreCurrentlyPlaying;
 87   unsigned fReferenceCount;
 88 
 89   Port fServerRTPPort, fServerRTCPPort;
 90 
 91   RTPSink* fRTPSink;
 92   BasicUDPSink* fUDPSink;
 93 
 94   float fStreamDuration;
 95   unsigned fTotalBW;
 96   RTCPInstance* fRTCPInstance;
 97 
 98   FramedSource* fMediaSource;
 99   float fStartNPT; // initial 'normal play time'; reset after each seek
100 
101   Groupsock* fRTPgs;
102   Groupsock* fRTCPgs;
103 };
　　接下来，在handleCmd_SETUP函数中，服务器首先找到客户端请求的ServerMediaSession，再找到客户端请求的ServerMediaSubsession，然后从客户端的请求中获取一些客户端参数（如：客户端的RTP端口、RTCP端口），最后调用OnDemandServerMediaSubsession::getStreamParameters函数创建RTP连接和RTCP连接。看一下getStreamParameters函数：
```
1 void OnDemandServerMediaSubsession
  2 ::getStreamParameters(unsigned clientSessionId,netAddressBits clientAddress,Port const& clientRTPPort,
  3               Port const& clientRTCPPort,int tcpSocketNum,unsigned char rtpChannelId,
  4               unsigned char rtcpChannelId,netAddressBits& destinationAddress,u_int8_t& /*destinationTTL*/,
  5               Boolean& isMulticast,Port& serverRTPPort,Port& serverRTCPPort,void*& streamToken) {
  6   if (destinationAddress == 0) destinationAddress = clientAddress;
  7   struct in_addr destinationAddr; destinationAddr.s_addr = destinationAddress;
  8   isMulticast = False;
  9 
 10   if (fLastStreamToken != NULL && fReuseFirstSource) {                       // 如果fReuseFirstSource为true，则使用之前已经创建的StreamState对象
 11     // Special case: Rather than creating a new 'StreamState',
 12     // we reuse the one that we've already created:
 13     serverRTPPort = ((StreamState*)fLastStreamToken)->serverRTPPort();
 14     serverRTCPPort = ((StreamState*)fLastStreamToken)->serverRTCPPort();
 15     ++((StreamState*)fLastStreamToken)->referenceCount();
 16     streamToken = fLastStreamToken;
 17   } else {　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　// 对于该ServerMediaSubsession尚未创建StreamState对象，或者fReuseFirstSource为false
 18     // Normal case: Create a new media source:
 19     unsigned streamBitrate;
 20     FramedSource* mediaSource　　　　　　// 创建FramedSource对象，实际调用的是子类的createNewStreamSource函数，对应H264VideoFileServerMediaSubsession创建的是ByteStreamFileSource
 21       = createNewStreamSource(clientSessionId, streamBitrate);
 22 
 23     // Create 'groupsock' and 'sink' objects for the destination,
 24     // using previously unused server port numbers:
 25     RTPSink* rtpSink = NULL;
 26     BasicUDPSink* udpSink = NULL;
 27     Groupsock* rtpGroupsock = NULL;
 28     Groupsock* rtcpGroupsock = NULL;
 29 
 30     if (clientRTPPort.num() != 0 || tcpSocketNum >= 0) { // Normal case: Create destinations
 31       portNumBits serverPortNum;
 32       if (clientRTCPPort.num() == 0) {
 33     // We're streaming raw UDP (not RTP). Create a single groupsock:
 34     NoReuse dummy(envir()); // ensures that we skip over ports that are already in use
 35     for (serverPortNum = fInitialPortNum; ; ++serverPortNum) {
 36       struct in_addr dummyAddr; dummyAddr.s_addr = 0;
 37       
 38       serverRTPPort = serverPortNum;
 39       rtpGroupsock = new Groupsock(envir(), dummyAddr, serverRTPPort, 255);
 40       if (rtpGroupsock->socketNum() >= 0) break; // success
 41     }
 42 
 43     udpSink = BasicUDPSink::createNew(envir(), rtpGroupsock);
 44       } else {　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　// 创建两个服务器端socket用来传输RTP包和RTCP包，对应rtpGroupsock和rtcpGroupsock
 45     // Normal case: We're streaming RTP (over UDP or TCP).  Create a pair of
 46     // groupsocks (RTP and RTCP), with adjacent port numbers (RTP port number even).
 47     // (If we're multiplexing RTCP and RTP over the same port number, it can be odd or even.)
 48     NoReuse dummy(envir()); // ensures that we skip over ports that are already in use
 49     for (portNumBits serverPortNum = fInitialPortNum; ; ++serverPortNum) {
 50       struct in_addr dummyAddr; dummyAddr.s_addr = 0;
 51 
 52       serverRTPPort = serverPortNum;
 53       rtpGroupsock = new Groupsock(envir(), dummyAddr, serverRTPPort, 255);
 54       if (rtpGroupsock->socketNum() < 0) {
 55         delete rtpGroupsock;
 56         continue; // try again
 57       }
 58 
 59       if (fMultiplexRTCPWithRTP) {
 60         // Use the RTP 'groupsock' object for RTCP as well:
 61         serverRTCPPort = serverRTPPort;
 62         rtcpGroupsock = rtpGroupsock;
 63       } else {
 64         // Create a separate 'groupsock' object (with the next (odd) port number) for RTCP:
 65         serverRTCPPort = ++serverPortNum;
 66         rtcpGroupsock = new Groupsock(envir(), dummyAddr, serverRTCPPort, 255);
 67         if (rtcpGroupsock->socketNum() < 0) {
 68           delete rtpGroupsock;
 69           delete rtcpGroupsock;
 70           continue; // try again
 71         }
 72       }
 73 
 74       break; // success
 75     }
 76 
 77     unsigned char rtpPayloadType = 96 + trackNumber()-1; // if dynamic
         //创建RTPSink，实际调用的是子类的createNewRTPSink函数，对应H264VideoFileServerMediaSubsession创建的是H264VideoRTPSink
 78     rtpSink = createNewRTPSink(rtpGroupsock, rtpPayloadType, mediaSource);
 79     if (rtpSink != NULL && rtpSink->estimatedBitrate() > 0) streamBitrate = rtpSink->estimatedBitrate();    80       }
 81 
 82       // Turn off the destinations for each groupsock.  They'll get set later
 83       // (unless TCP is used instead):
 84 
 85       if (rtpGroupsock != NULL) rtpGroupsock->removeAllDestinations();
 86       if (rtcpGroupsock != NULL) rtcpGroupsock->removeAllDestinations();
 87 
 88       if (rtpGroupsock != NULL) {
 89     // Try to use a big send buffer for RTP -  at least 0.1 second of
 90     // specified bandwidth and at least 50 KB
 91     
 92     unsigned rtpBufSize = streamBitrate * 25 / 2; // 1 kbps * 0.1 s = 12.5 bytes
 93     if (rtpBufSize < 50 * 1024) rtpBufSize = 50 * 1024;
 94     increaseSendBufferTo(envir(), rtpGroupsock->socketNum(), rtpBufSize);
 95       }
 96     }
 97 
 98     // Set up the state of the stream.  The stream will get started later:
 99     streamToken = fLastStreamToken
100       = new StreamState(*this, serverRTPPort, serverRTCPPort, rtpSink, udpSink,
101             streamBitrate, mediaSource,
102             rtpGroupsock, rtcpGroupsock);
103   }
104 
105   // Record these destinations as being for this client session id:
106   Destinations* destinations;
107   if (tcpSocketNum < 0) { // UDP
108     destinations = new Destinations(destinationAddr, clientRTPPort, clientRTCPPort);
109   } else { // TCP
110     destinations = new Destinations(tcpSocketNum, rtpChannelId, rtcpChannelId);
111   }
112   fDestinationsHashTable->Add((char const*)clientSessionId, destinations);
113 }
　　经过上面的步骤后，服务器端就已经准备好向客户端传送RTP包以及RTCP包了，等待客户端发送PLAY命令后开始传输。服务器端收到PLAY命令后，调用RTSPClientSession::handleCmd_PLAY函数处理。在handleCmd_PLAY函数中，首先提取Scale，表示客户端期望的播放速度（正常、快进、快退），然后提取Range，表示客户端期望的播放起止范围，根据这两个参数分别调用ServerMediaSubsession::setStreamScale函数和ServerMediaSubsession::seekStream函数，最后调用ServerMediaSubsession::startStream函数开始传输数据。实际调用的是OnDemandServerMediaSubsession::startStream函数，看一下这个函数的内容：
```
1 void OnDemandServerMediaSubsession::startStream(unsigned clientSessionId,void* streamToken,
 2                         TaskFunc* rtcpRRHandler,void* rtcpRRHandlerClientData,
 3                         unsigned short& rtpSeqNum,unsigned& rtpTimestamp,
 4                         ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
 5                         void* serverRequestAlternativeByteHandlerClientData) {
 6   StreamState* streamState = (StreamState*)streamToken;
 7   Destinations* destinations
 8     = (Destinations*)(fDestinationsHashTable->Lookup((char const*)clientSessionId));　　　　　　// 查找目的客户端的地址
 9   if (streamState != NULL) {
10     streamState->startPlaying(destinations,
11                   rtcpRRHandler, rtcpRRHandlerClientData,
12                   serverRequestAlternativeByteHandler, serverRequestAlternativeByteHandlerClientData);　　　//调用StreamState::startPlaying函数开始播放
13     
14     RTPSink* rtpSink = streamState->rtpSink(); // alias
15     if (rtpSink != NULL) {
16       rtpSeqNum = rtpSink->currentSeqNo();
17       rtpTimestamp = rtpSink->presetNextTimestamp();
18     }
19   }
20 }
　　在OnDemandServerMediaSubsessionstartStream函数中，主要是调用了StreamState::startPlaying函数，来看一下这个函数：
```
1 void StreamState
 2 ::startPlaying(Destinations* dests,
 3            TaskFunc* rtcpRRHandler, void* rtcpRRHandlerClientData,
 4            ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
 5            void* serverRequestAlternativeByteHandlerClientData) {
 6   if (dests == NULL) return;
 7 
 8   if (fRTCPInstance == NULL && fRTPSink != NULL) {
 9     // Create (and start) a 'RTCP instance' for this RTP sink:
10     fRTCPInstance
11       = RTCPInstance::createNew(fRTPSink->envir(), fRTCPgs,
12                 fTotalBW, (unsigned char*)fMaster.fCNAME,
13                 fRTPSink, NULL /* we're a server */);
14         // Note: This starts RTCP running automatically
15   }
16 
17   if (dests->isTCP) {　　　　　　// 使用TCP传输RTP包和RTCP包
18     19     // Change RTP and RTCP to use the TCP socket instead of UDP:
20     if (fRTPSink != NULL) {
21       fRTPSink->addStreamSocket(dests->tcpSocketNum, dests->rtpChannelId);
22       RTPInterface
23     ::setServerRequestAlternativeByteHandler(fRTPSink->envir(), dests->tcpSocketNum,
24                          serverRequestAlternativeByteHandler, serverRequestAlternativeByteHandlerClientData);
25         // So that we continue to handle RTSP commands from the client
26     }
27     if (fRTCPInstance != NULL) {
28       fRTCPInstance->addStreamSocket(dests->tcpSocketNum, dests->rtcpChannelId);
29       fRTCPInstance->setSpecificRRHandler(dests->tcpSocketNum, dests->rtcpChannelId,
30                       rtcpRRHandler, rtcpRRHandlerClientData);
31     }
32   } else {　　　　　　　　　　　　　　// 使用UDP传输RTP包和RTCP包
33       34     // Tell the RTP and RTCP 'groupsocks' about this destination
35     // (in case they don't already have it):
36     if (fRTPgs != NULL) fRTPgs->addDestination(dests->addr, dests->rtpPort);
37     if (fRTCPgs != NULL) fRTCPgs->addDestination(dests->addr, dests->rtcpPort);
38     if (fRTCPInstance != NULL) {
39       fRTCPInstance->setSpecificRRHandler(dests->addr.s_addr, dests->rtcpPort,
40                       rtcpRRHandler, rtcpRRHandlerClientData);
41     }
42   }
43 
44   if (fRTCPInstance != NULL) {
45     // Hack: Send an initial RTCP "SR" packet, before the initial RTP packet, so that receivers will (likely) be able to
46     // get RTCP-synchronized presentation times immediately:
47     fRTCPInstance->sendReport();
48   }
49 
50   if (!fAreCurrentlyPlaying && fMediaSource != NULL) {　　　　　　　　　　　　　　　　//调用RTPSink::startPlaying函数开始传输数据
51     if (fRTPSink != NULL) {
52       fRTPSink->startPlaying(*fMediaSource, afterPlayingStreamState, this);　　　　　　
53       fAreCurrentlyPlaying = True;
54     } else if (fUDPSink != NULL) {
55       fUDPSink->startPlaying(*fMediaSource, afterPlayingStreamState, this);
56       fAreCurrentlyPlaying = True;
57     }
58   }
59 }
　　在StreamState::startPlaying函数中，将客户端添加到目的客户端列表中去，然后调用RTPSink::startPlaying函数，实际调用的是MediaSink::startPlaying函数:
```
1 Boolean MediaSink::startPlaying(MediaSource& source,
 2                 afterPlayingFunc* afterFunc,
 3                 void* afterClientData) {
 4   // Make sure we're not already being played:
 5   if (fSource != NULL) {
 6     envir().setResultMsg("This sink is already being played");
 7     return False;
 8   }
 9 
10   // Make sure our source is compatible:
11   if (!sourceIsCompatibleWithUs(source)) {
12     envir().setResultMsg("MediaSink::startPlaying(): source is not compatible!");
13     return False;
14   }
15   fSource = (FramedSource*)&source;
16 
17   fAfterFunc = afterFunc;　　　　　　　　　　　　　// 设置好回调函数后，就调用continuePlaying函数开始播放
18   fAfterClientData = afterClientData;
19   return continuePlaying();
20 }
21   /* 这里我们的媒体资源是.264文件，对应的是H264VideoFileServerMediaSubsession，对应H264VideoFileServerMediaSubsession创建的是
　　　H264VideoRTPSink对象，H264VideoRTPSink是H264or5VideoRTPSink的子类，因此上面实际调用的是H264or5VideoRTPSink::continuePlaying函数 */
22 Boolean H264or5VideoRTPSink::continuePlaying() {
23   // First, check whether we have a 'fragmenter' class set up yet.
24   // If not, create it now:　　　　　创建一个H264or5Fragmenter对象，　　
25   if (fOurFragmenter == NULL) {
26     fOurFragmenter = new H264or5Fragmenter(fHNumber, envir(), fSource, OutPacketBuffer::maxSize,
27                        ourMaxPacketSize() - 12/*RTP hdr size*/);
28   } else {
29     fOurFragmenter->reassignInputSource(fSource);
30   }
31   fSource = fOurFragmenter;   // 注意，此处fSource变成了H264or5Fragmenter对象，H264or5Fragmenter是FramedFilter的子类
32   // FramedFilter是FramedSource的子类，从名字可以看出FramedFilter的作用是对FramedSource送来的数据做一些“过滤”，并且FramedFilter的
        结果数据还可以给另外一个FramedFilter做进一步的“过滤”，这里类似于Java中的IO装饰流，使用了装饰模式。
33   // Then call the parent class's implementation:
34   return MultiFramedRTPSink::continuePlaying();　　　　　　//调用了MultiFramedRTPSink的continuePlaying函数
35 }
```
1  Boolean MultiFramedRTPSink::continuePlaying() {
 2    // Send the first packet.
 3    // (This will also schedule any future sends.)
 4    buildAndSendPacket(True);
 5    return True;
 6  }
 7  
 8  void MultiFramedRTPSink::buildAndSendPacket(Boolean isFirstPacket) {
 9    fIsFirstPacket = isFirstPacket;
　　　 // RTP version 2; marker ('M') bit not set (by default; it can be set later)
10    unsigned rtpHdr = 0x80000000;   Set up the RTP header:　　　/设置RTP头部
11    rtpHdr |= (fRTPPayloadType<<16);
12    rtpHdr |= fSeqNo; // sequence number
13    fOutBuf->enqueueWord(rtpHdr);
14    // Note where the RTP timestamp will go.
15    // (We can't fill this in until we start packing payload frames.)
16    fTimestampPosition = fOutBuf->curPacketSize();
17    fOutBuf->skipBytes(4); // leave a hole for the timestamp
18    fOutBuf->enqueueWord(SSRC());
19    // Allow for a special, payload-format-specific header following the
20    // RTP header:
21    fSpecialHeaderPosition = fOutBuf->curPacketSize();
22    fSpecialHeaderSize = specialHeaderSize();
23    fOutBuf->skipBytes(fSpecialHeaderSize);
24  
25    // Begin packing as many (complete) frames into the packet as we can:
26    fTotalFrameSpecificHeaderSizes = 0;
27    fNoFramesLeft = False;
28    fNumFramesUsedSoFar = 0;
29    packFrame();                      // 调用packFrame函数
30  }
31  
32  void MultiFramedRTPSink::packFrame() {
33    // Get the next frame.
34  
35    // First, see if we have an overflow frame that was too big for the last pkt
36    if (fOutBuf->haveOverflowData()) {
37      // Use this frame before reading a new one from the source
38      unsigned frameSize = fOutBuf->overflowDataSize();
39      struct timeval presentationTime = fOutBuf->overflowPresentationTime();
40      unsigned durationInMicroseconds = fOutBuf->overflowDurationInMicroseconds();
41      fOutBuf->useOverflowData();
42  
43      afterGettingFrame1(frameSize, 0, presentationTime, durationInMicroseconds);
44    } else {
45      // Normal case: we need to read a new frame from the source
46      if (fSource == NULL) return;
47  
48      fCurFrameSpecificHeaderPosition = fOutBuf->curPacketSize();
49      fCurFrameSpecificHeaderSize = frameSpecificHeaderSize();
50      fOutBuf->skipBytes(fCurFrameSpecificHeaderSize);
51      fTotalFrameSpecificHeaderSizes += fCurFrameSpecificHeaderSize;
52      fSource->getNextFrame(fOutBuf->curPtr(), fOutBuf->totalBytesAvailable(),  // 调用FramedSource::getNextFrame函数获取帧数据保存到fOutBuf中
53                afterGettingFrame, this, ourHandleClosure, this);       // 获取后回调MultiFramedRTPSink::afterGettingFrame函数
        //对应H264VideoFileServerMediaSubsession，是在H264or5Fragmenter中回调MultiFramedRTPSink::afterGettingFrame函数
54    }
55  }
56 
57 void FramedSource::getNextFrame(unsigned char* to, unsigned maxSize,
58                 afterGettingFunc* afterGettingFunc,void* afterGettingClientData,
59                 onCloseFunc* onCloseFunc,void* onCloseClientData) {
60   // Make sure we're not already being read:
61   if (fIsCurrentlyAwaitingData) {
62     envir() << "FramedSource[" << this << "]::getNextFrame(): attempting to read more than once at the same time!\n";
63     envir().internalError();
64   }
65 
66   fTo = to;
67   fMaxSize = maxSize;
68   fNumTruncatedBytes = 0; // by default; could be changed by doGetNextFrame()
69   fDurationInMicroseconds = 0; // by default; could be changed by doGetNextFrame()
70   fAfterGettingFunc = afterGettingFunc;
71   fAfterGettingClientData = afterGettingClientData;
72   fOnCloseFunc = onCloseFunc;
73   fOnCloseClientData = onCloseClientData;
74   fIsCurrentlyAwaitingData = True;
75 
76   doGetNextFrame();　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　// 调用doGetNextFrame函数
77 }
　　对应H264VideoFileServerMediaSubsession之前创建的FramedSource是ByteStreamFileSource对象，然后又在ByteStreamFileSource外面套了一个H264or5Fragmenter，因此调用的是H264or5Fragmenter::doGetNextFrame函数：
```
1 void H264or5Fragmenter::doGetNextFrame() {
 2   if (fNumValidDataBytes == 1) {
 3     // We have no NAL unit data currently in the buffer.  Read a new one:
        //在这里又调用了ByteStreamFileSource的doGetNextFrame函数，并且设置回调函数为H264or5Fragmenter::afterGettingFrame函数
 4     fInputSource->getNextFrame(&fInputBuffer[1], fInputBufferSize - 1,        
 5                    afterGettingFrame, this,
 6                    FramedSource::handleClosure, this);
 7   } else {
 8     // We have NAL unit data in the buffer.  There are three cases to consider:
 9     // 1. There is a new NAL unit in the buffer, and it's small enough to deliver
10     //    to the RTP sink (as is).
11     // 2. There is a new NAL unit in the buffer, but it's too large to deliver to
12     //    the RTP sink in its entirety.  Deliver the first fragment of this data,
13     //    as a FU packet, with one extra preceding header byte (for the "FU header").
14     // 3. There is a NAL unit in the buffer, and we've already delivered some
15     //    fragment(s) of this.  Deliver the next fragment of this data,
16     //    as a FU packet, with two (H.264) or three (H.265) extra preceding header bytes
17     //    (for the "NAL header" and the "FU header").
18 
19     if (fMaxSize < fMaxOutputPacketSize) { // shouldn't happen
20       envir() << "H264or5Fragmenter::doGetNextFrame(): fMaxSize ("
21           << fMaxSize << ") is smaller than expected\n";
22     } else {
23       fMaxSize = fMaxOutputPacketSize;
24     }
25 
26     fLastFragmentCompletedNALUnit = True; // by default
27     if (fCurDataOffset == 1) { // case 1 or 2
28       if (fNumValidDataBytes - 1 <= fMaxSize) { // case 1
29     memmove(fTo, &fInputBuffer[1], fNumValidDataBytes - 1);
30     fFrameSize = fNumValidDataBytes - 1;
31     fCurDataOffset = fNumValidDataBytes;
32       } else { // case 2
33     // We need to send the NAL unit data as FU packets.  Deliver the first
34     // packet now.  Note that we add "NAL header" and "FU header" bytes to the front
35     // of the packet (overwriting the existing "NAL header").
36     if (fHNumber == 264) {
37       fInputBuffer[0] = (fInputBuffer[1] & 0xE0) | 28; // FU indicator
38       fInputBuffer[1] = 0x80 | (fInputBuffer[1] & 0x1F); // FU header (with S bit)
39     } else { // 265
40       u_int8_t nal_unit_type = (fInputBuffer[1]&0x7E)>>1;
41       fInputBuffer[0] = (fInputBuffer[1] & 0x81) | (49<<1); // Payload header (1st byte)
42       fInputBuffer[1] = fInputBuffer[2]; // Payload header (2nd byte)
43       fInputBuffer[2] = 0x80 | nal_unit_type; // FU header (with S bit)
44     }
45     memmove(fTo, fInputBuffer, fMaxSize);
46     fFrameSize = fMaxSize;
47     fCurDataOffset += fMaxSize - 1;
48     fLastFragmentCompletedNALUnit = False;
49       }
50     } else { // case 3
51       // We are sending this NAL unit data as FU packets.  We've already sent the
52       // first packet (fragment).  Now, send the next fragment.  Note that we add
53       // "NAL header" and "FU header" bytes to the front.  (We reuse these bytes that
54       // we already sent for the first fragment, but clear the S bit, and add the E
55       // bit if this is the last fragment.)
56       unsigned numExtraHeaderBytes;
57       if (fHNumber == 264) {
58     fInputBuffer[fCurDataOffset-2] = fInputBuffer[0]; // FU indicator
59     fInputBuffer[fCurDataOffset-1] = fInputBuffer[1]&~0x80; // FU header (no S bit)
60     numExtraHeaderBytes = 2;
61       } else { // 265
62     fInputBuffer[fCurDataOffset-3] = fInputBuffer[0]; // Payload header (1st byte)
63     fInputBuffer[fCurDataOffset-2] = fInputBuffer[1]; // Payload header (2nd byte)
64     fInputBuffer[fCurDataOffset-1] = fInputBuffer[2]&~0x80; // FU header (no S bit)
65     numExtraHeaderBytes = 3;
66       }
67       unsigned numBytesToSend = numExtraHeaderBytes + (fNumValidDataBytes - fCurDataOffset);
68       if (numBytesToSend > fMaxSize) {
69     // We can't send all of the remaining data this time:
70     numBytesToSend = fMaxSize;
71     fLastFragmentCompletedNALUnit = False;
72       } else {
73     // This is the last fragment:
74     fInputBuffer[fCurDataOffset-1] |= 0x40; // set the E bit in the FU header
75     fNumTruncatedBytes = fSaveNumTruncatedBytes;
76       }
77       memmove(fTo, &fInputBuffer[fCurDataOffset-numExtraHeaderBytes], numBytesToSend);
78       fFrameSize = numBytesToSend;
79       fCurDataOffset += numBytesToSend - numExtraHeaderBytes;
80     }
81 
82     if (fCurDataOffset >= fNumValidDataBytes) {
83       // We're done with this data.  Reset the pointers for receiving new data:
84       fNumValidDataBytes = fCurDataOffset = 1;
85     }
86 
87     // Complete delivery to the client:
88     FramedSource::afterGetting(this);       // 回调MultiFramedRTPSink::afterGettingFrame函数    
89   }
90 }
```
1 void ByteStreamFileSource::doGetNextFrame() {
 2   if (feof(fFid) || ferror(fFid) || (fLimitNumBytesToStream && fNumBytesToStream == 0)) {
 3     handleClosure();
 4     return;
 5   }
 6 
 7 #ifdef READ_FROM_FILES_SYNCHRONOUSLY
 8   doReadFromFile();　　　　　　　　　　　　　　　　　　　　　　　　　　　// 读文件
 9 #else
10   if (!fHaveStartedReading) {
11     // Await readable data from the file:
12     envir().taskScheduler().turnOnBackgroundReadHandling(fileno(fFid),
13            (TaskScheduler::BackgroundHandlerProc*)&fileReadableHandler, this);
14     fHaveStartedReading = True;
15   }
16 #endif
17 }
18 
19 void ByteStreamFileSource::doReadFromFile() {
20   // Try to read as many bytes as will fit in the buffer provided (or "fPreferredFrameSize" if less)
21   if (fLimitNumBytesToStream && fNumBytesToStream < (u_int64_t)fMaxSize) {
22     fMaxSize = (unsigned)fNumBytesToStream;
23   }
24   if (fPreferredFrameSize > 0 && fPreferredFrameSize < fMaxSize) {
25     fMaxSize = fPreferredFrameSize;
26   }
27 #ifdef READ_FROM_FILES_SYNCHRONOUSLY
28   fFrameSize = fread(fTo, 1, fMaxSize, fFid);        //调用fread函数读取数据
29 #else
30   if (fFidIsSeekable) {
31     fFrameSize = fread(fTo, 1, fMaxSize, fFid);
32   } else {
33     // For non-seekable files (e.g., pipes), call "read()" rather than "fread()", to ensure that the read doesn't block:
34     fFrameSize = read(fileno(fFid), fTo, fMaxSize);
35   }
36 #endif
37   if (fFrameSize == 0) {
38     handleClosure();
39     return;
40   }
41   fNumBytesToStream -= fFrameSize;
42 
43   // Set the 'presentation time':
44   if (fPlayTimePerFrame > 0 && fPreferredFrameSize > 0) {
45     if (fPresentationTime.tv_sec == 0 && fPresentationTime.tv_usec == 0) {
46       // This is the first frame, so use the current time:
47       gettimeofday(&fPresentationTime, NULL);
48     } else {
49       // Increment by the play time of the previous data:
50       unsigned uSeconds    = fPresentationTime.tv_usec + fLastPlayTime;
51       fPresentationTime.tv_sec += uSeconds/1000000;
52       fPresentationTime.tv_usec = uSeconds%1000000;
53     }
54 
55     // Remember the play time of this data:
56     fLastPlayTime = (fPlayTimePerFrame*fFrameSize)/fPreferredFrameSize;
57     fDurationInMicroseconds = fLastPlayTime;
58   } else {
59     // We don't know a specific play time duration for this data,
60     // so just record the current time as being the 'presentation time':
61     gettimeofday(&fPresentationTime, NULL);
62   }
63  //读取数据后，调用FramedSource::afterGetting函数
64   // Inform the reader that he has data:
65 #ifdef READ_FROM_FILES_SYNCHRONOUSLY
66   // To avoid possible infinite recursion, we need to return to the event loop to do this:
67   nextTask() = envir().taskScheduler().scheduleDelayedTask(0,
68                 (TaskFunc*)FramedSource::afterGetting, this);
69 #else
70   // Because the file read was done from the event loop, we can call the
71   // 'after getting' function directly, without risk of infinite recursion:
72   FramedSource::afterGetting(this);
73 #endif
74 }
75 
76 void FramedSource::afterGetting(FramedSource* source) {
77   source->fIsCurrentlyAwaitingData = False;
78       // indicates that we can be read again
79       // Note that this needs to be done here, in case the "fAfterFunc"
80       // called below tries to read another frame (which it usually will)
81    
82   if (source->fAfterGettingFunc != NULL) {　　　　　　　　　　　　　　　　　　　　　　　　　
83     (*(source->fAfterGettingFunc))(source->fAfterGettingClientData,
84                    source->fFrameSize, source->fNumTruncatedBytes,
85                    source->fPresentationTime,
86                    source->fDurationInMicroseconds);
87   }
88 }
　　在FramedSource::afterGetting函数中调用fAfterGettingFunc函数，对于ByteStreamFileSource对象，fAfterGettingFunc在之前被设置为H264or5Fragmenter::afterGettingFrame函数：
```
1 void H264or5Fragmenter::afterGettingFrame(void* clientData, unsigned frameSize,
 2                       unsigned numTruncatedBytes,
 3                       struct timeval presentationTime,
 4                       unsigned durationInMicroseconds) {
 5   H264or5Fragmenter* fragmenter = (H264or5Fragmenter*)clientData;
 6   fragmenter->afterGettingFrame1(frameSize, numTruncatedBytes, presentationTime,
 7                  durationInMicroseconds);
 8 }
 9 
10 void H264or5Fragmenter::afterGettingFrame1(unsigned frameSize,
11                        unsigned numTruncatedBytes,
12                        struct timeval presentationTime,
13                        unsigned durationInMicroseconds) {
14   fNumValidDataBytes += frameSize;
15   fSaveNumTruncatedBytes = numTruncatedBytes;
16   fPresentationTime = presentationTime;
17   fDurationInMicroseconds = durationInMicroseconds;
18 
19   // Deliver data to the client:
20   doGetNextFrame();                    
21 }
 　　在H264or5Fragmenter::afterGettingFrame1函数中又调用了H264or5Fragmenter::doGetNextFrame函数，在H264or5Fragmenter::doGetNextFrame函数中，当读取的帧数据满足条件时就又回调MultiFrameRTPSink的afterGettingFrame函数。
```
1 void MultiFramedRTPSink
  2 ::afterGettingFrame(void* clientData, unsigned numBytesRead,
  3             unsigned numTruncatedBytes,
  4             struct timeval presentationTime,
  5             unsigned durationInMicroseconds) {
  6   MultiFramedRTPSink* sink = (MultiFramedRTPSink*)clientData;
  7   sink->afterGettingFrame1(numBytesRead, numTruncatedBytes,
  8                presentationTime, durationInMicroseconds);
  9 }
 10 
 11 void MultiFramedRTPSink
 12 ::afterGettingFrame1(unsigned frameSize, unsigned numTruncatedBytes,
 13              struct timeval presentationTime,
 14              unsigned durationInMicroseconds) {
 15   if (fIsFirstPacket) {
 16     // Record the fact that we're starting to play now:
 17     gettimeofday(&fNextSendTime, NULL);
 18   }
 19 
 20   fMostRecentPresentationTime = presentationTime;
 21   if (fInitialPresentationTime.tv_sec == 0 && fInitialPresentationTime.tv_usec == 0) {
 22     fInitialPresentationTime = presentationTime;
 23   }    
 24 
 25   if (numTruncatedBytes > 0) {　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　//超出缓冲区大小要被舍弃的数据
 26     unsigned const bufferSize = fOutBuf->totalBytesAvailable();
 27     envir() << "MultiFramedRTPSink::afterGettingFrame1(): The input frame data was too large for our buffer size ("
 28         << bufferSize << ").  "
 29         << numTruncatedBytes << " bytes of trailing data was dropped!  Correct this by increasing \"OutPacketBuffer::maxSize\" to at least "
 30         << OutPacketBuffer::maxSize + numTruncatedBytes << ", *before* creating this 'RTPSink'.  (Current value is "
 31         << OutPacketBuffer::maxSize << ".)\n";
 32   }
 33   unsigned curFragmentationOffset = fCurFragmentationOffset;
 34   unsigned numFrameBytesToUse = frameSize;
 35   unsigned overflowBytes = 0;
 36 
 37   // If we have already packed one or more frames into this packet,
 38   // check whether this new frame is eligible to be packed after them.
 39   // (This is independent of whether the packet has enough room for this
 40   // new frame; that check comes later.)
 41   if (fNumFramesUsedSoFar > 0) {                              // 在这个RTP包中已经包含了若干帧数据
 42     if ((fPreviousFrameEndedFragmentation
 43      && !allowOtherFramesAfterLastFragment())
 44     || !frameCanAppearAfterPacketStart(fOutBuf->curPtr(), frameSize)) {      //这个RTP包中不允许再添加多余的帧（比如：前面的帧作了结尾标记）
 45       // Save away this frame for next time:
 46       numFrameBytesToUse = 0;
 47       fOutBuf->setOverflowData(fOutBuf->curPacketSize(), frameSize,
 48                    presentationTime, durationInMicroseconds);
 49     }
 50   }
 51   fPreviousFrameEndedFragmentation = False;
 52 
 53   if (numFrameBytesToUse > 0) {　　　　　　　　　　　　　　　　                  // 允许将这一帧添加到RTP包中，但要检查大小是否超出了RTP包的剩余空间　　　　　　　　　　　　　　　  
 54     // Check whether this frame overflows the packet        
 55     if (fOutBuf->wouldOverflow(frameSize)) {　　　　　　　　　　　　　　　　　　　　　　　　 //这一帧数据超出了RTP包剩余空间
 56       // Don't use this frame now; instead, save it as overflow data, and
 57       // send it in the next packet instead.  However, if the frame is too
 58       // big to fit in a packet by itself, then we need to fragment it (and
 59       // use some of it in this packet, if the payload format permits this.)
 60       if (isTooBigForAPacket(frameSize)
 61           && (fNumFramesUsedSoFar == 0 || allowFragmentationAfterStart())) {      //发送这一帧数据的一部分
 62         // We need to fragment this frame, and use some of it now:
 63         overflowBytes = computeOverflowForNewFrame(frameSize);
 64         numFrameBytesToUse -= overflowBytes;
 65         fCurFragmentationOffset += numFrameBytesToUse;
 66       } else {
 67         // We don't use any of this frame now:                              // 不添加这一帧数据
 68         overflowBytes = frameSize;
 69         numFrameBytesToUse = 0;
 70       }
 71       fOutBuf->setOverflowData(fOutBuf->curPacketSize() + numFrameBytesToUse,         // 标记超出帧的位置和大小，以便后面调整packet
 72                    overflowBytes, presentationTime, durationInMicroseconds);
 73     } else if (fCurFragmentationOffset > 0) {                                  
 74       // This is the last fragment of a frame that was fragmented over
 75       // more than one packet.  Do any special handling for this case:
 76       fCurFragmentationOffset = 0;
 77       fPreviousFrameEndedFragmentation = True;
 78     }
 79   }
 80 
 81   if (numFrameBytesToUse == 0 && frameSize > 0) {　　　　　　// 读取适当的数据后，开始发送RTP包
 82     // Send our packet now, because we have filled it up:
 83     sendPacketIfNecessary();　　　　　　　　　　　　　　　　　　　　　　　　 
 84   } else {
 85     // Use this frame in our outgoing packet:
 86     unsigned char* frameStart = fOutBuf->curPtr();
 87     fOutBuf->increment(numFrameBytesToUse);
 88         // do this now, in case "doSpecialFrameHandling()" calls "setFramePadding()" to append padding bytes
 89 
 90     // Here's where any payload format specific processing gets done:
 91     doSpecialFrameHandling(curFragmentationOffset, frameStart,
 92                numFrameBytesToUse, presentationTime,
 93                overflowBytes);
 94 
 95     ++fNumFramesUsedSoFar;
 96 
 97     // Update the time at which the next packet should be sent, based
 98     // on the duration of the frame that we just packed into it.
 99     // However, if this frame has overflow data remaining, then don't
100     // count its duration yet.
101     if (overflowBytes == 0) {
102       fNextSendTime.tv_usec += durationInMicroseconds;
103       fNextSendTime.tv_sec += fNextSendTime.tv_usec/1000000;
104       fNextSendTime.tv_usec %= 1000000;
105     }
106 
107     // Send our packet now if (i) it's already at our preferred size, or
108     // (ii) (heuristic) another frame of the same size as the one we just
109     //      read would overflow the packet, or
110     // (iii) it contains the last fragment of a fragmented frame, and we
111     //      don't allow anything else to follow this or
112     // (iv) one frame per packet is allowed:
113     if (fOutBuf->isPreferredSize()
114         || fOutBuf->wouldOverflow(numFrameBytesToUse)
115         || (fPreviousFrameEndedFragmentation &&
116             !allowOtherFramesAfterLastFragment())
117         || !frameCanAppearAfterPacketStart(fOutBuf->curPtr() - frameSize,
118                        frameSize) ) {
119       // The packet is ready to be sent now
120       sendPacketIfNecessary();
121     } else {
122       // There's room for more frames; try getting another:
123       packFrame();　　　　　　　　　　　　　　　　　　　　　　　　　　// 调用packFrame函数读取更多的数据
124     }
125   }
126 }
　　在MultiFramedRTPSink中尽量读取多的帧之后，调用sendPacketIfNecessary函数发送给客户端:
```
1 void MultiFramedRTPSink::sendPacketIfNecessary() {
 2   if (fNumFramesUsedSoFar > 0) {
 3     // Send the packet:
 4 #ifdef TEST_LOSS
 5     if ((our_random()%10) != 0) // simulate 10% packet loss #####
 6 #endif
 7       if (!fRTPInterface.sendPacket(fOutBuf->packet(), fOutBuf->curPacketSize())) {      // 通过RTPInterface发送RTP包
 8     // if failure handler has been specified, call it
 9     if (fOnSendErrorFunc != NULL) (*fOnSendErrorFunc)(fOnSendErrorData);
10       }
11     ++fPacketCount;
12     fTotalOctetCount += fOutBuf->curPacketSize();
13     fOctetCount += fOutBuf->curPacketSize()
14       - rtpHeaderSize - fSpecialHeaderSize - fTotalFrameSpecificHeaderSizes;
15 
16     ++fSeqNo; // for next time
17   }
18 
19   if (fOutBuf->haveOverflowData()　　　　　　　　　　　　　　　　　　　　　　　　　　　　//未发送的帧数据，对RTP包作出调整
20       && fOutBuf->totalBytesAvailable() > fOutBuf->totalBufferSize()/2) {
21     // Efficiency hack: Reset the packet start pointer to just in front of
22     // the overflow data (allowing for the RTP header and special headers),
23     // so that we probably don't have to "memmove()" the overflow data
24     // into place when building the next packet:
25     unsigned newPacketStart = fOutBuf->curPacketSize()
26       - (rtpHeaderSize + fSpecialHeaderSize + frameSpecificHeaderSize());
27     fOutBuf->adjustPacketStart(newPacketStart);　　　　　　　　　　　　　　　　　　　　　　　　　　　　
28   } else {
29     // Normal case: Reset the packet start pointer back to the start:
30     fOutBuf->resetPacketStart();
31   }
32   fOutBuf->resetOffset();
33   fNumFramesUsedSoFar = 0;
34 
35   if (fNoFramesLeft) {
36     // We're done:
37     onSourceClosure();
38   } else {
39     // We have more frames left to send.  Figure out when the next frame
40     // is due to start playing, then make sure that we wait this long before
41     // sending the next packet.
42     struct timeval timeNow;
43     gettimeofday(&timeNow, NULL);
44     int secsDiff = fNextSendTime.tv_sec - timeNow.tv_sec;
45     int64_t uSecondsToGo = secsDiff*1000000 + (fNextSendTime.tv_usec - timeNow.tv_usec);
46     if (uSecondsToGo < 0 || secsDiff < 0) { // sanity check: Make sure that the time-to-delay is non-negative:
47       uSecondsToGo = 0;
48     }
49 
50     // Delay this amount of time:        // 准备下一次发送RTP包
51     nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecondsToGo, (TaskFunc*)sendNext, this);
52   }
53 }
54 
55 // The following is called after each delay between packet sends:
56 void MultiFramedRTPSink::sendNext(void* firstArg) {
57   MultiFramedRTPSink* sink = (MultiFramedRTPSink*)firstArg;
58   sink->buildAndSendPacket(False);                                //循环调用buildAndSendPacket
59 }
　　以上的调用函数过程比较乱，特附一张图以更清晰地展示以上的流程
![](http://images.cnitblog.com/blog2015/688670/201504/061257290409525.jpg)
　　
　　服务器端通过RTPSink去读数据，在RTPSink中又通过FramedSource读数据，读完数据后交给RTPSink处理，RTPSink处理完后继续通过FramedSource读取数据，如此在RTPSink和FramedSoruce之间形成一个循环，这是Live555读取发送数据的总体流程。
　　以上便是从建立RTSP连接到发送RTP数据的流程（以H264文件为例），后面的停止发送数据到断开连接不再关注和详述。
　Live555不仅实现了RTSP服务器端，还实现了RTSP客户端，我们通过testRTSPClient.cpp这个程序来看一下，Live555的RTSP客户端与服务器端建立RTSP连接的过程。
　　首先来看一下main函数：
```
1 char eventLoopWatchVariable = 0;
 2 
 3 int main(int argc, char** argv) {
 4   // Begin by setting up our usage environment:
 5   TaskScheduler* scheduler = BasicTaskScheduler::createNew();
 6   UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
 7 
 8   // We need at least one "rtsp://" URL argument:
 9   if (argc < 2) {
10     usage(*env, argv[0]);
11     return 1;
12   }
13 
14   // There are argc-1 URLs: argv[1] through argv[argc-1].  Open and start streaming each one:
15   for (int i = 1; i <= argc-1; ++i) {
16     openURL(*env, argv[0], argv[i]);
17   }
18 
19   // All subsequent activity takes place within the event loop:
20   env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
21     // This function call does not return, unless, at some point in time, "eventLoopWatchVariable" gets set to something non-zero.
22 
23   return 0;
24 
25   // If you choose to continue the application past this point (i.e., if you comment out the "return 0;" statement above),
26   // and if you don't intend to do anything more with the "TaskScheduler" and "UsageEnvironment" objects,
27   // then you can also reclaim the (small) memory used by these objects by uncommenting the following code:
28   /*
29     env->reclaim(); env = NULL;
30     delete scheduler; scheduler = NULL;
31   */
32 }
　　和testOnDeamandRTSPServer.cpp一样，首先也是创建TaskScheduler对象和UsageEnvironment对象，然后调用openURL函数去请求某个媒体资源，参数是该媒体资源的RTSP地址，最后使程序进入主循环。
```
1 void openURL(UsageEnvironment& env, char const* progName, char const* rtspURL) {
 2   // Begin by creating a "RTSPClient" object.  Note that there is a separate "RTSPClient" object for each stream that we wish
 3   // to receive (even if more than stream uses the same "rtsp://" URL).
 4   RTSPClient* rtspClient = ourRTSPClient::createNew(env, rtspURL, RTSP_CLIENT_VERBOSITY_LEVEL, progName);
 5   if (rtspClient == NULL) {
 6     env << "Failed to create a RTSP client for URL \"" << rtspURL << "\": " << env.getResultMsg() << "\n";
 7     return;
 8   }
 9 
10   ++rtspClientCount;
11 
12   // Next, send a RTSP "DESCRIBE" command, to get a SDP description for the stream.
13   // Note that this command - like all RTSP commands - is sent asynchronously; we do not block, waiting for a response.
14   // Instead, the following function call returns immediately, and we handle the RTSP response later, from within the event loop:
15   rtspClient->sendDescribeCommand(continueAfterDESCRIBE); //发送DESCRIBE命令，并传入回调函数
16 }
　　OpenURL函数很简单，创建一个RTSPClient对象，一个RTSPClient对象代表一个RTSP客户端。然后调用sendDescribeCommand函数发送DESCRIBE命令，回调函数是continueAfterDESCRIBE函数，在收到RTSP服务器端对DESCRIBE命令的回复时调用。
```
1 void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString) {
 2   do {
 3     UsageEnvironment& env = rtspClient->envir(); // alias
 4     StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias
 5 
 6     if (resultCode != 0) {  // 返回结果码非0表示出错
 7       env << *rtspClient << "Failed to get a SDP description: " << resultString << "\n";
 8       delete[] resultString;
 9       break;
10     }
11     // resultString即从服务器端返回的SDP信息字符串
12     char* const sdpDescription = resultString;
13     env << *rtspClient << "Got a SDP description:\n" << sdpDescription << "\n";
14 
15     // Create a media session object from this SDP description:
16     scs.session = MediaSession::createNew(env, sdpDescription);   //根据SDP信息创建一个MediaSession对象
17     delete[] sdpDescription; // because we don't need it anymore
18     if (scs.session == NULL) {
19       env << *rtspClient << "Failed to create a MediaSession object from the SDP description: " << env.getResultMsg() << "\n";
20       break;
21     } else if (!scs.session->hasSubsessions()) {
22       env << *rtspClient << "This session has no media subsessions (i.e., no \"m=\" lines)\n";
23       break;
24     }
25 
26     // Then, create and set up our data source objects for the session.  We do this by iterating over the session's 'subsessions',
27     // calling "MediaSubsession::initiate()", and then sending a RTSP "SETUP" command, on each one.
28     // (Each 'subsession' will have its own data source.)
29     scs.iter = new MediaSubsessionIterator(*scs.session);
30     setupNextSubsession(rtspClient);　　　　　　　　//开始对服务器端的每个ServerMediaSubsession发送SETUP命令请求建立连接
31     return;
32   } while (0);
33 
34   // An unrecoverable error occurred with this stream.
35   shutdownStream(rtspClient);
36 }
　　客户端收到服务器端对DESCRIBE命令的回复，取得SDP信息后，客户端创建一个MediaSession对象。MediaSession和ServerMediaSession是相对应的概念，MediaSession表示客户端请求服务器端某个媒体资源的会话，类似地，客户端还存在与ServerMediaSubsession相对应的MediaSubsession，表示MediaSession的子会话，创建MediaSession的同时也创建了包含的MediaSubsession对象。然后客户端对服务器端的每个ServerMediaSubsession发送SETUP命令请求建立连接。
```
1 void setupNextSubsession(RTSPClient* rtspClient) {
 2   UsageEnvironment& env = rtspClient->envir(); // alias
 3   StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias
 4   
 5   scs.subsession = scs.iter->next();
 6   if (scs.subsession != NULL) {
 7     if (!scs.subsession->initiate()) {  // 调用initiate函数初始化MediaSubsession对象
 8       env << *rtspClient << "Failed to initiate the \"" << *scs.subsession << "\" subsession: " << env.getResultMsg() << "\n";
 9       setupNextSubsession(rtspClient); // give up on this subsession; go to the next one
10     } else {
11       env << *rtspClient << "Initiated the \"" << *scs.subsession << "\" subsession (";
12       if (scs.subsession->rtcpIsMuxed()) {
13     env << "client port " << scs.subsession->clientPortNum();
14       } else {
15     env << "client ports " << scs.subsession->clientPortNum() << "-" << scs.subsession->clientPortNum()+1;
16       }
17       env << ")\n";
18       // 发送SETUP命令
19       // Continue setting up this subsession, by sending a RTSP "SETUP" command:
20       rtspClient->sendSetupCommand(*scs.subsession, continueAfterSETUP, False, REQUEST_STREAMING_OVER_TCP);
21     }
22     return;
23   }
24   // 成功与所有的ServerMediaSubsession建立了连接，现在发送PLAY命令
25   // We've finished setting up all of the subsessions.  Now, send a RTSP "PLAY" command to start the streaming:
26   if (scs.session->absStartTime() != NULL) {
27     // Special case: The stream is indexed by 'absolute' time, so send an appropriate "PLAY" command:
28     rtspClient->sendPlayCommand(*scs.session, continueAfterPLAY, scs.session->absStartTime(), scs.session->absEndTime());
29   } else {
30     scs.duration = scs.session->playEndTime() - scs.session->playStartTime();
31     rtspClient->sendPlayCommand(*scs.session, continueAfterPLAY);
32   }
33 }
34 
35 void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString) {
36   do {
37     UsageEnvironment& env = rtspClient->envir(); // alias
38     StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias
39 
40     if (resultCode != 0) {
41       env << *rtspClient << "Failed to set up the \"" << *scs.subsession << "\" subsession: " << resultString << "\n";
42       break;
43     }
44 
45     env << *rtspClient << "Set up the \"" << *scs.subsession << "\" subsession (";
46     if (scs.subsession->rtcpIsMuxed()) {
47       env << "client port " << scs.subsession->clientPortNum();
48     } else {
49       env << "client ports " << scs.subsession->clientPortNum() << "-" << scs.subsession->clientPortNum()+1;
50     }
51     env << ")\n";
52 
53     // Having successfully setup the subsession, create a data sink for it, and call "startPlaying()" on it.
54     // (This will prepare the data sink to receive data; the actual flow of data from the client won't start happening until later,
55     // after we've sent a RTSP "PLAY" command.)
56     //对每个MediaSubsession创建一个MediaSink对象来请求和保存数据
57     scs.subsession->sink = DummySink::createNew(env, *scs.subsession, rtspClient->url());
58       // perhaps use your own custom "MediaSink" subclass instead
59     if (scs.subsession->sink == NULL) {
60       env << *rtspClient << "Failed to create a data sink for the \"" << *scs.subsession
61       << "\" subsession: " << env.getResultMsg() << "\n";
62       break;
63     }
64 
65     env << *rtspClient << "Created a data sink for the \"" << *scs.subsession << "\" subsession\n";
66     scs.subsession->miscPtr = rtspClient; // a hack to let subsession handle functions get the "RTSPClient" from the subsession 
67     scs.subsession->sink->startPlaying(*(scs.subsession->readSource()),
68                        subsessionAfterPlaying, scs.subsession);          // 调用MediaSink的startPlaying函数准备播放
69     // Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
70     if (scs.subsession->rtcpInstance() != NULL) {
71       scs.subsession->rtcpInstance()->setByeHandler(subsessionByeHandler, scs.subsession);
72     }
73   } while (0);
74   delete[] resultString;
75 
76   // Set up the next subsession, if any:  与下一个ServerMediaSubsession建立连接
77   setupNextSubsession(rtspClient);
78 }
　　setupNextSubsession函数中首先调用MediaSubsession的initiate函数初始化MediaSubsession，然后对ServerMediaSubsession发送SETUP命令，收到回复后回调continueAfterSETUP函数。在continueAfterSETUP函数中，为MediaSubsession创建MediaSink对象来请求和保存服务器端发送的数据，然后调用MediaSink::startPlaying函数开始准备播放对应的ServerMediaSubsession，最后调用setupNextSubsession函数与下一个ServerMediaSubsession建立连接，在setupNextSubsession函数中，会检查是否与所有的ServerMediaSubsession都建立了连接，是则发送PLAY命令请求开始传送数据，收到回复则调用continueAfterPLAY函数。
　　在客户端发送PLAY命令之前，我们先看一下MediaSubsession::initiate函数的内容：
```
1 Boolean MediaSubsession::initiate(int useSpecialRTPoffset) {
  2   if (fReadSource != NULL) return True; // has already been initiated
  3 
  4   do {
  5     if (fCodecName == NULL) {
  6       env().setResultMsg("Codec is unspecified");
  7       break;
  8     }
  9     //创建客户端socket，包括RTP socket和RTCP socket，准备从服务器端接收数据
 10     // Create RTP and RTCP 'Groupsocks' on which to receive incoming data.
 11     // (Groupsocks will work even for unicast addresses)
 12     struct in_addr tempAddr;
 13     tempAddr.s_addr = connectionEndpointAddress();
 14         // This could get changed later, as a result of a RTSP "SETUP"
 15     //使用指定的RTP端口和RTCP端口，RTP端口必须是偶数，而RTCP端口必须是（RTP端口+1）
 16     if (fClientPortNum != 0 && (honorSDPPortChoice || IsMulticastAddress(tempAddr.s_addr))) {
 17       // The sockets' port numbers were specified for us.  Use these:
 18       Boolean const protocolIsRTP = strcmp(fProtocolName, "RTP") == 0;
 19       if (protocolIsRTP && !fMultiplexRTCPWithRTP) {
 20     fClientPortNum = fClientPortNum&~1;
 21         // use an even-numbered port for RTP, and the next (odd-numbered) port for RTCP
 22       }
 23       if (isSSM()) {
 24     fRTPSocket = new Groupsock(env(), tempAddr, fSourceFilterAddr, fClientPortNum);
 25       } else {
 26     fRTPSocket = new Groupsock(env(), tempAddr, fClientPortNum, 255);
 27       }
 28       if (fRTPSocket == NULL) {
 29     env().setResultMsg("Failed to create RTP socket");
 30     break;
 31       }
 32       
 33       if (protocolIsRTP) {
 34     if (fMultiplexRTCPWithRTP) {
 35       // Use the RTP 'groupsock' object for RTCP as well:
 36       fRTCPSocket = fRTPSocket;
 37     } else {
 38       // Set our RTCP port to be the RTP port + 1:
 39       portNumBits const rtcpPortNum = fClientPortNum|1;
 40       if (isSSM()) {
 41         fRTCPSocket = new Groupsock(env(), tempAddr, fSourceFilterAddr, rtcpPortNum);
 42       } else {
 43         fRTCPSocket = new Groupsock(env(), tempAddr, rtcpPortNum, 255);
 44       }
 45     }
 46       }
 47     } else {
          // 选取随机的RTP端口和RTCP端口
 48       // Port numbers were not specified in advance, so we use ephemeral port numbers.
 49       // Create sockets until we get a port-number pair (even: RTP; even+1: RTCP).
 50       // (However, if we're multiplexing RTCP with RTP, then we create only one socket,
 51       // and the port number  can be even or odd.)
 52       // We need to make sure that we don't keep trying to use the same bad port numbers over
 53       // and over again, so we store bad sockets in a table, and delete them all when we're done.
 54       HashTable* socketHashTable = HashTable::create(ONE_WORD_HASH_KEYS);
 55       if (socketHashTable == NULL) break;
 56       Boolean success = False;
 57       NoReuse dummy(env());
 58           // ensures that our new ephemeral port number won't be one that's already in use
 59 
 60       while (1) {
 61     // Create a new socket:
 62     if (isSSM()) {
 63       fRTPSocket = new Groupsock(env(), tempAddr, fSourceFilterAddr, 0);
 64     } else {
 65       fRTPSocket = new Groupsock(env(), tempAddr, 0, 255);
 66     }
 67     if (fRTPSocket == NULL) {
 68       env().setResultMsg("MediaSession::initiate(): unable to create RTP and RTCP sockets");
 69       break;
 70     }
 71 
 72     // Get the client port number:
 73     Port clientPort(0);
 74     if (!getSourcePort(env(), fRTPSocket->socketNum(), clientPort)) {
 75       break;
 76     }
 77     fClientPortNum = ntohs(clientPort.num()); 
 78 
 79     if (fMultiplexRTCPWithRTP) {
 80       // Use this RTP 'groupsock' object for RTCP as well:
 81       fRTCPSocket = fRTPSocket;
 82       success = True;
 83       break;
 84     }      
 85 
 86     // To be usable for RTP, the client port number must be even:
 87     if ((fClientPortNum&1) != 0) { // it's odd
 88       // Record this socket in our table, and keep trying:
 89       unsigned key = (unsigned)fClientPortNum;
 90       Groupsock* existing = (Groupsock*)socketHashTable->Add((char const*)key, fRTPSocket);
 91       delete existing; // in case it wasn't NULL
 92       continue;
 93     }
 94 
 95     // Make sure we can use the next (i.e., odd) port number, for RTCP:
 96     portNumBits rtcpPortNum = fClientPortNum|1;
 97     if (isSSM()) {
 98       fRTCPSocket = new Groupsock(env(), tempAddr, fSourceFilterAddr, rtcpPortNum);
 99     } else {
100       fRTCPSocket = new Groupsock(env(), tempAddr, rtcpPortNum, 255);
101     }
102     if (fRTCPSocket != NULL && fRTCPSocket->socketNum() >= 0) {
103       // Success! Use these two sockets.
104       success = True;
105       break;
106     } else {
107       // We couldn't create the RTCP socket (perhaps that port number's already in use elsewhere?).
108       delete fRTCPSocket; fRTCPSocket = NULL;
109 
110       // Record the first socket in our table, and keep trying:
111       unsigned key = (unsigned)fClientPortNum;
112       Groupsock* existing = (Groupsock*)socketHashTable->Add((char const*)key, fRTPSocket);
113       delete existing; // in case it wasn't NULL
114       continue;
115     }
116       }
117 
118       // Clean up the socket hash table (and contents):
119       Groupsock* oldGS;
120       while ((oldGS = (Groupsock*)socketHashTable->RemoveNext()) != NULL) {
121     delete oldGS;
122       }
123       delete socketHashTable;
124 
125       if (!success) break; // a fatal error occurred trying to create the RTP and RTCP sockets; we can't continue
126     }
127 
128     // Try to use a big receive buffer for RTP - at least 0.1 second of
129     // specified bandwidth and at least 50 KB
130     unsigned rtpBufSize = fBandwidth * 25 / 2; // 1 kbps * 0.1 s = 12.5 bytes
131     if (rtpBufSize < 50 * 1024)
132       rtpBufSize = 50 * 1024;
133     increaseReceiveBufferTo(env(), fRTPSocket->socketNum(), rtpBufSize);
134 
135     if (isSSM() && fRTCPSocket != NULL) {
136       // Special case for RTCP SSM: Send RTCP packets back to the source via unicast:
137       fRTCPSocket->changeDestinationParameters(fSourceFilterAddr,0,~0);
138     }
139     //创建FramedSource对象来请求数据
140     // Create "fRTPSource" and "fReadSource":
141     if (!createSourceObjects(useSpecialRTPoffset)) break;
142 
143     if (fReadSource == NULL) {
144       env().setResultMsg("Failed to create read source");
145       break;
146     }
147     // 创建RTCPInstance对象
148     // Finally, create our RTCP instance. (It starts running automatically)
149     if (fRTPSource != NULL && fRTCPSocket != NULL) {
150       // If bandwidth is specified, use it and add 5% for RTCP overhead.
151       // Otherwise make a guess at 500 kbps.
152       unsigned totSessionBandwidth
153     = fBandwidth ? fBandwidth + fBandwidth / 20 : 500;
154       fRTCPInstance = RTCPInstance::createNew(env(), fRTCPSocket,
155                           totSessionBandwidth,
156                           (unsigned char const*)
157                           fParent.CNAME(),
158                           NULL /* we're a client */,
159                           fRTPSource);
160       if (fRTCPInstance == NULL) {
161     env().setResultMsg("Failed to create RTCP instance");
162     break;
163       }
164     }
165 
166     return True;
167   } while (0);
168 
169   deInitiate();
170   fClientPortNum = 0;
171   return False;
172 }
　　在MediaSubsession::initiate函数中，首先创建了两个客户端socket分别用于接收RTP数据和RTCP数据；然后创建FramedSource对象用来从服务器端请求数据，FramedSource对象在createSourceObjects函数中被创建，createSourceObjects根据ServerMediaSubsession资源的不同格式创建不同的FramedSource，我们还是以H264视频为例，则创建的是H264VideoRTPSource对象；最后还创建了RTCPInstance对象。
　　接下来，我们继续看客户端收到PLAY命令回复后调用continueAfterPLAY函数：
```
1 void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString) {
 2   Boolean success = False;
 3 
 4   do {
 5     UsageEnvironment& env = rtspClient->envir(); // alias
 6     StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias
 7 
 8     if (resultCode != 0) {
 9       env << *rtspClient << "Failed to start playing session: " << resultString << "\n";
10       break;
11     }
12 
13     // Set a timer to be handled at the end of the stream's expected duration (if the stream does not already signal its end
14     // using a RTCP "BYE").  This is optional.  If, instead, you want to keep the stream active - e.g., so you can later
15     // 'seek' back within it and do another RTSP "PLAY" - then you can omit this code.
16     // (Alternatively, if you don't want to receive the entire stream, you could set this timer for some shorter value.)
17     if (scs.duration > 0) {
18       unsigned const delaySlop = 2; // number of seconds extra to delay, after the stream's expected duration.  (This is optional.)
19       scs.duration += delaySlop;
20       unsigned uSecsToDelay = (unsigned)(scs.duration*1000000);
21       scs.streamTimerTask = env.taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)streamTimerHandler, rtspClient);
22     }
23 
24     env << *rtspClient << "Started playing session";
25     if (scs.duration > 0) {
26       env << " (for up to " << scs.duration << " seconds)";
27     }
28     env << "...\n";
29 
30     success = True;
31   } while (0);
32   delete[] resultString;
33 
34   if (!success) {
35     // An unrecoverable error occurred with this stream.
36     shutdownStream(rtspClient);
37   }
38 }
　　continueAfterPLAY函数的内容很简单，只是简单地打印出“Started  playing  session”。在服务器端收到PLAY命令后，就开始向客户端发送RTP数据包和RTCP数据包，而客户端在MediaSink::startPlaying函数中就开始等待接收来自服务器端的视频数据。
　　在continueAfterSETUP函数中创建的MediaSink是DummySink对象，DummySink是MediaSink的子类，这个例子中客户端没有利用收到的视频数据，所以叫做DummySink。
　　客户端调用MediaSink::startPlaying函数开始接收服务器端的数据，这个函数和之前介绍服务器端建立RTSP连接过程时是同一个函数
```
1 Boolean MediaSink::startPlaying(MediaSource& source,
 2                 afterPlayingFunc* afterFunc,
 3                 void* afterClientData) {
 4   // Make sure we're not already being played:
 5   if (fSource != NULL) {
 6     envir().setResultMsg("This sink is already being played");
 7     return False;
 8   }
 9 
10   // Make sure our source is compatible:
11   if (!sourceIsCompatibleWithUs(source)) {
12     envir().setResultMsg("MediaSink::startPlaying(): source is not compatible!");
13     return False;
14   }
15   fSource = (FramedSource*)&source;       //此处的fSource是之前创立的H264VideoRTPSource对象
16 
17   fAfterFunc = afterFunc;
18   fAfterClientData = afterClientData;
19   return continuePlaying();
20 }
　在MediaSink::startPlaying函数中又调用DummySink::continuePlaying函数
```
1 Boolean DummySink::continuePlaying() {
2   if (fSource == NULL) return False; // sanity check (should not happen)
3 
4   // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
5   fSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE,
6                         afterGettingFrame, this,
7                         onSourceClosure, this);
8   return True;
9 }
　　在DummySink::continuePlaying函数中通过H264VideoRTPSource对象请求服务器端的数据，H264VideoRTPSource是MultiFramedRTPSource的子类，请求成功后回调DummySink::afterGettingFrame函数。在FramedSource::getNextFrame函数中，调用了MultiFramedRTPSource::doGetNextFrame函数：
```
1 void MultiFramedRTPSource::doGetNextFrame() {
 2   if (!fAreDoingNetworkReads) {
 3     // Turn on background read handling of incoming packets:
 4     fAreDoingNetworkReads = True;
 5     TaskScheduler::BackgroundHandlerProc* handler
 6       = (TaskScheduler::BackgroundHandlerProc*)&networkReadHandler;
 7     fRTPInterface.startNetworkReading(handler);　　//通过RTPInterface对象读取网络数据，在服务器端是通过RTPInterface对象发送网络数据
　　　　//读到数据后回调networkReadHandler函数来处理
 8   }
 9 
10   fSavedTo = fTo;　　　　　　　　　　　　//读到的数据保存在fTo中
11   fSavedMaxSize = fMaxSize;
12   fFrameSize = 0; // for now
13   fNeedDelivery = True;
14   doGetNextFrame1();
15 }
16 
17 void MultiFramedRTPSource::doGetNextFrame1() {
18   while (fNeedDelivery) {　　　　　　　　　　　　　　　　　　　　　　　　　　19     // If we already have packet data available, then deliver it now.
20     Boolean packetLossPrecededThis;
21     BufferedPacket* nextPacket
22       = fReorderingBuffer->getNextCompletedPacket(packetLossPrecededThis);
23     if (nextPacket == NULL) break;
24 
25     fNeedDelivery = False;
26 
27     if (nextPacket->useCount() == 0) {
28       // Before using the packet, check whether it has a special header
29       // that needs to be processed:
30       unsigned specialHeaderSize;
31       if (!processSpecialHeader(nextPacket, specialHeaderSize)) {
32     // Something's wrong with the header; reject the packet:
33     fReorderingBuffer->releaseUsedPacket(nextPacket);
34     fNeedDelivery = True;
35     break;
36       }
37       nextPacket->skip(specialHeaderSize);
38     }
39 
40     // Check whether we're part of a multi-packet frame, and whether
41     // there was packet loss that would render this packet unusable:
42     if (fCurrentPacketBeginsFrame) {
43       if (packetLossPrecededThis || fPacketLossInFragmentedFrame) {
44     // We didn't get all of the previous frame.
45     // Forget any data that we used from it:
46     fTo = fSavedTo; fMaxSize = fSavedMaxSize;
47     fFrameSize = 0;
48       }
49       fPacketLossInFragmentedFrame = False;
50     } else if (packetLossPrecededThis) {
51       // We're in a multi-packet frame, with preceding packet loss
52       fPacketLossInFragmentedFrame = True;
53     }
54     if (fPacketLossInFragmentedFrame) {
55       // This packet is unusable; reject it:
56       fReorderingBuffer->releaseUsedPacket(nextPacket);
57       fNeedDelivery = True;
58       break;
59     }
60 
61     // The packet is usable. Deliver all or part of it to our caller:
62     unsigned frameSize;
63     nextPacket->use(fTo, fMaxSize, frameSize, fNumTruncatedBytes,
64             fCurPacketRTPSeqNum, fCurPacketRTPTimestamp,
65             fPresentationTime, fCurPacketHasBeenSynchronizedUsingRTCP,
66             fCurPacketMarkerBit);
67     fFrameSize += frameSize;
68 
69     if (!nextPacket->hasUsableData()) {
70       // We're completely done with this packet now
71       fReorderingBuffer->releaseUsedPacket(nextPacket);
72     }
73 
74     if (fCurrentPacketCompletesFrame) {         // 成功读到一帧数据
75       // We have all the data that the client wants.
76       if (fNumTruncatedBytes > 0) {
77     envir() << "MultiFramedRTPSource::doGetNextFrame1(): The total received frame size exceeds the client's buffer size ("
78         << fSavedMaxSize << ").  "
79         << fNumTruncatedBytes << " bytes of trailing data will be dropped!\n";
80       }
81       // Call our own 'after getting' function, so that the downstream object can consume the data:
82       if (fReorderingBuffer->isEmpty()) {
83     // Common case optimization: There are no more queued incoming packets, so this code will not get
84     // executed again without having first returned to the event loop.  Call our 'after getting' function
85     // directly, because there's no risk of a long chain of recursion (and thus stack overflow):
86     afterGetting(this);
87       } else {
88     // Special case: Call our 'after getting' function via the event loop.
89     nextTask() = envir().taskScheduler().scheduleDelayedTask(0,
90                                  (TaskFunc*)FramedSource::afterGetting, this);
91       }
92     } else {
93       // This packet contained fragmented data, and does not complete
94       // the data that the client wants.  Keep getting data:
95       fTo += frameSize; fMaxSize -= frameSize;
96       fNeedDelivery = True;
97     }
98   }
99 }
 　　在doGetNextFrame1函数中，若成功读取到一个完整的帧，则调用Framed::afterGetting函数，进一步回调了DummySink::afterGettingFrame函数
```
1 void DummySink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
 2                   struct timeval presentationTime, unsigned durationInMicroseconds) {
 3   DummySink* sink = (DummySink*)clientData;
 4   sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
 5 }
 6 
 7 // If you don't want to see debugging output for each received frame, then comment out the following line:
 8 #define DEBUG_PRINT_EACH_RECEIVED_FRAME 1
 9 
10 void DummySink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
11                   struct timeval presentationTime, unsigned /*durationInMicroseconds*/) {
12   // We've just received a frame of data.  (Optionally) print out information about it:
13 #ifdef DEBUG_PRINT_EACH_RECEIVED_FRAME
14   if (fStreamId != NULL) envir() << "Stream \"" << fStreamId << "\"; ";
15   envir() << fSubsession.mediumName() << "/" << fSubsession.codecName() << ":\tReceived " << frameSize << " bytes";
16   if (numTruncatedBytes > 0) envir() << " (with " << numTruncatedBytes << " bytes truncated)";
17   char uSecsStr[6+1]; // used to output the 'microseconds' part of the presentation time
18   sprintf(uSecsStr, "%06u", (unsigned)presentationTime.tv_usec);
19   envir() << ".\tPresentation time: " << (int)presentationTime.tv_sec << "." << uSecsStr;
20   if (fSubsession.rtpSource() != NULL && !fSubsession.rtpSource()->hasBeenSynchronizedUsingRTCP()) {
21     envir() << "!"; // mark the debugging output to indicate that this presentation time is not RTCP-synchronized
22   }
23 #ifdef DEBUG_PRINT_NPT
24   envir() << "\tNPT: " << fSubsession.getNormalPlayTime(presentationTime);
25 #endif
26   envir() << "\n";
27 #endif
28   
29   // Then continue, to request the next frame of data:
30   continuePlaying();
31 }
　　在DummySink::afterGettingFrame函数中只是简单地打印出了某个MediaSubsession接收到了多少字节的数据，然后接着利用FramedSource去读取数据。可以看出，在RTSP客户端，Live555也是在MediaSink和FramedSource之间形成了一个循环，不停地从服务器端读取数据。
　live555ProxyServer.cpp在live/proxyServer目录下，这个程序展示了如何利用live555来做一个代理服务器转发rtsp视频（例如，IPCamera的视频）。
　　首先来看一下main函数
```
1 int main(int argc, char** argv) 
 2 {
 3   // Increase the maximum size of video frames that we can 'proxy' without truncation.
 4   // (Such frames are unreasonably large; the back-end servers should really not be sending frames this large!)
 5   OutPacketBuffer::maxSize = 300000; // bytes
 6 
 7   // Begin by setting up our usage environment:
 8   TaskScheduler* scheduler = BasicTaskScheduler::createNew();
 9   env = BasicUsageEnvironment::createNew(*scheduler);
10 
11   /*
12    .... 对各种输入参数的处理，在此略去
13   */
14   
15 // Create the RTSP server.  Try first with the default port number (554),
16   // and then with the alternative port number (8554):
17   RTSPServer* rtspServer;
18   portNumBits rtspServerPortNum = 554;
19   rtspServer = createRTSPServer(rtspServerPortNum);
20   if (rtspServer == NULL) {
21     rtspServerPortNum = 8554;
22     rtspServer = createRTSPServer(rtspServerPortNum);
23   }
24   if (rtspServer == NULL) {
25     *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
26     exit(1);
27   }
28 
29   // Create a proxy for each "rtsp://" URL specified on the command line:
30   for (i = 1; i < argc; ++i) {
31     char const* proxiedStreamURL = argv[i];
32     char streamName[30];
33     if (argc == 2) {
34       sprintf(streamName, "%s", "proxyStream"); // there's just one stream; give it this name
35     } else {
36       sprintf(streamName, "proxyStream-%d", i); // there's more than one stream; distinguish them by name
37     }
38     ServerMediaSession* sms
39       = ProxyServerMediaSession::createNew(*env, rtspServer,
40                        proxiedStreamURL, streamName,
41                        username, password, tunnelOverHTTPPortNum, verbosityLevel);
42     rtspServer->addServerMediaSession(sms);
43     // proxiedStreamURL是代理的源rtsp地址字符串，streamName表示代理后的ServerMediaSession的名字
44     char* proxyStreamURL = rtspServer->rtspURL(sms);
45     *env << "RTSP stream, proxying the stream \"" << proxiedStreamURL << "\"\n";
46     *env << "\tPlay this stream using the URL: " << proxyStreamURL << "\n";
47     delete[] proxyStreamURL;
48   }
49 
50   if (proxyREGISTERRequests) {
51     *env << "(We handle incoming \"REGISTER\" requests on port " << rtspServerPortNum << ")\n";
52   }
53 
54   // Also, attempt to create a HTTP server for RTSP-over-HTTP tunneling.
55   // Try first with the default HTTP port (80), and then with the alternative HTTP
56   // port numbers (8000 and 8080).
57 
58   if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080)) {
59     *env << "\n(We use port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-HTTP tunneling.)\n";
60   } else {
61     *env << "\n(RTSP-over-HTTP tunneling is not available.)\n";
62   }
63 
64   // Now, enter the event loop:
65   env->taskScheduler().doEventLoop(); // does not return
66 
67   return 0; // only to prevent compiler warning
68 }
　　main函数还是很简单，第一行是设置OutPacketBuffer::maxSize的值，经过测试，我设置成300000个字节时就可以传送1080p的视频了。
　　然后还是创建TaskShcheduler和UsageEnvironment对象，中间是对各种输入参数的处理，在此我就省略不作分析了。
　　然后创建RTSPServer，根据输入的rtsp地址串创建ProxyServerMediaSession并添加到RTSPServer，然后开始程序的无限循环。
　　看一下ProxyServerMediaSession这个类
```
1 class ProxyServerMediaSession: public ServerMediaSession {
 2 public:
 3   static ProxyServerMediaSession* createNew(UsageEnvironment& env,
 4                         RTSPServer* ourRTSPServer, // Note: We can be used by just one "RTSPServer"
 5                         char const* inputStreamURL, // the "rtsp://" URL of the stream we'll be proxying
 6                         char const* streamName = NULL,
 7                         char const* username = NULL, char const* password = NULL,
 8                         portNumBits tunnelOverHTTPPortNum = 0,
 9                             // for streaming the *proxied* (i.e., back-end) stream
10                         int verbosityLevel = 0,
11                         int socketNumToServer = -1);
12  // Hack: "tunnelOverHTTPPortNum" == 0xFFFF (i.e., all-ones) means: Stream RTP/RTCP-over-TCP, but *not* using HTTP
13    // "verbosityLevel" == 1 means display basic proxy setup info; "verbosityLevel" == 2 means display RTSP client protocol also.
14 // If "socketNumToServer" >= 0,then it is the socket number of an already-existing TCP connection to the server.
15 //(In this case, "inputStreamURL" must point to the socket's endpoint, so that it can be accessed via the socket.)
16 
17   virtual ~ProxyServerMediaSession();
18 
19   char const* url() const;
20 
21   char describeCompletedFlag;
22     // initialized to 0; set to 1 when the back-end "DESCRIBE" completes.
23     // (This can be used as a 'watch variable' in "doEventLoop()".)
24   Boolean describeCompletedSuccessfully() const { return fClientMediaSession != NULL; }
25     // This can be used - along with "describeCompletdFlag" - to check whether the back-end "DESCRIBE" completed *successfully*.
26 
27 protected:
28   ProxyServerMediaSession(UsageEnvironment& env, RTSPServer* ourRTSPServer,
29               char const* inputStreamURL, char const* streamName,
30               char const* username, char const* password,
31               portNumBits tunnelOverHTTPPortNum, int verbosityLevel,
32               int socketNumToServer,
33               createNewProxyRTSPClientFunc* ourCreateNewProxyRTSPClientFunc
34               = defaultCreateNewProxyRTSPClientFunc);
35 
36   // If you subclass "ProxyRTSPClient", then you will also need to define your own function
37   // - with signature "createNewProxyRTSPClientFunc" (see above) - that creates a new object
38   // of this subclass.  You should also subclass "ProxyServerMediaSession" and, in your
39   // subclass's constructor, initialize the parent class (i.e., "ProxyServerMediaSession")
40   // constructor by passing your new function as the "ourCreateNewProxyRTSPClientFunc"
41   // parameter.
42 
43 protected:
44   RTSPServer* fOurRTSPServer;                  // 添加该ProxyServerMediaSession的RTSPServer对象
45   ProxyRTSPClient* fProxyRTSPClient;　　　　    // 通过一个ProxyRTSPClient对象与给定rtsp服务器进行沟通
46   MediaSession* fClientMediaSession;           // 通过一个MediaSession对象去请求给定rtsp地址表示的媒体资源
47 
48 private:
49   friend class ProxyRTSPClient;
50   friend class ProxyServerMediaSubsession;
51   void continueAfterDESCRIBE(char const* sdpDescription);
52   void resetDESCRIBEState(); // undoes what was done by "contineAfterDESCRIBE()"
53 
54 private:
55   int fVerbosityLevel;
56   class PresentationTimeSessionNormalizer* fPresentationTimeSessionNormalizer;
57   createNewProxyRTSPClientFunc* fCreateNewProxyRTSPClientFunc;
58 };
　　ProxyServerMediaSession是ServerMediaSession的子类，它与普通的ServerMediaSession相比多了三个重要的成员变量：RTSPServer* fOurRTSPServer，ProxyRTSPClient* fProxyRTSPClient，MediaSession* fClientMediaSession。fOurRTSPServer保存添加该ProxyServerMediaSession的RTSPServer对象，fProxyRTSPClient保存该ProxyServerMediaSession对应的ProxyRTSPClient对象，fClientMediaSession保存该ProxyServerMediaSession对应的MediaSession对象。每个ProxyServerMediaSession对应一个ProxyRTSPClient对象和MediaSession对象，从这个地方可以看出，live555代理服务器同时作为RTSP服务器端和RTSP客户端，作为RTSP客户端去获取给定rtsp地址（比如IPCamera的rtsp地址）的媒体资源，然后作为RTSP服务器端转发给其他的RTSP客户端（比如VLC）。
　　ProxyRTSPClient是RTSPClient的子类，我们来看一下它的定义
```
1 // A subclass of "RTSPClient", used to refer to the particular "ProxyServerMediaSession" object being used.
 2 // It is used only within the implementation of "ProxyServerMediaSession", but is defined here, in case developers wish to
 3 // subclass it.
 4 
 5 class ProxyRTSPClient: public RTSPClient {
 6 public:
 7   ProxyRTSPClient(class ProxyServerMediaSession& ourServerMediaSession, char const* rtspURL,
 8                   char const* username, char const* password,
 9                   portNumBits tunnelOverHTTPPortNum, int verbosityLevel, int socketNumToServer);
10   virtual ~ProxyRTSPClient();
11 
12   void continueAfterDESCRIBE(char const* sdpDescription);   //包含了continueAfterDESCRIBE回调函数
13   void continueAfterLivenessCommand(int resultCode, Boolean serverSupportsGetParameter); //发送心跳命令后的回调函数 
14   void continueAfterSETUP();                                //包含了continueAfterSETUP回调函数
15 
16 private:
17   void reset();
18 
19   Authenticator* auth() { return fOurAuthenticator; }
20 
21   void scheduleLivenessCommand();                      // 设置何时执行发送心跳命令的任务
22   static void sendLivenessCommand(void* clientData);   // 发送心跳命令
23 
24   void scheduleDESCRIBECommand();　　　　　　　　　　　　　// 设置何时执行发送DESCRIBE命令的任务
25   static void sendDESCRIBE(void* clientData);　　　　　　// 发送DESCRIBE命令
26 
27   static void subsessionTimeout(void* clientData);
28   void handleSubsessionTimeout();
29 
30 private:
31   friend class ProxyServerMediaSession;
32   friend class ProxyServerMediaSubsession;
33   ProxyServerMediaSession& fOurServerMediaSession;
34   char* fOurURL;
35   Authenticator* fOurAuthenticator;
36   Boolean fStreamRTPOverTCP;
37   class ProxyServerMediaSubsession *fSetupQueueHead, *fSetupQueueTail;
38   unsigned fNumSetupsDone;
39   unsigned fNextDESCRIBEDelay; // in seconds
40   Boolean fServerSupportsGetParameter, fLastCommandWasPLAY;
41   TaskToken fLivenessCommandTask, fDESCRIBECommandTask, fSubsessionTimerTask;
42 };
　　我们接下来看一下创建ProxyServerMediaSession对象的过程
```
1 ProxyServerMediaSession* ProxyServerMediaSession
 2 ::createNew(UsageEnvironment& env, RTSPServer* ourRTSPServer,
 3         char const* inputStreamURL, char const* streamName,
 4         char const* username, char const* password,
 5         portNumBits tunnelOverHTTPPortNum, int verbosityLevel, int socketNumToServer) {
 6   return new ProxyServerMediaSession(env, ourRTSPServer, inputStreamURL, streamName, username, password,
 7                      tunnelOverHTTPPortNum, verbosityLevel, socketNumToServer);
 8 }
 9 
10 
11 ProxyServerMediaSession
12 ::ProxyServerMediaSession(UsageEnvironment& env, RTSPServer* ourRTSPServer,
13               char const* inputStreamURL, char const* streamName,
14               char const* username, char const* password,
15               portNumBits tunnelOverHTTPPortNum, int verbosityLevel,
16               int socketNumToServer,
17               createNewProxyRTSPClientFunc* ourCreateNewProxyRTSPClientFunc)
18   : ServerMediaSession(env, streamName, NULL, NULL, False, NULL),
19     describeCompletedFlag(0), fOurRTSPServer(ourRTSPServer), fClientMediaSession(NULL),
20     fVerbosityLevel(verbosityLevel),
21     fPresentationTimeSessionNormalizer(new PresentationTimeSessionNormalizer(envir())),
22     fCreateNewProxyRTSPClientFunc(ourCreateNewProxyRTSPClientFunc) {
23   // Open a RTSP connection to the input stream, and send a "DESCRIBE" command.
24   // We'll use the SDP description in the response to set ourselves up.
25   fProxyRTSPClient
26     = (*fCreateNewProxyRTSPClientFunc)(*this, inputStreamURL, username, password,
27                        tunnelOverHTTPPortNum,
28                        verbosityLevel > 0 ? verbosityLevel-1 : verbosityLevel,
29                        socketNumToServer);
30   ProxyRTSPClient::sendDESCRIBE(fProxyRTSPClient);
31 }
　　在ProxyServerMediaSession中创建了ProxyRTSPClient对象，是通过fCreateNewProxyRTSPClientFunc函数来创建的，该函数默认是defaultCreateNewProxyRTSPClientFunc函数。
```
1 ProxyRTSPClient*
2 defaultCreateNewProxyRTSPClientFunc(ProxyServerMediaSession& ourServerMediaSession,
3                     char const* rtspURL,
4                     char const* username, char const* password,
5                     portNumBits tunnelOverHTTPPortNum, int verbosityLevel,
6                     int socketNumToServer) {
7   return new ProxyRTSPClient(ourServerMediaSession, rtspURL, username, password,
8                  tunnelOverHTTPPortNum, verbosityLevel, socketNumToServer);
9 }
　　然后就通过刚创建的ProxyRTSPClient对象发送DESCRIBE命令，请求获得媒体资源的SDP信息。
```
1 void ProxyRTSPClient::sendDESCRIBE(void* clientData) {
 2   ProxyRTSPClient* rtspClient = (ProxyRTSPClient*)clientData;
 3   if (rtspClient != NULL) rtspClient->sendDescribeCommand(::continueAfterDESCRIBE, rtspClient->auth());
 4 }
 5 
 6 void ProxyRTSPClient::continueAfterDESCRIBE(char const* sdpDescription) {
 7   if (sdpDescription != NULL) {
 8     fOurServerMediaSession.continueAfterDESCRIBE(sdpDescription);
 9 
10     // Unlike most RTSP streams, there might be a long delay between this "DESCRIBE" command (to the downstream server) and the
11     // subsequent "SETUP"/"PLAY" - which doesn't occur until the first time that a client requests the stream.
12     // To prevent the proxied connection (between us and the downstream server) from timing out, we send periodic 'liveness'
13     // ("OPTIONS" or "GET_PARAMETER") commands.  (The usual RTCP liveness mechanism wouldn't work here, because RTCP packets
14     // don't get sent until after the "PLAY" command.)
15     scheduleLivenessCommand();
16   } else {
17     // The "DESCRIBE" command failed, most likely because the server or the stream is not yet running.
18     // Reschedule another "DESCRIBE" command to take place later:
19     scheduleDESCRIBECommand();
20   }
21 }
22 
23 void ProxyRTSPClient::scheduleLivenessCommand() {
24   // Delay a random time before sending another 'liveness' command.
25   unsigned delayMax = sessionTimeoutParameter(); // if the server specified a maximum time between 'liveness' probes, then use that
26   if (delayMax == 0) {
27     delayMax = 60;
28   }
29 
30   // Choose a random time from [delayMax/2,delayMax-1) seconds:
31   unsigned const us_1stPart = delayMax*500000;
32   unsigned uSecondsToDelay;
33   if (us_1stPart <= 1000000) {
34     uSecondsToDelay = us_1stPart;
35   } else {
36     unsigned const us_2ndPart = us_1stPart-1000000;
37     uSecondsToDelay = us_1stPart + (us_2ndPart*our_random())%us_2ndPart;
38   }
39   fLivenessCommandTask = envir().taskScheduler().scheduleDelayedTask(uSecondsToDelay, sendLivenessCommand, this);
40 }
41 
42 void ProxyRTSPClient::sendLivenessCommand(void* clientData) {
43   ProxyRTSPClient* rtspClient = (ProxyRTSPClient*)clientData;
44 
45   // Note.  By default, we do not send "GET_PARAMETER" as our 'liveness notification' command, even if the server previously
46   // indicated (in its response to our earlier "OPTIONS" command) that it supported "GET_PARAMETER".  This is because
47   // "GET_PARAMETER" crashes some camera servers (even though they claimed to support "GET_PARAMETER").
48 #ifdef SEND_GET_PARAMETER_IF_SUPPORTED
49   MediaSession* sess = rtspClient->fOurServerMediaSession.fClientMediaSession;
50 
51   if (rtspClient->fServerSupportsGetParameter && rtspClient->fNumSetupsDone > 0 && sess != NULL) {
52     rtspClient->sendGetParameterCommand(*sess, ::continueAfterGET_PARAMETER, "", rtspClient->auth());
53   } else {
54 #endif
55     rtspClient->sendOptionsCommand(::continueAfterOPTIONS, rtspClient->auth());
56 #ifdef SEND_GET_PARAMETER_IF_SUPPORTED
57   }
58 #endif
59 }
60 
61 void ProxyRTSPClient::scheduleDESCRIBECommand() {
62   // Delay 1s, 2s, 4s, 8s ... 256s until sending the next "DESCRIBE".  Then, keep delaying a random time from [256..511] seconds:
63   unsigned secondsToDelay;
64   if (fNextDESCRIBEDelay <= 256) {
65     secondsToDelay = fNextDESCRIBEDelay;
66     fNextDESCRIBEDelay *= 2;
67   } else {
68     secondsToDelay = 256 + (our_random()&0xFF); // [256..511] seconds
69   }
70 
71   if (fVerbosityLevel > 0) {
72     envir() << *this << ": RTSP \"DESCRIBE\" command failed; trying again in " << secondsToDelay << " seconds\n";
73   }
74   fDESCRIBECommandTask = envir().taskScheduler().scheduleDelayedTask(secondsToDelay*MILLION, sendDESCRIBE, this);
75 }
76 
77 void ProxyRTSPClient::sendDESCRIBE(void* clientData) {
78   ProxyRTSPClient* rtspClient = (ProxyRTSPClient*)clientData;
79   if (rtspClient != NULL) rtspClient->sendDescribeCommand(::continueAfterDESCRIBE, rtspClient->auth());
80 }
 　　发送DESCRIBE命令后，回调::continueAfterDESCRIBE函数(static void continueAfterDESCRIBE函数)，在该函数中再调用ProxyServerMediaSession::continueAfterDESCRIBE函数，在ProxyServerMediaSession::continueAfterDESCRIBE函数中判断是否成功获取了SDP信息。若成功获取了，则调用ProxyServerMediaSession::continueAfterDESCRIBE，然后调用scheduleLivenessCommand函数设置发送心跳命令的任务；若没有成功获取则调用scheduleDESCRIBECommand函数设置重新发送DESCRIBE命令的任务。
　　ProxyRTSPClient使用GET_PARAMETER命令或者OPTIONS命令作为心跳命令，scheduleLivenessCommand函数中，从[delayMax / 2,delayMax - 1)中随机选取一个值作为发送下一个心跳命令的延时。scheduleDESCRIBECommand函数中，根据上次发送DESCRIBE命令的延时来计算下一次发送DESCRIBE命令的延时，若上次发送DESCRIBE命令的延时小于256s，则按照1,2,4,8,.....256这样一个等比数列来选择一个值作为发送下一个DESCRIBE命令的延时，否则就从[256,511]中随机选择一个值作为下次发送DESCRIBE命令的延时。
　　成功获取SDP信息后，调用ProxyServerMediaSession::continueAfterDESCRIBE函数：
```
1 void ProxyServerMediaSession::continueAfterDESCRIBE(char const* sdpDescription) {
 2   describeCompletedFlag = 1;
 3 
 4   // Create a (client) "MediaSession" object from the stream's SDP description ("resultString"), then iterate through its
 5   // "MediaSubsession" objects, to set up corresponding "ServerMediaSubsession" objects that we'll use to serve the stream's tracks.
 6   do {
 7     fClientMediaSession = MediaSession::createNew(envir(), sdpDescription);
 8     if (fClientMediaSession == NULL) break;
 9 
10     MediaSubsessionIterator iter(*fClientMediaSession);
11     for (MediaSubsession* mss = iter.next(); mss != NULL; mss = iter.next()) {
12       ServerMediaSubsession* smss = new ProxyServerMediaSubsession(*mss);
13       addSubsession(smss);
14       if (fVerbosityLevel > 0) {
15     envir() << *this << " added new \"ProxyServerMediaSubsession\" for "
16         << mss->protocolName() << "/" << mss->mediumName() << "/" << mss->codecName() << " track\n";
17       }
18     }
19   } while (0);
20 }
　　在continueAfterDESCRIBE函数中，首先创建了MediaSession对象，然后创建ProxyServerMediaSubsession对象并添加到ProxyServerMediaSession。ProxyServerMediaSubsession继承自OnDemandServerMediaSubsession类
```
1 class ProxyServerMediaSubsession: public OnDemandServerMediaSubsession {
 2 public:
 3   ProxyServerMediaSubsession(MediaSubsession& mediaSubsession);
 4   virtual ~ProxyServerMediaSubsession();
 5 
 6   char const* codecName() const { return fClientMediaSubsession.codecName(); }
 7 
 8 private: // redefined virtual functions
 9   virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
10                                               unsigned& estBitrate);
11   virtual void closeStreamSource(FramedSource *inputSource);
12   virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
13                                     unsigned char rtpPayloadTypeIfDynamic,
14                                     FramedSource* inputSource);
15 
16 private:
17   static void subsessionByeHandler(void* clientData);
18   void subsessionByeHandler();
19 
20   int verbosityLevel() const { return ((ProxyServerMediaSession*)fParentSession)->fVerbosityLevel; }
21 
22 private:
23   friend class ProxyRTSPClient;
24   MediaSubsession& fClientMediaSubsession; // the 'client' media subsession object that corresponds to this 'server' media subsession
25   ProxyServerMediaSubsession* fNext; // used when we're part of a queue
26   Boolean fHaveSetupStream;
27 };
　　ProxyServerMediaSubsession类中有一个MediaSubsession的引用，一个ProxyServerMediaSubsession对象对应一个MediaSubsession对象。ProxyServerMediaSubsession接下来并不会急着发送SETUP命令，而是等到有RTSP客户端（比如VLC）请求它时再发送SETUP命令去请求建立与IPCamera的连接。
　　然后，RTSPServer等待着RTSP客户端来请求，现在我们假设收到了来自VLC客户端的rtsp请求，然后流程就和前面《建立RTSP连接的过程（RTSP服务器端）》类似。下面我们简要来看一下这个流程，主要突出与之前不同的步骤，我们从RTSPServer::handleCmd_DESCRIBE函数看起：
　　hanleCmd_DESCRIBE函数处理来自客户端的DESCRIBE命令，调用ServerMediaSession::generateSDPDescription函数；
　　ServerMediaSession::generateSDPDescription函数中调用的是OnDemandServerMediaSubsession::sdpLines函数；
　　在sdpLines函数中，调用ProxyServerMediaSubsession::createNewStreamSource函数创建一个临时的FramedSource对象，调用ProxyServerMediaSubsession::createNewRTPSink创建临时的RTPSink对象，然后调用OnDemandServerMediaSubsession::setSDPLinesFromRTPSink函数。
```
1 FramedSource* ProxyServerMediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate)
    {   
  2    ProxyServerMediaSession* const sms = (ProxyServerMediaSession*)fParentSession; 
  3 
  4   if (verbosityLevel() > 0) {
  5     envir() << *this << "::createNewStreamSource(session id " << clientSessionId << ")\n";
  6   }
  7 
  8   // If we haven't yet created a data source from our 'media subsession' object, initiate() it to do so:
  9   if (fClientMediaSubsession.readSource() == NULL) {
 10     fClientMediaSubsession.receiveRawMP3ADUs(); // hack for MPA-ROBUST streams
 11     fClientMediaSubsession.receiveRawJPEGFrames(); // hack for proxying JPEG/RTP streams. (Don't do this if we're transcoding.)
 12     fClientMediaSubsession.initiate();        // 调用MediaSubsession的initiate函数，初始化MediaSubsession对象
 13     if (verbosityLevel() > 0) {
 14       envir() << "\tInitiated: " << *this << "\n";
 15     }
 16      // 在fReadSource前面添加PresentationTimeSessionNormalizer作为Filter
 17     if (fClientMediaSubsession.readSource() != NULL) {
 18       // Add to the front of all data sources a filter that will 'normalize' their frames' presentation times,
 19       // before the frames get re-transmitted by our server:
 20       char const* const codecName = fClientMediaSubsession.codecName();
 21       FramedFilter* normalizerFilter = sms->fPresentationTimeSessionNormalizer
 22     ->createNewPresentationTimeSubsessionNormalizer(fClientMediaSubsession.readSource(), fClientMediaSubsession.rtpSource(),codecName);
 23
 24       fClientMediaSubsession.addFilter(normalizerFilter);   // ProxyServerMediaSubsession的FramedSource以MediaSubsession的FramedSource作为媒体源
 25 
 26       // Some data sources require a 'framer' object to be added, before they can be fed into
 27       // a "RTPSink".  Adjust for this now:
 28       if (strcmp(codecName, "H264") == 0) {  // 再在fReadSource前面添加H264VideoStreamDiscreteFramer作为Filter
 29     fClientMediaSubsession.addFilter(H264VideoStreamDiscreteFramer
 30                      ::createNew(envir(), fClientMediaSubsession.readSource()));
 31       } else if (strcmp(codecName, "H265") == 0) {
 32     fClientMediaSubsession.addFilter(H265VideoStreamDiscreteFramer
 33                      ::createNew(envir(), fClientMediaSubsession.readSource()));
 34       } else if (strcmp(codecName, "MP4V-ES") == 0) {
 35     fClientMediaSubsession.addFilter(MPEG4VideoStreamDiscreteFramer
 36                      ::createNew(envir(), fClientMediaSubsession.readSource(),
 37                              True/* leave PTs unmodified*/));
 38       } else if (strcmp(codecName, "MPV") == 0) {
 39     fClientMediaSubsession.addFilter(MPEG1or2VideoStreamDiscreteFramer
 40                      ::createNew(envir(), fClientMediaSubsession.readSource(),
 41                              False, 5.0, True/* leave PTs unmodified*/));
 42       } else if (strcmp(codecName, "DV") == 0) {
 43     fClientMediaSubsession.addFilter(DVVideoStreamFramer
 44                      ::createNew(envir(), fClientMediaSubsession.readSource(),
 45                              False, True/* leave PTs unmodified*/));
 46       }
 47     }
 48 
 49     if (fClientMediaSubsession.rtcpInstance() != NULL) {
 50       fClientMediaSubsession.rtcpInstance()->setByeHandler(subsessionByeHandler, this);
 51     }
 52   }
 53 
 54   ProxyRTSPClient* const proxyRTSPClient = sms->fProxyRTSPClient;
 55   if (clientSessionId != 0) {　　　　//为了形成SDP信息而创建临时FramedSource时，传入的clientSessionID参数为0，就不会发送SETUP命令
 56     // We're being called as a result of implementing a RTSP "SETUP".
 57     if (!fHaveSetupStream) {
 58       // This is our first "SETUP".  Send RTSP "SETUP" and later "PLAY" commands to the proxied server, to start streaming:
 59       // (Before sending "SETUP", enqueue ourselves on the "RTSPClient"s 'SETUP queue', so we'll be able to get the correct
 60       //  "ProxyServerMediaSubsession" to handle the response.  (Note that responses come back in the same order as requests.))
 61       Boolean queueWasEmpty = proxyRTSPClient->fSetupQueueHead == NULL;
 62       if (queueWasEmpty) {
 63     proxyRTSPClient->fSetupQueueHead = this;
 64       } else {
 65     proxyRTSPClient->fSetupQueueTail->fNext = this;
 66       }
 67       proxyRTSPClient->fSetupQueueTail = this;
 68 
 69       // Hack: If there's already a pending "SETUP" request (for another track), don't send this track's "SETUP" right away, because
 70       // the server might not properly handle 'pipelined' requests.  Instead, wait until after previous "SETUP" responses come back.
 71       if (queueWasEmpty) {   // 发送SETUP命令
 72     proxyRTSPClient->sendSetupCommand(fClientMediaSubsession, ::continueAfterSETUP,
 73                       False, proxyRTSPClient->fStreamRTPOverTCP, False, proxyRTSPClient->auth());
 74     ++proxyRTSPClient->fNumSetupsDone;
 75     fHaveSetupStream = True;
 76       }
 77     } else {
 78       // This is a "SETUP" from a new client.  We know that there are no other currently active clients (otherwise we wouldn't
 79       // have been called here), so we know that the substream was previously "PAUSE"d.  Send "PLAY" downstream once again,
 80       // to resume the stream:
 81       if (!proxyRTSPClient->fLastCommandWasPLAY) { // so that we send only one "PLAY"; not one for each subsession
 82     proxyRTSPClient->sendPlayCommand(fClientMediaSubsession.parentSession(), NULL, -1.0f/*resume from previous point*/,
 83                      -1.0f, 1.0f, proxyRTSPClient->auth());
 84     proxyRTSPClient->fLastCommandWasPLAY = True;
 85       }
 86     }
 87   }
 88 
 89   estBitrate = fClientMediaSubsession.bandwidth();
 90   if (estBitrate == 0) estBitrate = 50; // kbps, estimate
 91   return fClientMediaSubsession.readSource();
 92 }
 93 
 94 RTPSink* ProxyServerMediaSubsession
 95 ::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource) {
 96   if (verbosityLevel() > 0) {
 97     envir() << *this << "::createNewRTPSink()\n";
 98   }
 99 
100   // Create (and return) the appropriate "RTPSink" object for our codec:
101   RTPSink* newSink;
102   char const* const codecName = fClientMediaSubsession.codecName();
103   if (strcmp(codecName, "AC3") == 0 || strcmp(codecName, "EAC3") == 0) {
104     newSink = AC3AudioRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
105                      fClientMediaSubsession.rtpTimestampFrequency()); 
106 #if 0 // This code does not work; do *not* enable it:
107   } else if (strcmp(codecName, "AMR") == 0 || strcmp(codecName, "AMR-WB") == 0) {
108     Boolean isWideband = strcmp(codecName, "AMR-WB") == 0;
109     newSink = AMRAudioRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
110                      isWideband, fClientMediaSubsession.numChannels());
111 #endif
112   } else if (strcmp(codecName, "DV") == 0) {
113     newSink = DVVideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
114   } else if (strcmp(codecName, "GSM") == 0) {
115     newSink = GSMAudioRTPSink::createNew(envir(), rtpGroupsock);
116   } else if (strcmp(codecName, "H263-1998") == 0 || strcmp(codecName, "H263-2000") == 0) {
117     newSink = H263plusVideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
118                           fClientMediaSubsession.rtpTimestampFrequency()); 
119   } else if (strcmp(codecName, "H264") == 0) {  //创建H264VideoRTPSink对象
120     newSink = H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
121                       fClientMediaSubsession.fmtp_spropparametersets());
122   } else if (strcmp(codecName, "H265") == 0) {
123     newSink = H265VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
124                       fClientMediaSubsession.fmtp_spropvps(),
125                       fClientMediaSubsession.fmtp_spropsps(),
126                       fClientMediaSubsession.fmtp_sproppps());
127   } else if (strcmp(codecName, "JPEG") == 0) {
128     newSink = SimpleRTPSink::createNew(envir(), rtpGroupsock, 26, 90000, "video", "JPEG",
129                        1/*numChannels*/, False/*allowMultipleFramesPerPacket*/, False/*doNormalMBitRule*/);
130   } else if (strcmp(codecName, "MP4A-LATM") == 0) {
131     newSink = MPEG4LATMAudioRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
132                            fClientMediaSubsession.rtpTimestampFrequency(),
133                            fClientMediaSubsession.fmtp_config(),
134                            fClientMediaSubsession.numChannels());
135   } else if (strcmp(codecName, "MP4V-ES") == 0) {
136     newSink = MPEG4ESVideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
137                          fClientMediaSubsession.rtpTimestampFrequency(),
138                          fClientMediaSubsession.attrVal_unsigned("profile-level-id"),
139                          fClientMediaSubsession.fmtp_config()); 
140   } else if (strcmp(codecName, "MPA") == 0) {
141     newSink = MPEG1or2AudioRTPSink::createNew(envir(), rtpGroupsock);
142   } else if (strcmp(codecName, "MPA-ROBUST") == 0) {
143     newSink = MP3ADURTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
144   } else if (strcmp(codecName, "MPEG4-GENERIC") == 0) {
145     newSink = MPEG4GenericRTPSink::createNew(envir(), rtpGroupsock,
146                          rtpPayloadTypeIfDynamic, fClientMediaSubsession.rtpTimestampFrequency(),
147                          fClientMediaSubsession.mediumName(),
148                          fClientMediaSubsession.attrVal_strToLower("mode"),
149                          fClientMediaSubsession.fmtp_config(), fClientMediaSubsession.numChannels());
150   } else if (strcmp(codecName, "MPV") == 0) {
151     newSink = MPEG1or2VideoRTPSink::createNew(envir(), rtpGroupsock);
152   } else if (strcmp(codecName, "OPUS") == 0) {
153     newSink = SimpleRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
154                        48000, "audio", "OPUS", 2, False/*only 1 Opus 'packet' in each RTP packet*/);
155   } else if (strcmp(codecName, "T140") == 0) {
156     newSink = T140TextRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
157   } else if (strcmp(codecName, "THEORA") == 0) {
158     newSink = TheoraVideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
159                         fClientMediaSubsession.fmtp_config()); 
160   } else if (strcmp(codecName, "VORBIS") == 0) {
161     newSink = VorbisAudioRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
162                         fClientMediaSubsession.rtpTimestampFrequency(), fClientMediaSubsession.numChannels(),
163                         fClientMediaSubsession.fmtp_config()); 
164   } else if (strcmp(codecName, "VP8") == 0) {
165     newSink = VP8VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
166   } else if (strcmp(codecName, "AMR") == 0 || strcmp(codecName, "AMR-WB") == 0) {
167     // Proxying of these codecs is currently *not* supported, because the data received by the "RTPSource" object is not in a
168     // form that can be fed directly into a corresponding "RTPSink" object.
169     if (verbosityLevel() > 0) {
170       envir() << "\treturns NULL (because we currently don't support the proxying of \""
171           << fClientMediaSubsession.mediumName() << "/" << codecName << "\" streams)\n";
172     }
173     return NULL;
174   } else if (strcmp(codecName, "QCELP") == 0 ||
175          strcmp(codecName, "H261") == 0 ||
176          strcmp(codecName, "H263-1998") == 0 || strcmp(codecName, "H263-2000") == 0 ||
177          strcmp(codecName, "X-QT") == 0 || strcmp(codecName, "X-QUICKTIME") == 0) {
178     // This codec requires a specialized RTP payload format; however, we don't yet have an appropriate "RTPSink" subclass for it:
179     if (verbosityLevel() > 0) {
180       envir() << "\treturns NULL (because we don't have a \"RTPSink\" subclass for this RTP payload format)\n";
181     }
182     return NULL;
183   } else {
184     // This codec is assumed to have a simple RTP payload format that can be implemented just with a "SimpleRTPSink":
185     Boolean allowMultipleFramesPerPacket = True; // by default
186     Boolean doNormalMBitRule = True; // by default
187     // Some codecs change the above default parameters:
188     if (strcmp(codecName, "MP2T") == 0) {
189       doNormalMBitRule = False; // no RTP 'M' bit
190     }
191     newSink = SimpleRTPSink::createNew(envir(), rtpGroupsock,
192                        rtpPayloadTypeIfDynamic, fClientMediaSubsession.rtpTimestampFrequency(),
193                        fClientMediaSubsession.mediumName(), fClientMediaSubsession.codecName(),
194                        fClientMediaSubsession.numChannels(), allowMultipleFramesPerPacket, doNormalMBitRule);
195   }
196 
197   // Because our relayed frames' presentation times are inaccurate until the input frames have been RTCP-synchronized,
198   // we temporarily disable RTCP "SR" reports for this "RTPSink" object:
199   newSink->enableRTCPReports() = False;
200 
201   // Also tell our "PresentationTimeSubsessionNormalizer" object about the "RTPSink", so it can enable RTCP "SR" reports later:
202   PresentationTimeSubsessionNormalizer* ssNormalizer;
203   if (strcmp(codecName, "H264") == 0 ||
204       strcmp(codecName, "H265") == 0 ||
205       strcmp(codecName, "MP4V-ES") == 0 ||
206       strcmp(codecName, "MPV") == 0 ||
207       strcmp(codecName, "DV") == 0) {
208     // There was a separate 'framer' object in front of the "PresentationTimeSubsessionNormalizer", so go back one object to get it:
209     ssNormalizer = (PresentationTimeSubsessionNormalizer*)(((FramedFilter*)inputSource)->inputSource());
210   } else {
211     ssNormalizer = (PresentationTimeSubsessionNormalizer*)inputSource;
212   }
213   ssNormalizer->setRTPSink(newSink);
214 
215   return newSink;
216 }
　　在ProxyServerMediaSubsession::createNewStreamSource函数中，首先调用MediaSubsession::initiate函数进行初始化，然后添加两个Filter：PresentationTimeSessionNormalizer和H264VideoStreamDiscreteFramer。PresentationTimeSessionNormalizer我没有细致的去看，大概的作用应该是给帧打时间戳的，H264VideoStreamDiscreteFramer是用来从接收到的数据分离出每一帧数据。
　　在ProxyServerMediaSubsession::createNewRTPSink函数中，主要就是创建了一个H264VideoRTPSink对象。
　　执行完以上两个函数后，调用OnDemandServerMediaSubsession::setSDPLinesFromRTPSink函数；
　　在setSDPLinesFromRTPSink函数中，调用OnDemandServerMediaSubsession::getAuxSDPLine函数；
　　在getAuxSDPLine函数中，调用H264VideoRTPSink::auxSDPLine函数：
```
1 char const* H264VideoRTPSink::auxSDPLine() {
 2   // Generate a new "a=fmtp:" line each time, using our SPS and PPS (if we have them),
 3   // otherwise parameters from our framer source (in case they've changed since the last time that
 4   // we were called):
 5   H264or5VideoStreamFramer* framerSource = NULL;
 6   u_int8_t* vpsDummy = NULL; unsigned vpsDummySize = 0;
 7   u_int8_t* sps = fSPS; unsigned spsSize = fSPSSize;
 8   u_int8_t* pps = fPPS; unsigned ppsSize = fPPSSize;
 9   if (sps == NULL || pps == NULL) {
10     // We need to get SPS and PPS from our framer source:
11     if (fOurFragmenter == NULL) return NULL; // we don't yet have a fragmenter (and therefore not a source)
12     framerSource = (H264or5VideoStreamFramer*)(fOurFragmenter->inputSource());
13     if (framerSource == NULL) return NULL; // we don't yet have a source
14     //获取VPS、SPS以及PPS信息
15     framerSource->getVPSandSPSandPPS(vpsDummy, vpsDummySize, sps, spsSize, pps, ppsSize);
16     if (sps == NULL || pps == NULL) return NULL; // our source isn't ready
17   }
18 
19   // Set up the "a=fmtp:" SDP line for this stream:
20   u_int8_t* spsWEB = new u_int8_t[spsSize]; // "WEB" means "Without Emulation Bytes"
21   unsigned spsWEBSize = removeH264or5EmulationBytes(spsWEB, spsSize, sps, spsSize);
22   if (spsWEBSize < 4) { // Bad SPS size => assume our source isn't ready
23     delete[] spsWEB;
24     return NULL;
25   }
26   u_int32_t profileLevelId = (spsWEB[1]<<16) | (spsWEB[2]<<8) | spsWEB[3];
27   delete[] spsWEB;
28 
29   char* sps_base64 = base64Encode((char*)sps, spsSize);
30   char* pps_base64 = base64Encode((char*)pps, ppsSize);
31 
32   char const* fmtpFmt =
33     "a=fmtp:%d packetization-mode=1"
34     ";profile-level-id=%06X"
35     ";sprop-parameter-sets=%s,%s\r\n";
36   unsigned fmtpFmtSize = strlen(fmtpFmt)
37     + 3 /* max char len */
38     + 6 /* 3 bytes in hex */
39     + strlen(sps_base64) + strlen(pps_base64);
40   char* fmtp = new char[fmtpFmtSize];
41   sprintf(fmtp, fmtpFmt,
42           rtpPayloadType(),
43       profileLevelId,
44           sps_base64, pps_base64);
45 
46   delete[] sps_base64;
47   delete[] pps_base64;
48 
49   delete[] fFmtpSDPLine; fFmtpSDPLine = fmtp;
50   return fFmtpSDPLine;
51 }
　　在H264VideoRTPSink::auxSDPLine函数中，调用getVPSandSPSandPPS函数获取VPS、SPS和PPS信息，此后将组成的SDP信息发送给RTSP客户端（VLC客户端）。
　　然后RTSPServer就等待RTSP客户端（VLC客户端）发送SETUP命令，收到SETUP命令后就调用RTSPServer::handleCmd_SETUP函数来处理；
　　在handleCmd_SETUP函数中，调用OnDemandServerMediaSubsession::getStreamParameters函数；
　　在getStreamParameters函数中又调用ProxyServerMediaSubsession::createNewStreamSource函数创建FramedSource，调用ProxyServerMediaSubsession::createNewRTPSink函数创建RTPSink。这次调用createNewStreamSource函数的时候传入的参数clientSessionId就是一个非0值，这样在createNewStreamSource函数里，就会发送SETUP命令给IPCamera请求建立连接。并且在收到回复后会回调::continueAfterSETUP（static
 void continueAfterSETUP），在其中又调用ProxyRTSPClient::continueAfterSETUP函数。
```
1 void ProxyRTSPClient::continueAfterSETUP() {
 2   if (fVerbosityLevel > 0) {
 3     envir() << *this << "::continueAfterSETUP(): head codec: " << fSetupQueueHead->fClientMediaSubsession.codecName()
 4         << "; numSubsessions " << fSetupQueueHead->fParentSession->numSubsessions() << "\n\tqueue:";
 5     for (ProxyServerMediaSubsession* p = fSetupQueueHead; p != NULL; p = p->fNext) {
 6       envir() << "\t" << p->fClientMediaSubsession.codecName();
 7     }
 8     envir() << "\n";
 9   }
10   envir().taskScheduler().unscheduleDelayedTask(fSubsessionTimerTask); // in case it had been set
11 
12   // Dequeue the first "ProxyServerMediaSubsession" from our 'SETUP queue'.  It will be the one for which this "SETUP" was done:
13   ProxyServerMediaSubsession* smss = fSetupQueueHead; // Assert: != NULL
14   fSetupQueueHead = fSetupQueueHead->fNext;
15   if (fSetupQueueHead == NULL) fSetupQueueTail = NULL;
16 
17   if (fSetupQueueHead != NULL) {
18     // There are still entries in the queue, for tracks for which we have still to do a "SETUP".
19     // "SETUP" the first of these now:
20     sendSetupCommand(fSetupQueueHead->fClientMediaSubsession, ::continueAfterSETUP,
21              False, fStreamRTPOverTCP, False, fOurAuthenticator);
22     ++fNumSetupsDone;
23     fSetupQueueHead->fHaveSetupStream = True;
24   } else {
25     if (fNumSetupsDone >= smss->fParentSession->numSubsessions()) {
26       // We've now finished setting up each of our subsessions (i.e., 'tracks').
27       // Continue by sending a "PLAY" command (an 'aggregate' "PLAY" command, on the whole session):
28       sendPlayCommand(smss->fClientMediaSubsession.parentSession(), NULL, -1.0f, -1.0f, 1.0f, fOurAuthenticator);
29           // the "-1.0f" "start" parameter causes the "PLAY" to be sent without a "Range:" header, in case we'd already done
30           // a "PLAY" before (as a result of a 'subsession timeout' (note below))
31       fLastCommandWasPLAY = True;
32     } else {
33       // Some of this session's subsessions (i.e., 'tracks') remain to be "SETUP".  They might get "SETUP" very soon, but it's
34       // also possible - if the remote client chose to play only some of the session's tracks - that they might not.
35       // To allow for this possibility, we set a timer.  If the timer expires without the remaining subsessions getting "SETUP",
36       // then we send a "PLAY" command anyway:
37       fSubsessionTimerTask = envir().taskScheduler().scheduleDelayedTask(SUBSESSION_TIMEOUT_SECONDS*MILLION, (TaskFunc*)subsessionTimeout, this);
38
39     }
40   }
41 }
　　在ProxyRTSPClient::continueAfterSETUP函数中，为剩余未建立连接的MediaSubsession发送SETUP命令，当所有的MediaSubsession都建立连接后，向IPCamera发送PLAY命令，开始请求传输媒体流。
 　　然后RTSPServer等待RTSP客户端（VLC客户端）的PLAY命令，收到PLAY命令后，调用RTSPServer::RTSPClientSession::handleCmd_PLAY函数进行处理；
　　然后调用OnDemandServerMediaSubsession::startStream函数，在其中调用StreamState::startPlaying函数；
　　然后就是H264VideoRTPSink不断地从H264VideoStreamDiscreteFramer中获取数据然后传给RTSP客户端（VLC客户端），而H264VideoStreamDiscreteFramer从MediaSubsession的FramedSource获取数据，MediaSubsession的FramedSource从IPCamera获取数据。
　　
　　以上就是live555作为代理服务器转发RTSP实时视频的过程，实际上是综合了前面两篇介绍的流程，对于IPCamera作为RTSP客户端，对于VLC作为RTSP服务器端。
　　关于live555ProxyServer.cpp的几个修改建议：
　   我们可以使用live555ProxyServer.cpp这个程序很方便地构建一个转发RTSP实时视频的代理服务器，比如转发IPCamera的实时视频。但我经过试验发现这个程序还是存在一些问题，还需要作出一些修改才能更好地作为代理服务器运行。由于楼主理解能力有限，这些修改不一定是从根本上解决问题，仅供大家参考。
　　（1）main函数的开头有OutPacketBuffer::maxSize = 300000，原本的语句是OutPacketBuffer::maxSize=30000。但我发现转发高清实时视频的时候，VLC会有大面积马赛克，而live555服务器端也打印出"MultiFramedRTPSink::afterGettingFrame1(): The input frame data was too large for out buffer size .............."。
　　　　我们找到这个提示语句在MultiFramedRTPSink::afterGettingFram1函数中，明显从提示的意思来看是说我们RTPSink的缓冲区设置的太小了，而高清视频的一帧数据太大了。MultiFramedRTPSink将数据保存在fOutBuf中，fOutBuf是指向OutPacketBuffer实例的指针，看一下OutPacketBuffer::totalBytesAvailable函数：
```
1 unsigned totalBytesAvailable() const {
2     return fLimit - (fPacketStart + fCurOffset);
3   }
```
内容很简单，那么totalBytesAvaiable返回值太小的就说明fLimit太小了，fLimit的值在OutPacketBuffer的构造函数中设置了：
```
1 OutPacketBuffer
 2 ::OutPacketBuffer(unsigned preferredPacketSize, unsigned maxPacketSize, unsigned maxBufferSize)
 3   : fPreferred(preferredPacketSize), fMax(maxPacketSize),
 4     fOverflowDataSize(0) {
 5   if (maxBufferSize == 0) maxBufferSize = maxSize;      // maxBufferSize的默认值是0
 6   unsigned maxNumPackets = (maxBufferSize + (maxPacketSize-1))/maxPacketSize;
 7   fLimit = maxNumPackets*maxPacketSize;
 8   fBuf = new unsigned char[fLimit];
 9   resetPacketStart();
10   resetOffset();
11   resetOverflowData();
12 }
可以看出，fLimit的大小取决于maxNumPackets和maxPacketSize，maxPacketSize的值是在MultiFramedRTPSink类的构造函数中设置：
```
1 MultiFramedRTPSink::MultiFramedRTPSink(UsageEnvironment& env,
 2                        Groupsock* rtpGS,
 3                        unsigned char rtpPayloadType,
 4                        unsigned rtpTimestampFrequency,
 5                        char const* rtpPayloadFormatName,
 6                        unsigned numChannels)
 7   : RTPSink(env, rtpGS, rtpPayloadType, rtpTimestampFrequency,
 8         rtpPayloadFormatName, numChannels),
 9     fOutBuf(NULL), fCurFragmentationOffset(0), fPreviousFrameEndedFragmentation(False),
10     fOnSendErrorFunc(NULL), fOnSendErrorData(NULL) {
11   setPacketSizes(1000, 1448);
12       // Default max packet size (1500, minus allowance for IP, UDP, UMTP headers)
13       // (Also, make it a multiple of 4 bytes, just in case that matters.)
14 }
15 
16 void MultiFramedRTPSink::setPacketSizes(unsigned preferredPacketSize,
17                     unsigned maxPacketSize) {
18   if (preferredPacketSize > maxPacketSize || preferredPacketSize == 0) return;
19       // sanity check
20 
21   delete fOutBuf;
22   fOutBuf = new OutPacketBuffer(preferredPacketSize, maxPacketSize);
23   fOurMaxPacketSize = maxPacketSize; // save value, in case subclasses need it
24 }
可以看出maxPacketSize的大小默认值是1448，则fLimit太小就说明了maxBufferSize太小，maxBufferSize = maxSize，因为在OutPacketBuffer类的构造函数声明中可以看到maxBufferSize默认值是0，然后就会被赋值maxSize。而maxSize是OutPacketBuffer类的一个static的成员，因此，只要把OutPacketBuffer::maxSize的值设大一些就可以了。经过测试，我发现设置成300000时就可以转发1080p的高清视频。
　　（2）当向live555请求某一路视频资源的VLC客户端的数量减少到0时，live555会给出以下错误信息 RTCPInstance error: Hit limit when reading incoming packet over TCP. Increase "maxRTCPPacketSize"，我们找到此提示信息在RTCPInstance::incomingReportHandler1函数的最开头。提示信息让我们增大maxRTCPPacketSize的值，可是无论我怎么增大都还是会出现这个信息，无奈不知如何解决，然后觉得关于RTCP的一些包不去处理应该不会对转发数据有太大影响，但这样不停的提示总是很烦的，于是就采用了以下办法：
```
1 void RTCPInstance::incomingReportHandler1() 
 2 {
 3   do {
 4     if (fNumBytesAlreadyRead >= maxRTCPPacketSize) {
 5        memset(fInBuf,0,fNumBytesAlreadyRead);
 6        fNumBytesAlreadyRead = 0;
 7        break;
 8     }
 9     
10    /*
11     ......................     略去
12      
13    */
14 }
　　（3）在live555ProxyServer.cpp的main函数中有一个输入参数是streamRTPOverTCP，streamRTPOverTCP默认是false。
　　首先，想要外网的客户端能访问流媒体服务器，则必须将streamRTPOverTCP设置为True；
      其次，想要转发外网的摄像机，也必须将streamRTPOverTCP设置为True。
　　（4）在ProxyServerMediaSession.cpp文件的ProxyServerMediaSubsession::closeStreamSource函数中，我们需要注释掉if(fHaveSetupStream)这个if语句，因为对于转发实时视频是不支持PAUSE命令的。如果不注释，当请求某一路实时视频的VLC客户端数目减少到0，再有VLC客户端重新请求该视频时就无法再播放了。
　　（5）对于同一路视频流，当请求的VLC客户端越来越多时，会发现后面请求的VLC客户端正在播放但没有图像。我们找到RTPInterface.cpp文件中RTPInterface的构造函数，注释掉其中调用makeSokcetNonBlocking函数的那一句即可。
```
1 RTPInterface::RTPInterface(Medium* owner, Groupsock* gs)
 2   : fOwner(owner), fGS(gs),
 3     fTCPStreams(NULL),
 4     fNextTCPReadSize(0), fNextTCPReadStreamSocketNum(-1),
 5     fNextTCPReadStreamChannelId(0xFF), fReadHandlerProc(NULL),
 6     fAuxReadHandlerFunc(NULL), fAuxReadHandlerClientData(NULL) {
 7   // Make the socket non-blocking, even though it will be read from only asynchronously, when packets arrive.
 8   // The reason for this is that, in some OSs, reads on a blocking socket can (allegedly) sometimes block,
 9   // even if the socket was previously reported (e.g., by "select()") as having data available.
10   // (This can supposedly happen if the UDP checksum fails, for example.)
11         
12   //makeSocketNonBlocking(fGS->socketNum());           //注释掉这一句
13   increaseSendBufferTo(envir(), fGS->socketNum(), 50*1024);
14 }
　　makeSocketNonBlocking这个函数顾名思义是使某个Socket成为非阻塞式的，在RTPInterface构造函数调用的这一句就是使发送RTP包给VLC客户端的Socket成为非阻塞。由于多个VLC客户端共享RTPInteface缓冲区中的RTP数据，那么当从IPCamera获取数据的速率要快于将缓冲区中的数据发送给所有VLC客户端的速率时（这种情况应该只可能发生在局域网内的测试环境，在生产环境中建议还是不要注释这一句了），缓冲区的数据就会被冲刷导致后面播放的VLC客户端播放不出图像。将makeSocketNonBlocking这一句注释掉后，就会等到给所有的VLC客户端都发送完数据后才会再从IPCamera获取数据。
　　（6）在OnDemandServerMediaSubsession.cpp文件中，找到OnDemandServerMediaSubsession::deleteStream函数
```
1 void OnDemandServerMediaSubsession::deleteStream(unsigned clientSessionId,
 2                          void*& streamToken) {
 3   StreamState* streamState = (StreamState*)streamToken;
 4 
 5   // Look up (and remove) the destinations for this client session:
 6   Destinations* destinations
 7     = (Destinations*)(fDestinationsHashTable->Lookup((char const*)clientSessionId));
 8   if (destinations != NULL) {
 9     fDestinationsHashTable->Remove((char const*)clientSessionId);
10 
11     // Stop streaming to these destinations:
12     if (streamState != NULL) streamState->endPlaying(destinations);
13   }
14 
15   // Delete the "StreamState" structure if it's no longer being used:
16   if (streamState != NULL) {
17     if (streamState->referenceCount() > 0) --streamState->referenceCount();
18     if (streamState->referenceCount() == 0) { //将这一句修改为if(streamState->referenceCount() == 0 && fParentSession->deleteWhenUnreferenced())
19       delete streamState;
20       streamToken = NULL;
21     }
22   }
23 
24   // Finally, delete the destinations themselves:
25   delete destinations;
26 }
　　将if(streamState->referenceCount() == 0)修改为 if(streamState->referenceCount() == 0 && fParentSession->deleteWhenUnreferenced())。修改之前，当请求某路视频资源的VLC客户端的数目减少到0时，就会delete streamState，即释放与该路视频流相关的资源，这样下次再有VLC客户端请求该路视频资源时，就需要重新申请资源，速度会比较慢。而且，在Windows下测试发现，执行delete
 streamState这一句时偶尔会发生异常崩溃。
　　ProxyServerMediaSubsession是OnDemandServerMediaSubsession的子类，但对于ProxyServerMediaSubsession而言，我们可以在请求该路视频流的VLC客户端数目减少到0时不释放相关资源，这样后面再有VLC客户端请求时速度就会加快。ServerMeiaSubsessio类中会保存有父会话ServerMediaSession的指针，ServerMediaSession类有一个属性fDeleteWhenUnreferenced，这个属性表示当不再被请求时是否删除会话并释放资源，默认是false。
　　（7）楼主本想使用此程序开发出一个Live555的代理服务器，结果发现在局域网内，Live555转发IPCamera的视频给VLC客户端，延时都将近3s（这个地方的延时是指VLC点击播放按钮后要等3s才能出来图像），楼主最终也找不到解决的办法（听说出不来图像是因为还没有I帧，但楼主水平有限，对视频编码什么的一窍不通）。哪位仁兄找到解决办法请联系我，谢谢。
     （8）问题（7）后来发现是VLC播放器的用法不正确，网上说是设置缓存时间的问题，在局域网内设置缓存时间是有些效果，但对于客户端在外网访问流媒体服务器，还是很慢，后来发现在"工具-首选项"中设置"live555流传输"方式为"RTP over RTSP"即可。如下图所示：
![](http://images0.cnblogs.com/blog2015/688670/201506/102031500358690.jpg)
　　此问题刚解决后，又发现一个由此而来的新问题：对于同一路流，后面请求的VLC客户端会把前面请求的VLC客户端"挤掉"，具体表现是，后开启的客户端开始播放画面时，前面请求的客户端的画面就停止不动了，然后紧接着就和流媒体服务器断开了连接。并且，在之前将"Live555流传输"设置为"HTTP"时没有这个问题。
　　后来发现将RTPInterface::sendRTPorRTCPPacketOverTCP函数中的if(!sendDataOverTCP(socketNum,framingHeader,4,False))中的False修改为True即可。
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
