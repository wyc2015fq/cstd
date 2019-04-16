# live555学习笔记－RTSPClient分析 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月20日 17:27:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：56
个人分类：[Live555](https://blog.csdn.net/leixiaohua1020/article/category/1362936)










八 RTSPClient分析

有RTSPServer，当然就要有RTSPClient。
 如果按照Server端的架构，想一下Client端各部分的组成可能是这样：
 因为要连接RTSP server，所以RTSPClient要有TCP socket。当获取到server端的DESCRIBE后，应建立一个对应于ServerMediaSession的ClientMediaSession。对应每个Track，ClientMediaSession中应建立ClientMediaSubsession。当建立RTP Session时，应分别为所拥有的Track发送SETUP请求连接，在获取回应后，分别为所有的track建立RTP socket，然后请求PLAY，然后开始传输数据。事实是这样吗？只能分析代码了。


 testProgs中的OpenRTSP是典型的RTSPClient示例，所以分析它吧。
 main()函数在playCommon.cpp文件中。main()的流程比较简单，跟服务端差别不大：建立任务计划对象－－建立环境对象－－处理用户输入的参数(RTSP地址)－－创建RTSPClient实例－－发出第一个RTSP请求（可能是OPTIONS也可能是DESCRIBE）－－进入Loop。


 RTSP的tcp连接是在发送第一个RTSP请求时才建立的，在RTSPClient的那几个发请求的函数sendXXXXXXCommand()中最终都调用sendRequest()，sendRequest()中会跟据情况建立起TCP连接。在建立连接时马上向任务计划中加入处理从这个TCP接收数据的socket handler：RTSPClient::incomingDataHandler()。
 下面就是发送RTSP请求，OPTIONS就不必看了，从请求DESCRIBE开始：





**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6927461)[copy](http://blog.csdn.net/niu_gao/article/details/6927461)



- voidgetSDPDescription(RTSPClient::responseHandler*afterFunc)
- {
- ourRTSPClient->sendDescribeCommand(afterFunc,ourAuthenticator);
- }
- unsignedRTSPClient::sendDescribeCommand(responseHandler*responseHandler,
- Authenticator*authenticator)
- {
- if(authenticator!=NULL)
- fCurrentAuthenticator=*authenticator;
- returnsendRequest(newRequestRecord(++fCSeq,"DESCRIBE",responseHandler));
- }


参数responseHandler是调用者提供的回调函数，用于在处理完请求的回应后再调用之。并且在这个回调函数中会发出下一个请求－－所有的请求都是这样依次发出的。使用回调函数的原因主要是因为socket的发送与接收不是同步进行的。类RequestRecord就代表一个请求,它不但保存了RTSP请求相关的信息，而且保存了请求完成后的回调函数－－就是responseHandler。有些请求发出时还没建立tcp连接，不能立即发送，则加入fRequestsAwaitingConnection队列；有些发出后要等待Server端的回应，就加入fRequestsAwaitingResponse队列，当收到回应后再从队列中把它取出。
由于RTSPClient::sendRequest()太复杂，就不列其代码了，其无非是建立起RTSP请求字符串然后用TCP socket发送之。




 现在看一下收到DESCRIBE的回应后如何处理它。理论上是跟据媒体信息建立起MediaSession了，看看是不是这样：





**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6927461)[copy](http://blog.csdn.net/niu_gao/article/details/6927461)



- voidcontinueAfterDESCRIBE(RTSPClient*,intresultCode,char*resultString)
- {
- char*sdpDescription=resultString;
- //跟据SDP创建MediaSession。
- //CreateamediasessionobjectfromthisSDPdescription:
- session=MediaSession::createNew(*env,sdpDescription);
- delete[]sdpDescription;
- 
- //Then,setupthe"RTPSource"sforthesession:
- MediaSubsessionIteratoriter(*session);
- MediaSubsession*subsession;
- BooleanmadeProgress=False;
- charconst*singleMediumToTest=singleMedium;
- //循环所有的MediaSubsession，为每个设置其RTPSource的参数
- while((subsession=iter.next())!=NULL){
- //初始化subsession，在其中会建立RTP/RTCPsocket以及RTPSource。
- if(subsession->initiate(simpleRTPoffsetArg)){
- madeProgress=True;
- if(subsession->rtpSource()!=NULL){
- //Becausewe'resavingtheincomingdata,ratherthanplaying
- //itinrealtime,allowanespeciallylargetimethreshold
- //(1second)forreorderingmisorderedincomingpackets:
- unsignedconstthresh=1000000;//1second
- subsession->rtpSource()->setPacketReorderingThresholdTime(thresh);
- 
- //SettheRTPsource'sOSsocketbuffersizeasappropriate-eitherifwewereexplicitlyasked(using-B),
- //orifthedesiredFileSinkbuffersizehappenstobelargerthanthecurrentOSsocketbuffersize.
- //(Thelattercaseisaheuristic,ontheassumptionthatiftheuseraskedforalargeFileSinkbuffersize,
- //thentheinputdataratemaybelargeenoughtojustifyincreasingtheOSsocketbuffersizealso.)
- intsocketNum=subsession->rtpSource()->RTPgs()->socketNum();
- unsignedcurBufferSize=getReceiveBufferSize(*env,socketNum);
- if(socketInputBufferSize>0　||fileSinkBufferSize>curBufferSize){
- unsignednewBufferSize=　socketInputBufferSize>0?　
- socketInputBufferSize:fileSinkBufferSize;
- newBufferSize=setReceiveBufferTo(*env,socketNum,newBufferSize);
- if(socketInputBufferSize>0){//Theuserexplicitlyaskedforthenewsocketbuffersize;announceit:
- *env
- <<"Changedsocketreceivebuffersizeforthe\""
- <<subsession->mediumName()<<"/"
- <<subsession->codecName()
- <<"\"subsessionfrom"<<curBufferSize
- <<"to"<<newBufferSize<<"bytes\n";
- }
- }
- }
- }
- }
- if(!madeProgress)
- shutdown();
- 
- //Performadditional'setup'oneachsubsession,beforeplayingthem:
- //下一步就是发送SETUP请求了。需要为每个Track分别发送一次。
- setupStreams();
- }


此函数被删掉很多枝叶，所以发现与原版不同请不要惊掉大牙。
的确在DESCRIBE回应后建立起了MediaSession，而且我们发现Client端的MediaSession不叫ClientMediaSesson，SubSession亦不是。我现在很想看看MediaSession与MediaSubsession的建立过程:







**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6927461)[copy](http://blog.csdn.net/niu_gao/article/details/6927461)



- MediaSession*MediaSession::createNew(UsageEnvironment&env,charconst*sdpDescription)
- {
- MediaSession*newSession=newMediaSession(env);
- if(newSession!=NULL){
- if(!newSession->initializeWithSDP(sdpDescription)){
- deletenewSession;
- returnNULL;
- }
- }
- 
- returnnewSession;
- }



我可以告诉你，MediaSession的构造函数没什么可看的，那么就来看initializeWithSDP()：
内容太多，不必看了，我大体说说吧：就是处理SDP，跟据每一行来初始化一些变量。当遇到"m="行时，就建立一个MediaSubsession，然后再处理这一行之下，下一个"m="行之上的行们，用这些参数初始化MediaSubsession的变量。循环往复，直到尽头。然而这其中并没有建立RTP socket。我们发现在continueAfterDESCRIBE()中，创建MediaSession之后又调用了subsession->initiate(simpleRTPoffsetArg)，那么socket是不是在它里面创建的呢？look:







**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6927461)[copy](http://blog.csdn.net/niu_gao/article/details/6927461)



- BooleanMediaSubsession::initiate(intuseSpecialRTPoffset)
- {
- if(fReadSource!=NULL)
- returnTrue;//hasalreadybeeninitiated
- 
- do{
- if(fCodecName==NULL){
- env().setResultMsg("Codecisunspecified");
- break;
- }
- 
- //创建RTP/RTCPsockets
- //CreateRTPandRTCP'Groupsocks'onwhichtoreceiveincomingdata.
- //(Groupsockswillworkevenforunicastaddresses)
- structin_addrtempAddr;
- tempAddr.s_addr=connectionEndpointAddress();
- //Thiscouldgetchangedlater,asaresultofaRTSP"SETUP"
- 
- if(fClientPortNum!=0){
- //当server端指定了建议的client端口
- //Thesockets'portnumberswerespecifiedforus.Usethese:
- fClientPortNum=fClientPortNum&~1;//even
- if(isSSM()){
- fRTPSocket=newGroupsock(env(),tempAddr,fSourceFilterAddr,
- fClientPortNum);
- }else{
- fRTPSocket=newGroupsock(env(),tempAddr,fClientPortNum,
- 255);
- }
- if(fRTPSocket==NULL){
- env().setResultMsg("FailedtocreateRTPsocket");
- break;
- }
- 
- //SetourRTCPporttobetheRTPport+1
- portNumBitsconstrtcpPortNum=fClientPortNum|1;
- if(isSSM()){
- fRTCPSocket=newGroupsock(env(),tempAddr,fSourceFilterAddr,
- rtcpPortNum);
- }else{
- fRTCPSocket=newGroupsock(env(),tempAddr,rtcpPortNum,255);
- }
- if(fRTCPSocket==NULL){
- chartmpBuf[100];
- sprintf(tmpBuf,"FailedtocreateRTCPsocket(port%d)",
- rtcpPortNum);
- env().setResultMsg(tmpBuf);
- break;
- }
- }else{
- //Server端没有指定client端口，我们自己找一个。之所以做的这样复杂，是为了能找到连续的两个端口
- //RTP/RTCP的端口号不是要连续吗？还记得不？
- //Portnumberswerenotspecifiedinadvance,soweuseephemeralportnumbers.
- //Createsocketsuntilwegetaport-numberpair(even:RTP;even+1:RTCP).
- //Weneedtomakesurethatwedon'tkeeptryingtousethesamebadportnumbersoverandoveragain.
- //sowestorebadsocketsinatable,anddeletethemallwhenwe'redone.
- HashTable*socketHashTable=HashTable::create(ONE_WORD_HASH_KEYS);
- if(socketHashTable==NULL)
- break;
- Booleansuccess=False;
- NoReusedummy;//ensuresthatournewephemeralportnumberwon'tbeonethat'salreadyinuse
- 
- while(1){
- //Createanewsocket:
- if(isSSM()){
- fRTPSocket=newGroupsock(env(),tempAddr,
- fSourceFilterAddr,0);
- }else{
- fRTPSocket=newGroupsock(env(),tempAddr,0,255);
- }
- if(fRTPSocket==NULL){
- env().setResultMsg(
- "MediaSession::initiate():unabletocreateRTPandRTCPsockets");
- break;
- }
- 
- //Gettheclientportnumber,andcheckwhetherit'seven(forRTP):
- PortclientPort(0);
- if(!getSourcePort(env(),fRTPSocket->socketNum(),
- clientPort)){
- break;
- }
- fClientPortNum=ntohs(clientPort.num());
- if((fClientPortNum&1)!=0){//it'sodd
- //Recordthissocketinourtable,andkeeptrying:
- unsignedkey=(unsigned)fClientPortNum;
- Groupsock*existing=(Groupsock*)socketHashTable->Add(
- (charconst*)key,fRTPSocket);
- deleteexisting;//incaseitwasn'tNULL
- continue;
- }
- 
- //Makesurewecanusethenext(i.e.,odd)portnumber,forRTCP:
- portNumBitsrtcpPortNum=fClientPortNum|1;
- if(isSSM()){
- fRTCPSocket=newGroupsock(env(),tempAddr,
- fSourceFilterAddr,rtcpPortNum);
- }else{
- fRTCPSocket=newGroupsock(env(),tempAddr,rtcpPortNum,
- 255);
- }
- if(fRTCPSocket!=NULL&&fRTCPSocket->socketNum()>=0){
- //Success!Usethesetwosockets.
- success=True;
- break;
- }else{
- //Wecouldn'tcreatetheRTCPsocket(perhapsthatportnumber'salreadyinuseelsewhere?).
- deletefRTCPSocket;
- 
- //Recordthefirstsocketinourtable,andkeeptrying:
- unsignedkey=(unsigned)fClientPortNum;
- Groupsock*existing=(Groupsock*)socketHashTable->Add(
- (charconst*)key,fRTPSocket);
- deleteexisting;//incaseitwasn'tNULL
- continue;
- }
- }
- 
- //Cleanupthesockethashtable(andcontents):
- Groupsock*oldGS;
- while((oldGS=(Groupsock*)socketHashTable->RemoveNext())!=NULL){
- deleteoldGS;
- }
- deletesocketHashTable;
- 
- if(!success)
- break;//afatalerroroccurredtryingtocreatetheRTPandRTCPsockets;wecan'tcontinue
- }
- 
- //TrytouseabigreceivebufferforRTP-atleast0.1secondof
- //specifiedbandwidthandatleast50KB
- unsignedrtpBufSize=fBandwidth*25/2;//1kbps*0.1s=12.5bytes
- if(rtpBufSize<50*1024)
- rtpBufSize=50*1024;
- increaseReceiveBufferTo(env(),fRTPSocket->socketNum(),rtpBufSize);
- 
- //ASSERT:fRTPSocket!=NULL&&fRTCPSocket!=NULL
- if(isSSM()){
- //SpecialcaseforRTCPSSM:SendRTCPpacketsbacktothesourceviaunicast:
- fRTCPSocket->changeDestinationParameters(fSourceFilterAddr,0,~0);
- }
- 
- //创建RTPSource的地方
- //Create"fRTPSource"and"fReadSource":
- if(!createSourceObjects(useSpecialRTPoffset))
- break;
- 
- if(fReadSource==NULL){
- env().setResultMsg("Failedtocreatereadsource");
- break;
- }
- 
- //Finally,createourRTCPinstance.(Itstartsrunningautomatically)
- if(fRTPSource!=NULL){
- //Ifbandwidthisspecified,useitandadd5%forRTCPoverhead.
- //Otherwisemakeaguessat500kbps.
- unsignedtotSessionBandwidth=
- fBandwidth?fBandwidth+fBandwidth/20:500;
- fRTCPInstance=RTCPInstance::createNew(env(),fRTCPSocket,
- totSessionBandwidth,(unsignedcharconst*)fParent.CNAME(),
- NULL/*we'reaclient*/,fRTPSource);
- if(fRTCPInstance==NULL){
- env().setResultMsg("FailedtocreateRTCPinstance");
- break;
- }
- }
- 
- returnTrue;
- }while(0);
- 
- //失败时执行到这里
- deletefRTPSocket;
- fRTPSocket=NULL;
- deletefRTCPSocket;
- fRTCPSocket=NULL;
- Medium::close(fRTCPInstance);
- fRTCPInstance=NULL;
- Medium::close(fReadSource);
- fReadSource=fRTPSource=NULL;
- fClientPortNum=0;
- returnFalse;
- }


是的，在其中创建了RTP/RTCP socket并创建了RTPSource，创建RTPSource在函数createSourceObjects()中，看一下：







**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6927461)[copy](http://blog.csdn.net/niu_gao/article/details/6927461)



- BooleanMediaSubsession::createSourceObjects(intuseSpecialRTPoffset)
- {
- do{
- //First,check"fProtocolName"
- if(strcmp(fProtocolName,"UDP")==0){
- //AUDP-packetizedstream(*not*aRTPstream)
- fReadSource=BasicUDPSource::createNew(env(),fRTPSocket);
- fRTPSource=NULL;//Note!
- 
- if(strcmp(fCodecName,"MP2T")==0){//MPEG-2TransportStream
- fReadSource=MPEG2TransportStreamFramer::createNew(env(),
- fReadSource);
- //thissets"durationInMicroseconds"correctly,basedonthePCRvalues
- }
- }else{
- //Check"fCodecName"againstthesetofcodecsthatwesupport,
- //andcreateourRTPsourceaccordingly
- //(Latermakethiscodemoreefficient,asthissetgrows#####)
- //(Also,addmorefmtsthatcanbeimplementedbySimpleRTPSource#####)
- BooleancreateSimpleRTPSource=False;//bydefault;canbechangedbelow
- BooleandoNormalMBitRule=False;//defaultbehaviorif"createSimpleRTPSource"isTrue
- if(strcmp(fCodecName,"QCELP")==0){//QCELPaudio
- fReadSource=QCELPAudioRTPSource::createNew(env(),fRTPSocket,
- fRTPSource,fRTPPayloadFormat,fRTPTimestampFrequency);
- //NotethatfReadSourcewilldifferfromfRTPSourceinthiscase
- }elseif(strcmp(fCodecName,"AMR")==0){//AMRaudio(narrowband)
- fReadSource=AMRAudioRTPSource::createNew(env(),fRTPSocket,
- fRTPSource,fRTPPayloadFormat,0/*isWideband*/,
- fNumChannels,fOctetalign,fInterleaving,
- fRobustsorting,fCRC);
- //NotethatfReadSourcewilldifferfromfRTPSourceinthiscase
- }elseif(strcmp(fCodecName,"AMR-WB")==0){//AMRaudio(wideband)
- fReadSource=AMRAudioRTPSource::createNew(env(),fRTPSocket,
- fRTPSource,fRTPPayloadFormat,1/*isWideband*/,
- fNumChannels,fOctetalign,fInterleaving,
- fRobustsorting,fCRC);
- //NotethatfReadSourcewilldifferfromfRTPSourceinthiscase
- }elseif(strcmp(fCodecName,"MPA")==0){//MPEG-1or2audio
- fReadSource=fRTPSource=MPEG1or2AudioRTPSource::createNew(
- env(),fRTPSocket,fRTPPayloadFormat,
- fRTPTimestampFrequency);
- }elseif(strcmp(fCodecName,"MPA-ROBUST")==0){//robustMP3audio
- fRTPSource=MP3ADURTPSource::createNew(env(),fRTPSocket,
- fRTPPayloadFormat,fRTPTimestampFrequency);
- if(fRTPSource==NULL)
- break;
- 
- //AddafilterthatdeinterleavestheADUsafterdepacketizingthem:
- MP3ADUdeinterleaver*deinterleaver=MP3ADUdeinterleaver::createNew(
- env(),fRTPSource);
- if(deinterleaver==NULL)
- break;
- 
- //AddanotherfilterthatconvertstheseADUstoMP3frames:
- fReadSource=MP3FromADUSource::createNew(env(),deinterleaver);
- }elseif(strcmp(fCodecName,"X-MP3-DRAFT-00")==0){
- //anon-standardvariantof"MPA-ROBUST"usedbyRealNetworks
- //(one'ADU'izedMP3frameperpacket;noheaders)
- fRTPSource=SimpleRTPSource::createNew(env(),fRTPSocket,
- fRTPPayloadFormat,fRTPTimestampFrequency,
- "audio/MPA-ROBUST"/*hack*/);
- if(fRTPSource==NULL)
- break;
- 
- //AddafilterthatconvertstheseADUstoMP3frames:
- fReadSource=MP3FromADUSource::createNew(env(),fRTPSource,
- False/*noADUheader*/);
- }elseif(strcmp(fCodecName,"MP4A-LATM")==0){//MPEG-4LATMaudio
- fReadSource=fRTPSource=MPEG4LATMAudioRTPSource::createNew(
- env(),fRTPSocket,fRTPPayloadFormat,
- fRTPTimestampFrequency);
- }elseif(strcmp(fCodecName,"AC3")==0
- ||strcmp(fCodecName,"EAC3")==0){//AC3audio
- fReadSource=fRTPSource=AC3AudioRTPSource::createNew(env(),
- fRTPSocket,fRTPPayloadFormat,fRTPTimestampFrequency);
- }elseif(strcmp(fCodecName,"MP4V-ES")==0){//MPEG-4ElemStrvid
- fReadSource=fRTPSource=MPEG4ESVideoRTPSource::createNew(
- env(),fRTPSocket,fRTPPayloadFormat,
- fRTPTimestampFrequency);
- }elseif(strcmp(fCodecName,"MPEG4-GENERIC")==0){
- fReadSource=fRTPSource=MPEG4GenericRTPSource::createNew(
- env(),fRTPSocket,fRTPPayloadFormat,
- fRTPTimestampFrequency,fMediumName,fMode,fSizelength,
- fIndexlength,fIndexdeltalength);
- }elseif(strcmp(fCodecName,"MPV")==0){//MPEG-1or2video
- fReadSource=fRTPSource=MPEG1or2VideoRTPSource::createNew(
- env(),fRTPSocket,fRTPPayloadFormat,
- fRTPTimestampFrequency);
- }elseif(strcmp(fCodecName,"MP2T")==0){//MPEG-2TransportStream
- fRTPSource=SimpleRTPSource::createNew(env(),fRTPSocket,
- fRTPPayloadFormat,fRTPTimestampFrequency,"video/MP2T",
- 0,False);
- fReadSource=MPEG2TransportStreamFramer::createNew(env(),
- fRTPSource);
- //thissets"durationInMicroseconds"correctly,basedonthePCRvalues
- }elseif(strcmp(fCodecName,"H261")==0){//H.261
- fReadSource=fRTPSource=H261VideoRTPSource::createNew(env(),
- fRTPSocket,fRTPPayloadFormat,fRTPTimestampFrequency);
- }elseif(strcmp(fCodecName,"H263-1998")==0
- ||strcmp(fCodecName,"H263-2000")==0){//H.263+
- fReadSource=fRTPSource=H263plusVideoRTPSource::createNew(
- env(),fRTPSocket,fRTPPayloadFormat,
- fRTPTimestampFrequency);
- }elseif(strcmp(fCodecName,"H264")==0){
- fReadSource=fRTPSource=H264VideoRTPSource::createNew(env(),
- fRTPSocket,fRTPPayloadFormat,fRTPTimestampFrequency);
- }elseif(strcmp(fCodecName,"DV")==0){
- fReadSource=fRTPSource=DVVideoRTPSource::createNew(env(),
- fRTPSocket,fRTPPayloadFormat,fRTPTimestampFrequency);
- }elseif(strcmp(fCodecName,"JPEG")==0){//motionJPEG
- fReadSource=fRTPSource=JPEGVideoRTPSource::createNew(env(),
- fRTPSocket,fRTPPayloadFormat,fRTPTimestampFrequency,
- videoWidth(),videoHeight());
- }elseif(strcmp(fCodecName,"X-QT")==0
- ||strcmp(fCodecName,"X-QUICKTIME")==0){
- //GenericQuickTimestreams,asdefinedin
- //<http://developer.apple.com/quicktime/icefloe/dispatch026.html>
- char*mimeType=newchar[strlen(mediumName())
- +strlen(codecName())+2];
- sprintf(mimeType,"%s/%s",mediumName(),codecName());
- fReadSource=fRTPSource=QuickTimeGenericRTPSource::createNew(
- env(),fRTPSocket,fRTPPayloadFormat,
- fRTPTimestampFrequency,mimeType);
- delete[]mimeType;
- }elseif(strcmp(fCodecName,"PCMU")==0//PCMu-lawaudio
- ||strcmp(fCodecName,"GSM")==0//GSMaudio
- ||strcmp(fCodecName,"DVI4")==0//DVI4(IMAADPCM)audio
- ||strcmp(fCodecName,"PCMA")==0//PCMa-lawaudio
- ||strcmp(fCodecName,"MP1S")==0//MPEG-1SystemStream
- ||strcmp(fCodecName,"MP2P")==0//MPEG-2ProgramStream
- ||strcmp(fCodecName,"L8")==0//8-bitlinearaudio
- ||strcmp(fCodecName,"L16")==0//16-bitlinearaudio
- ||strcmp(fCodecName,"L20")==0//20-bitlinearaudio(RFC3190)
- ||strcmp(fCodecName,"L24")==0//24-bitlinearaudio(RFC3190)
- ||strcmp(fCodecName,"G726-16")==0//G.726,16kbps
- ||strcmp(fCodecName,"G726-24")==0//G.726,24kbps
- ||strcmp(fCodecName,"G726-32")==0//G.726,32kbps
- ||strcmp(fCodecName,"G726-40")==0//G.726,40kbps
- ||strcmp(fCodecName,"SPEEX")==0//SPEEXaudio
- ||strcmp(fCodecName,"T140")==0//T.140text(RFC4103)
- ||strcmp(fCodecName,"DAT12")==0//12-bitnonlinearaudio(RFC3190)
- ){
- createSimpleRTPSource=True;
- useSpecialRTPoffset=0;
- }elseif(useSpecialRTPoffset>=0){
- //Wedon'tknowthisRTPpayloadformat,buttrytoreceive
- //itusinga'SimpleRTPSource'withthespecifiedheaderoffset:
- createSimpleRTPSource=True;
- }else{
- env().setResultMsg(
- "RTPpayloadformatunknownornotsupported");
- break;
- }
- 
- if(createSimpleRTPSource){
- char*mimeType=newchar[strlen(mediumName())
- +strlen(codecName())+2];
- sprintf(mimeType,"%s/%s",mediumName(),codecName());
- fReadSource=fRTPSource=SimpleRTPSource::createNew(env(),
- fRTPSocket,fRTPPayloadFormat,fRTPTimestampFrequency,
- mimeType,(unsigned)useSpecialRTPoffset,
- doNormalMBitRule);
- delete[]mimeType;
- }
- }
- 
- returnTrue;
- }while(0);
- 
- returnFalse;//anerroroccurred
- }


可以看到，这个函数里主要是跟据前面分析出的媒体和传输信息建立合适的Source。



socket建立了，Source也创建了，下一步应该是连接Sink，形成一个流。到此为止还未看到Sink的影子，应该是在下一步SETUP中建立，我们看到在continueAfterDESCRIBE()的最后调用了setupStreams（），那么就来探索一下setupStreams():





**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6927461)[copy](http://blog.csdn.net/niu_gao/article/details/6927461)



- voidsetupStreams()
- {
- staticMediaSubsessionIterator*setupIter=NULL;
- if(setupIter==NULL)
- setupIter=newMediaSubsessionIterator(*session);
- 
- //每次调用此函数只为一个Subsession发出SETUP请求。
- while((subsession=setupIter->next())!=NULL){
- //Wehaveanothersubsessionlefttosetup:
- if(subsession->clientPortNum()==0)
- continue;//port#wasnotset
- 
- //为一个Subsession发送SETUP请求。请求处理完成时调用continueAfterSETUP()，
- //continueAfterSETUP()又调用了setupStreams()，在此函数中为下一个SubSession发送SETUP请求。




**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6927461)[copy](http://blog.csdn.net/niu_gao/article/details/6927461)



- <spanstyle="white-space:pre"></span>//直到处理完所有的SubSession
- setupSubsession(subsession,streamUsingTCP,continueAfterSETUP);
- return;
- }
- 
- //执行到这里时，已循环完所有的SubSession了
- //We'redonesettingupsubsessions.
- deletesetupIter;
- if(!madeProgress)
- shutdown();
- 
- //创建输出文件，看来是在这里创建Sink了。创建sink后，就开始播放它。这个播放应该只是把socket的handler加入到
- //计划任务中，而没有数据的接收或发送。只有等到发出PLAY请求后才有数据的收发。
- //Createoutputfiles:
- if(createReceivers){
- if(outputQuickTimeFile){
- //Createa"QuickTimeFileSink",towriteto'stdout':
- qtOut=QuickTimeFileSink::createNew(*env,*session,"stdout",
- fileSinkBufferSize,movieWidth,movieHeight,movieFPS,
- packetLossCompensate,syncStreams,generateHintTracks,
- generateMP4Format);
- if(qtOut==NULL){
- *env<<"FailedtocreateQuickTimefilesinkforstdout:"
- <<env->getResultMsg();
- shutdown();
- }
- 
- qtOut->startPlaying(sessionAfterPlaying,NULL);
- }elseif(outputAVIFile){
- //Createan"AVIFileSink",towriteto'stdout':
- aviOut=AVIFileSink::createNew(*env,*session,"stdout",
- fileSinkBufferSize,movieWidth,movieHeight,movieFPS,
- packetLossCompensate);
- if(aviOut==NULL){
- *env<<"FailedtocreateAVIfilesinkforstdout:"
- <<env->getResultMsg();
- shutdown();
- }
- 
- aviOut->startPlaying(sessionAfterPlaying,NULL);
- }else{
- //Createandstart"FileSink"sforeachsubsession:
- madeProgress=False;
- MediaSubsessionIteratoriter(*session);
- while((subsession=iter.next())!=NULL){
- if(subsession->readSource()==NULL)
- continue;//wasnotinitiated
- 
- //Createanoutputfileforeachdesiredstream:
- charoutFileName[1000];
- if(singleMedium==NULL){
- //Outputfilenameis
- //"<filename-prefix><medium_name>-<codec_name>-<counter>"
- staticunsignedstreamCounter=0;
- snprintf(outFileName,sizeofoutFileName,"%s%s-%s-%d",
- fileNamePrefix,subsession->mediumName(),
- subsession->codecName(),++streamCounter);
- }else{
- sprintf(outFileName,"stdout");
- }
- FileSink*fileSink;
- if(strcmp(subsession->mediumName(),"audio")==0
- &&(strcmp(subsession->codecName(),"AMR")==0
- ||strcmp(subsession->codecName(),"AMR-WB")
- ==0)){
- //ForAMRaudiostreams,weuseaspecialsinkthatinsertsAMRframehdrs:
- fileSink=AMRAudioFileSink::createNew(*env,outFileName,
- fileSinkBufferSize,oneFilePerFrame);
- }elseif(strcmp(subsession->mediumName(),"video")==0
- &&(strcmp(subsession->codecName(),"H264")==0)){
- //ForH.264videostream,weuseaspecialsinkthatinsertstart_codes:
- fileSink=H264VideoFileSink::createNew(*env,outFileName,
- subsession->fmtp_spropparametersets(),
- fileSinkBufferSize,oneFilePerFrame);
- }else{
- //Normalcase:
- fileSink=FileSink::createNew(*env,outFileName,
- fileSinkBufferSize,oneFilePerFrame);
- }
- subsession->sink=fileSink;
- if(subsession->sink==NULL){
- *env<<"FailedtocreateFileSinkfor\""<<outFileName
- <<"\":"<<env->getResultMsg()<<"\n";
- }else{
- if(singleMedium==NULL){
- *env<<"Createdoutputfile:\""<<outFileName
- <<"\"\n";
- }else{
- *env<<"Outputtingdatafromthe\""
- <<subsession->mediumName()<<"/"
- <<subsession->codecName()
- <<"\"subsessionto'stdout'\n";
- }
- 
- if(strcmp(subsession->mediumName(),"video")==0
- &&strcmp(subsession->codecName(),"MP4V-ES")==0&&
- subsession->fmtp_config()!=NULL){
- //ForMPEG-4videoRTPstreams,the'config'information
- //fromtheSDPdescriptioncontainsusefulVOLetc.headers.
- //Insertthisdataatthefrontoftheoutputfile:
- unsignedconfigLen;
- unsignedchar*configData
- =parseGeneralConfigStr(subsession->fmtp_config(),configLen);
- structtimevaltimeNow;
- gettimeofday(&timeNow,NULL);
- fileSink->addData(configData,configLen,timeNow);
- delete[]configData;
- }
- 
- //开始传输
- subsession->sink->startPlaying(*(subsession->readSource()),
- subsessionAfterPlaying,subsession);
- 
- //AlsosetahandlertobecalledifaRTCP"BYE"arrives
- //forthissubsession:
- if(subsession->rtcpInstance()!=NULL){
- subsession->rtcpInstance()->setByeHandler(
- subsessionByeHandler,subsession);
- }
- 
- madeProgress=True;
- }
- }
- if(!madeProgress)
- shutdown();
- }
- }
- 
- //Finally,startplayingeachsubsession,tostartthedataflow:
- if(duration==0){
- if(scale>0)
- duration=session->playEndTime()-initialSeekTime;//useSDPendtime
- elseif(scale<0)
- duration=initialSeekTime;
- }
- if(duration<0)
- duration=0.0;
- 
- endTime=initialSeekTime;
- if(scale>0){
- if(duration<=0)
- endTime=-1.0f;
- else
- endTime=initialSeekTime+duration;
- }else{
- endTime=initialSeekTime-duration;
- if(endTime<0)
- endTime=0.0f;
- }
- 
- //发送PLAY请求，之后才能从Server端接收数据
- startPlayingSession(session,initialSeekTime,endTime,scale,
- continueAfterPLAY);
- }


仔细看看注释，应很容易了解此函数。



原文地址：[http://blog.csdn.net/niu_gao/article/details/6927461](http://blog.csdn.net/niu_gao/article/details/6927461)

live555源代码（VC6）：[http://download.csdn.net/detail/leixiaohua1020/6374387](http://download.csdn.net/detail/leixiaohua1020/6374387)




