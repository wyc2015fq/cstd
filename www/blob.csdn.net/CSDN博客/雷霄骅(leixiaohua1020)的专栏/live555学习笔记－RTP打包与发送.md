# live555学习笔记－RTP打包与发送 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月20日 17:16:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：70









RTP打包与发送

rtp传送开始于函数：MediaSink::startPlaying()。想想也有道理，应是sink跟source要数据，所以从sink上调用startplaying（嘿嘿，相当于directshow的拉模式）。

看一下这个函数：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6921145)[copy](http://blog.csdn.net/niu_gao/article/details/6921145)



- BooleanMediaSink::startPlaying(MediaSource&source,
- afterPlayingFunc*afterFunc,void*afterClientData)
- {
- //参数afterFunc是在播放结束时才被调用。
- //Makesurewe'renotalreadybeingplayed:
- if(fSource!=NULL){
- envir().setResultMsg("Thissinkisalreadybeingplayed");
- returnFalse;
- }
- 
- 
- //Makesureoursourceiscompatible:
- if(!sourceIsCompatibleWithUs(source)){
- envir().setResultMsg(
- "MediaSink::startPlaying():sourceisnotcompatible!");
- returnFalse;
- }
- //记下一些要使用的对象
- fSource=(FramedSource*)&source;
- 
- 
- fAfterFunc=afterFunc;
- fAfterClientData=afterClientData;
- returncontinuePlaying();
- }


为了进一步封装（让继承类少写一些代码），搞出了一个虚函数continuePlaying()。让我们来看一下：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6921145)[copy](http://blog.csdn.net/niu_gao/article/details/6921145)



- BooleanMultiFramedRTPSink::continuePlaying(){
- //Sendthefirstpacket.
- //(Thiswillalsoscheduleanyfuturesends.)
- buildAndSendPacket(True);
- returnTrue;
- }


MultiFramedRTPSink是与帧有关的类，其实它要求每次必须从source获得一个帧的数据，所以才叫这个name。可以看到continuePlaying()完全被buildAndSendPacket()代替。看一下buildAndSendPacket():



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6921145)[copy](http://blog.csdn.net/niu_gao/article/details/6921145)



- voidMultiFramedRTPSink::buildAndSendPacket(BooleanisFirstPacket)
- {
- //此函数中主要是准备rtp包的头，为一些需要跟据实际数据改变的字段留出位置。
- fIsFirstPacket=isFirstPacket;
- 
- 
- //SetuptheRTPheader:
- unsignedrtpHdr=0x80000000;//RTPversion2;marker('M')bitnotset(bydefault;itcanbesetlater)
- rtpHdr|=(fRTPPayloadType<<16);
- rtpHdr|=fSeqNo;//sequencenumber
- fOutBuf->enqueueWord(rtpHdr);//向包中加入一个字
- 
- 
- //NotewheretheRTPtimestampwillgo.
- //(Wecan'tfillthisinuntilwestartpackingpayloadframes.)
- fTimestampPosition=fOutBuf->curPacketSize();
- fOutBuf->skipBytes(4);//leaveaholeforthetimestamp　在缓冲中空出时间戳的位置
- 
- 
- fOutBuf->enqueueWord(SSRC());
- 
- 
- //Allowforaspecial,payload-format-specificheaderfollowingthe
- //RTPheader:
- fSpecialHeaderPosition=fOutBuf->curPacketSize();
- fSpecialHeaderSize=specialHeaderSize();
- fOutBuf->skipBytes(fSpecialHeaderSize);
- 
- 
- //Beginpackingasmany(complete)framesintothepacketaswecan:
- fTotalFrameSpecificHeaderSizes=0;
- fNoFramesLeft=False;
- fNumFramesUsedSoFar=0;//一个包中已打入的帧数。
- //头准备好了，再打包帧数据
- packFrame();
- }


继续看packFrame()：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6921145)[copy](http://blog.csdn.net/niu_gao/article/details/6921145)



- voidMultiFramedRTPSink::packFrame()
- {
- //First,seeifwehaveanoverflowframethatwastoobigforthelastpkt
- if(fOutBuf->haveOverflowData()){
- //如果有帧数据，则使用之。OverflowData是指上次打包时剩下的帧数据，因为一个包可能容纳不了一个帧。
- //Usethisframebeforereadinganewonefromthesource
- unsignedframeSize=fOutBuf->overflowDataSize();
- structtimevalpresentationTime=fOutBuf->overflowPresentationTime();
- unsigneddurationInMicroseconds=fOutBuf->overflowDurationInMicroseconds();
- fOutBuf->useOverflowData();
- 
- 
- afterGettingFrame1(frameSize,0,presentationTime,durationInMicroseconds);
- }else{
- //一点帧数据都没有，跟source要吧。
- //Normalcase:weneedtoreadanewframefromthesource
- if(fSource==NULL)
- return;
- 
- 
- //更新缓冲中的一些位置
- fCurFrameSpecificHeaderPosition=fOutBuf->curPacketSize();
- fCurFrameSpecificHeaderSize=frameSpecificHeaderSize();
- fOutBuf->skipBytes(fCurFrameSpecificHeaderSize);
- fTotalFrameSpecificHeaderSizes+=fCurFrameSpecificHeaderSize;
- 
- 
- //从source获取下一帧
- fSource->getNextFrame(fOutBuf->curPtr(),//新数据存放开始的位置
- fOutBuf->totalBytesAvailable(),//缓冲中空余的空间大小
- afterGettingFrame,//因为可能source中的读数据函数会被放在任务调度中，所以把获取帧后应调用的函数传给source
- this,
- ourHandleClosure,//这个是source结束时(比如文件读完了)要调用的函数。
- this);
- }
- }


可以想像下面就是source从文件（或某个设备）中读取一帧数据，读完后返回给sink，当然不是从函数返回了，而是以调用afterGettingFrame这个回调函数的方式。所以下面看一下afterGettingFrame():



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6921145)[copy](http://blog.csdn.net/niu_gao/article/details/6921145)



- voidMultiFramedRTPSink::afterGettingFrame(void*clientData,
- unsignednumBytesRead,unsignednumTruncatedBytes,
- structtimevalpresentationTime,unsigneddurationInMicroseconds)
- {
- MultiFramedRTPSink*sink=(MultiFramedRTPSink*)clientData;
- sink->afterGettingFrame1(numBytesRead,numTruncatedBytes,presentationTime,
- durationInMicroseconds);
- }


没什么可看的，只是过度为调用成员函数，所以afterGettingFrame1()才是重点：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6921145)[copy](http://blog.csdn.net/niu_gao/article/details/6921145)



- voidMultiFramedRTPSink::afterGettingFrame1(
- unsignedframeSize,
- unsignednumTruncatedBytes,
- structtimevalpresentationTime,
- unsigneddurationInMicroseconds)
- {
- if(fIsFirstPacket){
- //Recordthefactthatwe'restartingtoplaynow:
- gettimeofday(&fNextSendTime,NULL);
- }
- 
- 
- //如果给予一帧的缓冲不够大，就会发生截断一帧数据的现象。但也只能提示一下用户
- if(numTruncatedBytes>0){
- 
- 
- unsignedconstbufferSize=fOutBuf->totalBytesAvailable();
- envir()
- <<"MultiFramedRTPSink::afterGettingFrame1():Theinputframedatawastoolargeforourbuffersize("
- <<bufferSize
- <<")."
- <<numTruncatedBytes
- <<"bytesoftrailingdatawasdropped!Correctthisbyincreasing\"OutPacketBuffer::maxSize\"toatleast"
- <<OutPacketBuffer::maxSize+numTruncatedBytes
- <<",*before*creatingthis'RTPSink'.(Currentvalueis"
- <<OutPacketBuffer::maxSize<<".)\n";
- }
- unsignedcurFragmentationOffset=fCurFragmentationOffset;
- unsignednumFrameBytesToUse=frameSize;
- unsignedoverflowBytes=0;
- 
- 
- //如果包只已经打入帧数据了，并且不能再向这个包中加数据了，则把新获得的帧数据保存下来。
- //Ifwehavealreadypackedoneormoreframesintothispacket,
- //checkwhetherthisnewframeiseligibletobepackedafterthem.
- //(Thisisindependentofwhetherthepackethasenoughroomforthis
- //newframe;thatcheckcomeslater.)
- if(fNumFramesUsedSoFar>0){
- //如果包中已有了一个帧，并且不允许再打入新的帧了，则只记录下新的帧。
- if((fPreviousFrameEndedFragmentation&&!allowOtherFramesAfterLastFragment())
- ||!frameCanAppearAfterPacketStart(fOutBuf->curPtr(),frameSize))
- {
- //Saveawaythisframefornexttime:
- numFrameBytesToUse=0;
- fOutBuf->setOverflowData(fOutBuf->curPacketSize(),frameSize,
- presentationTime,durationInMicroseconds);
- }
- }
- 
- //表示当前打入的是否是上一个帧的最后一块数据。
- fPreviousFrameEndedFragmentation=False;
- 
- 
- //下面是计算获取的帧中有多少数据可以打到当前包中，剩下的数据就作为overflow数据保存下来。
- if(numFrameBytesToUse>0){
- //Checkwhetherthisframeoverflowsthepacket
- if(fOutBuf->wouldOverflow(frameSize)){
- //Don'tusethisframenow;instead,saveitasoverflowdata,and
- //senditinthenextpacketinstead.However,iftheframeistoo
- //bigtofitinapacketbyitself,thenweneedtofragmentit(and
- //usesomeofitinthispacket,ifthepayloadformatpermitsthis.)
- if(isTooBigForAPacket(frameSize)
- &&(fNumFramesUsedSoFar==0||allowFragmentationAfterStart())){
- //Weneedtofragmentthisframe,andusesomeofitnow:
- overflowBytes=computeOverflowForNewFrame(frameSize);
- numFrameBytesToUse-=overflowBytes;
- fCurFragmentationOffset+=numFrameBytesToUse;
- }else{
- //Wedon'tuseanyofthisframenow:
- overflowBytes=frameSize;
- numFrameBytesToUse=0;
- }
- fOutBuf->setOverflowData(fOutBuf->curPacketSize()+numFrameBytesToUse,
- overflowBytes,presentationTime,durationInMicroseconds);
- }elseif(fCurFragmentationOffset>0){
- //Thisisthelastfragmentofaframethatwasfragmentedover
- //morethanonepacket.Doanyspecialhandlingforthiscase:
- fCurFragmentationOffset=0;
- fPreviousFrameEndedFragmentation=True;
- }
- }
- 
- 
- 
- if(numFrameBytesToUse==0&&frameSize>0){
- //如果包中有数据并且没有新数据了，则发送之。（这种情况好像很难发生啊！）
- //Sendourpacketnow,becausewehavefilleditup:
- sendPacketIfNecessary();
- }else{
- //需要向包中打入数据。
- 
- //Usethisframeinouroutgoingpacket:
- unsignedchar*frameStart=fOutBuf->curPtr();
- fOutBuf->increment(numFrameBytesToUse);
- //dothisnow,incase"doSpecialFrameHandling()"calls"setFramePadding()"toappendpaddingbytes
- 
- 
- //Here'swhereanypayloadformatspecificprocessinggetsdone:
- doSpecialFrameHandling(curFragmentationOffset,frameStart,
- numFrameBytesToUse,presentationTime,overflowBytes);
- 
- 
- ++fNumFramesUsedSoFar;
- 
- 
- //Updatethetimeatwhichthenextpacketshouldbesent,based
- //onthedurationoftheframethatwejustpackedintoit.
- //However,ifthisframehasoverflowdataremaining,thendon't
- //countitsdurationyet.
- if(overflowBytes==0){
- fNextSendTime.tv_usec+=durationInMicroseconds;
- fNextSendTime.tv_sec+=fNextSendTime.tv_usec/1000000;
- fNextSendTime.tv_usec%=1000000;
- }
- 
- 
- //如果需要，就发出包，否则继续打入数据。
- //Sendourpacketnowif(i)it'salreadyatourpreferredsize,or
- //(ii)(heuristic)anotherframeofthesamesizeastheonewejust
- //readwouldoverflowthepacket,or
- //(iii)itcontainsthelastfragmentofafragmentedframe,andwe
- //don'tallowanythingelsetofollowthisor
- //(iv)oneframeperpacketisallowed:
- if(fOutBuf->isPreferredSize()
- ||fOutBuf->wouldOverflow(numFrameBytesToUse)
- ||(fPreviousFrameEndedFragmentation
- &&!allowOtherFramesAfterLastFragment())
- ||!frameCanAppearAfterPacketStart(
- fOutBuf->curPtr()-frameSize,frameSize)){
- //Thepacketisreadytobesentnow
- sendPacketIfNecessary();
- }else{
- //There'sroomformoreframes;trygettinganother:
- packFrame();
- }
- }
- }



看一下发送数据的函数：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6921145)[copy](http://blog.csdn.net/niu_gao/article/details/6921145)



- voidMultiFramedRTPSink::sendPacketIfNecessary()
- {
- //发送包
- if(fNumFramesUsedSoFar>0){
- //Sendthepacket:
- #ifdefTEST_LOSS
- if((our_random()%10)!=0)//simulate10%packetloss#####
- #endif
- if(!fRTPInterface.sendPacket(fOutBuf->packet(),fOutBuf->curPacketSize())){
- //iffailurehandlerhasbeenspecified,callit
- if(fOnSendErrorFunc!=NULL)
- (*fOnSendErrorFunc)(fOnSendErrorData);
- }
- ++fPacketCount;
- fTotalOctetCount+=fOutBuf->curPacketSize();
- fOctetCount+=fOutBuf->curPacketSize()-rtpHeaderSize
- -fSpecialHeaderSize-fTotalFrameSpecificHeaderSizes;
- 
- 
- ++fSeqNo;//fornexttime
- }
- 
- 
- //如果还有剩余数据，则调整缓冲区
- if(fOutBuf->haveOverflowData()
- &&fOutBuf->totalBytesAvailable()>fOutBuf->totalBufferSize()/2){
- //Efficiencyhack:Resetthepacketstartpointertojustinfrontof
- //theoverflowdata(allowingfortheRTPheaderandspecialheaders),
- //sothatweprobablydon'thaveto"memmove()"theoverflowdata
- //intoplacewhenbuildingthenextpacket:
- unsignednewPacketStart=fOutBuf->curPacketSize()-
- (rtpHeaderSize+fSpecialHeaderSize+frameSpecificHeaderSize());
- fOutBuf->adjustPacketStart(newPacketStart);
- }else{
- //Normalcase:Resetthepacketstartpointerbacktothestart:
- fOutBuf->resetPacketStart();
- }
- fOutBuf->resetOffset();
- fNumFramesUsedSoFar=0;
- 
- 
- if(fNoFramesLeft){
- //如果再没有数据了，则结束之
- //We'redone:
- onSourceClosure(this);
- }else{
- //如果还有数据，则在下一次需要发送的时间再次打包发送。
- //Wehavemoreframeslefttosend.Figureoutwhenthenextframe
- //isduetostartplaying,thenmakesurethatwewaitthislongbefore
- //sendingthenextpacket.
- structtimevaltimeNow;
- gettimeofday(&timeNow,NULL);
- intsecsDiff=fNextSendTime.tv_sec-timeNow.tv_sec;
- int64_tuSecondsToGo=secsDiff*1000000
- +(fNextSendTime.tv_usec-timeNow.tv_usec);
- if(uSecondsToGo<0||secsDiff<0){//sanitycheck:Makesurethatthetime-to-delayisnon-negative:
- uSecondsToGo=0;
- }
- 
- 
- //Delaythisamountoftime:
- nextTask()=envir().taskScheduler().scheduleDelayedTask(uSecondsToGo,
- (TaskFunc*)sendNext,this);
- }
- }



可以看到为了延迟包的发送，使用了delay task来执行下次打包发送任务。

sendNext()中又调用了buildAndSendPacket()函数，呵呵，又是一个圈圈。

总结一下调用过程：

![](https://img-blog.csdn.net/20130920171528156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

最后，再说明一下包缓冲区的使用：

MultiFramedRTPSink中的帧数据和包缓冲区共用一个，只是用一些额外的变量指明缓冲区中属于包的部分以及属于帧数据的部分（包以外的数据叫做overflow data）。它有时会把overflow data以mem move的方式移到包开始的位置，有时把包的开始位置直接设置到overflow data开始的地方。那么这个缓冲的大小是怎样确定的呢？是跟据调用者指定的的一个最大的包的大小+60000算出的。这个地方把我搞胡涂了：如果一次从source获取一个帧的话，那这个缓冲应设为不小于最大的一个帧的大小才是，为何是按包的大小设置呢？可以看到，当缓冲不够时只是提示一下：



**[cpp]**[view plain](http://blog.csdn.net/niu_gao/article/details/6921145)[copy](http://blog.csdn.net/niu_gao/article/details/6921145)



- if(numTruncatedBytes>0){
- 
- 
- unsignedconstbufferSize=fOutBuf->totalBytesAvailable();
- envir()
- <<"MultiFramedRTPSink::afterGettingFrame1():Theinputframedatawastoolargeforourbuffersize("
- <<bufferSize
- <<")."
- <<numTruncatedBytes
- <<"bytesoftrailingdatawasdropped!Correctthisbyincreasing\"OutPacketBuffer::maxSize\"toatleast"
- <<OutPacketBuffer::maxSize+numTruncatedBytes
- <<",*before*creatingthis'RTPSink'.(Currentvalueis"
- <<OutPacketBuffer::maxSize<<".)\n";
- }


当然此时不会出错，但有可能导致时间戳计算不准，或增加时间戳计算与source端处理的复杂性(因为一次取一帧时间戳是很好计算的)。



原文地址：[http://blog.csdn.net/niu_gao/article/details/6921145](http://blog.csdn.net/niu_gao/article/details/6921145)

live555源代码（VC6）：[http://download.csdn.net/detail/leixiaohua1020/6374387](http://download.csdn.net/detail/leixiaohua1020/6374387)




