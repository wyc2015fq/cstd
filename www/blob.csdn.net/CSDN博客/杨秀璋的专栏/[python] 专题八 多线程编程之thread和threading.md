
# [python] 专题八.多线程编程之thread和threading - 杨秀璋的专栏 - CSDN博客

2015年12月09日 05:04:43[Eastmount](https://me.csdn.net/Eastmount)阅读数：19798所属专栏：[Python学习系列](https://blog.csdn.net/column/details/eastmount-python.html)



就个人而言，学了这么多年的课程又写了这么多年的程序，虽然没有涉及到企业级的项目，但还是体会到了有几个知识点是非常重要的，包括：面向对象的思想、如何架构一个项目、设计模式来具体解决问题、应用机器学习和深度学习的方法，当然也包括我这篇文章的内容——多线程和并行化处理数据。
这篇文章主要是参考Wesley J. Chun的《Python核心编程(第二版)》书籍多线程部分，并结合我以前的一些实例进行简单分析。尤其是在大数据、Hadoop\Spark、分布式开发流行的今天，这些基础同样很重要。希望对你有所帮助吧！
PS：推荐大家阅读《Python核心编程》和《Python基础教程》两本书~
强推：[http://www.cnblogs.com/huxi/archive/2010/06/26/1765808.html](http://www.cnblogs.com/huxi/archive/2010/06/26/1765808.html)

## 一. 线程和进程的概念
**1.为什么引入多线程编程？**
在多线程（Multithreaded，MT）编程出现之前，电脑程序的运行由一个执行序列组成，执行序列按顺序在主机的中央处理器CPU中运行。即使整个程序由多个相互独立无关的子任务组成，程序都会顺序执行。
由于并行处理可以大幅度地提升整个任务的效率，故引入多线程编程。
多线程中任务具有以下特点：
(1) 这些任务的本质是异步的，需要有多个并发事务；
(2) 各个事务的运行顺序可以是不确定的、随机的、不可预测的。
这样的编程任务可以分成多个执行流，每个流都有一个要完成的目标。再根据不同的应用，这些子任务可能都要计算出一个中间结果，用于合并得到最后的结果。
**2.什么是进程？**
计算机程序只不过是磁盘中可执行的二进制（或其他类型）的数据。它们只有在被读取到内存中，被操作系统调用时才开始它们的生命周期。
进程（亦称为重量级进程）是程序的一次执行。每个进程都有自己的地址空间、内存、数据栈及其他记录其运行轨迹的辅助数据。操作系统管理在其上运行所有的进程，并为这些进程公平分配时间、进程也可以通过[fork](http://www.cnblogs.com/bastard/archive/2012/08/31/2664896.html)和spawn操作来完成其他的任务。
不过进程有自己的内存空间，数据栈等，所以只能使用进程间通讯（interprocess communication, IPC），而不能直接共享信息。
**3.什么是线程？**
线程（亦称为轻量级进程）跟进程有些相似，不同的是：所有的线程运行在同一个进程中，共享相同的运行环境。它们可以被想象成是在主进程或“主线程”中并行运行的“迷你进程”。
线程有开始，顺序执行和结束三部分。它有一个自己的指令指针，记录自己运行到什么地方。线程的运行可能被抢占（中断）或暂时的被挂起（睡眠），让其他线程运行，这叫做让步。
一个进程中的各个线程之间共享同一片数据空间，所以线程之间可以比进程之间更方便地共享数据以及相互通讯。线程一般都是并发执行的，正是由于这种并行和数据共享的机制使得多个任务的合作变成可能。
实际上，在单CPU的系统中，真正的并发是不可能的，每个线程会被安排成每次只运行一小会，然后就把CPU让出来，让其他的线程去运行。在进程的整个运行过程中，每个线程都只做自己的事，在需要的时候跟其他的线程共享运行的结果。
当然，这样的共享并不是完全没有危险的。如果多个线程共同访问同一片数据，则由于数据访问的顺序不同，有可能导致数据结果的不一致的问题，即竞态条件（race condition）。同样，大多数线程库都带有一些列的同步原语，来控制线程的执行和数据的访问。
另一个需要注意的是由于有的函数会在完成之前阻塞住，在没有特别为多线程做修改的情况下，这种“贪婪”的函数会让CPU的时间分配有所倾斜，导致各个线程分配到的运行时间可能不尽相同，不尽公平。
**4.简述进程和线程的区别**
参考下面三篇文章：
[进程和线程关系及区别 - yaosiming2011](http://blog.csdn.net/yaosiming2011/article/details/44280797)
[进程与线程的区别 - flashsky](http://www.cnblogs.com/flashsky/articles/642720.html)
[应届生经典面试题：说说进程与线程的区别与联系 - way_testlife](http://www.cnblogs.com/way_testlife/archive/2011/04/16/2018312.html)


## 二. Python线程和全局解释器锁
**1.全局解释器锁(GIL)**
Python代码的执行由Python虚拟机（也叫解释器主循环）来控制。Python在设置之初就考虑到要在主循环中，同时只有一个线程在执行，就像单CPU的系统中运行多个进程那样，内存中可以存放多个程序，但任意时刻，只有一个程序在CPU中运行。同样，虽然Python解释器可以“运行”多个线程，但任意时刻，只有一个线程在解释器中运行。
对Python虚拟机的访问由全局解释器锁（global interpreter lock，GIL）来控制，正是这个锁能保证同一时刻只有一个线程在运行。在多线程环境中，Python虚拟机按一下方式执行：
(1) 设置GIL
(2) 切换到一个线程去运行
(3) 运行：
a. 指定数量的字节码的指令，或者
b. 线程主动让出控制（可以调用time.sleep(0)）
(4) 把线程设置为睡眠状态
(5) 解锁GIL
(6) 再次重复以上所有步骤
在调用外部代码（如C/C++扩展函数）的时候，GIL将会被锁定，直到这个函数结束为止（由于这期间没有Python的字节码被运行，所以不会做线程切换）。编写扩展的程序员可以主动解锁GIL。不过Python开发人员则不用担心在这些情况下你的Python代码会被锁住。
对源代码，解释器主循环和GIL感兴趣的人，可以看看Python/ceval.c文件。
**2.退出线程**
当一个线程结束计算，它就退出了。线程可以调用thread.exit()之类的退出函数，也可以使用Python退出进程的标准方法，如sys.exit()或抛出一个SystemExit异常等。不过，你不可以直接杀掉Kill一个线程。
后面会讲述两个与线程相关的模块，在这两个模块中，该书中不建议使用thread模块。主要原因是当主线程退出的时候，其他所有线程没有被清除就退出了。而threading模块就能确保所有“重要的”子线程都退出后，进程才会结束。
主线程应该是一个好的管理者，它要了解每个线程都要做些什么事，线程都需要什么数据和什么参数，以及在线程结束的时候，它们都提供了什么结果。这样，主线程就可以把各个线程的结果组成一个有意义的最后结果。
在Python2.7交互式解释器中导入**import thread**没有报错即表示线程可用。
**3.没有线程的例子**
使用time.sleep()函数来演示线程的工作，这个例子主要为后面线程做对比。time.sleep()需要一个浮点型的参数，来指定“睡眠”的时间（单位秒）。这就相当于程序的运行会被挂起指定的时间。
代码解释：两个计时器，loop0睡眠4秒，loop1()睡眠2秒，它们是在一个进程或者线程中，顺序地执行loop0()和loop1()，那总运行时间为6秒。有可能启动过程中会再花些时间。

```python
from time import sleep, ctime
def loop0():
    print 'Start loop 0 at:', ctime()
    sleep(4)
    print 'Loop 0 done at:', ctime()
def loop1():
    print 'Start loop 1 at:', ctime()
    sleep(2)
    print 'Loop 1 done at:', ctime()
def main():
    print 'Starting at:', ctime()
    loop0()
    loop1()
    print 'All done at:', ctime()
if __name__ == '__main__':
    main()
```
代码的运行结果如下图所示，它将和后面的并行代码做对比。
![](https://img-blog.csdn.net/20151208032829194)

**4.避免使用thread模块**
Python提供了几个用于多线程编程的模块，包括thread、threading和Queue等。
(1) thread模块: 允许程序员创建和管理线程，它提供了基本的线程和锁的支持。
(2) threading模块: 允许程序员创建和管理线程，它提供了更高级别，更强的线程管理的功能。
(3) Queue模块: 允许用户创建一个可用于多个线程间共享数据的队列数据结构。
下面简单分析为什么需要避免使用thread模块？
(1) 首先更高级别的threading模块更为先进，对线程的支持更为完善，而且使用thread模块里的属性有可能会与threading出现冲突。
(2) 其次，低级别的thread模块的同步原语很少（实际只有一个），而threading模块则有很多。
(3) 另一个原因是thread对你的进程什么时候应该结束完全没有控制，当主线程结束时，所有的线程都会被强制结束掉，没有警告也不会有正常的清除工作。而threading模块能确保重要的子线程退出后进程才退出。
当然，为了你更好的理解线程，还是会对thread进行讲解。但是我们只建议那些有经验的专家想访问线程的底层结构时，才使用thread模块。而如果可以，你的第一个线程程序应尽可能使用threading等高级别的模块。

## 三. thread模块
**1.基础知识**
首先来看看thread模块都提供了些什么。除了产生线程外，thread模块也提供了基本的同步数据结构锁对象（lock object，也叫原语锁、简单锁、互斥锁、互斥量、二值信号量）。同步原语与线程的管理是密不可分的。
常用的线程模块函数
|模块函数
|描述
|
|start_new_thread(function, args kwargs=None)
|产生一个新线程，在新线程中用指定的参数和可选的kwargs来调用该函数
|
|allocate_lock()
|分配一个LockType类型的锁对象
|
|exit()
|让线程退出
|
LockType类型锁对象方
|类型锁对象方法
|描述
|
|acquire(wait=None)
|尝试获取锁对象
|
|locked()
|如果获取了锁对象返回True，否则返回False
|
|release()
|释放锁
|
start_new_thread()函数是thread模块的一个关键函数，它的语法和内建的apply()函数一样，其参数为：函数，函数的参数以及可选的关键字的参数。不同的是，函数不是在主线程里运行，而是产生一个新的线程来运行这个函数。
**2.Thread模块实现代码**
现在实现一个线程的代码，与前面没有线程总运行时间为6秒的进行对比。
```python
import thread
from time import sleep, ctime
def loop0():
    print 'Start loop 0 at:', ctime()
    sleep(4)
    print 'Loop 0 done at:', ctime()
def loop1():
    print 'Start loop 1 at:', ctime()
    sleep(2)
    print 'Loop 1 done at:', ctime()
def main():
    try:
        print 'Starting at:', ctime()
        thread.start_new_thread(loop0, ())
        thread.start_new_thread(loop1, ())
        sleep(6)
        print 'All done at:', ctime()
    except Exception,e:      
        print 'Error:',e  
    finally:    
        print 'END\n'  
if __name__ == '__main__':
    main()
```
代码解释：
使用thread模块提供简单的额多线程机制。loop0和loop1并发地被执行（显然，短的那个先结束），总的运行时间为最慢的那个线程的运行时间，而不是所有的线程的运行时间之和。start_new_thread()要求一定要有前两个参数，即使运行的函数不要参数，也要传一个空的元组。
由于采用Python IDLE运行总是报错Runtime，而且已经设置了sleep(6)。运行一个线程勉强能运行，两个线程无论是thread或threading都报错，估计环境配置问题。
![](https://img-blog.csdn.net/20151208180607605)
最后采用Cygwin Terminal模拟Linux下运行程序。可以发现loop1和loop0是并发执行的，其中loop1先结束运行2秒，而loop0运行4秒。
同时程序主函数中多了个sleep(6)，为什么要加这一句话呢？
因为如果我们没有让主线程停下来，那主线程就会运行下一条语句，显示“All done”，然后就关闭运行着loop0和loop1的两个线程，退出了。
我们没有写让主线程停下来等所有子线程结束后再继续运行的代码，这就是前面所说的需要同步的原因。在这里，我们使用sleep(6)作为同步机制。设置6秒，两个线程一个4秒（53-57），一个2秒（53-55），在主线程等待6秒（53-59）后应该已经结束了。
![](https://img-blog.csdn.net/20151208181410570)
cygwin需要用到的常见用法包括，也可以安装VIM编辑器：
cd c:    　　　　　　  进入 'c:' 目录，空格用'\ '转义字符
pwd      　　　 　　   显示工作路径
ls         　　　　　　查看目录中的文件
file test.py                 查看文件内容
python test.py          运行python程序
配置方法见：[http://blog.sina.com.cn/s/blog_691ebcfc0101lgme.html](http://blog.sina.com.cn/s/blog_691ebcfc0101lgme.html)
下载地址见：[http://pan.baidu.com/s/1jGYEtro](http://pan.baidu.com/s/1jGYEtro)
**3.线程加锁方法**
那么，有什么好的管理线程的方法呢？而不是在主线程里做个额外的延时6秒操作。因为总的运行时间并不比单线程的代码少；而且使用sleep()函数做线程的同步操作是不可靠的；如果循环的执行时间不能事先确定的话，这可能会造成主线程过早或过晚的退出。
这就需要引入锁的概念。下面代码执行loop函数，与前面代码的区别是不用为线程什么时候结束再做额外的等待了。使用锁之后，可以在两个线程都退出后，马上退出。
```python
#coding=utf-8
import thread 
from time import sleep, ctime 
 
loops = [4,2]                           #等待时间
#锁序号 等待时间 锁对象
def loop(nloop, nsec, lock):
    print 'start loop', nloop, 'at:', ctime() 
    sleep(nsec) 
    print 'loop', nloop, 'done at:', ctime()
    lock.release()                      #解锁                  
 
def main():
    print 'starting at:', ctime()
    locks =[]
    nloops = range(len(loops))          #以loops数组创建列表并赋值给nloops
         
    for i in nloops:
        lock = thread.allocate_lock()   #创建锁对象
        lock.acquire()                  #获取锁对象 加锁
        locks.append(lock)              #追加到locks[]数组中 
    #执行多线程 (函数名,函数参数)
    for i in nloops:
        thread.start_new_thread(loop,(i,loops[i],locks[i]))
    #循环等待顺序检查每个所都被解锁才停止    
    for i in nloops:
        while locks[i].locked():
            pass
    print 'all end:', ctime() 
if __name__ == '__main__': 
    main()
```
运行结果如下：
Starting at: Tue Dec  8 21:57:56 2015
Start loop 0 at: Tue Dec  8 21:57:56 2015
Start loop 1 at: Tue Dec  8 21:57:56 2015
Loop 1 done at: Tue Dec  8 21:57:58 2015
Loop 0 done at: Tue Dec  8 21:58:00 2015
All end: Tue Dec  8 21:58:00 2015
我们在函数中记录下循环的号码和睡眠的时间，同时每个线程都会被分配一个事先已经获得的锁，在sleep()的时间到了之后就释放相应的锁以通知住线程，这个线程已经结束了。
(1) loops[4, 2]定义睡眠时间 nloops=range(len(loops))创建列表[0, 1] 号码；
(2) 调用thread.allocate_lock()函数创建一个锁的列表，并分别调用各个锁的acquire()函数获得锁对象。获得锁表示“把锁锁上”，并放到锁列表locks中；
(3) 再循环创建线程，调用thread.start_new_thread(loop,(i,loops[i],locks[i]))。参数对应线程循环号、睡眠时间和锁。
(4) 在线程结束时，需要做解锁操作，调用lock.release()函数；
(5) 最后一个循环是坐在那一直等待（达到暂停主线程的目的），直到两个锁都被解锁才继续运行。它是顺序检查每个锁，主线程需不停地对所有锁进行检查直到都释放。
为什么我们不在创建锁的循环里创建线程呢？一方面是想实现线程的同步，所以要让“所有的马同时冲出栅栏”；另外获取锁要花些时间，如果线程退出太快，可能导致还没有获得锁，线程就已经结束了。
最后再强调下，thread模块仅仅了解就行，你应该使用更高级别的threading等。

## 四. threading模块
threading模块不仅提供了Thread类，还提供了各种非常好用的同步机制。如下表列出了threading模块里所有的对象。
|threading模块对象
|描述
|
|Thread
|表示一个线程的执行的对象
|
|Lock
|锁原语对象（跟thread模块里的锁对象相同）
|
|RLock
|可重入锁对象。使单线程可以再次获得已经获得了的锁（递归锁定）
|
|Condition
|条件变量对象能让一个线程停下来，等待其他线程满足了某个“条件”。如状态的改变或值的改变
|
|Event
|通用的条件变量。多个线程可以等待某个时间的发生，在事件发生后，所有的线程都被激活
|
|Semaphore
|为等待锁的线程提供一个类似“等候室”的结构
|
|BoundedSemaphore
|与Semaphore类似，只是它不允许超过初始值
|
|Timer
|与thread类似，只是它要等待一段时间后才开始运行
|
**1.守护线程**
其中thread模块需要避免的一个原因是：它不支持守护线程。当主线程退出时，所有的子线程不论它们是否还在工作，都会被强行退出。有时我们并不期望这种行为，这就引入了守护线程的概念。
Threading模块支持守护线程，它们工作流程如下：守护线程一般是一个等待客户请求的服务器，如果没有客户提出请求，它就在那等着。如果你设定一个线程为守护线程，就表示你在说这个线程是不重要的，在进程退出时，不用等待这个线程退出，正如网络编程中服务器线程运行在一个无限循环中，一般不会退出的。
如果你的主线程要退出的时候，不用等待那些子线程完成，那就设定这些线程的**daemon属性**。即，线程开始（调用thread.start()）之前，调用**setDaemon()函数**设定线程的daemon标准（thread.setDaemon(True)）就表示这个线程“不重要”。
如果你想要等待子线程完成再退出，那就什么都不用做，或者显示地调用**thread.setDaemon(False)**以保证其daemon标志位False。你可以调用thread.isDaemon()函数来判断其daemon标志的值。
新的子线程会继承其父线程的daemon标志，整个Python会在所有的非守护线程退出后才会结束，即进程中没有非守护线程存在的时候才结束。
**2.Thread类**
threading的Thread类是你主要的运行对象。它有很多thread模块里没有的函数。
|函数
|描述
|
|start()
|开始线程的执行
|
|run()
|定义线程的功能的函数（一般会被子类重写）
|
|join(timeout=None)
|程序挂起，直到线程结束；如果给了timeout，则最多阻塞timeout秒
|
|getName()
|返回线程的名字
|
|setName(name)
|设置线程的名字
|
|isAlive()
|布尔标志，表示这个线程是否还在运行中
|
|isDaemon()
|返回线程的daemon标志
|
|setDaemon(daemonic)
|把线程的daemon标志设为daemonic（一定要在调用start()函数前调用）
|
用Thread类，可以用多种方法来创建线程。现在介绍三种方法，你可以选择自己喜欢或社和自己程序的方法（通常选择最后一个）：
(1) 创建一个Thread的实例，传给它一个函数；
(2) 创建一个Thread的实例，传给它一个可调用的类对象；
(3) 从Thread派生出一个子类，创建一个这个子类的实例。
**3.创建Thread实例，传给它一个函数**
这第一个例子使用方法一，把函数及其参数如上面Thread模块的例子一样传进去。主要变化包括：添加了一些Thread对象；在实例化每个Thread对象时，把函数（target）和参数（args）都传进去，得到返回的Thread实例。
实例化一个Thread调用Thread()方法与调用thread.start_new_thread()之间的最大区别是：新的线程不会立即开始。在你创建线程对象，但不想马上开始运行线程的时候，这是一个很有用的同步特性。
threading模块的Thread类有一个join()函数，允许主线程等待线程的结束。
```python
#coding=utf-8
import threading 
from time import sleep, ctime 
 
loops = [4,2]                      #睡眠时间
 
def loop(nloop, nsec):
    print 'Start loop', nloop, 'at:', ctime() 
    sleep(nsec) 
    print 'Loop', nloop, 'done at:', ctime()
    
 
def main():
    print 'Starting at:', ctime()
    threads = []
    nloops = range(len(loops))     #列表[0,1]
        
    #创建线程
    for i in nloops:
        t = threading.Thread(target=loop,args=(i,loops[i]))
        threads.append(t)
    #开始线程
    for i in nloops:
        threads[i].start()
    #等待所有结束线程
    for i in nloops:
        threads[i].join()
    print 'All end:', ctime() 
if __name__ == '__main__': 
    main()
```
运行结果如下图所示：其中loop0和loop1并行执行，loop1先结束共执行2秒，loop0后结束执行4秒，总共运行时间4秒。注意：此时Start是分行显示了。
![](https://img-blog.csdn.net/20151209020707012)
所有的线程都创建之后，再一起调用**start()函数启动线程**，而不是创建一个启动一个。而且，不用再管理一堆锁（分配锁、获得锁、释放锁、检查锁的状态等），只要简单地对每个线程调用**join()函数**就可以了。
join()会**等到线程结束**，或者在给了timeout参数的时候，等到超时为止。使用join()比使用一个等待锁释放的无限循环清楚一些（也称“自旋锁”）。
join()的另一个比较重要的方法是它可以**完全不用调用**。一旦线程启动后，就会一直运行，直到线程的函数结束，退出为止。
如果你的主线程除了等线程结束外，还有其他的事情要做（如处理或等待其他的客户请求），那就不用调用join()，只有在你要等待线程结束的时候才要调用join()。
**4.创建一个Thread实例，传给它一个可调用的类对象**
这是第二个方法，与传一个函数很相似，但它是传一个可调用的类的实例供线程启动的时候执行，这是多线程编程的一个更为面向对象的方法。相对于一个或几个函数来说，由于类对象里可以使用类请打的功能，可以保存更多的信息，这种方法更为灵活。
```python
#coding=utf-8
import threading 
from time import sleep, ctime 
 
loops = [4,2]                   #睡眠时间
class ThreadFunc(object):
    def __init__(self, func, args, name=''):
        self.name=name
        self.func=func
        self.args=args
    def __call__(self):
        apply(self.func, self.args)
def loop(nloop, nsec):
    print "Start loop", nloop, 'at:', ctime()
    sleep(nsec)
    print 'Loop', nloop, 'done at:', ctime()
def main():
    print 'Starting at:', ctime()
    threads=[]
    nloops = range(len(loops))   #列表[0,1]
    for i in nloops:
        #调用ThreadFunc类实例化的对象，创建所有线程
        t = threading.Thread(
                target=ThreadFunc(loop, (i,loops[i]), loop.__name__)
            ) 
        threads.append(t)
        
    #开始线程
    for i in nloops:
        threads[i].start()
    #等待所有结束线程
    for i in nloops:
        threads[i].join()
    print 'All end:', ctime() 
if __name__ == '__main__': 
    main()
```
运行结果如下图所示，传递的是一个可调用的类，而不是一个函数。
![](https://img-blog.csdn.net/20151209025305486)
创建Thread对象时会实例化一个可调用类ThreadFunc的类对象。这个类保存了函数的参数，函数本身以及函数的名字字符串。
构造器__init__()函数：初始化赋值工作；
特殊函数__call__()：由于我们已经有要用的参数，所以就不用再传到Thread()构造器中；由于我们有一个参数的元组，这时要在代码中使用apply()函数。
apply(func [, args [, kwargs ]])函数：用于当函数参数已经存在于一个元组或字典中时，间接地调用函数。args是一个包含将要提供给函数的按位置传递的参数的元组。如果省略了args，任何参数都不会被传递，kwargs是一个包含关键字参数的字典。
```python
def say(a, b):
    print a, b
apply(say,("Eastmount", "Python线程"))
# 输出
# Eastmount Python线程
```
**5.Thread派生一个子类，创建这个子类的实例**
这是第三个方法，主要是如何子类化Thread类，该方法与第二个方法类似。其中创建子类方法和调用类对象方法的最重要改变是：
(1) MyThread子类的构造器一定要先调用基类的构造器；
(2) 之前特殊函数__call__()在子类中，名字要改为run()。
```python
#coding=utf-8
import threading 
from time import sleep, ctime 
 
loops = [4,2]                   #睡眠时间
class MyThread(threading.Thread):
    def __init__(self, func, args, name=''):
        threading.Thread.__init__(self)
        self.name=name
        self.func=func
        self.args=args
    def run(self):              #run()函数
        apply(self.func, self.args)
def loop(nloop, nsec):
    print "Start loop", nloop, 'at:', ctime()
    sleep(nsec)
    print 'Loop', nloop, 'done at:', ctime()
def main():
    print 'Starting at:', ctime()
    threads=[]
    nloops = range(len(loops))   #列表[0,1]
    for i in nloops:
        #子类MyThread实例化，创建所有线程
        t = MyThread(loop, (i,loops[i]), loop.__name__)
        threads.append(t)
        
    #开始线程
    for i in nloops:
        threads[i].start()
    #等待所有结束线程
    for i in nloops:
        threads[i].join()
    print 'All end:', ctime() 
if __name__ == '__main__': 
    main()
```
运行结果如下图所示：
![](https://img-blog.csdn.net/20151209035341251)
**6.线程运行斐波那契、阶乘和加和**
需要在MyThread类中加入输出信息，除了使用apply()函数运行斐波那契、接触和加和函数外，还把结果保存到实现的self.res属性中，并创建一个函数getResult()得到结果。换句话说，子类方法更加灵活。
```python
#coding=utf-8
import threading 
from time import sleep, ctime 
class MyThread(threading.Thread):
    def __init__(self, func, args, name=''):
        threading.Thread.__init__(self)
        self.name=name
        self.func=func
        self.args=args
    def getResult(self):
        return self.res
    
    def run(self):              #run()函数
        print "Starting", self.name, 'at:', ctime()
        self.res = apply(self.func, self.args)
        print self.name, 'finished at:', ctime()
```
在threadfunc.py文件中调用前面定义的Thread子类，myThread.py中的MyThread类。由于这些函数运行得很快（斐波那契函数运行慢些），使用sleep()函数比较它们的时间。实际工作中不需要添加sleep()函数。
```python
#coding=utf-8
from myThread import MyThread   #myThread.py文件中MyThread类
from time import sleep, ctime 
#斐波那契函数
def fib(x):
    sleep(0.005)
    if x < 2:
        return 1
    return (fib(x-2) + fib(x-1))
#阶乘函数 factorial calculation 
def fac(x):
    sleep(0.1)
    if x < 2:
        return 1
    return (x * fac(x-1))
#求和函数
def sum(x):
    sleep(0.1)
    if x < 2:
        return 1
    return (x + sum(x-1))
funcs = [fib, fac, sum]
n = 14
def main():
    nfuncs = range(len(funcs))
    print '*****单线程方法*****'
    for i in nfuncs:
        print 'Starting', funcs[i].__name__, 'at:', ctime()
        print funcs[i](n)
        print 'Finished', funcs[i].__name__, 'at:', ctime()
    print '*****结束单线程*****'
    print ' '
    print '*****多线程方法*****'
    threads = []
    for i in nfuncs:
        #调用MyThread类实例化的对象，创建所有线程
        t = MyThread(funcs[i], (n,), funcs[i].__name__)
        threads.append(t)
        
    #开始线程
    for i in nfuncs:
        threads[i].start()
    #等待所有结束线程
    for i in nfuncs:
        threads[i].join()
        print threads[i].getResult()
    print '*****结束多线程*****'
if __name__ == '__main__': 
    main()
```
运行结果如下图所示，单线程运行10s，多线程运行6s。
![](https://img-blog.csdn.net/20151209044834260)

至于Queue模块这里就不再叙述了。
下面介绍除了各种同步对象和线程对象外，threading模块还提供了一些函数。
|函数
|描述
|
|activeCount()
|当前活动的线程对象的数量
|
|currentThread()
|返回当前线程对象
|
|enumerate()
|返回当前活动线程的列表
|
|settrace(func)
|为所有线程设置一个跟踪函数
|
|setprofile(func)
|为所有线程设置一个profile函数
|
最后给出一些多线程编程中可能用得到的模块。
|模块
|描述
|
|thread
|基本的、低级别的线程模块
|
|threading
|高级别的线程和同步对象
|
|Queue
|供多线程使用的同步先进先出（FIFO）队列
|
|mutex
|互斥对象
|
|SocketServer
|具有线程控制的TCP和UDP管理器
|
总之，这篇文章主要是参考《Python核心编程》的，希望文章对你有所帮助~尤其是初学Python编程的，同时为后面我学习多线程的爬虫或分布式爬虫做铺垫。这篇文章花了自己一些时间，写到半夜；写文不易，且看且珍惜吧！勿喷~
（By:Eastmount 2015-12-09 半夜5点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


