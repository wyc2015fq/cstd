# ICE笔记(08)：C++线程与并发 - gauss的专栏 - CSDN博客
2014年04月25日 23:26:40[gauss](https://me.csdn.net/mathlmx)阅读数：276
**【 [colorado](http://blog.csdn.net/colorado) 】**
Ice 提供了一个简单的线程抽象层，用以编写可移植的多线程程序。通过同步原语可以实现不同粒度的并发控制。
**1、互斥体**
IceUtil::Mutex类和IceUtil::StaticMutex提供了简单的非递归互斥机制。两者完全类似，只是StaticMutex可以静态声明，如下所示：
static IceUtil::StaticMutex myStaticMutex =ICE_STATIC_MUTEX_INITIALIZER;
成员函数：
• lock：尝试锁定互斥体。如果已锁定，就会挂起直到获得互斥体。
• tryLock：尝试锁定互斥体。如果已被当前线程锁定就返回true，否则返回false。
• unlock：解锁互斥体。
使用这些类必须遵守以下规则：
• 不能重复调用同一互斥体的lock，多次调用的行为不确定。
• 只能在已锁定的互斥体上解锁。否则会导致不确定的行为。
例如：
IceUtil::Mutex mutex;
mutex.lock();
Lines lines= getLines();
mutex.unlock();
缺点：无法确保解锁，异常和提前返回都可能造成难以跟踪的死锁问题。为了避免发生这样的问题，Mutex类含有两个助手类Lock 和TryLock：
成员函数：
• void acquire() const;
挂起当前线程直到获得锁。如果调用者在一个它已锁定的互斥体上调用acquire，函数将抛出ThreadLockedException。
• bool tryAcquire() const;
如果能够获取互斥体，就返回true及锁定的互斥体；否则返回false。如果调用者在一个它已锁定的互斥体上调用tryAcquire，函数抛出ThreadLockedException。
 void release() const;
释放先前锁定的互斥体。如果调用者在一个它先前未加锁的互斥体上调用release，函数抛出ThreadLockedException。
• bool acquired() const;
如果调用者先前已锁定互斥体，就返回true；否则返回false。如果使用TryLock，必须在实例化之后，调用acquired以测试是否真正获得了锁。
void SomeClass::someFunction(/* params here... */)
{
    IceUtil::Mutex::Lock lock(mutex); // 使用Lock类加锁
    // ...
    if (someCondition) {
        // ...
        return; // 提前返回
    }
    // ...
} // lock析构解锁
**2、 递归互斥体**
非递归互斥体不能被多次加锁，即使是持有锁的线程也不行。有些情况下需要对互斥体多次加锁，这就需要递归互斥体RecMutex，它的成员函数如下：
• lock
尝试获取互斥体。如果互斥体被另一个线程锁住就挂起当前线程，直到互斥体变得可用为止。如果互斥体可用或已被当前线程锁住，就会锁住互斥体，并立即返回。
• tryLock
tryLock函数的功能与lock类似，但如果互斥体已被另一个线程锁住，它不会阻塞调用者，而会返回false。否则返回值是true。
• unlock
解锁互斥体。
使用递归互斥体必须遵守的规则：
• 只有持有互斥体，才能调用该互斥体的unlock。
• 调用unlock的次数必须和调用lock的次数相同。
{
    IceUtil::RecMutex::Lock lock(_mutex);
    // ...
}
**3、读写递归互斥体**
RWRecMutex类把单个锁操作划分成了readLock 和 writeLock 操作。多个读取者可以并行地获取互斥体，但任一时刻只有一个写入者能够持有互斥体。
成员函数：
• readLock
尝试获取读锁。如果目前有写入者持有互斥体或线程正在等待锁升级，调用者就会挂起，直到互斥体变得可用于读取为止。如果可以获取互斥体，或者目前只有读取者持有互斥体，这个调用就会锁住互斥体，并立即返回。
• tryReadLock
尝试获取读锁。如果锁目前由写入者持有或线程正在等待锁升级，这个函数就会返回false。否则，它获取锁并返回true。
• timedReadLock
类似tryReadLock，有超时限制。
• writeLock
获取写锁。如果目前有读取者或写入者持有互斥体或另一个线程正在等待锁升级，调用者就会挂起，直到互斥体可用于写为止。如果可以获取互斥体，这个调用就获取锁并立即返回。
• tryWriteLock
尝试获取写锁。如果锁目前由读取者或写入者持有，或者另一个线程正在等待锁升级，这个函数返回false。否则，它获取锁并返回true。
• timedWriteLock
类似tryWriteLock，有超时限制。
• unlock
解锁互斥体（不管目前持有锁的是读取者还是写入者）。
• upgrade
使读锁升级成写锁。如果目前有其他读取者持有互斥体，调用者就会挂起，直到互斥体变得可用于写为止。如果互斥体可用，调用者就持有锁并立即返回。一次只有一个读取者可升级锁。
• timedUpgrade
类似upgrade，有超时限制。
•downgrade
将写锁降级为读锁。
使用读、写锁必须遵守一些规则：
• 只有持有锁，才能解锁。
• 锁定次数必须与解锁次数相同。
• 只能在读锁上升级锁，升级为写锁。
• 只能在写锁上降级锁，降级为读锁。
• 升级锁是非递归的。
• 调用downgrade(或unlock)的次数必须与调用writeLock和upgrade次数一样多。
读、写锁的使用
namespace Filesystem {
    // ...
    class FileI : virtual public File,virtual public Filesystem::NodeI {
    public:
        // ...
    private:
        Lines _lines;
        IceUtil::RWRecMutex _fileMutex;
    };
    // ...
}
Filesystem::Lines
Filesystem::FileI::read(const Ice::Current&) const
{
    IceUtil::RWRecMutex::RLock lock(_fileMutex); // Read lock
    return _lines;
}
void
Filesystem::FileI::write(const Filesystem::Lines& text,const Ice::Current&)
{
    IceUtil::RWRecMutex::WLock lock(_fileMutex); // Write lock
    _lines = text;
}
读、写锁定函数的超时时间由IceUtil::Time类指定。例如：
IceUtil::RWRecMutex::TryWLock lock(_mutex, IceUtil::Time::seconds(2));
**4、监视器**
监视器与互斥体相同之处在于：任一时刻只有一个线程在临界区内活动；不同之处则是，监视器可以在临界区内挂起线程；这样，另一个线程就能进入临界区。后进来的线程可以离开监视器（从而解除监视器的加锁），或者在监视器内挂起自己；不管哪种情况，原来的线程都会被唤醒，继续在监视器内执行。监视器也可以在临界区中挂起多个线程。
监视器允许线程检查某个条件，如果条件为假，就让自己休眠；另一个线程通过改变该条件可以唤醒休眠的线程。
成员函数：
• lock
尝试锁住监视器。如果监视器已被另外的线程锁住，线程就会挂起，直到监视器可用为止。在调用返回时，监视器已被锁住。
• tryLock
尝试锁住监视器。如果能够锁住监视器就返回true。如果监视器已被另外的线程锁住，返回false。
• unlock
解除监视器的加锁。如果有另外的线程在等待进入监视器（也就是阻塞在lock 调用中），其中一个线程会被唤醒，并锁住监控器。
• wait
挂起发出调用的线程，同时释放监视器上的锁。其他线程可以调用notify 或notifyAll 来唤醒在wait调用中挂起的线程。当wait 调用返回时，监视器重新被锁住，而挂起的线程会恢复执行。
• timedWait
挂起发出调用的线程，直到到达指定的超时值。如果有另外的线程调用notify 或notifyAll，在发生超时之前唤醒挂起的线程，这个调用返回true，监视器重新被锁住，挂起的线程恢复执行。如果发生超时，函数返回false。
• notify
这个函数唤醒目前在wait 调用中挂起的一个线程。如果在调用notify 时没有这样的线程，通知就会丢弃。
发出通知不会导致另外的线程立即运行。只有当发出通知的线程调用wait或timedWait或解锁监视器时，另外的线程才会得以运行。
• notifyAll
唤醒目前在wait 调用中挂起的所有线程。和notify一样，如果这时没有挂起的线程，对notifyAll 的调用就会丢弃。
使用监视器必须遵守一些规则：
• 只有持有锁时，才能调用unlock。
• 只有持有锁时，才能调用wait或timedWait。
• 只有持有锁时，调用notify或notifyAll。
• 解锁次数必须与加锁次数相同。
• wait返回时，须要重新测试条件。
实例：使用队列的生产者、消费者。
#include <IceUtil/Monitor.h>
// Queue 是监视器
template<class T> class Queue : public IceUtil::Monitor<IceUtil::Mutex> {
public:
    // 向队列中放入数据
    void put(const T& item) {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this); //锁住监视器
        _q.push_back(item);
        notify(); //唤醒休眠在wait调用中的消费者，通知它们已有数据可用。
    }
    // 从队列中取出数据
    T get() {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this); //锁住监视器
        while (_q.size() == 0)    wait(); //消费者在wait调用中挂起，解锁监视器
        T item = _q.front();            //以便生产者进入监视器，把数据项放入队列
        _q.pop_front();
        return item;
    }
private:
    list<T> _q;
};
在编写代码时，必须遵循同样的模式：
• 只有持有锁时，才测试某个条件。
• 总是在围绕wait的循环中重新测试条件。如果条件仍未满足，就再次调用wait。
为避免在放入N个数据时，发送N个通知，可以限制对notify()的调用：
if (_q.size() == 1) notify();
只要放入一个数据，就发送通知；其它时候不发送通知。这个方法适用于一个读取者的情况，有多个读取者时，其它的读取者会永远睡眠在get方法中。采取如下方法改进：
if (_q.size() >= _wakeupThreshold) notifyAll();
一旦队列长度超过阀值，写入者就会唤醒所有等待的读取者。缺点是阀值难以确定。多个读取者同时醒来，获取监视器的只能有一个读取者，这会造成大量线程上下文切换。因此更好的办法是一次唤醒一个等待者：
#include <IceUtil/Monitor.h>
template<class T> class Queue: public IceUtil::Monitor<IceUtil::Mutex> {
public:
    Queue() : _waitingReaders(0) {} //缺省构造器，初始化处于等待中的读取者的数目
    void put(const T & item) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    _q.push_back(item);
    if (_waitingReaders) notify();
    }
    T get() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    while (_q.size() == 0) {
        try {
            ++_waitingReaders;
            wait();
            --_waitingReaders;
        } catch (...) {
            --_waitingReaders;
            throw;
        }
    }
    T item = _q.front();
    _q.pop_front();
    return item;
    }
