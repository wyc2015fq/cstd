# darwin Stream Server源码分析 - DoubleLi - 博客园







*摘要* ​所谓的流式媒体简单的讲就是指人们通过网络实时的收看多媒体信息：如音频流、视频流等。与流式媒体对应的传统工作方式是下载+播放模式，即用户首先下载多媒体文件，然后再在本地播放，这种方法的一个主要缺点是启动延迟较大，例如一个30分钟长的MPEG-I文件（相当于VCD质量），即使使用1.5Mbps的速率下载，也需要半个小时才能完成，这样一个漫长的等待时间实在是无法忍受。在窄带网络环境中，几乎所有基于Internet的流式媒体产品都有着类似的工作原理：首先需要开发高效的压缩编码技术，并通过一套完整有效的传输体系将其发布到用户的桌面上。目前在流式媒体领域，有三种占有主导地位的产品，它们分别是App...


2     Darwin流化服务器介绍
DSS源代码完全采用标准C++语言写成，编程风格非常优秀，每个C++类都对应着一对和类同名的.h/.cpp文件。但是由于大量采用了面向对象的概念，如继承、多态等等；而且源文件和类相当多，所以不太容易讲清楚。因此，读者最好事先把代码完整的过滤一两遍，再配合本文，就能看得更清楚点。
其中，最为重要的是基础功能类库（CommonUtilitiesLib）和流化服务器（StreamingServer）两个工程，前者是整个系统的通用代码工具箱，包括了线程管理、数据结构、网络和文本分析等多个功能模块。DSS和其他相关的工具使用基础功能类库工程中定义的功能类实现以下三个目标：
（1）抽象出系统中相同或类似的功能，用于降低代码的冗余度；
（2）封装基本功能，简化高层编码的复杂度；
（3）隔离开操作系统平台相关的代码。
而流化服务器工程中包含了DSS对多个国际标准的实现，是整个服务器的主工程。在本文中，我们将重点分析这两个工程中的核心代码和模块。另外，我们还将简单介绍利用DSS提供的开发接口（Module）扩展和定制服务器的方法。
DSS实现了四种IETF制定的国际标准，分别是：实时流传输协议RTSP（Real-time Streaming Protocol, RFC 2326）、实时传输协议（RTP Real-time Transfer Protocol，RFC 1889）、实时传输控制协议RTCP（Real-time Transport Control Protocol，RFC 1889）、会话描述协议SDP（Session Description Protocol，RFC 2327）。这四个标准是开发所有流式媒体产品都必须掌握的，因此在对相关代码进行分析和二次开发之前，希望读者了解上述四种协议的基本思想，上述协议样本可从以下网站获得：http://www.ietf.org
3     基础功能类库（Common Utilities）
3.2   Tasks类
因为服务器从整体上采用了异步的运行模式，这就需要一种用于事件通信的机制。举例来说：一个RTSP连接对应的Socket端口监测到网络上有数据到达，此时必须有一个模块（或代码）被通知（notify）去处理这些数据。为此，DSS定义了Task及其相关类作为实现这一通信机制的核心。
在Task.h/cpp文件中，定义了三个主要的类，分别是：任务线程池类（TaskThreadPool Class）、任务线程类（TaskThread Class）以及任务类（Task Class）。
每个Task对象有两个主要的方法：Signal和Run。当服务器希望发送一个事件给某个Task对象时，就会调用Signal()方法；而Run()方法是在Task对象获得处理该事件的时间片后运行的，服务器中的大部分工作都是在不同Task对象的Run()函数中进行的。每个Task对象的目标就是利用很小的且不会阻塞的时间片完成服务器指定某个工作。
任务线程类是上文介绍的OSThread类的一个子类，代表专门用于运行任务类的一个线程。在每个任务线程对象内部都有一个OSQueue_Blocking类型的任务队列，存储该线程需要执行的任务。后面的分析可以看到，服务器调用一个任务的Signal函数，实际上就是将该任务加入到某个任务线程类的任务队列中去。另外，为了统一管理这些任务线程，DSS还开发了任务线程池类，该类负责生成、删除以及维护内部的任务线程列表。图4描述了任务类的运行。
       下面我们首先分析TashThread类，该类的定义如下：
class TaskThread : public OSThread     //OSThread的子类
{     //提示：所有的Task对象都将在TaskThread中运行
       1     public:
       2     TaskThread() :       OSThread(), fTaskThreadPoolElem(this){}  //构造函数
3     virtual                   ~TaskThread() { this->StopAndWaitForThread()； } //析构函数
       4     private:
              …
       5     virtual void     Entry()；       //从OSThread重载的执行函数，仍然能够被子类重载
       6     Task*                   WaitForTask()；    //检测是否有该执行的任务

