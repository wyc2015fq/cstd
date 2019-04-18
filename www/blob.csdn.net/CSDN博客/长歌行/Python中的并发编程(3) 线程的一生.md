# Python中的并发编程(3): 线程的一生 - 长歌行 - CSDN博客





2018年03月28日 21:54:30[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：190
所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









## Python中的线程

先来看看线程的构造函数：

```
class threading.Thread(group=None, target=None, name=None, args=(), kwargs={}, *, daemon=None)
```

这些参数的意思是：
- group的值设置为None即可，是为了以后线程组扩展用的
- target需要传递一个可被run()调用的对象，默认值为None
- name是线程的名字，默认情况下为”Thread-N”, N为十进制序号
- args是传递给target的参数元组，默认是()
- kwargs是传递给target调用的关键词字典菜蔬，默认是{}
- daemon若不是None的话，该参数会显示的指明该线程是否为守护线程，如果是None的话，该属性由当前线程继承

注意的是，如果派生类重载了构造函数，需要先调用下基类的构造函数(Thread._init_())

其他方法引用如下，就不一一中文介绍，在后面使用到的时候，再以示例详解：

> 
start() 

  Start the thread’s activity.

It must be called at most once per thread object. It arranges for the object’s run() method to be invoked in a separate thread of control.

This method will raise a RuntimeError if called more than once on the same thread object.


> 
run() 

  Method representing the thread’s activity. 

  You may override this method in a subclass. The standard run() method invokes the callable object passed to the object’s constructor as the target argument, if any, with sequential and keyword arguments taken from the args and kwargs arguments, respectively.


> 
join(timeout=None) 

  Wait until the thread terminates. This blocks the calling thread until the thread whose join() method is called terminates – either normally or through an unhandled exception – or until the optional timeout occurs.

When the timeout argument is present and not None, it should be a floating point number specifying a timeout for the operation in seconds (or fractions thereof). As join() always returns None, you must call isalive() after join() to decide whether a timeout happened – if the thread is still alive, the join() call timed out.

When the timeout argument is not present or None, the operation will block until the thread terminates.

A thread can be join()ed many times.

join() raises a RuntimeError if an attempt is made to join the current thread as that would cause a deadlock. It is also an error to join() a thread before it has been started and attempts to do so raise the same exception. 


> 
name 

  A string used for identification purposes only. It has no semantics. Multiple threads may be given the same name. The initial name is set by the constructor.

getName() 

  setName() 

  Old getter/setter API for name; use it directly as a property instead.


> 
ident 

  The ‘thread identifier’ of this thread or None if the thread has not been started. This is a nonzero integer. See the getident() function. Thread identifiers may be recycled when a thread exits and another thread is created. The identifier is available even after the thread has exited.


> 
isalive() 

  Return whether the thread is alive.

This method returns True just before the run() method starts until just after the run() method terminates. The module function enumerate() returns a list of all alive threads.


> 
daemon 

  A boolean value indicating whether this thread is a daemon thread (True) or not (False). This must be set before start() is called, otherwise RuntimeError is raised. Its initial value is inherited from the creating thread; the main thread is not a daemon thread and therefore all threads created in the main thread default to daemon = False.

The entire Python program exits when no alive non-daemon threads are left.

isDaemon() 

  setDaemon() 

  Old getter/setter API for daemon; use it directly as a property instead.


## 线程的状态

大体来讲可以分为以下几种：

### New Thread

创建阶段：这个时候，线程刚被创建，没有分配任何资源，只是一个对象的实例。

```
myThread = threading.Thread(target=myFunc)
```

### Runnable

可运行阶段：此时，线程正在等待运行，所有资源就位，只需要把它排上任务安排中即可。如果我们调用Start()，此时系统就会给线程分配资源，并进入Runnable状态。

```
myThread.start()
```

### Running

运行阶段：开始执行target的时候，线程状态就由可运行状态变成运行状态。

### Not-running

非运行状态：如果任务阻塞，线程暂停，此时就进入了非运行状态。

```
time.sleep(10)
```

### Dead

消亡状态：线程运行结束（target函数执行完毕或者超时），即进入消亡状态。我们也可以使用join()来等待线程结束。

```
myThread.join()
```

### 运行周期图

运行周期如下图所示： 
![img](http://or45huni1.bkt.clouddn.com/18-3-28/42275358.jpg)

代码示例如下：

```
import threading
import time

# A very simple method for our thread to execute
def threadWorker():
  # it is only at the point where the thread starts executing
  # that it's state goes from 'Runnable' to a 'Running'
  # state
  print("My Thread has entered the 'Running' State")

  # If we call the time.sleep() method then our thread
  # goes into a not-runnable state. We can do no further work
  # on this particular thread
  time.sleep(10)
  # Thread then completes its tasks and terminates
  print("My Thread is terminating")

# At this point in time, the thread has no state
# it hasn't been allocated any system resources
myThread = threading.Thread(target=threadWorker)

# When we call myThread.start(), Python allocates the necessary system 
# resources in order for our thread to run and then calls the thread's
# run method. It goes from 'Starting' state to 'Runnable' but not running 
myThread.start()

# Here we join the thread and when this method is called
# our thread goes into a 'Dead' state. It has finished the
# job that it was intended to do.
myThread.join()
print("My Thead has entered a 'Dead' state")
```

## 多个线程的识别

对于多个线程，我们也有一系列函数可以进行识别，管理。 

-   使用os.getpid()可以获取当前线程id 

-   threading.currentThread()返回当前线程变量 

-   使用threading.enumerate()可以枚举当前活跃线程列表。

## 守护线程

可以根据构造函数，或者setDaemon()来设置守护线程。在主线程退出的时候，不需要等待守护线程完成，守护线程会自动退出。所以守护线程可以用于发送主线程的心跳。

```
import threading 
import time 


def standardThread(): 
    print("Starting my Standard Thread") 
    time.sleep(20) 
    print("Ending my standard thread") 


def daemonThread(): 
    while True: 
        print("Sending Out Heartbeat Signal") 
        time.sleep(2) 


if __name__ == '__main__': 
    standardThread = threading.Thread(target=standardThread) 
    daemonThread = threading.Thread(target=daemonThread) 
    daemonThread.setDaemon(True) 
    daemonThread.start() 
    standardThread.start()
```

## 线程种类

其实有c++编程基础的同学对这个应该很熟悉了，就在此做一个简单介绍吧。在应用层面主要的差别是跨平台方面。

### POSIX线程

> 
POSIX线程（POSIX threads），简称Pthreads，是线程的POSIX标准。该标准定义了创建和操纵线程的一整套API。在类Unix操作系统（Unix、Linux、Mac OS X等）中，都使用Pthreads作为操作系统的线程。Windows操作系统也有其移植版pthreads-win32。


我们也可以简称为pThread

### Win32线程

> 
Win32线程是Windows API的一部分，上下文包括：寄存器、核心栈、线程环境块和用户栈。


## 引用
- 
More details from [here](https://docs.python.org/3/library/threading.html).

- 
代码原地址[点击](https://github.com/elliotforbes/Concurrency-With-Python/blob/master/Chapter%252003/threadLifecycle.py)




