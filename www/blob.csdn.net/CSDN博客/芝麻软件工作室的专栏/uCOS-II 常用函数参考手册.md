
# uCOS-II 常用函数参考手册 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月13日 10:28:50[seven-soft](https://me.csdn.net/softn)阅读数：1732


任务管理
1 OSTaskCreate()
建立一个新任务。任务的建立可以在多任务环境启动之前，也可以在正在运行的任务中建立。中断处理程序中不能建立任务。一个任务可以为无限循环的结构。
函数原型：INT8U OSTaskCreate(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio);
参数说明：task 是指向任务代码首地址的指针。
pdata 指向一个数据结构，该结构用来在建立任务时向任务传递参数。
ptos is a pointer to the task's top of stack. If the configuration constant OS_STK_GROWTH is set to 1, the stack is assumed to grow downward (i.e. from high memory to low memory). 'pstk' will thus point to the highest (valid) memory location of the stack.
 If OS_STK_GROWTH is set to 0, 'pstk' will point to the lowest memory location of the stack and the stack will grow with increasing memory locations.
prio is the task's priority. A unique priority MUST be assigned to each task and the lower the number, the higher the priority.
返回值：
OSTaskCreate() 的返回值为下述之一：
* OS_NO_ERR：函数调用成功。
* OS_PRIO_EXIST：具有该优先级的任务已经存在。
* OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
* OS_NO_MORE_TCB：系统中没有OS_TCB可以分配给任务了。
2 OSTaskSuspend()
无条件挂起一个任务。调用此函数的任务也可以传递参数 OS_PRIO_SELF，挂起调用任务本身。当前任务挂起后，只有其他任务才能唤醒被挂起的任务。任务挂起后，系统会重新进行任务调度，运行下一个优先 级最高的就绪任务。唤醒挂起任务需要调用函数OSTaskResume()。
任务的挂起是可以叠加到其他操作上的。例如，任务被挂起时正在进行延时操作，那么任务的唤醒就需要两个条件：延时的结束以及其他任务的唤醒操作。又如，任务被挂起时正在等待信号量，当任务从信号量的等待对列中清除后也不能立即运行，而必须等到被唤醒后。
函数原型：INT8U OSTaskSuspend(INT8U prio);
参数说明：prio为指定要获取挂起的任务优先级，也可以指定参数 OS_PRIO_SELF，挂起任务本身。此时，下一个优先级最高的就绪任务将运行。
返回值：
OSTaskSuspend() 的返回值为下述之一：
* OS_NO_ERR：函数调用成功。
* OS_TASK_SUSPEND_IDLE：试图挂起μC/OS-II中的空闲任务（Idle task）。此为非法操作。
* OS_PRIO_INVALID：参数指定的优先级大于 OS_LOWEST_PRIO 或没有设定 OS_PRIO_SELF 的值。
* OS_TASK_SUSPEND_PRIO：要挂起的任务不存在。

3 OSTaskResume()
唤醒一个用 OSTaskSuspend() 函数挂起的任务。OSTaskResume() 也是唯一能“解挂”挂起任务的函数。
函数原型：INT8U OSTaskResume(INT8U prio);
参数说明：prio指定要唤醒任务的优先级。
返回值：
OSTaskResume() 的返回值为下述之一：
* OS_NO_ERR：函数调用成功。
* OS_TASK_RESUME_PRIO：要唤醒的任务不存在。
* OS_TASK_NOT_SUSPENDED：要唤醒的任务不在挂起状态。
* OS_PRIO_INVALID：参数指定的优先级大于或等于OS_LOWEST_PRIO。
信号量
1 OSSemCreate()
该函数建立并初始化一个信号量，信号量的作用如下：
* 允许一个任务和其他任务或者中断同步
* 取得设备的使用权
* 标志事件的发生
函数原型：OS_EVENT *OSSemCreate(INT16U value);
参数说明：value 参数是所建立的信号量的初始值，可以取0到65535之间的任何值。
返回值：
OSSemCreate() 函数返回指向分配给所建立的信号量的控制块的指针。如果没有可用的控制块，OSSemCreate() 函数返回空指针。
2 OSSemPend()
该函数用于任务试图取得设备的使用权、任务需要和其他任务或中断同步、任务需要等待特定事件的发生的场合。如果任务调用OSSemPend() 函数时，信号量的值大于零，OSSemPend() 函数递减该值并返回该值。如果调用时信号量值等于零，OSSemPend() 函数将任务加入该信号量的等待队列。OSSemPend() 函数挂起当前任务直到其他的任务或中断设置信号量或超出等待的预期时间。如果在预期的时钟节拍内信号量被设置，μC/OS-Ⅱ默认让最高优先级的任务取得 信号量并回到就绪状态。一个被OSTaskSuspend()
 函数挂起的任务也可以接受信号量，但这个任务将一直保持挂起状态直到通过调用OSTaskResume() 函数恢复该任务的运行。
