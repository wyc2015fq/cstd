
# uC-OS-II内核架构解析---uC-OS-II通信与同步 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:09:26[seven-soft](https://me.csdn.net/softn)阅读数：1700



## 1. 消息邮箱Mbox
Mbox用于多任务间单一消息的传递，uC/OS-II使用ECB管理Mbox的基本信息，OSEventPtr指向创建Mbox时指定的内存空间。事件的创建由具体的事件管理程序实现。主要包含在C源文件OS_MBOX.C中。
OS_EVENT *OSMboxCreate(void *msg);
void *OSMboxPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);
void *OSMboxAccept(OS_EVENT *pevent);
INT8U OSMboxPost(OS_EVENT *pevent, void *msg);
INT8U OSMboxPostOpt(OS_EVENT *pevent, void *msg, INT8U opt);
OS_EVENT *OSMboxDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
INT8U OSMboxQuery(OS_EVENT *pevent, OS_MBOX_DATA *);
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115235056.jpg)
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051116115125.jpg)
## 2. 消息队列msgQ
### (1) msgQ基本内容
msgQ是uC/OS-II任务间通信的机制，可实现多条消息传递，即可以同时存储多条消息。uC/OS-II使用循环队列管理机制。主要包含在C源文件OS_Q.C中。
msgQ管理：使用指针数组存储所有消息的位置；使用QCB标识指针数组中消息的基本信息；使用ECB管理整个msgQ。QCB在编译时分配空间，即当前系统中可用的msgQ个数是预先设置的，系统运行时不能修改。
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115252380.jpg)
### (2) msgQ全局变量
OS_EXT OS_Q *OSQTbl[OS_MAX_QS]; 　　//QCB结构体数组
OS_EXT OS_Q *OSQFreeList;                    //空闲QCB头指针
typedef struct os_q{                                //消息队列控制块
struct os_q *OSQPtr;        //用于构建空闲QCB链表
void **OSQStart;              //指向msgQ指针数组的起始位置
void **OSQEnd;               //指向msgQ指针数组的结束位置
void **OSQIn;    //指向msgQ指针数组下一个可以插入消息的位置
void **OSQOut; //指向msgQ指针数组下一个可以读出消息的位置
INT16U OSQSize;            //msgQ指针数组的大小
INT16U OSQEntries;              //msgQ指针数组当前可以读取的消息个数
}OS_Q;
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051116181554.jpg)
### (3) msgQ管理函数
OS_EVENT *OSQCreate(void **start, INT16U size);
INT8U OSQPost(OS_EVENT *pevent, void *msg); //发送消息到队尾
INT8U OSQPostFront(OS_EVENT *pevent, void *msg);      //msg至队首
INT8U OSQPostOpt(OS_EVENT *pevent, void *msg, INT8U opt);
void *OSQPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);
void *OSQAccept(OS_EVENT *pevent, INT8U *err);
OS_EVENT *OSQDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
INT8U OSQQuery(OS_EVENT *pevent, OS_Q_DATA*);
INT8U OSQFlush(OS_EVENT *pevent);
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115275215.jpg)
### (4) msgQ几个问题
uC/OS-II中，什么是事件？事件是uC/OS-II管理任务间同步与通信的机制。
事件是处理事件的对象感兴趣的，能够感知或捕获到一种事件状态的改变。
## 3. 信号量Sem
Sem主要用来实现任务间同步及标识某类资源的可用个数，即某个特定资源可供多少任务同时使用。主要包含在C源文件OS_SEM.C中。
OS_EVENT *OSSemCreate(INT16U cnt);
void OSSemPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);
INT16U OSSemAccept(OS_EVENT *pevent);
INT8U OSSemPost(OS_EVENT *pevent);
OS_EVENT *OSSemDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
INT8U OSSemQuery(OS_EVENT *pevent, OS_SEM_DATA*);
void OSSemSet(OS_EVENT *pevent, INT16U cnt, INT8U *err);
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115304776.jpg)
## 4. 互斥锁Mutex
### (1) Mutex基本原理
Mutex用来实现对资源的排他性访问，可能引起优先级反转。任何任务在占有某个互斥锁事件时，都不能阻塞等待其它任何事件，否则会造成死锁。主要包含在C源文件OS_MUTEX.C中。
优先级反转是指，低优先级任务占有高优先级任务运行所需的资源，而使高优先级不得不等低优先级任务把资源释放才能执行。
uC/OS-II使用ECB管理Mutex，其成员变量OSEventCnt：高8位存储Mutex被使用时提供给任务的prio；低8位在没有任务占有Mutex时为0xFF，否则为占有任务的prio。
优先级反转及优先级反转避免分别如下图所示：
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051116135683.jpg)
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051116155026.jpg)
### (2) 提升/恢复优先级
a) 提升Mutex拥有者任务的优先级的相关操作：
如果该任务原来处于就绪状态，则从就绪表中将其删除；如果该任务正在等待某个事件，则从该事件的任务等待表中将其删除；
修改拥有Mutex的TCB，将其OSTCBPrio修改为欲提升的优先级；
如果该任务处于就绪状态，则将提升的优先级加载到任务就绪表中；如果该任务未就绪且正在等待某个事件，则将提升的优先级添加到该事件的任务等待表中，并修改TCB中OSTCBEventPtr；
修改TCB中与优先级相关的成员变量。
b) 恢复Mutex拥有任务的优先级的相关操作：
从任务就绪表中删除提升过的优先级值，修改当前TCB中与优先级相关的所有成员变量；
再次保留提升的优先级值控制块入口，避免将其分配给其它任务。
（3）Mutex管理函数
\#define  OS_MUTEX_KEEP_LOWER_8   0x00FF
\#define  OS_MUTEX_KEEP_UPPER_8   0xFF00
\#define  OS_MUTEX_AVAILABLE      0x00FF
OS_EVENT *OSMutexCreate(INT8U prio, INT8U *err);
void OSMutexPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);
INT8U OSMutexAccept(OS_EVENT *pevent, INT8U *err);
INT8U OSMutexPost(OS_EVENT *pevent);
OS_EVENT *OSMutexDel(OS_EVENT*, INT8U opt, INT8U *err);
INT8U OSMutexQuery(OS_EVENT*, OS_MUTEX_DATA*);
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115332131.jpg)
## 5. 事件组标志Flag
### (1) Flag基本原理
uC/OS-II提供事件组标志实现多事件管理。Flag只是使用0/1来表示某个事件是否发生过，而不能直接被用来传递数据和消息。可以选择性地设置一个Flag最多可以管理的任务同步状态。主要包含在C源文件OS_FLAG.C中。
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115362340.jpg)
### (2) Flag数据结构
\#define OS_FLAGS_NBITS 8/16/32     //定义OS_FLAGS的位数
FCB结构体：
typedef struct os_flag_grp{
INT8U OSFlagType;                       　//事件类型
void *OSFlagWaitList;                      //指向等待的任务链表
OS_FLAGS OSFlagFlags;             　　//信号列表
INT8U OSFlagName[OS_FLAG_NAME_SIZE];
}OS_FLAG_GRP;
事件标志等待链表结点
typedef struct os_flag_node{
void *OSFlagNodeNext;
void *OSFlagNodePrev;
void *OSFlagNodeTCB;
void *OSFlagNodeFlagGrp;          //指向此任务所等待的事件组标志
OS_FLAGS OSFlagNodeFlags;     //等待的事件
INT8U OSFlagNodeWaitType;       //等待方式
}OS_FLAG_NODE;
OS_EXT OS_FLAG_GRP OSFlagTbl[OS_MAX_FLAGS];
OS_EXT OS_FLAG_GRP *OSFlagFreeList;
OS_FLAG_GRP *OSFlagCreate(OS_FLAGS flags, INT8U *err);
OS_FLAGS OSFlagPend(OS_FLAG_GRP *pgrp, OS_FLAGS flags,
### (3) Flag管理函数
INT8U wait_type, INT16U timeout, INT8U *err);
static void OS_FlagBlock(OS_FLAG_GRP *pgrp,
OS_FLAG_NODE *pnode,
OS_FLAGS flags,
INT8U wait_type,    //挂起任务，
INT16U timeout);     //直到等待的事件或超时
类似于：OS_EventTaskWait();
void OS_FlagUnlink(OS_FLAG_NODE *pnode);   //等待超时删除结点
类似于：OS_EventTO();
OS_FLAGS OSFlagAccept(OS_FLAG_GRP *pgrp,
OS_FLAGS flags,
INT8U wait_type,
INT8U *err);
OS_FLAGS OSFlagPost(OS_FLAG_GRP *pgrp,
OS_FLAGS flags,
INT8U opt,
INT8U *err);
static BOOLEAN OS_FLAGTaskRdy(OS_FLAG_NODE *pnode,
OS_FLAGS flags_rdy);
OS_FLAG_GRP *OSFlagDel(OS_FLAG_GRP*, INT8U opt, INT8U *err);
OS_FLAGS OSFlagPendGetFlagsRdy(void);    //获取任务就绪标志
OS_FLAGS OSFlagQuery(OS_FLAG_GRP*, INT8U *err);
INT8U OSFlagNameGet(OS_FLAG_GRP*, INT8U *pname, INT8U *err);
void OSFlagNameSet(OS_FLAG_GRP*, INT8U *pname, INT8U *err);
![](http://pic002.cnblogs.com/img/Ruyi0912/201005/2010051115415193.jpg)
## 6. Task
## 就绪状态
## 判断？？？
a) OSRdyTbl[ptcb->OSTCBY] & ptcb->OSTCBBitX != 0
如：函数OSMutexPend()
b) (ptcb->OSTCBStat & OS_STAT_SUSPEND) == OS_STAT_RDY
如：函数OSTimeTick()
c) ptcb->OSTCBStat == OS_STAT_RDY
如：函数OS_EventTaskRdy()

