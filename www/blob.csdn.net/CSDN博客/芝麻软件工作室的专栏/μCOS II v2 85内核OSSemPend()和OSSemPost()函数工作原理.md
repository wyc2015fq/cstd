
# μCOS-II v2.85内核OSSemPend()和OSSemPost()函数工作原理 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月10日 16:57:24[seven-soft](https://me.csdn.net/softn)阅读数：525


//----------------------------------------------------------------------
//1.OSSemPend()函数
voidOSSemPend(OS_EVENT*pevent,INT16U timeout,INT8U*perr)
{
INT8U pend_stat;
\#ifOS_CRITICAL_METHOD==3
OS_CPU_SR cpu_sr=0;
\#endif
\#ifOS_ARG_CHK_EN>0
if(perr==(INT8U*)0){
return;
}
if(pevent==(OS_EVENT*)0){
*perr=OS_ERR_PEVENT_NULL;
return;
}
\#endif
if(pevent->OSEventType!=OS_EVENT_TYPE_SEM){
//确保该event控制块是Sem类型
*perr=OS_ERR_EVENT_TYPE;
return;
}
if(OSIntNesting>0){
//ISR中,不能使用OSSemPend()
*perr=OS_ERR_PEND_ISR;
return;
}
if(OSLockNesting>0){
//μCOS/II v2.85内核已经被强制锁住
*perr=OS_ERR_PEND_LOCKED;
return;
}
//非法的统统不是,信号正常,所以有必要进一步处理
OS_ENTER_CRITICAL();
if(pevent->OSEventCnt>0){
//程序的其他地方已经触发了事件,异或在初始化时设定了n,如:OSSemCreate(2);
//所以该task无需悬停,直接获得事件的使用权
pevent->OSEventCnt--;
OS_EXIT_CRITICAL();
*perr=OS_ERR_NONE;
return;
}
//当前还没有任何事件发生,所以本task需要悬停,让出cpu[gliethttp]
OSTCBCur->OSTCBStat|=OS_STAT_SEM;//是sem事件让本task进入悬停等待的
OSTCBCur->OSTCBStatPend=OS_STAT_PEND_OK;//假定不是超时,为正常收到信号
//超时,如果timeout=0,那么,本task将一直悬停,仅仅当收到事件触发信号后才重新进入调度队列
OSTCBCur->OSTCBDly=timeout;
//OS_EventTaskWait()函数实现的功能:
//把本task从就绪控制矩阵中摘下,放到pevent事件专有的进程事件控制矩阵表中.
OS_EventTaskWait(pevent);
OS_EXIT_CRITICAL();
//因为本task正在运行,所以本task现在的优先级最高,现在本task已经将自己从就绪控制矩阵--调度器(x,y)矩形阵列中
//把自己摘掉,所以调度函数OS_Sched()一定会切换到另一个task中执行新task的代码[gliethttp]
OS_Sched();//具体参见《浅析μC/OS-II v2.85内核调度函数》
//2007-09-09 gliethttp
//可能因为OSSemPend()中指定的timeout已经超时
//[由OSTimeTick()函数把本task重新置入了就绪控制矩阵,具体参考《浅析μC/OS-II v2.85内核OSTimeDly()函数工作原理》],
//又或者确实在应用程序的某个地方调用了OSSemPost(),以下代码将具体解析是有什么引起的:1.超时,2.收到正常信号
OS_ENTER_CRITICAL();
if(OSTCBCur->OSTCBStatPend!=OS_STAT_PEND_OK){
//是因为timeout超时,使得本task获得重新执行的机会
pend_stat=OSTCBCur->OSTCBStatPend;
//清除event事件块上本task的标志
OS_EventTOAbort(pevent);
OS_EXIT_CRITICAL();
switch(pend_stat){
caseOS_STAT_PEND_TO:
default:
*perr=OS_ERR_TIMEOUT;
break;
caseOS_STAT_PEND_ABORT:
*perr=OS_ERR_PEND_ABORT;
break;
}
return;
}
//由OSSemPost()抛出正常事件,唤醒了本task,因为在OSSemPost()时,
//已经将本task在event事件控制矩阵上的对应位清除掉,并且把本task放入了就绪控制矩阵中,
//否则本task也不会执行至此.
OSTCBCur->OSTCBEventPtr=(OS_EVENT*)0;//现在本task不悬停在任何event事件上
OS_EXIT_CRITICAL();
*perr=OS_ERR_NONE;
}
//----------------------------------------------------------------------
//2.OS_EventTaskWait()函数
voidOS_EventTaskWait(OS_EVENT*pevent)
{
INT8U y;
//2007-09-09 gliethttp
//pevent为此次task挂起的EventPtr单元
OSTCBCur->OSTCBEventPtr=pevent;
//清除调度器中该task对应的标志位
y=OSTCBCur->OSTCBY;
OSRdyTbl[y]&=~OSTCBCur->OSTCBBitX;
if(OSRdyTbl[y]==0){
//当前y行对应的8个或16个task都已经悬停,那么当前y行也清除.
OSRdyGrp&=~OSTCBCur->OSTCBBitY;
}
//2007-09-09 gliethttp
//将该task的prio添加到pevent事件控制矩阵中,这个矩阵的功能和OSRdyGrp、OSRdyTbl没有区别
//都是用来计算出已经就绪tasks中的优先级最高的那个
pevent->OSEventTbl[OSTCBCur->OSTCBY]|=OSTCBCur->OSTCBBitX;
pevent->OSEventGrp|=OSTCBCur->OSTCBBitY;
}
//----------------------------------------------------------------------
//3.OS_EventTOAbort()函数
voidOS_EventTOAbort(OS_EVENT*pevent)
{
INT8U y;
//清除event事件控制矩阵上本task的标志,因为OSTimeTick()函数未清除该单元
//它仅仅把本task放入就绪控制矩阵中,使得本task重新获得被OS调度的机会而已
//具体的清除工作还要自己完成
//具体参考《浅析μC/OS-II v2.85内核OSTimeDly()函数工作原理》
y=OSTCBCur->OSTCBY;
pevent->OSEventTbl[y]&=~OSTCBCur->OSTCBBitX;
if(pevent->OSEventTbl[y]==0x00){
pevent->OSEventGrp&=~OSTCBCur->OSTCBBitY;
}
OSTCBCur->OSTCBStatPend=OS_STAT_PEND_OK;
OSTCBCur->OSTCBStat=OS_STAT_RDY;
OSTCBCur->OSTCBEventPtr=(OS_EVENT*)0;//现在本task不悬停在任何event事件上
}
//----------------------------------------------------------------------
//4.OSSemPost()函数
INT8U OSSemPost(OS_EVENT*pevent)
{
\#ifOS_CRITICAL_METHOD==3
OS_CPU_SR cpu_sr=0;//方式3将把cpsr状态寄存器推入临时堆栈cpu_sr中,可以安全返回之前的中断状态
\#endif
\#ifOS_ARG_CHK_EN>0
if(pevent==(OS_EVENT*)0){
return(OS_ERR_PEVENT_NULL);
}
\#endif
if(pevent->OSEventType!=OS_EVENT_TYPE_SEM){
return(OS_ERR_EVENT_TYPE);
}
OS_ENTER_CRITICAL();
if(pevent->OSEventGrp!=0){
//2007-09-09 gliethttp
//OS_EventTaskRdy()函数将摘掉等待在pevent事件控制矩阵上的task中优先级最高的task
//如果该task仅仅等待该pevent事件,那么将该task添加到就绪控制矩阵中
//OSRdyGrp |= bity;
//OSRdyTbl[y] |= bitx;这样调度程序就会根据情况调度OS_Sched()该task了
(void)OS_EventTaskRdy(pevent,(void*)0,OS_STAT_SEM,OS_STAT_PEND_OK);
OS_EXIT_CRITICAL();
//可能刚刚放到就绪控制矩阵上的被唤醒的task的优先级比调用OSSemPost()函数的进程B优先级高
//所以需要调用shedule函数,
//如果真的高,那么调用OSSemPost()函数的进程B就要被抢占,os将会切换到新的task去执行[gliethttp]
//如果没有调用OSSemPost()函数的进程B优先级高,那么os不会切换,仍然继续执行进程B,OSSemPost()正常返回
OS_Sched();
return(OS_ERR_NONE);
}
//没有任何一个task悬停在本event事件控制矩阵上,
//那么单纯的对pevent->OSEventCnt++加操作.
if(pevent->OSEventCnt<65535u){
pevent->OSEventCnt++;
OS_EXIT_CRITICAL();
return(OS_ERR_NONE);
}
//已经堆积了0xffff次,溢出
OS_EXIT_CRITICAL();
return(OS_ERR_SEM_OVF);
}
//----------------------------------------------------------------------
//5.OS_EventTaskRdy()函数
INT8U OS_EventTaskRdy(OS_EVENT*pevent,void*pmsg,INT8U
 msk,INT8U pend_stat)
{
OS_TCB*ptcb;
INT8U x;
INT8U y;
INT8U prio;
\#ifOS_LOWEST_PRIO<=63
INT8U bitx;
INT8U bity;
\#else
INT16U bitx;
INT16U bity;
INT16U*ptbl;
\#endif
\#ifOS_LOWEST_PRIO<=63
//小于64个task时,快速计算
//最有优先权的task位于事件控制矩阵中的第y行的第x列
y=OSUnMapTbl[pevent->OSEventGrp];
bity=(INT8U)(1<<y);
x=OSUnMapTbl[pevent->OSEventTbl[y]];
bitx=(INT8U)(1<<x);
prio=(INT8U)((y<<3)+x);
\#else
//对于256个task
//最有优先权的task位于事件控制矩阵中的第y行的第x列
//以下的操作原理具体参见《浅析μC/OS-II v2.85内核调度函数》
if((pevent->OSEventGrp&0xFF)!=0){
y=OSUnMapTbl[pevent->OSEventGrp&0xFF];
}else{
y=OSUnMapTbl[(pevent->OSEventGrp>>8)&0xFF]+8;
}
bity=(INT16U)(1<<y);
ptbl=&pevent->OSEventTbl[y];
if((*ptbl&0xFF)!=0){
x=OSUnMapTbl[*ptbl&0xFF];
}else{
x=OSUnMapTbl[(*ptbl>>8)&0xFF]+8;
}
bitx=(INT16U)(1<<x);
prio=(INT8U)((y<<4)+x);//该task对应的prio优先级值
//ok,等待在event事件上的所有task中,只有在事件控制矩阵中的第y行的第x列task
//优先级最高、最值的成为此次事件的唤醒对象[gliethttp]
\#endif
//清除此task在event事件控制矩阵中的bit位
pevent->OSEventTbl[y]&=~bitx;
if(pevent->OSEventTbl[y]==0){
pevent->OSEventGrp&=~bity;
}
//通过prio优先级找到该prio唯一对应的task对应的ptcb进程上下文控制块
ptcb=OSTCBPrioTbl[prio];
ptcb->OSTCBDly=0;//复原为正常
ptcb->OSTCBEventPtr=(OS_EVENT*)0;//现在本task不悬停在任何event事件上
\#if((OS_Q_EN>0)&&(OS_MAX_QS>0))||(OS_MBOX_EN>0)
ptcb->OSTCBMsg=pmsg;//传递消息指针
\#else
pmsg=pmsg;
\#endif
ptcb->OSTCBStatPend=pend_stat;//悬停状态值
ptcb->OSTCBStat&=~msk;//该msk事件已经发生,清除task上下文控制块上的msk位,如:OS_STAT_SEM
if(ptcb->OSTCBStat==OS_STAT_RDY){
//如果当前task只是等待该事件,那么把该task放到就绪控制矩阵中,允许内核调度本task
OSRdyGrp|=bity;
OSRdyTbl[y]|=bitx;
}
return(prio);//返回本task对应的优先级值
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
PS:"所以从这里来看,os中的各个功能单元管理着自己的事情,就像面向对象的封装一样,"
"事件控制矩阵和就绪控制矩阵是各个对象独立自治的关键因素"
"其他对象,都努力说服自己相信别的对象是独立的、可信任的、安全的"[gliethttp]
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