函数原型：void OSSemPend ( OS_EVNNT *pevent, INT16U timeout, int8u *err );
参数说明：pevent 是指向信号量的指针。该指针的值在建立该信号量时可以得到。（参考OSSemCreate() 函数）。
timeout 允许一个任务在经过了指定数目的时钟节拍后还没有得到需要的信号量时恢复就绪状态。如果该值为零表示任务将持续地等待信号量，最大的等待时间为65535个时钟节拍。这个时间长度并不是非常严格的，可能存在一个时钟节拍的误差。
err 是指向包含错误码的变量的指针，返回的错误码可能为下述几种：
* OS_NO_ERR ：信号量不为零。
* OS_TIMEOUT ：信号量没有在指定数目的时钟周期内被设置。
* OS_ERR_PEND_ISR ：从中断调用该函数。虽然规定了不允许从中断调用该函数，但μC/OS-Ⅱ仍然包含了检测这种情况的功能。
* OS_ERR_EVENT_TYPE ：pevent 不是指向信号量的指针。
返回值：无
3 OSSemPost()
该函数用于设置指定的信号量。如果指定的信号量是零或大于零，OSSemPost() 函数递增该信号量的值并返回。如果有任何任务在等待该信号量，则最高优先级的任务将得到信号量并进入就绪状态。任务调度函数将进行任务调度，决定当前运行 的任务是否仍然为最高优先级的就绪任务。
函数原型：INT8U OSSemPost（OS_EVENT *pevent）;
参数说明：pevent 是指向信号量的指针。该指针的值在建立该信号量时可以得到。（参考OSSemCreate() 函数）。
返回值：
OSSemPost() 函数的返回值为下述之一：
* OS_NO_ERR ：信号量被成功地设置
* OS_SEM_OVF ：信号量的值溢出
* OS_ERR_EVENT_TYPE ：pevent 不是指向信号量的指针
时间
1 OSTimeDly()
该函数用于将一个任务延时若干个时钟节拍。如果延时时间大于0，系统将立即进行任务调度。延时时间的长度可从0到65535个时钟节拍。延时时间0表示不 进行延时，函数将立即返回调用者。延时的具体时间依赖于系统每秒钟有多少个时钟节拍（由文件OS_CFG.H中的OS_TICKS_PER_SEC宏来设 定）。
函数原型：void OSTimeDly (INT16U ticks);
参数说明：ticks 为要延时的时钟节拍数。
返回值：无
2 OSTimeDlyHMSM()
该函数用于将一个任务延时若干时间。延时的单位是小时、分、秒、毫秒。调用 OSTimeDlyHMSM() 后，如果延时时间不为0，系统将立即进行任务调度。
函数原型: INT8U OSTimeDlyHMSM (INT8U hours，INT8U minutes，INT8U seconds，INT16U milli);
参数说明：hours 为延时小时数，范围从0-255。
minutes 为延时分钟数，范围从0-59。
seconds 为延时秒数，范围从0-59
milli 为延时毫秒数，范围从0-999。
需要说明的是，操作系统在处理延时操作时都是以时钟节拍为单位的，实际的延时时间是时钟节拍的整数倍。如果系统时钟节拍的间隔是10ms，而设定延时为 5ms的话，则不会产生延时操作；而如果设定延时为15ms，则实际的延时是两个时钟节拍，也就是20ms。
返回值：
OSTimeDlyHMSM() 的返回值为下述之一：
* OS_NO_ERR：函数调用成功。
* OS_TIME_INVALID_MINUTES：参数错误，分钟数大于59。
* OS_TIME_INVALID_SECONDS：参数错误，秒数大于59。
* OS_TIME_INVALID_MILLI：参数错误，毫秒数大于999。
* OS_TIME_ZERO_DLY：四个参数全为0。
内存管理
1 OSMemCreate()
该函数建立并初始化一个用于动态内存分配的区域，该内存区域包含指定数目的、大小确定的内存块。应用可以动态申请这些内存块并在用完后将其释放回这个内存 区域。该函数的返回值就是指向这个内存区域控制块的指针，并作为OSMemGet()，OSMemPut()，OSMemQuery() 等相关调用的参数。
函数原型：OS_MEM *OSMemCreate( void *addr, INT32U nblks, INT32U blksize, INT8U *err );
参数说明：addr 建立的内存区域的起始地址。可以使用静态数组或在系统初始化时使用 malloc() 函数来分配这个区域的空间。
nblks 内存块的数目。每一个内存区域最少需要定义两个内存块。
blksize 每个内存块的大小，最小应该能够容纳一个指针变量。
err 是指向包含错误码的变量的指针。Err可能是如下几种情况：
* OS_NO_ERR ：成功建立内存区域。
* OS_MEM_INVALID_ADDR ：非法地址，即地址为空指针。
* OS_MEM_INVALID_PART ：没有空闲的内存区域。
* OS_MEM_INVALID_BLKS ：没有为内存区域建立至少两个内存块。
* OS_MEM_INVALID_SIZE ：内存块大小不足以容纳一个指针变量。
返回值：
OSMemCreate() 函数返回指向所创建的内存区域控制块的指针。如果创建失败，函数返回空指针。
2 OSMemGet()
该函数用于从内存区域分配一个内存块。用户程序必须知道所建立的内存块的大小，并必须在使用完内存块后释放它。可以多次调用 OSMemGet() 函数。它的返回值就是指向所分配内存块的指针，并作为 OSMemPut() 函数的参数。
函数原型：void *OSMemGet(OS_MEM *pmem, INT8U *err);
参数说明：pmem 是指向内存区域控制块的指针，可以从 OSMemCreate() 函数的返回值中得到。
err 是指向包含错误码的变量的指针。Err可能是如下情况：
* OS_NO_ERR ：成功得到一个内存块。
* OS_MEM_NO_FREE_BLKS ：内存区域中已经没有足够的内存块。
返回值：
OSMemGet() 函数返回指向所分配内存块的指针。如果没有可分配的内存块，OSMemGet() 函数返回空指针。
3 OSMemPut()
该函数用于释放一个内存块，内存块必须释放回它原先所在的内存区域，否则会造成系统错误。
函数原型：INT8U OSMemPut (OS_MEM *pmem, void *pblk);
参数说明：pmem 是指向内存区域控制块的指针，可以从 OSMemCreate() 函数的返回值中得到。
pblk 是指向将被释放的内存块的指针。
返回值：
OSMemPut() 函数的返回值为下述之一：
* OS_NO_ERR ：成功释放内存块
* OS_MEM_FULL ：内存区域已满，不能再接受更多释放的内存块。这种情况说明用户程序出现了错误，释放了多于用 OSMemGet() 函数得到的内存块。
4 OSMemQuery()
该函数用于得到内存区域的信息。
函数原型：INT8U OSMemQuery(OS_MEM *pmem, OS_MEM_DATA *pdata);
参数说明：pmem 是指向内存区域控制块的指针，可以从 OSMemCreate() 函数的返回值中得到。
pdata 是一个指向 OS_MEM_DATA 数据结构的指针，该数据结构包含了以下的域：
void     OSAddr;           /* 指向内存区域起始地址的指针 */
void     OSFreeList;       /* 指向空闲内存块列表起始地址的指针 */
INT32U   OSBlkSize;        /* 每个内存块的大小 */
INT32U   OSNBlks;          /* 该内存区域中的内存块总数 */
INT32U   OSNFree;          /* 空闲的内存块数目 */
INT32U   OSNUsed;          /* 已使用的内存块数目 */
消息队列
1 OSQCreate()
该函数用于建立一个消息队列。任务或中断可以通过消息队列向一个或多个任务发送消息。消息的含义是和具体的应用密切相关的。
函数原型：OS_EVENT *OSQCreate (void **start, INT8U size);
参数说明：start 是消息内存区的首地址，消息内存区是一个指针数组。
size 是消息内存区的大小。
返回值：
OSQCreate() 函数返回一个指向消息队列控制块的指针。如果没有空闲的控制块，OSQCreate() 函数返回空指针。
2 OSQPend()
该函数用于任务等待消息。消息通过中断或任务发送给需要的任务。消息是一个指针变量，在不同的应用中消息的具体含义不同。如果调用 OSQPend() 函数时队列中已经存在消息，那么该消息被返回给 OSQPend() 函数的调用者，该消息同时从队列中清除。如果调用 OSQPend() 函数时队列中没有消息，OSQPend() 函数挂起调用任务直到得到消息或超出定义的超时时间。如果同时有多个任务等待同一个消息，μC/OS-Ⅱ默认最高优先级的任务取得消息。一个由 OSTaskSuspend() 函数挂起的任务也可以接受消息，但这个任务将一直保持挂起状态直到通过调用
 OSTaskResume() 函数恢复任务的运行。
