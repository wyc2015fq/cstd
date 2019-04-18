# 基于ACE应用编程框架——线程池 - 深之JohnChen的专栏 - CSDN博客

2010年03月12日 11:47:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1707标签：[框架																[编程																[manager																[thread																[processing																[returning](https://so.csdn.net/so/search/s.do?q=returning&t=blog)](https://so.csdn.net/so/search/s.do?q=processing&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=manager&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)
个人分类：[ACE网络编程](https://blog.csdn.net/byxdaz/article/category/628882)


一、基本的实现模型：
    线程池的实现模型主要有两种：
    1、半同步/半异步模型
    在这种模型中，一个侦听线程负责接受请求，并在某个队列中缓冲它们。另外一组工作者线程负责处理请求。因此接受请求的线程并不是处理请求的线程。
    2、领导者/跟随者模型
    在这种模型中，有一个线程是领导者，其余线程是线程中的跟随者。当请求到达时，领导者首先获取请求，并在跟随者中选取一个作为新的领导者，然后继续处理请求。因此接受请求的线程就是处理请求的线程。

二、半同步/半异步模型
    在这种模型中，线程池划分成三个不同的层次：
    异步层，负责接受异步请求
    排队层，对请求进行缓冲
    同步层，多个阻塞在排队层上的处理线程

    一般的半同步/半异步模型会有一个manager线程用于请求消息转发和调度，和一组worker线程构成线程池由manager来统一调度。

    下面是一个半同步/半异步模型的示例，代码摘自APG范例threadpool.cpp，在ACE的源代码发布包中可以找到。下面的代码增加了相关注释，调整了缩进以便于阅读。范例中主要使用ACE一下几个类：ACE_Task，ACE_Message_Block，ACE_Condition，ACE_Unbounded_Queue。

#include "ace/OS.h"
#include "ace/Task.h"
#include  "ace/Thread.h"
#include "ace/Synch.h"
#pragma comment(lib,"aced.lib")

class Worker;

class IManager
{
public:
 virtual int return_to_work (Worker *worker) = 0;
};

class Worker : public ACE_Task<ACE_MT_SYNCH>
{
public:
 Worker (IManager *manager) : manager_(manager) { }

 //线程启动之后进入本函数
 virtual int svc (void)
 {
  thread_id_ = ACE_Thread::self();

  //工作线程启动之后只有收到MB_HANGUP类型的消息它才会退出
  while (1)
  {
   ACE_Message_Block *mb = 0;

   //如果队列中没有数据，本线程将被阻塞
   if (this->getq(mb) == -1)
    ACE_ERROR_BREAK((LM_ERROR, ACE_TEXT ("%p "), ACE_TEXT ("getq")));

   // 如果是MB_HANGUP消息，就结束线程
   if (mb->msg_type() == ACE_Message_Block::MB_HANGUP)
   {
    ACE_DEBUG ((LM_INFO,
     ACE_TEXT ("(%t) Shutting down ")));
    mb->release();

    break;
   }

   // Process the message.
   process_message (mb);

   // Return to work.
   // 这里会将自己放到线程池中，并通过workers_cond_来通知manager
   this->manager_->return_to_work (this);
  }

  return 0;
 }

 ACE_thread_t thread_id(void)
 {
  return thread_id_;
 }

private:

 //处理消息
 void process_message (ACE_Message_Block *mb)
 {
  ACE_TRACE (ACE_TEXT ("Worker::process_message"));

  int msgId;

  ACE_OS::memcpy (&msgId, mb->rd_ptr(), sizeof(int));

  mb->release();

  ACE_DEBUG ((LM_DEBUG,
   ACE_TEXT ("(%t) Started processing message %d "),
   msgId));

  ACE_OS::sleep(3);

  ACE_DEBUG ((LM_DEBUG,
   ACE_TEXT ("(%t) Finished processing message %d "),
   msgId));
 }

 //指向线程池管理器
 IManager *manager_;

 //保存本线程id号
 ACE_thread_t thread_id_;
};

class Manager : public ACE_Task<ACE_MT_SYNCH>, public IManager
{
public:
 enum {POOL_SIZE = 5, MAX_TIMEOUT = 5};

 Manager ()
  : shutdown_(0), workers_lock_(), workers_cond_(workers_lock_)
 {
  ACE_TRACE (ACE_TEXT ("Manager::Manager"));
 }

 /* 线程处理函数 */
 int svc (void)
 {
  ACE_TRACE (ACE_TEXT ("Manager::svc"));

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) Manager started ")));

  // Create pool.
  create_worker_pool();

  while (true)
  {
   ACE_Message_Block *mb = 0;
   ACE_Time_Value tv ((long)MAX_TIMEOUT);
   tv += ACE_OS::time (0);

   // Get a message request.
   if (this->getq (mb, &tv) < 0)
   {
    shut_down ();
    break;
   }

   // Choose a worker.
   Worker *worker = 0;

   /* 
   这对大括号中的代码从worker线程池中获取一个工作线程，线程池由
   this->workers_lock_互斥体加以保护，如果没有worker可用，manager
   会阻塞在workers_lock_条件变量上，等待某个线程回来工作
   */
   {
    ACE_GUARD_RETURN (ACE_Thread_Mutex,
     worker_mon, this->workers_lock_, -1);

    /* 
    阻塞在workers_lock_.wait()上直到有worker可用，当某个worker回来后
    会把自己放到线程池队列上，同时通过触发workers_cond_来通知manager
    */
    while (this->workers_.is_empty ())
     workers_cond_.wait ();

    /* 将获取的worker从线程池队列中删除 */
    this->workers_.dequeue_head (worker);
   }

   // Ask the worker to do the job.
   // 将请求消息放入到worker的消息队列中
   worker->putq (mb);
  }

  return 0;
 }

 int shut_down (void)
 {
  ACE_TRACE (ACE_TEXT ("ACE_ThreadPool::DestroyPool"));

  ACE_Unbounded_Queue<Worker* >::ITERATOR iter = this->workers_.begin();

  Worker** worker_ptr = NULL;

  do
  {
   iter.next (worker_ptr);

   Worker *worker = (*worker_ptr);

   // Send the hangup message.
   ACE_Message_Block *mb;
   ACE_NEW_RETURN(
    mb,
    ACE_Message_Block(0,
    ACE_Message_Block::MB_HANGUP),
    -1);

   worker->putq(mb);

   // Wait for the exit.
   worker->wait();

   ACE_ASSERT (worker->msg_queue()->is_empty ());

   delete worker;
  }while (iter.advance());

  return 0;
 };

 ACE_thread_t thread_id (Worker *worker);

 /* 提供给worker的接口，用于在worker完成处理后，将自己放入到线程池队列，并通知manager */
 virtual int return_to_work (Worker *worker)
 {
  ACE_GUARD_RETURN (ACE_Thread_Mutex,
   worker_mon, this->workers_lock_, -1);

  ACE_DEBUG ((LM_DEBUG,
   ACE_TEXT ("(%t) Worker %u returning to work. "),
   worker->thr_mgr()->thr_self()));

  // 将worker放入到线程池队列
  this->workers_.enqueue_tail (worker);

  // 触发条件变量，通知manager
  this->workers_cond_.signal ();

  return 0;
 }

private:
 // 创建worker线程池
 int create_worker_pool (void)
 {
  ACE_GUARD_RETURN (ACE_Thread_Mutex,
   worker_mon,
   this->workers_lock_,
   -1);

  for (int i = 0; i < POOL_SIZE; i++)
  {
   Worker *worker;

   // 创建worker
   ACE_NEW_RETURN (worker, Worker (this), -1);

   // 放入线程池队列
   this->workers_.enqueue_tail (worker);

   // 激活线程，调用该函数后，worker线程被创建，由于worker
   // 是ACE_Task的子类，线程激活后，从svc函数开始执行
   worker->activate ();
  }

  return 0;
 }

private:
 int shutdown_;

 /* workers_lock_ 线程池队列的互斥体，在对线程池进行操作时，需要通过互斥锁来保护
 所以在所有的线程池队列队列操作前都有这样的语句:
 ACE_GUARD_RETURN (ACE_Thread_Mutex,
 worker_mon, this->workers_lock_, -1);
 */
 ACE_Thread_Mutex workers_lock_;
 ACE_Condition<ACE_Thread_Mutex> workers_cond_;

 /* 线程池队列 */
 ACE_Unbounded_Queue<Worker* > workers_;
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
 Manager tp;
 tp.activate ();

 // Wait for a moment every time you send a message.
 ACE_Time_Value tv;
 tv.msec (100);

 ACE_Message_Block *mb;
 for (int i = 0; i < 10; i++)
 {
  ACE_NEW_RETURN(mb, ACE_Message_Block(sizeof(int)), -1);

  ACE_OS::memcpy (mb->wr_ptr(), &i, sizeof(int));

  ACE_OS::sleep(tv);

  // Add a new work item.
  // 这里将请求消息首先发到了manager线程，由manager线程负责分发
  tp.putq (mb);
 }

 // 主线程等待子线程结束
 ACE_Thread_Manager::instance()->wait();

 return 0;
}


