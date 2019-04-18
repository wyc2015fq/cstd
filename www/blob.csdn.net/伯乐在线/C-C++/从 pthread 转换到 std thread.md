# 从 pthread 转换到 std::thread - 文章 - 伯乐在线
原文出处： [spacewander](https://segmentfault.com/a/1190000002655852)
以前一直都是用pthread的API写C++的多线程程序。虽然很早之前就听说，从C++11开始，标准库里已经包含了对线程的支持，不过一直没有拿来用，最近刚好有空，借着pthread的经验学习下std::thread的用法。
# Thread
std::thread的构造函数方便得出人意料，这得感谢std::bind这个神奇的函数。在std::thread的构造函数里，你可以直接传递一个函数和这个函数的参数列表给这个线程。你甚至可以传递一个类成员函数。如果你这么做了，参数列表的第二个参数（第一个参数是被传递的成员函数）会被作为该类成员函数所作用的实例。
是不是有点绕……举个例子来说吧：


```
// 假设buy是一个可调用的函数对象，它即可能是函数指针，也可能是函数对象
std::thread Annie(buy);
// Annie会去执行buy()
std::thread Bob(buy, book, food);
// Bob会去执行buy(book, food)
// 假设buy是Consumer的一个可调用的成员函数
Consumer Clara;
std::thread action(buy, Clara, phone);
// Clara会去执行Consumer.buy(phone)
```
随便提一下，当你创建了一个（非空的）线程对象时，对应线程就会执行，不需要显式的调用start或者run。
如果之前你没有用过pthread，也许不会理解何为“方便得出人意料”。
在pthread里面，你需要这样指定线程执行的函数:


```
pthread_create(&thread, &attr, f, static_cast<void *>(&args));
// 其中f是函数，args是所有参数打包成的结构体。因为pthread_create的第四个参数类型是void*，所以需要强制转型
```
考虑下之前那个Bob买书和饭菜的例子，如果要在pthread里面实现，首先需要定义一个结构体，然后把book和food赋值给这个结构体的成员。
接着把结构体转换成void*类型，传递进去。
这还没完呢，因为刚刚的几步只是实现了“传进去”，还得“取出来”。
之后在函数buy中，再把void*的参数重新转型成某个（可能是一次性的）结构体，最后取出book和food这两个值。
Ok！终于搞定了。随便一提，pthread_create只接受void *f(void *)这样的函数签名。如果你想调用现成的函数，你得包装一下。
这就是为什么std::thread的构造函数“方便得出人意料”。
创建线程后，调用Thread.join就会阻塞到线程执行结束为止（相当于pthread_join）。你也可以选择detach该线程，这时候线程会独立执行，不会随调用者终止而结束。
# Mutex
有时候需要限制多个线程对同一资源的访问，这时候一般会使用Mutex。Mutex就是一把锁，只有某些线程可以同时占用它（通过lock操作）。当线程不用的时候，就得通过unlock操作来释放它。
对于Mutex，std::thread和pthread差不多，无非是pthread_mutex_lock(&mutex)变成了mutex.lock()等等。
不过在std::thread中，mutex往往和lock系列模板一起使用。这是因为lock系列模板包装了mutex类，提供了RAII风格的加锁解锁。


```
{
    std::lock_guard<std::mutex> guard(mutex); // 加锁
    ...
    // 自动解锁
}
```
# Condition variable
有时候线程之间需要某种同步——当某些条件不满足时，停止执行直到该条件被满足。这时候需要引入condition variable，状态变量。
在经典的生产者消费者模式下，生产者和消费者就是通过condition variable来实现同步的。当有限的生产力无法满足日益增长的消费需求时，消费者进程就会去睡一觉，直到它想要的东西生产出来才醒来。


```
std::condition_variable condvar;
consumer:
        std::unique_lock<std::mutex> ulock(mutex);
        condvar.wait(ulock, []{ return msgQueue.size() > 0;});
producer:
        condvar.notify_all();
```
condition_variable需要和unique_lock搭配使用。在一个线程调用wait之前，它必须持有unique_lock锁。当wait被调用时，该锁会被释放，线程会陷入沉睡，等待着~~王子~~生产者发过来的唤醒信号。当生产者调用同一个condition_variable的notify_all方法时，所有沉睡在该变量前的消费者会被唤醒，并尝试重新获取之前释放的unique_lock，继续执行下去。（注意这里发生了锁争用，只有一个消费者能够获得锁，其他消费者得等待该消费者释放锁）。如果只想叫醒一个线程，可以用notify_one。pthread中也提供了对应的方法，分别是pthread_cond_wait,pthread_cond_broadcast,pthread_cond_signal。
wait可以接受两个参数。此时第二个参数用于判断当前是否要沉睡。


```
[]{ return msgQueue.size() > 0;});
```
相当于


```
while (msgQueue.size() <= 0) {
    condvar.wait()
}
```
嗯，还有一个问题。这里把沉睡的线程比作睡美人，万一王子变成了青蛙，来不及唤醒她，那睡美人不就得睡到天荒地老海枯石烂？
为了解决这个问题，通过wait_until和wait_for，你可以设定线程的等待时间。设置notify_all_at_thread_exit也许能帮得上忙。在pthread，对应的调用是pthread_cond_timedwait。
# More
C++11的线程库还提供了其他多线程编程的概念，比如future和atomic。
future
future包装了未来某个计算结果的期诺。当你对所获得的future调用get时，程序会一直阻塞直到future的值被计算出来。（如果future的值已经计算出来了，get调用会立刻获得返回值）而这一切都是在后台执行的。
举个例子：（future相关的内容需要#include <future>）


```
#include <chrono>
#include <iostream>
#include <future>
using namespace std;
int main()
{
    future<int> f1 = async(launch::async, [](){
        std::chrono::milliseconds dura(2000);
        std::this_thread::sleep_for(dura);
        return 0; 
    });
    future<int> f2 = async(launch::async, [](){
        std::chrono::milliseconds dura(2000);
        std::this_thread::sleep_for(dura);
        return 1; 
    });
    cout << "Results are: "
        << f1.get() << " " << f2.get() << "\n";
    return 0;
}
$ g++ -std=c++11 -pthread ./future.cpp
$ time ./a.out 
Results are: 0 1
./a.out  0.00s user 0.00s system 0% cpu 2.012 total # 是两秒左右而不是四秒哦
```
除了async， packaged_task和promise也都返回一个future。也许接下来我可能会写一篇文章，讲讲这三者之间的差别。
# atomic
atomic位于头文件atomic下，实现了类似于java.util.concurrent.atomic的功能。它提供了一组轻量级的、作用在单个变量上的原子操作，是volatile的替代品。有些时候你也可以用它来替换掉Lock（假如整个race condition中只有单个变量）
下面这个例子解释了什么叫做原子操作：


```
#include <atomic>
#include <iostream>
#include <thread>
using namespace std;
const int NUM = 100;
int target = 0;
atomic<int> atomicTarget(0);
template<typename T>
void atomicPlusOne(int trys)
{
    while (trys > 0) {
        atomicTarget.fetch_add(1);
        --trys;
    }
}
void plusOne(int trys)
{
    while (trys > 0) {
        ++target;
        --trys;
    }
}
int main()
{
    thread threads[NUM];
    thread atomicThreads[NUM];
    for (int i = 0; i < NUM; i++) {
        atomicThreads[i] = thread(atomicPlusOne<int>, 10000);
    }
    for (int i = 0; i < NUM; i++) {
        threads[i] = thread(plusOne, 10000);
    }
    for (int i = 0; i < NUM; i++) {
        atomicThreads[i].join();
    }
    for (int i = 0; i < NUM; i++) {
        threads[i].join();
    }
    cout << "Atomic target's value : " << atomicTarget << "\n";
    cout << "Non-atomic target's value : " << target << "\n";
    // atomicTarget的值总是固定的，而target的值每次运行时各不相同
    //
    // g++ -std=c++11 -pthread ./atom.cpp
    // Atomic target's value : 1000000
    // Non-atomic target's value : 842480
    return 0;
}
```
# Pros and Cons
最后总结下std::thread对比于pthread的优缺点：
优点：
1. 简单，易用
2. 跨平台，pthread只能用在POSIX系统上（其他系统有其独立的thread实现）
3. 提供了更多高级功能，比如future
4. 更加C++（跟匿名函数，std::bind，RAII等C++特性更好的集成）
缺点：
1. 没有RWlock。有一个类似的shared_mutex，不过它属于C++14,你的编译器很有可能不支持。
2. 操作线程和Mutex等的API较少。毕竟为了跨平台，只能选取各原生实现的子集。如果你需要设置某些属性，需要通过API调用返回原生平台上的对应对象，再对返回的对象进行操作。
附上我自己写的，分别用std::thread和pthread实现的多生产者多消费者程序。注意行数上的差距。
pthread版本


```
#include <pthread.h>
#include <queue>
#include <stdio.h>
#include <unistd.h>
// 注意pthread_*函数返回的异常值，为了简单（偷懒），我没有去处理它们
pthread_mutex_t mutex;
pthread_cond_t condvar;
std::queue<int> msgQueue;
struct Produce_range {
    int start;
    int end;
};
void *producer(void *args)
{
    int start = static_cast<Produce_range *>(args)->start;
    int end = static_cast<Produce_range *>(args)->end;
    for (int x = start; x < end; x++) {
        usleep(200 * 1000);
        pthread_mutex_lock(&mutex);
        msgQueue.push(x);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&condvar);
        printf("Produce message %d\n", x);
    }
    pthread_exit((void *)0);
    return NULL;
}
void *consumer(void *args)
{
    int demand = *static_cast<int *>(args);
    while (true) {
        pthread_mutex_lock(&mutex);
        if (msgQueue.size() <= 0) {
            pthread_cond_wait(&condvar, &mutex);
        }
        if (msgQueue.size() > 0) {
            printf("Consume message %d\n", msgQueue.front());
            msgQueue.pop();
            --demand;
        }
        pthread_mutex_unlock(&mutex);
        if (!demand) break;
    }
    pthread_exit((void *)0);
    return NULL;
}
int main()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condvar, NULL);
    pthread_t producer1, producer2, producer3, consumer1, consumer2;
    Produce_range range1 = {0, 10};
    pthread_create(&producer1, &attr, producer, static_cast<void *>(&range1));
    Produce_range range2 = {range1.end, range1.end + 10};
    pthread_create(&producer2, &attr, producer, static_cast<void *>(&range2));
    Produce_range range3 = {range2.end, range2.end + 10};
    pthread_create(&producer3, &attr, producer, static_cast<void *>(&range3));
    int consume_demand1 = 20;
    int consume_demand2 = 10;
    pthread_create(&consumer1, &attr, consumer, 
            static_cast<void *>(&consume_demand1));
    pthread_create(&consumer2, &attr, consumer, 
            static_cast<void *>(&consume_demand2));
    pthread_join(producer1, NULL);
    pthread_join(producer2, NULL);
    pthread_join(producer3, NULL);
    pthread_join(consumer1, NULL);
    pthread_join(consumer2, NULL);
}
```
std::thread版本


```
#include <chrono>
#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
// 注意某些调用可能会抛出std::system_error， 为了简单（偷懒），我没有去捕获
std::mutex mutex;
std::condition_variable condvar;
std::queue<int> msgQueue;
void producer(int start, int end)
{
    for (int x = start; x < end; x++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        {        
            std::lock_guard<std::mutex> guard(mutex);
            msgQueue.push(x);
        }
        printf("Produce message %d\n", x);
        condvar.notify_all();
    }
}
void consumer(int demand)
{
    while (true) {
        std::unique_lock<std::mutex> ulock(mutex);
        condvar.wait(ulock, []{ return msgQueue.size() > 0;});
        // wait的第二个参数使得显式的double check不再必要
        printf("Consume message %d\n", msgQueue.front());
        msgQueue.pop();
        --demand;
        if (!demand) break;
    }
}
int main()
{
    std::thread producer1(producer, 0, 10);
    std::thread producer2(producer, 10, 20);
    std::thread producer3(producer, 20, 30);
    std::thread consumer1(consumer, 20);
    std::thread consumer2(consumer, 10);
    producer1.join();
    producer2.join();
    producer3.join();
    consumer1.join();
    consumer2.join();
}
```
