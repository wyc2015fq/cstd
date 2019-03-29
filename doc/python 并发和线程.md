# python 并发和线程



## 并发和线程

### 基本概念 - 并行、并发

**并行**, parallel

​    互不干扰的在同一时刻做多件事;

​    如,同一时刻,同时有多辆车在多条车道上跑,即同时发生的概念.

 

**并发**, concurrency

​    同时做某些事,但是强调同一时段做多件事.

​    如,同一路口,发生了车辆要同时通过路面的事件.

 

**队列,** **缓冲区**

​    类似排队,是一种天然解决并发的办法.排队区域就是缓冲区.

 

**解决并发**:

   【 "食堂打饭模型", 中午12点,大家都涌向食堂,就是并发.人很多就是高并发.】

 

​    1、队列, 缓冲区:

​        队列: 即排队.

​        缓冲区: 排成的队列.

​        优先队列: 如果有男生队伍和女生队伍,女生队伍优先打饭,就是优先队列.

 

​    2、争抢:

​        锁机制: 争抢打饭,有人抢到,该窗口在某一时刻就只能为这个人服务,锁定窗口,即锁机制.

​        争抢也是一种高并发解决方案,但是有可能有人很长时间抢不到,所以不推荐.

 

​    3、预处理:

​        统计大家爱吃的菜品,最爱吃的80%热门菜提前做好,20%冷门菜现做,这样即使有人锁定窗口,也能很快释放.

​        这是一种提前加载用户需要的数据的思路,预处理思想,缓存常用.

 

​    4、并行:

​        开多个打饭窗口,同时提供服务.

​        IT日常可以通过购买更多服务器,或多开线程,进程实现并行处理,解决并发问题.

​        这是一种水平扩展的思路.

​        注: 如果线程在单CPU上处理,就不是并行了.

 

​    5、提速:

​        通过提高单个窗口的打饭速度,也是解决并发的方式.

​        IT方面提高单个CPU性能,或单个服务器安装更多的CPU.   

​        这是一种垂直扩展的思想.

 

​    6、消息中间件:

​        如上地地铁站的九曲回肠的走廊,缓冲人流.

​        常见消息中间件: RabbitMQ, ActiveMQ(Apache), RocketMQ(阿里Apache), kafka(Apache)等.

 

【以上例子说明: 技术源于生活! 】

 

### 进程和线程

a)   在实现了线程的操作系统中,线程是操作系统能够运算调度的最小单位.

b)   线程被包含在进程中,是进程的实际运作单位.

c)   一个程序的执行实例就是一个进程.

 

  进程(Process)是计算机中的程序关于某数据集合上的一次运行活动,是系统进行资源分配和调度的基本单位,是操作系统结构的基础.

​    进程和程序的关系: 进程是线程的容器.

  Linux进程有父进程和子进程之分,windows的进程是平等关系.

​          线程有时称为轻量级进程,一个标准的线程由线程ID,当前指令指针,寄存器集合和堆栈组成.

 

当运行一个程序时,OS会创建一个进程。它会使用系统资源（CPU、内存和磁盘空间）和OS内核中的数据结构（文件、网络连接、用量统计等）。

进程之间是互相隔离的，即一个进程既无法访问其他进程的内容，也无法操作其他进程。

操作系统会跟踪所有正在运行的进程，给每个进程一小段运行时间，然后切换到其他进程，这样既可以做到公平又可以响应用户操作。

可以在图形界面中查看进程状态，在在Windows上可以使用任务管理器。也可以自己编写程序来获取进程信息。

 

获取正在运行的python解释器的进程号和当前工作目录,及用户ID、用户组ID。

```
In [1]: import os
In [2]: os.getpid()
Out[2]: 2550
In [3]: os.getuid()
Out[3]: 0
In [4]: os.getcwd()
Out[4]: '/root'
In [5]: os.getgid()
Out[5]: 0
In [6]:
```

　　

