# 【Darwin学习笔记】之TaskThread - DoubleLi - 博客园






【转载请注明出处】：http://blog.csdn.net/longlong530

学习TaskThread主要有三个类要关注：

TaskTreadPool： 任务线程池

TaskThread：任务线程

Task: 任务



1. TaskThreadPool
Darwin运行着一个或者多个任务（Task）线程，并将他们统一在线程池TaskThreadPool中管理。任务线程从事件线程中接收RTSP和RTP请求，然后把请求传递到恰当的服务器模块进行处理，把数据包发送给客户端。
缺省情况下，核心服务器为每一个处理器创建一个任务线程。（numProcessors = OS::GetNumProcessors();）     
- 



TaskThreadPool::SetNumShortTaskThreads(numShortTaskThreads);        


- 



TaskThreadPool::SetNumBlockingTaskThreads(numBlockingThreads);        


- 



TaskThreadPool::AddThreads(numThreads);		


- 



sServer->InitNumThreads(numThreads);




2. TaskThread

所有的Task对象都将在TaskThread中运行,TaskThread通过运行Task类型对象的Run方法来完成相应Task的处理。TaskThread的个数是可配置的，缺省情况下TaskThread的个数与处理器的个数一致。等待被TaskThread调用并运行的Task放在队列或者堆中。




- 



void TaskThread::Entry()


- 



{


- 



    Task* theTask = NULL;


- 




- 



while (true) 


- 



    {


- 



//不停的从内部的队列中取到Task才返回;,此时的Task既不在堆中，也不在队列中;


- 



        theTask = this->WaitForTask();


- 



/*此处省略，主要是对调用task的Run方法的返回值做不同的分支处理，详细分析在Task章节介绍*/


- 



     }


- 







我们还是仔细分析下WaitForTask方法吧：
- 



Task* TaskThread::WaitForTask()


- 



{


- 



while (true)


- 



    {


- 



        SInt64 theCurrentTime = OS::Milliseconds();


- 



/*如果堆中有任务，且任务已经到执行时间，返回该任务。


- 



		PeekMin函数见OSHeap.h，窃听堆中第一个元素（但不取出）*/


- 



if ((fHeap.PeekMin() != NULL) && (fHeap.PeekMin()->GetValue() <= theCurrentTime))


- 



        {    


- 



if (TASK_DEBUG) qtss_printf("TaskThread::WaitForTask found timer-task=%s thread %p fHeap.CurrentHeapSize(%"_U32BITARG_") taskElem = %p enclose=%p\n",((Task*)fHeap.PeekMin()->GetEnclosingObject())->fTaskName, (void *) this, fHeap.CurrentHeapSize(), (void *) fHeap.PeekMin(), (void *) fHeap.PeekMin()->GetEnclosingObject());


- 



return (Task*)fHeap.ExtractMin()->GetEnclosingObject();//从堆中取出第一个任务返回


- 



        }


- 



//如果堆中有任务，但是尚未到执行时间，计算需要等待的时间


- 



//if there is an element waiting for a timeout, figure out how long we should wait.


- 



        SInt64 theTimeout = 0;


- 



if (fHeap.PeekMin() != NULL)


- 



            theTimeout = fHeap.PeekMin()->GetValue() - theCurrentTime;


- 



        Assert(theTimeout >= 0);


- 




- 



//


- 



// Make sure we can't go to sleep for some ridiculously short


- 



// period of time


- 



// Do not allow a timeout below 10 ms without first verifying reliable udp 1-2mbit live streams. 


- 



// Test with streamingserver.xml pref reliablUDP printfs enabled and look for packet loss and check client for  buffer ahead recovery.


- 



if (theTimeout < 10) 


- 



           theTimeout = 10;


- 




- 



//wait...


- 



//如果任务队列为空，就等待theTimeout时间后从堆中取出任务返回;


- 



//如果任务队列不为空，就不等待，直接取队列中的任务;


- 



        OSQueueElem* theElem = fTaskQueue.DeQueueBlocking(this, (SInt32) theTimeout);


- 



if (theElem != NULL)


- 



        {    


- 



if (TASK_DEBUG) qtss_printf("TaskThread::WaitForTask found signal-task=%s thread %p fTaskQueue.GetLength(%"_U32BITARG_") taskElem = %p enclose=%p\n", ((Task*)theElem->GetEnclosingObject())->fTaskName,  (void *) this, fTaskQueue.GetQueue()->GetLength(), (void *)  theElem,  (void *)theElem->GetEnclosingObject() );


- 



return (Task*)theElem->GetEnclosingObject();


- 



        }


- 




- 



//


- 



// If we are supposed to stop, return NULL, which signals the caller to stop


- 



if (OSThread::GetCurrent()->IsStopRequested())


- 



return NULL;


- 



    }   


- 



}




3. Task
每个Task对象有两个主要的方法：Signal和Run。



3.1 Run()


Run()方法是在Task对象获得处理该事件的时间片后运行的，Darwin中的大部分工作都是在不同Task对象的Run()函数中进行的。
比如常见的Task类型是RTSPSession和RTPSession。
在这里程序的返回值是一个重要的信息:
当返回-1时,表明任务已经完全结束,程序会直接删除该Task指针;
当返回0时，线程接着执行下一个Task，表明任务希望在下次传信时被再次立即执行;
当返回一个正数n时，线程会在 n毫秒后再次调用这个Task的Run函数。将该任务加入到Heap中，并且纪录它希望等待的时间。Entry()函数将通过waitfortask()函数进行检测，如果等待的时间到了，就再次运行该任务
(注意; 在我们继承Task类，而重写Run虚函数时，我们第一件要作的事情是运行Task类的GetEvent（）函数。)

```cpp
virtual SInt64 Run() = 0;
```



3.2 Signal()

当服务器希望发送一个事件给某个Task对象时，就会调用Signal()方法;
将该任务加入到指定任务线程的任务队列中，如果不存在指定的任务线程，就在线程池中随机选择一个任务线程运行该任务

```cpp
void  Signal(EventFlags eventFlags);
```











