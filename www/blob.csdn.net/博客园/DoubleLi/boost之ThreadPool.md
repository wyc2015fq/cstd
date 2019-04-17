# boost之ThreadPool - DoubleLi - 博客园






threadpool是基于boost库实现的一个线程池子库，但线程池实现起来不是很复杂。我们从threadpool中又能学到什么东西呢？

它是基于boost库实现的，如果大家对boost库有兴趣，看看一个简单的实现还是可以学到点东西的。

threadpool基本功能

1、任务封装，包括普通任务（task_func）和优先级任务（prio_task_func）。

2、调度策略，包括fifo_scheduler（先进先出）、lifo_scheduler（后进先出）、prio_scheduler（优先级）。

3、结束策略，包括wait_for_all_tasks（全部任务等待）、wait_for_active_tasks（激活任务等待）、immediately（立即结束）。

4、动态修改线程池个数功能。

5、基于future封装的异步返回值获取功能。



 在sorceforge上有一个用boost编写的线程池。该线程池和boost结合的比较好，并且提供了多种任务执行策略，使用也非常简单。 下载地址： [http://threadpool.sourceforge.net/](http://threadpool.sourceforge.net/) 这个线程池不需要编译，只要在项目中包含其头文件就可以了。

一、源代码分析

quickstart分析（/threadpool/libs/threadpool/quickstart）

这个例子的代码很简单，但已经全部展示了线程池的核心内容，包括建立、调度、同步等操作。



view plaincopy to clipboardprint?

// Create fifo thread pool container with two threads.  

pool tp(2);  



// Add some tasks to the pool.  

tp.schedule(&first_task);  

tp.schedule(&second_task);     



// Wait until all tasks are finished.  

tp.wait(); 

// Create fifo thread pool container with two threads.

pool tp(2);



// Add some tasks to the pool.

tp.schedule(&first_task);

tp.schedule(&second_task);  



// Wait until all tasks are finished.

tp.wait();



pool的定义具体见pool.hpp，但使用了pimpl模式，核心代码见pool_core.hpp文件。

下面是pool的定义

typedef thread_pool<task_func, fifo_scheduler, static_size, resize_controller, wait_for_all_tasks> fifo_pool;

typedef fifo_pool pool;

从上面可以知道，pool实际就是fifo_pool，从模板参数可以看到，使用了fifo_scheduler和wait_for_all_tasks。



对于线程池有点理解的都知道，一般都是那几样东西，线程的封装，条件变量，队列数据结构。

所以简单的能做的很简单，复杂的的就看你的策略需求了。

对基于boost库的threadpool子库来说，上面的三样东西都是现成的，线程封装和条件变量直接使用thread子库就行，队列使用stl的标准容器。



task_adaptors.hpp

对线程任务的封装，所谓task，我们可以理解成需要运行的函数。

threadpool最大限度的使用了function和bind功能来封装函数，这点和thread子库类似。

文件中涉及的内容主要有三个：task_func、prio_task_func和looped_task_func。



对普通task的封装

typedef function0<void> task_func;

如果对bind和function熟悉的应该很好理解。



对优先级任务的封装

class prio_task_func

这个类很简单，重载了两个方法，

operator()是仿函数的用法，

operator<是用于优先级比较使用的，用于stl容器的元素比较。





size_policies.hpp

对size的封装，包括empty_controller、resize_controller和static_size。





shutdown_policies.hpp

对线程池结束的策略封装，包括wait_for_all_tasks、wait_for_active_tasks和immediately。

这几个类很简单，具体操作封装在pool中。

线程池运行过程中，包括队列中等待的task，线程正在运行的task。

所以结束的时候，对这些task的策略操作是有选择的。





scheduling_policies.hpp

对任务调度测试的封装，包括fifo_scheduler、lifo_scheduler和prio_scheduler。

实际上，这三个类的相似程度很高，大家可能更喜欢用继承和虚函数实现。

前面说到保存task的队列数据结构，在这里就看的很清楚了。

fifo和lifo使用的是std::deque，prio使用的是std::priority_queue，其他部分代码没什么好说的了。





pool_adaptors.hpp

对全局schedule函数的几种封装。





future.hpp

好像thread子库也有future，但不清楚是否是一样的内容。

threadpool的future是为了封装异步函数调用返回值实现的。

简单点理解，就是schedule任务的时候，把一个指针在两者间绑定起来，后面就可以通过future来获取返回值了。

当然，获取返回值的过程应该是阻塞的，任务未完成时只能wait。





locking_ptr.hpp

LockingPtr的简单封装，具体可google《volatile - Multithreaded Programmer's Best Friend》。

threadpool大量使用了volatile关键字，所以需要LockingPtr保护。





scope_guard.hpp

对函数对象的封装，利用C++析构函数时调用一个在构造函数时绑定的函数对象。





worker_thread.hpp

对工作线程的封装，这个封装不是指底层线程api封装，因为这部分是由boost的thread子库提供的。

封装针对的是循环执行task的逻辑函数（线程跑起来就loop run某个函数，从队列中获取task执行，空闲时等待。）

我们重点看的是run和create_and_attach。

这两个函数连起来看，就很清楚了，create_and_attach通过bind方式生成一个thread执行run方法。

run方法中的这条语句就是一个简单的loop操作，

while(m_pool->execute_task()) {}

所以，当execute_task返回值为false时，run函数就结束了，bind该函数的thread也就结束了。





ok，来到这里，有必要简单的把整个调用过程说明一下。

// Create fifo thread pool container with two threads.

pool tp(2);



该操作会调用pool的构造函数



view plaincopy to clipboardprint?

thread_pool(size_t initial_threads = 0)  

: m_core(new pool_core_type)  

, m_shutdown_controller(static_cast<void*>(0), bind(&pool_core_type::shutdown, m_core))  

{  

  size_policy_type::init(*m_core, initial_threads);  

} 

thread_pool(size_t initial_threads = 0)

: m_core(new pool_core_type)

, m_shutdown_controller(static_cast<void*>(0), bind(&pool_core_type::shutdown, m_core))

{

  size_policy_type::init(*m_core, initial_threads);

}



由于pimpl模式，所以所有代码都封装在m_core内实现的。

pool默认的线程个数为0，通过size_policy_type::init来初始化。

而size_policy_type是一个模板参数，pool对应的是fifo，所以也就是static_size类型了。



//static_size类的init函数



view plaincopy to clipboardprint?

static void init(Pool& pool, size_t const worker_count)  

{  

  pool.resize(worker_count);  

} 

static void init(Pool& pool, size_t const worker_count)

{

  pool.resize(worker_count);

}



//pool_core的resize函数

这个函数有点长，主要是做动态配置线程个数的逻辑操作，create_and_attach也是在这里调用的。



view plaincopy to clipboardprint?

//worker_thread的create_and_attach函数  

static void create_and_attach(shared_ptr<pool_type> const & pool)  

{  

 shared_ptr<worker_thread> worker(new worker_thread(pool));  

 if(worker)  

 {  

   //run是线程的loop函数  

   worker->m_thread.reset(new boost::thread(bind(&worker_thread::run, worker)));  

 }  

} 

//worker_thread的create_and_attach函数

static void create_and_attach(shared_ptr<pool_type> const & pool)

{

 shared_ptr<worker_thread> worker(new worker_thread(pool));

 if(worker)

 {

   //run是线程的loop函数

   worker->m_thread.reset(new boost::thread(bind(&worker_thread::run, worker)));

 }

}



view plaincopy to clipboardprint?

//worker_thread的run函数  

void run()  

{   

  scope_guard notify_exception(bind(&worker_thread::died_unexpectedly, this));  

  while(m_pool->execute_task()) {} //loop直到返回值为false  

  notify_exception.disable();  

  m_pool->worker_destructed(this->shared_from_this());  

} 

//worker_thread的run函数

void run()

{

  scope_guard notify_exception(bind(&worker_thread::died_unexpectedly, this));

  while(m_pool->execute_task()) {} //loop直到返回值为false

  notify_exception.disable();

  m_pool->worker_destructed(this->shared_from_this());

}





//pool_core的execute_task函数

这个函数有点长，简单点说，就是从队列中获取task然后执行，如果队列为空，则线程需要wait操作。

由于threadpool支持动态resize线程个数，从该函数我们也是可以看出来是如何做到的。



view plaincopy to clipboardprint?

// decrease number of threads if necessary  

if(m_worker_count > m_target_worker_count)  

{   

  return false; // terminate worker  

} 

// decrease number of threads if necessary

if(m_worker_count > m_target_worker_count)

{

  return false; // terminate worker

}





pool内部使用了多个整数来记录现在个数，譬如m_worker_count和m_target_worker_count。

m_worker_count是当前激活运行中的线程个数。

m_target_worker_count是最新动态配置的线程个数。

当个数不匹配时，通过返回false方式结束线程。





// Add some tasks to the pool.

tp.schedule(&first_task);





view plaincopy to clipboardprint?

//thread_pool的schedule函数  

bool schedule(task_type const & task)  

{   

  return m_core->schedule(task);  

}  



//pool_core的schedule函数（和execute_task函数强相关）  

bool schedule(task_type const & task) volatile 

{   

  locking_ptr<pool_type, recursive_mutex> lockedThis(*this, m_monitor);   



  if(lockedThis->m_scheduler.push(task))  

  {  

 //task成功入队列后，notify_one一个线程。  

 lockedThis->m_task_or_terminate_workers_event.notify_one();  

 return true;  

  }  

  else 

  {  

 return false;  

  }  

} 

//thread_pool的schedule函数

bool schedule(task_type const & task)

{

  return m_core->schedule(task);

}



//pool_core的schedule函数（和execute_task函数强相关）

bool schedule(task_type const & task) volatile

{

  locking_ptr<pool_type, recursive_mutex> lockedThis(*this, m_monitor);



  if(lockedThis->m_scheduler.push(task))

  {

 //task成功入队列后，notify_one一个线程。

 lockedThis->m_task_or_terminate_workers_event.notify_one();

 return true;

  }

  else

  {

 return false;

  }

}









// Wait until all tasks are finished.

tp.wait();





//pool_core的wait函数

void wait(size_t const task_threshold = 0) const volatile

bool wait(xtime const & timestamp, size_t const task_threshold = 0) const volatile

wait函数是一个阻塞操作，内部逻辑实现使用了一个条件变量，提供超时等待方式。





二、boost线程池使用实例

线程池可以减少创建和切换线程的额外开销,利用已经存在的线程多次循环执行多个任务从而提高系统的处理能力，有关线程池的概念可google搜索，下面将其使用实例：

#include <iostream>
#include <sstream>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

#include <boost/threadpool.hpp>

using namespace std;
using namespace boost::threadpool;


//
// Helpers
boost::mutex m_io_monitor;

void print(string text)
{
 boost::mutex::scoped_lock lock(m_io_monitor);//每个线程使用全局互斥来保证每次只有一个线程执行
 cout << text;
}

template<typename T>
string to_string(T const & value)
{
 ostringstream ost;
 ost << value;
 ost.flush();
 return ost.str();
}



//
// An example task functions
void task_1()
{
 print("  task_1()/n");
}

void task_2()
{
 print("  task_2()/n");
}

void task_3()
{
 print("  task_3()/n");
}

int task_4()
{
 print("  task_4()/n");
 return 4;
}

void task_with_parameter(int value)
{
 print("  task_with_parameter(" + to_string(value) + ")/n");
}

int loops = 0;
bool looped_task()
{
 print("  looped_task()/n");
 return ++loops < 5;
}


int task_int()
{
 print("  task_int()/n");
 return 23;
}


void fifo_pool_test()
{
 pool tp;

 tp.schedule(&task_1);
 tp.schedule(boost::bind(task_with_parameter, 4));

 if(!tp.empty())
 {
  tp.clear();  // remove all tasks -> no output in this test
 }

 size_t active_threads   = tp.active();
 size_t pending_threads  = tp.pending();
 size_t total_threads    = tp.size();

 size_t dummy = active_threads + pending_threads + total_threads;
 dummy++;

 tp.size_controller().resize(5);
 tp.wait();
}

void lifo_pool_test()
{
 lifo_pool tp;
 tp.size_controller().resize(0);
 schedule(tp, &task_1);
 tp.size_controller().resize(10);
 tp.wait();
}

void prio_pool_test()
{
 prio_pool tp(2);
 schedule(tp, prio_task_func(1, &task_1));
 schedule(tp, prio_task_func(10,&task_2));
}


void future_test()
{
 fifo_pool tp(5);
 future<int> fut = schedule(tp, &task_4);
 int res = fut();
}


int main (int , char * const [])
{
 fifo_pool_test();
 lifo_pool_test();
 prio_pool_test();
 future_test();
 return 0;
}

### 任务返回值的获取：

一般异步调用中，返回值的获取有同步获取和异步获取两种形式。

同步获取返回值：

int task_int_23()
{
    cout<<"task_int_23()/n";
    return 23;
}

future<int> res = schedule(tp, &task_int_23);
res.wait();

cout<<"get res value:"<<res.get()<<endl;

异步获取返回值：

不知道是设计者就不打算使用异步回调获取返回值还是我看的不够仔细，异步获取返回值的方式还真没有找着，只好自己简单的写了一个回调的仿函数来实现异步返回值的获取。

//R为任务函数的返回值类型
template<class R>
class callback_task
{
    typedef boost::function<void (R)> callback;
    typedef boost::function<R ()> function;

private:
    callback c_;
    function f_;

public:
    //F: 任务执行函数 C：结果回调函数
    template<class F,class C>
    callback_task(F f,C c)
    {
        f_ = f;
        c_ = c;
    }

    void operator()()
    {
        c_(f_());
    }
};

通过这个对象可以很容易的实现异步结果的回调。

//task_int_23的结果回调函数
void callback(int k)
{
    cout<<"get callback value:"<<k<<endl;
}

//通过回调的形式获取任务的返回值
tp.schedule(callback_task<int>(&task_int_23,&callback));



参考资料：

boost官方网站: [http://www.boost.org/](http://www.boost.org/)