函数原型：void *OSQPend (OS_EVENT *pevent, INT16U timeout, INT8U *err);
参数说明：pevent 是指向消息队列的指针，该指针的值在建立该队列时可以得到。（参考 OSQCreate() 函数）。
timeout 允许一个任务以指定数目的时钟节拍等待消息。超时后如果还没有得到消息则恢复成就绪状态。如果该值设置成零则表示任务将持续地等待消息，最大的等待时间为 65535个时钟节拍。这个时间长度并不是非常严格的，可能存在一个时钟节拍的误差。
err 是指向包含错误码的变量的指针。OSQPend() 函数返回的错误码可能为下述几种：
* OS_NO_ERR ：消息被正确地接受。
* OS_TIMEOUT ：消息没有在指定的时钟周期数内接收到消息。
* OS_ERR_PEND_ISR ：从中断调用该函数。虽然规定了不允许从中断中调用该函数，但μC/OS-Ⅱ仍然包含了检测这种情况的功能。
* OS_ERR_EVENT_TYPE ：pevent 不是指向消息队列的指针。
返回值：
OSQPend() 函数返回取得的消息并将 *err 置为 OS_NO_ERR。如果没有在指定数目的时钟节拍内接受到消息，OSQPend() 函数返回空指针并将 *err 设置为 OS_TIMEOUT。
3 OSQPostFront()
该函数用于向消息队列发送消息。OSQPostFront() 函数和 OSQPost() 函数非常相似，不同之处在于 OSQPostFront() 函数将发送的消息插到消息队列的最前端。也就是说，OSQPostFront() 函数使得消息队列按照后入先出（LIFO）的方式工作，而不是先入先出（FIFO）。消息是一个指针长度的变量，在不同的应用中消息的含义也可能不同。如 果队列中已经存满消息，则此调用将返回错误码。OSQPost() 函数也是如此。在调用此函数时如果有任何任务在等待队列中的消息，则最高优先级的任务将得到这个消息。如果等待消息的任务优先级比发送消息的任务优先级
 高，那么高优先级的任务在得到消息后将立即抢占当前任务执行，也就是说，将发生一次任务切换。
