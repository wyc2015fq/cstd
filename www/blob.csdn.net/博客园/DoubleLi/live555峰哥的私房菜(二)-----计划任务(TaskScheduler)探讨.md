# live555峰哥的私房菜(二)-----计划任务(TaskScheduler)探讨 - DoubleLi - 博客园






# 计划任务(TaskScheduler)探讨 

上一篇谈到SingleStep()函数会找到三种任务类型并执行之。

这三种任务是：

socket handler, event handler, delay task 。 

1、socket handler 保存在队列BasicTaskScheduler0::HandlerSet* fHandlers中;

2、event handler保存在数组BasicTaskScheduler0::TaskFunc * fTriggeredEventHandlers[MAX_NUM_EVENT_TRIGGERS] 中;

3、delay task 保存在队列BasicTaskScheduler0::DelayQueue fDelayQueue 中。 

下面看一下三种任务的执行函数的定义： 
socket handler 为 
typedef void BackgroundHandlerProc (void* clientData, int mask); 
event handler为 
typedef void TaskFunc(void* clientData); 
delay task  为 
typedef void TaskFunc(void* clientData);// 跟event handler一样。 
再看一下向任务调度对象添加三种任务的函数的样子： 
socket handler 为： 
void setBackgroundHandling(int socketNum, int conditionSet   ,BackgroundHandlerProc* 
handlerProc, void* clientData)

event handler为: 
EventTriggerId createEventTrigger(TaskFunc* eventHandlerProc) 
delay task 为： 
TaskToken scheduleDelayedTask(int64_t  microseconds, TaskFunc* proc,void* 
clientData)



socket handler 添加时为什么需要那些参数呢？socketNum 是需要的，因为要select socket
（socketNum 即是socket() 返回的那个socket 对象）。conditionSet 也是需要的，它用于
表明socket 在select 时查看哪种装态，是可读？可写？还是出错？proc 和clientData 这两
个参数就不必说了（真有不明白的吗？）。再看BackgroundHandlerProc 的参数，socketNum
不必解释，mask是什么呢？它正是对应着 conditionSet ，但它表明的是 select 之后的结果，
比如一个socket 可能需要检查其读/ 写状态，而当前只能读，不能写，那么mask中就只有
表明读的位被设置。

- 



void BasicTaskScheduler


- 



  ::setBackgroundHandling(int socketNum, int conditionSet, BackgroundHandlerProc* handlerProc, void* clientData) {


- 



if (socketNum < 0) return;


- 



  FD_CLR((unsigned)socketNum, &fReadSet);


- 



  FD_CLR((unsigned)socketNum, &fWriteSet);


- 



  FD_CLR((unsigned)socketNum, &fExceptionSet);


- 



if (conditionSet == 0) {


- 



    fHandlers->clearHandler(socketNum);


- 



if (socketNum+1 == fMaxNumSockets) {


- 



      --fMaxNumSockets;


- 



    }


- 



  } else {


- 



    fHandlers->assignHandler(socketNum, conditionSet, handlerProc, clientData);


- 



if (socketNum+1 > fMaxNumSockets) {


- 



      fMaxNumSockets = socketNum+1;


- 



    }


- 



if (conditionSet&SOCKET_READABLE) FD_SET((unsigned)socketNum, &fReadSet);


- 



if (conditionSet&SOCKET_WRITABLE) FD_SET((unsigned)socketNum, &fWriteSet);


- 



if (conditionSet&SOCKET_EXCEPTION) FD_SET((unsigned)socketNum, &fExceptionSet);


- 



  }


- 



}


event handler是被存在数组中。数组大小固定，是32项，用 EventTriggerId 来表示数组中
的项，EventTriggerId 是一个32位整数，因为数组是32项，所以用EventTriggerId 中的
第n 位置１表明对应数组中的第n 项。成员变量fTriggersAwaitingHandling 也是
EventTriggerId 类型，它里面置 1 的那些位对应了数组中所有需要处理的项。这样做节省了
内存和计算，但降低了可读性，呵呵，而且也不够灵活，只能支持32项或64项，其它数
量不被支持。以下是函数体 

- 



EventTriggerId BasicTaskScheduler0::createEventTrigger(TaskFunc* eventHandlerProc) {


- 



unsigned i = fLastUsedTriggerNum;


- 



  EventTriggerId mask = fLastUsedTriggerMask;


- 




- 



do {


- 



    i = (i+1)%MAX_NUM_EVENT_TRIGGERS;


- 



    mask >>= 1;


- 



if (mask == 0) mask = 0x80000000;


- 




- 



if (fTriggeredEventHandlers[i] == NULL) {


- 



// This trigger number is free; use it:


- 



      fTriggeredEventHandlers[i] = eventHandlerProc;


- 



      fTriggeredEventClientDatas[i] = NULL; // sanity


- 




- 



      fLastUsedTriggerMask = mask;


- 



      fLastUsedTriggerNum = i;


- 




- 



return mask;


- 



    }


- 



  } while (i != fLastUsedTriggerNum);


- 




- 



// All available event triggers are allocated; return 0 instead:


- 



return 0;


- 



}


