# 深入浅出Python中的异步与协程 - Machine Learning with Peppa - CSDN博客





2018年04月02日 23:20:30[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：109标签：[Python																[服务器																[网络																[多线程](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[***系统与网络***																[编程之美：Python](https://blog.csdn.net/qq_39521554/article/category/7392111)](https://blog.csdn.net/qq_39521554/article/category/7361345)





## 协程

协程，又称微线程，纤程。英文名Coroutine。一句话说明什么是线程：协程是一种用户态的轻量级线程。实现单线程的并发。

协程拥有自己的寄存器上下文和栈。协程调度切换时，将寄存器上下文和栈保存到其他地方，在切回来的时候，恢复先前保存的寄存器上下文和栈。因此：

协程能保留上一次调用时的状态（即所有局部状态的一个特定组合），每次过程重入时，就相当于进入上一次调用的状态，换种说法：进入上一次离开时所处逻辑流的位置。



协程的好处：
- 无需线程上下文切换的开销
- 无需原子操作锁定及同步的开销（协程本身就是单线程，单线程操作是原子的不需要加锁）- 　　"原子操作(atomic operation)是不需要synchronized"，所谓原子操作是指不会被线程调度机制打断的操作；这种操作一旦开始，就一直运行到结束，中间不会有任何 context switch （切换到另一个线程）。原子操作可以是一个步骤，也可以是多个操作步骤，但是其顺序是不可以被打乱，或者切割掉只执行部分。视作整体是原子性的核心。

- 方便切换控制流，简化编程模型
- 高并发+高扩展性+低成本：一个CPU支持上万的协程都不是问题。所以很适合用于高并发处理。

缺点：
- 无法利用多核资源：协程的本质是个单线程,它不能同时将 单个CPU 的多个核用上,协程需要和进程配合才能运行在多CPU上.当然我们日常所编写的绝大部分应用都没有这个必要，除非是cpu密集型应用。
- 进行阻塞（Blocking）操作（如IO时）会阻塞掉整个程序

协程如何实现在单线程里面多并发的效果？ 其核心就是遇到 IO操作就切换，那么怎么知道 IO 操作已完成，自动切换回去呢？回调





协程的实现方式：

yield实现协程

greenlet是一个用C实现的协程模块，相比与python自带的yield，它可以使你在任意函数之间随意切换，而不需把这个函数先声明为generator

Gevent 是一个第三方库，可以轻松通过gevent实现并发同步或异步编程，在gevent中用到的主要模式是Greenlet, 它是以C扩展模块形式接入Python的轻量级协程。 Greenlet全部运行在主程序操作系统进程的内部，但它们被协作式地调度



gevent的使用：

join方法：等待协程执行完在退出，和多线程类似

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
from gevent import monkey
import time

monkey.patch_all()
import gevent
from  urllib.request import urlopen


def f(url):
    print('GET: %s' % url)
    resp = urlopen(url)
    data = resp.read()
    print('%d bytes received from %s.' % (len(data), url))

start_time_1 = time.time()

i = 'http://www.cnblogs.com/alex3714/articles/5248247.html'
a= gevent.spawn(f,i)
a.join()    #等待gevent执行完毕，否则程序直接退出等不到结果。不加join（） 程序直接退出和多线程类似


print("cost is %s..." % str(time.time() - start_time_1))

#执行结果：
GET: http://www.cnblogs.com/alex3714/articles/5248247.html
92210 bytes received from http://www.cnblogs.com/alex3714/articles/5248247.html.
cost is 9.593548774719238...

#不加join的结果：
cost is 0.003000020980834961...




![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
#遇到IO阻塞时会自动切换任务

from gevent import monkey
import time

monkey.patch_all()
import gevent
from  urllib.request import urlopen


def f(url):
    print('GET: %s' % url)
    resp = urlopen(url)
    data = resp.read()
    print('%d bytes received from %s.' % (len(data), url))

start_time = time.time()
gevent.joinall([                           
    gevent.spawn(f, 'http://www.cnblogs.com/alex3714/articles/5248247.html'),
    gevent.spawn(f, 'https://github.com/'),
])

print("cost is %s..." % str(time.time() - start_time))

执行结果：
GET: http://www.cnblogs.com/alex3714/articles/5248247.html
GET: https://github.com/
51594 bytes received from https://github.com/.
92210 bytes received from http://www.cnblogs.com/alex3714/articles/5248247.html.
cost is 9.431539297103882...

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

不加join方法，如何让协程不退出？
`![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View Code`


### 论事件驱动与异步IO

通常，我们写服务器处理模型的程序时，有以下几种模型：

（1）每收到一个请求，创建一个新的进程，来处理该请求；

（2）每收到一个请求，创建一个新的线程，来处理该请求；

（3）每收到一个请求，放入一个事件列表，让主进程通过非阻塞I/O方式来处理请求，即协程；

上面的几种方式，各有千秋，

第（1）中方法，由于创建新的进程的开销比较大，所以，会导致服务器性能比较差,但实现比较简单。

第（2）种方式，由于要涉及到线程的同步，有可能会面临[死锁](http://baike.baidu.com/subview/121723/121723.htm)等问题。

第（3）种方式，在写应用程序代码时，逻辑比前面两种都复杂。

综合考虑各方面因素，一般普遍认为第（3）种方式是大多数[网络服务器](http://baike.baidu.com/view/813.htm)采用的方式 ，如nginx



事件驱动模型

类似生产者消费者模型：
目前大部分的UI编程都是事件驱动模型，如很多UI平台都会提供onClick()事件，这个事件就代表鼠标按下事件。事件驱动模型大体思路如下：
1. 有一个事件（消息）队列；
2. 鼠标按下时，往这个队列中增加一个点击事件（消息）；
3. 有个循环，不断从队列取出事件，根据不同的事件，调用不同的函数，如onClick()、onKeyDown()等；
4. 事件（消息）一般都各自保存各自的处理函数指针，这样，每个消息都有独立的处理函数；



上面的事件驱动模型中，只要一遇到IO就注册一个事件，然后主程序就可以继续干其它的事情了，只到io处理完毕后，继续恢复之前中断的任务，这本质上是怎么实现的呢？



IO 多路复用

同步IO和异步IO，阻塞IO和非阻塞IO分别是什么，到底有什么区别？


#### 缓存 I/O

缓存 I/O 又被称作标准 I/O，大多数文件系统的默认 I/O 操作都是缓存 I/O。在 Linux 的缓存 I/O 机制中，操作系统会将 I/O 的数据缓存在文件系统的页缓存（ page cache ）中，也就是说，数据会先被拷贝到操作系统内核的缓冲区中，然后才会从操作系统内核的缓冲区拷贝到应用程序的地址空间。

缓存 I/O 的缺点：
数据在传输过程中需要在应用程序地址空间和内核进行多次数据拷贝操作，这些数据拷贝操作所带来的 CPU 以及内存开销是非常大的。



### 二 IO模式

刚才说了，对于一次IO访问（以read举例），数据会先被拷贝到操作系统内核的缓冲区中，然后才会从操作系统内核的缓冲区拷贝到应用程序的地址空间。所以说，当一个read操作发生时，它会经历两个阶段：
1. 等待内核中数据准备 (Waiting for the data to be ready)
2. 将数据从内核拷贝到进程中 (Copying the data from the kernel to the process)

正式因为这两个阶段，linux系统产生了下面五种网络模式的方案。
- 阻塞 I/O（blocking IO）
- 非阻塞 I/O（nonblocking IO）
- I/O 多路复用（ IO multiplexing）
- 信号驱动 I/O（ signal driven IO）
- 异步 I/O（asynchronous IO）

#### 阻塞 I/O（blocking IO）

blocking IO的特点就是在IO执行的两个阶段都被block了。

#### 非阻塞 I/O（nonblocking IO）

当用户进程发出read操作时，如果kernel中的数据还没有准备好，那么它并不会block用户进程，而是立刻返回一个error。用户进程判断结果是一个error时，它就知道数据还没有准备好，于是它做点其他事，一会可以再次发送read操作。一旦kernel中的数据准备好了，并且又再次收到了用户进程的system call，那么它马上就将数据拷贝到了用户内存，然后返回。

> 
所以，nonblocking IO的特点是用户进程需要不断的主动询问kernel数据好了没有。


#### I/O 多路复用（ IO multiplexing）

IO multiplexing就是我们说的select，poll，epoll，有些地方也称这种IO方式为event driven IO。select/epoll的好处就在于单个process就可以同时处理多个网络连接的IO。它的基本原理就是select，poll，epoll这个function会不断的轮询所负责的所有socket，当某个socket有数据到达了，就通知用户进程。



`当用户进程调用了select，那么整个进程会被block`，而同时，kernel会“监视”所有select负责的socket，当任何一个socket中的数据准备好了，select就会返回。这个时候用户进程再调用read操作，将数据从kernel拷贝到用户进程。



> 
所以，I/O 多路复用的特点是通过一种机制一个进程能同时等待多个文件描述符，而这些文件描述符（套接字描述符）其中的任意一个进入读就绪状态，select()函数就可以返回。 


#### 异步 I/O（asynchronous IO）

用户进程发起read操作之后，立刻就可以开始去做其它的事。而另一方面，从kernel的角度，当它受到一个asynchronous read之后，首先它会立刻返回，所以不会对用户进程产生任何block。然后，kernel会等待数据准备完成，然后将数据拷贝到用户内存，当这一切都完成之后，kernel会给用户进程发送一个signal，告诉它read操作完成了。



#### blocking和non-blocking的区别

调用blocking IO会一直block住对应的进程直到操作完成，而non-blocking IO在kernel还准备数据的情况下会立刻返回。

有人会说，non-blocking IO并没有被block啊。这里有个非常“狡猾”的地方，定义中所指的”IO operation”是指真实的IO操作，就是例子中的recvfrom这个system call。non-blocking IO在执行recvfrom这个system call的时候，如果kernel的数据没有准备好，这时候不会block进程。但是，当kernel中数据准备好的时候，recvfrom会将数据从kernel拷贝到用户内存中，这个时候进程是被block了，在这段时间内，进程是被block的。

#### synchronous IO和asynchronous IO的区别

两者的区别就在于synchronous IO做”IO operation”的时候会将process阻塞。按照这个定义，之前所述的blocking IO，non-blocking IO，IO multiplexing都属于synchronous IO。

而asynchronous IO则不一样，当进程发起IO 操作之后，就直接返回再也不理睬了，直到kernel发送一个信号，告诉进程说IO完成。在这整个过程中，进程完全没有被block。



#### Select\Poll\Epoll （ I/O 多路复用 IO multiplexing）

select的一个缺点在于单个进程能够监视的文件描述符的数量存在最大限制，在Linux上一般为1024,即默认只监视1024 个socket连接。在用户设置里修改宏定义可更改最大文件描述符数


poll在1986年诞生于System V Release 3，它和select在本质上没有多大差别，但是poll没有最大文件描述符数量的限制。白话理解为 ，当select()，poll监控1000个socket连接，有数据返回时，用户去取数据但会对所有socket进行一次线性扫描，所以这也浪费了一定的开销。


epoll 直到Linux2.6才出现了由内核直接支持的实现方法，那就是epoll，它几乎具备了之前所说的一切优点，被公认为Linux2.6下性能最好的多路I/O就绪通知方法。白话理解为，当监控1000个socket连接。有2个活跃的连接有数据，就告诉用户这两连接有数据，让用户扫描只扫描这两个连接来取数据。

I/O 多路复用要有用户取数据的过程，虽然没有异步IO方式好。但是I/O 多路复用是最常用的方式如 nginx，trnoado，twisted。 python3中有一个新的模块(asyncio) 支持异步IO,但是不多数场合用不到。](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)