函数原型：INT8U OSQPostFront(OS_EVENT *pevent, void *msg);
参数说明：pevent 是指向即将接收消息的消息队列的指针。该指针的值在建立队列时可以得到。（参考 OSQCreate() 函数）。
msg 是即将发送的消息的指针。不允许传递一个空指针。
返回值：
OSQPostFront() 函数的返回值为下述之一：
* OS_NO_ERR ：消息成功地放到消息队列中。
* OS_Q_FULL ：消息队列已满。
* OS_ERR_EVENT_TYPE ：pevent 不是指向消息队列的指针。
4 OSQPost()
该函数用于向消息队列发送消息。消息是一个指针长度的变量，在不同的应用中消息的含义也可能不同。如果队列中已经存满消息，则此调用返回错误码。如果有任 何任务在等待队列中的消息，则最高优先级的任务将得到这个消息。如果等待消息的任务优先级比发送消息的任务优先级高，那么高优先级的任务将在得到消息后立 即抢占当前任务执行，也就是说，将发生一次任务切换。消息是以先入先出（FIFO）方式进入队列的，即先进入队列的消息先被传递给任务。
函数原型：INT8U OSQPost(OS_EVENT *pevent, void *msg);
参数说明：pevent 是指向即将接受消息的消息队列的指针。该指针的值在建立队列时可以得到。（参考 OSQCreate() 函数）。
msg 是即将发送给队列的消息。不允许传递一个空指针。
返回值：
OSQPost() 函数的返回值为下述之一：
* OS_NO_ERR ：消息成功地放到消息队列中。
* OS_Q_FULL ：消息队列已满。
* OS_ERR_EVENT_TYPE ：pevent 不是指向消息队列的指针。
5 OSQFlush()
该函数用于清空消息队列。
函数原型：INT8U *OSQFlush（OS_EVENT *pevent）;
参数说明：pevent 是指向消息队列的指针。该指针的值在建立队列时可以得到。（参考 OSQCreate() 函数）。
返回值：
OSQFlush() 函数的返回值为下述之一：
* OS_NO_ERR ：消息队列被成功清空
* OS_ERR_EVENT_TYPE ：试图清除不是消息队列的对象
6 OSQQuery()
该函数用来取得消息队列的信息。用户程序必须建立一个 OS_Q_DATA 的数据结构，该结构用来保存从消息队列的控制块得到的数据。通过调用该函数可以知道是否有任务在等待消息、有多少个任务在等待消息、队列中有多少消息以及 消息队列可以容纳的消息数。OSQQuery() 函数还可以得到即将被传递给任务的消息。
函数原型：INT8U OSQQuery(OS_EVENT *pevent, OS_Q_DATA *pdata);
参数说明：pevent 是指向消息队列的指针。该指针的值在建立消息队列时可以得到。（参考 OSQCreate() 函数）。
pdata 是指向 OS_Q_DATA 数据结构的指针，该数据结构包含下述成员：
void          *OSMsg;                        /* 下一个可用的消息 */
INT16U        OSNMsgs;                       /* 队列中的消息数目 */
INT16U        OSQSize;                       /* 消息队列的大小 */
INT8U         OSEventTbl[OS_EVENT_TBL_SIZE]; /* 消息队列的等待队列 */
INT8U         OSEventGrp；
返回值：
OSQQuery() 函数的返回值为下述之一：
* OS_NO_ERR ：调用成功
* OS_ERR_EVENT_TYPE ：pevent 不是指向消息队列的指针。
7 OSQDel()
该函数用于删除指定的消息队列。