​    对线程、线程的理解:

- 进程是独立的王国,进程间不能随便共享数据.
- 线程是省份,同一进程内的线程可以共享进程的资源,每一个线程有自己独立的堆栈.       

​    线程的状态: 

- 就绪(Ready): 线程一旦运行,就在等待被调度.
- 运行(Running): 线程正在运行.
- 阻塞(Blocked): 线程等待外部事件发生而无法运行,如I/O操作.
- 终止(Terminated): 线程完成或退出,或被取消.

   

python中的进程和线程: 进程会启动一个解释器进程,线程共享一个解释器进程. 

### python的线程开发

python线程开发使用标准库threading.

#### thread类

# 签名

```python
def __init__(self, group=None, target=None, name=None, args=(), kwargs=None, *, daemon=None)
```

　

- target: 线程调用的对象,就是目标函数.
- name: 为线程起个名字.
- args: 为目标函数传递实参, 元组.
- kwargs: 为目标函数关键字传参, 字典.

#### 线程启动

import threading

 

# 最简单的线程程序

```python
def worker():
    print("I'm working")
    print("Finished")
t = threading.Thread(target=worker, name='worker')  # 线程对象.
t.start()
```

**通过threading.Thread****创建一个线程对象**,target是目标函数,name可以指定名称.

但是**线程没有启动****,****需要调用start****方法.**

线程会执行函数,是因为线程中就是执行代码的,而最简单的封装就是函数,所以还是函数调用.

函数执行完,线程也会随之退出.

如果不让线程退出，或者让线程一直工作: 函数内部使用while循环.

```python
import threading
import time
def worker():
    while True:
        time.sleep(1)
        print("I'm work")
    print('Finished')
t = threading.Thread(target=worker, name='worker')  # 线程对象.
t.start()   # 启动.
```

　　

线程退出

python没有提供线程退出的方法,在下面情况时会退出:

- 线程函数内语句执行完毕.
- 线程函数中抛出未处理的异常.

```python
import threading
import time 
def worker():
    count = 0
    while True:
        if (count > 5):
            raise RuntimeError()
            # return
        time.sleep(1)
        print("I'm working")
        count += 1
t = threading.Thread(target=worker, name='worker')  # 线程对象.
t.start()  # 启动.
print("==End==")
```

 

**python****的线程没有优先级,****没有线程组的概念,****也不能被销毁、停止、挂起,****自然也没有恢复、中断.**

 线程的传参

```python
import threading
import time
def add(x, y):
    print('{} + {} = {}'.format(x, y, x + y, threading.current_thread()))
thread1 = threading.Thread(target=add, name='add', args=(4, 5))  # 线程对象.
thread1.start()  # 启动.
time.sleep(2)
thread2 = threading.Thread(target=add, name='add',args=(5, ), kwargs={'y': 4})  # 线程对象.
thread2.start()  # 启动.
time.sleep(2)
thread3 = threading.Thread(target=add, name='add', kwargs={'x': 4, 'y': 5})  # 线程对象.
thread3.start()  # 启动.
```

　　

线程传参和函数传参没什么区别,本质上就是函数传参.

 

#### threading的属性和方法

current_thread()  # 返回当前线程对象.

main_thread()  # 返回主线程对象.

active_count()  # 当前处于alive状态的线程个数.

enumerate()  # 返回所有活着的线程的列表,不包括已经终止的线程和未开始的线程.

get_ident()  # 返回当前线程ID,非0整数.

 

active_count、enumerate方法返回的值还包括主线程。

```python
import threading
import time
def showthreadinfo():
    print('currentthread = {}'.format(threading.current_thread()))
    print('main thread = {}'.format(threading.main_thread()), '"主线程对象"')
    print('active count = {}'.format(threading.active_count()), '"alive"')
def worker():
    count = 1
    showthreadinfo()
    while True:
        if (count > 5):
            break
        time.sleep(1)
        count += 1
        print("I'm working")
t = threading.Thread(target=worker, name='worker')  # 线程对象.
showthreadinfo()
t.start()  # 启动.
print('==END==')
```

　　

