# python 线程 进程 协程_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
**知识点：线程，进程，协程，threading.local()，**
进程是具有一定独立功能的程序关于某个数据集合上的一次运行活动,进程是系统进行资源分配和调度的一个独立单位.
线程是进程的一个实体,是CPU调度和分派的基本单位,它是比进程更小的能独立运行的基本单位.线程自己基本上不拥有系统资源,只拥有一点在运行中必不可少的资源(如程序计数器,一组寄存器和栈),但是它可与同属一个进程的其他的线程共享进程所拥有的全部资源.
一个线程可以创建和撤销另一个线程;同一个进程中的多个线程之间可以并发执行.
**进程和线程的主要差别在于它们是不同的操作系统资源管理方式。进程有独立的地址空间，一个进程崩溃后，在保护模式下不会对其它进程产生影响，而线程只是一个进程中的不同执行路径。线程有自己的堆栈和局部变量，但线程之间没有单独的地址空间，一个线程死掉就等于整个进程死掉，所以多进程的程序要比多线程的程序健壮，但在进程切换时，耗费资源较大，效率要差一些。但对于一些要求同时进行并且又要共享某些变量的并发操作，只能用线程，不能用进程。如果有兴趣深入的话，我建议你们看看《现代操作系统》或者《操作系统的设计与实现》。对就个问题说得比较清楚。**
多进程：
根据给定的接口创建进程，执行子进程代码。然后就是进程之间的通信，首先想到的是
通信模式分类，一个读一个写，队列形式的，等，但要求同一时刻只有一个进行可以进行操作，
来避免数据混乱。可以使用专门给定的数据结构来实现这些操作。
多线程：
Python的标准库提供了两个模块：thread和threading，thread是低级模块，
threading是高级模块，对thread进行了封装。多线程执行lock.acquire()，lock对象
实际上代表了变量的处理权限，令牌，但是lock设置不当，有可能导致死锁。
Python的线程虽然是真正的线程，但解释器执行代码时，有一个GIL锁：Global Interpreter
Lock，
任何Python线程执行前，必须先获得GIL锁，然后，每执行100条字节码，解释器就自动释放GIL锁，
让别的线程有机会执行。这个GIL全局锁实际上把所有线程的执行代码都给上了锁，所以，
多线程在Python中只能交替执行，即使100个线程跑在100核CPU上，也只能用到1个核。
GIL是Python解释器设计的历史遗留问题，通常我们用的解释器是官方实现的CPython，
要真正利用多核，除非重写一个不带GIL的解释器。所以，在Python中，可以使用多线程，
但不要指望能有效利用多核。如果一定要通过多线程利用多核，那只能通过C扩展来实现，
不过这样就失去了Python简单易用的特点。不过，也不用过于担心，Python虽然不能利用多线程实现多核任务，
但可以通过多进程实现多核任务。多个Python进程有各自独立的GIL锁，互不影响。
针对IO操作的多线程是有意义的，如果是CPU密集的多线程是没有意义的
多进程对全局变量时不共享的，多线程对全局变量是共享的。但是为了向进程间拥有各自独立的，全局变量。
可以将定义一个全局变量ThreadLocalHelper =
threading.local()，然后将变量保存到ThreadLocalHelper里面，这样就相当于访问全局变量，在ThreadLocalHelper里面，各个线程之间变量的名字是不冲突的。相当于ThreadLocalHelper对变量做了些存和取的操作，绑定了线程id信息。
分布式进程：如下例子，使用网络Queue进行通信。也叫异步。
协程和第三方库gevent：协程就是一种"用户态"内的上下文切换技术，进程线程都是系统态切换的，子程序就是协程的一种特例。
协程的特点在于是一个线程执行。第一，不需要切换上下文。第二，第二大优势就是不需要多线程的锁机制，
因为只有一个线程，也不存在同时写变量冲突，在协程中控制共享资源不加锁。当一个greenlet遇到IO操作时，
比如访问网络，就自动切换到其他的greenlet，等到IO操作完成，再在适当的时候切换回来继续执行。
由于IO操作非常耗时，经常使程序处于等待状态，有了gevent为我们自动切换协程，就保证总有greenlet在运行，而不是等待IO。
由于切换是在IO操作时自动完成，所以gevent需要修改Python自带的一些标准库，这一过程在启动时通过monkey
patch完成。
因为协程是一个线程执行，那怎么利用多核CPU呢？最简单的方法是多进程+协程，既充分利用多核，
又充分发挥协程的高效率，可获得极高的性能。
在单个线程中执行的协程，可以视为单线程应用。这些协程，在未执行到特定位置(基本就是阻塞操作)前，
是不会被抢占，也不会和其他CPU上的上下文发生同步问题的。因此，一段协程代码，中间没有可能导致阻塞的调用，
执行在单个线程中。那么这段内容可以被视为同步的。
再深入一点就涉及到系统设计模式之类的问题，事件通知机制，epoll，CPS回调等问题，
yield参见"迭代器和生成器和yield"
### 进程间通信
```
Process
```
之间肯定是需要通信的，操作系统提供了很多机制来实现进程间的通信。Python的`multiprocessing`模块包装了底层的机制，提供了`Queue`、`Pipes`等多种方式来交换数据。
我们以`Queue`为例，在父进程中创建两个子进程，一个往`Queue`里写数据，一个从`Queue`里读数据：
```
from multiprocessing import Process, Queue
import os, time, random
# 写数据进程执行的代码:
def write(q):
    for value in ['A', 'B', 'C']:
        print 'Put %s to queue...' % value
        q.put(value)
        time.sleep(random.random())
# 读数据进程执行的代码:
def read(q):
    while True:
        value = q.get(True)
        print 'Get %s from queue.' % value
if __name__=='__main__':
    # 父进程创建Queue，并传给各个子进程：
    q = Queue()
    pw = Process(target=write, args=(q,))
    pr = Process(target=read, args=(q,))
    # 启动子进程pw，写入:
    pw.start()
    # 启动子进程pr，读取:
    pr.start()
    # 等待pw结束:
    pw.join()
    # pr进程里是死循环，无法等待其结束，只能强行终止:
    pr.terminate()
```
# taskmanager.py
import random, time, Queue
from multiprocessing.managers import BaseManager
# 发送任务的队列:
task_queue = Queue.Queue()
# 接收结果的队列:
result_queue = Queue.Queue()
# 从BaseManager继承的QueueManager:
class QueueManager(BaseManager):
 pass
