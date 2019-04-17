# 【转】Darwin Streaming Server 核心代码分析 - DoubleLi - 博客园







无意中看到了**[dqzhangp](http://blog.sina.com.cn/s/blog_6a4c492f0100witu.html)**的一篇博客，分析了DSS的核心架构，读完顿时感觉豁然开朗，茅塞顿开，写得非常的鞭辟入里，言简意赅，我想没有相当的功力是写不出这样的文章的，情不自禁转到自己空间来，生怕弄丢了。



**基本概念**



首先，我针对的代码是Darwin StreamingServer 6.0.3未经任何改动的版本。



DarwinStreaming Server从设计模式上看，采用了Reactor的并发服务器设计模式，如果对Reactor有一定的了解会有助于对DarwinStreaming Server核心代码的理解。



Reactor模式是典型的事件触发模式，当有事件发生时则完成相应的Task，Task的完成是通过调用相应的handle来实现的，对于handle的调用是由有限个数的Thread来完成的。



DarwinStreaming Server中定义了一个Task类。Task类有两个典型的方法，一个是Signal，一个是Run。调用Signal把一个Task加入到TaskThread的Task队列中，等待完成，Run就是完成一个任务的handle。基于Task类，定义了三种类型的Task，分别是IdleTask，TimeoutTask，以及普通的Task。



在Darwin StreamingServer中，除了主线程以外，有三种类型的线程，分别是TaskThread，EventThread以及IdleTaskThread：



1.    TaskThread，TaskThread通过运行Task类型对象的Run方法来完成相应Task的处理。典型的Task类型是RTSPSession和RTPSession。TaskThread的个数是可配置的，缺省情况下TaskThread的个数与处理器的个数一致。等待被TaskThread调用并运行的Task放在队列或者堆中。



2.    EventThread，EventThread负责侦听套接口事件，在DarwinStreaming Server中，有两种被侦听的事件，分别是建立RTSP连接请求的到达和RTSP请求的到达。对于RTSP连接请求的事件，EventThread建立一个RTSPSession，并启动针对相应的socket的侦听。对于RTSP请求的事件，EventThread把对应的RTSPSession类型的Task加入到TaskThread的队列中，等待RTSP请求被处理。



3.    IdleTaskThread，IdleTaskThread管理IdleTask类型对象的队列，根据预先设定的定时器触发IdleTask的调度。TCPListenerSocket就是一个IdleTask的派生类，当并发连接数达到设定的最大值时，会把派生自TCPListenerSocket的RTSPListenerSocket加入到IdleTaskThread管理的IdleTask队列中，暂时停止对RTSP端口的侦听，直到被设定好的定时器触发。



**核心架构**



下图是DarwinStreaming Server核心架构的示意图。在这个示意图中有三种类型的要素，分别是线程，Task队列或者堆，被侦听的事件。图中的文字都是从源代码中copy出来的，便于读者通过查找与源代码对应起来。



[【图片】](http://photo.blog.sina.com.cn/showpic.html#blogid=6a4c492f0100witu&url=http://s9.sinaimg.cn/orignal/6a4c492fgb408b1d1dd18)



图中给出了三个线程，分别是TaskThread::Entry，EventThread::Entry以及IdleTaskThread::Entry。前文已经对这三种线程进行了概要描述。



除了三个线程，图中还有另外五个矩形块。与TaskThread::Entry线程相关联的有两个，分别是TaskThread::fTaskQueue队列和TaskThread::fHeap堆，通过调用Signal被调度等待完成的Task就放在队列或者堆中。与IdleTaskThread::Entry线程相关联的有一个，是IdleTaskThread::IdleHeap堆。与EventThread::Entry相关联的是EventContext::fEventReq，是被侦听的端口。还有一个是TimeoutTaskThread::fQueue队列，它事实上是通过TimeoutTask与TaskThread::Entry相关联。



图中指向线程的连接线表明从队列或者堆中取出Task，而对于EventThread::Entry线程来说，则是被侦听事件的发生。指向被侦听的端口的连接线表明把端口加入侦听，指向Task的队列或堆的连接线，表明把Task加入到队列或者堆中。连接线的文字给出的是相应的函数调用，可以直接在源代码中搜索到。



**EventThread**



系统启动的时候调用QTSServer::StartTasks()把RTSP服务端口加入到侦听队列中。此时便开始接收客户端的RTSP连接请求了。



在EventThread::Entry中调用select_waitevent函数等待事件的发生，当有事件发生的时候，就通过调用ProcessEvent方法对事件进行相应的处理。注意ProcessEvent是一个虚函数，共有两个实现。EventContext类中实现了ProcessEvent方法，EventContext的派生类TCPListenerSocket中也实现了ProcessEvent方法。



对于建立RTSP连接的请求，调用TCPListenerSocket::ProcessEvent方法来处理，此方法调用RTSPListenerSocket的GetSessionTask方法建立一个RTSPSession，并把相应的套接口加入侦听队列，等待RTSP请求。然后还需调用this->RequestEvent(EV_RE)把建立RTSP连接的请求加入到侦听队列。



对于RTSP连接上的RTSP请求事件，调用的是EventContext::ProcessEvent方法，通过Task的Signal把对应的RTSPSession类型的Task加入到TaskThread::fTaskQueue中等待TaskThread处理。



**TaskThread与Task**



TaskThread::Entry调用TaskThread::WaitForTask()方法获得下一个需要处理的Task。TaskThread::WaitForTask()首先从TaskThread::fHeap中获得Task，如果TaskThread::fHeap中没有满足条件的Task，则从TaskThread::fTaskQueue中获得Task。



TaskThread::Entry调用Task::Run方法来完成对应的Task，Task::Run方法的返回值类型是SInt64，也即signedlong long int类型。TaskThread::Entry根据Task::Run方法的返回值进行不同的处理。对于小于0的返回值，需delete这个Task；对于大于0的返回值，返回值代表了下次处理这个Task需等待的时间，TaskThread::Entry调用fHeap.Insert(&theTask->fTimerHeapElem)把Task插入到堆里，并设定等待时间。对于等于0的返回值，TaskThread::Entry不再理会该Task。



**TimeoutTask**



从代码中看，TimeoutTaskThread是IdleTask的派生类，分析后发现从TimeoutTaskThread与IdleTask没有任何关系，完全可以从Task派生，修改代码后验证了这个想法。因此TimeoutTaskThread就是一个普通的Task，TimeoutTaskThread通过其Run方法监控一组超时任务，具体的比如RTSP协议或者RTP协议超时。



在系统启动的时候TimeoutTaskThread被加入到TaskThread的队列中，这是通过在StartServer函数中调用TimeoutTask::Initialize()来实现的。TimeoutTaskThread::Run函数的返回值是intervalMilli= kIntervalSeconds * 1000，也就是一个正数，于是TimeoutTaskThread这个Task会加入到TaskThread::fHeap中被周期性的调用。



TimeoutTaskThread::Run方法发现有超时的任务，则通过Signal方法调度这个Task，event为Task::kTimeoutEvent。被管理的这组任务，要有RefreshTimeout的机制。



**一次点播请求的处理**



为了更好的理解DarwinStreaming Server的架构，我们从客户端发起点播，触发服务器的建立RTSP连接事件的发生开始，看看DSS的工作流程是什么样的。



针对RTSP协议，DarwingStreaming Server在554端口上侦听，当有连接请求到达时，通过accept调用返回一个socket，对应的后续RTSP请求都是通过这个socket来传送的。我们把RTSP相关的事件分成两类，一类是RTSP连接请求，一类是RTSP请求。先来看RTSP连接请求的过程：



1.    RTSP连接请求到达后，被select_waitevent函数捕获，代码在EventContext.cpp的EventThread::Entry中232行。



2.    查找EventThread::fRefTable，获取对应的EventContext。得到的是EventContext类型的派生类RTSPListenerSocket。相应的代码在EventContext.cpp中的249到253行。



3.    调用ProcessEvent，处理事件。相应的代码在EventContext.cpp中的257行。注意，由于对应的EventContext类其实是RTSPListenerSocket，因此调用的应该是TCPListenerSocket::ProcessEvent。



4.    在TCPListenerSocket.cpp的106行TCPListenerSocket::ProcessEvent方法中，调用accept得到socket，在160行调用了GetSessionTask方法，对应的是RTSPListenerSocket::GetSessionTask，在QTSServer.cpp中定义。



5.    在RTSPListenerSocket::GetSessionTask方法中，QTSServer.cpp的1077行，调用NEWRTSPSession建立了一个新的RTSPSession。



6.    回到TCPListenerSocket.cpp文件中的TCPListenerSocket::ProcessEvent方法，注意189行，把刚刚建立好的RTSP连接加入到侦听队列，等待RTSP请求的到来。



**RTSP请求的处理流程步骤如下**，注意前面第一步是一样的：



1.    RTSP连接请求到达后，被select_waitevent函数捕获，代码在EventContext.cpp的EventThread::Entry中232行。



2.    查找EventThread::fRefTable，获取对应的EventContext。得到的是EventContext类。相应的代码在EventContext.cpp中的249到253行。



3.    调用ProcessEvent，处理事件。相应的代码在EventContext.cpp中的257行。注意，此时调用的是EventContext::ProcessEvent。



4.    EventContext::ProcessEvent方法在EventContext.h中实现，在127行。在138行调用了fTask->Signal(Task::kReadEvent)，fTask就是RTSPSession类。把RTSPSession加入到TaskThread的队列等待RTSPSession::Run()被调用。



5.    后续就是RTSPSession::Run()对RTSP请求的具体的处理。





（全文完）













