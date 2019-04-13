
# uCOS II 之事件控制块学习 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月13日 10:29:38[seven-soft](https://me.csdn.net/softn)阅读数：304


一。 事件控制块ECB数据结构：
typedef struct
{
INT8U OSEventType; //事件类型
INT8U OSEventGrp; //等待任务所在的组
INT16U OSEventCnt; //当事件是信号量时的计数器
void *OSEventPtr; //指向消息或消息队列的指针
INT8U OSEventTbl［OS_EVENT_TBL_SIZE］; //等待任务列表
} OS_EVENT;
二．关键[算法](http://www.ofweek.com/SEARCH/WENZHANG/%E7%AE%97%E6%B3%95.HTM)：
1、将一个任务插入到等待事件的任务列表中：
当调用函数OS_EventTaskWait（ ）使一个任务进入等待某事件发生时，需要此算法，从而将任务插入到等待事件的任务列表中。
pevent -》 OSEventGrp |= OSMapTbl［prio 》》 3］;
pevent -》 OSEventTbl［prio》》3］ = OSMapTbl［prio & 0x07］;
2、从等待事件的任务列表中使任务脱离等待状态：
当调用函数OS_EventTaskRdy（ ）使一个任务进入就绪态时，需要调用此算法，从而使等待的任务脱离等待状态进入就绪。
if （（pevent -》 OSEventTbl［prio 》》3］ &= ~OSMapTbl［prio & 0x07］） == 0）
{
pevent -》 OSEventGrp &= ~OSMapTbl［prio 》》3］;
}
3、在等待事件的任务列表中查找优先级最高的任务：
当调用函数OS_EventTaskRdy（ ）使一个任务进入就绪态时，需要调用此算法，从而首先找出在等待事件任务列表中最高优先级的任务进入就绪状态。
y = OSUnMapTbl［pevent -》 OSEventGrp］;
x = OSUnMapTbl［pevent -》 OSEventTbl［y］;
prio = （y 《《 3） + x;
三．对事件控制块ECB的基本操作
1、初始化一个事件控制块：
函数OSS[emC](http://www.ofweek.com/SEARCH/WENZHANG/emc.HTM)reate（ ），OSMutexCreate（
 ），OSMboxCreate（ ），OSQCreate（ ）建立时，必须调用此函数进行初始化，初始化一个空的等待列表，表中没有任何等待事件的任务。
OS_EventWaitListInit（ ）;
2、使一个任务进入就绪态：
当某个事件发生了时，要将事件等待任务列表中最高优先级的任务进入就绪态，函数OSSemPost，OSMutexPost（ ），OSMboxPost（ ），OSQPost（ ）必将调用此函数从而使一个任务进入就绪态。
OS_EventTaskRdy（ ）;
3、使一个任务进入等待某事件发生：
当某个任务须等待一个事件的发生时，信号量、互斥型信号量、邮箱、消息队列会通过相应的PEND函数来调用这个函数。
OS_EventTaskWait（ ）;
4、由于等待超时而将任务置为就绪态：
如果在预先指定的等待时间内任务等待的事件没有发生，那么PNED类型函数将会调用此函数从而将等待超时的任务进入就绪态。
OS_EventTo（ ）;
四．事件控制块ECB基本操作代码的分析。
1、OS_EventWaitListInit（ ）;
void OS_EventWaitListInit（OS_EVENT *pevent）
{
INT8U *ptbl; //定义指针变量ptbl
pevent -》 OSEventGrp = 0x00; //清除任务所在的组
OS_EVENT_TBL_SIZE在UCOS_ II.H中定义大小
ptbl = &pevent -》 OSEventTbl［0］;
\#if OS_EVENT_TBL_SIZE 》 0
*ptbl++ = 0x00; //清除等待任务列表，在这里没有使用for循环是为了节省系统开销
\#endif
\#if OS_EVENT_TBL_SIZE 》 1
*ptbl++ = 0x00;
\#endif
\#if OS_EVENT_TBL_SIZE 》 2
*ptbl++ = 0x00;
\#endif
\#if OS_EVENT_TBL_SIZE 》 3
*ptbl++ = 0x00;
\#endif
\#if OS_EVENT_TBL_SIZE 》 4
*ptbl++ = 0x00;
\#endif
\#if OS_EVENT_TBL_SIZE 》 5
*ptbl++ = 0x00;
\#endif
\#if OS_EVENT_TBL_SIZE 》 6
*ptbl++ = 0x00;
\#endif
\#if OS_EVENT_TBL_SIZE 》 7
*ptbl++ = 0x00;
\#endif
}
2、OS_EventTaskRdy（ ）;
INT8U OS_EventTaskRdy（OS_EVENT *pevent， void *msg，INT8U msk）
{
OS_TCB *ptcb;
INT8U x;
INT8U y;
按照关键算法3，在等待事件的任务列表中找到优先级最高的任务，并确定其优先级。
INT8U bitx;
INT8U bity;
INT8U prio;
y = OSUnMapTbl［pevent -》 OSEventGrp］;
bity = OSMapTbl［y］;
x = OSUnMapTbl［pevent -》 OSEventTbl［y］］;
利用算法2，从等待任务列表中删除此优先级最高的任务。
bitx = OSMapTbl［x］;
prio = （INT8U）（（ y 《《 3） + x）;
if （（pevent -》 OSEventTbl［y］ &= ~bitx） == 0x00）
{ pevent -》 OSEventGrp &= ~bity;}
ptcb = O[STC](http://www.ofweek.com/SEARCH/WENZHANG/stc.HTM)BPrioTbl［prio］;
 //知道了任务优先级，从而找到任务控制块的指针
ptcb -》 OSTCBDly = 0; //由于此任务已经不再等待事件的发生，因此直接清0
ptcb -》 OSTCBEventPtr = （OS_EVENT *）0;//由于不再等待事件的发生，使指向事件控制块的指针指向NULL
\#if （（OS_Q_EN 》 0） && （OS_MAX_QS 》 0）） || （OS_MBOX_EN 》 0）
ptcb -》 OSTCBMsg = msg;//如果此函数是邮箱或队列POST函数调用的，则需要将传递来的参数放到它的任务控制块中
\#else
msg = msg;
\#endif
ptcb -》 OSTCBStat &= ~msk;
if （ptcb -》 OSTCBStat == OS_STAT_RDY）］
{
OSRdyGrp |= bity; ---最高优先级的任务插入到就绪任务列表，由后续POST函数中的
OSRdyTbl［y］ |= bitx; ---OSSched（ ）函数进行[调度](http://www.ofweek.com/SEARCH/WENZHANG/%E8%B0%83%E5%BA%A6.HTM)使任务进入就绪，请参考第三章之就绪表
}
return （prio）;
}
3、OS_EventTaskWait（ ）;
void OS_EventTaskWait（OS_EVENT *pevent）
{
OSTCBCur -》 OSTCBEventPtr = pevent;
if （（ OSRdyTbl［OSTCBCur -》 OSTCBY］ &= ~OSTCBCur-》OSTCBBitx） == 0x00）
{ OSRdyGrp &= ~OSTCBCur -》 OSTCBBity; } //如果在PEND函数中没有收到有效的信号量，
//那么将调用此函数进入睡眠态，在这里将任务在任务就绪列表中删除
pevent -》 OSEventTbl［OSTCBCur -》 OSTCBY］ |= OSTCBCur -》 OSTCBBitx;
pevent -》 OSEventGrp |= OSTCBCur -》 OSTCBBity;//把此任务放到ECB的等待事件的任务列表中去
}
4、OS_EventTo（ ）;
void OS_EventTO （OS_EVENT *pevent）
{
if （（ pevent -》 OSEventTbl［OSTCBCur -》 OSTCBY］ &= ~OSTCBCur -》 OSTCBBitx） ==0x00）
{ pevent -》 OSEventGrp &= ~OSTCBCur -》 OSTCBBitY; }//将超时的任务在等待任务列表中删除
OSTCBCur -》 OSTCBStat = OS_STAT_RDY; //任务置为就绪态
OSTCBCur -》 OSTCBEventPtr = （OS_EVENT *）0;

