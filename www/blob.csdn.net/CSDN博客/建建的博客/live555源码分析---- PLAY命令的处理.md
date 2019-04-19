# live555源码分析---- PLAY命令的处理 - 建建的博客 - CSDN博客
2018年10月29日 17:24:30[纪建](https://me.csdn.net/u013898698)阅读数：61
PLAY命令概述
PLAY命令要求在SETUP命令之后进行，此命令处理过程中就开始发送数据了，在处理PLAY命令过程中还创建了RTCPInstance实例。
客户端可以通过PLAY命令的Scale头部域，指定播放速率，不过这个功能要看服务器对特定媒体的具体实现，当sacale＝1时正常播放，sacale>1时快进，sacale<0时快退。
客户端可以通过PLAY命令的Range头部域，指定播放的时间范围，同样此功能也依赖于服务器中特定媒体的具体实现。
对于PLAY命令请求中的URL有以下几种情况(与PAUSE、TEARDOWN、GET_PARAMETER、SET_PARAMETER处理是一样的):
1)非聚合，如rtsp://192.168.1.1/urlPreSuffix/urlSuffix,urlPreSuffix作为stream name, urlSuffix作为subsession的trackId
2)非聚合的情况下，才能根据trackId找到subsession
3)聚合，如
    rtsp://192.168.1.1/urlPreSuffix/urlSuffix, 将urlSuffix作为stream name，而urlPreSuffix忽略
    rtsp://192.168.1.1/urlPreSuffix, 只存在urlPreSuffix，并将其作为stream name, 这应该是最常见的情况
