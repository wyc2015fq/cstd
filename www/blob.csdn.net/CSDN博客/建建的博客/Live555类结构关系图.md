# Live555类结构关系图 - 建建的博客 - CSDN博客
2017年03月01日 17:03:08[纪建](https://me.csdn.net/u013898698)阅读数：789
先对live555有一个宏观的认识，为今后的实战打个基础。
liveMedia项目的源代码包含四个主要的库，各种測试代码以及IVE555 Media Server。四个主要的库各自是UsageEnvironment&TaskScheduler，groupsock，liveMedia，BasicUsageEnvironment。
这几个类都是整个系统的基础功能类,这些都是抽象类，在应用程序中基于这些类实现自己的子类。
UsageEnvironment:代表了整个系统运行的环境，它提供了错误记录、报告和log输出的功能，不管哪一个类要输出错误，就须要保存UsageEnvironment的指针(比如:能够通过  
```cpp
UsageEnvironment* env;
UsageEnvironment* env =BasicUsageEnvironment::createNew(*scheduler);
*env << "Play this stream usingthe URL \"
```
这样的操作能够实现log的输出。
TaskScheduler:则提供了任务调度功能．整个程序的运行发动机就是它，它调度任务。运行任务（任务就是一个函数）．TaskScheduler因为在全局中仅仅有一个，所以保存在了UsageEnvironment中．而全部的类又都保存了UsageEnvironment的指针。所以谁想把自己的任务增加调度中，那是非常easy的．
在此还看到一个结论：整个live555（服务端）仅仅有一个线程．当然如今有网友已经实现了服务端的多线程.
Groupsock：类是对网络接口的封装。用于收发数据包。
这个是放在单独的库Groupsock中。它封装了socket操作，增加了多播放支持和一对多单播的功能．同一时候支持UDP和TCP协议传输，两种方式可供选择。它管理着一个本地socket和多个目的地址，仅仅需知道对方地址和port就可以发送数据。Groupsock的构造函数有一个參数是struct in_addr const& groupAddr，在构造函数中首先会调用父类构造函数创建socket对象，然后推断这个地址。若是多播地址，则增加多播组。
Groupsock的两个成员变量destRecord* fDests和DirectedNetInterfaceSet fMembers都表示目的地址集和。但我始终看不出DirectedNetInterfaceSet fMembers有什么用，且DirectedNetInterfaceSet是一个没有被继承的虚类，看起来fMembers没有什么用。仅fDesk也够用了，在addDestination()和removeDestination()函数中就是操作fDesk。增加或删除目的地址。
BasicUsageEnvironment：主要是针对简单的控制台应用程序，利用select实现事件获取和处理。
HashTable：实现了哈稀表． 
DelayQueue：译为＂延迟队列＂，它是一个队列，每一项代表了一个要调度的任务（在它的fToken变量中保存）．同一时候保存了这个任务离运行时间点的剩余时间．能够预见，它就是在TaskScheduler中用于管理调度任务的东西．注意，此队列中的任务仅仅被运行一次！运行完后这一项即被抛弃！ 
HandlerSet：Handler集合．Handler是什么呢？它是一种专门用于运行socket操作的任务（函数），HandlerSet被TaskScheduler用来管理全部的socket任务（增删改查）．所以TaskScheduler中如今已调度两种任务了：socket任务（handlerSet）和延迟任务(DelayQueue)．事实上TaskScheduler还调度第三种任务：Event，后面再说．
以上各关系见例如以下UML图：
![](https://img-blog.csdn.net/20140723125923078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1a3VuMzIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
liveMedia: 库中有一系列类，基类是Medium，这些类针对不同的流媒体类型和编码。
这里还要再说明一下Sink  source，这两者的概念及关系：
liveMedia库中Sink就是消费数据的对象，比方把接收到的数据存储到文件，这个文件就是一个Sink。
Source就是生产数据的对象。比方通过 RTP读取数据。数据流经过多个source和sinks。
继承Medium基类的类，及他们间的关系见例如以下UML图：
![](https://img-blog.csdn.net/20140723131602098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1a3VuMzIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140723131734139?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1a3VuMzIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140723131925741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1a3VuMzIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
到此，我们已经对Live555基类和框架有了一个初步的认识。这对我们今后将live555灵活运用到实际项目中起了至关关键的数据。