       7     OSQueueElem        fTaskThreadPoolElem；       //对应的线程池对象
       8     OSHeap                        fHeap； //纪录任务运行时间的堆，用于WaitForTask函数
              /*关键数据结构：任务队列；在Task的Signal函数中直接调用fTaskQueue对象的EnQueue函数将自己加入任务队列*/
       9     OSQueue_Blocking fTaskQueue； 
              //此处略…
       }
       作为OSThread的子类，TaskThread重载了Entry函数，一旦TaskThread的对象被实例化，便运行该函数。Entry()函数的主要任务就是调用WaitForTask()函数监测任务队列，如果发现新任务，就在规定时间内执行；否则，就被阻塞。下面我们简要分析Entry()函数的流程：
       void TaskThread::Entry()
{
       1     Task* theTask = NULL； //空任务

       2     while (true) //线程循环执行
       3     {     //监测是否有需要执行的任务，如果有就返回该任务；否则阻塞；
       4            theTask = this->WaitForTask()； 
       5            Assert(theTask != NULL)；

6            Bool16 doneProcessingEvent = false； //尚未处理事件

7            while (!doneProcessingEvent)
       8            {
       9            theTask->fUseThisThread = NULL； // 对任务的调度独立于线程
       10           SInt64 theTimeout = 0；      //Task中Run函数的返回值，重要
                     //核心部分：运行任务，根据返回值判断任务进度
       11           if (theTask->fWriteLock)
       12           {     //如果任务中有写锁，需要使用写互斥量，否则可能造成死锁
       13                  OSMutexWriteLocker mutexLocker(&TaskThreadPool::sMutexRW)；
       14                  theTimeout = theTask->Run()；   //运行任务，得到返回值
       15                  theTask->fWriteLock = false；
       16           }
       17           else
       18           {     //使用读互斥量
       19                  OSMutexReadLocker mutexLocker(&TaskThreadPool::sMutexRW)；
       20                  theTimeout = theTask->Run()；   //运行任务，得到返回值
       21           }
       22           //监测Task中Run()函数的返回值，共有三种情况
       23           //1、返回负数，表明任务已经完全结束
       24           if (theTimeout        25           {
       26                  delete theTask；     //删除Task对象
       27                  theTask = NULL；
       28                  doneProcessingEvent = true；
       19           }
       30           //2、返回0，表明任务希望在下次传信时被再次立即执行
       31           else if (theTimeout=0)
       32           {
       33                  doneProcessingEvent = compare_and_store(Task::kAlive, 0, &theTask->fEvents)；
       34                  if (doneProcessingEvent)
       35                         theTask = NULL； 
       36           }
                     //3、返回正数，表明任务希望在等待theTimeout时间后再次执行
       37           else
       38           {
                     /*将该任务加入到Heap中，并且纪录它希望等待的时间。Entry()函数将通过waitfortask()函数进行检测，如果等待的时间到了，就再次运行该任务*/
       39                  theTask->fTimerHeapElem.SetValue(OS::Milliseconds() + theTimeout)；
       40                  fHeap.Insert(&theTask->fTimerHeapElem)；
       41                  (void)atomic_or(&theTask->fEvents, Task::kIdleEvent)；//设置Idle事件
       42                  doneProcessingEvent = true；
       43           }
              //此处略…
       }
       注意，如果Task的Run()函数返回值TimeOut为正数，意味着该任务是一个周期性的工作，例如发送数据的视频泵（pump），需要每隔一定时间就发出一定量的视频数据，直至整个节目结束。为此，在第38~43行，将该任务加入到堆fHeap中去，并且标记该任务下次运行的时间为TimeOut毫秒之后。将来通过调用WaitForTask()函数就能检测到该任务是否到达规定的运行时间，WaitForTask()函数的代码如下：
       Task* TaskThread::WaitForTask()
{
       1     while (true)
       2     {     //得到当前时间，该函数为静态函数，定义见OS.h
       3            SInt64 theCurrentTime = OS::Milliseconds()； 
                     /*如果堆中有任务，且任务已经到执行时间，返回该任务。 PeekMin函数见OSHeap.h，窃听堆中第一个元素（但不取出）*/
4     if ((fHeap.PeekMin() != NULL) && (fHeap.PeekMin()->GetValue() 从堆中取出第一个任务返回
5                   return (Task*)fHeap.ExtractMin()->GetEnclosingObject()；
              //如果堆中有任务，但是尚未到执行时间，计算需要等待的时间
       6            SInt32 theTimeout = 0；
       7            if (fHeap.PeekMin() != NULL)      //计算还需等待的时间
       8                   theTimeout = fHeap.PeekMin()->GetValue() - theCurrentTime；
       9            Assert(theTimeout >= 0)；

              //等待theTimeout时间后从堆中取出任务返回
       10           OSQueueElem* theElem = fTaskQueue.DeQueueBlocking(this, theTimeout)；
       11           if (theElem != NULL)
       12                  return (Task*)theElem->GetEnclosingObject()；
       13    }     
}
       void Task::Signal(EventFlags events)
{
              …
              // fUseThisThread用于指定该任务运行的任务线程
       1     if (fUseThisThread != NULL)       //存在指定任务线程
                     //将该任务加入到指定任务线程的任务队列中
       2            fUseThisThread->fTaskQueue.EnQueue(&fTaskQueueElem)；
              //不存在指定的任务线程，随机选择一个任务线程运行该任务
3     else
       4     {
                     //从线程池中随机选择一个任务线程
       5            unsigned int theThread = atomic_add(&sThreadPicker, 1)；
       6            theThread %= TaskThreadPool::sNumTaskThreads；
                     //将该任务加入到上面选择的任务线程的任务队列中
       7            TaskThreadPool::sTaskThreadArray[theThread]-> fTaskQueue.EnQueue (&fTaskQueueElem)；
       8            }
       }
       至此我们已经将DSS的线程和任务运行机制分析完了，这种由事件去触发任务的概念已经被集成到了DSS的各个子系统中。例如，在DSS中经常将一个Task对象和一个Socket对象关联在一起，当Socket对象收到事件（通过select()函数），相对应的Task对象就会被传信（通过Signal()函数）；而包含着处理代码的Run()函数就将在某个任务线程中运行。
       因此，通过使用这些Task对象，我们就可以让所有连接都使用一个线程来处理，这也是DSS的缺省配置方法。
3.3   Socket类
作为一个典型的网络服务器，DSS源代码中的Socket编程部分是其精华之一。DSS定义了一系列Socket类用于屏蔽不同平台在TCP/UDP编程接口和使用方法上的差异。DSS中的Socket类一般都采用异步模式的（即非阻塞的），而且能够向对应的Task对象传信（Signal），这点我们在上一节介绍过。Socket类中具有代表性的类是：EventContext、EventThread、Socket、UDPSocket、TCPSocket以及TCPListenerSocket等等，它们之间的继承关系见图5。
       在eventcontext.h/.cpp文件中，定义了两个类：EventContext类和EventThread类。 Event Context提供了检测Unix式的文件描述符（Socket就是一种文件描述符）产生的事件（通常是EV_RE 或 EV_WR）的能力，同时还可以传信指定的任务。EventThread类是OSThread类的子类，它本身很简单，只是重载了OSThread的纯虚函数Entry()，用以监控所有的Socket端口是否有数据到来，其代码分析如下：
       void EventThread::Entry()
{
/*该结构定义在ev.h中，记录Socket描述符和在该描述符上发生的事件*/
       1     struct eventreq theCurrentEvent；       
       2     ::memset( &theCurrentEvent, '\0', sizeof(theCurrentEvent) )；   //初始化该结构

       3     while (true)
4     {
//首先监听Socket端口的事件
       5            int theErrno = EINTR；
       6            while (theErrno=EINTR)
       7            {
8     #if MACOSXEVENTQUEUE //Macos平台
       9                   int theReturnValue = waitevent(&theCurrentEvent, NULL)；
10    #else       //其他平台
              /*调用select_waitevent函数监听所有的Socket端口，直到有事件发生为止*/
       11                  int theReturnValue = select_waitevent(&theCurrentEvent, NULL)；
12    #endif     
              …
              //有事件发生，唤醒相应的Socket端口
13    if (theCurrentEvent.er_data != NULL)
       14    {
                     //通过事件中的标识找到相应的对象参考指针
       15           StrPtrLen idStr((char*)&theCurrentEvent.er_data, sizeof(theCurrentEvent.er_data))；
       16           OSRef* ref = fRefTable.Resolve(&idStr)；
       17           if (ref != NULL)
       18           {     //通过参考指针得到EventContext对象
       19                  EventContext* theContext = (EventContext*)ref->GetObject()；
                            //利用EventContext对象的ProcessEvent方法传信对应的Task
       20                  theContext->ProcessEvent(theCurrentEvent.er_eventbits)；
       21                  fRefTable.Release(ref)；       //减少引用计数
       22           }
//此处略…
}
       上述代码有两点需要注意：首先在第11行，调用select_waitevent函数监听所有Socket端口的事件。该函数在Windows平台上是采用WSAAsyncSelect（异步选择）模型实现的。具体实现是：系统首先创建一个窗口类，该类专门用于接受消息；在每个Socket端口创建后，调用WSAsyncSelect函数，同时将上述窗口类的句柄作为参数传入；将来这些Socket端口有事件发生时，Windows就会自动将这些事件映射为标准的Windows消息发送给窗口类，此时select_waitevent函数通过检查消息就能够获得对应Socket端口发生的事件。对于Windows平台下Socket的异步编程技术细节请参阅《Windows网络编程技术》一书。
       另外，在第20行调用的EventContext对象的ProcessEvent函数实现上很简单，只有一行代码：fTask->Signal(Task::kReadEvent)；其中fTask为该EventContext对象对应的Task对象；ProcessEvent函数向Task对象传信，以便及时处理刚刚发生的Socket事件。
       与EventThread对应的EventContext对象负责维护指定的描述符，其主要函数包括InitNonBlocking、CleanUp和RequestEvent等。其中InitNonBlocking函数调用Socket API ioctlsocket将用户指定的描述符设置为异步，CleanUp函数用于关闭该描述符；另外，用户通过RequestEvent函数申请对该描述符中某些事件的监听，如前所述，该函数内部调用了WSAsyncSelect来实现这一功能。
       Socket Class、UDPSocket Class和TCPSocketClass三个类都是EventContext的子类，它们封装了TCP和UDP的部分实现，同时扩展了EventContext中的事件，但都没有改变其运行机制，因此此处不再详述，留给读者自行分析。我们要为大家分析的是另外一个比较复杂的Socket类TCPListenerSocket类。TCPListenerSocket用于监听TCP端口，当一个新连接请求到达后，该类将赋予这个新连接一个Socket对象和一个Task对象的配对。首先分析TCPListenerSocket类的主要定义如下：
       class TCPListenerSocket : public TCPSocket, public IdleTask
{
/*提示：该类从有两个基类，所以它既是一个事件监听者，同时也是一个任务Task。作为一个任务，给TCPListenerObject发送Kill事件就可以删除它*/
       1     public:
       2            TCPListenerSocket() :   TCPSocket(NULL, Socket::kNonBlockingSocketType), IdleTask(), fAddr(0), fPort(0), fOutOfDescriptors(false) {}  //构造函数
       3            virtual ~TCPListenerSocket() {}   //析构函数

                     //addr为地址，port为端口号，初始化函数自动监听TCP端口
       4            OS_Error              Initialize(UInt32 addr, UInt16 port)；
                     //子类必须重载该纯虚函数，用于建立新连接时生成任务对象
       5            virtual Task*   GetSessionTask(TCPSocket** outSocket) = 0；
       6            virtual SInt64  Run()；  //重载Task的Run函数，子类仍可重载

       7     private:
                     //重载EventContext的ProcessEvent函数，用于产生Socket和Task对象配对
8            virtual void ProcessEvent(int eventBits)；
       9            OS_Error       Listen(UInt32 queueLength)；
//其他略…
}
       前面我们分析得知，EventContext类通过ProcessEvent函数来实现对任务的传信工作，但在TCPListenerSocket 中，ProcessEvent函数被重载用来创建Socket和Task对象得配对，该函数的实现如下：
       void TCPListenerSocket::ProcessEvent(int /*eventBits*/)
{     /*提示：该函数运行于系统唯一的EventThread线程中，所以要尽量快速，以免占用过多的系统资源*/
              //此处略去部分定义…
       1     Task* theTask = NULL；     //Task对象
       2     TCPSocket* theSocket = NULL；       //Socket对象

              //创建对象配对
       3     while (true)
       4     {     //accept连接
       5            int osSocket = accept(fFileDesc, (struct sockaddr*)&addr, &size)； 
       6            if (osSocket == -1) //监听端口出错
       7            {     //此处略去出错处理     }
                     //用子类重载的GetSessionTask函数创建Task对象
       8            if ((theTask = this->GetSessionTask(&theSocket))=NULL) //创建出错
       9                   close(osSocket)；
       10           else  //创建成功，接着创建Socket对象
       11           {     
       12                  Assert(osSocket != EventContext::kInvalidFileDesc)；
                            //此处略去部分对新建连接端口的设置(setsockopt函数)
                            //创建新的Socket对象
       13                  theSocket->Set(osSocket, &addr)；
       14                  theSocket->InitNonBlocking(osSocket)； //初始化
       15                  theSocket->SetTask(theTask)； //设置对应的任务
       16           theSocket->RequestEvent(EV_RE)； //新对象监听读事件
       17           }
       18    }
              //处理完一次连接请求后，TCPListenerSocket对象还要接着监听
       19    this->RequestEvent(EV_RE)；
}
       对Socket类的分析基本完成了，从中我们可以发现，DSS对于网络传信和任务调度之间的处理非常精密，环环相扣，在某种程度上甚至是有些过a于花哨。但是这些基本类是上层RTSP/RTP等服务器子系统编码的基础，因此希望读者能够从本质上掌握这些代码。
4     核心功能库（Server Core）
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
                     kPostProcessingRequest       = 7,
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
       1     EventFlags events = this->GetEvents()；     //取出事件
       2     QTSS_Error err = QTSS_NoErr；
       3     QTSSModule* theModule = NULL；
       4     UInt32 numModules = 0；

       // 设定当前的Module状态
       5     OSThread::GetCurrent()->SetThreadData(&fModuleState)；

       //检查该连接是否超时，如果是就设定状态断掉该连接
       6     if ((events & Task::kTimeoutEvent) || (events & Task::kKillEvent))
       7            fLiveSession = false；

       8     while (this->IsLiveSession()) //如果连接尚未拆除，执行状态机
9     {
              /* 提示：下面是RTSPSession的状态机。因为在处理RTSP请求过程中，有多个地方需要Run方法返回以便继续监听新的事件。为此，我们需要跟踪当前的运行状态，以便在被打断后还能回到原状态*/
       10           switch (fState)
       11           {
       12                  case 状态1: //处理略
13    case 状态2: //处理略…
14    case 状态n: //处理略
       15           }     //此处略…
       }
       Run函数的主框架比较简单，其核心就在于10~15的状态机，因此我们希望按照客户请求到达并且被处理的主要流程为读者描述该状态机的运转。
       1第一次请求到达进入kReadingFirstRequest状态，该状态主要负责从RTSPRequestStream类的对象fInputStream中读出客户的RTSP请求，其处理如下：
              case kReadingFirstRequest:
              {
              1     if ((err = fInputStream.ReadRequest())=QTSS_NoErr)
              2     {/* RequestStream返回QTSS_NoErr意味着所有数据已经从Socket中读出，但尚不能构成一个完整的请求，因此必须等待更多的数据到达*/
              3            fInputSocketP->RequestEvent(EV_RE)； //接着请求监听读事件
              4            return 0；      //Run函数返回，等待下一个事件发生
              5     }
              6     if ((err != QTSS_RequestArrived) && (err != E2BIG))
              7     {//出错，停止处理
              8            Assert(err > 0)； 
              9            Assert(!this->IsLiveSession())；
              10           break；
              11    }
                     //请求已经完全到达，转入kHTTPFilteringRequest状态
              12    if (err = QTSS_RequestArrived)
              13           fState = kHTTPFilteringRequest；
                     //接收缓冲区溢出，转入kHaveNonTunnelMessage状态
       14    if (err=E2BIG)
              15           fState = kHaveNonTunnelMessage；
              }
              continue；
       2正常情况下，在获得一个完整的RTSP请求后（上第12行），系统将进入kHTTPFilteringRequest状态该状态检查RTSP连接是否需要经过HTTP代理实现；如不需要，转入kHaveNonTunnelMessage状态。
       3进入kHaveNonTunnelMessage状态后，系统创建了RTSPRequest类的对象fRequest，该对象解析客户的RTSP请求，并保存各种属性。fRequest对象被传递给其他状态处理。
       4接着进入kFilteringRequest状态，二次开发人员可以通过编写Module对客户的请求做出特殊处理。如果客户的请求为正常的RTSP请求，系统调用SetupRequest函数建立用于管理数据传输的RTPSession类对象，其源代码分析如下：
       void RTSPSession::SetupRequest()
{
       // 首先分析RTSP请求，细节见RTSPRequest.h/.cpp
       1     QTSS_Error theErr = fRequest->Parse()；
2     if (theErr != QTSS_NoErr)   
       3            return；

              //OPTIONS请求，简单发回标准OPTIONS响应即可
4     if (fRequest->GetMethod() = qtssOptionsMethod)
       5     {//此处略去部分处理代码…
6     }

       //DESCRIBE请求，必须保证已经有了SessionID
       7     if (fRequest->GetMethod() = qtssDescribeMethod)
       8     {
       9            if (fRequest->GetHeaderDictionary()->GetValue(qtssSessionHeader)->Len > 0)
       10           {
       11                  (void)QTSSModuleUtils::SendErrorResponse(fRequest, qtssClientHeaderFieldNotValid, qtssMsgNoSesIDOnDescribe)；
12                  return；
       13           }
14    }

              //查找该请求的RTPSession
       15    OSRefTable* theMap = QTSServerInterface::GetServer()->GetRTPSessionMap()；
       16    theErr = this->FindRTPSession(theMap)；
       17    if (theErr != QTSS_NoErr)
       18           return；

       //如果未查找到，建立一个新的RTPSession
       19    if (fRTPSession= NULL)
       20    {
       21           theErr = this->CreateNewRTPSession(theMap)；
       22           if (theErr != QTSS_NoErr)
       23                  return；
       24    }
              //此处略…
}
       5进入kRoutingRequest状态，调用二次开发人员加入的Module，用于将该请求路由（Routing）出去。缺省情况下，系统本身对此状态不做处理。
       6进入kAuthenticatingRequest状态，调用二次开发人员加入的安全模块，主要用于客户身份验证以及其他如规则的处理。读者如果希望开发具有商业用途的流式媒体服务器，该模块必须进行二次开发。
       7进入kPreprocessingRequest和kProcessingRequest及kPostProcessingRequest状态，这三种状态都是通过调用系统自带或二次开发人员添加的Module来处理RTSP请求，例如系统提供了QTSSReflector Module、QTSSSplitter Module以及QTSSFile Module等模块。其中比较重要的QTSSFile Module属于QTLib库的部分，此处不再详述。
       8进入kSendingResponse状态，用于发送对客户RTSP请求处理完成之后的响应。系统在该状态调用了fOutputStream.Flush()函数将在fOutputStream中尚未发出的请求响应通过Socket端口完全发送出去。
       9进入kCleaningUp状态，清除所有上次处理的数据，并将状态设置为kReadingRequest等待下次请求到达。
       RTSPSession的主流程分析完了，但辅助其操作的多个RTSP类还需要读者自行分析，它们分别是：RTSPSessionInterface Class、RTSPRequest Class、RTSPRequestInterface Class、RTSPRequestStream Class以及RTSPResponseStream Class等等。
4.2 RTP子系统
       RTP标准是实时传输协议（Real-Time Transfer Protocol）的简称，它被客户和流式媒体服务器用来处理流式媒体数据的传输。在介绍RTSP的运行流程时，我们发现RTSPSession对象通过调用SetupRequest函数为客户建立RTPSession对象。RTPSession类是Task类的子类，因此它重载了Task类的Run函数，该函数通过调用FileModule.cpp文件中的SendPacket()函数向客户发送RTP协议打包的流式媒体数据。当客户通过利用RTSP向RTSPSession对象发出PLAY命令后，RTSPSession对象将调用RTPSession对象的Play()函数。Play函数准备好需要打包发送的数据后，利用Task类的Signal函数传信RTPSession对象，使其被加入某个TaskThread的任务队列，从而运行其Run函数。
另外，对同一个节目中的每一个独立的RTP流（如音频流或视频流等），DSS都定义了一个RTPStream类与之对应；显然一个RTPSession对象可能包含多个RTPStream对象。整个RTP子系统的核心运行流程见图8。
       下面，我们首先分析RTPSession中Run()函数的用法：
       SInt64 RTPSession::Run()
{ //提示：该函数代码在TaskThread内运行
1     EventFlags events = this->GetEvents()； //取出事件
2     QTSS_RoleParams theParams；
       //提供给其他Module运行的参数，第一个成员是对象本身
       3     theParams.clientSessionClosingParams.inClientSession = this；        
       //设定自己为当前运行的线程
       4     OSThread::GetCurrent()->SetThreadData(&fModuleState)；
              /*如果事件是通知RTPSession对象死亡，就准备自杀。可能导致这种情况的有两种事件：自杀kKillEvent；超时kTimeoutEvent*/
       5     if ((events & Task::kKillEvent) || (events & Task::kTimeoutEvent) || (fModuleDoingAsyncStuff))
       6     {     //处理对象自杀代码，此处略…
       7            return –1；     //返回出错信息，这样析构函数就会被调用，从而让对象完全死亡
       8     }
              //如果正处于暂停（PAUSE）状态，什么都不做就返回，等待PLAY命令
       9     if ((fState == qtssPausedState) || (fModule == NULL))
       10           return 0；

              //下面代码负责发送数据
       11    {     //对Session互斥量加锁，防止发送数据过程中RTSP请求到来
       12           OSMutexLocker locker(&fSessionMutex)；
                     //设定数据包发送时间，防止被提前发送
       13           theParams.rtpSendPacketsParams.inCurrentTime = OS::Milliseconds()；
       14           if (fPlayTime > theParams.rtpSendPacketsParams.inCurrentTime) //未到发送时间
       15                  theParams.rtpSendPacketsParams.outNextPacketTime=fPlayTime- theParams.rtpSendPacketsParams.inCurrentTime； //计算还需多长时间才可运行
       16           else
       17           {     //下次运行时间的缺缺省值为0
       18                  theParams.rtpSendPacketsParams.outNextPacketTime = 0；
                     // 设置Module状态
       19                  fModuleState.eventRequested = false；
       20                  Assert(fModule != NULL)；
                            //调用QTSS_RTPSendPackets_Role内的函数发送数据，见FileModule.cpp
       21                  (void)fModule->CallDispatch(QTSS_RTPSendPackets_Role, &theParams)；
                            //将返回值从负数改为0，否则任务对象就会被TaskThread删除
       22                  if (theParams.rtpSendPacketsParams.outNextPacketTime        23                         theParams.rtpSendPacketsParams.outNextPacketTime = 0；
       24           }
       25    }
              //返回下一次希望被运行的时间；返回值含义见前文的分析
       26    return theParams.rtpSendPacketsParams.outNextPacketTime；
}
       从上面分析可见，正常状态下Run函数的返回值有两种：如果返回值为正数，代表下一次发送数据包的时间，规定时间到来的时候，TaskThread线程会自动调用Run函数；如果返回值等于0，在下次任何事件发生时，Run函数就会被调用，这种情况往往发生在所有数据都已经发送完成或者该RTPSession对象将要被杀死的时候。
       在第21行我们看到，Run函数调用了QTSSFileModule中的QTSS_RTPSendPackets_Role发送数据。在QTSSFileModule.cpp文件的QTSSFileModule_Main函数内，系统又调用了SendPackets函数，这才是真正发送RTP数据包的函数，我们对其代码分析如下：
       QTSS_Error SendPackets(QTSS_RTPSendPackets_Params* inParams)
{
              //此处略去部分定义…
       //得到要发送数据的FileSession对象，其定义见QTSSFileModule.cpp文件
       1     FileSession** theFile = NULL；
       2     UInt32 theLen = 0；
       3     QTSS_Error theErr = QTSS_GetValuePtr(inParams->inClientSession, sFileSessionAttr, 0, (void**)&theFile, &theLen)； 
       4     if ((theErr != QTSS_NoErr) || (theLen != sizeof(FileSession*))) //出错
       5     {     //设定出错原因，然后断掉连接，并返回
       6            QTSS_CliSesTeardownReason reason = qtssCliSesTearDownServerInternalErr；
       7            (void) QTSS_SetValue(inParams->inClientSession, qtssCliTeardownReason, 0, &reason, sizeof(reason))；
       8            (void)QTSS_Teardown(inParams->inClientSession)；
       9            return QTSS_RequestFailed；
       10    }
       //该节目文件中音频所能忍受的最大延迟
       11    maxDelayToleranceForStream = (*theFile)->fMaxAudioDelayTolerance；

       12    while (true)
       13    {     
                     //不存在待发送数据包，可能是文件尚未打开
       14           if ((*theFile)->fNextPacket == NULL)
       15           {
       16                  void* theCookie = NULL；
                            //获得第一个数据包，theTransmitTime为传输数据花费的时间
       17                  Float64 theTransmitTime = (*theFile)->fFile.GetNextPacket(&(*theFile)->fNextPacket, &(*theFile)->fNextPacketLen, &theCookie)；
       18                  if ( QTRTPFile::errNoError != (*theFile)->fFile.Error() )
                            {//读数据出错，断掉连接，返回。此处略 }
                            …
       19                  (*theFile)->fStream = (QTSS_RTPStreamObject)theCookie； //得到RTPStream对象
       20                  (*theFile)->fPacketPlayTime = (*theFile)->fAdjustedPlayTime + ((SInt64)(theTransmitTime * 1000))； //推迟theTransmitTime长度的播放时间
       21                  (*theFile)->fPacketWasJustFetched = true；       
       22                  if ((*theFile)->fNextPacket != NULL)
       23                  {     // 判断流格式
       24                         QTSS_RTPPayloadType* thePayloadType = NULL；
       25                         QTSS_Error theErr = QTSS_GetValuePtr( (*theFile)->fStream, qtssRTPStrPayloadType, 0, (void**)&thePayloadType, &theLen )；
                                   //设定视频流可忍受的最大延迟时间
       26                         if (*thePayloadType == qtssVideoPayloadType)
       27                         maxDelayToleranceForStream = (*theFile)->fMaxVideoDelayTolerance；
       28                  }
       29           }

                     //仍无数据，说明所有数据已经传输完成了
       30           if ((*theFile)->fNextPacket = NULL)
       31           {     //向fStream中写入长度为0的空数据，以便强制缓冲区刷新
       32                  (void)QTSS_Write((*theFile)->fStream, NULL, 0, NULL, qtssWriteFlagsIsRTP)；
       33                  inParams->outNextPacketTime = qtssDontCallSendPacketsAgain；
       34                  return QTSS_NoErr； //完成任务返回
       35           }
                     //提示：开始发送RTP数据包
                     //计算当前时间和该段数据应该发送的时间之间的相对间隔
       36           SInt64 theRelativePacketTime = (*theFile)->fPacketPlayTime - inParams->inCurrentTime；  // inCurrentTime = OS::Milliseconds()；

       37           SInt32 currentDelay = theRelativePacketTime * -1L； //计算传输延迟
       38           theErr =  QTSS_SetValue( (*theFile)->fStream, qtssRTPStrCurrentPacketDelay, 0, ¤tDelay, sizeof(currentDelay) )； //保存该延迟
                     //如果延迟过大，就丢弃该包，等待发送下一个数据包
       39           if (theRelativePacketTime > sMaxAdvSendTimeInMsec)
       40           {
       41                  Assert( theRelativePacketTime > 0 )；
       42                  inParams->outNextPacketTime = theRelativePacketTime；
       43                  return QTSS_NoErr；
       44           }
                     //此处略去部分处理视频质量的代码…
                     // 发送当前数据包
       45           QTSS_Error writeErr = QTSS_Write((*theFile)->fStream, (*theFile)->fNextPacket, (*theFile)->fNextPacketLen, NULL, qtssWriteFlagsIsRTP)；

//其余代码略…
}
       RTP子系统是DSS中最为复杂的部分之一，这是因为发送RTP数据包的过程不但涉及到网络接口，而且和文件系统有着密切的关系。DSS的一个重要特征就是能够将线索化（Hinted）过的QuickTime文件通过RTSP和RTP协议流化出去。所有分析这些文件的代码都被提取出来并且封装在QTFile库中。这种封装方式使得系统的各个部分都变得简单：QTFile负责处理文件的分析；而DSS其他部分负责处理网络和协议。服务器中的RTPFileModule调用QTFile库检索索引过的QuickTime文件的数据包和元数据。QTFile库的讲解超出了本文的范围，但是希望让DSS支持其他媒体格式的读者能够掌握它的实现机制。
5  DSS二次开发接口：Module开发流程
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
{     //根据传入的任务名称和入参执行相应的处理函数
       switch (inRole)      //任务名称
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
              QTSS_Register_Params                             regParams；
              QTSS_Initialize_Params                            initParams；
              QTSS_ErrorLog_Params                           errorParams；
              //此处略去其他多个数据结构…
} QTSS_RoleParams, *QTSS_RoleParamPtr；
       DSS提供了两种方式把我们自己开发的Module添加到服务器中：一种称为静态模块（Static Module），该方式将我们开发的Module代码直接编译到内核中去；另一种称为动态模块（Dynamic Module），该方式将我们开发的Module单独编译称为一个动态库，然后修改配置，使服务器在启动时将其加载。图9描述了DSS启动和关闭时模块调用流程。
       当服务器启动时，它首先装载没有被编译进内核的动态模块，然后才装载被编译进内核的静态模块；由于现有的大部分系统功能都是以静态模块的方式存在的，如果你希望用自己的模块替换某个系统功能，最好是编写一个动态模块，因为它们将早于静态模块被装载。
       无论是静态模块还是动态模块，它们的代码都是相同的，唯一的不同就是它们的编译方式。首先为了将静态模块编译到服务器中，我们必须修改QTSServer.cpp文件中的QTSServer::LoadCompiledInModules，并向其中加入以下代码：
       QTSSModule*       myModule=new QTSSModule(*_XYZ_*)；
       (void)myModule->Initialize(&sCallbacks,&_XYZMAIN_)；
       (void)AddModule(MyModule)；
       其中，XYZ是静态模块的名字，而XYZMAIN则是其主函数入口。
       动态模块的编译方法如下：首先单独编译动态模块为一个动态共享库；将该共享库与QTSS API stub library链接到一起；最后将结果文件放置到/usr/sbin/QTSSModules目录中去。此后，服务器在启动时就将自动调用该动态模块。














