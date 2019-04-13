
# μC-OS-II任务管理中的主要函数解析 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:58:45[seven-soft](https://me.csdn.net/softn)阅读数：697


μC/OS-II中，任务是基本的执行单元，所以任务如何管理对于基于μC/OS-II系统的驱动的编写是很关键的，下面就来浅析几个主要的任务管理函数的源码。其任务管理函数主要就在源码目录的Os_task.c中。
首先先来看一下任务是如何创建的，μC/OS-II中最基本的任务创建函数是OSTaskCreate ()函数，它的源码如下：
\#if OS_TASK_CREATE_EN > 0
INT8U  OSTaskCreate (void(*task)(void*p_arg),void*p_arg, OS_STK *ptos, INT8U prio)
{
OS_STK    *psp;
INT8U      err;
\#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register               */
OS_CPU_SR  cpu_sr = 0;
\#endif

\#if OS_ARG_CHK_EN > 0
if(prio > OS_LOWEST_PRIO) {/* Make sure priority is within allowable range           */
return(OS_PRIO_INVALID);
}
\#endif
OS_ENTER_CRITICAL();
if(OSIntNesting > 0) {/* Make sure we don't create the task from within an ISR  */
OS_EXIT_CRITICAL();
return(OS_ERR_TASK_CREATE_ISR);
}
if(OSTCBPrioTbl[prio] == (OS_TCB *)0) {/* Make sure task doesn't already exist at this priority  */
OSTCBPrioTbl[prio] = (OS_TCB *)1;/* Reserve the priority to prevent others from doing ...  */
/* ... the same thing until task is created.              */
OS_EXIT_CRITICAL();
psp = OSTaskStkInit(task, p_arg, ptos, 0);/* Initialize the task's stack         */
err = OS_TCBInit(prio, psp, (OS_STK *)0, 0, 0, (void*)0, 0);
if(err == OS_NO_ERR) {
if(OSRunning == TRUE) {/* Find highest priority task if multitasking has started */
OS_Sched();
}
}else{
OS_ENTER_CRITICAL();
OSTCBPrioTbl[prio] = (OS_TCB *)0;/* Make this priority available to others                 */
OS_EXIT_CRITICAL();
}
return(err);
}
OS_EXIT_CRITICAL();
return(OS_PRIO_EXIST);
}
\#endif
先来看前面一段：\#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register               */
OS_CPU_SR  cpu_sr = 0;
\#endif
这个事进入临界状态方法的一个选择，一般在ARM中选择的都是第三种,其是把寄存器的值保存到临界变量cpu_sr中。
\#define OS_ENTER_CRITICAL() {cpu_sr = OS_CPU_SR_Save();}
\#define OS_EXIT_CRITICAL() {OS_CPU_SR_Restore(cpu_sr);}
上面就是进入临界状态和退出临界状态的方法，首先通过函数将寄存器中的值保存在cpu_sr中，然后在退出临界状态时再将寄存器的值恢复。
\#if OS_ARG_CHK_EN>0
if (prio>OS_LOWEST_PRIO) {             /* Make sure priority is within allowable range           */
return (OS_PRIO_INVALID);
}
\#endif
上面这一段是检测任务的优先级的值是否在规定的范围之内。OS_ENTER_CRITICAL();
if (OSIntNesting>0) {                  /* Make sure we don't create the task from within an ISR  OSIntNesting为中断嵌套次数，没有中断则为0*/
OS_EXIT_CRITICAL();
return (OS_ERR_TASK_CREATE_ISR);
}
if (OSTCBPrioTbl[prio] == (OS_TCB *)0) { /* Make sure task doesn't already exist at this priority  */
OSTCBPrioTbl[prio] = (OS_TCB *)1;    /* Reserve the priority to prevent others from doing ...  */
/* ... the same thing until task is created.              */
OS_EXIT_CRITICAL();
psp=OSTaskStkInit(task, p_arg, ptos, 0);              /* Initialize the task's stack         */
err=OS_TCBInit(prio, psp, (OS_STK *)0, 0, 0, (void *)0, 0);
if (err== OS_NO_ERR) {
if (OSRunning== TRUE) {         /* Find highest priority task if multitasking has started */
OS_Sched();
}
} else {
OS_ENTER_CRITICAL();
OSTCBPrioTbl[prio] = (OS_TCB *)0;/* Make this priority available to others                 */
OS_EXIT_CRITICAL();
}
return (err);
}
OS_EXIT_CRITICAL();
return (OS_PRIO_EXIST);
}
上 面首先是判断是否在中断服务程序中执行此任务，如果是进入中断的话则返回错误。然后再检查任务的优先级是否已经存在，如果存在的话返回优先级已经存在的错 误提示，如果不存在，则注册此优先级。下面则由函数OSTaskStkInit()初始化任务栈空间，再由函数OS_TCBInit()初始化任务控制 块，如果任务快初始化失败，则释放注册的优先级占用。然后检测CPU是否在运行状态，如果在运行，则调度优先级最高的任务。感兴趣的可以把把初始化任务栈
 和TCB的函数调出来看看。下面我们再来看OSTaskCreate()的升级版OSTaskCreateExt()函数：
