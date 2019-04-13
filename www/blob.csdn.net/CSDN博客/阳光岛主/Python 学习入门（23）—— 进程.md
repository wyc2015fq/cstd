
# Python 学习入门（23）—— 进程 - 阳光岛主 - CSDN博客

2013年12月11日 00:06:05[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5908所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



本文介绍Python的os包中有查询和修改进程信息的函数，Python的这些工具符合Linux系统的相关概念，所以可以帮助理解Linux体系。
**1. 进程信息**
os包中相关函数如下：
uname()返回操作系统相关信息，类似于Linux上的uname命令。
umask()设置该进程创建文件时的权限mask，类似于Linux上的umask命令。
get*()查询 (*由以下代替)
uid, euid, resuid, gid, egid, resgid ：权限相关，其中resuid主要用来返回saved UID。相关介绍见Linux用户与“最小权限”原则
pid, pgid, ppid, sid                 ：进程相关。相关介绍见Linux进程关系
put*()设置 (*由以下代替)
euid, egid： 用于更改euid，egid。
uid, gid  ： 改变进程的uid, gid。只有super user才有权改变进程uid和gid (意味着要以$sudo python的方式运行Python)。
pgid, sid ： 改变进程所在的进程组(process group)和会话(session)。
getenviron()：获得进程的环境变量
setenviron()：更改进程的环境变量
例1，进程的real UID和real GID， \#注释后面是结果

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
import os
print(os.getuid())              # 1000
print(os.getpid())              # 9047
print(os.getppid())             # 6829
print(os.getgid())              # 1000
print(os.getgroups())           # [4, 24, 27, 30, 46, 109, 124, 1000]
print(os.getenv("JAVA_HOME", ))      # /home/homer/eclipse/jdk1.6.0_22
```
将上面的程序保存为py_id.py文件，分别用$python py_id.py和$sudo python py_id.py看一下运行结果

**2. 有关saved UID和saved GID**
saved UID和saved GID很难如同我们在Linux用户与“最小权限”原则中描述的那样在Python程序工作。原因在于，当我们写一个Python脚本后，我们实际运行的是python这个解释器，而不是python脚本文件 (而C语言则是直接运行由C语言编译成的执行文件)。我们必须更改python这个执行文件本身的权限来运用saved UID机制，然而这么做又是异常危险的。
比如说，我们的python执行文件为/usr/bin/python (你可以通过$which python获知)
我们先看一下
$ls -l /usr/bin/python
的结果:
-rwxr-xr-x root root
我们修改权限以设置set UID和set GID位 (参考[Linux用户与“最小权限”原则](http://www.cnblogs.com/vamei/archive/2012/10/07/2713593.html))
$sudo chmod 6755 /usr/bin/python
/usr/bin/python的权限成为:
-rwsr-sr-x root root
随后，我们运行文件下面test.py文件，这个文件可以是由普通用户vamei所有:
importosprint(os.getresuid())
我们得到结果:
(1000, 0, 0)
上面分别是UID，EUID，saved UID。我们只用执行一个由普通用户拥有的python脚本，就可以得到super user的权限！所以，这样做是极度危险的，我们相当于交出了系统的保护系统。想像一下Python强大的功能，别人现在可以用这些强大的功能作为攻击你的武器了！使用下面命令来恢复到从前:
$sudo chmod 0755 /usr/bin/python
**总结:**
get*, set*
umask(), uname()


**Python 多进程**
**1. threading 和 multiprocessing**
multiprocessing包是Python中的多进程管理包。与threading.Thread类似，它可以利用multiprocessing.Process对象来创建一个进程。该进程可以运行在Python程序内部编写的函数。该Process对象与Thread对象的用法相同，也有start(), run(), join()的方法。此外multiprocessing包中也有Lock/Event/Semaphore/Condition类(这些对象可以像多线程那样，通过参数传递给各个进程)，用以同步进程，其用法与threading包中的同名类一致。所以，multiprocessing的很大一部份与threading使用同一套API，只不过换到了多进程的情境。
但在使用这些共享API的时候，我们要注意以下几点:
在UNIX平台上，当某个进程终结之后，该进程需要被其父进程调用wait，否则进程成为僵尸进程(Zombie)。所以，有必要对每个Process对象调用join()方法 (实际上等同于wait)。对于多线程来说，由于只有一个进程，所以不存在此必要性。
multiprocessing提供了threading包中没有的IPC(比如Pipe和Queue)，效率上更高。应优先考虑Pipe和Queue，避免使用Lock/Event/Semaphore/Condition等同步方式 (因为它们占据的不是用户进程的资源)。
多进程应该避免共享资源。在多线程中，我们可以比较容易地共享资源，比如使用全局变量或者传递参数。在多进程情况下，由于每个进程有自己独立的内存空间，以上方法并不合适。此时我们可以通过共享内存和Manager的方法来共享资源。但这样做提高了程序的复杂度，并因为同步的需要而降低了程序的效率。
Process.PID中保存有PID，如果进程还没有start()，则PID为None。
我们可以从下面的程序中看到Thread对象和Process对象在使用上的相似性与结果上的不同。各个线程和进程都做一件事：打印PID。但问题是，所有的任务在打印的时候都会向同一个标准输出(stdout)输出。这样输出的字符会混合在一起，无法阅读。使用Lock同步，在一个任务输出完成之后，再允许另一个任务输出，可以避免多个任务同时向终端输出。

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
import os
import threading
import multiprocessing
# worker function
def worker(sign, lock):
    lock.acquire()
    print(sign, os.getpid())
    lock.release()
# Main
print('Main:', os.getpid())      # 主进程
# Multi-thread
record = []
lock  = threading.Lock()
for i in range(5):
    thread = threading.Thread(target=worker,args=('thread', lock))
    thread.start()
    record.append(thread)
for thread in record:
    print(thread)
    thread.join()
# Multi-process
record = []
lock = multiprocessing.Lock()
for i in range(5):
    process = multiprocessing.Process(target=worker,args=('process',lock))
    process.start()
    record.append(process)
for process in record:
    print(process)
    process.join()
```
运行结果：

```python
('Main:', 9904)
('thread', 9904)
('thread', 9904)
('thread', 9904)
('thread'<Thread(Thread-1, stopped 140098907965184)>
<Thread(Thread-2, stopped 140098907965184)>
<Thread(Thread-3, stopped 140098899572480)>
<Thread(Thread-4, started 140098907965184)>
, 9904)
<Thread(Thread-5, started 140098899572480)>(
'thread', 9904)
('process', 9914)
('process', 9915)
('proces<Process(Process-1, stopped)>s', 9916)
<Process(Process-2, stopped)>
<Process(Process-3, started)>
<Process(Process-4, started)>
('process', 9917)
<Process(Process-5, started)>
('process', 9918)
```
所有Thread的PID都与主程序相同，而每个Process都有一个不同的PID。
使用mutiprocessing包将Python多线程与同步中的多线程程序更改为多进程程序

**2. Pipe和Queue**
正如我们在Linux多线程中介绍的管道PIPE和消息队列message queue，在multiprocessing包中有Pipe类和Queue类来分别支持这两种IPC机制。Pipe和Queue可以用来传送常见的对象。
1) Pipe可以是单向(half-duplex)，也可以是双向(duplex)。我们通过mutiprocessing.Pipe(duplex=False)创建单向管道 (默认为双向)。一个进程从PIPE一端输入对象，然后PIPE另一端的进程接收对象。单向管道只允许管道一端的进程输入，而双向管道则允许从两端输入。
下面的程序展示了Pipe的使用:

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
import multiprocessing as multipro
def proc1(pipe):
    pipe.send('hello')
    print('proc1 recv:', pipe.recv())
