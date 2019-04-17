# Apple公司Darwin流式服务器源代码分析(三) - 一世豁然的专栏 - CSDN博客





2016年10月25日 19:32:16[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1164








本博客转载于：[http://www.cublog.cn/u/11445/showart.php?id=225276](http://www.cublog.cn/u/11445/showart.php?id=225276)





3.2   Tasks类
因为服务器从整体上采用了异步的运行模式，这就需要一种用于事件通信的机制。举例来说：一个RTSP连接对应的Socket端口监测到网络上有数据到达，此时必须有一个模块（或代码）被通知（notify）去处理这些数据。为此，DSS定义了Task及其相关类作为实现这一通信机制的核心。
在Task.h/cpp文件中，定义了三个主要的类，分别是：任务线程池类（TaskThreadPool Class）、任务线程类（TaskThread Class）以及任务类（Task
 Class）。
每个Task对象有两个主要的方法：Signal和Run。当服务器希望发送一个事件给某个Task对象时，就会调用Signal()方法；而Run()方法是在Task对象获得处理该事件的时间片后运行的，服务器中的大部分工作都是在不同Task对象的Run()函数中进行的。每个Task对象的目标就是利用很小的且不会阻塞的时间片完成服务器指定某个工作。
任务线程类是上文介绍的OSThread类的一个子类，代表专门用于运行任务类的一个线程。在每个任务线程对象内部都有一个OSQueue_Blocking类型的任务队列，存储该线程需要执行的任务。后面的分析可以看到，服务器调用一个任务的Signal函数，实际上就是将该任务加入到某个任务线程类的任务队列中去。另外，为了统一管理这些任务线程，DSS还开发了任务线程池类，该类负责生成、删除以及维护内部的任务线程列表。图4描述了任务类的运行。
下面我们首先分析TashThread类，该类的定义如下：
class TaskThread : public OSThread     //OSThread的子类
{     //提示：所有的Task对象都将在TaskThread中运行
       1     public:

       2     TaskThread() :       OSThread(), fTaskThreadPoolElem(this){}  //构造函数
3     virtual                   ~TaskThread() { this->StopAndWaitForThread()； } //析构函数

       4     private:

              …

       5     virtual void     Entry()；       //从OSThread重载的执行函数，仍然能够被子类重载
       6     Task*                   WaitForTask()；    //检测是否有该执行的任务


       7     OSQueueElem        fTaskThreadPoolElem；       //对应的线程池对象
       8     OSHeap                        fHeap； //纪录任务运行时间的堆，用于WaitForTask函数
              /*关键数据结构：任务队列；在Task的Signal函数中直接调用fTaskQueue对象的EnQueue函数将自己加入任务队列*/

       9     OSQueue_Blocking fTaskQueue；

              //此处略…

       }
作为OSThread的子类，TaskThread重载了Entry函数，一旦TaskThread的对象被实例化，便运行该函数。Entry()函数的主要任务就是调用WaitForTask()函数监测任务队列，如果发现新任务，就在规定时间内执行；否则，就被阻塞。下面我们简要分析Entry()函数的流程：
       void TaskThread::Entry()

{

       1     Task* theTask = NULL； //空任务


       2     while (true) //线程循环执行
       3     {     //监测是否有需要执行的任务，如果有就返回该任务；否则阻塞；
       4            theTask = this->WaitForTask()；

       5            Assert(theTask != NULL)；


6            Bool16 doneProcessingEvent = false； //尚未处理事件


7            while (!doneProcessingEvent)

       8            {

       9            theTask->fUseThisThread = NULL； //
对任务的调度独立于线程
       10           SInt64 theTimeout = 0；      //Task中Run函数的返回值，重要
                     //核心部分：运行任务，根据返回值判断任务进度
       11           if (theTask->fWriteLock)

       12           {     //如果任务中有写锁，需要使用写互斥量，否则可能造成死锁
       13                  OSMutexWriteLocker mutexLocker(&TaskThreadPool::sMutexRW)；
       14                  theTimeout = theTask->Run()；   //运行任务，得到返回值
       15                  theTask->fWriteLock = false；
       16           }

       17           else

       18           {     //使用读互斥量
       19                  OSMutexReadLocker mutexLocker(&TaskThreadPool::sMutexRW)；
       20                  theTimeout = theTask->Run()；   //运行任务，得到返回值
       21           }

       22           //监测Task中Run()函数的返回值，共有三种情况
       23           //1、返回负数，表明任务已经完全结束
       24           if (theTimeout < 0)

       25           {

       26                  delete theTask；     //删除Task对象
       27                  theTask = NULL；
       28                  doneProcessingEvent = true；
       19           }

       30           //2、返回0，表明任务希望在下次传信时被再次立即执行
       31           else if (theTimeout=0)

       32           {

       33                  doneProcessingEvent = compare_and_store(Task::kAlive, 0, &theTask->fEvents)；
       34                  if (doneProcessingEvent)

       35                         theTask = NULL；

       36           }

                     //3、返回正数，表明任务希望在等待theTimeout时间后再次执行
       37           else

       38           {

                     /*将该任务加入到Heap中，并且纪录它希望等待的时间。Entry()函数将通过waitfortask()函数进行检测，如果等待的时间到了，就再次运行该任务*/

       39                  theTask->fTimerHeapElem.SetValue(OS::Milliseconds() + theTimeout)；
       40                  fHeap.Insert(&theTask->fTimerHeapElem)；
       41                  (void)atomic_or(&theTask->fEvents, Task::kIdleEvent)；//设置Idle事件
       42                  doneProcessingEvent = true；
       43           }

              //此处略…

       }
注意，如果Task的Run()函数返回值TimeOut为正数，意味着该任务是一个周期性的工作，例如发送数据的视频泵（pump），需要每隔一定时间就发出一定量的视频数据，直至整个节目结束。为此，在第38~43行，将该任务加入到堆fHeap中去，并且标记该任务下次运行的时间为TimeOut毫秒之后。将来通过调用WaitForTask()函数就能检测到该任务是否到达规定的运行时间，WaitForTask()函数的代码如下：
       Task* TaskThread::WaitForTask()

{

       1     while (true)

       2     {     //得到当前时间，该函数为静态函数，定义见OS.h

       3            SInt64 theCurrentTime = OS::Milliseconds()；

                     /*如果堆中有任务，且任务已经到执行时间，返回该任务。 PeekMin函数见OSHeap.h，窃听堆中第一个元素（但不取出）*/

4     if ((fHeap.PeekMin() != NULL) && (fHeap.PeekMin()->GetValue() <= theCurrentTime))

//从堆中取出第一个任务返回
5                   return (Task*)fHeap.ExtractMin()->GetEnclosingObject()；

              //如果堆中有任务，但是尚未到执行时间，计算需要等待的时间
       6            SInt32 theTimeout = 0；
       7            if (fHeap.PeekMin() != NULL)      //计算还需等待的时间
       8                   theTimeout = fHeap.PeekMin()->GetValue() - theCurrentTime；
       9            Assert(theTimeout >= 0)；


              //等待theTimeout时间后从堆中取出任务返回
       10           OSQueueElem* theElem = fTaskQueue.DeQueueBlocking(this, theTimeout)；
       11           if (theElem != NULL)

       12                  return (Task*)theElem->GetEnclosingObject()；
       13    }     

}
上文曾经提到，Task对象内有两个方法：Signal和Run。Run函数是一个虚函数，由Task的子类重载，它的用法我们在分析TaskThread的Entry()函数和WaitForTask()函数中已经讨论了。而另一个Signal()函数也十分重要：服务器通过调用该函数将Task加入TaskThread，并且执行Run()函数。Signal()函数的核心部分如下：
       void Task::Signal(EventFlags events)

{

              …

              // fUseThisThread用于指定该任务运行的任务线程
       1     if (fUseThisThread != NULL)       //存在指定任务线程
                     //将该任务加入到指定任务线程的任务队列中
       2            fUseThisThread->fTaskQueue.EnQueue(&fTaskQueueElem)；
              //不存在指定的任务线程，随机选择一个任务线程运行该任务
3     else

       4     {

                     //从线程池中随机选择一个任务线程
       5            unsigned int theThread = atomic_add(&sThreadPicker, 1)；
       6            theThread %= TaskThreadPool::sNumTaskThreads；
                     //将该任务加入到上面选择的任务线程的任务队列中
       7            TaskThreadPool::sTaskThreadArray[theThread]-> fTaskQueue.EnQueue (&fTaskQueueElem)；
       8            }

       }
至此我们已经将DSS的线程和任务运行机制分析完了，这种由事件去触发任务的概念已经被集成到了DSS的各个子系统中。例如，在DSS中经常将一个Task对象和一个Socket对象关联在一起，当Socket对象收到事件（通过select()函数），相对应的Task对象就会被传信（通过Signal()函数）；而包含着处理代码的Run()函数就将在某个任务线程中运行。
因此，通过使用这些Task对象，我们就可以让所有连接都使用一个线程来处理，这也是DSS的缺省配置方法。
3.3   Socket类
作为一个典型的网络服务器，DSS源代码中的Socket编程部分是其精华之一。DSS定义了一系列Socket类用于屏蔽不同平台在TCP/UDP编程接口和使用方法上的差异。DSS中的Socket类一般都采用异步模式的（即非阻塞的），而且能够向对应的Task对象传信（Signal），这点我们在上一节介绍过。Socket类中具有代表性的类是：EventContext、EventThread、Socket、UDPSocket、TCPSocket以及TCPListenerSocket等等，它们之间的继承关系见图5。





在eventcontext.h/.cpp文件中，定义了两个类：EventContext类和EventThread类。
 Event Context提供了检测Unix式的文件描述符（Socket就是一种文件描述符）产生的事件（通常是EV_RE
或 EV_WR）的能力，同时还可以传信指定的任务。EventThread类是OSThread类的子类，它本身很简单，只是重载了OSThread的纯虚函数Entry()，用以监控所有的Socket端口是否有数据到来，其代码分析如下：
       void EventThread::Entry()

{

/*该结构定义在ev.h中，记录Socket描述符和在该描述符上发生的事件*/

       1     struct eventreq theCurrentEvent；

       2     ::memset( &theCurrentEvent, '\0', sizeof(theCurrentEvent) )；   //初始化该结构


       3     while (true)

4     {

//首先监听Socket端口的事件
       5            int theErrno = EINTR；
       6            while (theErrno=EINTR)

       7            {

8     #if MACOSXEVENTQUEUE //Macos平台
       9                   int theReturnValue = waitevent(&theCurrentEvent, NULL)；
10    #else       //其他平台
              /*调用select_waitevent函数监听所有的Socket端口，直到有事件发生为止*/

       11                  int theReturnValue = select_waitevent(&theCurrentEvent, NULL)；
12    #endif     

              …

              //有事件发生，唤醒相应的Socket端口
13    if (theCurrentEvent.er_data != NULL)

       14    {

                     //通过事件中的标识找到相应的对象参考指针
       15           StrPtrLen idStr((char*)&theCurrentEvent.er_data, sizeof(theCurrentEvent.er_data))；
       16           OSRef* ref = fRefTable.Resolve(&idStr)；
       17           if (ref != NULL)

       18           {     //通过参考指针得到EventContext对象
       19                  EventContext* theContext = (EventContext*)ref->GetObject()；
                            //利用EventContext对象的ProcessEvent方法传信对应的Task

       20                  theContext->ProcessEvent(theCurrentEvent.er_eventbits)；
       21                  fRefTable.Release(ref)；       //减少引用计数
       22           }

//此处略…

}
上述代码有两点需要注意：首先在第11行，调用select_waitevent函数监听所有Socket端口的事件。该函数在Windows平台上是采用WSAAsyncSelect（异步选择）模型实现的。具体实现是：系统首先创建一个窗口类，该类专门用于接受消息；在每个Socket端口创建后，调用WSAsyncSelect函数，同时将上述窗口类的句柄作为参数传入；将来这些Socket端口有事件发生时，Windows就会自动将这些事件映射为标准的Windows消息发送给窗口类，此时select_waitevent函数通过检查消息就能够获得对应Socket端口发生的事件。对于Windows平台下Socket的异步编程技术细节请参阅《Windows网络编程技术》一书。
另外，在第20行调用的EventContext对象的ProcessEvent函数实现上很简单，只有一行代码：fTask->Signal(Task::kReadEvent)；其中fTask为该EventContext对象对应的Task对象；ProcessEvent函数向Task对象传信，以便及时处理刚刚发生的Socket事件。
与EventThread对应的EventContext对象负责维护指定的描述符，其主要函数包括InitNonBlocking、CleanUp和RequestEvent等。其中InitNonBlocking函数调用Socket
 API ioctlsocket将用户指定的描述符设置为异步，CleanUp函数用于关闭该描述符；另外，用户通过RequestEvent函数申请对该描述符中某些事件的监听，如前所述，该函数内部调用了WSAsyncSelect来实现这一功能。
       Socket Class、UDPSocket Class和TCPSocketClass三个类都是EventContext的子类，它们封装了TCP和UDP的部分实现，同时扩展了EventContext中的事件，但都没有改变其运行机制，因此此处不再详述，留给读者自行分析。我们要为大家分析的是另外一个比较复杂的Socket类TCPListenerSocket类。TCPListenerSocket用于监听TCP端口，当一个新连接请求到达后，该类将赋予这个新连接一个Socket对象和一个Task对象的配对。首先分析TCPListenerSocket类的主要定义如下：
       class TCPListenerSocket : public TCPSocket, public IdleTask

{

/*提示：该类从有两个基类，所以它既是一个事件监听者，同时也是一个任务Task。作为一个任务，给TCPListenerObject发送Kill事件就可以删除它*/

       1     public:

       2            TCPListenerSocket() :   TCPSocket(NULL, Socket::kNonBlockingSocketType), IdleTask(), fAddr(0), fPort(0), fOutOfDescriptors(false) {}  //构造函数
       3            virtual ~TCPListenerSocket() {}   //析构函数


                     //addr为地址，port为端口号，初始化函数自动监听TCP端口
       4            OS_Error              Initialize(UInt32 addr, UInt16 port)；
                     //子类必须重载该纯虚函数，用于建立新连接时生成任务对象
       5            virtual Task*   GetSessionTask(TCPSocket** outSocket) = 0；
       6            virtual SInt64  Run()；  //重载Task的Run函数，子类仍可重载


       7     private:

                     //重载EventContext的ProcessEvent函数，用于产生Socket和Task对象配对
8            virtual void ProcessEvent(int eventBits)；
       9            OS_Error       Listen(UInt32 queueLength)；
//其他略…

}
前面我们分析得知，EventContext类通过ProcessEvent函数来实现对任务的传信工作，但在TCPListenerSocket
中，ProcessEvent函数被重载用来创建Socket和Task对象得配对，该函数的实现如下：
       void TCPListenerSocket::ProcessEvent(int /*eventBits*/)

{     /*提示：该函数运行于系统唯一的EventThread线程中，所以要尽量快速，以免占用过多的系统资源*/

              //此处略去部分定义…

       1     Task* theTask = NULL；     //Task对象
       2     TCPSocket* theSocket = NULL；       //Socket对象


              //创建对象配对
       3     while (true)

       4     {     //accept连接
       5            int osSocket = accept(fFileDesc, (struct sockaddr*)&addr, &size)；

       6            if (osSocket == -1) //监听端口出错
       7            {     //此处略去出错处理     }

                     //用子类重载的GetSessionTask函数创建Task对象
       8            if ((theTask = this->GetSessionTask(&theSocket))=NULL) //创建出错
       9                   close(osSocket)；
       10           else  //创建成功，接着创建Socket对象
       11           {    


       12                  Assert(osSocket != EventContext::kInvalidFileDesc)；
                            //此处略去部分对新建连接端口的设置(setsockopt函数)

                            //创建新的Socket对象
       13                  theSocket->Set(osSocket, &addr)；
       14                  theSocket->InitNonBlocking(osSocket)； //初始化
       15                  theSocket->SetTask(theTask)； //设置对应的任务
       16           theSocket->RequestEvent(EV_RE)； //新对象监听读事件
       17           }

       18    }

              //处理完一次连接请求后，TCPListenerSocket对象还要接着监听
       19    this->RequestEvent(EV_RE)；
}
对Socket类的分析基本完成了，从中我们可以发现，DSS对于网络传信和任务调度之间的处理非常精密，环环相扣，在某种程度上甚至是有些过a于花哨。但是这些基本类是上层RTSP/RTP等服务器子系统编码的基础，因此希望读者能够从本质上掌握这些代码。




