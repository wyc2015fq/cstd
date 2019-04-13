
# uC-OS-II内核架构解析---uC-OS-II系统核心 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:07:02[seven-soft](https://me.csdn.net/softn)阅读数：686



## 1. uC/OS-II任务调度
### (1) uC/OS-II调度算法
uC/OS-II采用基于优先级的调度算法，总是选择当前处于就绪状态的优先级最高的任务进行调度。uC/OS-II是可抢占性的强实时性OS，在完成中断后允许进行新的任务调度。
uC/OS-II有两种调度方式：任务级任务调度、中断级任务调度。
### (2) 任务就绪表
INT8U const OSUnMapTbl[256] = {…};
OS_EXT INT8U OSRdyGrp;
OS_EXT INT8U OSRdyTbl[OS_RDY_TBL_SIZE];
添加就绪任务至就绪表；
从就绪表删除就绪任务；
查找最高优先级就绪任务OS_SchedNew()；
### (3) 任务级任务调度
指在非中断返回时进行任务调度，一般发生在当前任务因时间延迟或等待某事件而阻塞或被挂起，或有更高优先级的任务处于就绪状态。
任务的基本信息：
CPU的PC寄存器：任务当前执行的位置；
CPU的通用寄存器：任务当前执行涉及的临时数据；
CPU的状态寄存器：存储当前CPU的状态。
任务级任务切换：从一个任务直接切换至另一个任务，不涉及CPU状态的切换，OS_TASK_SW()既保存当前任务上下文，又恢复新任务上下文。
过程：OS_Sched() -> OS_SchedNew() -> OS_TASK_SW()
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051114524015.jpg)