#### thread实例的属性和方法

name: 只是一个名称标识,可以重名, getName()、setName()来获取、设置这个名词。

ident: 线程ID, 它是非0整数。线程启动后才会有ID，否则为None。线程退出，此ID依旧可以访问。此ID可以重复使用。

is_alive(): 返回线程是否活着。

 

注： 线程的name是一个名称，可以重复; ID必须唯一，但可以在线程退出后再利用。

```python
import threading
import time
def worker():
    count = 0
    while True:
        if (count > 5):
            break
        time.sleep(1)
        count += 1
        print(threading.current_thread().name, '~~~~~~~~~~~~~~~~~~~~~~~')
t = threading.Thread(name='worker', target=worker)
print(t.ident)
t.start()
while True:
    time.sleep(1)
    if t.is_alive():
        print('{} {} alive'.format(t.name, t.ident))
    else:
        print('{} {} dead'.format(t.name, t.ident))
t.start()
```

　　

start(): 启动线程。每一个线程必须且只能执行该方法一次。

run(): 运行线程函数。

为了演示,派生一个Thread子类

# start方法.

```python
import threading
import time
def worker():
    count = 0
    while True:
        if (count >= 5):
            break
        time.sleep(1)
        count += 1
        print('worker running')
class MyThread(threading.Thread):
    def start(self):
        print('start~~~~~~~~~~~~~')
        super().start()
    def run(self):
        print('run~~~~~~~~~~~~~~~~~')
        super().run()
t = MyThread(name='worker', target=worker)
t.start()
```

 

run方法

```python
import threading
import time
def worker():
    count = 0
    while True:
        if (count > 5):
            break
        time.sleep(1)
        count += 1
        print('worker running')
class MyThread(threading.Thread):
    def start(self):
        print('start~~~~~~~~~~~~~~~')
        super().start()
    def run(self):
        print('run~~~~~~~~~~~~~~~~~')
        super().run()
t = MyThread(name='worker', target=worker)
```

　　

# t.start()

t.run()

 start()方法会调用run()方法，而run()方法可以运行函数。

 这两个方法看似功能重复，但不能只留其一,如下：

```python
import threading
import time
def worker():
    count = 0
    while True:
        if (count > 5):
            break
        time.sleep(1)
        count += 1
        print("worker running")
        print(threading.current_thread().name)
class MyThread(threading.Thread):
    def start(self):
        print('start~~~~~~~~~~~~~')
        super().start()
    def run(self):
        print('run~~~~~~~~~~~~~~~')
        super().run()
t = MyThread(name='worker', target=worker)
# t.start()
```

　　

t.run()  # 分别执行start或者run方法。

 

使用start方法启动线程，启动了一个新的线程，名字叫做worker running,但是使用run方法启动的线程，并没有启动新的线程，只是在主线程中调用了一个普通的函数而已。

因此,启动线程要使用start方法，才能启动多个线程。

#### 多线程

顾名思义，多个线程，一个进程中如果有多个线程，就是多线程，实现一种并发。

 

```
import threading
import time
def worker():
    count = 0
    while True:
        if (count > 5):
            break
        time.sleep(2)
        count += 1
        print('worker running')
        print(threading.current_thread().name, threading.current_thread().ident)
class MyThread(threading.Thread):
    def start(self):
        print('start~~~~~~~~~~~~~~')
        super().start()
    def run(self):
        print('run~~~~~~~~~~~~~~~~~~')
        super().run()  # 查看父类在做什么？
t1 = MyThread(name='worker1', target=worker)
t2 = MyThread(name='worker2', target=worker)
t1.start()
t2.start()
```

可以看到worker1和worker2交替执行。

 

