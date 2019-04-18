# 自己动手实现Epoll - qq_42302962的博客 - CSDN博客
2018年05月30日 09:11:50[Linux高级开发](https://me.csdn.net/qq_42302962)阅读数：234
Epoll是Linux IO多路复用的管理机制。作为现在Linux平台高性能网络IO必要的组件。内核的实现可以参照：fs/eventpoll.c .
为什么需要自己实现epoll呢？现在自己打算做一个用户态的协议栈。采用单线程的模式。[https://github.com/wangbojing/NtyTcp](https://github.com/wangbojing/NtyTcp)，至于为什么要实现用户态协议栈？可以自行百度C10M的问题。
由于协议栈做到了用户态故需要自己实现高性能网络IO的管理。所以epoll就自己实现一下。代码：[https://github.com/wangbojing/NtyTcp/blob/master/src/nty_epoll_rb.c](https://github.com/wangbojing/NtyTcp/blob/master/src/nty_epoll_rb.c)
在实现epoll之前，先得好好理解内核epoll的运行原理。内核的epoll可以从四方面来理解。
1.Epoll的数据结构，rbtree对<fd, event>的存储，ready队列存储就绪io。
2.Epoll的线程安全，SMP的运行，以及防止死锁。
3.Epoll内核回调。
4.Epoll的LT（水平触发）与ET（边沿触发）
下面从这四个方面来实现epoll。
## 一、Epoll数据结构
Epoll主要由两个结构体：eventpoll与epitem。Epitem是每一个IO所对应的的事件。比如 epoll_ctl EPOLL_CTL_ADD操作的时候，就需要创建一个epitem。Eventpoll是每一个epoll所对应的的。比如epoll_create 就是创建一个eventpoll。
Epitem的定义
![epitem.png](http://s1.51cto.com/images/20180325/1521963156677386.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
Eventpoll的定义
![eventpoll.png](http://s1.51cto.com/images/20180325/1521963182898556.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
数据结构如下图所示。
![data_struct.png](http://s1.51cto.com/images/20180325/1521963216966024.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
List 用来存储准备就绪的IO。对于数据结构主要讨论两方面：insert与remove。同样如此，对于list我们也讨论insert与remove。何时将数据插入到list中呢？当内核IO准备就绪的时候，则会执行epoll_event_callback的回调函数，将epitem添加到list中。
那何时删除list中的数据呢？当epoll_wait激活重新运行的时候，将list的epitem逐一copy到events参数中。
Rbtree用来存储所有io的数据，方便快速通io_fd查找。也从insert与remove来讨论。
对于rbtree何时添加：当App执行epoll_ctl EPOLL_CTL_ADD操作，将epitem添加到rbtree中。何时删除呢？当App执行epoll_ctl EPOLL_CTL_DEL操作，将epitem添加到rbtree中。
List与rbtree的操作又如何做到线程安全，SMP，防止死锁呢？
## 二、Epoll锁机制
Epoll 从以下几个方面是需要加锁保护的。List的操作，rbtree的操作，epoll_wait的等待。
List使用最小粒度的锁spinlock，便于在SMP下添加操作的时候，能够快速操作list。
List添加
![spinlock_insert.png](http://s1.51cto.com/images/20180325/1521963235904885.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
346行：获取spinlock。
347行：epitem 的rdy置为1，代表epitem已经在就绪队列中，后续再触发相同事件就只需更改event。
348行：添加到list中。
349行：将eventpoll的rdnum域加1。
350行：释放spinlock
List删除
![spinlock_remove.png](http://s1.51cto.com/images/20180325/1521963256969795.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
301行：获取spinlock
304行：判读rdnum与maxevents的大小，避免event溢出。
307行：循环遍历list，判断添加list不能为空
309行：获取list首个结点
310行：移除list首个结点。
311行：将epitem的rdy域置为0，标识epitem不再就绪队列中。
313行：copy epitem的event到用户空间的events。
316行：copy数量加1
317行：eventpoll中rdnum减一。
避免SMP体系下，多核竞争。此处采用自旋锁，不适合采用睡眠锁。
Rbtree的添加
![rbtree_insert.png](http://s1.51cto.com/images/20180325/1521963277949683.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
149行：获取互斥锁。
153行：查找sockid的epitem是否存在。存在则不能添加，不存在则可以添加。
160行：分配epitem。
167行：sockid赋值
168行：将设置的event添加到epitem的event域。
170行：将epitem添加到rbrtree中。
173行：释放互斥锁。
Rbtree删除：
![rbtree_remove.png](http://s1.51cto.com/images/20180325/1521963292101784.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
177行：获取互斥锁。
181行：删除sockid的结点，如果不存在，则rbtree返回-1。
188行：释放epitem
190行：释放互斥锁。
Epoll_wait的挂起。
采用pthread_cond_wait，具体实现可以参照。
[https://github.com/wangbojing/NtyTcp/blob/master/src/nty_epoll_rb.c](https://github.com/wangbojing/NtyTcp/blob/master/src/nty_epoll_rb.c)
## 三、Epoll回调
Epoll 的回调函数何时执行，此部分需要与Tcp的协议栈一起来阐述。Tcp协议栈的时序图如下图所示，epoll从协议栈回调的部分从下图的编号1,2,3,4。具体Tcp协议栈的实现，后续从另外的文章中表述出来。下面分别对四个步骤详细描述
编号1：是tcp三次握手，对端反馈ack后，socket进入rcvd状态。需要将监听socket的event置为EPOLLIN，此时标识可以进入到accept读取socket数据。
编号2：在established状态，收到数据以后，需要将socket的event置为EPOLLIN状态。
编号3：在established状态，收到fin时，此时socket进入到close_wait。需要socket的event置为EPOLLIN。读取断开信息。
编号4：检测socket的send状态，如果对端cwnd>0是可以，发送的数据。故需要将socket置为EPOLLOUT。
所以在此四处添加EPOLL的回调函数，即可使得epoll正常接收到io事件。
![QQ截图20180325152626.png](http://s1.51cto.com/images/20180325/1521963328184050.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![](http://blog.51cto.com/static/js/ueditor1.4.3/themes/default/images/spacer.gif)![](http://blog.51cto.com/static/js/ueditor1.4.3/themes/default/images/spacer.gif)![](http://blog.51cto.com/static/js/ueditor1.4.3/themes/default/images/spacer.gif)
## 四、LT与ET
LT（水平触发）与ET（边沿触发）是电子信号里面的概念。不清楚可以man epoll查看的。如下图所示：
![lt_et.png](http://s1.51cto.com/images/20180325/1521963344257598.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
比如：event = EPOLLIN | EPOLLLT，将event设置为EPOLLIN与水平触发。只要event为EPOLLIN时就能不断调用epoll回调函数。
比如: event = EPOLLIN | EPOLLET，event如果从EPOLLOUT变化为EPOLLIN的时候，就会触发。在此情形下，变化只发生一次，故只调用一次epoll回调函数。关于水平触发与边沿触发放在epoll回调函数执行的时候，如果为EPOLLET（边沿触发），与之前的event对比，如果发生改变则调用epoll回调函数，如果为EPOLLLT（水平触发），则查看event是否为EPOLLIN，即可调用epoll回调函数。
