
# uC-OS II学习时的一个重要函数 OSInit( ) -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:14:54[seven-soft](https://me.csdn.net/softn)阅读数：468


在uC/OS II的学习中，OSInit( );是一个重要的函数，它在OS应用中的void main();函数中首先被调用，是OS运行的第一个函数,如下程序，它完成各初始变量的初始化，建立任务空闲键表OSTCBFreeList, 建立事件空闲键表OSEventFreeList.创建一个空闲任务OS_TaskIdle，以及初始化其它如果有用到的功能 Q,MEM等。我个人的理解也是从这里开始的。通过空闲任务的创建基本上涉及到OS中任务块和事件块的管理。我想这也是理解了OS在内核结构上的一半以 上。
/********************************************/
//系统主函数
/********************************************/
void main (void)
{
OSInit( );      //第一个OS内核运行函数.
Targetinit( ); //目标板初始化.
OSTaskCreate(.......); // 在OSStart( );前必须至少创建一个任务
OSStart( );
}
OSInit ( );原形如下：
Void OSInit(void)
{
OS_InitMisc( ); //完成一般变量初始化
OS_InitRdyList( );//就绪列表初始化
OS_InitTCBList( );//空闲任务键表OSTCBFreeList建立
OS_InitEventList( );//事件空闲键表OSEventFreeList建立。
OS_FlagInit( );  //其它相关功能参数初始化。
OS_MemInit( );
OS_QInit( );
OS_InitTaskIdle( );//创建空闲任务OS_TaskIdle.
OS_InitTaskStat( );
OSInitHookEnd( );
}
//***************************************************************
static  void  OS_InitMisc (void)
{
\#if OS_TIME_GET_SET_EN > 0   //如果开启获得时间函数，则需要初始化此变量。
OSTime        = 0L;
\#endif
OSIntNesting  = 0;
OSLockNesting = 0;
OSTaskCtr     = 0;
OSRunning     = FALSE;   /* Indicate that multitasking not started */
OSCtxSwCtr    = 0;        /* Clear the context switch counter  */
OSIdleCtr     = 0L;        /* Clear the 32-bit idle counter   */
\#if OS_TASK_STAT_EN > 0
OSIdleCtrRun  = 0L;
OSIdleCtrMax  = 0L;
OSStatRdy     = FALSE;    /* Statistic task is not ready   */
\#endif
}
//*************************************************************
static  void  OS_InitRdyList (void)
{
INT8U    i;
INT8U   *prdytbl;
OSRdyGrp = 0x00;        /* Clear the ready list  */
prdytbl = &OSRdyTbl[0];
for (i = 0; i < OS_RDY_TBL_SIZE; i++)
{
*prdytbl++ = 0x00;
}
OSPrioCur     = 0;
OSPrioHighRdy = 0;
OSTCBHighRdy  = (OS_TCB *)0;
OSTCBCur      = (OS_TCB *)0;
}
//***********************************************************
static  void  OS_InitTCBList (void)
{
INT8U    i;
OS_TCB  *ptcb1;
OS_TCB  *ptcb2;
ptcb1 = &OSTCBTbl[0];
ptcb2 = &OSTCBTbl[1];
for (i = 0; i < (OS_MAX_TASKS + OS_N_SYS_TASKS - 1); i++)
{
ptcb1->OSTCBNext = ptcb2;   /* Init. list of free TCBs */
ptcb1++;
ptcb2++;
}
ptcb1->OSTCBNext = (OS_TCB *)0; /* Last OS_TCB */
OSTCBList = (OS_TCB *)0;     /* TCB lists initializations          */
OSTCBFreeList = &OSTCBTbl[0];
}
//****************************************************************
static  void  OS_InitEventList (void)
{
\#if OS_EVENT_EN && (OS_MAX_EVENTS > 0)
\#if (OS_MAX_EVENTS > 1)
INT16U     i;
OS_EVENT  *pevent1;
OS_EVENT  *pevent2;
pevent1 = &OSEventTbl[0];
pevent2 = &OSEventTbl[1];
for (i = 0; i < (OS_MAX_EVENTS - 1); i++)
{
pevent1->OSEventType = OS_EVENT_TYPE_UNUSED;
pevent1->OSEventPtr = pevent2;
pevent1++;
pevent2++;
}
pevent1->OSEventType = OS_EVENT_TYPE_UNUSED;
pevent1->OSEventPtr = (OS_EVENT *)0;
OSEventFreeList = &OSEventTbl[0];
\#else
OSEventFreeList = &OSEventTbl[0]; /* Only have ONE event control block       */
OSEventFreeList->OSEventType    = OS_EVENT_TYPE_UNUSED;
OSEventFreeList->OSEventPtr = (OS_EVENT *)0;
\#endif
\#endif
}