换成run方法试试：

```python
import threading
import time
def worker():
    count = 0
    while True:
        if (count > 5):
            break
        time.sleep(1)
        count += 1
        print('worker running')
        print(threading.current_thread().name, threading.current_thread().ident)
class MyThread(threading.Thread):
    def start(self):
        print('start~~~~~~')
        super().start()
    def run(self):
        print('run~~~~~~~~~~~~')
        super().run()
t1 = MyThread(name='worker1', target=worker)
t2 = MyThread(name='worker2', target=worker)
# t1.start()
# t2.start()
t1.run()
t2.run()
```

 

没有开新的线程，这就是普通函数调用，所以执行完t1.run(),然后执行t2.run(),这里就不是多线程。

**当使用start****方法启动线程后，进程内有多个活动的线程并行的工作，就是多线程。**

一个进程中至少有一个线程，并作为程序的入口，这个线程就是主线程。一个进程至少有一个主线程。

其他线程称为工作线程。

#### 线程安全

需要在ipython中演示：

 

```python
In [1]: import threading
   ...:  def worker():
   ...:     for x in range(5):
   ...:         print("{} is running".format(threading.current_thread().name))
   ...:
   ...:  for x in range(1, 5):
   ...:     name = 'worker{}'.format(x)
   ...:     t = threading.Thread(name=name, target=worker)
   ...:     t.start()
   ...:
```

　　

可以看到运行结果中，本应该是一行行打印，但很多字符串打印在了一起，这说明print函数被打断了，被线程切换打断了。

print函数分两步，第一步打印字符串，第二部换行，就在这之间，发生了线程的切换。

说明print函数不是线程安全函数。

 

**线程安全:** 线程执行一段代码，不会产生不确定的结果，那这段代码就是线程安全的。

 

1、不让print打印换行：

```python
import threading
def worker():
    for x in range(100):
        print('{} is running\n'.format(threading.current_thread().name), end='')
for x in range(1, 5):
    name = 'worker{}'.format(x)
    t = threading.Thread(name=name, target=worker)
    t.start()
```

　　

字符串是不可变类型，它可以作为一个整体不可分割输出。end=''的作用就是不让print输出换行。

 

2、使用logging.

标准库里面的logging模块、日志处理模块、线程安全、生成环境代码都使用logging。

```python
import threading
import logging
def worker():
    for x in range(100):
        # print("{} is running.\n".format(threading.current_thread().name), end='')
        logging.warning('{} is running'.format(threading.current_thread().name))
for x in range(1, 5):
    name = 'work{}'.format(x)
    t = threading.Thread(name=name, target=worker)
    t.start()
```

　　

#### daemon线程和non-daemon线程

注：这里的daemon不是Linux中的守护进程。

 

进程靠线程执行代码，至少有一个主线程，其他线程是工作线程。

主线程是第一个启动的线程。

父线程：如果线程A中启动了一个线程B，A就是B的父线程。

子线程：B就是A的子线程。

 

python中构造线程的时候可以设置daemon属性，这个属性必须在start方法之前设置好。

 

源码Thread的__init__方法中:

```python
if daemon is not None:
    self._daemonic = daemon  # 用户设定bool值。
else:
    self._daemonic = current_thread().daemon
self._ident = None
```

　　

线程daemon属性,如果设定就是用户的设置,否则就取当前线程的daemon值。

主线程是non-daemon,即daemon=False。

 

```python
import time
import threading
def foo():
    time.sleep(5)
    for i in range(20):
        print(i)
```

# 主线程是non-daemon线程.

t = threading.Thread(target=foo, daemon=False)

t.start()

 

print('Main Thread Exiting')

 

**运行发现线程t****依然执行，主线程已经执行完，但是一直等着线程t.**

修改为 t = threading.Threading(target=foo, **daemon=True),****运行发现主线程执行完程序立即结束了**,根本没有等线程t.

 

