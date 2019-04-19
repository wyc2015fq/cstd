# Live555  源码分析之延迟事件处理 - 建建的博客 - CSDN博客
2017年01月25日 15:50:32[纪建](https://me.csdn.net/u013898698)阅读数：453
live555的延迟事件
       主要存放在BasicTaskScheduler0的成员变量
       DelayQueue fDelayQueue;中
       其中
DelayQueue 是 DelayQueueEntry的子类
DelayQueue 含有对延迟事件单元的增删改查
       DelayQueueEntry中有DelayInterval fDeltaTimeRemaining;//任务剩余时间
       但是DelayQueue 存储的对象并不是DelayQueueEntry
       而是AlarmHandler
AlarmHandler也是DelayQueueEntry的子类
AlarmHandler中有二个成员变量
      TaskFunc* fProc;延迟事件到了以后需要执行的函数
      void* fClientData;延迟函数的参数
      检测延迟函数是否到期是在SingleStep中
 
     调用BasicTaskScheduler0中的
 
    Synchronize()
 
   该函数主要计算延迟队列第一个元素有没有到期
 
   实际上每次有新的延迟任务加到队列中的时候都会重新计算时间，每一项中fDeltaTimeRemaining都是与前一项的差值。延迟任务的执行函数是 fDelayQueue.handleAlarm();   
 
   延迟任务的添加是
 
   scheduleDelayedTask(int64_t microseconds,TaskFunc* proc,void* clientData)函数
 
   添加的时候会传入需要等待的时间，回调函数，回调函数参数
 
   以及添加到队列哪个位置
 
   返回延迟事件唯一标示
 
   unscheduleDelayedTask(TaskToken& prevTask)
 
   根据标识符删除延迟事件
