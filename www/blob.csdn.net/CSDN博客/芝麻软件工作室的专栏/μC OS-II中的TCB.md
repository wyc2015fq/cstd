
# μC-OS-II中的TCB -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:17:42[seven-soft](https://me.csdn.net/softn)阅读数：417


TCB为Task Control Block的简称，即任务控制快，它是用来存储一个任务当前的一些属性，其包括：
1、任务栈空间位置
2、任务与其他任务通信的数据空间
3、任务的当前状态，即任务是否在等待还是在运行
4、任务的优先级，任务的标识，其是任务调度的依据。
下面我们就将μC/OS-II中的TCB的结构体提取出来：
/*
*********************************************************************************************************
*                                          TASK CONTROL BLOCK
*********************************************************************************************************
*/
typedefstructos_tcb {
OS_STK          *OSTCBStkPtr;/* Pointer to current top of stack                              */
\#if OS_TASK_CREATE_EXT_EN > 0
void*OSTCBExtPtr;/* Pointer to user definable data for TCB extension             */
OS_STK          *OSTCBStkBottom;/* Pointer to bottom of stack                                   */
INT32U           OSTCBStkSize;/* Size of task stack (in number of stack elements)             */
INT16U           OSTCBOpt;/* Task options as passed by OSTaskCreateExt()                  */
INT16U           OSTCBId;/* Task ID (0..65535)                                           */
\#endif
structos_tcb   *OSTCBNext;/* Pointer to next     TCB in the TCB list                      */
structos_tcb   *OSTCBPrev;/* Pointer to previous TCB in the TCB list                      */
\#if OS_EVENT_EN
OS_EVENT        *OSTCBEventPtr;/* Pointer to event control block                               */
\#endif
\#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0)
void*OSTCBMsg;/* Message received from OSMboxPost() or OSQPost()              */
\#endif
\#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
\#if OS_TASK_DEL_EN > 0
OS_FLAG_NODE    *OSTCBFlagNode;/* Pointer to event flag node                                   */
\#endif
OS_FLAGS         OSTCBFlagsRdy;/* Event flags that made task ready to run                      */
\#endif
INT16U           OSTCBDly;/* Nbr ticks to delay task or, timeout waiting for event        */
INT8U            OSTCBStat;/* Task status                                                  */
BOOLEANOSTCBPendTO;/* Flag indicating PEND timed out (TRUE == timed out)           */
INT8U            OSTCBPrio;/* Task priority (0 == highest)                                 */
INT8U            OSTCBX;/* Bit position in group  corresponding to task priority        */
INT8U            OSTCBY;/* Index into ready table corresponding to task priority        */
\#if OS_LOWEST_PRIO <= 63
INT8U            OSTCBBitX;/* Bit mask to access bit position in ready table               */
INT8U            OSTCBBitY;/* Bit mask to access bit position in ready group               */
\#else
INT16U           OSTCBBitX;/* Bit mask to access bit position in ready table               */
INT16U           OSTCBBitY;/* Bit mask to access bit position in ready group               */
\#endif
\#if OS_TASK_DEL_EN > 0
INT8U            OSTCBDelReq;/* Indicates whether a task needs to delete itself              */
\#endif
\#if OS_TASK_PROFILE_EN > 0
INT32U           OSTCBCtxSwCtr;/* Number of time the task was switched in                      */
INT32U           OSTCBCyclesTot;/* Total number of clock cycles the task has been running       */
INT32U           OSTCBCyclesStart;/* Snapshot of cycle counter at start of task resumption        */
OS_STK          *OSTCBStkBase;/* Pointer to the beginning of the task stack                   */
INT32U           OSTCBStkUsed;/* Number of bytes used from the stack                          */
\#endif
\#if OS_TASK_NAME_SIZE > 1
INT8U            OSTCBTaskName[OS_TASK_NAME_SIZE];
\#endif
} OS_TCB;
第一个属性OSTCBStkPtr我们前面已经说过，它指向任务的栈顶指针。
\#if OS_TASK_CREATE_EXT_EN > 0
void*OSTCBExtPtr;/* Pointer to user definable data for TCB extension             */
OS_STK          *OSTCBStkBottom;/* Pointer to bottom of stack                                   */
INT32U           OSTCBStkSize;/* Size of task stack (in number of stack elements)             */
INT16U           OSTCBOpt;/* Task options as passed by OSTaskCreateExt()                  */
INT16U           OSTCBId;/* Task ID (0..65535)                                           */
\#endif
可以看出，它是当任务由OSTaskCreateExt()创建时才会具有的参数，各个参数的功能注释已经给出。
structos_tcb   *OSTCBNext;/* Pointer to next     TCB in the TCB list                      */
structos_tcb   *OSTCBPrev;/* Pointer to previous TCB in the TCB list                      */
这两个指针分别是指向TCB链表中的下一个TCB和上一个TCB。
\#if OS_EVENT_EN
OS_EVENT        *OSTCBEventPtr;/* Pointer to event control block                               */
\#endif
当使用了事件时，则需要申请事件的控制块空间，其指针指向时间控制块。
当使用消息队列时，则需要使用以下成员：
\#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0)
void*OSTCBMsg;/* Message received from OSMboxPost() or OSQPost()              */
\#endif
其指向从OSMboxPost() 或 OSQPost()接收到的消息。
\#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
\#if OS_TASK_DEL_EN > 0
OS_FLAG_NODE    *OSTCBFlagNode;/* Pointer to event flag node                                   */
\#endif
OS_FLAGS         OSTCBFlagsRdy;/* Event flags that made task ready to run                      */
\#endif
这个是当使用了实践组标识所需要的成员。
INT16U           OSTCBDly;/* Nbr ticks to delay task or, timeout waiting for event        */
INT8U            OSTCBStat;/* Task status                                                  */
BOOLEANOSTCBPendTO;/* Flag indicating PEND timed out (TRUE == timed out)           */
INT8U            OSTCBPrio;/* Task priority (0 == highest)                                 */
其中，OSTCBDly标识任务是否处于等待状态，为0是即为等待，不为0，则根据OSTCBDly的值来确定等待的时间。
OSTCBStat用来表示当前任务的状态。
OSTCBPendTO标识任务是否处于阻塞状态。
OSTCBPrio标识任务的优先级。
INT8U            OSTCBX;/* Bit position in group  corresponding to task priority        */
INT8U            OSTCBY;/* Index into ready table corresponding to task priority        */
\#if OS_LOWEST_PRIO <= 63
INT8U            OSTCBBitX;/* Bit mask to access bit position in ready table               */
INT8U            OSTCBBitY;/* Bit mask to access bit position in ready group               */
\#else
INT16U           OSTCBBitX;/* Bit mask to access bit position in ready table               */
INT16U           OSTCBBitY;/* Bit mask to access bit position in ready group               */
\#endif
\#if OS_TASK_DEL_EN > 0
INT8U            OSTCBDelReq;/* Indicates whether a task needs to delete itself              */
\#endif
这一部分是用于管理任务就绪表。
\#if OS_TASK_PROFILE_EN > 0
INT32U           OSTCBCtxSwCtr;/* Number of time the task was switched in                      */
INT32U           OSTCBCyclesTot;/* Total number of clock cycles the task has been running       */
INT32U           OSTCBCyclesStart;/* Snapshot of cycle counter at start of task resumption        */
OS_STK          *OSTCBStkBase;/* Pointer to the beginning of the task stack                   */
INT32U           OSTCBStkUsed;/* Number of bytes used from the stack                          */
\#endif
这一部分也是由OSTaskCreateExt()创建的任务多具有的属性，分别为任务切换的次数、任务执行所需的总时钟数、任务恢复起始处的循环计数器、任务栈起始位置、栈中已使用的空间的大小。
\#if OS_TASK_NAME_SIZE > 1
INT8U            OSTCBTaskName[OS_TASK_NAME_SIZE];
\#endif
这个是当任务设置自己的任务名称的时候，申请的任务名称的空间。
上面就是os_tcb的一个大概的结构，而TCB的管理则是通过一些结构体数组、双向链表等完成，比如OSTCBTb1[]是用来存储系统中当前所有的任务块数据；
OS_EXT OS_TCB *OSTCBList；//指向被使用的任务控制块链表
OS_EXT OS_TCB *OSTCBFreeList；//指向空闲的任务快链表
还有诸如此类的一些结构体用来管理TCB，这里就不一一详述了，可以根据需要自行查阅。