private:
    list<T> _q;
    short _waitingReaders;
};
**5、条件变量**
条件变量类似监视器，允许线程进入一个临界区，测试条件，释放它持有的锁并在临界区内睡眠。另一线程随即可进入临界区，更改条件，最后向睡眠线程发信号，使之在睡眠点处恢复并再次锁定临界区。
条件变量提供了监视器功能的子集，监视器总能替代条件变量。但条件变量更小，在严格受内存限制的情况下它会很重要。
Cond 成员函数：
• wait
挂起发起调用的线程，同时释放条件变量的锁。在调用中挂起的线程等待另一线程调用signal或broadcast唤醒自己。当wait完成时，挂起的线程持有锁，并恢复执行。
• timedWait
挂起发起调用的线程直到指定的超时值。如果另一线程调用signal或broadcast，并在超时到期前唤醒挂起的线程，本调用返回true，挂起的线程持有锁，并恢复执行。否则，超时过期，本函数返回false。
• signal
唤醒一个当前调用time或timedWait挂起的线程。如果调用signal时，没有线程在调用wait或timedWait中挂起，信号就被丢弃。发送信号不一定会立即切换到另一线程；调用singal的线程可能会继续运行。当然，根据线程库的不同，signal也可能会立即切换到另一线程上下文。
• broadcast
唤醒当前在调用wait或timedWait中挂起的所有线程。类似signal，如果此时没有线程被挂起，对broadcast的调用将被丢弃。
监视器需要持有锁才能调用notify和notifyAll，与之对照，条件变量不需持有锁就可调用signal和broadcast。正确使用条件变量的方法是在解锁的情况下调用signal；
Mutex m;
Cond c;
// ...
{
    Mutex::Lock sync(m);
    //...
}
c.signal();
如果在sync锁定情况下调用signal，就会发生两次唤醒，第一次是因调用signal，但当前线程仍然锁定互斥体，会使等待线程立即挂起；当离开锁定范围，互斥体解锁后，再次唤醒等待线程。因此调用signal的正确方法是在解除了sync的锁定之后进行。
broadcast应慎用，它会使在wait中挂起的所有线程会立即试图获取互斥体，但只有一个会成功，所有其它线程会再次被挂起。
**6、效率**
RWRecMutex最慢，Mutex最快。
**7、线程**
Ice中的线程由两个类实现：ThreadControl 和Thread。
创建线程，必须继承Thread 类，实现run方法。不能从run抛出异常，如果run被异常终止，就调用::std::terminate。
Thread 成员函数 ：
• start
启动新创建的线程，调用run 方法。返回新线程的ThreadControl对象。在指定的线程上只能调用一次。
• getThreadControl
返回线程的ThreadControl对象，只有线程启动后才能调用。
• id
返回线程ID，用于调试和跟踪。
•isAlive
测试线程是否正在运行。
• operator==，operator!=，operator<
比较线程的内存地址。可将Thread对象用于有序的STL容器。
ThreadPtr为智能指针，用以保证自动进行清理：一旦引用计数降到零，它的析构器就调用delete，释放动态分配的Thread对象。
注意如下情况：
不能在堆上分配线程对象；
不要明确删除线程，ThreadPtr自动销毁；
不要在线程对象的构造函数中启动线程。
ThreadControl类
• ThreadControl
缺省构造器返回的当前线程对象。
IceUtil::ThreadControl self; 
cout << self.id() << endl; 
• join
挂起发出调用的线程，直到联结的线程终止。例如：
IceUtil::ThreadPtr t = new ReaderThread;
IceUtil::ThreadControl tc = t->start(); 
tc.join();
只能在一个线程中调用另一个线程的join 方法，不能在多个线程中调用某个线程的join 方法。如果对已联结的线程或已分离的线程调用join，也会产生不确定的行为。必须在主线程中联结每个子线程。若联结失败，则具有不确定的行为。
• detach
分离一个线程。一旦线程分离就不能再联结。对已分离的线程或已联结的线程调用detach，会产生不确定的行为。
• sleep
挂起线程，时间由Time 参数指定。
• yield
使得它所针对的线程放弃CPU，让其他线程运行。
• operator==，operator!=
比较线程ID。仅对未分离或未联结的线程，比较才有意义。
使用线程必须遵守的规则：
• 不允许run抛出异常。
• 不要联结或分离不是你自己创建的线程。
• 对于创建的每个线程，必须严格地进行一次联结或分离；否则可能造成资源泄漏。
• 不要在多个线程中针对某个线程调用join。
• 在你创建的所有其他线程终止之前，不要离开main。
• 在销毁你创建的所有Ice::Communicator对象之前，不要离开main。
• 不能在临界区里调用yield。
#include <vector>
#include <IceUtil/Thread.h>
// ...
Queue q;
class ReaderThread : public IceUtil::Thread {
    virtual void run() {
        for (int i = 0; i < 100; ++i)
            cout << q.get() << endl;
    }
};
class WriterThread : public IceUtil::Thread {
    virtual void run() {
        for (int i = 0; i < 100; ++i)
            q.put(i);
    }
};
int main()
{
    vector<IceUtil::ThreadControl> threads;
    int i;
    for (i = 0; i < 5; ++i) {
        IceUtil::ThreadPtr t = new ReaderThread;
        threads.push_back(t->start());
    }
    for (i = 0; i < 5; ++i) {
        IceUtil::ThreadPtr t = new WriterThread;
        threads.push_back(t->start());
    }
    for (vector<IceUtil::ThreadControl>::iterator i= threads.begin(); i != threads.end(); ++i) {
        i->join();
    }
}
**8、可移植的信号处理**
IceUtil::CtrlCHandler类用于处理Ctrl+C及其他类似的发给C++进程的信号，常用于关闭Ice 服务器中的通信器。它的成员函数：
• 构造器
CtrlCHandler(CtrlCHandlerCallback = 0);
用回调函数构造实例，缺省回调函数为0，表示忽略信号。一个进程中任一时刻只能有一个CtrlCHandler实例，CtrlCHandler 实例必须在启动任何线程之前创建，特别是必须在初始化Ice 通信器之前创建。
• 析构器
~CtrlCHandler();
销毁实例，后续信号会被忽略。
• void setCallback(CtrlCHandlerCallback);
设置新的回调函数。
• CtrlCHandlerCallback getCallback() const;
获得当前回调函数。