def proc2(pipe):
    print('proc2 recv:', pipe.recv())
    pipe.send('hello, too')
# Build a pipe
pipe = multipro.Pipe()
# Pass an end of the pipe to process 2
p1 = multipro.Process(target=proc1, args=(pipe[0],))
# Pass the other end of the pipe to process 1
p2 = multipro.Process(target=proc2, args=(pipe[1],))
p1.start()
p2.start()
p1.join()
p2.join()
```
运行结果：
('proc2 rec:', 'hello')
('proc1 rec:', 'hello, too')
这里的Pipe是双向的。
Pipe对象建立的时候，返回一个含有两个元素的表，每个元素代表Pipe的一端(Connection对象)。我们对Pipe的某一端调用send()方法来传送对象，在另一端使用recv()来接收。
2) Queue与Pipe相类似，都是先进先出的结构。但Queue允许多个进程放入，多个进程从队列取出对象。Queue使用mutiprocessing.Queue(maxsize)创建，maxsize表示队列中可以存放对象的最大数量。
下面的程序展示了Queue的使用:

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
import os
import multiprocessing
import time
# input worker
def inputQ(queue):
    info = str(os.getpid()) + ' (put): ' + str(time.strftime("%Y-%m-%d__%H:%M:%S", time.localtime(time.time())))
    queue.put(info)
# output worker
def outputQ(queue, lock):
    info = queue.get()
    lock.acquire()
    print (str(os.getpid()) + '(get):' + info + "\n")
    lock.release()
    
# Main
record1 = []                        # store input processes
record2 = []                        # store output processes
lock = multiprocessing.Lock()       # To prevent messy print
queue = multiprocessing.Queue(3)
# input processes
for i in range(10):
    process = multiprocessing.Process(target=inputQ, args=(queue,))
    process.start()
    record1.append(process)
# output processes
for i in range(10):
    process = multiprocessing.Process(target=outputQ, args=(queue, lock))
    process.start()
    record2.append(process)
for p in record1:
    p.join()
queue.close()  # No more object will come, close the queue
for p in record2:
    p.join()
```
运行结果：

```python
10370(get):10357 (put): 2013-12-11__19:32:09
10369(get):10356 (put): 2013-12-11__19:32:09
10372(get):10359 (put): 2013-12-11__19:32:09
10371(get):10360 (put): 2013-12-11__19:32:09
10378(get):10366 (put): 2013-12-11__19:32:09
10374(get):10365 (put): 2013-12-11__19:32:09
10376(get):10364 (put): 2013-12-11__19:32:09
10380(get):10361 (put): 2013-12-11__19:32:09
10381(get):10368 (put): 2013-12-11__19:32:09
10383(get):10367 (put): 2013-12-11__19:32:09
```

一些进程使用put()在Queue中放入字符串，这个字符串中包含PID和时间。另一些进程从Queue中取出，并打印自己的PID以及get()的字符串。
**总结：**
Process, Lock, Event, Semaphore, Condition
Pipe, Queue


**博客之星评选，请投我一票：**
**http://vote.blog.csdn.net/blogstaritem/blogstar2013/sunboy_2050**