uCOS II 消息队列的使用方法：
1、 需在以下文件中配置如下内容
OS_CFG.H
OS_MAX_QS      N     你需要的值
根据需要自己配置
\#define OS_Q_EN                   1    /* Enable (1) or Disable (0) code generation for QUEUES         */
\#define OS_Q_ACCEPT_EN            1    /*     Include code for OSQAccept()                             */
\#define OS_Q_DEL_EN               1    /*     Include code for OSQDel()                                */
\#define OS_Q_FLUSH_EN             1    /*     Include code for OSQFlush()                              */
\#define OS_Q_POST_EN              1    /*     Include code for OSQPost()                               */
\#define OS_Q_POST_FRONT_EN        1    /*     Include code for OSQPostFront()                          */
\#define OS_Q_POST_OPT_EN          1    /*     Include code for OSQPostOpt()                            */
\#define OS_Q_QUERY_EN             1    /*     Include code for OSQQuery()                              */
2、 建立一个指向消息数组的指针和数组的大小，该指针数组必须申明为void类型，如下：
void    *MyArrayOfMsg[SIZE];
3、 声明一个OS_EVENT类型的指针指向生成的队列，如下：
OS_EVENT *QSem；
4、 调用OSQcreate()函数创建消息队列，如下：
QSem      = OSQcreate(&MyArrayOfMsg[0],SIZE);
5、 等待消息队列中的消息，OSQPend()。void *OSQPend (OS_EVENT *pevent, INT16U timeout, INT8U *err):
必须保证消息队列已经被建立。
timeout定义的是等待超时时间，如果为0则表示无期限的等待
err表示的是在等待消息队列出错时的返回类型，有以下几种：
OS_ERR_PEVENT_NULL    //消息队列不存在
OS_ERR_EVENT_TYPE
OS_TIMEOUT   //消息队列等待超时
OS_NO_ERR             //消息队列接收到消息
获得消息队列示例
type   *GETQ;
INT8U   err;
GETQ   = (type *)OSQPend(QSem, time, &err);
if(err == OS_NO_ERR){
无错处理
}
else{
出错处理
}
6.1 向消息队列发送一则消息(FIFO),OSQPost(); INT8U OSQPost (OS_EVENT *pevent, void *msg):
函数返回值有：
OS_ERR_PEVENT_NULL
OS_ERR_POST_NULL_PTR
OS_ERR_EVENT_TYPE
OS_Q_FULL
OS_NO_ERR
参数：pevent,*msg
6.2 向消息队列发送一则消息(LIFO) INT8U OSQPostFront (OS_EVENT *pevent, void *msg)
6.3 向消息队列发送一则消息(LIFO或者FIFO) INT8U OSQPostOpt (OS_EVENT *pevent, void *msg, INT8U opt)
参数： opt
如果经opt参数中的OS_POST_OPT_BROADCAST位置为1，则所有正在等待消息的任务都能接收到这则消息，并且被OS_EventTaskRdy()从等待列表中删除
如果不是广播方式，则只有等待消息的任务中优先级最高的任务能够进入就绪态。然后，OS_EventTaskRdy()从等待列表中把等待消息的任务中优先级最高的任务删除。
注: 如果此函数由ISR调用，则不会发生任务切换，直到中断嵌套的最外层中断服务子程序调用OSIntExit()函数时，才能进行任务切换
7、 无等待的从消息队列中获得消息，OSQAccept(); void *OSQAccept (OS_EVENT *pevent, INT8U *err)
err可能的返回值：
OS_ERR_PEVENT_NULL
OS_Q_EMPTY
OS_NO_ERR
函数的返回值:消息，0
8、 清空消息队列 INT8U OSQFlush (OS_EVENT *pevent)
函数返回值：
OS_ERR_PEVENT_NULL
OS_ERR_EVENT_TYPE
OS_NO_ERR
9、 获取消息队列的状态，OSQQuery(); INT8U OSQQuery (OS_EVENT *pevent, OS_Q_DATA *p_q_data)
函数返回值：
OS_ERR_PEVENT_NULL
OS_ERR_EVENT_TYPE
OS_NO_ERR
OS_Q_DATA数据结构在ucos_ii.h中
// 采用消息队列的ADC采样任务原型代码，建议与uC/OS-II作者的ADC通用例程一起使用
// 说明这里消息队列msg_q不用于储存ADC结果。
void ADCTask(void * pParam)
{
char *cmd;
pParam=pParam;
while(1)
{
cmd=OSQPend(msg_q,100,&err); // waiting for command
if(err==OS_NO_ERR)
{
switch (*cmd)
{
case '1': printf("Command 1\n"); break;
case '2': printf("Command 2\n"); break;
default : printf("Error command.\n"); break;
}
}
else
{// no command , then sampling...
if(err==OS_TIMEOUT) // sampling while timeout.
printf("ADC sampling ...");
StoreADResult();
}
}
}
数据队列 Queue 的使用：
数据队列
数据队列一般用于数据缓存，可以用来平衡速率不同的两个部件，使快速部件无需等待慢速部件。
数据队列一般是先入先出的，但本数据队列可以配置成后入先出。本数据队列是可配置可裁剪的模块，并且不依赖于操作系统，可以在前后台系统中使用。数据队列使用的空间由用户分配且由这个空间的地址唯一识别一个数据队列。
API函数
数据队列软件模块包括的API函数如下所示：
API函数名                                     功能简介
QueueCreate                                  建立数据队列
QueueRead                                    获取队列中的数据
QueueWrite                                   先进先出方式发送数据
QueueWriteFront                           后进先出方式发送数据
QueueFlush                                    清空队列
QueueNData                                  取得队列中已存储数据的数据
QueueSize                                       取得队列中总共可以存储的数据数目
/******************************************************************************************
** 文件名: queue.c
** 描 述: 数据队列的中间件
********************************************************************************************************/
\#include "app_cfg.h"
\#define IN_QUEUE
\#define QUEUE_OK 1
\#define NOT_OK 0
\#define QUEUE_EMPTY 2
\#define QUEUE_FULL 3
\#define Q_WRITE_MODE 4
\#define Q_WRITE_FRONT_MODE 5
\#define QUEUE_DATA_TYPE INT8U
typedef struct {
QUEUE_DATA_TYPE     *pOut;           /* 指向数据输出位置         */
QUEUE_DATA_TYPE     *pIn;            /* 指向数据输入位置         */
QUEUE_DATA_TYPE     *pEnd;           /* 指向Buf的结束位置        */
uint16               nBytesPerRec;   /* 队列中每个记录包含数据个数           */
uint16               nMaxRecNum;     /* 队列可以存储的记录数目 */
uint16               nRecNum;        /* 队列中数据个数 */
uint8               (* ReadEmpty)(); /* 读空处理函数*/
uint8               (* WriteFull)(); /* 写满处理函数             */
QUEUE_DATA_TYPE     Buf[1];          /* 存储数据的空间           */
} DataQueue;

