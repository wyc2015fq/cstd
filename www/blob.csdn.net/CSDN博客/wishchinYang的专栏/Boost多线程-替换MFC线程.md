# Boost多线程-替换MFC线程 - wishchinYang的专栏 - CSDN博客
2014年05月18日 12:16:24[wishchin](https://me.csdn.net/wishchin)阅读数：1266
       Mfc的多线程看起来简单，可以把线程直接压入向量，由系统类似进行调配，其实在内存的处理问题上留下了漏洞。在新线程里面载入大量流，会导致内存泄露。
方便之处：直接使用结构体传入函数参数，供函数使用。
使用boost多线程，boost库给出了一个相对理想的多线程使用组合库。
      参考链接：[http://blog.csdn.net/iamnieo/article/details/2908621](http://blog.csdn.net/iamnieo/article/details/2908621)
**一：**使用参数的boost进程：
**（1）：**建立参数类
```cpp
class  BoostThreadFunc       {
public:
    pcl::PointCloud<pcl::PointXYZRGB>  ModelCloud;
    MyRect                                                                    BBX;
    CAviTestDlg*                                                           dlg; //使用主框的变量
public:
    BoostThreadFunc();//构造函数....
    BoostThreadFunc( pcl::PointCloud<pcl::PointXYZRGB>  &ModelCloud  ,   MyRect     &BBX ,  CAviTestDlg*     dlg ) {
        this->BBX              =       BBX;
        this->ModelCloud       =ModelCloud;
        this->dlg              =       dlg;    }//重载构造函数....
    void run(
        pcl::PointCloud<pcl::PointXYZRGB>  &ModelCloud,
        MyRect                                    &BBX,
        CAviTestDlg*                              &dlg   );//主要运行函数....
    //创建函数对象....
    void operator()() {   this->run(  this->ModelCloud,  this->RatioRange,  this->BBX, this->dlg);  }
};
void     BoostThreadFunc ::run(
    pcl::PointCloud<pcl::PointXYZRGB>  &ModelCloud,
    MyRect                                    &BBX,
    CAviTestDlg*                              &dlg   )
    {
    .............................................................
    }//主要运行函数....
```
**（2）：创建一个线程：创建线程**
```cpp
boost::thread myThread(threadFun);
```
需要注意的是：参数可以是**函数对象**或者**函数指针**。并且这个**函数无参数，并返回void类型**。
所以，取巧的方式是把参数放入 构造函数里面，使用函数对象调用运行函数。
使用方式：
```cpp
BoostThreadFunc      MyThreadFunc( *ModelCloud,  BBX,   this);//在主框里调用boost线程.
boost::thread  MyThread( MyThreadFunc);
MyThread.join();
```
注意：使用的过程中脑残了一番：boost::thread  MyThread(& MyThreadFunc); 造成thread.hpp的f()函数无法展开！什么原因？？？
是否？boost::thread  MyThread( MyThreadFunc);载入的是无参数函数对象/函数指针，而boost::thread  MyThread(& MyThreadFunc);中若使用&必须绑定全局静态函数.
**二：**参考链接：[http://www.cnblogs.com/younes/archive/2010/06/06/1752745.html](http://www.cnblogs.com/younes/archive/2010/06/06/1752745.html)
**使用线程组**
    如果你需要创建几个线程，考虑使用一个线程组对象thread_group来组织它们。一个thread_group对象可以使用多种方法管理线程。首先，可以使用一个指向**动态创建的线程对象**的指针作为参数来调用add_thread方法，将这个线程加入线程组。也可以直接使用线程组类的create_thread方法，可不先创建线程而直接把线程加入到线程组中。
    当线程组对象的析构函数被调用时，它将删除（delete）所有这些通过add_thread方法加入的线程指针。所以，只能将**堆上的线程对象指针**通过add_thread方法加入线程组。remove_thread方法从线程组删除某个线程的指针，但是我们仍需负责把线程本身内存释放掉。
    线程组对象的成员方法join_all方法等待线程组中所有线程结束，才返回。
```cpp
boost::thread_group                                 grp;
boost::thread          *p =newboost::thread(threadFun);
grp.add_thread(p);
//do something...
grp.remove_thread(p);
 
grp.create_thread(threadFun);
grp.create_thread(threadFun);  //Now there are two threads in grp
 
grp.join_all();                //Wait for all threads to finish
```
**三、使资源是线程安全的**
保证同一时刻多个线程不会同时修改同一个共享资源，那么这个程序是线程安全的，或者是串行化访问资源的。可以使用mutex类来控制线程的并发问题。
```
```cpp
#i**nclude <iostream>**
```
**```cpp#include <boost/thread/thread.hpp>```**
**```cpp#include <string>```**
**```cpp// A simple queue class; don't do this, use std::queue```**
**```cpptemplate``````cpp<``````cpptypename``````cppT>```**
**```cppclass``````cppQueue {```**
**```cpppublic``````cpp:```**
**```cpp``````cppQueue( ) {}```**
**```cpp``````cpp~Queue( ) {}```**
**```cpp``````cppvoid``````cppenqueue(``````cppconst``````cppT& x) {```**
**```cpp``````cpp```**
**```cpp``````cppboost::mutex::scoped_lock lock(mutex_);**```cpp``````cpp// Lock the mutex for this queue```**```**
**```cpp``````cpplist_.push_back(x);``````cpp``````cpp// A scoped_lock is automatically destroyed (and thus unlocked)``````cpp``````cpp// when it goes out of scope```**
**```cpp``````cpp}```**
**```cpp``````cppT dequeue( ) {```**
**```cpp``````cppboost::mutex::scoped_lock lock(mutex_);```**
**```cpp``````cppif``````cpp(list_.empty( ))```**
**```cpp``````cppthrow``````cpp"empty!"``````cpp;``````cpp// This leaves the current scope, so the```**
**```cpp``````cppT tmp = list_.front( );``````cpp// lock is released```**
**```cpp``````cpplist_.pop_front( );```**
**```cpp``````cppreturn``````cpp(tmp);```**
**```cpp``````cpp}``````cpp// Again: when scope ends, mutex_ is unlocked```**
**```cppprivate``````cpp:```**
**```cpp``````cppstd::list<T> list_;```**
**```cpp``````cppboost::mutex mutex_;```**
**```cpp};```**
**```cppQueue<std::string> queueOfStrings;```**
**```cppvoid``````cppsendSomething( ) {```**
**```cpp``````cppstd::string s;```**
**```cpp``````cppfor``````cpp(``````cppint``````cppi = 0; i < 10; ++i) {```**
**```cpp``````cppqueueOfStrings.enqueue(``````cpp"Cyrus"``````cpp);```**
**```cpp``````cpp}```**
**```cpp}```**
**```cppvoid``````cpprecvSomething( ) {```**
**```cpp``````cppstd::string s;```**
**```cpp``````cppfor``````cpp(``````cppint``````cppi = 0; i < 10; ++i) {```**
**```cpp``````cpptry``````cpp{s = queueOfStrings.dequeue( );}```**
**```cpp``````cppcatch``````cpp(...) {}```**
**```cpp``````cpp}```**
**```cpp}```**
**```cppint``````cppmain( ) {```**
**```cpp``````cppboost::``````cppthread``````cppthr1(sendSomething);```**
**```cpp``````cppboost::``````cppthread``````cppthr2(recvSomething);```**
**```cpp``````cppthr1.join( );```**
**```cpp``````cppthr2.join( );```**
```cpp
}
```
```
    mutex对象本身并不知道它代表什么，它仅仅是被多个消费者线程使用的资源访问的锁定解锁标志。在某个时刻，只有一个线程可以锁定这个mutex对象，这就阻止了同一时刻有多个线程并发访问共享资源。一个mutex就是一个简单的信号机制。
    给mutex加解锁有多种策略，最简单的是使用scoped_lock类，它使用一个mutex参数来构造，并一直锁定这个mutex直到对象被销毁。如果这个正在被构造的mutex已经被别的线程锁定的话，当前线程就会进入wait状态，直到这个锁被解开。
**三、读写锁**
mutex有一个美中不足，它不区分读和写。线程如果只是进行读操作，mutex强制线程串行化访问资源，效率低。而且这种操作不需要排他性访问。基于这个原因，Boost线程库提供了read_write_mutex。
```
**```cpp#include <iostream>```**
**```cpp#include <boost/thread/thread.hpp>```**
**```cpp#include <boost/thread/read_write_mutex.hpp>```**
**```cpp#include <string>```**
**```cpptemplate``````cpp<``````cpptypename``````cppT>```**
**```cppclass``````cppQueue {```**
**```cpppublic``````cpp:```**
**```cpp``````cppQueue( ) :**   ```cpp**```cpprwMutex_(boost::read_write_scheduling_policy::writer_priority){}```**```**```cpp``````**
**```cpp``````cpp**```cpp**```cpp``````cpp// Use a read/write mutex and give writers priorit```**```**```**
**```cpp``````cpp~Queue( ) {}**```cpp```**```**
**```cpp``````cppvoid``````cppenqueue(``````cppconst``````cppT& x) {```**
**```cpp**```cpp``````cpp// Use a r/w lock since enqueue updates the state```**``````cpp```**
**```cpp``````cppboost::read_write_mutex::scoped_write_lock writeLock(rwMutex_);```**
**```cpp``````cpplist_.push_back(x);```**
**```cpp``````cpp}```**
**```cpp``````cppT dequeue( ) {```**
**```cpp``````cpp// Again, use a write lock```**
**```cpp``````cppboost::read_write_mutex::scoped_write_lock writeLock(rwMutex_);```**
**```cpp``````cppif``````cpp(list_.empty( ))```**
**```cpp``````cppthrow``````cpp"empty!"``````cpp;```**
**```cpp``````cppT tmp = list_.front( );```**
**```cpp``````cpplist_.pop_front( );```**
**```cpp``````cppreturn``````cpp(tmp);```**
**```cpp``````cpp}```**
**```cpp``````cppT getFront( ) {```**
**```cpp``````cpp// This is a read-only operation, so you only need a read lock```**
**```cpp``````cppboost::read_write_mutex::scoped_read_lock readLock(rwMutex_);```**
**```cpp``````cppif``````cpp(list_.empty( ))```**
**```cpp``````cppthrow``````cpp"empty!"``````cpp;```**
**```cpp``````cppreturn``````cpp(list_.front( ));```**
**```cpp``````cpp}```**
**```cppprivate``````cpp:```**
**```cpp``````cppstd::list<T> list_;```**
**```cpp``````cppboost::read_write_mutex rwMutex_;```**
**```cpp};```**
**```cppQueue<std::string> queueOfStrings;```**
**```cppvoid``````cppsendSomething( ) {```**
**```cpp``````cppstd::string s;```**
**```cpp``````cppfor``````cpp(``````cppint``````cppi = 0; i < 10; ++i) {```**
**```cpp``````cppqueueOfStrings.enqueue(``````cpp"Cyrus"``````cpp);```**
**```cpp``````cpp}```**
**```cpp}```**
**```cppvoid``````cppcheckTheFront( ) {```**
**```cpp``````cppstd::string s;```**
**```cpp``````cppfor``````cpp(``````cppint``````cppi = 0; i < 10; ++i) {```**
**```cpp``````cpptry``````cpp{s = queueOfStrings.getFront( );}```**
**```cpp``````cppcatch``````cpp(...) {}```**
**```cpp``````cpp}```**
**```cpp}```**
**```cppint``````cppmain( ) {```**
**```cpp``````cppboost::``````cppthread``````cppthr1(sendSomething);```**
**```cpp``````cppboost::thread_group grp;```**
**```cpp``````cppgrp.create_thread(checkTheFront);```**
**```cpp``````cppgrp.create_thread(checkTheFront);```**
**```cpp``````cppgrp.create_thread(checkTheFront);```**
**```cpp``````cppgrp.create_thread(checkTheFront);```**
**```cpp``````cppthr1.join( );```**
**```cpp``````cppgrp.join_all( );```**
**```cpp}```**
```
     注意Queue的构造函数中队读写锁rwMutex的初始化。同一时刻，可能有多个读写线程要锁定一个read_write_mutex，而这些锁的调度策略依赖于构造这个mutex时选定的调度策略。Boost库中提供了四种调度策略：
> 
1）reader_priority：等待读锁的线程优先于等待写锁的线程
2）writer_priority：等待写锁的线程优先于等待读锁的线程
3）alternating_single_read：在读锁和写锁之间交替
4）alternating_many_reads：在读锁和写锁之间交替，这个策略将在两个写锁之间使得所有的在这个queue上挂起的读锁都被允许。
     选择使用哪种策略要慎重，因为使用前两种的话可能会导致某些锁始终不能成功，出现**饿死**的现象。
**死锁、饿死和竞态条件**
    1）死锁，是涉及至少2个线程和2个资源的情况。线程A和B，资源X和Y。A锁定了X，而B锁定了Y。此时A和B有彼此想要对方的资源，死锁就出现了。
> 
死锁的预防有两种方法。一种是，通过小心的按照一定的顺序对不同的mutex来加锁。另一种是，使用Boost提供的try_mutex互斥量和scoped_try_lock。或者使用**时间锁**。scoped_try_lock对try_mutex加锁时，可能成功，也可能失败，但不会阻塞。时间锁则有一个超时时间。
```
**```cppbool``````cppdequeue(T& x)```**
**```cpp{```**
**```cpp``````cppboost::try_mutex::scope_try_lock lock(tryMutex_);```**
**```cpp``````cppif``````cpp(!lock.locked())```**
**```cpp``````cppreturn``````cppfalse``````cpp;```**
**```cpp``````cppelse``````cpp{```**
**```cpp``````cppif``````cpp(list_.empty())```**
**```cpp``````cppthrow``````cpp"empty!"``````cpp;```**
**```cpp``````cppx = list_.front();```**
**```cpp``````cpplist_.pop_front();```**
**```cpp``````cppreturn``````cpptrue``````cpp;```**
**```cpp``````cpp}```**
**```cpp}```**
**```cppprivate``````cpp:```**
**```cpp``````cppboost::try_mutex tryMutex_;```**
```
    2）饿死，如果你正在使用write_priority策略，并且你有很多创建写锁的线程，那么读锁的线程就可能饿死。
    3）竞态条件，
> 
```
**```cppif``````cpp(q.getFront() ==``````cpp"Cyrus"``````cpp){```**
**```cpp``````cppstr = q.dequeue();```**
**```cpp``````cpp//....```**
**```cpp}```**
```
       这个代码在单线程环境中工作很好，因为q在第一行和第二行代码之间不会被修改。多线程环境中则会出现问题。此为竞态条件。解决的方法是为Queue添加一个成员函数dequeueIfEquals，在函数执行过程中始终锁定互斥量。
**四、从一个线程中给另一个线程发送通知**
    当需要线程等待某个事物时，可以创建一个condition对象，然后通过这个对象来通知那些等待的线程。
```
**```cpp#include <iostream>```**
**```cpp#include <boost/thread/thread.hpp>```**
**```cpp#include <boost/thread/condition.hpp>```**
**```cpp#include <boost/thread/mutex.hpp>```**
**```cpp#include <list>```**
**```cpp#include <string>```**
**```cppclass``````cppRequest {``````cpp/*...*/``````cpp};```**
**```cpp// A simple job queue class; don't do this, use std::queue```**
**```cpptemplate``````cpp<``````cpptypename``````cppT>```**
**```cppclass``````cppJobQueue {```**
**```cpppublic``````cpp:```**
**```cpp``````cppJobQueue( ) {}```**
**```cpp``````cpp~JobQueue( ) {}```**
**```cpp``````cppvoid``````cppsubmitJob(``````cppconst``````cppT& x) {```**
**```cpp``````cppboost::mutex::scoped_lock lock(mutex_);```**
**```cpp``````cpplist_.push_back(x);```**
**```cpp``````cppworkToBeDone_.notify_one( );```**
**```cpp``````cpp}```**
**```cpp``````cppT getJob( ) {```**
**```cpp``````cppboost::mutex::scoped_lock lock(mutex_);```**
**```cpp``````cppworkToBeDone_.wait(lock);``````cpp// Wait until this condition is```**
**```cpp``````cpp// satisfied, then lock the mutex```**
**```cpp``````cppT tmp = list_.front( );```**
**```cpp``````cpplist_.pop_front( );```**
**```cpp``````cppreturn``````cpp(tmp);```**
**```cpp``````cpp}```**
**```cppprivate``````cpp:```**
**```cpp``````cppstd::list<T> list_;```**
**```cpp``````cppboost::mutex mutex_;```**
**```cpp``````cppboost::condition workToBeDone_;```**
**```cpp};```**
**```cppJobQueue<Request> myJobQueue;```**
**```cppvoid``````cppboss( ) {```**
**```cpp``````cppfor``````cpp(;;) {```**
**```cpp``````cpp// Get the request from somewhere```**
**```cpp``````cppRequest req;```**
**```cpp``````cppmyJobQueue.submitJob(req);```**
**```cpp``````cpp}```**
**```cpp}```**
**```cppvoid``````cppworker( ) {```**
**```cpp``````cppfor``````cpp(;;) {```**
**```cpp``````cppRequest r(myJobQueue.getJob( ));```**
**```cpp``````cpp// Do something with the job...```**
**```cpp``````cpp}```**
**```cpp}```**
**```cppint``````cppmain( ) {```**
**```cpp``````cppboost::``````cppthread``````cppthr1(boss);```**
**```cpp``````cppboost::``````cppthread``````cppthr2(worker);```**
**```cpp``````cppboost::``````cppthread``````cppthr3(worker);```**
**```cpp``````cppthr1.join( );```**
**```cpp``````cppthr2.join( );```**
**```cpp``````cppthr3.join( );```**
**```cpp}```**
```
boost::mutex::scoped_lock lock(mutex_); 
workToBeDone_.wait(lock);
     这两行代码，第一行锁定这个mutex对象。第二行代码解开这个mutex上的锁，然后进行等待或者休眠，直到它的条件得到了满足。这个mutex互斥对象的解锁让其他的线程能够使用这个mutex对象，它们中的某个需要设置这个等待条件，之后通知另外的线程。
    notify_all函数，通知那些所有正在等待某个条件变为真的线程，那些线程随后进入运行状态。wait方法做两件事情：它一直等待直到有人在它正等待的condition上调用notify_one或notify_all，然后它就试图锁定相关的mutex。当调用的是notify_all时，尽管多个等待的线程都尽量去获得下一个锁，但谁将获得依赖于这个mutex的类型和使用的优先策略。
    一个condition对象能让消费者线程休眠，因此在还没有碰到一个condition时处理器可以去处理别的事情。例如一个web服务器使用一个工作线程池来处理进来的请求。当没有需求进来时，让这些子线程处于等待状态比让它们循环的查询或者睡眠然后偶尔唤醒来检查这个队列，要好很多。
**五、只初始化一次共享资源**
```
**```cpp#include <iostream>```**
**```cpp#include <boost/thread/thread.hpp>```**
**```cpp#include <boost/thread/once.hpp>```**
**```cpp// Some sort of connection class that should only be initialized once```**
**```cppstruct``````cppConn {```**
**```cpp``````cppstatic``````cppvoid``````cppinit( ) { ++i_; }```**
**```cpp``````cppstatic``````cppboost::once_flag init_;```**
**```cpp``````cppstatic``````cppint``````cppi_;```**
**```cpp``````cpp// ...```**
**```cpp};```**
**```cppint``````cppConn::i_ = 0;```**
**```cppboost::once_flag Conn::init_ = BOOST_ONCE_INIT;```**
**```cppvoid``````cppworker( ) {```**
**```cpp``````cppboost::call_once(Conn::init, Conn::init_);```**
**```cpp``````cpp// Do the real work...```**
**```cpp}```**
**```cppConn c;``````cpp// You probably don't want to use a global, so see the```**
**```cpp``````cpp// next Recipe```**
**```cppint``````cppmain( ) {```**
**```cpp``````cppboost::thread_group grp;```**
**```cpp``````cppfor``````cpp(``````cppint``````cppi = 0; i < 100; ++i)```**
**```cpp``````cppgrp.create_thread(worker);```**
**```cpp``````cppgrp.join_all( );```**
**```cpp``````cppstd::cout << c.i_ <<``````cpp'\n'``````cpp;``````cpp// c.i_ = 1```**
```cpp
}
```
```
    一个共享资源不得不在某个地方被初始化，并且你希望第一次使用这个资源的线程来完成初始化工作。一个once_flag类型和call_once函数能够保证多个线程不会重复的初始化同一个对象。首先，必须使用BOOST_ONCE_INIT宏来初始化这个once_flag对象。boost::once_flag Conn::init_ = BOOST_ONCE_INIT; 之后调用call_once函数，boost::call_once(Conn::init, Conn::init_); 第一个形参是希望被执行一次的初始化函数的地址。
**六、给线程函数传递一个参数**
```
**```cpp#include <iostream>```**
**```cpp#include <string>```**
**```cpp#include <functional>```**
**```cpp#include <boost/thread/thread.hpp>```**
**```cpp// A typedef to make the declarations below easier to read```**
**```cpptypedef``````cppvoid``````cpp(*WorkerFunPtr)(``````cppconst``````cppstd::string&);```**
**```cpptemplate``````cpp<``````cpptypename``````cppFunT,``````cpp// The type of the function being called```**
**```cpp``````cpptypename``````cppParamT >``````cpp// The type of its parameter```**
**```cppstruct``````cppAdapter {```**
**```cpp``````cppAdapter( FunT f, ParamT& p) :``````cpp// Construct this adapter and set the```**
**```cpp``````cppf_(f), p_(&p) {}``````cpp// members to the function and its arg```**
**```cpp``````cppvoid``````cppoperator( )( ) {``````cpp// This just calls the function with its arg```**
**```cpp``````cppf_(*p_);```**
**```cpp``````cpp}```**
**```cppprivate``````cpp:```**
**```cpp``````cppFunT    f_;```**
**```cpp``````cppParamT* p_;``````cpp// Use the parameter's address to avoid extra copying```**
**```cpp};```**
**```cppvoid``````cppworker(``````cppconst``````cppstd::string& s) {```**
**```cpp``````cppstd::cout << s <<``````cpp'\n'``````cpp;```**
**```cpp}```**
**```cppint``````cppmain( ) {```**
**```cpp``````cppstd::string s1 =``````cpp"This is the first thread!"``````cpp;```**
**```cpp``````cppstd::string s2 =``````cpp"This is the second thread!"``````cpp;```**
**```cpp``````cppboost::``````cppthread``````cppthr1(Adapter<WorkerFunPtr, std::string>(worker, s1));```**
**```cpp``````cppboost::``````cppthread``````cppthr2(Adapter<WorkerFunPtr, std::string>(worker, s2));```**
**```cpp``````cppthr1.join( );```**
**```cpp``````cppthr2.join( );```**
**```cpp}```**
```
    使用这个函数适配器类模板，你就可以给线程函数传递参数了。如果你需要传递多个参数，仅需要在这个适配器中增加另一个类型和成员变量。

