# live555源码学习笔记之TaskScheduler - DoubleLi - 博客园






今天抽空研究了下live555的任务实现：

TaskScheduler分为三种任务：socket handler,event handler,delay task。这三种任务的特点是，前两个加入执行队列后会一直存在，而delay task在执行完一次后会立即弃掉。


socket handler保存在队列BasicTaskScheduler0::HandlerSet* fHandlers中;

event handler保存在数组BasicTaskScheduler0::TaskFunc * fTriggeredEventHandlers[MAX_NUM_EVENT_TRIGGERS]中;

delay task保存在队列BasicTaskScheduler0::DelayQueue fDelayQueue中。



![](https://img-blog.csdn.net/20130705161437109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2xvbmc1MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

一.研究delaytask：

可以参考：[http://www.cnblogs.com/nightwatcher/archive/2011/04/10/2011158.html](http://www.cnblogs.com/nightwatcher/archive/2011/04/10/2011158.html)

在学习操作delay task的函数之前，先研究下live555中的这个DelayQueue ：


- 



class DelayQueueEntry {


- 



public:


- 



virtual ~DelayQueueEntry();


- 




- 



intptr_t token() {


- 



return fToken;


- 



  }


- 




- 



protected: // abstract base class


- 



  DelayQueueEntry(DelayInterval delay);


- 




- 



virtual void handleTimeout();  //执行超时任务;


- 




- 



private:


- 



friend class DelayQueue;


- 



  DelayQueueEntry* fNext;  //后一个对象


- 



  DelayQueueEntry* fPrev;  //前一个对象


- 



  DelayInterval fDeltaTimeRemaining;  超时时间（倒计时），该结构体含两个参数，一个是秒，一个是微秒;


- 




- 



intptr_t fToken;               //游标，方便查表


- 



static intptr_t tokenCounter;  //队列计数器;


- 



};


- 




- 




- 



class DelayQueue: public DelayQueueEntry {


- 



public:


- 



  DelayQueue();


- 



virtual ~DelayQueue();


- 




- 



void addEntry(DelayQueueEntry* newEntry); // returns a token for the entry


- 



void updateEntry(DelayQueueEntry* entry, DelayInterval newDelay);   //更新一个任务的超时时间


- 



void updateEntry(intptr_t tokenToFind, DelayInterval newDelay);     //通过游标查找某个任务，再更新其超时时间


- 



void removeEntry(DelayQueueEntry* entry); // but doesn't delete it  //将某个任务从队列从移除，但是不销毁该对象


- 



DelayQueueEntry* removeEntry(intptr_t tokenToFind); // but doesn't delete it  /通过游标，将某个任务从队列从移除，但是不销毁该对象


- 




- 



DelayInterval const& timeToNextAlarm();  //倒计时还剩多久时间


- 



void handleAlarm();        //将超时的任务移除，然后执行


- 




- 



private:


- 



DelayQueueEntry* head() { return fNext; }


- 



DelayQueueEntry* findEntryByToken(intptr_t token);


- 



void synchronize(); //更新超时时间


- 




- 



  EventTime fLastSyncTime;


- 



};


- 




- 




- 




- 



class AlarmHandler: public DelayQueueEntry {


- 



public:


- 



  AlarmHandler(TaskFunc* proc, void* clientData, DelayInterval timeToDelay)


- 



    : DelayQueueEntry(timeToDelay), fProc(proc), fClientData(clientData) {


- 



  }


- 




- 



private: // redefined virtual functions


- 



virtual void handleTimeout() {


- 



    (*fProc)(fClientData);   //通过调用函数指针 +参数　执行任务


- 



    DelayQueueEntry::handleTimeout();


- 



  }


- 




- 



private:


- 



  TaskFunc* fProc;     //delaytask的函数指针


- 



void* fClientData;   //delaytask的函数所需要的参数


- 



};







操作该队列的方法在BasicTaskScheduler0.h、BasicTaskScheduler0.cpp 中声明和实现：


- 



TaskToken BasicTaskScheduler0::scheduleDelayedTask(int64_t microseconds, TaskFunc* proc, void* clientData) {


- 



if (microseconds < 0) microseconds = 0;


- 



DelayInterval timeToDelay((long)(microseconds/1000000), (long)(microseconds%1000000));


- 



  AlarmHandler* alarmHandler = new AlarmHandler(proc, clientData, timeToDelay); //创建一个AlarmHandler对象;


- 



  fDelayQueue.addEntry(alarmHandler);                                           //加入任务队列中;


- 




- 



return (void*)(alarmHandler->token());//返回该任务在队列中的游标


- 



}


- 




- 



void BasicTaskScheduler0::unscheduleDelayedTask(TaskToken& prevTask) {


- 



  DelayQueueEntry* alarmHandler = fDelayQueue.removeEntry((intptr_t)prevTask);//根据任务对象的游标，将任务移除队列


- 



  prevTask = NULL;


- 



delete alarmHandler;//销毁对象


- 



}





二.event handler

 event handler是被存在数组中。数组大小固定等于32（#define MAX_NUM_EVENT_TRIGGERS 32），用EventTriggerId来表示数组中的项，EventTriggerId是一个32位整数，因为数组是32项，所以用EventTriggerId中的第n位置为1，则表明对应数组中的第n项。成员变量fTriggersAwaitingHandling也是EventTriggerId类型，它里面置1的那些位对应了数组中所有需要处理的项。这样做节省了内存和计算，但降低了可读性，呵呵，而且也不够灵活，只能支持32项或64项，其它数量不被支持。


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



    i = (i+1)%MAX_NUM_EVENT_TRIGGERS;   //序号加一


- 



    mask >>= 1;                         //mask右移一位，代表一个新的序号


- 



if (mask == 0) mask = 0x80000000;   //默认为第32位为1


- 




- 



if (fTriggeredEventHandlers[i] == NULL) {//如果数组的该位置没有存入数据，则将函数指针存入fTriggeredEventHandlers，数据位置空，触发事件时传入


- 



// This trigger number is free; use it:


- 



      fTriggeredEventHandlers[i] = eventHandlerProc;


- 



      fTriggeredEventClientDatas[i] = NULL; // sanity


- 




- 



      fLastUsedTriggerMask = mask;      //记录最新的Mask和序号


- 



      fLastUsedTriggerNum = i;


- 




- 



return mask; //分配成功，返回项数


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


- 




- 



void BasicTaskScheduler0::deleteEventTrigger(EventTriggerId eventTriggerId) {


- 



  fTriggersAwaitingHandling &=~ eventTriggerId;


- 




- 



if (eventTriggerId == fLastUsedTriggerMask) { // common-case optimization:如果删除的事件正好是最后一个非空项，则直接将函数指针和参数置空;


- 



    fTriggeredEventHandlers[fLastUsedTriggerNum] = NULL;


- 



    fTriggeredEventClientDatas[fLastUsedTriggerNum] = NULL;


- 



  } else {         


- 



// "eventTriggerId" should have just one bit set.


- 



// However, we do the reasonable thing if the user happened to 'or' together two or more "EventTriggerId"s:


- 



    EventTriggerId mask = 0x80000000;


- 



for (unsigned i = 0; i < MAX_NUM_EVENT_TRIGGERS; ++i) {


- 



if ((eventTriggerId&mask) != 0) {  //通过移位，然后与操作比对，找出要删除的元素


- 



	fTriggeredEventHandlers[i] = NULL;


- 



	fTriggeredEventClientDatas[i] = NULL;


- 



      }


- 



      mask >>= 1;


- 



    }


- 



  }


- 



}


- 




- 



void BasicTaskScheduler0::triggerEvent(EventTriggerId eventTriggerId, void* clientData) {


- 



// First, record the "clientData":


- 



if (eventTriggerId == fLastUsedTriggerMask) { // common-case optimization:


- 



    fTriggeredEventClientDatas[fLastUsedTriggerNum] = clientData;


- 



  } else {


- 



    EventTriggerId mask = 0x80000000;


- 



for (unsigned i = 0; i < MAX_NUM_EVENT_TRIGGERS; ++i) {


- 



if ((eventTriggerId&mask) != 0) {


- 



	fTriggeredEventClientDatas[i] = clientData; //事件触发时，传入函数的参数


- 




- 



	fLastUsedTriggerMask = mask;


- 



	fLastUsedTriggerNum = i;


- 



      }


- 



      mask >>= 1;


- 



    }


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



最后在BasicTaskScheduler::SingleStep中执行该事件函数。



三.socket handler

先看下几个用到的结构体


- 



class HandlerDescriptor {


- 



  HandlerDescriptor(HandlerDescriptor* nextHandler);


- 



virtual ~HandlerDescriptor();


- 




- 



public:


- 



int socketNum;//套接字的序号;


- 



int conditionSet; //socket状态，有SOCKET_READABLE，SOCKET_WRITABLE，SOCKET_EXCEPTION三种;


- 



  TaskScheduler::BackgroundHandlerProc* handlerProc;//事件执行的函数;


- 



void* clientData;//事件执行的函数的参数;


- 




- 



private:


- 



// Descriptors are linked together in a doubly-linked list:


- 



friend class HandlerSet;


- 



friend class HandlerIterator;


- 



  HandlerDescriptor* fNextHandler;//下一个节点;


- 



  HandlerDescriptor* fPrevHandler;//上一个节点;


- 



};


- 




- 



//Handlerset主要实现了一个HandlerDescriptort的双向链表，并实现了对链表的插入，查找，删除,移动的操作;


- 



class HandlerSet {


- 



public:


- 



  HandlerSet();


- 



virtual ~HandlerSet();


- 




- 



void assignHandler(int socketNum, int conditionSet, TaskScheduler::BackgroundHandlerProc* handlerProc, void* clientData);//插入;


- 



void clearHandler(int socketNum);//删除;


- 



void moveHandler(int oldSocketNum, int newSocketNum);//移动;


- 




- 



private:


- 



HandlerDescriptor* lookupHandler(int socketNum);//查找;


- 




- 



private:


- 



friend class HandlerIterator;


- 



  HandlerDescriptor fHandlers;//HandlerDescriptort链表头部;


- 



};


- 




- 



//主要实现在HandlerSet中的迭代容器;


- 



class HandlerIterator {


- 



public:


- 



  HandlerIterator(HandlerSet& handlerSet);


- 



virtual ~HandlerIterator();


- 




- 



HandlerDescriptor* next(); //返回set中的下一个HandlerDescriptor，并保存当前的查找位置;


- 



void reset();


- 




- 



private:


- 



  HandlerSet& fOurSet;


- 



  HandlerDescriptor* fNextPtr;


- 



};





处理socket handler的函数：




- 



void BasicTaskScheduler


- 



  ::setBackgroundHandling(int socketNum, int conditionSet, BackgroundHandlerProc* handlerProc, void* clientData) {


- 



if (socketNum < 0) return;


- 



  FD_CLR((unsigned)socketNum, &fReadSet);  //删除该套接字;


- 



  FD_CLR((unsigned)socketNum, &fWriteSet); //删除该套接字;


- 



  FD_CLR((unsigned)socketNum, &fExceptionSet); //删除该套接字;


- 



if (conditionSet == 0) {                  //将该套接字对应的节点从链表中删除;


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



    fHandlers->assignHandler(socketNum, conditionSet, handlerProc, clientData);//将该节点加入双向链表;


- 



if (socketNum+1 > fMaxNumSockets) {


- 



      fMaxNumSockets = socketNum+1;


- 



    }


- 



if (conditionSet&SOCKET_READABLE) FD_SET((unsigned)socketNum, &fReadSet);//加入可读集合;


- 



if (conditionSet&SOCKET_WRITABLE) FD_SET((unsigned)socketNum, &fWriteSet);//加入可写集合;


- 



if (conditionSet&SOCKET_EXCEPTION) FD_SET((unsigned)socketNum, &fExceptionSet);//加入异常集合;


- 



  }


- 



}


- 




- 



void BasicTaskScheduler::moveSocketHandling(int oldSocketNum, int newSocketNum) {


- 



//先对套接字参数进行验证;


- 



if (oldSocketNum < 0 || newSocketNum < 0) return; // sanity check


- 



//根据老的套接字对应的集合，设置新套接字的集合;


- 



if (FD_ISSET(oldSocketNum, &fReadSet)) {FD_CLR((unsigned)oldSocketNum, &fReadSet); FD_SET((unsigned)newSocketNum, &fReadSet);}


- 



if (FD_ISSET(oldSocketNum, &fWriteSet)) {FD_CLR((unsigned)oldSocketNum, &fWriteSet); FD_SET((unsigned)newSocketNum, &fWriteSet);}


- 



if (FD_ISSET(oldSocketNum, &fExceptionSet)) {FD_CLR((unsigned)oldSocketNum, &fExceptionSet); FD_SET((unsigned)newSocketNum, &fExceptionSet);}


- 



//更新套接字;


- 



  fHandlers->moveHandler(oldSocketNum, newSocketNum);


- 




- 



if (oldSocketNum+1 == fMaxNumSockets) {


- 



    --fMaxNumSockets;


- 



  }


- 



if (newSocketNum+1 > fMaxNumSockets) {


- 



    fMaxNumSockets = newSocketNum+1;


- 



  }


- 



}


- 