//SizeOfBuf是Buf的字节长度。
//QUEUE_DATA_TYPE是指可定义的数据类型可以Uint8,Uint32,等。
//Queue是DataQueue型结构体。

/*********************************************************************************************************
** 函数名称: QueueCreate
** 功能描述: 初始化数据队列
** 输 入: Buf ：为队列分配的存储空间地址
** SizeOfBuf：为队列分配的存储空间大小（字节）
** BytesPerRec: 队列每个记录占用字节数
** ReadEmpty：为队列读空时处理程序
** WriteFull：为队列写满时处理程序
** 输 出: NOT_OK:参数错误
** QUEUE_OK:成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint8 QueueCreate( void *Buf, uint32 SizeOfBuf, uint16 BytesPerRec,
uint8 (* ReadEmpty)(), uint8 (* WriteFull)() )
{
DataQueue *Queue;
if (Buf != NULL && SizeOfBuf >= (sizeof(DataQueue))) /* 判断参数是否有效 */
{
Queue = (DataQueue *)Buf;
/* 初始化结构体数据 */
OS_ENTER_CRITICAL();
Queue->nMaxRecNum = (SizeOfBuf - (uint32)(((DataQueue *)0)->Buf)) /
BytesPerRec; /* 计算队列可以存储的记录数目 */
Queue->pEnd = Queue->Buf + Queue->nMaxRecNum * BytesPerRec; /* 计算数据缓冲的结束地址 */
Queue->pOut = Queue->Buf;
Queue->pIn = Queue->Buf;
Queue->nRecNum = 0;
Queue->nBytesPerRec = BytesPerRec;
Queue->ReadEmpty = ReadEmpty;
Queue->WriteFull = WriteFull;
OS_EXIT_CRITICAL();
return QUEUE_OK;
}
else
{
return NOT_OK;
}
}

