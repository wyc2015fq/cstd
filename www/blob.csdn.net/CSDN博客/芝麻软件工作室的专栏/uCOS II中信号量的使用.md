
# uCOS II中信号量的使用 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月13日 10:27:44[seven-soft](https://me.csdn.net/softn)阅读数：1899


对于信号量，uCOS II中共提供了六个函数：OSSemCreate 、OSSemDel、OSSemPend、OSSemPost、OSSemAccept、OSSemQuery。其中我们常用的只有三个即 OSSemCreate 、OSSemPend、OSSemPost 。
信号量的结构为：
typedef struct {
INT8U   OSEventType;
INT8U   OSEventGrp;
INT16U  OSEventCnt;
void   *OSEventPtr;
INT8U   OSEventTbl[OS_EVENT_TBL_SIZE];
} OS_EVENT;
OSSemCreate的函数原型为：OS_EVENT *OSSemCreate(INT16U cnt)，cnt 表示在实际使用中有多少个相同的资源，cnt的值就是OSEventCnt的值。
OSSemPend的函数原型为：void OSSemPend(OS_EVENT *pevent,INT16U timeout,INT8U err)，pevent：就是要请求的信号量地址；timeout：请求的超时时间，如果为0表示无限时等待；err：用来返回是否出错。每执行一次 OSSemPend，OSEventCnt就会减1。如果OSEventCnt值为0，执行OSSemPend时，当时任务将因为无法得到相应的资源而被 挂起。
OSSemPost的函数原型为：INT8U OSSemPost(OS_EVENT *pevent)，同样pevent 就是要发送的信号量地址；每执行一次OSSemPost，OSEventCnt就会加1。

