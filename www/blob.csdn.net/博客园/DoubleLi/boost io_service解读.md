# boost::io_service解读 - DoubleLi - 博客园






# boost::io_service解读

asio是boost提供的一个c++异步编程模型库，其核心类io_service,在多线程编程里面提供了任务队列和任务分发功能，在socket、io编程里主要作为一个事件驱动器(完成端口、select、poll、epoll等)。

## 队列模型

每个io_service都一个公有任务队列，和多个私有任务队列，公有队列由各个线程共享，私有队列则是每个线程独享一个。

![队列模型](http://img.blog.csdn.net/20161126162556488)

io_service的任务执行流程大致如下：
- 调用run方法，进入主loop；
- 判断公有队列是否为空，不为空则取出任务并执行，当任务数大于1时同时唤醒其他空闲线程；
- 任务执行结束，把各个线程的私有队里面的任务移动到公有任务队列里面；
- 触发reactor，linux下面一般是epoll，当有事件时，把相应的事件的任务放到私有队列里。
- 当队列为空时，把当前线程加到空闲线程队列里面，同时进入wait状态，等待其他线程的唤醒（task_operation）。
- 当用户调用post时，任务是直接投递到公有队列op_queue里面。

## 线程池模型

常用的线程池模型有两种：

一种是多个线程共享一个任务队列，用户把任务投递到该任务队列中，其他线程竞争从该队列中获取任务执行。结合boost::thread，在多个线程里面调用run方法，即可实现该线程池：

```
using namespace boost;
using namespace boost::asio;
io_service ios;
int thread_num = 10;
thread *t[thread_num] = {0};
// 创建线程池
for(int i = 0; i < thread_num; ++i)
{
    t[i] = new thread(bind(&io_service::run, &ios));
}

// 向任务队列中投递任务，该任务将随机由一个调用run方法的线程执行
ios.post(func);

// 等待线程退出
for(int i = 0; i < thread_num; ++i)
{
    t[i]->join();
}
```

很容易看出来，这种线程池的瓶颈在于一个任务队列，多个线程竞争取任务，在大并发的程序里面容易导致性能下降。

另一种是每个线程各自维护一个任务队列，用户可以选择随机或者轮训地投递任务到其中一个任务队列里面，该任务队列中的任务只由其所在的线程才能消费。这种线程池在boost的example里面也有相应的实现(io_service_pool)，基本方法是创建多个io_service对象，每个对应一个thread，代码如下：

```
using namespace boost;
using namespace boost::asio;
int thread_num = 10;
io_service ios[thread_num];
thread *t[thread_num] = {0};
// 创建线程池
for(int i = 0; i < thread_num; ++i)
{
    t[i] = new thread(bind(&io_service::run, &ios[i]));
}

// 轮训投递任务
for(int i = 0; i < thread_num; ++i)
{
    ios[i].post(func);
}   

// 等待线程退出
for(int i = 0; i < thread_num; ++i)
{
    t[i]->join();
}
```

## 源码剖析

以下是基于linux环境作的类图，因为在windows上，有些类有不同的定义。

![uml](http://img.blog.csdn.net/20161126162639332)

io_service定义了主要的接口，在linux下的实现是task_io_service。

task_io_service主要定义了三个东西：
- 一个reactor，reactor就是完成端口、select、poll、epoll之类的事件驱动器；
- 一个公共的任务队列op_queue，用来存放用户post的任务，和reactor返回的任务；
- 线程相关变量。io_service本身不会创建任何线程，但它会保存一些线程调用信息，如线程私有队列等。

此外，task_io_service还维护了一个空闲线程列表，在有多余任务到来时，唤醒其中一个空闲线程。在常见的linux单任务队列的线程池里面，用一个condition变量来唤醒线程，在多核系统里面，一次pthread_cond_signal调用，会唤起处于wait状态的一个或多个线程（参考[https://linux.die.net/man/3/pthread_cond_signal](https://linux.die.net/man/3/pthread_cond_signal)），尽管只有一个任务，如果采用空闲线程的方法，有任务时只唤醒一个空闲线程，可以减少很多不必要唤醒。

thread_info_base类维护了一个简单的内存池，只有一块内存，仅在连续的申请释放内存情况下，可以减少内存开辟的开销。

io_service::work的作用：io_service::run在任务执行完毕时会立刻返回，这个并不是编写常驻服务的程序所想要的，boost给的解决方案是定义一个work变量，乍看很神奇，这个跟io_server似乎没有任何关系的变量竟然能控制run的行为，打开源码一看，work的实现简单的出奇，它仅仅是在构造函数里调用io_service的worked_started()方法，使其待处理任务数（++outstanding_work_）大于0，如此，io_service::run就认为一直有任务待处理而不返回了。

谢谢。