/*********************************************************************************************************
** 函数名称: QueueRead
** 功能描述: 获取队列中的数据
** 输 入: pRet:存储返回的记录数据的地址
** Buf:指向队列的指针
** 输 出: NOT_OK ：参数错误
** QUEUE_OK ：收到消息
** QUEUE_EMPTY：无消息
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint8 QueueRead(uint8 *pRet, void *Buf)
{
uint8 err;
uint16 i;
DataQueue *Queue;
err = NOT_OK;
if (Buf != NULL) /* 队列是否有效 */
{ /* 有效 */
Queue = (DataQueue *)Buf;
OS_ENTER_CRITICAL();
if (Queue->nRecNum > 0) /* 队列是否为空 */
{ /* 不空 */
for( i = 0; i < Queue->nBytesPerRec; i++ )
*pRet++ = *Queue->pOut++; /* 数据出队 */
/* 调整出队指针 */
if (Queue->pOut >= Queue->pEnd)
{
Queue->pOut = Queue->Buf;
}
Queue->nRecNum--; /* 数据减少*/
err = QUEUE_OK;
}
else
{ /* 空 */
err = QUEUE_EMPTY;
if (Queue->ReadEmpty != NULL) /* 调用用户处理函数 */
{
err = Queue->ReadEmpty(pRet, Queue);
}
}
OS_EXIT_CRITICAL();
}
return err;
}
/*********************************************************************************************************
** 函数名称: QueueWrite
** 功能描述: FIFO方式发送数据
** 输 入: Buf :指向队列的指针
** pRec:写入记录的首地址
** 输 出: NOT_OK :参数错误
** QUEUE_FULL:队列满
** QUEUE_OK :发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint8 QueueWrite(void *Buf, uint8* pRec)
{
uint8 err;
uint16 i;
DataQueue *Queue;
err = NOT_OK;
if (Buf != NULL) /* 队列是否有效 */
{
Queue = (DataQueue *)Buf;
OS_ENTER_CRITICAL();
if (Queue->nRecNum < Queue->nMaxRecNum) /* 队列是否满 */
{ /* 不满 */
for( i = 0; i < Queue->nBytesPerRec; i++ )
*Queue->pIn++ = *pRec++; /* 数据入队 */
/* 调整入队指针 */
if (Queue->pIn >= Queue->pEnd)
{
Queue->pIn = Queue->Buf;
}
Queue->nRecNum++; /* 数据增加 */
err = QUEUE_OK;
}
else
{ /* 满 */
err = QUEUE_FULL;
if (Queue->WriteFull != NULL) /* 调用用户处理函数 */
{
err = Queue->WriteFull(Queue, *pRec, Q_WRITE_MODE);
}
}
OS_EXIT_CRITICAL();
}
return err;
}
/*********************************************************************************************************
** 函数名称: QueueWriteFront
** 功能描述: LIFO方式发送数据
** 输 入: Buf:指向队列的指针
** pRec:写入记录的首地址
** 输 出: QUEUE_FULL:队列满
** QUEUE_OK:发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint8 QueueWriteFront(void *Buf, uint8 *pRec)
{
uint8 err;
uint16 i;
DataQueue *Queue;
err = NOT_OK;
if (Buf != NULL) /* 队列是否有效 */
{
Queue = (DataQueue *)Buf;
OS_ENTER_CRITICAL();
if (Queue->nRecNum < Queue->nMaxRecNum) /* 队列是否满 */
{ /* 不满 */
Queue->pOut--; /* 调整出队指针 */
if (Queue->pOut < Queue->Buf)
{
Queue->pOut = Queue->pEnd - 1;
}
for( i = 0; i < Queue->nBytesPerRec; i++ ) /* 不满 */
*Queue->pOut-- = *( pRec + Queue->nBytesPerRec - 1 - i ); /* 数据入队 */
Queue->pOut++;
Queue->nRecNum++; /* 数据数目增加 */
err = QUEUE_OK;
}
else
{ /* 满 */
err = QUEUE_FULL;
if (Queue->WriteFull != NULL) /* 调用用户处理函数 */
{
err = Queue->WriteFull(Queue, *pRec, Q_WRITE_FRONT_MODE);
}
}
OS_EXIT_CRITICAL();
}
return err;
}

