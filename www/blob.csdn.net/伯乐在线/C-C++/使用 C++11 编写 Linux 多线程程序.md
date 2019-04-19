# 使用 C++11 编写 Linux 多线程程序 - 文章 - 伯乐在线
原文出处： [IBM DeveloperWorks - 朱广旭](http://www.ibm.com/developerworks/cn/linux/1412_zhupx_thread/index.html)
## 前言
在这个多核时代，如何充分利用每个 CPU 内核是一个绕不开的话题，从需要为成千上万的用户同时提供服务的服务端应用程序，到需要同时打开十几个页面，每个页面都有几十上百个链接的 web 浏览器应用程序，从保持着几 t 甚或几 p 的数据的数据库系统，到手机上的一个有良好用户响应能力的 app，为了充分利用每个 CPU 内核，都会想到是否可以使用多线程技术。这里所说的“充分利用”包含了两个层面的意思，一个是使用到所有的内核，再一个是内核不空闲，不让某个内核长时间处于空闲状态。在 C++98 的时代，C++标准并没有包含多线程的支持，人们只能直接调用操作系统提供的 SDK API 来编写多线程程序，不同的操作系统提供的 SDK API 以及线程控制能力不尽相同，到了 C++11，终于在标准之中加入了正式的多线程的支持，从而我们可以使用标准形式的类来创建与执行线程，也使得我们可以使用标准形式的锁、原子操作、线程本地存储 (TLS) 等来进行复杂的各种模式的多线程编程，而且，C++11 还提供了一些高级概念，比如 promise/future，packaged_task，async 等以简化某些模式的多线程编程。
多线程可以让我们的应用程序拥有更加出色的性能，同时，如果没有用好，多线程又是比较容易出错的且难以查找错误所在，甚至可以让人们觉得自己陷进了泥潭，希望本文能够帮助您更好地使用 C++11 来进行 Linux 下的多线程编程。
## 认识多线程
首先我们应该正确地认识线程。维基百科对线程的定义是：线程是一个编排好的指令序列，这个指令序列（线程）可以和其它的指令序列（线程）并行执行，操作系统调度器将线程作为最小的 CPU 调度单元。在进行架构设计时，我们应该多从操作系统线程调度的角度去考虑应用程序的线程安排，而不仅仅是代码。
当只有一个 CPU 内核可供调度时，多个线程的运行示意如下：
##### 图 1、单个 CPU 内核上的多个线程运行示意图
![](http://ww1.sinaimg.cn/mw690/6941baebgw1ena5rlfnmwj20e804gwei.jpg)
我们可以看到，这时的多线程本质上是单个 CPU 的时间分片，一个时间片运行一个线程的代码，它可以支持并发处理，但是不能说是真正的并行计算。
当有多个 CPU 或者多个内核可供调度时，可以做到真正的并行计算，多个线程的运行示意如下：
##### 图 2、双核 CPU 上的多个线程运行示意图
![](http://ww4.sinaimg.cn/mw690/6941baebgw1ena5rl132bj20e804gq36.jpg)
从上述两图，我们可以直接得到使用多线程的一些常见场景：
- 进程中的某个线程执行了一个阻塞操作时，其它线程可以依然运行，比如，等待用户输入或者等待网络数据包的时候处理启动后台线程处理业务，或者在一个游戏引擎中，一个线程等待用户的交互动作输入，另外一个线程在后台合成下一帧要画的图像或者播放背景音乐等。
- 将某个任务分解为小的可以并行进行的子任务，让这些子任务在不同的 CPU 或者内核上同时进行计算，然后汇总结果，比如归并排序，或者分段查找，这样子来提高任务的执行速度。
需要注意一点，因为单个 CPU 内核下多个线程并不是真正的并行，有些问题，比如 CPU 缓存不一致问题，不一定能表现出来，一旦这些代码被放到了多核或者多 CPU 的环境运行，就很可能会出现“在开发测试环境一切没有问题，到了实施现场就莫名其妙”的情况，所以，在进行多线程开发时，开发与测试环境应该是多核或者多 CPU 的，以避免出现这类情况。
## C++11 的线程类 std::thread
C++11 的标准类 std::thread 对线程进行了封装，它的声明放在头文件 thread 中，其中声明了线程类 thread, 线程标识符 id，以及名字空间 this_thread，按照 C++11 规范，这个头文件至少应该兼容如下内容：
##### 清单 1.例子 thread 头文件主要内容


C++
```
namespace std{
 struct thread{
 // native_handle_type 是连接 thread 类和操作系统 SDK API 之间的桥梁。
 typedef implementation-dependent native_handle_type;
 native_handle_type native_handle();
 //
 struct id{
 id() noexcept;
 // 可以由==, < 两个运算衍生出其它大小关系运算。
 bool operator==(thread::id x, thread::id y) noexcept;
 bool operator<(thread::id x, thread::id y) noexcept;
 template<class charT, class traits>
 basic_ostream<charT, traits>&
 operator<<(basic_ostream<charT, traits>&out, thread::id id);
 // 哈希函数
 template <class T> struct hash;
 template <> struct hash<thread::id>;
 };
 id get_id() const noexcept;
 // 构造与析构
 thread() noexcept;
 template<class F, class… Args> explicit thread(F&f, Args&&… args);
 ~thread();
 thread(const thread&) = delete;
 thread(thread&&) noexcept;
 thread& operator=( const thread&) = delete;
 thread& operator=(thread&&) noexcept;
 //
 void swap(thread&) noexcept;
 bool joinable() const noexcept;
 void join();
 void detach();
 // 获取物理线程数目
 static unsigned hardware_concurrency() noexcept;
 }
 namespace this_thead{
 thread::id get_id();
 void yield();
 template<class Clock, class Duration>
 void sleep_until(const chrono::time_point<Clock, Duration>& abs_time);
 template<class Rep, class Period>
 void sleep_for(const chromo::duration<Rep, Period>& rel_time);
 }
}
```
和有些语言中定义的线程不同，C++11 所定义的线程是和操作系的线程是一一对应的，也就是说我们生成的线程都是直接接受操作系统的调度的，通过操作系统的相关命令（比如 ps -M 命令）是可以看到的，一个进程所能创建的线程数目以及一个操作系统所能创建的总的线程数目等都由运行时操作系统限定。
native_handle_type 是连接 thread 类和操作系统 SDK API 之间的桥梁，在 g++(libstdc++) for Linux 里面，native_handle_type 其实就是 pthread 里面的 pthread_t 类型，当 thread 类的功能不能满足我们的要求的时候（比如改变某个线程的优先级），可以通过 thread 类实例的 native_handle() 返回值作为参数来调用相关的 pthread 函数达到目的。thread::id 定义了在运行时操作系统内唯一能够标识该线程的标识符，同时其值还能指示所标识的线程的状态，其默认值 (thread::id()) 表示不存在可控的正在执行的线程（即空线程，比如，调用 thead() 生成的没有指定入口函数的线程类实例），当一个线程类实例的 get_id() 等于默认值的时候，即 get_id() == thread::id()，表示这个线程类实例处于下述状态之一：
- 尚未指定运行的任务
- 线程运行完毕
- 线程已经被转移 (move) 到另外一个线程类实例
- 线程已经被分离 (detached)
空线程 id 字符串表示形式依具体实现而定，有些编译器为 0x0，有些为一句语义解释。
有时候我们需要在线程执行代码里面对当前调用者线程进行操作，针对这种情况，C++11 里面专门定义了一个名字空间 this_thread，其中包括 get_id() 函数可用来获取当前调用者线程的 id，yield() 函数可以用来将调用者线程跳出运行状态，重新交给操作系统进行调度，sleep_until 和 sleep_for 函数则可以让调用者线程休眠若干时间。get_id() 函数实际上是通过调用 pthread_self() 函数获得调用者线程的标识符，而 yield() 函数则是通过调用操作系统 API sched_yield() 进行调度切换。
## 如何创建和结束一个线程
和 pthread_create 不同，使用 thread 类创建线程可以使用一个函数作为入口，也可以是其它的 Callable 对象，而且，可以给入口传入任意个数任意类型的参数：
##### 清单 2.例子 thread_run_func_var_args.cc


C++
```
int funcReturnInt(const char* fmt, ...){
 va_list ap;
 va_start(ap, fmt);
 vprintf( fmt, ap );
 va_end(ap);
 return 0xabcd;
}
void threadRunFunction(void){
 thread* t = new thread(funcReturnInt, "%d%s\n", 100, "\%");
 t->join();
 delete t;
}
我们也可以传入一个 Lambda 表达式作为入口，比如：
```
##### 清单 3.例子 thread_run_lambda.cc


C++
```
void threadRunLambda(void){
 int a = 100,
 b = 200;
 thread* t = new thread( [](int ia, int ib){
 cout << (ia + ib) << endl;
 },
 a,
 b );
 t->join();
 delete t;
}
```
一个类的成员函数也可以作为线程入口：
##### 清单 4.例子 thread_run_member_func.cc


C++
```
struct God{
 void create(const char* anything){
 cout << "create " << anything << endl;
 }
};
void threadRunMemberFunction(void){
 God god;
 thread* t = new thread( &God::create, god, "the world" );
 t->join();
 delete t;
}
```
虽然 thread 类的初始化可以提供这么丰富和方便的形式，其实现的底层依然是创建一个 pthread 线程并运行之，有些实现甚至是直接调用 pthread_create 来创建。
创建一个线程之后，我们还需要考虑一个问题：该如何处理这个线程的结束？一种方式是等待这个线程结束，在一个合适的地方调用 thread 实例的 join() 方法，调用者线程将会一直等待着目标线程的结束，当目标线程结束之后调用者线程继续运行；另一个方式是将这个线程分离，由其自己结束，通过调用 thread 实例的 detach() 方法将目标线程置于分离模式。一个线程的 join() 方法与 detach() 方法只能调用一次，不能在调用了 join() 之后又调用 detach()，也不能在调用 detach() 之后又调用 join()，在调用了 join() 或者 detach() 之后，该线程的 id 即被置为默认值（空线程），表示不能继续再对该线程作修改变化。如果没有调用 join() 或者 detach()，那么，在析构的时候，该线程实例将会调用 std::terminate()，这会导致整个进程退出，所以，如果没有特别需要，一般都建议在生成子线程后调用其 join() 方法等待其退出，这样子最起码知道这些子线程在什么时候已经确保结束。
在 C++11 里面没有提供 kill 掉某个线程的能力，只能被动地等待某个线程的自然结束，如果我们要主动停止某个线程的话，可以通过调用 Linux 操作系统提供的 pthread_kill 函数给目标线程发送信号来实现，示例如下：
##### 清单 5.例子 thread_kill.cc


C++
```
static void on_signal_term(int sig){
 cout << "on SIGTERM:" << this_thread::get_id() << endl;
 pthread_exit(NULL); 
}
void threadPosixKill(void){
 signal(SIGTERM, on_signal_term);
 thread* t = new thread( [](){
 while(true){
 ++counter;
 }
 });
 pthread_t tid = t->native_handle();
 cout << "tid=" << tid << endl;
 // 确保子线程已经在运行。
 this_thread::sleep_for( chrono::seconds(1) );
 pthread_kill(tid, SIGTERM);
 t->join();
 delete t;
 cout << "thread destroyed." << endl;
}
```
上述例子还可以用来给某个线程发送其它信号，具体的 pthread_exit 函数调用的约定依赖于具体的操作系统的实现，所以，这个方法是依赖于具体的操作系统的，而且，因为在 C++11 里面没有这方面的具体约定，用这种方式也是依赖于 C++编译器的具体实现的。
## 线程类 std::thread 的其它方法和特点
thread 类是一个特殊的类，它不能被拷贝，只能被转移或者互换，这是符合线程的语义的，不要忘记这里所说的线程是直接被操作系统调度的。线程的转移使用 move 函数，示例如下：
##### 清单 6.例子 thread_move.cc


C++
```
void threadMove(void){
 int a = 1;
 thread t( [](int* pa){
 for(;;){
 *pa = (*pa * 33) % 0x7fffffff;
 if ( ( (*pa) >> 30) & 1) break;
 }
 }, &a);
 thread t2 = move(t);	// 改为 t2 = t 将不能编译。
 t2.join();
 cout << "a=" << a << endl;
}
```
在这个例子中，如果将 t2.join() 改为 t.join() 将会导致整个进程被结束，因为忘记了调用 t2 也就是被转移的线程的 join() 方法，从而导致整个进程被结束，而 t 则因为已经被转移，其 id 已被置空。
线程实例互换使用 swap 函数，示例如下：
##### 清单 7.例子 thread_swap.cc


C++
```
void threadSwap(void){
 int a = 1;
 thread t( [](int* pa){
 for(;;){
 *pa = (*pa * 33) % 0x7fffffff;
 if ( ( (*pa) >> 30) & 1) break;
 }
 }, &a);
 thread t2;
 cout << "before swap: t=" << t.get_id() 
 << ", t2=" << t2.get_id() << endl;
 swap(t, t2);
 cout << "after swap : t=" << t.get_id() 
 << ", t2=" << t2.get_id() << endl;
 t2.join();
 cout << "a=" << a << endl;
}
```
互换和转移很类似，但是互换仅仅进行实例（以 id 作标识）的互换，而**转移则在进行实例标识的互换之前，还进行了转移目的实例（如下例的****t2****）的清理**，如果 t2 是可聚合的（joinable() 方法返回 true），则调用 std::terminate()，这会导致整个进程退出，比如下面这个例子：
##### 清单 8.例子 thread_move_term.cc


C++
```
void threadMoveTerm(void){
 int a = 1;
 thread t( [](int* pa){
 for(;;){
 *pa = (*pa * 33) % 0x7fffffff;
 if ( ( (*pa) >> 30) & 1) break;
 }
 }, &a);
 thread t2( [](){
 int i = 0;
 for(;;)i++;
 } );
 t2 = move(t);	// 将会导致 std::terminate()
 cout << "should not reach here" << endl;
 t2.join();
}
```
所以，在进行线程实例转移的时候，要注意判断目的实例的 id 是否为空值（即 id()）。
如果我们继承了 thread 类，则还需要禁止拷贝构造函数、拷贝赋值函数以及赋值操作符重载函数等，另外，thread 类的析构函数并不是虚析构函数。示例如下：
##### 清单 9.例子 thread_inherit.cc


C++
```
class MyThread : public thread{
public:
 MyThread() noexcept : thread(){};
 template<typename Callable, typename... Args>
 explicit
 MyThread(Callable&& func, Args&&... args) : 
 thread( std::forward<Callable>(func), 
 std::forward<Args>(args)...){
 }
 ~MyThread() { thread::~thread(); }
 // disable copy constructors
 MyThread( MyThread& ) = delete;
 MyThread( const MyThread& ) = delete;
 MyThread& operator=(const MyThread&) = delete;
};
```
因为 thread 类的析构函数不是虚析构函数，在上例中，需要避免出现下面这种情况：
MyThread* tc = new MyThread(…);
…
thread* tp = tc;
…
delete tp;
这种情况会导致 MyThread 的析构函数没有被调用。
## 线程的调度
我们可以调用 this_thread::yield() 将当前调用者线程切换到重新等待调度，但是不能对非调用者线程进行调度切换，也不能让非调用者线程休眠（这是操作系统调度器干的活）。
##### 清单 10.例子 thread_yield.cc


C++
```
void threadYield(void){
 unsigned int procs = thread::hardware_concurrency(), // 获取物理线程数目
 i = 0;
 thread* ta = new thread( [](){
 struct timeval t1, t2;
 gettimeofday(&t1, NULL);
 for(int i = 0, m = 13; i < COUNT; i++, m *= 17){
 this_thread::yield();
 }
 gettimeofday(&t2, NULL);
 print_time(t1, t2, " with yield");
 } );
 thread** tb = new thread*[ procs ];
 for( i = 0; i < procs; i++){
 tb[i] = new thread( [](){
 struct timeval t1, t2;
 gettimeofday(&t1, NULL);
 for(int i = 0, m = 13; i < COUNT; i++, m *= 17){
 do_nothing();
 }
 gettimeofday(&t2, NULL);
 print_time(t1, t2, "without yield");
 });
 }
 ta->join();
 delete ta;
 for( i = 0; i < procs; i++){
 tb[i]->join();
 delete tb[i];
 };
 delete tb;
}
```
ta 线程因为需要经常切换去重新等待调度，它运行的时间要比 tb 要多，比如在作者的机器上运行得到如下结果：

```
$time ./a.out
without yield elapse 0.050199s
without yield elapse 0.051042s
without yield elapse 0.05139s
without yield elapse 0.048782s
 with yield elapse 1.63366s
real	0m1.643s
user	0m1.175s
sys	0m0.611s
```
ta 线程即使扣除系统调用运行时间 0.611s 之后，它的运行时间也远大于没有进行切换的线程。
C++11 没有提供调整线程的调度策略或者优先级的能力，如果需要，只能通过调用相关的 pthread 函数来进行，需要的时候，可以通过调用 thread 类实例的 native_handle() 方法或者操作系统 API pthread_self() 来获得 pthread 线程 id，作为 pthread 函数的参数。
## 线程间的数据交互和数据争用 (Data Racing)
同一个进程内的多个线程之间多是免不了要有数据互相来往的，队列和共享数据是实现多个线程之间的数据交互的常用方式，封装好的队列使用起来相对来说不容易出错一些，而共享数据则是最基本的也是较容易出错的，因为它会产生数据争用的情况，即有超过一个线程试图同时抢占某个资源，比如对某块内存进行读写等，如下例所示：
##### 清单 11.例子 thread_data_race.cc


C++
```
static void
inc(int *p ){
 for(int i = 0; i < COUNT; i++){
 (*p)++;
 }
}
void threadDataRacing(void){
 int a = 0;
 thread ta( inc, &a);
 thread tb( inc, &a);
 ta.join();
 tb.join();
 cout << "a=" << a << endl;
}
```
这是简化了的极端情况，我们可以一眼看出来这是两个线程在同时对&a 这个内存地址进行写操作，但是在实际工作中，在代码的海洋中发现它并不一定容易。从表面看，两个线程执行完之后，最后的 a 值应该是 COUNT * 2，但是实际上并非如此，因为简单如 (*p)++这样的操作并不是一个原子动作，要解决这个问题，对于简单的基本类型数据如字符、整型、指针等，C++提供了原子模版类 atomic，而对于复杂的对象，则提供了最常用的锁机制，比如互斥类 mutex，门锁 lock_guard，唯一锁 unique_lock，条件变量 condition_variable 等。
现在我们使用原子模版类 atomic 改造上述例子得到预期结果：
##### 清单 12.例子 thread_atomic.cc


C++
```
static void
inc(atomic<int> *p ){
 for(int i = 0; i < COUNT; i++){
 (*p)++;
 }
}
void threadDataRacing(void){
 atomic<int> a(0) ;
 thread ta( inc, &a);
 thread tb( inc, &a);
 ta.join();
 tb.join();
 cout << "a=" << a << endl;
}
```
我们也可以使用 lock_guard，lock_guard 是一个范围锁，本质是 RAII(Resource Acquire Is Initialization)，在构建的时候自动加锁，在析构的时候自动解锁，这保证了每一次加锁都会得到解锁。即使是调用函数发生了异常，在清理栈帧的时候也会调用它的析构函数得到解锁，从而保证每次加锁都会解锁，但是我们不能手工调用加锁方法或者解锁方法来进行更加精细的资源占用管理，使用 lock_guard 示例如下：
##### 清单 13.例子 thread_lock_guard.cc


C++
```
static mutex g_mutex;
static void
inc(int *p ){
 for(int i = 0; i < COUNT; i++){
 lock_guard<mutex> _(g_mutex);
 (*p)++;
 }
}
void threadLockGuard(void){
 int a = 0;
 thread ta( inc, &a);
 thread tb( inc, &a);
 ta.join();
 tb.join();
 cout << "a=" << a << endl;
}
```
如果要支持手工加锁，可以考虑使用 unique_lock 或者直接使用 mutex。unique_lock 也支持 RAII，它也可以一次性将多个锁加锁；如果使用 mutex 则直接调用 mutex 类的 lock, unlock, trylock 等方法进行更加精细的锁管理：
##### 清单 14.例子 thread_mutex.cc


C++
```
static mutex g_mutex;
static void
inc(int *p ){
 thread_local int i; // TLS 变量
 for(; i < COUNT; i++){
 g_mutex.lock();
 (*p)++;
 g_mutex.unlock();
 }
}
void threadMutex(void){
 int a = 0;
 thread ta( inc, &a);
 thread tb( inc, &a);
 ta.join();
 tb.join();
 cout << "a=" << a << endl;
}
```
在上例中，我们还使用了线程本地存储 (TLS) 变量，我们只需要在变量前面声明它是 thread_local 即可。TLS 变量在线程栈内分配，线程栈只有在线程创建之后才生效，在线程退出的时候销毁，需要注意不同系统的线程栈的大小是不同的，如果 TLS 变量占用空间比较大，需要注意这个问题。TLS 变量一般不能跨线程，其初始化在调用线程第一次使用这个变量时进行，默认初始化为 0。
对于线程间的事件通知，C++11 提供了条件变量类 condition_variable，可视为 pthread_cond_t 的封装，使用条件变量可以让一个线程等待其它线程的通知 (wait，wait_for，wait_until)，也可以给其它线程发送通知 (notify_one，notify_all)，条件变量必须和锁配合使用，在等待时因为有解锁和重新加锁，所以，在等待时必须使用可以手工解锁和加锁的锁，比如 unique_lock，而不能使用 lock_guard，示例如下：
##### 清单 15.例子 thread_cond_var.cc


C++
```
#include <thread>
#include <iostream>
#include <condition_variable>
using namespace std;
mutex m;
condition_variable cv;
void threadCondVar(void){
# define THREAD_COUNT 10
 thread** t = new thread*[THREAD_COUNT];
 int i;
 for(i = 0; i < THREAD_COUNT; i++){
 t[i] = new thread( [](int index){
 unique_lock<mutex> lck(m);
 cv.wait_for(lck, chrono::hours(1000));
 cout << index << endl;
 }, i );
 this_thread::sleep_for( chrono::milliseconds(50));
 }
 for(i = 0; i < THREAD_COUNT; i++){
 lock_guard<mutex> _(m);
 cv.notify_one();
 }
 for(i = 0; i < THREAD_COUNT; i++){
 t[i]->join();
 delete t[i];
 }
 delete t;
}
```
从上例的运行结果也可以看到，条件变量是不保证次序的，即首先调用 wait 的不一定首先被唤醒。
## 几个高级概念
C++11 提供了若干多线程编程的高级概念：promise/future, packaged_task, async，来简化多线程编程，尤其是线程之间的数据交互比较简单的情况下，让我们可以将注意力更多地放在业务处理上。
promise/future 可以用来在线程之间进行简单的数据交互，而不需要考虑锁的问题，线程 A 将数据保存在一个 promise 变量中，另外一个线程 B 可以通过这个 promise 变量的 get_future() 获取其值，当线程 A 尚未在 promise 变量中赋值时，线程 B 也可以等待这个 promise 变量的赋值：
##### 清单 16.例子 thread_promise_future.cc


C++
```
promise<string> val;
static void
threadPromiseFuture(){
 thread ta([](){
 future<string> fu = val.get_future();
 cout << "waiting promise->future" << endl;
 cout << fu.get() << endl;
 });
 thread tb([](){
 this_thread::sleep_for( chrono::milliseconds(100) );
 val.set_value("promise is set");
 });
 ta.join();
 tb.join();
}
```
一个 future 变量只能调用一次 get()，如果需要多次调用 get()，可以使用 shared_future，通过 promise/future 还可以在线程之间传递异常。
如果将一个 callable 对象和一个 promise 组合，那就是 packaged_task，它可以进一步简化操作：
##### 清单 17.例子 thread_packaged_task.cc


C++
```
static mutex g_mutex;
static void
threadPackagedTask(){
 auto run = [=](int index){ 
 {
 lock_guard<mutex> _(g_mutex);
 cout << "tasklet " << index << endl;
 }
 this_thread::sleep_for( chrono::seconds(10) );
 return index * 1000;
 };
 packaged_task<int(int)> pt1(run);
 packaged_task<int(int)> pt2(run);
 thread t1([&](){pt1(2);} );
 thread t2([&](){pt2(3);} );
 int f1 = pt1.get_future().get();
 int f2 = pt2.get_future().get();
 cout << "task result=" << f1 << endl;
 cout << "task result=" << f2 << endl;
 t1.join();
 t2.join();
}
```
我们还可以试图将一个 packaged_task 和一个线程组合，那就是 async() 函数。使用 async() 函数启动执行代码，返回一个 future 对象来保存代码返回值，不需要我们显式地创建和销毁线程等，而是由 C++11 库的实现决定何时创建和销毁线程，以及创建几个线程等，示例如下：
##### 清单 18.例子 thread_async.cc


C++
```
static long
do_sum(vector<long> *arr, size_t start, size_t count){
 static mutex _m;
 long sum = 0;
 for(size_t i = 0; i < count; i++){
 sum += (*arr)[start + i];
 }
 {
 lock_guard<mutex> _(_m);
 cout << "thread " << this_thread::get_id() 
 << ", count=" << count
 << ", sum=" << sum << endl;
 }
 return sum;
}
static void
threadAsync(){
# define COUNT 1000000
 vector<long> data(COUNT);
 for(size_t i = 0; i < COUNT; i++){
 data[i] = random() & 0xff;
 }
 //
 vector< future<long> > result;
 size_t ptc = thread::hardware_concurrency() * 2;
 for(size_t batch = 0; batch < ptc; batch++){
 size_t batch_each = COUNT / ptc;
 if (batch == ptc - 1){
 batch_each = COUNT - (COUNT / ptc * batch);
 }
 result.push_back(async(do_sum, &data, batch * batch_each, batch_each));
 }
 long total = 0;
 for(size_t batch = 0; batch < ptc; batch++){
 total += result[batch].get();
 }
 cout << "total=" << total << endl;
}
```
如果是在多核或者多 CPU 的环境上面运行上述例子，仔细观察输出结果，可能会发现有些线程 ID 是重复的，这说明重复使用了线程，也就是说，通过使用 async() 还可达到一些线程池的功能。
## 几个需要注意的地方
thread 同时也是棉线、毛线、丝线等意思，我想大家都能体会面对一团乱麻不知从何处查找头绪的感受，不要忘了，线程不是静态的，它是不断变化的，请想像一下面对一团会动态变化的乱麻的情景。所以，使用多线程技术的首要准则是我们自己要十分清楚我们的线程在哪里？线头（线程入口和出口）在哪里？先安排好线程的运行，注意不同线程的交叉点（访问或者修改同一个资源，包括内存、I/O 设备等），尽量减少线程的交叉点，要知道几条线堆在一起最怕的是互相打结。
当我们的确需要不同线程访问一个共同的资源时，一般都需要进行加锁保护，否则很可能会出现数据不一致的情况，从而出现各种时现时不现的莫名其妙的问题，加锁保护时有几个问题需要特别注意：一是一个线程内连续多次调用非递归锁 (non-recursive lock) 的加锁动作，这很可能会导致异常；二是加锁的粒度；三是出现死锁 (deadlock)，多个线程互相等待对方释放锁导致这些线程全部处于罢工状态。
第一个问题只要根据场景调用合适的锁即可，当我们可能会在某个线程内重复调用某个锁的加锁动作时，我们应该使用递归锁 (recursive lock)，在 C++11 中，可以根据需要来使用 recursive_mutex，或者 recursive_timed_mutex。
第二个问题，即锁的粒度，原则上应该是粒度越小越好，那意味着阻塞的时间越少，效率更高，比如一个数据库，给一个数据行 (data row) 加锁当然比给一个表 (table) 加锁要高效，但是同时复杂度也会越大，越容易出错，比如死锁等。
对于第三个问题我们需要先看下出现死锁的条件：
- 资源互斥，某个资源在某一时刻只能被一个线程持有 (hold)；
- 吃着碗里的还看着锅里的，持有一个以上的互斥资源的线程在等待被其它进程持有的互斥资源；
- 不可抢占，只有在某互斥资源的持有线程释放了该资源之后，其它线程才能去持有该资源；
- 环形等待，有两个或者两个以上的线程各自持有某些互斥资源，并且各自在等待其它线程所持有的互斥资源。
我们只要不让上述四个条件中的任意一个不成立即可。在设计的时候，非常有必要先分析一下会否出现满足四个条件的情况，特别是检查有无试图去同时保持两个或者两个以上的锁，当我们发现试图去同时保持两个或者两个以上的锁的时候，就需要特别警惕了。下面我们来看一个简化了的死锁的例子：
##### 清单 19.例子 thread_deadlock.cc


C++
```
static mutex g_mutex1, g_mutex2;
static void
inc1(int *p ){
 for(int i = 0; i < COUNT; i++){
 g_mutex1.lock();
 (*p)++;
 g_mutex2.lock();
 // do something.
 g_mutex2.unlock();
 g_mutex1.unlock();
 }
}
static void
inc2(int *p ){
 for(int i = 0; i < COUNT; i++){
 g_mutex2.lock();
 g_mutex1.lock();
 (*p)++;
 g_mutex1.unlock();
 // do other thing.
 g_mutex2.unlock();
 }
}
void threadMutex(void){
 int a = 0;
 thread ta( inc1, &a);
 thread tb( inc2, &a);
 ta.join();
 tb.join();
 cout << "a=" << a << endl;
}
```
在这个例子中，g_mutex1 和 g_mutex2 都是互斥的资源，任意时刻都只有一个线程可以持有（加锁成功），而且只有持有线程调用 unlock 释放锁资源的时候其它线程才能去持有，满足条件 1 和 3，线程 ta 持有了 g_mutex1 之后，在释放 g_mutex1 之前试图去持有 g_mutex2，而线程 tb 持有了 g_mutex2 之后，在释放 g_mutex2 之前试图去持有 g_mutex1，满足条件 2 和 4，这种情况之下，当线程 ta 试图去持有 g_mutex2 的时候，如果 tb 正持有 g_mutex2 而试图去持有 g_mutex1 时就发生了死锁。在有些环境下，可能要多次运行这个例子才出现死锁，实际工作中这种偶现特性让查找问题变难。要破除这个死锁，我们只要按如下代码所示破除条件 3 和 4 即可：
##### 清单 20.例子 thread_break_deadlock.cc


C++
```
static mutex g_mutex1, g_mutex2;
static voi
inc1(int *p ){
 for(int i = 0; i < COUNT; i++){
 g_mutex1.lock();
 (*p)++;
 g_mutex1.unlock();
 g_mutex2.lock();
 // do something.
 g_mutex2.unlock();
 }
}
static void
inc2(int *p ){
 for(int i = 0; i < COUNT; i++){
 g_mutex2.lock();
 // do other thing.
 g_mutex2.unlock();
 g_mutex1.lock();
 (*p)++;
 g_mutex1.unlock();
 }
}
void threadMutex(void){
 int a = 0;
 thread ta( inc1, &a);
 thread tb( inc2, &a);
 ta.join();
 tb.join();
 cout << "a=" << a << endl;
}
```
在一些复杂的并行编程场景，如何避免死锁是一个很重要的话题，在实践中，当我们看到有两个锁嵌套加锁的时候就要特别提高警惕，它极有可能满足了条件 2 或者 4。
## 结束语
上述例子在 CentOS 6.5，g++ 4.8.1/g++4.9 以及 clang 3.5 下面编译通过，在编译的时候，请注意下述几点：
- 设置 -std=c++11；
- 链接的时候设置 -pthread；
- 使用 g++编译链接时设置 -Wl,–no-as-needed 传给链接器，有些版本的 g++需要这个设置；
- 设置宏定义 -D_REENTRANT，有些库函数是依赖于这个宏定义来确定是否使用多线程版本的。
具体可以参考本文所附的代码中的 Makefile 文件。
在用 gdb 调试多线程程序的时候，可以输入命令 info threads 查看当前的线程列表，通过命令 thread n 切换到第 n 个线程的上下文，这里的 n 是 info threads 命令输出的线程索引数字，例如，如果要切换到第 2 个线程的上下文，则输入命令 thread 2。
聪明地使用多线程，拥抱多线程吧。
## 参考资料
### 学习
- 关于 thread 的定义可参考 [http://en.wikipedia.org/wiki/Thread](http://en.wikipedia.org/wiki/Thread) 和[http://en.wikipedia.org/wiki/Thread_%28computing%29](http://en.wikipedia.org/wiki/Thread_%28computing%29)。
- 关于 C++11 标准中 thread 以及其它相关类的声明可以参考草案[N3242](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2011/n3242.pdf)。
- 参考 Bjarne Stroustrup 的《The C++ Programming Language》第 4 版是 C++11 的权威著作。
- 访问 developerWorks [Linux 专区](http://www.ibm.com/developerworks/cn/linux)，了解关于信息管理的更多信息，获取技术文档、how-to 文章、培训、下载、产品信息以及其他资源。
