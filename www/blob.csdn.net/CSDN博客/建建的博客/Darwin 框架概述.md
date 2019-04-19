# Darwin  框架概述 - 建建的博客 - CSDN博客
2017年06月23日 17:16:50[纪建](https://me.csdn.net/u013898698)阅读数：1175
转自： [http://www.rosoo.net/a/200904/7285.html](http://www.rosoo.net/a/200904/7285.html)
1. 前言
Darwin Streaming Server是Apple公司提供的开源实时流媒体播放服务器程序。整个程序使用C++编写，在设计上遵循高性能，简单，模块化等程序设计原则，务求做到程序高效，可扩充性好。
本文简述了程序的整个结构，目的是为了以后阅读及修改程序的方便。
解开程序，程序的主要目录结构及作用如下
APIModules/ 模块程序的目录
APIStubLib/ 程序公共接口类目录
CommonUtilitiesLib/ 通用库
Server.tproj/ 主程序目录
QTFileLib/ mov, MP4文件读写库
RTSPClientLib/ RTSP客户端协议库
RTCPUtilitiesLib/ RTCP协议库
PlaylistBroadcaster.tproj/ MP4播放列表广播器
2. 重要基类结构
每个C++程序总有自己的基本类库结构，这种结构一般是程序的工具类，及基本接口类。
在Darwin Streaming Server中CommonUtilitiesLib目录下是放着程序的基本类库结构
主要重要的是：
Task类，TaskThread类，TaskThreadPool类：这三个类封装了线程库。三个类相互作用，通过Friend Class,使表现在外端的只有Task类，在这里，我们可以这样看：Task就是CPU使用片断，TaskThreadPool是CPU管理程序，TaskThread就是CPU。这样就使我们只需关心Task类就可，至于怎么调度，这是TaskThread及TaskThreadPool的事了。
下面我们来看这三个类是怎么互相作用从而形成一个整体的。
a. Virtual TaskThread::Entry():这个函数是整个线程函数的第一个执行函数，线程入口函数是TaskThread 从父类继承过来的Static OSThread::_Entry()，这个静态函数的唯一作用是执行Entry这个函数，这时的this指针是传进来的参数。这样就实现了线程的类封装。TaskThread::Entry（）函数是一个循环，它不停的调用本身的WaitForTask函数，从自己内部的Queue中取得要运行的Task类指针。然后运行其中的Task:::Run（）函数。
b. Virtual TaskThread::WaitForTask() 这个函数负责不停的从内部的队列中取到Task才返回。
c. static TaskThread **TaskThreadPool:: sTaskThreadArray. 这个变量是一个很重要的变量，通过它，我们就能在线程外与线程内通信，（要知道TaskThread的指针在线程类就是this指针，在线程外就是sTaskThreadArray的里面放的值。这样的话，我们就可以通过这个指针（或者说死了，也就是这个队列）来进行通讯。
d. void Task::Signal() 这个函数就是我们要往TaskThread队列里面加Task的函数.这个函数直接访问TaskThreadPool::sTaskThreadArray指针，并且因此直接访问到TaskThread的内部队列变量。
e. Virtual int Task::Run() 这个函数就是我们程序要运行的任务，也就是程序要在这里做什么。在这里程序的返回值是一个重要的信息，当返回-1时程序会直接删除该Task指针，当返回一个正数n 时线程会在 n毫秒后再次调用这个Task的Run函数，当返回0时，线程并不删掉这个Task指针，只是接着执行下一个Task.
(注意; 在我们继承Task类，而重写Run虚函数时，我们第一件要作的事情是运行Task类的GetEvent（）函数。)
在Task，TaskThread, TaskThreadPool 三个类的结构中，通过友元包装了TaskThread.TaskThreadPool，只留下了Task给我们重写，这是一种类的重要组合方法。
Task (OutThread) <-- Signal(function) ----- TaskThreadPool(OutThread) --> TaskThread（InThread）
EventContext类, EventThread类：
这两个类也像上面的类一样，组合成一个事件发生及线程的包装类。我们可以把它看作一个Select的封装调用。这个线程就是专门用来监测事件的发生的（当然也可以直接在这里处理罗）。类的框架及主要函数如下
a. Virtual void EventThread::Entry(): 这个就是线程运行空间函数。 这个函数就是循环调用Select等待事件的发生。当事件发生时，它从内部Table中取得它的指针（这些指针都是在请求事件发生是调用函[FS:PAGE]数注册进去的，这种指针必须是EventContext的指针。也就是说事件处理类必须是EventContext的父类）。当取得EventContext的指针后，线程接着调用EventContext->ProcessEvent().
b. Virtual void EventContext::ProcessEvent(): 这个就是处理事件函数，这个处理事件函数的缺省调用就是掉用本身的Task *EventContext::fTask 变量的Signal函数。这是通过构建或EventContext::SetTask（）函数设的，联想到上面的Task,TaskThread,TaskThreadPool三者的关系，这个Signal函数就是将运行处理函数交给别的线程处理的唯一函数，但是你也可以改写掉这个函数，可以让他直接在这个线程处理就完了。当然这样会大大降低响应的速度。看到这里，整个程序的线程调度框架你大概可以猜出来了吧。
c. Void EventContext::RequestEvent(): 这个函数其实是事件请求函数，通过这个函数我们就相当于表达这样一种意思，当我们请求的事件发生的时候，请调用我们的ProcessEvent函数，也就是调用运行我们重写的Task::Run()函数。
整个事件的处理流程是：
I: EventContext::RequestEvent()
II: EventThread::Entry() 
III:当事件触发时：EventContext::ProcessEvent()
IIII: Task::Signal();
IIIII: Task::Run();
注：I,II, III，IIII是在监听线程中发生的，而IIIII是在处理线程中发生的。
d. 这样的话，我们就实现了专用线程select函数的封装调用。
TCPListenerSocket类：这个类其实就是上面EventThread的衍生使用，他专门用于包装Accept（）调用。当Accept发生时，也就是select Read事件的促发。他产生一个TCPSocket类并调用本身的Virtual TCPListenerSocket::GetSessionTask（）（这是一个纯虚函数）产生一个Task处理类，并且帮他注册一个kRead事件。
1. 程序框架结构
程序的基本框架：
a. 从线程划分来讲：程序可以理解为共有5个线程
（在[Linux](http://lib.csdn.net/base/linux)下看起来是6个，因为[linux](http://lib.csdn.net/base/linux)是进程模拟线程，所以其中有一个是管理线程）。
(1) EventMoitor线程：这就是我刚才在EventContext,EventThread结构中讲的EventThread线程，他的主要功能就是监视事件的发生，当事件发生时调用EventContext的ProcessEvent（）函数，（也就是调用Task::Signal（）函数），把任务加入到线程池TaskThreadPool处理中去，TaskThread会调用Task：：Run()自动处理的。同时这个线程也负责了Accept调用，TCPLisnterSocket（TCPListenerSocket类本身也是一个Socket类）也加入到这个调用中去，当kRead事件发生时，ProcessEvent()调用GetSessionTask（）产生一个Task子类和Socket子类。并将Task类加入到这个Socket类中（顺便说一下，这个Socket类是继承EventContext类的，他本身有一个socketfd，因此它本质上是围绕select调用而使用的。）同时为这个Socket类注册kRead事件（其实就是将描述符加入到select
 的ReadSet中去）。
（注：当你看ProcessEvent（）函数时，你可能会以为ProcessEvent是一个死循环.
其实这里并不是一个死循环，我们需要注意的是在程序中所有的Socket都是UnBlock方式，所以当再没有连接时，这个循环会从Break这个地方跳出去。追求每一点都是Unblock的，让程序不在任何一点阻塞，这是这个程序的一个很鲜明的特点。我们用线程替代进程，使用Select调用，追求的难道不是这一点吗。当处理繁忙时，把每一个CPU时间片都用在处理任务，这就是最高效的服务器程序所追求的。无点阻塞，这是程序所追求的，但也很可能形成这样一种情况，在空闲时，程序会形成死循环，以至于这个线程占用CPU过多，导致CPU负载过高，这个问题程序用了两个方法来解决，
1.在处理完一系列事件后，使用ThreadYield（）调用让出CPU时间。（这种方法可能在有些系统不能使用，例如Linux就是无效的，因为在Linux线程本质上就是线程，它[FS:PAGE]得到调度的机会和其他进程一样。因此只有在某些协作型的线程，例如Solaris的线程库，不将线程等同于进程来调度时（可以在程序中设置），即，线程和进程调度优先级不一样时（也就是所有的线程共享一个进程的CPU时间片），这样这个调用才真正有效。）
2。就是Select调用的Timeout参数。
(1) IdleTask线程：这是一个空闲线程，这相当于让CPU去干别的事情，他是通过OsCond和OsMutex来完成这个功能的（在不同的*作系统有不同的实现，例如在标准PosixThread是pthread_cond_wait,）。我们可以通过IdleTask:: SetIdleTimer（）函数来实现这个定时任务。在到时间了才唤醒，这样的话，当一个Task子类，例如TCPListenerSocket类它多重继承了TCPSocket类和IdleTask,当这个线程要等一段时间再执行时。TCPListenerSocket就调用TCPListnerSocket::SetIdleTimer(),即IdleTask::SetIdleTimer(),来实现一段定时。当前线程是被IdleThread这个线程阻塞的。（至于程序为什么这样写，我猜想是为了是方便的实现一种定时机制及让出CPU时间，还有为什么要为了这个而单独产生一个线程，这也可能是为了使用的方便吧！）
(1) TaskThread线程：终于进入了真正干事的进程了。这个就是所有Task的执行线程了，他的作用就是不停的执行Task（）里面的Run()函数，根据Run函数的返回值，再决定对Task如何处理。在程序的框架中，对于单个CPU，TaskThreadPool只产生一个线程，也就是一个TaskThread线程。（多个线程会产生上下文切换的问题，）但对于多个CPU则产生多个TaskThread线程（这点大家都明白吧）。（注：其实不同的问题有不同的解法，对于不会阻塞I/O上的程序我认为这一点是好的，越少的线程就意味着越少的上下文切换，也就意味这CPU化更多的时间在干正事上，但当线程中可能有单点阻塞时，这就在一定程度上会阻碍其他Task的执行，所以在写程序的时候一定要注意不能在任何一点阻塞）。因为在线程中要涉及文件读写I/O,所以我对在这里单CPU采用单线程在这里这一点持保留意见。不过作者在这里也考虑到了这个问题，你还记得Task：：Run()函数的返回值吗，当为正数n时，这就相当于一个在n毫秒之后再次调用，这在一定程度上可以减缓线程的阻塞，但这个n值实在是很难确定的。如果不这样做，那应该怎么设计呢？一个连接一个线程，这肯定是不可取的，在连接数少的情况下，这的确是很好的，但连接数一大也不行了，线程池应该是最好的选择，当也许多个线程的线程池也许更好，但也不是越多越好，这个数字的确定，我觉得应该是动态的，而不是静态的，就像apache那样）。
注：系统也可以安排多个TaskThread线程来处理流任务。
(2) 主进程：这是一个进程。你想要它干什么就干什么。在程序中这是一个监视进程，在这里主要是纪录一些系统信息。
(3) Wait进程：这个进程就是等待主进程的返回，没什么意义。
b. 从类的结构来划分：
程序的主要结构是：QTSServer这个类就是程序的主运行类。在Darwin Streaming Server程序中基本上所有的类都继承于QTSSDictionary类。QTSSDictionary这个类基本上它就是提供这样一个功能，它提供了像QTSSDictionary::GetValue(),QTSSDictionary::SetValue()，QTSSDictionary::AddInstanceAttribute(), QTSSDictionary::RemoveInstanceAtrribute()这样的函数，目的是为了提供一种动态实现属性的功能（即我们可以在任何时候加入这个类的成员变量或去掉这个类的成员变量。）。他在内部其实是通过一个复杂的类嵌套来实现的。（其实我觉得很奇怪，它为什么要用这么复杂的嵌套结构来实现这样一种功能呢？）。通过继承这个类，我们就可以得到动态属性的功能。
所以QTSServer的继承关系是这样的
QTSServer --> QTSServerInterface -->QTSSDictionary --> QTSSt[FS:PAGE]ream
在四层结构中：QTSServerInterface类是其实是QTSServer类的补充，它提供了一个Server所具有的所有成员变量。其实我们在这里可以将它看成是QTSServer的一个补充。在这里千万不要被QTSServerInterface这个名字给迷惑了，本来这两个类是可以合并成一个类，但作者可能基于类结构清晰的原因，而将它拆成了两层结构。
另外还具有重要意义的类还有两个，RTSPSession类，RTSPRequest类，RTPSession类。RTSPSession与RTSPRequest和RTPSession这三者的关系是通过复合的形式结合在一起的，即RTSPRequest和RTPSession是RTSPSession类的成员变量.
(在这里有必要讲一下整个流媒体播放器与服务器的交互过程。
1．Client Player ----------RTSP (TCP a) ------------> Streaming Server,
2．Client Player <-------RTSP(TCP a) --------------- Streaming Server.
3．Client Player -------- RTP(UDP b) --------------> Streaming Server.
4．Client Player ---------RTCP(UDP c) --------------> Streaming Server.
5．Client Player -------- RTP(UDP d) --------------> Streaming Server.
6．Client Player ---------RTCP(UDP e) --------------> Streaming Server.
c. 从重要函数上划分：
在Darwin Streaming Server中最重要的函数莫过于RTSPSession：：Run()这个函数。这个函数将负责RTSP命令的处理流程，这个函数非常长，它使用了Switch case语句降程序划分为好几个阶段，它使用设值的办法是程序跳过某些阶段，而又预示着程序下一步该执行什么阶段。（我个人觉得这是一个很好的处理方法，通过程序分段，也就实现了模块化的结构，也就是程序后面编写第三方模块的接口，这是后话）。如果你想了解程序在那个阶段应该执行什么，你应该好好的看看这个函数。这个RTSPSession类的产生就是通过我们前面讲的TCPListenerSocket::GetSessionTask()这个函数产生的。
其它重要的函数：
QTSServer::LoadCompiledInModules() 这是挂载函数模块的地方。
4. 模块程序编写
第三方程序的编写。在Darwin Streaming Server中，模块是分类型的，即在RTSPSessin::Run()函数的不同的阶段，模块的函数会得到调用（如果你注册了这个事件的话）。（略）
模块程序编写中最重要的一个问题是：不能在模块中添加有阻塞的处理过程。
5. 总结
总体来说，整个Darwin Streaming Server的程序结构还是比较清晰，在程序设计时很注重程序的性能，在类结构程次上，很注意类的封装，即组合，通常会把几个类*在一起来实现一个功能，当对外通常只提供一个外部接口（不一定是类或函数），这通常是通过友元类和静态变量来实现的。例如（Task, TaskThread,TaskThreadPool这三者的关系，QTSSDictionary，QTSSDictionaryMan这两者的关系等）。通过几个Task，EventContext就形成了整个程序的高效运行框架，这一点是很好的。
[](http://blog.csdn.net/fanbird2008/article/details/8988498#)[](http://blog.csdn.net/fanbird2008/article/details/8988498#)[](http://blog.csdn.net/fanbird2008/article/details/8988498#)[](http://blog.csdn.net/fanbird2008/article/details/8988498#)[](http://blog.csdn.net/fanbird2008/article/details/8988498#)[](http://blog.csdn.net/fanbird2008/article/details/8988498#)
- 
顶
0- 
踩