/*********************************************************************************************************
** 函数名称: QueueRecNum
** 功能描述: 取得队列中记录数
** 输 入: Buf:指向队列的指针
** 输 出: 消息数
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint16 QueueRecNum(void *Buf)
{
uint16 temp;
temp = 0; /* 队列无效返回0 */
if (Buf != NULL)
{
OS_ENTER_CRITICAL();
temp = ((DataQueue *)Buf)->nRecNum;
OS_EXIT_CRITICAL();
}
return temp;
}
/*********************************************************************************************************
** 函数名称: QueueMaxRecNum
** 功能描述: 取得队列记录总容量
** 输 入: Buf:指向队列的指针
** 输 出: 队列记录总容量
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint16 QueueMaxRecNum(void *Buf)
{
uint16 temp;
temp = 0; /* 队列无效返回0 */
if (Buf != NULL)
{
OS_ENTER_CRITICAL();
temp = ((DataQueue *)Buf)->nMaxRecNum;
OS_EXIT_CRITICAL();
}
return temp;
}
/*********************************************************************************************************
** 函数名称: OSQFlush
** 功能描述: 清空队列
** 输 入: Buf:指向队列的指针
** 输 出: 无
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
void QueueFlush(void *Buf)
{
DataQueue *Queue;
if (Buf != NULL) /* 队列是否有效 */
{ /* 有效*/
Queue = (DataQueue *)Buf;
OS_ENTER_CRITICAL();
Queue->pOut = Queue->Buf;
Queue->pIn = Queue->Buf;
Queue->nRecNum = 0; /* 数据数目为0 */
OS_EXIT_CRITICAL();
}
}


