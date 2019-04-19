# Windows下一个比较完美的线程池实现 - xqhrs232的专栏 - CSDN博客
2014年09月04日 16:57:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：577
原文地址::[http://blog.csdn.net/fishjam/article/details/8632049](http://blog.csdn.net/fishjam/article/details/8632049)
相关文章
1、[求解：每个进程都有一个线程池。线程池的默认大小为每个可用处理器有
 25 个线程](http://q.cnblogs.com/q/54902/)----[http://q.cnblogs.com/q/54902/](http://q.cnblogs.com/q/54902/)
2、线程池的介绍及简单实现----[http://www.ibm.com/developerworks/cn/java/l-threadPool/](http://www.ibm.com/developerworks/cn/java/l-threadPool/)
3、[线程池](http://blog.chinaunix.net/uid-26565142-id-3184894.html) ----[http://blog.chinaunix.net/uid-26565142-id-3184894.html](http://blog.chinaunix.net/uid-26565142-id-3184894.html)
# 1.  前言
线程池不是一个新鲜的东西，网上能找到很多原理、实现，甚至很多库都提供了实现，比如微软的 ATL::CThreadPool, Vista后提供的CreateThreadpoolWork, boost 中提供的 thread_pool， CSDN、CodeProject 等网站上很多人已经实现的类。但这些库往往只支持启动任务，而不能很好地停止任务(相信很多人都会和我一样有这个需求)，于是我在FTL中写了一个个人认为还比较完美的线程池。
# 2.  功能
本线程池提供了如下功能：
1.       能根据任务个数和当前线程的多少在最小/最大线程个数之间自动调整(Vista后的系统有 SetThreadpoolThreadMaximum 等函数有类似功能)；
2.      能方便的对任一任务进行取消操作，无论该任务是等待运行状态还是正在运行状态都支持(相比较而言，WaitForThreadpoolWorkCallbacks 函数只能取消尚未运行的任务)；
3.      能对整个线程池进行安全的暂停、继续、停止处理
4.      支持回调方式的反馈通知
5.      使用模版方式实现，能方便的进行参数传递
6.      在加入任务时可以设置优先级(目前尚不支持动态调整)
7.      使用的是微软的基本API，能支持WinXP、Vista、Win7等各种操作系统(CreateThreadpoolWork 等只能在Vista后才能使用)
# 3.  UML图示和简单说明
其UML图比较简单，主要的只有三个类:
CFThreadPool – 线程池的管理类，负责整个线程池的管理工作，直接使用即可。
CFJobBase – Job的基类，如果想实现自己的Job，必须从这个类继承，并实现其中的Run/ OnCancelJob 等函数。
IFThreadPoolCallBack– 可选的回调实现，可以通知调用段 Job 的启动、停止、取消、进度通知、错误等各种状态的改变。
![](https://img-my.csdn.net/uploads/201303/03/1362325355_1320.png)
# 4.  实现说明
以下部分简单介绍了一些比较重要的代码实现，具体请参见示例代码和其中的注释部分。
# 4.1.     Job容器
本线程池类中有两种Job的容器，分别是等待运行的Jobs和当前正在运行的Jobs。因为有不同的需求，其定义分别如下(两种容器类型的选择，其理由已经写得比较详细，大家自行分析即可)
//! 保存等待Job的信息，由于有优先级的问题，而且从最前面开始取任务，因此保存成set
//! 保证优先级高、JobIndex小(同优先级时FIFO) 的Job在最前面
typedeftypenameUnreferenceLess<CFJobBase<T>* >JobBaseUnreferenceLess;
typedefstd::set<CFJobBase<T>*,JobBaseUnreferenceLess > WaitingJobContainer;
WaitingJobContainerm_WaitingJobs;    //!等待运行的Job
//! 保存运行Job的信息，由于会频繁加入、删除，且需要按照JobIndex查找删除，因此保存成map
typedefstd::map<LONG,CFJobBase<T>*
 >   DoingJobContainer;
DoingJobContainerm_DoingJobs;  //! 正在运行的Job
# 4.2.     Job优先级
代码中通过operator < 方法比较CFJobBase<T>::m_nJobPriority和 JobIndex，在SubmitJob时保证其在 set 中的顺序来保证 优先级高的Job先执行，相同优先级的Job采用FIFO方式。
# 4.3.     Job对暂停、停止的支持
为了支持暂停、停止，Job的子类必须在工作循环中调用父类提供 GetJobWaitType方法，并判断其返回值。如果返回值为ftwtStop 表示用户请求了停止，需要进行必要的清除工作。GetJobWaitType的实现请参见代码（本质是等待m_hEventJobStop, m_pThreadPool->m_hEventStop,
 m_pThreadPool->m_hEventContinue这三个手动重置事件之一）
# 4.4.     线程池对Job的控制
线程池中主要有以下的一些函数，因为意义根据名字很容易猜出，就不再详细介绍，详见示例。
Start(LONGnMinNumThreads, LONG nMaxNumThreads);
StopAndWait(DWORDdwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK);
ClearUndoWork();
SubmitJob(CFJobBase<T>*pJob, LONG* pOutJobIndex); //加入Job，会返回Job在Pool中的唯一索引，可以通过CancelJob 取消。
Pause/Resume/Stop// 对整个线程池请求暂停、继续、停止的操作。注意：需要Job子类的配合才能达成目标。
# 4.5.     资源的处理
CFJobBase子类需要 Initialize、Run、Finalize、OnCancelJob 这几个虚函数。
当Job运行的时候，其逻辑为 if(Initialize){ Run -> Finalize; }，需要在 Finalize 中释放资源；
当Job没有运行的时候就被取消或Pool停止，则会调用 OnCancelJob，需要在其中释放资源。
# 5.  示例程序
编写了简单的MFC示例程序，其界面如图所示：
因为UI不是重点，而且本人也比较懒，所以没有弄很好的UI出来，所有的运行信息请参见VisualStudio中的“输出”窗口。
Start 按钮启动线程池，示例中设置的最小、最大线程个数是 2-4(即会根据加入的Job自动在 2-4 个线程之间自动调整)。
       三个AddJob按钮分别是增加 高、普通、低优先级的Job，加入后可以在日志中查看其运行的顺序。
       两个CancelJob按钮分别是从前面、后面的JobIndex取消Job(实际上支持任和有效的JobIndex)。
![](https://img-my.csdn.net/uploads/201303/03/1362325415_5805.png)
# 6.  特别说明
# 6.1.     IFThreadPoolCallBack 回调的同步
Pool 调用 IFThreadPoolCallBack 接口的各个方法时，为了性能上的考虑，没有加锁进行同步。使用时如果有需要，最好自行同步。
# 6.2.     单任务的暂停、继续 VS Pool的暂停、继续
在开发过程中，考虑过是提供单任务的暂停、继续 还是 整个Pool 的暂停、继续，考虑到目前的需求，暂时只支持整个Pool的暂停，免得过分复杂，如有需要，大家可以自行参考CancelJob实现。
# 6.3.     可能存在的Bug和解决方法
在开发的过程中，对各个部分都进行了详细的分析、考虑和测试，应该没有较大的Bug。目前只想到一种在极端情况下，可能会造成的Bug，特提出以免各位踩雷。
描述：因为是多线程的代码，所以在极端情况下，可能出现SubmitJob函数尚未返回，对应的Job就执行完毕(或 Initialize失败直接返回)的情况。此时如果调用端采用了将 *pOutJobIndex保存起来，在 OnJobEnd 中清除的逻辑，可能会因为
 OnJobEnd 找不到对应的JobIndex 而出现逻辑错误。
解决方法：在调用 SubmitJob 的代码和OnJobEnd 的回调代码中，使用相同的锁机制保证即可(这个也应该由调用者来保证![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif))。
源码和示例程序的下载地址请参见本人的资源列表(可能需要等一段CSDN刷新的时间)。
http://download.csdn.net/detail/fishjam/5106672