```python
import threading
import logging
logging.basicConfig(level=logging.INFO) #警告级别
import time
def worker():
    for x in range(10):
        time.sleep(1)
        msg = ("{} is running".format(threading.current_thread()))
        logging.info(msg)
        t = threading.Thread(target=worker1,name="worker1-{}".format(x),daemon=False)
        t.start()
        # t.join()
def worker1():
    for x in range(10):
        time.sleep(0.3)
        msg = ("￥￥￥￥￥{} is running".format(threading.current_thread()))
        logging.info(msg)
t = threading.Thread(target=worker,name='worker-{}'.format(0),daemon=True)
t.start()
# t.join()
time.sleep(0.3)
print('ending')
print(threading.enumerate())
```

　　

结论：

daemon=False **运行发现子线程依然执行，主线程已经执行完，但是主线程会一直等着子线程执行完.**

**daemon=True** **运行发现主线程执行完程序立即结束了。**

 

 daemon属性：表示线程是否是daemon线程，这个值必须在start()之前设置，否则引发RuntimeError异常。

isDaemon()：是否是daemon线程。

setDaemon：设置为daemon线程，必须在start方法之前设置。

 

总结:

线程具有一个daemon属性，可以显式设置为True或False,也可以不设置,不设置则取默认值None。

如果不设置daemon，就取当前线程的daemon来设置它。子子线程继承子线程的daemon值，作用和设置None一样。

主线程是non-daemon线程，即daemon=False。

从主线程创建的所有线程不设置daemon属性，则默认都是daemon=False,也就是non-daemon线程。

python程序在没有活着的non-daemon线程运行时退出，也就是剩下的只能是daemon线程，主线程才能退出，否则主线程就只能等待。

 

如下程序输出：

```python
import time
import threading
def bar():
    time.sleep(10)
    print('bar')
def foo():
    for i in range(20):
        print(i)
    t = threading.Thread(target=bar, daemon=False)
    t.start()
# 主线程是non-daemon线程.
t = threading.Thread(target=foo, daemon=True)
t.start()
print('Main Threading Exiting')
```

上例中，没有输出bar这个字符串，如何修改才会打印出来bar？

```python
import time
import threading
def bar():
    time.sleep(1)
    print('bar')
def foo():
    for i in range(5):
        print(i)
    t = threading.Thread(target=bar, daemon=False)
    t.start()
# 主线程是non-daemon线程.
t = threading.Thread(target=foo, daemon=True)
t.start()
time.sleep(1)
print('Main Threading Exiting')
```

 

再看一个例子，看看主线程合适结束daemon线程。

```python
import time
import threading
def foo(n):
    for i in range(n):
        print(i)
        time.sleep(1)
t1 = threading.Thread(target=foo, args = (10, ), daemon=True)  # 调换10和20，看看效果。
t1.start()
t2 = threading.Thread(target=foo, args = (20, ), daemon=False)
t2.start()
time.sleep(2)
print('Main Threading Exiting')
```

 

上例说明，如果有non-daemon线程的时候，主线程退出时，也不会杀掉所有daemon线程，直到所有non-daemon线程全部结束，

如果还有daemon线程，主线程需要退出，会结束所有 daemon线程，退出。

#### join方法

```python
import time
import threading
def foo(n):
    for i in range(n):
        print(i)
        time.sleep(1)
t1 = threading.Thread(target=foo, args=(10, ), daemon=False)
t1.start()
t1.join()  # 设置join.
print('Main Thread Exiting')
```

 

使用了join方法后，daemon线程执行完了，主线程才退出。

join(timeout=None),是线程的标准方法之一。

一个线程中调用另一个线程的join方法，调用者将被阻塞，直到被调用线程终止。

一个线程可以被join多次。

timeout参数指定调用者等待多久，没有设置超时，就一直等待被调用线程结束。

调用谁的join方法，就是join谁，就要等谁。

 