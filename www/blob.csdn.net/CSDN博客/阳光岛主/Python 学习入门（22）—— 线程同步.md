
# Python 学习入门（22）—— 线程同步 - 阳光岛主 - CSDN博客

2013年12月10日 23:16:07[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3842所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



Python主要通过标准库中的threading包来实现多线程。在当今网络时代，每个服务器都会接收到大量的请求。服务器可以利用多线程的方式来处理这些请求，以提高对网络端口的读写效率。Python是一种网络服务器的后台工作语言 (比如豆瓣网)，所以多线程也就很自然被Python语言支持。
**1. 多线程售票以及同步**
我们使用Python来实现[Linux多线程与同步](http://www.cnblogs.com/vamei/archive/2012/10/09/2715393.html)文中的售票程序。我们使用mutex (也就是Python中的Lock类对象) 来实现线程的同步:

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
import threading
import time
import os
# This function could be any function to do other chores.
def doChore():
    time.sleep(0.5)
# Function for each thread
def booth(tid):
    print("tid = " + str(tid))
    global i
    global lock
    while True:
        lock.acquire()                  # Lock; or wait if other thread is holding the lock
        if i != 0:
            i = i - 1                   # Sell tickets
            print(threading.current_thread().getName(),tid,':now left:',i)   # Tickets left
            doChore()                   # Other critical operations
        else:
            print("Thread_id",tid," No more tickets")
            os._exit(0)                 # Exit the whole process immediately
        lock.release()                  # Unblock
        print(threading.current_thread().getName() + ": release")
        doChore()                       # Non-critical operations
# Start of the main function
i = 15                                  # Available ticket number 
lock = threading.Lock()                 # Lock (i.e., mutex)
# 同时启动10个线程
for k in range(10):
    t = threading.Thread(target=booth, args=(k,))      # Set up thread; target: the callable (function) to be run, args: the argument for the callable 
    t.start()                                          # run the thread
```
运行结果：

```python
tid = 0
('Thread-1', 0, ':now left:', 14)
tid = 1
 tid = 2
tid = 3
tid = 4
 tid = 5
tid = 6
tid = 7
tid = 8
tid = 9
Thread-1: release
('Thread-3', 2, ':now left:', 13)
Thread-3: release('Thread-2', 1, ':now left:', 12)
Thread-2: release('Thread-4', 3, ':now left:', 11)
Thread-4: release
('Thread-6', 5, ':now left:', 10)
Thread-6: release
('Thread-5', 4, ':now left:', 9)
Thread-5: release
 ('Thread-7', 6, ':now left:', 8)
Thread-7: release
('Thread-8', 7, ':now left:', 7)
Thread-8: release
('Thread-9', 8, ':now left:', 6)
Thread-9: release
('Thread-10', 9, ':now left:', 5)
Thread-10: release
('Thread-1', 0, ':now left:', 4)
Thread-1: release('Thread-3', 2, ':now left:', 3)
Thread-3: release
 ('Thread-2', 1, ':now left:', 2)
Thread-2: release
('Thread-4', 3, ':now left:', 1)
Thread-4: release
('Thread-6', 5, ':now left:', 0)
Thread-6: release('Thread_id', 4, ' No more tickets')
```
我们使用了两个全局变量，一个是i，用以储存剩余票数；一个是lock对象，用于同步线程对i的修改。此外，在最后的for循环中，我们总共设置了10个线程。每个线程都执行booth()函数。线程在调用start()方法的时候正式启动 (实际上，计算机中最多会有11个线程，因为主程序本身也会占用一个线程)。Python使用threading.Thread对象来代表线程，用threading.Lock对象来代表一个互斥锁 (mutex)。
有两点需要注意:
我们在函数中使用global来声明变量为全局变量，从而让多线程共享i和lock (在C语言中，我们通过将变量放在所有函数外面来让它成为全局变量)。如果不这么声明，由于i和lock是不可变数据对象，它们将被当作一个局部变量(参看[Python动态类型](http://www.cnblogs.com/vamei/archive/2012/07/10/2582795.html))。如果是可变数据对象的话，则不需要global声明。我们甚至可以将可变数据对象作为参数来传递给线程函数。这些线程将共享这些可变数据对象。
我们在booth中使用了两个doChore()函数。可以在未来改进程序，以便让线程除了进行i=i-1之外，做更多的操作，比如打印剩余票数，找钱，或者喝口水之类的。第一个doChore()依然在Lock内部，所以可以安全地使用共享资源(critical operations, 比如打印剩余票数)。第二个doChore()时，Lock已经被释放，所以不能再去使用共享资源。这时候可以做一些不使用共享资源的操作 (non-critical operation, 比如找钱、喝水)。我故意让doChore()等待了0.5秒，以代表这些额外的操作可能花费的时间。你可以定义的函数来代替doChore()。

**2. OOP 创建线程**
上面的Python程序非常类似于一个面向过程的C程序。我们下面介绍如何通过面向对象(OOP， object-oriented programming，参看[Python面向对象的基本概念](http://www.cnblogs.com/vamei/archive/2012/06/02/2531515.html)和[Python面向对象的进一步拓展](http://www.cnblogs.com/vamei/archive/2012/06/02/2532018.html)) 的方法实现多线程，其核心是继承threading.Thread类。我们上面的for循环中已经利用了threading.Thread()的方法来创建一个Thread对象，并将函数booth()以及其参数传递给改对象，并调用start()方法来运行线程。OOP的话，通过修改Thread类的run()方法来定义线程所要执行的命令。

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
import threading
import time
import os
# This function could be any function to do other chores.
def doChore():
    time.sleep(0.5)
# Function for each thread
class BoothThread(threading.Thread):
    def __init__(self, tid, monitor):
        self.tid = tid
        self.monitor = monitor
        threading.Thread.__init__(self)
    def run(self):
        while True:
            monitor['lock'].acquire()                          # Lock; or wait if other thread is holding the lock
            if monitor['tick'] != 0:
                monitor['tick'] = monitor['tick'] - 1          # Sell tickets
                print(threading.current_thread().getName(),self.tid,':now left:',monitor['tick'])   # Tickets left
                doChore()                                      # Other critical operations
            else:
                print("Thread_id",self.tid," No more tickets")
                os._exit(0)                                    # Exit the whole process immediately
            monitor['lock'].release()                          # Unblock
            print(threading.current_thread().getName() + ": release")  
            doChore()                                          # Non-critical operations
# Start of the main function
monitor = {'tick':15, 'lock':threading.Lock()}
# 启动10个线程
for k in range(10):
    t = BoothThread(k, monitor)
    t.start()
```
运行结果：

```python
('Thread-1', 0, ':now left:', 14)
Thread-1: release
('Thread-2', 1, ':now left:', 13)
Thread-2: release
('Thread-3', 2, ':now left:', 12)
Thread-3: release
('Thread-4', 3, ':now left:', 11)
Thread-4: release
('Thread-5', 4, ':now left:', 10)
Thread-5: release('Thread-6', 5, ':now left:', 9)
Thread-6: release('Thread-7', 6, ':now left:', 8)
Thread-7: release('Thread-8', 7, ':now left:', 7)
Thread-8: release
('Thread-9', 8, ':now left:', 6)
Thread-9: release
('Thread-10', 9, ':now left:', 5)
Thread-10: release
('Thread-1', 0, ':now left:', 4)
Thread-1: release('Thread-2', 1, ':now left:', 3)
Thread-2: release
('Thread-3', 2, ':now left:', 2)
Thread-3: release
('Thread-4', 3, ':now left:', 1)
Thread-4: release('Thread-5', 4, ':now left:', 0)
Thread-5: release
('Thread_id', 5, ' No more tickets')
```

我们自己定义了一个类BoothThread, 这个类继承自thread.Threading类。然后我们把上面的booth()所进行的操作统统放入到BoothThread类的run()方法中。注意，我们没有使用全局变量声明global，而是使用了一个词典monitor存放全局变量，然后把词典作为参数传递给线程函数。由于词典是可变数据对象，所以当它被传递给函数的时候，函数所使用的依然是同一个对象，相当于被多个线程所共享。这也是多线程乃至于多进程编程的一个技巧 (应尽量避免上面的global声明的用法，因为它并不适用于windows平台)。
上面OOP编程方法与面向过程的编程方法相比，并没有带来太大实质性的差别。

**3. 其他**
threading.Thread对象：我们已经介绍了该对象的start()和run(), 此外：
join()方法，调用该方法的线程将等待直到该Thread对象完成，再恢复运行。这与进程间调用wait()函数相类似。
下面的对象用于处理多线程同步。对象一旦被建立，可以被多个线程共享，并根据情况阻塞某些进程。请与Linux多线程与同步中的同步工具参照阅读。
threading.Lock对象: mutex, 有acquire()和release()方法。
threading.Condition对象: condition variable，建立该对象时，会包含一个Lock对象 (因为condition variable总是和mutex一起使用)。可以对Condition对象调用acquire()和release()方法，以控制潜在的Lock对象。此外:
wait()方法，相当于cond_wait()
notify_all()，相当与cond_broadcast()
nofify()，与notify_all()功能类似，但只唤醒一个等待的线程，而不是全部
threading.Semaphore对象: semaphore，也就是计数锁(semaphore传统意义上是一种进程间同步工具，见Linux进程间通信)。创建对象的时候，可以传递一个整数作为计数上限(sema = threading.Semaphore(5))。它与Lock类似，也有Lock的两个方法。threading.Event对象: 与threading.Condition相类似，相当于没有潜在的Lock保护的condition variable。对象有True和False两个状态。可以多个线程使用wait()等待，直到某个线程调用该对象的set()方法，将对象设置为True。线程可以调用对象的clear()方法来重置对象为False状态。

**4. 练习**参照[Linux多线程与同步](http://www.cnblogs.com/vamei/archive/2012/10/09/2715393.html)中的condition variable的例子，使用Python实现。同时考虑使用面向过程和面向对象的编程方法。更多的threading的内容请参考：[threading](http://docs.python.org/library/threading.html)

**总结:**
threading.Thread
Lock, Condition, Semaphore, Event


**博客之星评选，请投我一票：**
**http://vote.blog.csdn.net/blogstaritem/blogstar2013/sunboy_2050**