可以看到最多添加32个事件，且添加事件时没有传入 clientData 参数。这个参数在
触发事件时传入，见以下函数： 


- 



void BasicTaskScheduler0::triggerEvent(EventTriggerId eventTriggerId, void* clientData) {


- 



// First, record the "clientData".  (Note that we allow "eventTriggerId" to be a combination of bits for multiple events.)


- 



  EventTriggerId mask = 0x80000000;


- 



for (unsigned i = 0; i < MAX_NUM_EVENT_TRIGGERS; ++i) {


- 



if ((eventTriggerId&mask) != 0) {


- 



      fTriggeredEventClientDatas[i] = clientData;


- 



    }


- 



    mask >>= 1;


- 



  }


- 




- 



// Then, note this event as being ready to be handled.


- 



// (Note that because this function (unlike others in the library) can be called from an external thread, we do this last, to


- 



//  reduce the risk of a race condition.)


- 



  fTriggersAwaitingHandling |= eventTriggerId;


- 



}


看，clientData 被传入了，这表明 clientData 在每次触发事件时是可以变的。此时再回去看
SingleStep()是不是更明了了？ 


delay task 添加时，需要传入 task 延迟等待的微秒（百万分之一秒）数( 第一个参数)，这个
弱智也可以理解吧？嘿嘿。分析一下介个函数：

- 



TaskToken BasicTaskScheduler0::scheduleDelayedTask(int64_t microseconds,


- 



						 TaskFunc* proc,


- 



void* clientData) {


- 



if (microseconds < 0) microseconds = 0;


- 



DelayInterval timeToDelay((long)(microseconds/1000000), (long)(microseconds%1000000));


- 



  AlarmHandler* alarmHandler = new AlarmHandler(proc, clientData, timeToDelay);


- 



  fDelayQueue.addEntry(alarmHandler);


- 




- 



return (void*)(alarmHandler->token());


- 



}


 delay task的执行都在函数fDelayQueue.handleAlarm() 中，handleAlarm()在类
DelayQueue 中实现。看一下handleAlarm():   

- 



void DelayQueue::handleAlarm() {


- 



//如果第一个任务的执行时间未到，则同步一下（重新计算各任务的等待时间）。     


- 



if (head()->fDeltaTimeRemaining != DELAY_ZERO) synchronize();


- 



//如果第一个任务的执行时间到了，则执行第一个，并把它从队列中删掉。


- 



if (head()->fDeltaTimeRemaining == DELAY_ZERO) {


- 



// This event is due to be handled:


- 



    DelayQueueEntry* toRemove = head();


- 



    removeEntry(toRemove); // do this first, in case handler accesses queue


- 



//执行任务，执行完后会把这一项销毁。


- 



    toRemove->handleTimeout();


- 



  }


- 



}






可能感觉奇怪，其它的任务队列都是先搜索第一个应该执行的项，然后再执行，这里干脆，
直接执行第一个完事。那就说明第一个就是最应该执行的一个吧？也就是等待时间最短的一
个吧？那么应该在添加任务时，将新任务跟据其等待时间插入到适当的位置而不是追加到尾
巴上吧？猜得对不对还得看fDelayQueue.addEntry(alarmHandler) 这个函数是怎么执行的。

- 



void DelayQueue::addEntry(DelayQueueEntry* newEntry) {


- 



// 重新计算各项的等待时间  


- 



  synchronize();


- 



// 取得第一项 


- 



  DelayQueueEntry* cur = head();


- 



// 从头至尾循环中将新项与各项的等待时间进行比较   


- 



while (newEntry->fDeltaTimeRemaining >= cur->fDeltaTimeRemaining) {


- 



// 如果新项等待时间长于当前项的等待时间，则减掉当前项的等待时间。  


- 



    newEntry->fDeltaTimeRemaining -= cur->fDeltaTimeRemaining;


- 



    cur = cur->fNext;


- 



  }


- 



//循环完毕，cur 就是找到的应插它前面的项，那就插它前面吧 


- 



  cur->fDeltaTimeRemaining -= newEntry->fDeltaTimeRemaining;


- 




- 



// Add "newEntry" to the queue, just before "cur":


- 



  newEntry->fNext = cur;


- 



  newEntry->fPrev = cur->fPrev;


- 



  cur->fPrev = newEntry->fPrev->fNext = newEntry;


- 



}



有个问题，while循环中为什么没有判断是否到达最后一下的代码呢？难道肯定能找到大于
新项的等待时间的项吗？是的！第一个加入项的等待时间是无穷大的，而且这一项永远存在
于队列中。