\#if OS_TASK_CREATE_EXT_EN > 0
INT8U  OSTaskCreateExt (void(*task)(void*p_arg),//指向任务代码的指针
void*p_arg,//指向传递给任务的参数的指针
OS_STK  *ptos,//指向栈顶指针
INT8U    prio,//任务优先级
INT16U   id,//任务ID值
OS_STK  *pbos,//任务栈底指针
INT32U   stk_size,//任务栈单元个数
void*pext,//任务控制块中扩展部分数据的位置
INT16U   opt)//任务栈初始化选项
{
OS_STK    *psp;
INT8U      err;
\#if OS_CRITICAL_METHOD == 3                  /* Allocate storage for CPU status register               */
OS_CPU_SR  cpu_sr = 0;
\#endif

\#if OS_ARG_CHK_EN > 0
if(prio > OS_LOWEST_PRIO) {/* Make sure priority is within allowable range           */
return(OS_PRIO_INVALID);
}
\#endif
OS_ENTER_CRITICAL();
if(OSIntNesting > 0) {/* Make sure we don't create the task from within an ISR  */
OS_EXIT_CRITICAL();
return(OS_ERR_TASK_CREATE_ISR);
}
if(OSTCBPrioTbl[prio] == (OS_TCB *)0) {/* Make sure task doesn't already exist at this priority  */
OSTCBPrioTbl[prio] = (OS_TCB *)1;/* Reserve the priority to prevent others from doing ...  */
/* ... the same thing until task is created.              */
OS_EXIT_CRITICAL();
OS_TaskStkClr(pbos, stk_size, opt);/* Clear the task stack (if needed)     */
psp = OSTaskStkInit(task, p_arg, ptos, opt);/* Initialize the task's stack          */
err = OS_TCBInit(prio, psp, pbos, id, stk_size, pext, opt);
if(err == OS_NO_ERR) {
if(OSRunning == TRUE) {/* Find HPT if multitasking has started */
OS_Sched();
}
}else{
OS_ENTER_CRITICAL();
OSTCBPrioTbl[prio] = (OS_TCB *)0;/* Make this priority avail. to others  */
OS_EXIT_CRITICAL();
}
return(err);
}
OS_EXIT_CRITICAL();
return(OS_PRIO_EXIST);
}
\#endif
先来看下opt的几种定义：/*
*********************************************************************************************************
*                                 TASK OPTIONS (see OSTaskCreateExt())
*********************************************************************************************************
*/
\#define  OS_TASK_OPT_NONE        0x0000u    /* NO option selected                                      */
\#define  OS_TASK_OPT_STK_CHK     0x0001u    /* Enable stack checking for the task                      */
\#define  OS_TASK_OPT_STK_CLR     0x0002u    /* Clear the stack when the task is create                 */
\#define  OS_TASK_OPT_SAVE_FP     0x0004u    /* Save the contents of any floating-point registers       */
/*
分别是没有选项位置、使能栈检查、创建栈时初始化为0、保存所有浮点指针寄存器的内容。代码前面和OSTaskCreate()大同小异，都是对优先级中断等的一些检测，初始化栈、任务控制块、引发任务调度。其中OS_TaskStkClr(0是清除栈空间，根据清理选项和栈增长方向逐个字节将栈空间初始化为0，其原型为：
\#if OS_TASK_CREATE_EXT_EN > 0
voidOS_TaskStkClr (OS_STK *pbos, INT32U size, INT16U opt)
{
if((opt & OS_TASK_OPT_STK_CHK) != 0x0000) {/* See if stack checking has been enabled       */
if((opt & OS_TASK_OPT_STK_CLR) != 0x0000) {/* See if stack needs to be cleared             */
\#if OS_STK_GROWTH == 1
while(size > 0) {/* Stack grows from HIGH to LOW memory          */
size--;
*pbos++ = (OS_STK)0;/* Clear from bottom of stack and up!           */
}
\#else
while(size > 0) {/* Stack grows from LOW to HIGH memory          */
size--;
*pbos-- = (OS_STK)0;/* Clear from bottom of stack and down          */
}
\#endif
}
}
}
\#endif


