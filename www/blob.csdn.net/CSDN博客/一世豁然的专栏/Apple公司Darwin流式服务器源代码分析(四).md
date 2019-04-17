# Apple公司Darwin流式服务器源代码分析(四) - 一世豁然的专栏 - CSDN博客





2016年10月25日 19:34:24[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1181








本博客转载于：[http://www.cublog.cn/u/11445/showart.php?id=225276](http://www.cublog.cn/u/11445/showart.php?id=225276)





4     核心功能库（Server Core）

4.1 RTSP 子系统
       RTSP标准是实时流控制协议（Real-Time Streaming Protocol RFC2326）的简称，它被客户和流式媒体服务器用来交换对媒体的控制信息。图6是RTSP基本操作的描述。
再给出一个RTSP协议的例子如下：
       DSS开发了一个RTSP子系统来支持标准的RTSP协议，本节将分析这些源代码。
首先，DSS定义了一个TCPListenerSocket类的子类RTSPListenerSocket，用于监听RTSP连接请求。RTSPListenerSocket类做的唯一一件事就是重载了GetSessionTask函数，当客户的连接请求到达后，它创建了一个Socket对象和RTSPSession对象的配对。RTSPSession对象是Task类的子类，是专门用于处理RTSP请求的任务类。
如图7所示，RTSP连接建立后，服务器会为每个客户维护一个Socket对象和RTSPSession对象的配对；当客户的RTSP请求到达时，Socket对象就会调用RTSPSession对象的Signal方法传信，即将RTSPSession对象加入到TaskThread对象的任务队列中去；而当时间片到来，TaskThread线程就会调用RTSPSession对象的Run方法，这个方法就会处理客户发送过来的RTSP请求。因此，下面我们将主要分析RTSPSession的Run方法。
为了跟踪当前处理的情况，RTSPSession类内部定义了多个状态，而Run方法其实就是通过在这些状态之间不断切换，同时对客户的RTSP请求做出不同的处理。
                     enum

                     {

                     //RTSPSession的基本状态
                     kReadingRequest= 0,

                     kFilteringRequest= 1,

                     kRoutingRequest= 2,

                     kAuthenticatingRequest= 3,

                     kPreprocessingRequest= 4,

                     kProcessingRequest= 5,

                     kSendingResponse= 6,

                     kPostProcessingRequest       = 7,

                     kCleaningUp= 8,


                     //当RTSP协议通过HTTP隧道实现时将用到下面的状态
       kWaitingToBindHTTPTunnel = 9,        


kSocketHasBeenBoundIntoHTTPTunnel = 10,

kHTTPFilteringRequest = 11,               

                     kReadingFirstRequest = 12,                 

                     kHaveNonTunnelMessage = 13                          

              }
另外，值得注意的是，DSS提供一种称为Module的二次开发模式，开发人员可以编写新的Module并且注册其希望运行的状态，系统就会在相应的状态下调用该Module，从而将控制权暂时交给二次开发的代码，以便增强系统的功能。简单起见，下面我们将分析不存在客户模块的Run()函数源代码。首先分析其主框架如下：
       SInt64 RTSPSession::Run()

{

       1     EventFlags events = this->GetEvents()；     //取出事件
       2     QTSS_Error err = QTSS_NoErr；
       3     QTSSModule* theModule = NULL；
       4     UInt32 numModules = 0；


       // 设定当前的Module状态
       5     OSThread::GetCurrent()->SetThreadData(&fModuleState)；


       //检查该连接是否超时，如果是就设定状态断掉该连接
       6     if ((events & Task::kTimeoutEvent) || (events & Task::kKillEvent))

       7            fLiveSession = false；


       8     while (this->IsLiveSession()) //如果连接尚未拆除，执行状态机
9     {

              /* 提示：下面是RTSPSession的状态机。因为在处理RTSP请求过程中，有多个地方需要Run方法返回以便继续监听新的事件。为此，我们需要跟踪当前的运行状态，以便在被打断后还能回到原状态*/

       10           switch (fState)

       11           {

       12                  case 状态1: //处理略
13    case 状态2: //处理略…

14    case 状态n: //处理略
       15           }     //此处略…

       }

       Run函数的主框架比较简单，其核心就在于10~15的状态机，因此我们希望按照客户请求到达并且被处理的主要流程为读者描述该状态机的运转。
       1第一次请求到达进入kReadingFirstRequest状态，该状态主要负责从RTSPRequestStream类的对象fInputStream中读出客户的RTSP请求，其处理如下：
              case kReadingFirstRequest:

              {

              1     if ((err = fInputStream.ReadRequest())=QTSS_NoErr)

              2     {/* RequestStream返回QTSS_NoErr意味着所有数据已经从Socket中读出，但尚不能构成一个完整的请求，因此必须等待更多的数据到达*/

              3            fInputSocketP->RequestEvent(EV_RE)； //接着请求监听读事件
              4            return 0；      //Run函数返回，等待下一个事件发生
              5     }

              6     if ((err != QTSS_RequestArrived) && (err != E2BIG))

              7     {//出错，停止处理
              8            Assert(err > 0)；

              9            Assert(!this->IsLiveSession())；
              10           break；
              11    }

                     //请求已经完全到达，转入kHTTPFilteringRequest状态
              12    if (err = QTSS_RequestArrived)

              13           fState = kHTTPFilteringRequest；
                     //接收缓冲区溢出，转入kHaveNonTunnelMessage状态
       14    if (err=E2BIG)

              15           fState = kHaveNonTunnelMessage；
              }

              continue；
       2正常情况下，在获得一个完整的RTSP请求后（上第12行），系统将进入kHTTPFilteringRequest状态该状态检查RTSP连接是否需要经过HTTP代理实现；如不需要，转入kHaveNonTunnelMessage状态。
       3进入kHaveNonTunnelMessage状态后，系统创建了RTSPRequest类的对象fRequest，该对象解析客户的RTSP请求，并保存各种属性。fRequest对象被传递给其他状态处理。
       4接着进入kFilteringRequest状态，二次开发人员可以通过编写Module对客户的请求做出特殊处理。如果客户的请求为正常的RTSP请求，系统调用SetupRequest函数建立用于管理数据传输的RTPSession类对象，其源代码分析如下：
       void RTSPSession::SetupRequest()

{

       // 首先分析RTSP请求，细节见RTSPRequest.h/.cpp

       1     QTSS_Error theErr = fRequest->Parse()；
2     if (theErr != QTSS_NoErr)  


       3            return；


              //OPTIONS请求，简单发回标准OPTIONS响应即可
4     if (fRequest->GetMethod() = qtssOptionsMethod)

       5     {//此处略去部分处理代码…

6     }


       //DESCRIBE请求，必须保证已经有了SessionID

       7     if (fRequest->GetMethod() = qtssDescribeMethod)

       8     {

       9            if (fRequest->GetHeaderDictionary()->GetValue(qtssSessionHeader)->Len > 0)

       10           {

       11                  (void)QTSSModuleUtils::SendErrorResponse(fRequest, qtssClientHeaderFieldNotValid, qtssMsgNoSesIDOnDescribe)；
12                  return；
       13           }

14    }


              //查找该请求的RTPSession

       15    OSRefTable* theMap = QTSServerInterface::GetServer()->GetRTPSessionMap()；
       16    theErr = this->FindRTPSession(theMap)；
       17    if (theErr != QTSS_NoErr)

       18           return；


       //如果未查找到，建立一个新的RTPSession

       19    if (fRTPSession= NULL)

       20    {

       21           theErr = this->CreateNewRTPSession(theMap)；
       22           if (theErr != QTSS_NoErr)

       23                  return；
       24    }

              //此处略…

}

       5进入kRoutingRequest状态，调用二次开发人员加入的Module，用于将该请求路由（Routing）出去。缺省情况下，系统本身对此状态不做处理。
       6进入kAuthenticatingRequest状态，调用二次开发人员加入的安全模块，主要用于客户身份验证以及其他如规则的处理。读者如果希望开发具有商业用途的流式媒体服务器，该模块必须进行二次开发。
       7进入kPreprocessingRequest和kProcessingRequest及kPostProcessingRequest状态，这三种状态都是通过调用系统自带或二次开发人员添加的Module来处理RTSP请求，例如系统提供了QTSSReflector
 Module、QTSSSplitter Module以及QTSSFile Module等模块。其中比较重要的QTSSFile Module属于QTLib库的部分，此处不再详述。
       8进入kSendingResponse状态，用于发送对客户RTSP请求处理完成之后的响应。系统在该状态调用了fOutputStream.Flush()函数将在fOutputStream中尚未发出的请求响应通过Socket端口完全发送出去。
       9进入kCleaningUp状态，清除所有上次处理的数据，并将状态设置为kReadingRequest等待下次请求到达。
       RTSPSession的主流程分析完了，但辅助其操作的多个RTSP类还需要读者自行分析，它们分别是：RTSPSessionInterface
 Class、RTSPRequest Class、RTSPRequestInterface Class、RTSPRequestStream Class以及RTSPResponseStream
 Class等等。
4.2 RTP子系统
       RTP标准是实时传输协议（Real-Time Transfer Protocol）的简称，它被客户和流式媒体服务器用来处理流式媒体数据的传输。在介绍RTSP的运行流程时，我们发现RTSPSession对象通过调用SetupRequest函数为客户建立RTPSession对象。RTPSession类是Task类的子类，因此它重载了Task类的Run函数，该函数通过调用FileModule.cpp文件中的SendPacket()函数向客户发送RTP协议打包的流式媒体数据。当客户通过利用RTSP向RTSPSession对象发出PLAY命令后，RTSPSession对象将调用RTPSession对象的Play()函数。Play函数准备好需要打包发送的数据后，利用Task类的Signal函数传信RTPSession对象，使其被加入某个TaskThread的任务队列，从而运行其Run函数。
另外，对同一个节目中的每一个独立的RTP流（如音频流或视频流等），DSS都定义了一个RTPStream类与之对应；显然一个RTPSession对象可能包含多个RTPStream对象。整个RTP子系统的核心运行流程见图8。
下面，我们首先分析RTPSession中Run()函数的用法：
       SInt64 RTPSession::Run()

{ //提示：该函数代码在TaskThread内运行
1     EventFlags events = this->GetEvents()； //取出事件
2     QTSS_RoleParams theParams；
       //提供给其他Module运行的参数，第一个成员是对象本身
       3     theParams.clientSessionClosingParams.inClientSession = this；

       //设定自己为当前运行的线程
       4     OSThread::GetCurrent()->SetThreadData(&fModuleState)；

              /*如果事件是通知RTPSession对象死亡，就准备自杀。可能导致这种情况的有两种事件：自杀kKillEvent；超时kTimeoutEvent*/

       5     if ((events & Task::kKillEvent) || (events & Task::kTimeoutEvent) || (fModuleDoingAsyncStuff))

       6     {     //处理对象自杀代码，此处略…

       7            return –1；     //返回出错信息，这样析构函数就会被调用，从而让对象完全死亡
       8     }


              //如果正处于暂停（PAUSE）状态，什么都不做就返回，等待PLAY命令
       9     if ((fState == qtssPausedState) || (fModule == NULL))

       10           return 0；


              //下面代码负责发送数据
       11    {     //对Session互斥量加锁，防止发送数据过程中RTSP请求到来
       12           OSMutexLocker locker(&fSessionMutex)；

                     //设定数据包发送时间，防止被提前发送
       13           theParams.rtpSendPacketsParams.inCurrentTime = OS::Milliseconds()；
       14           if (fPlayTime > theParams.rtpSendPacketsParams.inCurrentTime) //未到发送时间
       15                  theParams.rtpSendPacketsParams.outNextPacketTime=fPlayTime- theParams.rtpSendPacketsParams.inCurrentTime；
 //计算还需多长时间才可运行
       16           else

       17           {     //下次运行时间的缺缺省值为0

       18                  theParams.rtpSendPacketsParams.outNextPacketTime = 0；
                     //
设置Module状态
       19                  fModuleState.eventRequested = false；
       20                  Assert(fModule != NULL)；
                            //调用QTSS_RTPSendPackets_Role内的函数发送数据，见FileModule.cpp

       21                  (void)fModule->CallDispatch(QTSS_RTPSendPackets_Role, &theParams)；
                            //将返回值从负数改为0，否则任务对象就会被TaskThread删除
       22                  if (theParams.rtpSendPacketsParams.outNextPacketTime < 0)

       23                         theParams.rtpSendPacketsParams.outNextPacketTime = 0；
       24           }

       25    }

              //返回下一次希望被运行的时间；返回值含义见前文的分析
       26    return theParams.rtpSendPacketsParams.outNextPacketTime；
}
从上面分析可见，正常状态下Run函数的返回值有两种：如果返回值为正数，代表下一次发送数据包的时间，规定时间到来的时候，TaskThread线程会自动调用Run函数；如果返回值等于0，在下次任何事件发生时，Run函数就会被调用，这种情况往往发生在所有数据都已经发送完成或者该RTPSession对象将要被杀死的时候。
在第21行我们看到，Run函数调用了QTSSFileModule中的QTSS_RTPSendPackets_Role发送数据。在QTSSFileModule.cpp文件的QTSSFileModule_Main函数内，系统又调用了SendPackets函数，这才是真正发送RTP数据包的函数，我们对其代码分析如下：
       QTSS_Error SendPackets(QTSS_RTPSendPackets_Params* inParams)

{

              //此处略去部分定义…

       //得到要发送数据的FileSession对象，其定义见QTSSFileModule.cpp文件
       1     FileSession** theFile = NULL；
       2     UInt32 theLen = 0；
       3     QTSS_Error theErr = QTSS_GetValuePtr(inParams->inClientSession, sFileSessionAttr, 0, (void**)&theFile, &theLen)；

       4     if ((theErr != QTSS_NoErr) || (theLen != sizeof(FileSession*))) //出错
       5     {     //设定出错原因，然后断掉连接，并返回
       6            QTSS_CliSesTeardownReason reason = qtssCliSesTearDownServerInternalErr；
       7            (void) QTSS_SetValue(inParams->inClientSession, qtssCliTeardownReason, 0, &reason, sizeof(reason))；
       8            (void)QTSS_Teardown(inParams->inClientSession)；
       9            return QTSS_RequestFailed；
       10    }

       //该节目文件中音频所能忍受的最大延迟
       11    maxDelayToleranceForStream = (*theFile)->fMaxAudioDelayTolerance；


       12    while (true)

       13    {     

                     //不存在待发送数据包，可能是文件尚未打开
       14           if ((*theFile)->fNextPacket == NULL)

       15           {

       16                  void* theCookie = NULL；
                            //获得第一个数据包，theTransmitTime为传输数据花费的时间
       17                  Float64 theTransmitTime = (*theFile)->fFile.GetNextPacket(&(*theFile)->fNextPacket, &(*theFile)->fNextPacketLen, &theCookie)；
       18                  if ( QTRTPFile::errNoError != (*theFile)->fFile.Error() )

                            {//读数据出错，断掉连接，返回。此处略 }

                            …

       19                  (*theFile)->fStream = (QTSS_RTPStreamObject)theCookie； //得到RTPStream对象
       20                  (*theFile)->fPacketPlayTime = (*theFile)->fAdjustedPlayTime + ((SInt64)(theTransmitTime * 1000))；
 //推迟theTransmitTime长度的播放时间
       21                  (*theFile)->fPacketWasJustFetched = true；


       22                  if ((*theFile)->fNextPacket != NULL)

       23                  {     // 判断流格式
       24                         QTSS_RTPPayloadType* thePayloadType = NULL；
       25                         QTSS_Error theErr = QTSS_GetValuePtr( (*theFile)->fStream, qtssRTPStrPayloadType, 0, (void**)&thePayloadType, &theLen )；
                                   //设定视频流可忍受的最大延迟时间
       26                         if (*thePayloadType == qtssVideoPayloadType)

       27                         maxDelayToleranceForStream = (*theFile)->fMaxVideoDelayTolerance；
       28                  }

       29           }


                     //仍无数据，说明所有数据已经传输完成了
       30           if ((*theFile)->fNextPacket = NULL)

       31           {     //向fStream中写入长度为0的空数据，以便强制缓冲区刷新
       32                  (void)QTSS_Write((*theFile)->fStream, NULL, 0, NULL, qtssWriteFlagsIsRTP)；
       33                  inParams->outNextPacketTime = qtssDontCallSendPacketsAgain；
       34                  return QTSS_NoErr； //完成任务返回
       35           }

                     //提示：开始发送RTP数据包
                     //计算当前时间和该段数据应该发送的时间之间的相对间隔
       36           SInt64 theRelativePacketTime = (*theFile)->fPacketPlayTime - inParams->inCurrentTime；  // inCurrentTime
 = OS::Milliseconds()；


       37           SInt32 currentDelay = theRelativePacketTime * -1L； //计算传输延迟
       38           theErr =  QTSS_SetValue( (*theFile)->fStream, qtssRTPStrCurrentPacketDelay, 0, ¤tDelay, sizeof(currentDelay) )；
 //保存该延迟
                     //如果延迟过大，就丢弃该包，等待发送下一个数据包
       39           if (theRelativePacketTime > sMaxAdvSendTimeInMsec)

       40           {

       41                  Assert( theRelativePacketTime > 0 )；
       42                  inParams->outNextPacketTime = theRelativePacketTime；
       43                  return QTSS_NoErr；
       44           }

                     //此处略去部分处理视频质量的代码…


                     // 发送当前数据包
       45           QTSS_Error writeErr = QTSS_Write((*theFile)->fStream, (*theFile)->fNextPacket, (*theFile)->fNextPacketLen, NULL, qtssWriteFlagsIsRTP)；


//其余代码略…

}

       RTP子系统是DSS中最为复杂的部分之一，这是因为发送RTP数据包的过程不但涉及到网络接口，而且和文件系统有着密切的关系。DSS的一个重要特征就是能够将线索化（Hinted）过的QuickTime文件通过RTSP和RTP协议流化出去。所有分析这些文件的代码都被提取出来并且封装在QTFile库中。这种封装方式使得系统的各个部分都变得简单：QTFile负责处理文件的分析；而DSS其他部分负责处理网络和协议。服务器中的RTPFileModule调用QTFile库检索索引过的QuickTime文件的数据包和元数据。QTFile库的讲解超出了本文的范围，但是希望让DSS支持其他媒体格式的读者能够掌握它的实现机制。

5  DSS二次开发接口：Module开发流程

作为一个运行于多个操作系统平台的开发源代码的服务器，DSS提供了一种称为Module的二次开发接口。使用这个开发接口，我们可以充分利用服务器的可扩展性及其实现的多种协议，并且能够保证和将来版本兼容。DSS中的许多核心功能也是以Module的方式预先实现并且编译的，因此可以说对Module的支持已经被设计到DSS的内核中去了。
下面我们将分析DSS的一个内嵌Module：QTSSFileModule的源代码来说明Module的编程方式，QTSSFileModule的实现在QTSSFileModule.cpp文件中。
每个QTSS Module必须实现两个函数：
首先，每个QTSS Module必须实现一个主函数，服务器调用该函数用于启动和初始化模块中的QTSS函数；QTSSFileModule主函数的实现如下：
QTSS_Error QTSSFileModule_Main(void* inPrivateArgs)

{

       return _stublibrary_main(inPrivateArgs, QTSSFileModuleDispatch)；
}
其中QTSSFileModuleDispatch是Module必须实现的分发函数名。
另一个需要实现的是分发函数，服务器调用该函数实现某个特殊任务。此时，服务器将向分发函数传入任务的名字和一个任务相关的参数块。QTSSFileModule分发函数的实现如下：
QTSS_Error QTSSFileModuleDispatch(QTSS_Role inRole, QTSS_RoleParamPtr inParamBlock)

{     //根据传入的任务名称和入参执行相应的处理函数
       switch (inRole)      //任务名称
       {

              case QTSS_Register_Role:

                     return Register(&inParamBlock->regParams)；
              case QTSS_Initialize_Role:

                     return Initialize(&inParamBlock->initParams)；
              case QTSS_RereadPrefs_Role:

                     return RereadPrefs()；
              case QTSS_RTSPRequest_Role:

                     return ProcessRTSPRequest(&inParamBlock->rtspRequestParams)；
              case QTSS_RTPSendPackets_Role:

                     return SendPackets(&inParamBlock->rtpSendPacketsParams)；
              case QTSS_ClientSessionClosing_Role:

                     return DestroySession(&inParamBlock->clientSessionClosingParams)；
       }

       return QTSS_NoErr；
}
其中，分发函数的入参是一个联合，它根据任务名称的不同，具体的数据结构也不同，下面是该数据结构的定义：
       typedef union

{

              QTSS_Register_Params                             regParams；
              QTSS_Initialize_Params                            initParams；
              QTSS_ErrorLog_Params                           errorParams；
              //此处略去其他多个数据结构…

} QTSS_RoleParams, *QTSS_RoleParamPtr；
       DSS提供了两种方式把我们自己开发的Module添加到服务器中：一种称为静态模块（Static Module），该方式将我们开发的Module代码直接编译到内核中去；另一种称为动态模块（Dynamic
 Module），该方式将我们开发的Module单独编译称为一个动态库，然后修改配置，使服务器在启动时将其加载。图9描述了DSS启动和关闭时模块调用流程。
当服务器启动时，它首先装载没有被编译进内核的动态模块，然后才装载被编译进内核的静态模块；由于现有的大部分系统功能都是以静态模块的方式存在的，如果你希望用自己的模块替换某个系统功能，最好是编写一个动态模块，因为它们将早于静态模块被装载。
无论是静态模块还是动态模块，它们的代码都是相同的，唯一的不同就是它们的编译方式。首先为了将静态模块编译到服务器中，我们必须修改QTSServer.cpp文件中的QTSServer::LoadCompiledInModules，并向其中加入以下代码：
       QTSSModule*       myModule=new QTSSModule(*_XYZ_*)；
       (void)myModule->Initialize(&sCallbacks,&_XYZMAIN_)；
       (void)AddModule(MyModule)；
其中，XYZ是静态模块的名字，而XYZMAIN则是其主函数入口。
动态模块的编译方法如下：首先单独编译动态模块为一个动态共享库；将该共享库与QTSS API stub library链接到一起；最后将结果文件放置到/usr/sbin/QTSSModules目录中去。此后，服务器在启动时就将自动调用该动态模块。

6  结束语

DSS是一项十分庞大的工程，而且随着新版本的不断推出和功能的增强，其内容也越来越丰富。限于篇幅，本文只是介绍了一些笔者认为比较重要的模块或类，希望能够配合读者更好的掌握DSS的精髓。
我们之所以研究DSS的源代码，基本上有两个目标：一是希望利用DSS作为平台进行二次开发，如增加对媒体格式的支持，增加客户身份认证，增加对媒体内容的管理等模块，使DSS成为一个符合实际需求的实用系统。抱此目的的读者在掌握DSS整体流程的基础上，应着重于其二次开发平台（如Module）以及底层文件和媒体格式支持库的研究。另一类读者可能希望通过研究DSS源代码，掌握在Internet环境中处理流式媒体的关键技术，以便为将来开发相关底层应用做准备。对于这些读者，笔者认为需要下更多的功夫去研究DSS源代码中的许多细节部分：例如高级网络编程（Socket）、多线程之间的通信、任务调度、系统资源（CPU、磁盘等）的合理利用以及用于流式媒体的多个标准协议（RTP/RTCP、RTSP、SDP）的具体实现等等。
作为三大主要流式媒体应用中唯一一个开放源代码的产品，DSS让开发人员能够从最底层研究流式媒体技术，事实上，当前国内外许多公司正是在DSS的基础上开发了自己的流式媒体相关产品。但是需要指出，作为一个开放源代码的工程，DSS的分发和开发须遵循苹果公司给出的一份版权文件（Apple
 Public Source License），希望进行商业化开发的读者应该仔细研读，该文件可从以下网址获得：http://www.publicsource.apple.com。
最后，如果读者希望跟踪DSS的最新进展，可以申请加入其邮件列表。通过该邮件列表，读者可以和全球众多的DSS开发人员交流经验，而且苹果公司的技术人员将会定期的解答各种问题。该邮件列表的地址为：[http://www.lists.apple.com](http://www.lists.apple.com/)。






