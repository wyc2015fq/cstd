# C++11 thread - DoubleLi - 博客园






windows系统中，需要vs2012才支持。


1.线程的创建
C++11线程类std::thread，头文件include <thread>
首先，看一个最简单的例子：





**[cpp]**[view plain](http://blog.csdn.net/tujiaw/article/details/8245130#)[copy](http://blog.csdn.net/tujiaw/article/details/8245130#)



- void my_thread()  
- {  
-     puts("hello, world");  
- }  
- 
- int main(int argc, char *argv[])  
- {  
-     std::thread t(my_thread);  
-     t.join();  
- 
-     system("pause");  
- return 0;  
- }  







实例化一个线程对象t，参数my_thread是一个函数，在线程创建完成后将被执行，
t.join()等待子线程my_thread执行完之后，主线程才可以继续执行下去，此时主线程会
释放掉执行完后的子线程资源。


当然，如果不想等待子线程，可以在主线程里面执行t.detach()将子线程从主线程里分离，
子线程执行完成后会自己释放掉资源。分离后的线程，主线程将对它没有控制权了。
相对于以前使用过的beginthread传多个参数需要传入struct地址，
boost::thread传参需要bind，std::thread传参真的非常方便，而且可读性也很好。
下面例子在实例化线程对象的时候，在线程函数my_thread后面紧接着传入两个参数。



**[cpp]**[view plain](http://blog.csdn.net/tujiaw/article/details/8245130#)[copy](http://blog.csdn.net/tujiaw/article/details/8245130#)



- #include <iostream>  
- #include <stdlib.h>  
- #include <thread>  
- #include <string>  
- 
- void my_thread(int num, const std::string& str)  
- {  
-     std::cout << "num:" << num << ",name:" << str << std::endl;  
- }  
- 
- int main(int argc, char *argv[])  
- {  
- int num = 1234;  
-     std::string str = "tujiaw";  
-     std::thread t(my_thread, num, str);  
-     t.detach();  
- 
-     system("pause");  
- return 0;  
- }  




2.互斥量
多个线程同时访问共享资源的时候需要需要用到互斥量，当一个线程锁住了互斥量后，其他线程必须等待这个互斥量解锁后才能访问它。thread提供了四种不同的互斥量：
独占式互斥量non-recursive (std::mutex)
递归式互斥量recursive (std::recursive_mutex)
允许超时的独占式互斥量non-recursive that allows timeouts on the lock functions(std::timed_mutex)
允许超时的递归式互斥量recursive mutex that allows timeouts on the lock functions (std::recursive_timed_mutex)


独占式互斥量
独占式互斥量加解锁是成对的，同一个线程内独占式互斥量在没有解锁的情况下，再次对它进行加锁这是不对的，会得到一个未定义行为。
如果你想thread1输出10次10，thread2输出10次20，如果你想看到一个正确的显示效果，下面程序是做不到的，因为在thread1输出的时候，
thread2也会执行，输出的结果看起来有点乱（std::cout不是线程安全的），所以我们需要在它们访问共享资源的时候使用互斥量加锁。打开代码里面的三行注释就可以得到正确的结果了。在线程1中std::mutex使用成员函数lock加锁unlock解锁，看起来工作的很好，但这样是不安全的，你得始终记住lock之后一定要unlock，但是如果在它们中间出现了异常或者线程直接退出了unlock就没有执行，因为这个互斥量是独占式的，所以在thread1没有解锁之前，其他使用这个互斥量加锁的线程会一直处于等待状态得不到执行。lock_guard模板类使用RAII手法封装互斥量，在实例化对象的时候帮你加锁，并且能保证在离开作用域的时候自动解锁，所以你应该用lock_guard来帮你加解锁。



**[cpp]**[view plain](http://blog.csdn.net/tujiaw/article/details/8245130#)[copy](http://blog.csdn.net/tujiaw/article/details/8245130#)



- #include <iostream>  
- #include <stdlib.h>  
- #include <thread>  
- #include <string>  
- #include <mutex>  
- 
- int g_num = 0;  
- std::mutex g_mutex;  
- 
- void thread1()  
- {  
- //g_mutex.lock();  
-     g_num = 10;  
- for (int i=0; i<10; i++){  
-         std::cout << "thread1:" << g_num << std::endl;  
-     }  
- //g_mutex.unlock();  
- }  
- 
- void thread2()  
- {  
- //std::lock_guard<std::mutex> lg(g_mutex);  
-     g_num = 20;  
- for (int i=0; i<10; i++){  
-         std::cout << "thread2:" << g_num << std::endl;  
-     }  
- }  
- 
- int main(int argc, char *argv[])  
- {  
-     std::thread t1(thread1);  
-     std::thread t2(thread2);  
-     t1.join();  
-     t2.join();  
- 
-     system("pause");  
- return 0;  
- }  





递归式互斥量
与独占式互斥量不同的是，同一个线程内在互斥量没有解锁的情况下可以再次进行加锁，不过他们的加解锁次数需要一致，递归式互斥量我们平时可能用得比较少些。


允许超时的互斥量
如果线程1对共享资源的访问时间比较长，这时线程2可能等不了那么久，所以设置一个超时时间，在超时时间内如果线程1中的互斥量还没有解锁，线程2就不等了，继续往下执行。
lock_guard只是提供了对互斥量最基本的加解锁封装，而unique_lock提供了多种构造方法，使用起来更加灵活，对于允许超时的互斥量需要使用unnique_lock来包装。



**[cpp]**[view plain](http://blog.csdn.net/tujiaw/article/details/8245130#)[copy](http://blog.csdn.net/tujiaw/article/details/8245130#)



- std::timed_mutex g_timed_mutex;  
- void thread1()  
- {  
-     std::unique_lock<std::timed_mutex> tl(g_timed_mutex);  
-     ::Sleep(3000); // 睡眠3秒  
-     puts("thread1");  
- }  
- 
- void thread2()  
- {  
-     std::unique_lock<std::timed_mutex> tl(g_timed_mutex, std::chrono::milliseconds(1000)); // 超时时间1秒  
-     puts("thread2");  
- }  
- 
- int main(int argc, char *argv[])  
- {  
-     std::thread t1(thread1);  
-     ::Sleep(100); // 让线程1先启动  
-     std::thread t2(thread2);  
-     t1.join();  
-     t2.join();  
- 
-     system("pause");  
- return 0;  
- }  




注意死锁
有时，一个操作需要对一个以上的mutex加锁，这时请注意了，这样很可能造成死锁。



**[cpp]**[view plain](http://blog.csdn.net/tujiaw/article/details/8245130#)[copy](http://blog.csdn.net/tujiaw/article/details/8245130#)



- struct Widget  
- {  
-     std::mutex mutex_;  
-     std::string str_;  
- };  
- 
- void foo(Widget& w1, Widget& w2)  
- {  
-     std::unique_lock<std::mutex> t1(w1.mutex_);  
-     std::unique_lock<std::mutex> t2(w2.mutex_);  
- // do something  
- }  
- Widget g_w1, g_w2;  



当一个线程调用foo(g_w1, g_w2),另外一个线程调用foo(g_w2, g_w1)的时候，
线程1：                线程2：
w1.mutex_.lock         ...
...                    w2.mutex_.lock
...                    ...
w2.mutex_.lock等待     ...
                       w1.mutex_lock等待
可能的执行顺序：
线程1中的w1上锁；
线程2中的w2上锁；
线程1中的w2上锁，此时由于w2已经在线程2中上过锁了，所以必须等待；
线程2中的w1上锁，此时由于w1已经在线程1中上过锁了，所以必须等待；
这样两个线程都等不到对方释放锁，都处于等待状态造成了死锁。
thread提供了一个std::lock函数可以对多个互斥量同时加锁，每个线程里面的
w1和w2会同时上锁，他们之间就没有间隙了，如上将foo函数改为如下形式就可以了：



**[cpp]**[view plain](http://blog.csdn.net/tujiaw/article/details/8245130#)[copy](http://blog.csdn.net/tujiaw/article/details/8245130#)



- void foo(Widget& w1, Widget& w2)  
- {  
-     std::unique_lock<std::mutex> t1(w1.mutex_, std::defer_lock);  
-     std::unique_lock<std::mutex> t2(w2.mutex_, std::defer_lock);  
-     std::lock(t1, t2);  
- // do something  
- }  



在实例化的时候先不要加锁，等到两个对象都创建完成之后再一起加锁。


在初始化的时候保护数据

如果你的数据仅仅只在初始化的时候进行保护，使用一个互斥量是不行的，在初始化完成后会导致没必要的同步，[C++](http://lib.csdn.net/base/cplusplus)11提供了一些方法来解决这个问题。



3.线程间同步，条件变量

如果我们在线程间共享数据，经常会存在一个线程等待另外一个线程的情况，它们之间存在先后关系。

这个与互斥量不同，互斥量是保证多个线程的时候当前只有一个线程访问加锁的代码块，它们之间是不存在先后关系的。

如下例子：线程1需要等到线程2将flag设置为非0才进行打印





**[cpp]**[view plain](http://blog.csdn.net/tujiaw/article/details/8245130#)[copy](http://blog.csdn.net/tujiaw/article/details/8245130#)



- #include <iostream>  
- #include <thread>  
- #include <mutex>  
- #include <condition_variable>  
- #include <functional>  
- 
- class Foo  
- {  
- public:  
-     Foo()  
-         : flag_(0)  
-         , thread1_(std::bind(&Foo::threadFunc1, this))  
-         , thread2_(std::bind(&Foo::threadFunc2, this))  
-     {  
-     }  
- 
-     ~Foo()  
-     {  
-         thread1_.join();  
-         thread2_.join();  
-     }  
- 
- private:  
- void threadFunc1()  
-     {  
-         {  
-             std::unique_lock<std::mutex> ul(mutex_);  
- while (0 == flag_) {  
-                 cond_.wait(ul);  
-             }  
-             std::cout << flag_ << std::endl;  
-         }  
-     }  
- 
- void threadFunc2()  
-     {  
- // 为了测试，等待3秒  
-         std::this_thread::sleep_for((std::chrono::milliseconds(3000)));  
-         std::unique_lock<std::mutex> ul(mutex_);  
-         flag_ = 100;  
-         cond_.notify_one();  
-     }  
- 
- int flag_;  
-     std::mutex mutex_;  
-     std::condition_variable cond_;  
-     std::thread thread1_;  
-     std::thread thread2_;  
- };  
- 
- int _tmain(int argc, _TCHAR* argv[])  
- {  
-     Foo f;  
- 
-     system("pause");  
- return 0;  
- }  



从代码可以看出，虽然线程1明显比线程2快些（人为制造等待3秒），但是线程1还是会等待线程2将flag设置为非0后才进行打印的。



这里有几个地方需要注意：

1> Foo类成员变量定义的顺序，mutex和cond必须在thread的前面。

原因是：如果线程的定义在前面，线程初始化完成之后立马会执行线程函数，而线程函数里用到了mutex和cond，此时如果mutex和cond还没初始化完成，就会出现内存错误。

2>由于同时有两个线程需要操作flag变量，所以在读写的时候要加锁， std::unique_lock<std::mutex>会保证构造的时候加锁，离开作用域调用析构的时候解锁，所以不用担心加解锁不匹配。

3>threadFunc1中的while (0 == flag_)， 必须这样写不能写成if (0 == flag_)，因为在多核CPU下会存在虚假唤醒（ spurious wakes）的情况。

4>cond_.wait(ul);条件变量在wait的时候会释放锁的，所以其他线程可以继续执行。



4.线程池





**[cpp]**[view plain](http://blog.csdn.net/tujiaw/article/details/8245130#)[copy](http://blog.csdn.net/tujiaw/article/details/8245130#)



- #include <iostream>  
- #include <stdlib.h>  
- #include <functional>  
- #include <thread>  
- #include <string>  
- #include <mutex>  
- #include <condition_variable>  
- #include <vector>  
- #include <memory>  
- #include <assert.h>  
- #include <algorithm>  
- #include <queue>  
- #include <process.h>  
- #include <Windows.h>  
- 
- class ThreadPool  
- {  
- public:  
- typedef std::function<void()> Task;  
- 
-     ThreadPool(int num)  
-         : num_(num)  
-         , maxQueueSize_(0)  
-         , running_(false)  
-     {  
-     }  
- 
-     ~ThreadPool()  
-     {  
- if (running_) {  
-             stop();  
-         }  
-     }  
- 
-     ThreadPool(const ThreadPool&) = delete;  
- void operator=(const ThreadPool&) = delete;  
- 
- void setMaxQueueSize(int maxSize)  
-     {  
-         maxQueueSize_ = maxSize;  
-     }  
- 
- void start()  
-     {  
-         assert(threads_.empty());  
-         running_ = true;  
-         threads_.reserve(num_);  
- for (int i = 0; i<num_; i++) {  
-             threads_.push_back(std::thread(std::bind(&ThreadPool::threadFunc, this)));  
-         }  
-     }  
- 
- void stop()  
-     {  
-         {  
-             std::unique_lock<std::mutex> ul(mutex_);  
-             running_ = false;  
-             notEmpty_.notify_all();  
-         }  
- 
- for (auto &iter : threads_) {  
-             iter.join();  
-         }  
-     }  
- 
- void run(const Task &t)  
-     {  
- if (threads_.empty()) {  
-             t();  
-         }  
- else {  
-             std::unique_lock<std::mutex> ul(mutex_);  
- while (isFull()) {  
-                 notFull_.wait(ul);  
-             }  
-             assert(!isFull());  
-             queue_.push_back(t);  
-             notEmpty_.notify_one();  
-         }  
-     }  
- 
- private:  
- bool isFull() const  
-     {  
- return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;  
-     }  
- 
- void threadFunc()  
-     {  
-         printf("create id:%d\n", ::GetCurrentThreadId());  
- while (running_) {  
-             Task task(take());  
- if (task) {  
-                 task();  
-             }  
-         }  
-         printf("thread id:%d\n", ::GetCurrentThreadId());  
-     }  
- 
-     Task take()  
-     {  
-         std::unique_lock<std::mutex> ul(mutex_);  
- while (queue_.empty() && running_) {  
-             notEmpty_.wait(ul);  
-         }  
-         Task task;  
- if (!queue_.empty()) {  
-             task = queue_.front();  
-             queue_.pop_front();  
- if (maxQueueSize_ > 0) {  
-                 notFull_.notify_one();  
-             }  
-         }  
- return task;  
-     }  
- 
- private:  
- int num_;  
-     std::mutex mutex_;  
-     std::condition_variable notEmpty_;  
-     std::condition_variable notFull_;  
-     std::vector<std::thread> threads_;  
-     std::deque<Task> queue_;  
- size_t maxQueueSize_;  
- bool running_;  
- };  
- 
- void fun()  
- {  
-     printf("[id:%d] hello, world!\n", ::GetCurrentThreadId());  
- }  
- 
- int _tmain(int argc, _TCHAR* argv[])  
- {  
-     {  
-         printf("main thread id:%d\n", ::GetCurrentThreadId());  
-         ThreadPool pool(3);  
-         pool.setMaxQueueSize(100);  
-         pool.start();  
- //std::this_thread::sleep_for(std::chrono::milliseconds(3000));  
- 
- for (int i = 0; i < 1000; i++) {  
-             pool.run(fun);  
-         }  
-         std::this_thread::sleep_for(std::chrono::milliseconds(3000));  
-     }  
- 
-     system("pause");  
- return 0;  
- }  










