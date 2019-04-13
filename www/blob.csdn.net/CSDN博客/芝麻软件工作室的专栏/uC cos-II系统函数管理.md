
# uC-cos-II系统函数管理 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月13日 10:00:09[seven-soft](https://me.csdn.net/softn)阅读数：947


任务管理
1 OSTaskCreate()
建立一个新任务。任务的建立可以在多任务环境启动之前，也可以在正在运行的任务中建立。中断处理程序中不能建立任务。一个任务可以为无限循环的结构。
函数原型：INT8U OSTaskCreate(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio);
参数说明：task 是指向任务代码首地址的指针。
pdata 指向一个数据结构，该结构用来在建立任务时向任务传递参数。
ptos is a pointer to the task's top of stack. If the configuration constant OS_STK_GROWTH is set to 1, the stack is assumed to grow downward (i.e. from high memory to low memory). 'pstk' will thus point to the highest (valid) memory location
 of the stack. If OS_STK_GROWTH is set to 0, 'pstk' will point to the lowest memory location of the stack and the stack will grow with increasing memory locations.
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
需要说明的是，操作系统在处理延时操作时都是以时钟节拍为单位的，实际的延时时间是时钟节拍的整数倍。如果系统时钟节拍的间隔是10ms，而设定延时为5ms的话，则不会产生延时操作；而如果设定延时为15ms，则实际的延时是两个时钟节拍，也就是20ms。
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
该函数用于删除指定的消息队列