4)聚合，如rtsp://192.168.1.1/urlPreSuffix/urlSuffix,　将urlPreSuffix/urlSuffix整个作为stream name 
我们可以对session中的subsession进行单独控制(这需要提供subsession的trackId), 也可以对整个session进行控制(这种情况应该是最常见的吧)。
贴一个SETUP消息实例：
PLAY rtsp://192.168.9.80/123.mpg/ RTSP/1.0
CSeq: 5
Session: 263BD44B
Range: npt=0.000-
User-Agent: LibVLC/1.1.0 (LIVE555 Streaming Media v2010.03.16)
response: RTSP/1.0 200 OK
CSeq: 5
Date: Wed, Nov 30 2011 06:55:07 GMT
Range: npt=0.000-
Session: 263BD44B
RTP-Info: url=rtsp://192.168.9.80/123.mpg/track1;seq=38851;rtptime=1434098600,ur
l=rtsp://192.168.9.80/123.mpg/track2;seq=27752;rtptime=3595585826
代码分析的过程比较烦琐，就先把总结性的东西放到最前面了
1.关于SETUP命令请求包中的ULR处理
void RTSPServer::RTSPClientSession
::handleCmd_withinSession(char const* cmdName,
              char const* urlPreSuffix, char const* urlSuffix,
              char const* cseq, char const* fullRequestStr) {
    //非聚合，如rtsp://192.168.1.1/urlPreSuffix/urlSuffix,urlPreSuffix作为stream name, urlSuffix作为subsession的trackId
    //非聚合的情况下，才能根据trackId找到subsession
    //聚合，如
    //1)rtsp://192.168.1.1/urlPreSuffix/urlSuffix, 将urlSuffix作为stream name，而urlPreSuffix忽略
    //2)rtsp://192.168.1.1/urlPreSuffix, 只存在urlPreSuffix，并将其作为stream name, 这应该是最常见的情况
    //聚合，如rtsp://192.168.1.1/urlPreSuffix/urlSuffix,　将urlPreSuffix/urlSuffix整个作为stream name 
  // This will either be:
  // - an operation on the entire server, if "urlPreSuffix" is "", and "urlSuffix" is "*" (i.e., the special "*" URL), or
  // - a non-aggregated operation, if "urlPreSuffix" is the session (stream)
  //   name and "urlSuffix" is the subsession (track) name, or
  // - an aggregated operation, if "urlSuffix" is the session (stream) name,
  //   or "urlPreSuffix" is the session (stream) name, and "urlSuffix" is empty,
  //   or "urlPreSuffix" and "urlSuffix" are both nonempty, but when concatenated, (with "/") form the session (stream) name.
  // Begin by figuring out which of these it is:
  ServerMediaSubsession* subsession;
  if (urlPreSuffix[0] == '\0' && urlSuffix[0] == '*' && urlSuffix[1] == '\0') {
    // An operation on the entire server.  This works only for GET_PARAMETER and SET_PARAMETER:
    if (strcmp(cmdName, "GET_PARAMETER") == 0) {
      handleCmd_GET_PARAMETER(NULL, cseq, fullRequestStr);
    } else if (strcmp(cmdName, "SET_PARAMETER") == 0) {
      handleCmd_SET_PARAMETER(NULL, cseq, fullRequestStr);
    } else {
      handleCmd_notSupported(cseq);
    }
    return;
  } else if (fOurServerMediaSession == NULL) { // There wasn't a previous SETUP!
    handleCmd_notSupported(cseq);
    return;
  } else if (urlSuffix[0] != '\0' && strcmp(fOurServerMediaSession->streamName(), urlPreSuffix) == 0) {
    //非聚合，如rtsp://192.168.1.1/urlPreSuffix/urlSuffix,urlPreSuffix作为stream name, urlSuffix作为subsession的trackId
    //非聚合的情况下，才能根据trackId找到subsession
    // Non-aggregated operation.
    // Look up the media subsession whose track id is "urlSuffix":
    ServerMediaSubsessionIterator iter(*fOurServerMediaSession);
    while ((subsession = iter.next()) != NULL) {
      if (strcmp(subsession->trackId(), urlSuffix) == 0) break; // success
    }
    if (subsession == NULL) { // no such track!
      handleCmd_notFound(cseq);
      return;
    }
  } else if (strcmp(fOurServerMediaSession->streamName(), urlSuffix) == 0 ||
         (urlSuffix[0] == '\0' && strcmp(fOurServerMediaSession->streamName(), urlPreSuffix) == 0)) {
    //聚合，如
    //1)rtsp://192.168.1.1/urlPreSuffix/urlSuffix, 将urlSuffix作为stream name，而urlPreSuffix忽略
    //2)rtsp://192.168.1.1/urlPreSuffix, 只存在urlPreSuffix，并将其作为stream name
    // Aggregated operation
    subsession = NULL;
  } else if (urlPreSuffix[0] != '\0' && urlSuffix[0] != '\0') {
     //聚合，如rtsp://192.168.1.1/urlPreSuffix/urlSuffix,　将urlPreSuffix/urlSuffix整个作为stream name 
    //Aggregated operation,
    // Aggregated operation, if <urlPreSuffix>/<urlSuffix> is the session (stream) name:
    unsigned const urlPreSuffixLen = strlen(urlPreSuffix);
    if (strncmp(fOurServerMediaSession->streamName(), urlPreSuffix, urlPreSuffixLen) == 0 &&
    fOurServerMediaSession->streamName()[urlPreSuffixLen] == '/' &&
    strcmp(&(fOurServerMediaSession->streamName())[urlPreSuffixLen+1], urlSuffix) == 0) {
      subsession = NULL;
    } else {
      handleCmd_notFound(cseq);
      return;
    }
  } else { // the request doesn't match a known stream and/or track at all!
    handleCmd_notFound(cseq);
    return;
  }
  if (strcmp(cmdName, "TEARDOWN") == 0) {
    handleCmd_TEARDOWN(subsession, cseq);
  } else if (strcmp(cmdName, "PLAY") == 0) {
    handleCmd_PLAY(subsession, cseq, fullRequestStr);
  } else if (strcmp(cmdName, "PAUSE") == 0) {
    handleCmd_PAUSE(subsession, cseq);
  } else if (strcmp(cmdName, "GET_PARAMETER") == 0) {
    handleCmd_GET_PARAMETER(subsession, cseq, fullRequestStr);
  } else if (strcmp(cmdName, "SET_PARAMETER") == 0) {
    handleCmd_SET_PARAMETER(subsession, cseq, fullRequestStr);
  }
}
2.PLAY命令处理函数handleCmd_PLAY(1.1)
void RTSPServer::RTSPClientSession
  ::handleCmd_PLAY(ServerMediaSubsession* subsession, char const* cseq,
  char const* fullRequestStr) {
  char* rtspURL = fOurServer.rtspURL(fOurServerMediaSession, fClientInputSocket);
  unsigned rtspURLSize = strlen(rtspURL);
  //分析"Scale:"头部
  //Scale头，指示了播放的速率，scale = 1为正常播放，大于1快进，小于0则表示快退
  // Parse the client's "Scale:" header, if any:
  float scale;
  Boolean sawScaleHeader = parseScaleHeader(fullRequestStr, scale); 
  //测试scale的值是否能满足，这期间可能会改变scale的值
  // Try to set the stream's scale factor to this value:
  if (subsession == NULL /*aggregate op*/) {    //聚合的情况下，subsession还不确定
    fOurServerMediaSession->testScaleFactor(scale); //测试scale的值(见2.1)
  } else {
    subsession->testScaleFactor(scale);
  }
  char buf[100];
  char* scaleHeader;
  if (!sawScaleHeader) {
    buf[0] = '\0'; // Because we didn't see a Scale: header, don't send one back
  } else {
    sprintf(buf, "Scale: %f\r\n", scale);
  }
  scaleHeader = strDup(buf);
   //分析"Range:"头部
   //"Range:"头部，表示要播放的时间范围。如Range: npt=0.000-，从0时刻开始播放看到结束　
    //不含Range 首部域的PLAY 请求也是合法的。它从媒体流开头开始播放，直到媒体流被暂停
  // Parse the client's "Range:" header, if any:    
  double rangeStart = 0.0, rangeEnd = 0.0;
  Boolean sawRangeHeader = parseRangeHeader(fullRequestStr, rangeStart, rangeEnd);
  //关于"Range:"头部的其它操作
   ...
  //下面创建响应中的"RTP-Info:"行
  // Create a "RTP-Info:" line.  It will get filled in from each subsession's state:
  char const* rtpInfoFmt =
    "%s" // "RTP-Info:", plus any preceding rtpInfo items
    "%s" // comma separator, if needed
    "url=%s/%s"
    ";seq=%d"
    ";rtptime=%u"
    ;
  unsigned rtpInfoFmtSize = strlen(rtpInfoFmt);
  char* rtpInfo = strDup("RTP-Info: ");
  unsigned i, numRTPInfoItems = 0;
 //根据要求，在每个subsession上进行seeking/scaling操作
  // Do any required seeking/scaling on each subsession, before starting streaming:
  for (i = 0; i < fNumStreamStates; ++i) {
    if (subsession == NULL /* means: aggregated operation */
|| subsession == fStreamStates[i].subsession) {
      if (sawScaleHeader) {
fStreamStates[i].subsession->setStreamScale(fOurSessionId,      //设置subsession的scale值(见2.1) 
   fStreamStates[i].streamToken,
   scale);
      }
      if (sawRangeHeader) {
    //计算流的播放时间streamDuration    
double streamDuration = 0.0; // by default; means: stream until the end of the media
if (rangeEnd > 0.0 && (rangeEnd+0.001) < duration) { // the 0.001 is because we limited the values to 3 decimal places
 // We want the stream to end early.  Set the duration we want:
 streamDuration = rangeEnd - rangeStart;
 if (streamDuration < 0.0) streamDuration = -streamDuration; // should happen only if scale < 0.0  这里情况下进行快退操作
}
u_int64_t numBytes;
fStreamStates[i].subsession->seekStream(fOurSessionId,      //设置每个subsession上的播放时间范围(见2.2)
fStreamStates[i].streamToken,
rangeStart, streamDuration, numBytes);  
      }
    }
  }
  // Create the "Range:" header that we'll send back in our response.
  // (Note that we do this after seeking, in case the seeking operation changed the range start time.)
  char* rangeHeader;
  if (!sawRangeHeader) {
    buf[0] = '\0'; // Because we didn't see a Range: header, don't send one back
  } else if (rangeEnd == 0.0 && scale >= 0.0) {
    sprintf(buf, "Range: npt=%.3f-\r\n", rangeStart);
  } else {
    sprintf(buf, "Range: npt=%.3f-%.3f\r\n", rangeStart, rangeEnd);
  }
  rangeHeader = strDup(buf);
  //现在终于开始媒体数据传输了
  // Now, start streaming:
  for (i = 0; i < fNumStreamStates; ++i) {
    if (subsession == NULL /* means: aggregated operation */
|| subsession == fStreamStates[i].subsession) {
      unsigned short rtpSeqNum = 0;
      unsigned rtpTimestamp = 0;
      //开始各个subsession上的数据传输, 即开始播放了(见2.3)
      fStreamStates[i].subsession->startStream(fOurSessionId,
      fStreamStates[i].streamToken,
      (TaskFunc*)noteClientLiveness, this,
      rtpSeqNum, rtpTimestamp,
      handleAlternativeRequestByte, this);
      const char *urlSuffix = fStreamStates[i].subsession->trackId();
      char* prevRTPInfo = rtpInfo;
      unsigned rtpInfoSize = rtpInfoFmtSize
+ strlen(prevRTPInfo)
+ 1
+ rtspURLSize + strlen(urlSuffix)
+ 5 /*max unsigned short len*/
+ 10 /*max unsigned (32-bit) len*/
+ 2 /*allows for trailing \r\n at final end of string*/;
      rtpInfo = new char[rtpInfoSize];
      //subsession中的信息添加到"RTP-Info:"行中
      sprintf(rtpInfo, rtpInfoFmt,
     prevRTPInfo,
     numRTPInfoItems++ == 0 ? "" : ",",
     rtspURL, urlSuffix,
     rtpSeqNum,
     rtpTimestamp
     );
      delete[] prevRTPInfo;
    }
  }
  //下面是组装响应包的操作
...
}
3.关于播放速度参数scale(2.1)
scale参数指定了播放的速率，scale = 1为正常播放，大于1快进，小于0则表示快退。是否能满足scale的要求，要看服务器是否支持。看ServerMediaSession中的测试函数
void ServerMediaSession::testScaleFactor(float& scale) {
  // First, try setting all subsessions to the desired scale.
  // If the subsessions' actual scales differ from each other, choose the
  // value that's closest to 1, and then try re-setting all subsessions to that
  // value.  If the subsessions' actual scales still differ, re-set them all to 1.
  float minSSScale = 1.0;
  float maxSSScale = 1.0;
  float bestSSScale = 1.0;
  float bestDistanceTo1 = 0.0;
  ServerMediaSubsession* subsession;
  for (subsession = fSubsessionsHead; subsession != NULL;
       subsession = subsession->fNext) {
    float ssscale = scale;
    subsession->testScaleFactor(ssscale);
    if (subsession == fSubsessionsHead) { // this is the first subsession
      minSSScale = maxSSScale = bestSSScale = ssscale;
      bestDistanceTo1 = (float)fabs(ssscale - 1.0f);
    } else {
      if (ssscale < minSSScale) {
minSSScale = ssscale;
      } else if (ssscale > maxSSScale) {
maxSSScale = ssscale;
      }
      float distanceTo1 = (float)fabs(ssscale - 1.0f);
      if (distanceTo1 < bestDistanceTo1) {
bestSSScale = ssscale;
bestDistanceTo1 = distanceTo1;
      }
    }
  }
  if (minSSScale == maxSSScale) {
    // All subsessions are at the same scale: minSSScale == bestSSScale == maxSSScale
    scale = minSSScale;
    return;
  }
  // The scales for each subsession differ.  Try to set each one to the value
  // that's closest to 1:
  for (subsession = fSubsessionsHead; subsession != NULL;
       subsession = subsession->fNext) {
    float ssscale = bestSSScale;
    subsession->testScaleFactor(ssscale);
    if (ssscale != bestSSScale) break; // no luck
  }
  if (subsession == NULL) {
    // All subsessions are at the same scale: bestSSScale
    scale = bestSSScale;
    return;
  }
  // Still no luck.  Set each subsession's scale to 1:
  for (subsession = fSubsessionsHead; subsession != NULL;
       subsession = subsession->fNext) {
    float ssscale = 1;
    subsession->testScaleFactor(ssscale);
  }
  scale = 1;
}
上面的函数处理过程有些繁锁，主要是处理各subsession支持不同的scale的情况， 这种情况下将选取scale值最靠近1的值(1为正常速率)。上面的函数中实际上调用了subsession上的testScaleFactor函数。其在ServerMediaSubsession类中有默认实现，如下：
void ServerMediaSubsession::testScaleFactor(float& scale) {
  // default implementation: Support scale = 1 only
  scale = 1;
}
默认只能正常播放，再来看subsession上的setStreamScale函数
void ServerMediaSubsession::setStreamScale(unsigned /*clientSessionId*/,
  void* /*streamToken*/, float /*scale*/) {
  // default implementation: do nothing
}
do nothing!不过这是一个虚函数，在nDemandServerMediaSubsession中有重新实现
void OnDemandServerMediaSubsession::setStreamScale(unsigned /*clientSessionId*/,
  void* streamToken, float scale) {
  //当多个客户端从同一个source接收数据时，sacale值是不能改变的
  // Changing the scale factor isn't allowed if multiple clients are receiving data
  // from the same source:
  if (fReuseFirstSource) return;
  StreamState* streamState = (StreamState*)streamToken;
  if (streamState != NULL && streamState->mediaSource() != NULL) {
    setStreamSourceScale(streamState->mediaSource(), scale);
  }
}
继续看setStreamSourceScale函数
void OnDemandServerMediaSubsession
::setStreamSourceScale(FramedSource* /*inputSource*/, float /*scale*/) {
  // Default implementation: Do nothing
}
什么都做，这样如果要实现快进/快退操作, 只需要在自己的subsession中重新实现这个函数即可。
4.设置播放的时间范围(2.2)
seekStream是ServerMediaSubsession类的一个虚函数，默认实现do nothing，直接看OnDemandServerMediaSubsession类中的实现
void OnDemandServerMediaSubsession::seekStream(unsigned /*clientSessionId*/,
      void* streamToken, double& seekNPT, double streamDuration, u_int64_t& numBytes) {
  numBytes = 0; // by default: unknown
  //同样的多个客户端对应同一个source时，不充许此操作
  // Seeking isn't allowed if multiple clients are receiving data from
  // the same source:
  if (fReuseFirstSource) return;
  StreamState* streamState = (StreamState*)streamToken;
  if (streamState != NULL && streamState->mediaSource() != NULL) {
    seekStreamSource(streamState->mediaSource(), seekNPT, streamDuration, numBytes);
  }
}
seekStreamSource也是定义在OnDemandServerMediaSubsession上的虚函数，默认实现也是do nothing, 需要在自己的子类中重新实现。看了下H264VideoFileServerMediaSubsession类，并没有去实现seekStreamSource，所以*.264的文件每次打开只能从头看起了。
5.开始播放(2.3)
进行了如此多的工作，终于要开始播放了，期待。。。
startStream函数是定义在ServerMediaSubsession类中的纯虚函数，首先来看其子类OnDemandServerMediaSubsession中的实现
void OnDemandServerMediaSubsession::startStream(unsigned clientSessionId,
void* streamToken,
TaskFunc* rtcpRRHandler,
void* rtcpRRHandlerClientData,
unsigned short& rtpSeqNum,
unsigned& rtpTimestamp,
ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
void* serverRequestAlternativeByteHandlerClientData) {
  StreamState* streamState = (StreamState*)streamToken;
  Destinations* destinations
    = (Destinations*)(fDestinationsHashTable->Lookup((char const*)clientSessionId));
  if (streamState != NULL) {
    streamState->startPlaying(destinations,
     rtcpRRHandler, rtcpRRHandlerClientData,
     serverRequestAlternativeByteHandler, serverRequestAlternativeByteHandlerClientData);
    if (streamState->rtpSink() != NULL) {
      rtpSeqNum = streamState->rtpSink()->currentSeqNo();       //这个rtpSeqNum有什么用呢？
      rtpTimestamp = streamState->rtpSink()->presetNextTimestamp();
    }
  }
}
这里主要调用函数StreamState::startPlaying
void StreamState::startPlaying(Destinations* dests,
      TaskFunc* rtcpRRHandler, void* rtcpRRHandlerClientData,
      ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
      void* serverRequestAlternativeByteHandlerClientData) {
  if (dests == NULL) return;
  //创建RTCPInstance实例
  if (fRTCPInstance == NULL && fRTPSink != NULL) {
    // Create (and start) a 'RTCP instance' for this RTP sink:
    fRTCPInstance
      = RTCPInstance::createNew(fRTPSink->envir(), fRTCPgs,
fTotalBW, (unsigned char*)fMaster.fCNAME,
fRTPSink, NULL /* we're a server */);
        // Note: This starts RTCP running automatically
  }
  if (dests->isTCP) {
    //使用TCP传输RTP 和 RTCP
    // Change RTP and RTCP to use the TCP socket instead of UDP:
    if (fRTPSink != NULL) {
      fRTPSink->addStreamSocket(dests->tcpSocketNum, dests->rtpChannelId);
      fRTPSink->setServerRequestAlternativeByteHandler(dests->tcpSocketNum, serverRequestAlternativeByteHandler, serverRequestAlternativeByteHandlerClientData);
    }
    if (fRTCPInstance != NULL) {
      fRTCPInstance->addStreamSocket(dests->tcpSocketNum, dests->rtcpChannelId);
      fRTCPInstance->setSpecificRRHandler(dests->tcpSocketNum, dests->rtcpChannelId,
 rtcpRRHandler, rtcpRRHandlerClientData);
    }
  } else {
    //使用UDP传输RTP、RTCP
    // Tell the RTP and RTCP 'groupsocks' about this destination
    // (in case they don't already have it):
    if (fRTPgs != NULL) fRTPgs->addDestination(dests->addr, dests->rtpPort);
    if (fRTCPgs != NULL) fRTCPgs->addDestination(dests->addr, dests->rtcpPort);
    if (fRTCPInstance != NULL) {
      fRTCPInstance->setSpecificRRHandler(dests->addr.s_addr, dests->rtcpPort,
 rtcpRRHandler, rtcpRRHandlerClientData);
    }
  }
  //下面调用sink上的sdtartPlaying函数开始传输数据
  if (!fAreCurrentlyPlaying && fMediaSource != NULL) {
    if (fRTPSink != NULL) {     //通过RTP协议传输
      fRTPSink->startPlaying(*fMediaSource, afterPlayingStreamState, this);
      fAreCurrentlyPlaying = True;
    } else if (fUDPSink != NULL) {  //裸的UDP数据包，不使用RTP协议 
      fUDPSink->startPlaying(*fMediaSource, afterPlayingStreamState, this);
      fAreCurrentlyPlaying = True;
    }
  }
}
上面的函数最终调用了MediaSink::startPlaying函数，开始传输数据，这个过程的函数调用比较复杂，将在下一篇文章中单独分析。大致过程是，先取一个帧发送出去，然后此函数就返回了，再处理response包的发送等剩余操作。
--------------------- 
作者：gavinr 
来源：CSDN 
原文：https://blog.csdn.net/gavinr/article/details/7031437 
版权声明：本文为博主原创文章，转载请附上博文链接！
