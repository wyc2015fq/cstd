# live555学习笔记－RTSP服务运作 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月20日 17:13:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：85









RTSP服务运作

基础基本搞明白了，那么RTSP,RTP等这些协议又是如何利用这些基础机制运作的呢？
首先来看RTSP.

RTSP首先需建立TCP侦听socket。可见于此函数：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6911130)[copy](http://blog.csdn.net/niu_gao/article/details/6911130)



- DynamicRTSPServer*DynamicRTSPServer::createNew(UsageEnvironment&env,PortourPort,
- UserAuthenticationDatabase*authDatabase,
- unsignedreclamationTestSeconds){
- intourSocket=setUpOurSocket(env,ourPort);//建立TCPsocket
- if(ourSocket==-1)
- returnNULL;
- 
- 
- returnnewDynamicRTSPServer(env,ourSocket,ourPort,authDatabase,
- reclamationTestSeconds);
- }



要帧听客户端的连接，就需要利用任务调度机制了，所以需添加一个socket handler。可见于此函数：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6911130)[copy](http://blog.csdn.net/niu_gao/article/details/6911130)



- RTSPServer::RTSPServer(UsageEnvironment&env,
- intourSocket,
- PortourPort,
- UserAuthenticationDatabase*authDatabase,
- unsignedreclamationTestSeconds):
- Medium(env),
- fRTSPServerSocket(ourSocket),
- fRTSPServerPort(ourPort),
- fHTTPServerSocket(-1),
- fHTTPServerPort(0),
- fClientSessionsForHTTPTunneling(NULL),
- fAuthDB(authDatabase),
- fReclamationTestSeconds(reclamationTestSeconds),
- fServerMediaSessions(HashTable::create(STRING_HASH_KEYS))
- {
- #ifdefUSE_SIGNALS
- //IgnoretheSIGPIPEsignal,sothatclientsonthesamehostthatarekilled
- //don'talsokillus:
- signal(SIGPIPE,SIG_IGN);
- #endif
- 
- 
- //Arrangetohandleconnectionsfromothers:
- env.taskScheduler().turnOnBackgroundReadHandling(
- fRTSPServerSocket,
- (TaskScheduler::BackgroundHandlerProc*)&incomingConnectionHandlerRTSP,
- this);
- }






当收到客户的连接时需保存下代表客户端的新socket，以后用这个socket与这个客户通讯。每个客户将来会对应一个rtp会话，而且各客户的RTSP请求只控制自己的rtp会话，那么最好建立一个会话类，代表各客户的rtsp会话。于是类RTSPServer::RTSPClientSession产生，它保存的代表客户的socket。下为RTSPClientSession的创建过程





**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6911130)[copy](http://blog.csdn.net/niu_gao/article/details/6911130)



- voidRTSPServer::incomingConnectionHandler(intserverSocket)
- {
- structsockaddr_inclientAddr;
- SOCKLEN_TclientAddrLen=sizeofclientAddr;
- 
- //接受连接
- intclientSocket=accept(serverSocket,
- (structsockaddr*)&clientAddr,
- &clientAddrLen);
- 
- if(clientSocket<0){
- interr=envir().getErrno();
- if(err!=EWOULDBLOCK){
- envir().setResultErrMsg("accept()failed:");
- }
- return;
- }
- 
- //设置socket的参数
- makeSocketNonBlocking(clientSocket);
- increaseSendBufferTo(envir(),clientSocket,50*1024);
- 
- #ifdefDEBUG
- envir()<<"accept()edconnectionfrom"<<our_inet_ntoa(clientAddr.sin_addr)<<"\n";
- #endif
- 
- //产生一个sessonid
- 
- //CreateanewobjectforthisRTSPsession.
- //(Choosearandom32-bitintegerforthesessionid(itwillbeencodedasa8-digithexnumber).Wedon'tbothercheckingfor
- //acollision;theprobabilityoftwoconcurrentsessionsgettingthesamesessionidisverylow.)
- //(Wedo,however,avoidchoosingsessionid0,becausethathasaspecialuse(by"OnDemandServerMediaSubsession").)
- unsignedsessionId;
- do{
- sessionId=(unsigned)our_random();
- }while(sessionId==0);
- 
- //创建RTSPClientSession，注意传入的参数
- (void)createNewClientSession(sessionId,clientSocket,clientAddr);
- }




RTSPClientSession要提供什么功能呢？可以想象：需要监听客户端的rtsp请求并回应它，需要在DESCRIBE请求中返回所请求的流的信息，需要在SETUP请求中建立起RTP会话，需要在TEARDOWN请求中关闭RTP会话，等等...

RTSPClientSession要侦听客户端的请求，就需把自己的socket handler加入计划任务。证据如下：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6911130)[copy](http://blog.csdn.net/niu_gao/article/details/6911130)



- RTSPServer::RTSPClientSession::RTSPClientSession(
- RTSPServer&ourServer,
- unsignedsessionId,
- intclientSocket,
- structsockaddr_inclientAddr):
- fOurServer(ourServer),
- fOurSessionId(sessionId),
- fOurServerMediaSession(NULL),
- fClientInputSocket(clientSocket),
- fClientOutputSocket(clientSocket),
- fClientAddr(clientAddr),
- fSessionCookie(NULL),
- fLivenessCheckTask(NULL),
- fIsMulticast(False),
- fSessionIsActive(True),
- fStreamAfterSETUP(False),
- fTCPStreamIdCount(0),
- fNumStreamStates(0),
- fStreamStates(NULL),
- fRecursionCount(0)
- {
- //Arrangetohandleincomingrequests:
- resetRequestBuffer();
- envir().taskScheduler().turnOnBackgroundReadHandling(fClientInputSocket,
- (TaskScheduler::BackgroundHandlerProc*)&incomingRequestHandler,
- this);
- noteLiveness();
- }




下面重点讲一下下RTSPClientSession响应DESCRIBE请求的过程：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6911130)[copy](http://blog.csdn.net/niu_gao/article/details/6911130)



- voidRTSPServer::RTSPClientSession::handleCmd_DESCRIBE(
- charconst*cseq,
- charconst*urlPreSuffix,
- charconst*urlSuffix,
- charconst*fullRequestStr)
- {
- char*sdpDescription=NULL;
- char*rtspURL=NULL;
- do{
- //整理一下下RTSP地址
- charurlTotalSuffix[RTSP_PARAM_STRING_MAX];
- if(strlen(urlPreSuffix)+strlen(urlSuffix)+2
- >sizeofurlTotalSuffix){
- handleCmd_bad(cseq);
- break;
- }
- urlTotalSuffix[0]='\0';
- if(urlPreSuffix[0]!='\0'){
- strcat(urlTotalSuffix,urlPreSuffix);
- strcat(urlTotalSuffix,"/");
- }
- strcat(urlTotalSuffix,urlSuffix);
- 
- 
- //验证帐户和密码
- if(!authenticationOK("DESCRIBE",cseq,urlTotalSuffix,fullRequestStr))
- break;
- 
- 
- //Weshouldreallycheckthattherequestcontainsan"Accept:"#####
- //for"application/sdp",becausethat'swhatwe'resendingback#####
- 
- 
- //Beginbylookingupthe"ServerMediaSession"objectforthespecified"urlTotalSuffix":
- //跟据流的名字查找ServerMediaSession，如果找不到，会创建一个。每个ServerMediaSession中至少要包含一个
- //ServerMediaSubsession。一个ServerMediaSession对应一个媒体，可以认为是Server上的一个文件，或一个实时获取设备。其包含的每个ServerMediaSubSession代表媒体中的一个Track。所以一个ServerMediaSession对应一个媒体，如果客户请求的媒体名相同，就使用已存在的ServerMediaSession，如果不同，就创建一个新的。一个流对应一个StreamState，StreamState与ServerMediaSubsession相关，但代表的是动态的，而ServerMediaSubsession代表静态的。
- ServerMediaSession*session=fOurServer.lookupServerMediaSession(urlTotalSuffix);
- if(session==NULL){
- handleCmd_notFound(cseq);
- break;
- }
- 
- 
- //Then,assembleaSDPdescriptionforthissession:
- //获取SDP字符串，在函数内会依次获取每个ServerMediaSubSession的字符串然连接起来。
- sdpDescription=session->generateSDPDescription();
- if(sdpDescription==NULL){
- //Thisusuallymeansthatafilenamethatwasspecifiedfora
- //"ServerMediaSubsession"doesnotexist.
- snprintf((char*)fResponseBuffer,sizeoffResponseBuffer,
- "RTSP/1.0404FileNotFound,OrInIncorrectFormat\r\n"
- "CSeq:%s\r\n"
- "%s\r\n",cseq,dateHeader());
- break;
- }
- unsignedsdpDescriptionSize=strlen(sdpDescription);
- 
- 
- //Also,generateourRTSPURL,forthe"Content-Base:"header
- //(whichisnecessarytoensurethatthecorrectURLgetsusedin
- //subsequent"SETUP"requests).
- rtspURL=fOurServer.rtspURL(session,fClientInputSocket);
- 
- 
- //形成响应DESCRIBE请求的RTSP字符串。
- snprintf((char*)fResponseBuffer,sizeoffResponseBuffer,
- "RTSP/1.0200OK\r\nCSeq:%s\r\n"
- "%s"
- "Content-Base:%s/\r\n"
- "Content-Type:application/sdp\r\n"
- "Content-Length:%d\r\n\r\n"
- "%s",cseq,dateHeader(),rtspURL,sdpDescriptionSize,
- sdpDescription);
- }while(0);
- 
- 
- delete[]sdpDescription;
- delete[]rtspURL;
- 
- 
- //返回后会被立即发送（没有把socketwrite操作放入计划任务中）。
- }





fOurServer.lookupServerMediaSession(urlTotalSuffix)中会在找不到同名ServerMediaSession时新建一个，代表一个RTP流的ServerMediaSession们是被RTSPServer管理的，而不是被RTSPClientSession拥有。为什么呢？因为ServerMediaSession代表的是一个静态的流，也就是可以从它里面获取一个流的各种信息，但不能获取传输状态。不同客户可能连接到同一个流，所以ServerMediaSession应被RTSPServer所拥有。创建一个ServerMediaSession过程值得一观：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6911130)[copy](http://blog.csdn.net/niu_gao/article/details/6911130)



- staticServerMediaSession*createNewSMS(UsageEnvironment&env,charconst*fileName,FILE*/*fid*/)
- {
- //Usethefilenameextensiontodeterminethetypeof"ServerMediaSession":
- charconst*extension=strrchr(fileName,'.');
- if(extension==NULL)
- returnNULL;
- 
- 
- ServerMediaSession*sms=NULL;
- BooleanconstreuseSource=False;
- if(strcmp(extension,".aac")==0){
- //AssumedtobeanAACAudio(ADTSformat)file:
- NEW_SMS("AACAudio");
- sms->addSubsession(
- ADTSAudioFileServerMediaSubsession::createNew(env,fileName,
- reuseSource));
- }elseif(strcmp(extension,".amr")==0){
- //AssumedtobeanAMRAudiofile:
- NEW_SMS("AMRAudio");
- sms->addSubsession(
- AMRAudioFileServerMediaSubsession::createNew(env,fileName,
- reuseSource));
- }elseif(strcmp(extension,".ac3")==0){
- //AssumedtobeanAC-3Audiofile:
- NEW_SMS("AC-3Audio");
- sms->addSubsession(
- AC3AudioFileServerMediaSubsession::createNew(env,fileName,
- reuseSource));
- }elseif(strcmp(extension,".m4e")==0){
- //AssumedtobeaMPEG-4VideoElementaryStreamfile:
- NEW_SMS("MPEG-4Video");
- sms->addSubsession(
- MPEG4VideoFileServerMediaSubsession::createNew(env,fileName,
- reuseSource));
- }elseif(strcmp(extension,".264")==0){
- //AssumedtobeaH.264VideoElementaryStreamfile:
- NEW_SMS("H.264Video");
- OutPacketBuffer::maxSize=100000;//allowforsomepossiblylargeH.264frames
- sms->addSubsession(
- H264VideoFileServerMediaSubsession::createNew(env,fileName,
- reuseSource));
- }elseif(strcmp(extension,".mp3")==0){
- //AssumedtobeaMPEG-1or2Audiofile:
- NEW_SMS("MPEG-1or2Audio");
- //Tostreamusing'ADUs'ratherthanrawMP3frames,uncommentthefollowing:
- //#defineSTREAM_USING_ADUS1
- //ToalsoreorderADUsbeforestreaming,uncommentthefollowing:
- //#defineINTERLEAVE_ADUS1
- //(FormoreinformationaboutADUsandinterleaving,
- //see<http://www.live555.com/rtp-mp3/>)
- BooleanuseADUs=False;
- Interleaving*interleaving=NULL;
- #ifdefSTREAM_USING_ADUS
- useADUs=True;
- #ifdefINTERLEAVE_ADUS
- unsignedcharinterleaveCycle[]={0,2,1,3};//orchooseyourown...
- unsignedconstinterleaveCycleSize
- =(sizeofinterleaveCycle)/(sizeof(unsignedchar));
- interleaving=newInterleaving(interleaveCycleSize,interleaveCycle);
- #endif
- #endif
- sms->addSubsession(
- MP3AudioFileServerMediaSubsession::createNew(env,fileName,
- reuseSource,useADUs,interleaving));
- }elseif(strcmp(extension,".mpg")==0){
- //AssumedtobeaMPEG-1or2ProgramStream(audio+video)file:
- NEW_SMS("MPEG-1or2ProgramStream");
- MPEG1or2FileServerDemux*demux=MPEG1or2FileServerDemux::createNew(env,
- fileName,reuseSource);
- sms->addSubsession(demux->newVideoServerMediaSubsession());
- sms->addSubsession(demux->newAudioServerMediaSubsession());
- }elseif(strcmp(extension,".ts")==0){
- //AssumedtobeaMPEGTransportStreamfile:
- //Useanindexfilenamethat'sthesameastheTSfilename,exceptwith".tsx":
- unsignedindexFileNameLen=strlen(fileName)+2;//allowfortrailing"x\0"
- char*indexFileName=newchar[indexFileNameLen];
- sprintf(indexFileName,"%sx",fileName);
- NEW_SMS("MPEGTransportStream");
- sms->addSubsession(
- MPEG2TransportFileServerMediaSubsession::createNew(env,
- fileName,indexFileName,reuseSource));
- delete[]indexFileName;
- }elseif(strcmp(extension,".wav")==0){
- //AssumedtobeaWAVAudiofile:
- NEW_SMS("WAVAudioStream");
- //Toconvert16-bitPCMdatato8-bitu-law,priortostreaming,
- //changethefollowingtoTrue:
- BooleanconvertToULaw=False;
- sms->addSubsession(
- WAVAudioFileServerMediaSubsession::createNew(env,fileName,
- reuseSource,convertToULaw));
- }elseif(strcmp(extension,".dv")==0){
- //AssumedtobeaDVVideofile
- //First,makesurethattheRTPSinks'bufferswillbelargeenoughtohandlethehugesizeofDVframes(asbigas288000).
- OutPacketBuffer::maxSize=300000;
- 
- 
- NEW_SMS("DVVideo");
- sms->addSubsession(
- DVVideoFileServerMediaSubsession::createNew(env,fileName,
- reuseSource));
- }elseif(strcmp(extension,".mkv")==0){
- //AssumedtobeaMatroskafile
- NEW_SMS("Matroskavideo+audio+(optional)subtitles");
- 
- 
- //CreateaMatroskafileserverdemultiplexorforthespecifiedfile.(Weentertheeventlooptowaitforthistocomplete.)
- newMatroskaDemuxWatchVariable=0;
- MatroskaFileServerDemux::createNew(env,fileName,
- onMatroskaDemuxCreation,NULL);
- env.taskScheduler().doEventLoop(&newMatroskaDemuxWatchVariable);
- 
- 
- ServerMediaSubsession*smss;
- while((smss=demux->newServerMediaSubsession())!=NULL){
- sms->addSubsession(smss);
- }
- }
- 
- 
- returnsms;
- }




可以看到NEW_SMS("AMR Audio")会创建新的ServerMediaSession，之后马上调用sms->addSubsession（）为这个ServerMediaSession添加一个 ServerMediaSubSession 。看起来ServerMediaSession应该可以添加多个ServerMediaSubSession，但这里并没有这样做。如果可以添加多个 ServerMediaSubsession 那么ServerMediaSession与流名字所指定与文件是没有关系的，也就是说它不会操作文件，而文件的操作是放在 ServerMediaSubsession中的。具体应改是在ServerMediaSubsession的sdpLines()函数中打开。



原文地址：[http://blog.csdn.net/niu_gao/article/details/6911130](http://blog.csdn.net/niu_gao/article/details/6911130)

live555源代码（VC6）：[http://download.csdn.net/detail/leixiaohua1020/6374387](http://download.csdn.net/detail/leixiaohua1020/6374387)