接下来看删除任务函数：
\#if OS_TASK_DEL_EN > 0
INT8U  OSTaskDel (INT8U prio)
{
\#if OS_EVENT_EN
OS_EVENT     *pevent;
\#endif
\#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
OS_FLAG_NODE *pnode;
\#endif
OS_TCB       *ptcb;
INT8U         y;
\#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
OS_CPU_SR     cpu_sr = 0;
\#endif

if(OSIntNesting > 0) {/* See if trying to delete from ISR    */
return(OS_TASK_DEL_ISR);
}
\#if OS_ARG_CHK_EN > 0
if(prio == OS_IDLE_PRIO) {/* Not allowed to delete idle task     */
return(OS_TASK_DEL_IDLE);
}
if(prio >= OS_LOWEST_PRIO) {/* Task priority valid ?               */
if(prio != OS_PRIO_SELF) {
return(OS_PRIO_INVALID);
}
}
\#endif
OS_ENTER_CRITICAL();// 标记1
if(prio == OS_PRIO_SELF) {/* See if requesting to delete self    如果是删除自己*/
prio = OSTCBCur->OSTCBPrio;/* Set priority to delete to current   获取自己的优先级*/
}
ptcb = OSTCBPrioTbl[prio];/*查找要删除任务的任务控制块 */
if(ptcb == (OS_TCB *)0) {/* Task to delete must exist           */
OS_EXIT_CRITICAL();
return(OS_TASK_DEL_ERR);
}
if(ptcb == (OS_TCB *)1) {/* Must not be assigned to Mutex       */
OS_EXIT_CRITICAL();
return(OS_TASK_NOT_EXIST);
}
y            =  ptcb->OSTCBY;/*获取就绪表中对应的行*/
OSRdyTbl[y] &= ~ptcb->OSTCBBitX;/*清除就绪表中对应行的位*/
if(OSRdyTbl[y] == 0) {/* Make task not ready                 */
OSRdyGrp &= ~ptcb->OSTCBBitY;
}
\#if OS_EVENT_EN
pevent = ptcb->OSTCBEventPtr;/*如果定义了事件，找到任务所等待的事件*/
if(pevent != (OS_EVENT *)0) {/* If task is waiting on event         */
pevent->OSEventTbl[y] &= ~ptcb->OSTCBBitX;/*从该事件的等待列表中删除该任务*/
if(pevent->OSEventTbl[y] == 0) {/* ... remove task from ...            */
pevent->OSEventGrp &= ~ptcb->OSTCBBitY;/* ... event ctrl block                */
}
}
\#endif
\#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
pnode = ptcb->OSTCBFlagNode;
if(pnode != (OS_FLAG_NODE *)0) {/* If task is waiting on event flag  时间组标志  */
OS_FlagUnlink(pnode);/* Remove from wait list               */
}
\#endif
ptcb->OSTCBDly    = 0;/* Prevent OSTimeTick() from updating 置等待时间为0，放置系统时钟中断更新此值 */
ptcb->OSTCBStat   = OS_STAT_RDY;/* Prevent task from being resumed  防止任务在此运行   */
ptcb->OSTCBPendTO = FALSE;/*清除等待超时标志*/
if(OSLockNesting < 255u) {/* Make sure we don't context switch 禁止任务调度  */
OSLockNesting++;
}
OS_EXIT_CRITICAL();/* 标记2 Enabling INT. ignores next instruc. */
OS_Dummy();/* ... Dummy ensures that INTs will be 等待一个时间周期*/
OS_ENTER_CRITICAL();/*标记3 ... disabled HERE!                  */
if(OSLockNesting > 0) {/* Remove context switch lock          */
OSLockNesting--;
}
OSTaskDelHook(ptcb);/* Call user defined hook              */
OSTaskCtr--;/* One less task being managed 任务计数器减1        */
OSTCBPrioTbl[prio] = (OS_TCB *)0;/* Clear old priority entry   清除该优先级任务控制块入口     */
if(ptcb->OSTCBPrev == (OS_TCB *)0) {/* Remove from TCB chain  如果当前任务是TCB链表的第一个成员   */
ptcb->OSTCBNext->OSTCBPrev = (OS_TCB *)0;/*从TCB链表中删除此控制块*/
OSTCBList                  = ptcb->OSTCBNext;
}else{/*如果不是第一个成员*/
ptcb->OSTCBPrev->OSTCBNext = ptcb->OSTCBNext;
ptcb->OSTCBNext->OSTCBPrev = ptcb->OSTCBPrev;
}
ptcb->OSTCBNext   = OSTCBFreeList;/* Return TCB to free TCB list         */
OSTCBFreeList     = ptcb;
\#if OS_TASK_NAME_SIZE > 1                                       /*如果允许设置任务名*/
ptcb->OSTCBTaskName[0] ='?';/* Unknown name                        */
ptcb->OSTCBTaskName[1] = OS_ASCII_NUL;
\#endif
OS_EXIT_CRITICAL();//标记4
OS_Sched();/* Find new highest priority task      */
return(OS_NO_ERR);
}
\#endif
这个函数由其参数就可以看出，删除任务实际上就是删除任务的优先级，借此可以看出，任务是由优先级来标识的。首先还是进行任务的检测，如果在中断中，或者优先级不存在，或者空闲任务等，就不会执行删除任务。
从上面代码的标记1开始进入临界状态，禁止中断，如果直到标记4退出临界区，开中断， 这个时间会比较长，不利于高实时性。所以在标记2的地方开中断，响应等待的中断，但是此时可能会引发新的调度，而无法返回当前任务，导致后续的代码没有得 到执行，所以这里将OSLockNesting加1，保证只会响应中断而不会发生任务调度。之后再次关中断了以后，将OSLockNesting减1，恢
 复原值。
