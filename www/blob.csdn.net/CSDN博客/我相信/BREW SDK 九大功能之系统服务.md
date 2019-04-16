# BREW SDK 九大功能之系统服务 - 我相信...... - CSDN博客





2010年04月06日 11:41:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1239








BREW的系统服务包含广泛，应用程序管理，资源文件的管理，内存处理，获取设备和应用程序配置信息，BREW应用与手机之间的通信，定时器和告警等等，BREW3.X以前的版本主要是通过IShell接口来实现的。BREW3.X之后提供了IAlarmMgr增强处理告警，IAppletCtl和IAppHistory接口进一步增强了对应用程序状态的管理。

计时器由回调函数和创建计时器时提供的数据结构地址来标识。可以使用ISHELL_SetTimerEx或者是ISHELL_SetTimer;来设置定时器。如果定时期到期触发，则定时期自动取消，如果没有到时触发，则可以使用CALLBACK_Cancel和ISHELL_CancelTimer取消。定时器也可以重复触发，但是必须在定时器的回调函数里，再次设置定时器。ISHELL_GetTimerExPiration用于确定特定计时器到期之前剩余的毫秒数。定时器并不是到达指定时间立刻执行，而是必须等待当前正在执行的函数执行完毕，在其它排在定时器回调函数之前的回调执行完毕，在系统队列里的定时器回调函数才能执行。所以定时器的定时间隔可能大大延长，如果依靠定时器来做严格依赖于时间的操作，应该注意这一点。

BREW 如果长时占用CPU会导致复位手机，为了适应深度占用CPU的数**算和网络数据接收处理，BREW 2.1以上版本提供了IThread接口。Thread提供了启动、运行线程的接口和让位CPU的方法。线程函数是一个无限或者有限的循环过程，线程运算须划分成小计算单元以便循环往复，每个运算单元之后，必须调用让位函数：

            ISHELL_Resume( pIShell, pcb );

            ITHREAD_Suspend( me );

            其中 pcb 获取如下：

            IThread *me;

            AEECallback *pcb = ITHREAD_GetResumeCBK( me );

让位函数进行任务调度处理，暂停当前线程执行，切换到其它线程，同时恢复计数（避免复位手机）。编程时容易出现的问题是内存分配后忘记释放，造成内存泄露，同样，创建对象后需要释放，否则也同样会造成内存泄露。使用 IRscPool 接口，将动态分配的内存和资源统一释放，而不必逐个释放。IRscPool 是管理资源及内存分配的接，IThread 作为相对独立的模块，其内存分配和资源创建可能在一起，其释放也可能在一起的。因此线程应用场合最有可能应用IRscPool的功能。

            在BREW 中，当创建接口的时候，如果引用计数不为零，则取得已经创建的接口指针，同时引用计数加一，如果引用计数为零，则创建接口。删除接口的时候，引用计数减一，如果计数减到零，则从系统内存中卸载接口内存。BREW的所有接口都从IBase接口派生，IBase接口只有两个方法，IBaseAdRef和IBase_Release，各个派生接口的这两个方法称之为Ixxx_AddRef和IXXX_Release至于为什么名称不同能够基由相同的接口调用，将会在下文详细说明。在使用完接口以后，一定要记住使用IXXX_Release释放，否则会发生内存泄露的问题。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/03/26/5418824.aspx](http://blog.csdn.net/wireless_com/archive/2010/03/26/5418824.aspx)