# 把两个Queue都注册到网络上, callable参数关联了Queue对象:
QueueManager.register('get_task_queue', callable=lambda:
task_queue)
QueueManager.register('get_result_queue', callable=lambda:
result_queue)
# 绑定端口5000, 设置验证码'abc':
manager = QueueManager(address=('', 5000),
authkey='abc')
# 启动Queue:
manager.start()
# 获得通过网络访问的Queue对象:
task = manager.get_task_queue()
result = manager.get_result_queue()
# 放几个任务进去:
for i in range(10):
 n = random.randint(0,
10000)
 print('Put task %d...' %
n)
 task.put(n)
# 从result队列读取结果:
print('Try get results...')
for i in range(10):
 r =
result.get(timeout=10)
 print('Result: %s' %
r)
# 关闭:
manager.shutdown()

# taskworker.py
import time, sys, Queue
from multiprocessing.managers import BaseManager
# # 创建类似的QueueManager:
# class QueueManager(BaseManager):
#   pass
# 由于这个QueueManager只从网络上获取Queue，所以注册时只提供名字:
QueueManager.register('get_task_queue')
QueueManager.register('get_result_queue')
# 连接到服务器，也就是运行taskmanager.py的机器:
server_addr = '127.0.0.1'
print('Connect to server %s...' % server_addr)
# 端口和验证码注意保持与taskmanager.py设置的完全一致:
m = QueueManager(address=(server_addr, 5000),
authkey='abc')
# 从网络连接:
m.connect()
# 获取Queue的对象:
task = m.get_task_queue()
result = m.get_result_queue()
# 从task队列取任务,并把结果写入result队列:
for i in range(10):
 try:
 n = task.get(timeout=1)
 print('run task %d * %d...' % (n, n))
 r = '%d * %d = %d' % (n, n, n*n)
 time.sleep(1)
 result.put(r)
 except
Queue.Empty:
 print('task queue is empty.')