下面看请求删除任务：
\#if OS_TASK_DEL_EN > 0
INT8U  OSTaskDelReq (INT8U prio)
{
INT8U      stat;
OS_TCB    *ptcb;
\#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
OS_CPU_SR  cpu_sr = 0;
\#endif

\#if OS_ARG_CHK_EN > 0
if(prio == OS_IDLE_PRIO) {/* Not allowed to delete idle task     */
return(OS_TASK_DEL_IDLE);
}
if(prio >= OS_LOWEST_PRIO) {/* Task priority valid ?               */
if(prio != OS_PRIO_SELF) {
return(OS_PRIO_INVALID);
}
}
\#endif
if(prio == OS_PRIO_SELF) {/* See if a task is requesting to ...  */
OS_ENTER_CRITICAL();/* ... this task to delete itself      */
stat = OSTCBCur->OSTCBDelReq;/* Return request status to caller     */
OS_EXIT_CRITICAL();
return(stat);
}
OS_ENTER_CRITICAL();
ptcb = OSTCBPrioTbl[prio];
if(ptcb == (OS_TCB *)0) {/* Task to delete must exist           */
OS_EXIT_CRITICAL();
return(OS_TASK_NOT_EXIST);/* Task must already be deleted        */
}
if(ptcb == (OS_TCB *)1) {/* Must NOT be assigned to a Mutex     */
OS_EXIT_CRITICAL();
return(OS_TASK_DEL_ERR);
}
ptcb->OSTCBDelReq = OS_TASK_DEL_REQ;/* Set flag indicating task to be DEL. */
OS_EXIT_CRITICAL();
return(OS_NO_ERR);
}
\#endif
这个删除任务的主要功能就是删除任务本身，该任务在执行时释放申请的资源，然后再删除自己。前面还是一样，检测参数，后面就检测是否要求删除自己，如果是的话返回存储在TCB中的标志。
最后，将任务控制块的OSTCBDelReq的值设为OS_TASK_DEL_REQ，当删除任务被调度的时候，会检测这个值，当这个值是OS_TASK_DEL_REQ时，则就会调用OSTaskDel
 ()函数。