### (4) 中断级任务调度
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051114534579.jpg)
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051114544972.jpg)
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051114553389.jpg)
中断级任务切换：在中断处理完成后，通过OSIntExit()判断是否有更高优先级就绪任务。如果有，调用OSIntCtxSW()恢复新任务上下文。注意：在中断处理中，已经保存了被中断任务的上下文，所以这里仅仅恢复。
过程：OSIntExt() –> OSIntEnter() -> ISR –> OSIntExit() -> OSIntCtxSW()
### (5) 调度器上锁与解锁
uC/OS-II提供调度器锁定功能，在锁定期间不能进行任务调度。uC/OS-II使用全局变量OSLockNesting标识是否锁定了任务调度器。
OS_EXT INT8U OSLockNesting;
void OSSchedLock(void);
void OSSchedUnlock(void);
### (6) 中断管理函数
在中断处理中，不允许进行任务管理、事件管理及任务调度等操作。uC/OS-II通过全局变量OSIntNesting标识当前是否处于中断状态。在所有任务及事件管理的程序中，都有对OSIntNesting进行判断的语句。
void OSIntEnter(void);
void OSIntExit(void);
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051114575875.jpg)
### (7) 中断相关问题
OS_ENTER_CRITICAL()
OS_EXIT_CRITICAL()
关中断使得uC/OS-II能够同时避免有其他任务或中断服务进入临界代码段。调用uC/OS-II功能函数时，中断总应当是开着的。
uC/OS-II如何禁止调度？
在中断中允许调度吗？为什么？
uC/OS-II如何屏蔽中断？
## 2. uC/OS-II系统启动
uC/OS-II首先调用OSInit()进行初始化，然后创建任务（此时还未启动系统，仅仅为其分配资源），然后调用OSStart()启动系统，将CPU控制权交给uC/OS-II，OS根据任务优先级选择由哪个任务开始执行，或创建新的任务。
### (1) 初始化函数OSInit()
OSInit()主要完成初始化操作，包括初始化全局变量（在OS_InitMisc()中）、任务就绪表、TCB、ECB、FCB、内存单元、消息队列，并创建空闲任务。如果有必要，创建统计任务。
OS_InitMisc();                  //初始化部分全局变量
OS_InitRdyList();             //初始化任务就绪表
OS_InitTCBList();            //初始化空闲TCB链表
OS_InitEventList();          //初始化ECB链表
OS_FlagInit();                  //初始化事件组标志结构
OS_MemInit();                 //初始化内存管理
OS_QInit();                      //初始化消息队列
OS_InitTaskIdle();            //创建空闲任务
OS_InitTaskStat();            //创建统计任务
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115002566.jpg)
uC/OS-II初始化了5个空的数据结构缓冲区，每个缓冲区都是单向链表，允许uC/OS-II从缓冲区中迅速取得或释放一个缓冲区中的元素。![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115024586.jpg)
uC/OS-II调用OSInit()后的变量与数据结构如下图所示：
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051116014561.jpg)
### (2) 启动函数OSStart()
OSStart()在一切准备就绪且需要首先创建的任务都被创建后，启动uC/OS-II。它从就绪表中查找最高优先级就绪任务，并恢复其上下文开始执行。
过程：OSStart() -> OS_SchedNew() -> OSStartHighRdy()
问题：任务第一次被调用时，哪来的上下文供其恢复呢？创建任务时，调用了OSTaskStkInit()初始化任务堆栈，可此函数中没有涉及任务的上下文呀？
uC/OS-II调用OSStart()后的变量和数据结构如下图所示：
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051116040057.jpg)
### (3) 统计任务OSTaskStat
OSTaskStat用于计算CPU利用率。设置OS_CFG.H中的OS_TASK_STAT_EN为1，创建统计任务，在系统启动后一直处于就绪状 态。刚开始时，空闲任务运行1S，为计算CPU利用率提供一个基准值，并保存在统计任务的堆栈中，这个值不会改变除非重新启动CPU。此后空闲任每次被其 它任务抢去CPU时，它里面的计数器就会直接记录下CPU空闲的时间。
## 3. uC/OS-II系统时钟
任何实时系统的时钟硬件设备每隔一段时间（一个系统tick）产生一个硬件中断，OS接收到该中断后，更新时间计数器，更新所有对时钟依赖的程序代码，从而维持系统有序稳定的运行。
主要包含在C源文件OS_TIME.C中。
\#define OS_TICKS_PER_SEC 100     //系统时钟中断间隔
OS_EXT volatile INT32U OSTime;      //系统运行的时间值
void OSTimeTick(void);                         //时钟中断服务程序
void OSTimeDly(INT16U ticks);           //延迟指定时钟节拍
INT8U OSTimeDlyHMSM(…);             //延迟指定时间长度
INT8U OSTimeDlyResume(prio);  //恢复等待（时延/阻塞）任务
INT32U OSTimeGet(void);                   //读取当前时间
void OSTimeSet(INT32U ticks);            //设置当前时间
## 4. uC/OS-II事件管理
### (1)   事件控制块
INT8U OSEventType;                           //事件类型
void *OSEventPtr;                                //指向MBox或Queue
INT16U OSEventCnt;                           //信号量计数器（注：Mutex）
INT8U OSEventGrp;                            //事件等待组标志
INT8U OSEventTbl[];                          //时间任务等待表
INT8U OSEventName[];                      //时间名称
（2） ECB管理机制
OS_EXT OS_EVENT *OSEventFreeList;   //空闲ECB链表指针
OS_EXT OS_EVENT OSEventTbl[]；       //ECB结构体数组
（3） ECB管理函数
OS_InitEventList()：初始化ECB；
OS_EventWaitListInit()：在创建事件时调用，初始化ECB任务等待表；
OS_EventTaskRdy()：在事件发生时调用，修改此事件中最高优先级任务的TCB成员变量，在任务就绪表中添加此任务，将相应信息传递给该任务TCB，并将此任务从事件的任务等代表中删除；
OS_EventTaskWait()：在申请资源失败而暂停当前任务时被调用，将任务从任务就绪表中删除，并添加到事件的任务等代表中；
OS_EventTO()：在事件等待超时时调用，将此任务从事件的任务等代表中删除，并修改该任务的TCB成员变量；1. uC/OS-II任务调度
(1) uC/OS-II调度算法
uC/OS-II采用基于优先级的调度算法，总是选择当前处于就绪状态的优先级最高的任务进行调度。uC/OS-II是可抢占性的强实时性OS，在完成中断后允许进行新的任务调度。
uC/OS-II有两种调度方式：任务级任务调度、中断级任务调度。
(2) 任务就绪表
INT8U const OSUnMapTbl[256] = {…};
OS_EXT INT8U OSRdyGrp;
OS_EXT INT8U OSRdyTbl[OS_RDY_TBL_SIZE];
添加就绪任务至就绪表；
从就绪表删除就绪任务；
查找最高优先级就绪任务OS_SchedNew()；
(3) 任务级任务调度
指在非中断返回时进行任务调度，一般发生在当前任务因时间延迟或等待某事件而阻塞或被挂起，或有更高优先级的任务处于就绪状态。
任务的基本信息：
CPU的PC寄存器：任务当前执行的位置；
CPU的通用寄存器：任务当前执行涉及的临时数据；
CPU的状态寄存器：存储当前CPU的状态。
任务级任务切换：从一个任务直接切换至另一个任务，不涉及CPU状态的切换，OS_TASK_SW()既保存当前任务上下文，又恢复新任务上下文。
过程：OS_Sched() -> OS_SchedNew() -> OS_TASK_SW()
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051114524015.jpg)
(4) 中断级任务调度
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051114534579.jpg)
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051114544972.jpg)
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051114553389.jpg)
中断级任务切换：在中断处理完成后，通过OSIntExit()判断是否有更高优先级就绪任务。如果有，调用OSIntCtxSW()恢复新任务上下文。注意：在中断处理中，已经保存了被中断任务的上下文，所以这里仅仅恢复。
过程：OSIntExt() –> OSIntEnter() -> ISR –> OSIntExit() -> OSIntCtxSW()
(5) 调度器上锁与解锁
uC/OS-II提供调度器锁定功能，在锁定期间不能进行任务调度。uC/OS-II使用全局变量OSLockNesting标识是否锁定了任务调度器。
OS_EXT INT8U OSLockNesting;
void OSSchedLock(void);
void OSSchedUnlock(void);
(6) 中断管理函数
在中断处理中，不允许进行任务管理、事件管理及任务调度等操作。uC/OS-II通过全局变量OSIntNesting标识当前是否处于中断状态。在所有任务及事件管理的程序中，都有对OSIntNesting进行判断的语句。
void OSIntEnter(void);
void OSIntExit(void);
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051114575875.jpg)
(7) 中断相关问题
OS_ENTER_CRITICAL()
OS_EXIT_CRITICAL()
关中断使得uC/OS-II能够同时避免有其他任务或中断服务进入临界代码段。调用uC/OS-II功能函数时，中断总应当是开着的。
uC/OS-II如何禁止调度？
在中断中允许调度吗？为什么？
uC/OS-II如何屏蔽中断？
2. uC/OS-II系统启动
uC/OS-II首先调用OSInit()进行初始化，然后创建任务（此时还未启动系统，仅仅为其分配资源），然后调用OSStart()启动系统，将CPU控制权交给uC/OS-II，OS根据任务优先级选择由哪个任务开始执行，或创建新的任务。
(1) 初始化函数OSInit()
OSInit()主要完成初始化操作，包括初始化全局变量（在OS_InitMisc()中）、任务就绪表、TCB、ECB、FCB、内存单元、消息队列，并创建空闲任务。如果有必要，创建统计任务。
OS_InitMisc();                  //初始化部分全局变量
OS_InitRdyList();             //初始化任务就绪表
OS_InitTCBList();            //初始化空闲TCB链表
OS_InitEventList();          //初始化ECB链表
OS_FlagInit();                  //初始化事件组标志结构
OS_MemInit();                 //初始化内存管理
OS_QInit();                      //初始化消息队列
OS_InitTaskIdle();            //创建空闲任务
OS_InitTaskStat();            //创建统计任务
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115002566.jpg)
uC/OS-II初始化了5个空的数据结构缓冲区，每个缓冲区都是单向链表，允许uC/OS-II从缓冲区中迅速取得或释放一个缓冲区中的元素。![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115024586.jpg)
uC/OS-II调用OSInit()后的变量与数据结构如下图所示：
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051116014561.jpg)
(2) 启动函数OSStart()
OSStart()在一切准备就绪且需要首先创建的任务都被创建后，启动uC/OS-II。它从就绪表中查找最高优先级就绪任务，并恢复其上下文开始执行。
过程：OSStart() -> OS_SchedNew() -> OSStartHighRdy()
问题：任务第一次被调用时，哪来的上下文供其恢复呢？创建任务时，调用了OSTaskStkInit()初始化任务堆栈，可此函数中没有涉及任务的上下文呀？
uC/OS-II调用OSStart()后的变量和数据结构如下图所示：
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051116040057.jpg)
(3) 统计任务OSTaskStat
OSTaskStat用于计算CPU利用率。设置OS_CFG.H中的OS_TASK_STAT_EN为1，创建统计任务，在系统启动后一直处于就绪状 态。刚开始时，空闲任务运行1S，为计算CPU利用率提供一个基准值，并保存在统计任务的堆栈中，这个值不会改变除非重新启动CPU。此后空闲任每次被其 它任务抢去CPU时，它里面的计数器就会直接记录下CPU空闲的时间。
3. uC/OS-II系统时钟
任何实时系统的时钟硬件设备每隔一段时间（一个系统tick）产生一个硬件中断，OS接收到该中断后，更新时间计数器，更新所有对时钟依赖的程序代码，从而维持系统有序稳定的运行。
主要包含在C源文件OS_TIME.C中。
\#define OS_TICKS_PER_SEC 100     //系统时钟中断间隔
OS_EXT volatile INT32U OSTime;      //系统运行的时间值
void OSTimeTick(void);                         //时钟中断服务程序
void OSTimeDly(INT16U ticks);           //延迟指定时钟节拍
INT8U OSTimeDlyHMSM(…);             //延迟指定时间长度
INT8U OSTimeDlyResume(prio);  //恢复等待（时延/阻塞）任务
INT32U OSTimeGet(void);                   //读取当前时间
void OSTimeSet(INT32U ticks);            //设置当前时间
4. uC/OS-II事件管理
(1)   事件控制块
INT8U OSEventType;                           //事件类型
void *OSEventPtr;                                //指向MBox或Queue
INT16U OSEventCnt;                           //信号量计数器（注：Mutex）
INT8U OSEventGrp;                            //事件等待组标志
INT8U OSEventTbl[];                          //时间任务等待表
INT8U OSEventName[];                      //时间名称
（2） ECB管理机制
OS_EXT OS_EVENT *OSEventFreeList;   //空闲ECB链表指针
OS_EXT OS_EVENT OSEventTbl[]；       //ECB结构体数组
（3） ECB管理函数
OS_InitEventList()：初始化ECB；
OS_EventWaitListInit()：在创建事件时调用，初始化ECB任务等待表；
OS_EventTaskRdy()：在事件发生时调用，修改此事件中最高优先级任务的TCB成员变量，在任务就绪表中添加此任务，将相应信息传递给该任务TCB，并将此任务从事件的任务等代表中删除；
OS_EventTaskWait()：在申请资源失败而暂停当前任务时被调用，将任务从任务就绪表中删除，并添加到事件的任务等代表中；
OS_EventTO()：在事件等待超时时调用，将此任务从事件的任务等代表中删除，并修改该任务的TCB成员变量；




