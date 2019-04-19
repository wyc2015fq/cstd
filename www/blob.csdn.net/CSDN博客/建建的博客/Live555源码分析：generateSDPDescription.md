# Live555源码分析：generateSDPDescription - 建建的博客 - CSDN博客
2017年02月24日 10:15:22[纪建](https://me.csdn.net/u013898698)阅读数：371
 [3]generateSDPDescription
有了上述知识,我们继续看一下RTSPServer::RTSPClientConnection::handleCmd_DESCRIBE的处理.
**[cpp]**[view
 plain](http://blog.csdn.net/njzhujinhua/article/details/30087927#)[copy](http://blog.csdn.net/njzhujinhua/article/details/30087927#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/387721/fork)
- void RTSPServer::RTSPClientConnection::handleCmd_DESCRIBE(charconst* urlPreSuffix, charconst* urlSuffix, charconst* fullRequestStr)  
- {  
- char* sdpDescription = NULL;  
- char* rtspURL = NULL;  
- do {  
- char urlTotalSuffix[RTSP_PARAM_STRING_MAX];  
- if (strlen(urlPreSuffix) + strlen(urlSuffix) + 2 > sizeof urlTotalSuffix)  
-         {  
-             handleCmd_bad();  
- break;  
-         }  
-         urlTotalSuffix[0] = '\0';  
- if (urlPreSuffix[0] != '\0')  
-         {  
-             strcat(urlTotalSuffix, urlPreSuffix);  
-             strcat(urlTotalSuffix, "/");  
-         }  
-         strcat(urlTotalSuffix, urlSuffix);  //格式化请求流信息
- if (!authenticationOK("DESCRIBE", urlTotalSuffix, fullRequestStr)) break;  //用户鉴权
- // We should really check that the request contains an "Accept:" #####
- // for "application/sdp", because that's what we're sending back #####  //可惜现在代码没校验吧
- // Begin by looking up the "ServerMediaSession" object for the specified "urlTotalSuffix":
-         ServerMediaSession* session = fOurServer.lookupServerMediaSession(urlTotalSuffix);  
- //根据请求中指定的流查找ServerMediaSession会话。在目前分析的是testOnDemandRTPServer中使用的是RTSPServer::lookupServerMediaSession, 只从已创建回话中查找。而DynamicRTSPServer的重载了此函数，首先调用父类RTPServer的lookupServerMediaSession，然后检查存在与否，如果不存在则创建一个新的。
- if (session == NULL)  
-         {  
-             handleCmd_notFound();  
- break;  
-         }  
- // Then, assemble a SDP description for this session:
-         sdpDescription = session->generateSDPDescription();//获取SDP描述信息，describe命令主要内容
- if (sdpDescription == NULL)  
-         {  
- // This usually means that a file name that was specified for a
- // "ServerMediaSubsession" does not exist.
-             setRTSPResponse("404 File Not Found, Or In Incorrect Format");  
- break;  
-         }  
-         unsigned sdpDescriptionSize = strlen(sdpDescription);  
- // Also, generate our RTSP URL, for the "Content-Base:" header
- // (which is necessary to ensure that the correct URL gets used in subsequent "SETUP" requests).
-         rtspURL = fOurServer.rtspURL(session, fClientInputSocket); 生成Content-Base内容:RTSP URL  
-         snprintf((char*)fResponseBuffer, sizeof fResponseBuffer,  
- "RTSP/1.0 200 OK\r\nCSeq: %s\r\n"
- "%s"
- "Content-Base: %s/\r\n"
- "Content-Type: application/sdp\r\n"
- "Content-Length: %d\r\n\r\n"
- "%s",  
-             fCurrentCSeq,  
-             dateHeader(),  
-             rtspURL,  
-             sdpDescriptionSize,  
-             sdpDescription);                             //拼接DESCRIBE的response
-     } while (0);  
- delete[] sdpDescription;  
- delete[] rtspURL;  
- }  
在对describe命令的处理过程中,主要内容一是鉴权,这个在上一篇Live555源码分析[2]:RTSPServer中的用户认证中讲过.另一个就是返回请求流的sdp信息. 这个通过generateSDPDescription来实现
在ServerMediaSession::generateSDPDescription()中基本都是固定字符串的拼接，需要我们重点关注的是
**[cpp]**[view
 plain](http://blog.csdn.net/njzhujinhua/article/details/30087927#)[copy](http://blog.csdn.net/njzhujinhua/article/details/30087927#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/387721/fork)
- // Count the lengths of each subsession's media-level SDP lines.
- // (We do this first, because the call to "subsession->sdpLines()"
- // causes correct subsession 'duration()'s to be calculated later.)
- /生成sdpLines并计算其长度，拼接过程在最后，但在此做的目的是顺便调用subsession的duration接口以便返回媒体文件的长度  
-    unsigned sdpLength = 0;  
-    ServerMediaSubsession* subsession;  
- for (subsession = fSubsessionsHead; subsession != NULL;  
-  subsession = subsession->fNext) {  
- charconst* sdpLines = subsession->sdpLines();   //此时返回的虽然没有使用，但内部是保存好了，后面拼接时虽然再次调用，但也是直接返回而已。事实上，live555默认并没有删除用完的SMS及SMSS，因而在第二次再次请求此媒体流时，这个也不会再次生成了
- if (sdpLines == NULL) continue; // the media's not available
-      sdpLength += strlen(sdpLines);  
-    }  
- if (sdpLength == 0) break; // the session has no usable subsessions
- 
- 
- // Unless subsessions have differing durations, we also have a "a=range:" line:
- float dur = duration();   //返回媒体时长
OnDemandServerMediaSubsession::sdpLines()的实现如下
**[cpp]**[view
 plain](http://blog.csdn.net/njzhujinhua/article/details/30087927#)[copy](http://blog.csdn.net/njzhujinhua/article/details/30087927#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/387721/fork)
- charconst* OnDemandServerMediaSubsession::sdpLines()   
- {  
- if (fSDPLines == NULL) {  
- // We need to construct a set of SDP lines that describe this subsession (as a unicast stream). To do so, we first create
- // dummy (unused) source and "RTPSink" objects, whose parameters we use for the SDP lines:
-         unsigned estBitrate;  
-         FramedSource* inputSource = createNewStreamSource(0, estBitrate);  
- if (inputSource == NULL) return NULL; // file not found
- 
- 
- struct in_addr dummyAddr;  
-         dummyAddr.s_addr = 0;  
-         Groupsock dummyGroupsock(envir(), dummyAddr, 0, 0);  
-         unsigned char rtpPayloadType = 96 + trackNumber()-1; // if dynamic
-         RTPSink* dummyRTPSink  
-             = createNewRTPSink(&dummyGroupsock, rtpPayloadType, inputSource);  
- if (dummyRTPSink != NULL && dummyRTPSink->estimatedBitrate() > 0) estBitrate = dummyRTPSink->estimatedBitrate();  
- 
- 
-         setSDPLinesFromRTPSink(dummyRTPSink, inputSource, estBitrate);  
-         Medium::close(dummyRTPSink);  
-         closeStreamSource(inputSource);  
-     }  
- 
- 
- return fSDPLines;  
- }  
其中createNewStreamSource与createNewRTPSink均为虚函数，由具体ServerMediaSubsession负责实现。
其中FramedSource的子类FramedFilter提供一种抽象数据源的功能，其继承关系MediaSource->FramedSource->FramedFilter, 由其屏蔽不同类型数据源的区别。RTPSource则是负责RTP协议报文封装之类的处理了
创建完临时的Source和Sink之后， 从Sink中获取SDPLine， 其中有的媒体类型读取文件头即可， 有的需要先播放一段视频，如H.264。。。以此才能正确的获取到SDP信息...具体细节还没研究，