接下来我们继续看Os_task.c中的任务管理函数。
获取任务名称：
\#if OS_TASK_NAME_SIZE > 1
INT8U  OSTaskNameGet (INT8U prio, INT8U *pname, INT8U *err)
{
OS_TCB    *ptcb;
INT8U      len;
\#if OS_CRITICAL_METHOD == 3                              /* Allocate storage for CPU status register   */
OS_CPU_SR  cpu_sr = 0;
\#endif

\#if OS_ARG_CHK_EN > 0
if(err == (INT8U *)0) {/* Validate 'err'                             */
return(0);
}
if(prio > OS_LOWEST_PRIO) {/* Task priority valid ?                      */
if(prio != OS_PRIO_SELF) {
*err = OS_PRIO_INVALID;/* No                                         */
return(0);
}
}
if(pname == (INT8U *)0) {/* Is 'pname' a NULL pointer?   如果pname为空             */
*err = OS_ERR_PNAME_NULL;/* Yes                                       */
return(0);
}
\#endif
OS_ENTER_CRITICAL();
if(prio == OS_PRIO_SELF) {/* See if caller desires it's own name       */
prio = OSTCBCur->OSTCBPrio;
}
ptcb = OSTCBPrioTbl[prio];
if(ptcb == (OS_TCB *)0) {/* Does task exist?                          */
OS_EXIT_CRITICAL();/* No                                        */
*err = OS_TASK_NOT_EXIST;
return(0);
}
if(ptcb == (OS_TCB *)1) {/* Task assigned to a Mutex?                 */
OS_EXIT_CRITICAL();/* Yes                                       */
*err = OS_TASK_NOT_EXIST;
return(0);
}
len  = OS_StrCopy(pname, ptcb->OSTCBTaskName);/* Yes, copy name from TCB     拷贝任务名              */
OS_EXIT_CRITICAL();
*err = OS_NO_ERR;
return(len);
}
\#endif
这个函数主要就是通过优先级，获取到任务的TCB信息，最后通过OS_StrCopy()函数，将任务名赋值给len，最终将其返回。
设置任务名：
\#if OS_TASK_NAME_SIZE > 1
voidOSTaskNameSet (INT8U prio, INT8U *pname, INT8U *err)
{
INT8U      len;
OS_TCB    *ptcb;
\#if OS_CRITICAL_METHOD == 3                          /* Allocate storage for CPU status register       */
OS_CPU_SR  cpu_sr = 0;
\#endif

\#if OS_ARG_CHK_EN > 0
if(err == (INT8U *)0) {/* Validate 'err'                                 */
return;
}
if(prio > OS_LOWEST_PRIO) {/* Task priority valid ?                          */
if(prio != OS_PRIO_SELF) {
*err = OS_PRIO_INVALID;/* No                                             */
return;
}
}
if(pname == (INT8U *)0) {/* Is 'pname' a NULL pointer?                     */
*err = OS_ERR_PNAME_NULL;/* Yes                                            */
return;
}
\#endif
OS_ENTER_CRITICAL();
if(prio == OS_PRIO_SELF) {/* See if caller desires to set it's own name     */
prio = OSTCBCur->OSTCBPrio;
}
ptcb = OSTCBPrioTbl[prio];
if(ptcb == (OS_TCB *)0) {/* Does task exist?                               */
OS_EXIT_CRITICAL();/* No                                             */
*err = OS_TASK_NOT_EXIST;
return;
}
if(ptcb == (OS_TCB *)1) {/* Task assigned to a Mutex?                      */
OS_EXIT_CRITICAL();/* Yes                                            */
*err = OS_TASK_NOT_EXIST;
return;
}
len = OS_StrLen(pname);/* Yes, Can we fit the string in the TCB?         */
if(len > (OS_TASK_NAME_SIZE - 1)) {/*      No                                        */
OS_EXIT_CRITICAL();
*err = OS_ERR_TASK_NAME_TOO_LONG;
return;
}
(void)OS_StrCopy(ptcb->OSTCBTaskName, pname);/*      Yes, copy to TCB                          */
OS_EXIT_CRITICAL();
*err = OS_NO_ERR;
}
\#endif
通过OS_StrLen()函数获取预设值的名称的长度，然后检测其长度是否合理，最后通过OS_StrCopy()函数拷贝任务名到TCB中。
挂起任务：
\#if OS_TASK_SUSPEND_EN > 0
INT8U  OSTaskSuspend (INT8U prio)
{
BOOLEANself;
OS_TCB    *ptcb;
INT8U      y;
\#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
OS_CPU_SR  cpu_sr = 0;
\#endif

\#if OS_ARG_CHK_EN > 0
if(prio == OS_IDLE_PRIO) {/* Not allowed to suspend idle task    */
return(OS_TASK_SUSPEND_IDLE);
}
if(prio >= OS_LOWEST_PRIO) {/* Task priority valid ?               */
if(prio != OS_PRIO_SELF) {
return(OS_PRIO_INVALID);
}
}
\#endif
OS_ENTER_CRITICAL();
if(prio == OS_PRIO_SELF) {/* See if suspend SELF                 */
prio = OSTCBCur->OSTCBPrio;
self = TRUE;
}elseif(prio == OSTCBCur->OSTCBPrio) {/* See if suspending self              */
self = TRUE;
}else{
self = FALSE;/* No suspending another task          */
}
ptcb = OSTCBPrioTbl[prio];
if(ptcb == (OS_TCB *)0) {/* Task to suspend must exist          */
OS_EXIT_CRITICAL();
return(OS_TASK_SUSPEND_PRIO);
}
if(ptcb == (OS_TCB *)1) {/* See if assigned to Mutex            */
OS_EXIT_CRITICAL();
return(OS_TASK_NOT_EXIST);
}
y            = ptcb->OSTCBY;
OSRdyTbl[y] &= ~ptcb->OSTCBBitX;/* Make task not ready                 */
if(OSRdyTbl[y] == 0) {
OSRdyGrp &= ~ptcb->OSTCBBitY;
}
ptcb->OSTCBStat |= OS_STAT_SUSPEND;/* Status of task is 'SUSPENDED'       */
OS_EXIT_CRITICAL();
if(self == TRUE) {/* Context switch only if SELF         */
OS_Sched();
}
return(OS_NO_ERR);
}
\#endif
这个函数主要注意self值的设定，当需要挂起的是当前任务的话，则self为TRUE，当self为TRUE时，需要进行任务调度。还有将OSTCBStat设定为OS_STAT_SUSPEND。还有要将就绪表中的任务删除。
恢复任务：
\#if OS_TASK_SUSPEND_EN > 0
INT8U  OSTaskResume (INT8U prio)
{
OS_TCB    *ptcb;
\#if OS_CRITICAL_METHOD == 3                                   /* Storage for CPU status register       */
OS_CPU_SR  cpu_sr = 0;
\#endif

\#if OS_ARG_CHK_EN > 0
if(prio >= OS_LOWEST_PRIO) {/* Make sure task priority is valid      */
return(OS_PRIO_INVALID);
}
\#endif
OS_ENTER_CRITICAL();
ptcb = OSTCBPrioTbl[prio];
if(ptcb == (OS_TCB *)0) {/* Task to suspend must exist            */
OS_EXIT_CRITICAL();
return(OS_TASK_RESUME_PRIO);
}
if(ptcb == (OS_TCB *)1) {/* See if assigned to Mutex              */
OS_EXIT_CRITICAL();
return(OS_TASK_NOT_EXIST);
}
if((ptcb->OSTCBStat & OS_STAT_SUSPEND) != OS_STAT_RDY) {/* Task must be suspended                */
ptcb->OSTCBStat &= ~OS_STAT_SUSPEND;/* Remove suspension                     */
if(ptcb->OSTCBStat == OS_STAT_RDY) {/* See if task is now ready              */
if(ptcb->OSTCBDly == 0) {
OSRdyGrp               |= ptcb->OSTCBBitY;/* Yes, Make task ready to run           */
OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
OS_EXIT_CRITICAL();
OS_Sched();
}else{
OS_EXIT_CRITICAL();
}
}else{/* Must be pending on event              */
OS_EXIT_CRITICAL();
}
return(OS_NO_ERR);
}
OS_EXIT_CRITICAL();
return(OS_TASK_NOT_SUSPENDED);
}
\#endif
其主要就是检测任务是否为挂起，然后更改挂起状态，并且更新就绪表。
读取任务的TCB信息：
\#if OS_TASK_QUERY_EN > 0
INT8U  OSTaskQuery (INT8U prio, OS_TCB *p_task_data)
{
OS_TCB    *ptcb;
\#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
OS_CPU_SR  cpu_sr = 0;
\#endif

\#if OS_ARG_CHK_EN > 0
if(prio > OS_LOWEST_PRIO) {/* Task priority valid ?                              */
if(prio != OS_PRIO_SELF) {
return(OS_PRIO_INVALID);
}
}
if(p_task_data == (OS_TCB *)0) {/* Validate 'p_task_data'                             */
return(OS_ERR_PDATA_NULL);
}
\#endif
OS_ENTER_CRITICAL();
if(prio == OS_PRIO_SELF) {/* See if suspend SELF                                */
prio = OSTCBCur->OSTCBPrio;
}
ptcb = OSTCBPrioTbl[prio];
if(ptcb == (OS_TCB *)0) {/* Task to query must exist                           */
OS_EXIT_CRITICAL();
return(OS_PRIO_ERR);
}
if(ptcb == (OS_TCB *)1) {/* Task to query must not be assigned to a Mutex      */
OS_EXIT_CRITICAL();
return(OS_TASK_NOT_EXIST);
}
/* Copy TCB into user storage area                    */
OS_MemCopy((INT8U *)p_task_data, (INT8U *)ptcb,sizeof(OS_TCB));
OS_EXIT_CRITICAL();
return(OS_NO_ERR);
}
\#endif
将多选优先级的任务的TCB信息copy给指定的TCB结构体中，然后进行读取。
通过上述的许多的任务管理函数，有没有发现其实都是比较简单，他们的参数检测其实都是比较固定的，在μC/OS-II系统中，任务标识的最重要的参数是优先级，它可以标识唯一的一个任务。