# 处理结束:
print('worker exit.')
'''
Queue对象存储在哪？注意到taskworker.py中根本没有创建Queue的代码，
所以，Queue对象存储在taskmanager.py进程中：
'''
def foo():
 import threading
 from time import
ctime,sleep
 loops = [4,2]
 def
loop(nloop,nsec):
 print 'start loop',nloop,'at:',ctime()
 sleep(nsec)
 print 'loop',nloop,'done at:',ctime()
 def test1():
 '创建一个Thread的实例，传给它一个函数'
 print 'starting at:',ctime()
 threads = []
 nloops = range(len(loops))
 for i in nloops:
 t =
threading.Thread(target=loop,args=(i,loops[i]))
threads.append(t)
 for i in nloops:
threads[i].start()
 for i in nloops:
threads[i].join()
 print 'all DONE at:',ctime()
 class
ThreadFunc(object):
 def __init__(self,func,args,name=''):
 self.name
= name
 self.func
= func
 self.args
= args
 def __call__(self):
'apply用于当参数存放到一个元组的时候'
apply(self.func,self.args)
 def test2():
 '创建一个Thread的实例，传给它一个可调用的类对象'
 print 'starting at:',ctime()
 threads = []
 nloops = range(len(loops))
 for i in nloops:
 t =
threading.Thread(target=ThreadFunc(loop,(i,loops[i]),loop.__name__))
threads.append(t)
 for i in nloops:
threads[i].start()
 for i in nloops:
threads[i].join()
 print 'all DONE at:',ctime()
 class
MyThread(threading.Thread):
 def __init__(self, func, args, name=''):
threading.Thread.__init__(self)
 self.name
= name
 self.func
= func
 self.args
= args
 def getResult(self):
 return
self.res
 def run(self):
 print
'starting', self.name, 'at:', ctime()
 self.res =
apply(self.func, self.args)
 print
self.name, 'finished at:', ctime()
 def fib(x):
 sleep(0.005)
 if x<2: return 1
 return (fib(x-2) + fib(x-1))
 def fac(x):
 sleep(0.1)
 if x<2: return 1
 return (x*fac(x-1))
 def sum(x):
 sleep(0.1)
 if x<2: return 1
 return (x+sum(x-1))
 def test3():
 '从Thread派生出一个子类，创建一个这个子类的实例'
 funcs = [fib, fac, sum]
 n=12
 nfuncs = range(len(funcs))
 print '*** SINGLE THREAD'
 for i in nfuncs:
 print
'starting', funcs[i].__name__, 'at:', ctime()
 print
funcs[i](n)
 print
funcs[i].__name__, 'finished at:', ctime()
 print '\n*** MULTIPLE THREAD'  
 threads = []
 for i in nfuncs:
 t =
MyThread(funcs[i],(n,),funcs[i].__name__)
threads.append(t)
 for i in nfuncs:
threads[i].start()
 for i in nfuncs:
threads[i].join()
 print
threads[i].getResult()
 print 'all DONE'
 if
__name__=='__main__':
 #   test1()
 #   test2()
 test3()
def bar():
 from time import sleep,
ctime
 import thread
 def
loop(nloop,nsec,lock):
 print 'start loop',nloop,'at:',ctime()
 sleep(nsec)
 print 'loop',nloop,'done at:',ctime()
 lock.release()
 def test3():
 print 'starting at:',ctime()
 locks = []
 loops = [4,2]
 nloops = range(len(loops))
 for i in nloops:
 lock =
thread.allocate_lock()
lock.acquire()
locks.append(lock)
 for i in nloops:
thread.start_new_thread(loop, (i,loops[i],locks[i]))
 for i in nloops:
 while
locks[i].locked(): pass
 print 'all DONE at:',ctime()
 if
__name__=='__main__':
 test3()
def myGevent():
 from gevent import
monkey; monkey.patch_all()
 import gevent
 import urllib2
 def f(url):
 print('GET: %s' % url)
 resp = urllib2.urlopen(url)
 data = resp.read()
 print('%d bytes received from %s.' % (len(data),
url))
 gevent.joinall([
 gevent.spawn(f, 'https://www.yahoo.com/'),
 gevent.spawn(f, 'https://github.com/'),
 gevent.spawn(f, 'https://www.baidu.com/'),
 ]) #
依次返回百度、github和雅虎
