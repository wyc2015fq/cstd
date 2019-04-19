# C++11中std::condition_variable的使用 - xqhrs232的专栏 - CSDN博客
2018年07月11日 18:26:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：86
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/fengbingchun/article/details/73695596](https://blog.csdn.net/fengbingchun/article/details/73695596)
<condition_variable>是C++标准程序库中的一个头文件，定义了C++11标准中的一些用于并发编程时表示条件变量的类与方法等。
条件变量是并发程序设计中的一种控制结构。多个线程访问一个共享资源(或称临界区)时，不但需要用互斥锁实现独享访问以避免并发错误(称为竞争危害)，在获得互斥锁进入临界区后还需要检验特定条件是否成立：
(1)、如果不满足该条件，拥有互斥锁的线程应该释放该互斥锁，把自身阻塞(block)并挂到(suspend)条件变量的线程队列中
(2)、如果满足该条件，拥有互斥锁的线程在临界区内访问共享资源，在退出临界区时通知(notify)在条件变量的线程队列中处于阻塞状态的线程，被通知的线程必须重新申请对该互斥锁加锁。
C++11的标准库中新增加的条件变量的实现，与pthread的实现语义完全一致。使用条件变量做并发控制时，某一时刻阻塞在一个条件变量上的各个线程应该在调用wait操作时指明同一个互斥锁，此时该条件变量与该互斥锁绑定；否则程序的行为未定义。条件变量必须与互斥锁配合使用，其理由是程序需要判定某个条件(condition或称predict)是否成立，该条件可以是任意复杂。
离开临界区的线程用notify操作解除阻塞(unblock)在条件变量上的各个线程时，按照公平性(fairness)这些线程应该有平等的获得互斥锁的机会，不应让某个线程始终难以获得互斥锁被饿死(starvation)，并且比后来到临界区的其它线程更为优先(即基本上FIFO)。一种办法是调用了notify_all的线程保持互斥锁，直到所有从条件变量上解除阻塞的线程都已经挂起(suspend)到互斥锁上，然后发起了notify_all的线程再释放互斥锁。互斥锁上一般都有比较完善的阻塞线程调度算法，一般会按照线程优先级调度，相同优先级按照FIFO调度。
发起notify的线程不需要拥有互斥锁。即将离开临界区的线程是先释放互斥锁还是先notify操作解除在条件变量上挂起线程的阻塞？表面看两种顺序都可以。但一般建议是先notify操作，后对互斥锁解锁。因为这既有利于上述的公平性，同时还避免了相反顺序时可能的优先级倒置。这种先notify后解锁的做法是悲观的(pessimization)，因为被通知(notified)线程将立即被阻塞，等待通知(notifying)线程释放互斥锁。很多实现(特别是pthreads的很多实现)为了避免这种”匆忙与等待”(hurry up and wait)情形，把在条件变量的线程队列上处于等待的被通知线程直接移到互斥锁的线程队列上，而不唤醒这些线程。
C++11中引入了条件变量，其相关内容均在<condition_variable>中。这里主要介绍std::condition_variable类。
条件变量std::condition_variable用于多线程之间的通信，它可以阻塞一个或同时阻塞多个线程。std::condition_variable需要与std::unique_lock配合使用。std::condition_variable效果上相当于包装了pthread库中的pthread_cond_*()系列的函数。
当std::condition_variable对象的某个wait函数被调用的时候，它使用std::unique_lock(通过std::mutex)来锁住当前线程。当前线程会一直被阻塞，直到另外一个线程在相同的std::condition_variable对象上调用了notification函数来唤醒当前线程。
std::condition_variable对象通常使用std::unique_lock<std::mutex>来等待，如果需要使用另外的lockable类型，可以使用std::condition_variable_any类。
std::condition_variable类的成员函数：
(1)、构造函数：仅支持默认构造函数，拷贝、赋值和移动(move)均是被禁用的。
(2)、wait：当前线程调用wait()后将被阻塞，直到另外某个线程调用notify_*唤醒当前线程；当线程被阻塞时，该函数会自动调用std::mutex的unlock()释放锁，使得其它被阻塞在锁竞争上的线程得以继续执行。一旦当前线程获得通知(notify，通常是另外某个线程调用notify_*唤醒了当前线程)，wait()函数也是自动调用std::mutex的lock()。wait分为无条件被阻塞和带条件的被阻塞两种。
无条件被阻塞：调用该函数前，当前线程应该已经对unique_lock<mutex> lck完成了加锁。所有使用同一个条件变量的线程必须在wait函数中使用同一个unique_lock<mutex>。该wait函数内部会自动调用lck.unlock()对互斥锁解锁，使得其他被阻塞在互斥锁上的线程恢复执行。使用本函数被阻塞的当前线程在获得通知(notified，通过别的线程调用 notify_*系列的函数)而被唤醒后，wait()函数恢复执行并自动调用lck.lock()对互斥锁加锁。
带条件的被阻塞：wait函数设置了谓词(Predicate)，只有当pred条件为false时调用该wait函数才会阻塞当前线程，并且在收到其它线程的通知后只有当pred为true时才会被解除阻塞。因此，等效于while (!pred())  wait(lck).
(3)、wait_for：与wait()类似，只是wait_for可以指定一个时间段，在当前线程收到通知或者指定的时间超时之前，该线程都会处于阻塞状态。而一旦超时或者收到了其它线程的通知，wait_for返回，剩下的步骤和wait类似。
(4)、wait_until：与wait_for类似，只是wait_until可以指定一个时间点，在当前线程收到通知或者指定的时间点超时之前，该线程都会处于阻塞状态。而一旦超时或者收到了其它线程的通知，wait_until返回，剩下的处理步骤和wait类似。
(5)、notify_all: 唤醒所有的wait线程，如果当前没有等待线程，则该函数什么也不做。
(6)、notify_one：唤醒某个wait线程，如果当前没有等待线程，则该函数什么也不做；如果同时存在多个等待线程，则唤醒某个线程是不确定的(unspecified)。
条件变化存在虚假唤醒的情况，因此在线程被唤醒后需要检查条件是否满足。无论是notify_one或notify_all都是类似于发出脉冲信号，如果对wait的调用发生在notify之后是不会被唤醒的，所以接收者在使用wait等待之前也需要检查条件是否满足。
std::condition_variable_any类与std::condition_variable用法一样，区别仅在于std::condition_variable_any的wait函数可以接受任何lockable参数，而std::condition_variable只能接受std::unique_lock<std::mutex>类型的参数。
std::notify_all_at_thread_exit函数：当调用该函数的线程退出时，所有在cond条件变量上等待的线程都会收到通知。
std::condition_variable:A condition variable is an object able to block the calling thread until notified to resume. It uses a unique_lock (over a mutex) to lock the thread when one of its wait functions is called. The thread remains blocked until woken up by another thread that calls a notification function on the same condition_variable object. Objects of type condition_variable always use unique_lock<mutex> to wait: for an alternative that works with any kind of lockable type, see condition_variable_any.
The condition_variable class is a synchronization primitive that can be used to block a thread, or multiple threads at the same time, until another thread both modifies a shared variable (the condition), and notifies the condition_variable.
The thread that intends to modify the variable has to：(1)、acquire a std::mutex (typically via std::lock_guard)；(2)、perform the modification while the lock is held；(3)、execute notify_one or notify_all on the std::condition_variable (the lock does not need to be held for notification).
Any thread that intends to wait on std::condition_variable has to：(1)、acquire a std::unique_lock<std::mutex>, on the same mutex as used to protect the shared variable；(2)、execute wait, wait_for, or wait_until. The wait operations atomically release the mutex and suspend the execution of the thread；(3)、When the condition variable is notified, a timeout expires, or a spurious wake up occurs,the thread is awakened, and the mutex is atomically reacquired. The thread should then check the condition and resume waiting if the wake up was spurious.
std::condition_variable works only with std::unique_lock<std::mutex>; this restriction allows for maximal efficiency on some platforms. std::condition_variable_any provides a condition variable that works with any BasicLockable object, such as std::shared_lock.
下面是从其它文章中copy的std::condition_variable测试代码，详细内容介绍可以参考对应的reference：
```cpp
```
- 
#include"condition_variable.hpp"
- 
#include<iostream>
- 
#include<chrono>
- 
#include<thread>
- 
#include<mutex>
- 
#include<condition_variable>
- 
#include<string>
- 
- 
namespace condition_variable_ {
- 
- 
//////////////////////////////////////////////////////////////////////
- 
// reference: http://www.cplusplus.com/reference/condition_variable/condition_variable/
- 
std::mutex mtx;
- 
std::condition_variable cv;
- 
bool ready = false;
- 
- 
staticvoidprint_id(int id)
- 
{
- 
std::unique_lock<std::mutex> lck(mtx);
- 
while (!ready) cv.wait(lck);
- 
// ...
- 
std::cout << "thread " << id << '\n';
- 
}
- 
- 
staticvoidgo()
- 
{
- 
std::unique_lock<std::mutex> lck(mtx);
- 
	ready = true;
- 
	cv.notify_all();
- 
}
- 
- 
inttest_condition_variable_1()
- 
{
- 
std::thread threads[10];
- 
// spawn 10 threads:
- 
for (int i = 0; i<10; ++i)
- 
		threads[i] = std::thread(print_id, i);
- 
- 
std::cout << "10 threads ready to race...\n";
- 
	go();                       // go!
- 
- 
for (auto& th : threads) th.join();
- 
- 
return0;
- 
}
- 
- 
/////////////////////////////////////////////////////////////////////////
- 
// reference: http://www.cplusplus.com/reference/condition_variable/condition_variable/wait/
- 
// condition_variable::wait: Wait until notified,
- 
// The execution of the current thread (which shall have locked lck's mutex) is blocked until notified.
- 
// At the moment of blocking the thread, the function automatically calls lck.unlock(), allowing other locked threads to continue.
- 
// If pred is specified, the function only blocks if pred returns false,
- 
// and notifications can only unblock the thread when it becomes true (which is specially useful to check against spurious wake-up calls).
- 
std::mutex mtx2;
- 
std::condition_variable cv2;
- 
- 
int cargo = 0;
- 
staticboolshipment_available(){ return cargo != 0; }
- 
- 
staticvoidconsume(int n)
- 
{
- 
for (int i = 0; i<n; ++i) {
- 
std::unique_lock<std::mutex> lck(mtx2);
- 
		cv2.wait(lck, shipment_available);
- 
// consume:
- 
std::cout << cargo << '\n';
- 
		cargo = 0;
- 
std::cout << "****: " << cargo << std::endl;
- 
	}
- 
}
- 
- 
inttest_condition_variable_wait()
- 
{
- 
std::thread consumer_thread(consume, 10);
- 
- 
// produce 10 items when needed:
- 
for (int i = 0; i<10; ++i) {
- 
while (shipment_available()) std::this_thread::yield();
- 
std::unique_lock<std::mutex> lck(mtx2);
- 
		cargo = i + 1;
- 
		cv2.notify_one();
- 
	}
- 
- 
	consumer_thread.join();
- 
- 
return0;
- 
}
- 
- 
///////////////////////////////////////////////////////////////////////////
- 
// reference: http://www.cplusplus.com/reference/condition_variable/condition_variable/wait_for/
- 
// condition_variable::wait_for: Wait for timeout or until notified
- 
// The execution of the current thread (which shall have locked lck's mutex) is blocked during rel_time,
- 
// or until notified (if the latter happens first).
- 
// At the moment of blocking the thread, the function automatically calls lck.unlock(),
- 
// allowing other locked threads to continue.
- 
std::condition_variable cv3;
- 
int value;
- 
- 
staticvoidread_value()
- 
{
- 
std::cin >> value;
- 
	cv3.notify_one();
- 
}
- 
- 
inttest_condition_variable_wait_for()
- 
{
- 
std::cout << "Please, enter an integer (I'll be printing dots): \n";
- 
std::thread th(read_value);
- 
- 
std::mutex mtx;
- 
std::unique_lock<std::mutex> lck(mtx);
- 
while (cv3.wait_for(lck, std::chrono::seconds(1)) == std::cv_status::timeout) {
- 
std::cout << '.' << std::endl;
- 
	}
- 
std::cout << "You entered: " << value << '\n';
- 
- 
	th.join();
- 
- 
return0;
- 
}
- 
- 
//////////////////////////////////////////////////////////////////
- 
// reference: http://www.cplusplus.com/reference/condition_variable/condition_variable/notify_one/
- 
// condition_variable::notify_one: Notify one, Unblocks one of the threads currently waiting for this condition.
- 
// If no threads are waiting, the function does nothing.
- 
// If more than one, it is unspecified which of the threads is selected.
- 
std::mutex mtx4;
- 
std::condition_variable produce4, consume4;
- 
- 
int cargo4 = 0;     // shared value by producers and consumers
- 
- 
staticvoidconsumer4()
- 
{
- 
std::unique_lock<std::mutex> lck(mtx4);
- 
while (cargo4 == 0) consume4.wait(lck);
- 
std::cout << cargo4 << '\n';
- 
	cargo4 = 0;
- 
	produce4.notify_one();
- 
}
- 
- 
staticvoidproducer(int id)
- 
{
- 
std::unique_lock<std::mutex> lck(mtx4);
- 
while (cargo4 != 0) produce4.wait(lck);
- 
	cargo4 = id;
- 
	consume4.notify_one();
- 
}
- 
- 
inttest_condition_variable_notify_one()
- 
{
- 
std::thread consumers[10], producers[10];
- 
// spawn 10 consumers and 10 producers:
- 
for (int i = 0; i<10; ++i) {
- 
		consumers[i] = std::thread(consumer4);
- 
		producers[i] = std::thread(producer, i + 1);
- 
	}
- 
- 
// join them back:
- 
for (int i = 0; i<10; ++i) {
- 
		producers[i].join();
- 
		consumers[i].join();
- 
	}
- 
- 
return0;
- 
}
- 
- 
/////////////////////////////////////////////////////////////
- 
// reference: http://www.cplusplus.com/reference/condition_variable/condition_variable/notify_all/
- 
// condition_variable::notify_all: Notify all, Unblocks all threads currently waiting for this condition.
- 
// If no threads are waiting, the function does nothing.
- 
std::mutex mtx5;
- 
std::condition_variable cv5;
- 
bool ready5 = false;
- 
- 
staticvoidprint_id5(int id){
- 
std::unique_lock<std::mutex> lck(mtx5);
- 
while (!ready5) cv5.wait(lck);
- 
// ...
- 
std::cout << "thread " << id << '\n';
- 
}
- 
- 
staticvoidgo5()
- 
{
- 
std::unique_lock<std::mutex> lck(mtx5);
- 
	ready5 = true;
- 
	cv5.notify_all();
- 
}
- 
- 
inttest_condition_variable_notify_all()
- 
{
- 
std::thread threads[10];
- 
// spawn 10 threads:
- 
for (int i = 0; i<10; ++i)
- 
		threads[i] = std::thread(print_id5, i);
- 
- 
std::cout << "10 threads ready to race...\n";
- 
	go5();                       // go!
- 
- 
for (auto& th : threads) th.join();
- 
- 
return0;
- 
}
- 
- 
////////////////////////////////////////////////////////////
- 
// reference: http://en.cppreference.com/w/cpp/thread/condition_variable
- 
std::mutex m;
- 
std::condition_variable cv6;
- 
std::string data;
- 
bool ready6 = false;
- 
bool processed = false;
- 
- 
staticvoidworker_thread()
- 
{
- 
// Wait until main() sends data
- 
std::unique_lock<std::mutex> lk(m);
- 
	cv6.wait(lk, []{return ready6; });
- 
- 
// after the wait, we own the lock.
- 
std::cout << "Worker thread is processing data\n";
- 
	data += " after processing";
- 
- 
// Send data back to main()
- 
	processed = true;
- 
std::cout << "Worker thread signals data processing completed\n";
- 
- 
// Manual unlocking is done before notifying, to avoid waking up
- 
// the waiting thread only to block again (see notify_one for details)
- 
	lk.unlock();
- 
	cv6.notify_one();
- 
}
- 
- 
inttest_condition_variable_2()
- 
{
- 
std::thread worker(worker_thread);
- 
- 
	data = "Example data";
- 
// send data to the worker thread
- 
	{
- 
std::lock_guard<std::mutex> lk(m);
- 
		ready6 = true;
- 
std::cout << "main() signals data ready for processing\n";
- 
	}
- 
	cv6.notify_one();
- 
- 
// wait for the worker
- 
	{
- 
std::unique_lock<std::mutex> lk(m);
- 
		cv6.wait(lk, []{return processed; });
- 
	}
- 
std::cout << "Back in main(), data = " << data << '\n';
- 
- 
	worker.join();
- 
- 
return0;
- 
}
- 
- 
} // namespace condition_variable_
GitHub：[https://github.com/fengbingchun/Messy_Test](https://github.com/fengbingchun/Messy_Test)
