# EPOLL内核原理极简图文解读 - LeechanX - CSDN博客
2017年06月16日 16:57:16[LeechanXBlog](https://me.csdn.net/linkedin_38454662)阅读数：2385
### 预备知识：内核poll钩子原理
#### 内核函数poll_wait
把当前进程加入到驱动里自定义的等待队列上
当驱动事件就绪后，就可以在驱动里自定义的等待队列上唤醒调用poll的进程
> 
故poll_wait作用：可以让驱动知道 事件就绪的时候唤醒哪些等待进程
#### 钩子poll
内核f_op->poll必须配合驱动自己的等待队列才能用，不然驱动有事件产生后不知道哪些进程调用了poll来等待这个事件
内核f_op->poll要做的事情
- **调用`poll_wait`，将当前进程放入驱动设备的等待队列上，这样驱动就知道哪些进程在调用`poll`等待事件**
- **检查此时立刻已有的事件（`POLLIN\POLLOUT\POLLERR......`）并返回掩码表示**
f_op->poll是一个非阻塞的操作，立即返回，返回值以掩码形式表示当前已产生的事件集合
#### 举例
snull驱动的例子： 
snull驱动有两个自定义的等待队列：
```cpp
wait_queue_head_t inq;//读取进程无数据可读时，在此队列等待
wait_queue_head_t outq;//写入进程无空间可写时，在此队列等待
```
由于snull驱动的读操作read会在读取数据后，唤醒outq队列上的写进程们 
且snull驱动的写操作write会在写入数据后，唤醒inq队列上的读进程们
而snull驱动的poll操作：
```cpp
调用poll_wait将当前进程加入到inq;
调用poll_wait将当前进程加入到outq;
查看当前有什么事件，返回掩码
```
假设某进程X调用poll，则进程X会出现在inq与outq等待队列上
之后snull写入时，由于将唤醒inq，故调用poll而进入inq的进程X被唤醒 
同理，snull读取时，由于将唤醒outq，故调用poll而进入outq的进程X被唤醒
于是，读、写事件唤醒了调用`poll`等待事件产生的进程们
#### 白话
进程调用`poll`就是希望：事件产生的时候告诉我
而事件E产生的时候，认为其对应的等待队列上的进程是等待E事件的进程，于是会唤醒此等待队列上的进程们
所以进程调用`poll`的时候，`poll`内部应该把当前进程放到合适的等待队列上 
这样事件产生的时候，调用`poll`的进程由于已经在对应等待队列上了，于是就会被唤醒
### 预备知识2：等待队列
等待队列对头：`wait_queue_head_t`
队列的成员：`wait_queue_t`
`wait_queue_t`的成员：
```cpp
void *private; /*指向进程描述符task_struct*/
wait_queue_func_t  func;//唤醒时调用此函数，即钩子函数
struct list_head  task_list;//队列链表指针
```
一般钩子函数func是内核默认函数`default_wake_function`，功能就是唤醒了进程
我们也可以在**把进程放入等待队列时主动设定钩子函数，使得在唤醒进程时自动执行我们需要的操作**
`epoll`就利用了队列钩子函数：把产生的事件内容copy到rdlist 
这样，事件来临时会自动把事件内容放到rdlist中，而不需要我们自己遍历监听句柄们查有谁产生了事件
更多的内容见[http://www.cnblogs.com/apprentice89/archive/2013/05/09/3068274.htm](http://www.cnblogs.com/apprentice89/archive/2013/05/09/3068274.htm)
#### epoll内核原理1：调用epoll_create1/epoll_create
> 
创建了epoll句柄eventpoll，返回其文件表示的描述符epfd
![](https://img-blog.csdn.net/20170616165822228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlua2VkaW5fMzg0NTQ2NjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
eventpoll内部有以下关键数据结构：
- `rbtree`：红黑树，每个被加入到epoll监控的文件事件会创建一个epitem结构，作为rbtree节点
使用rbtree的优点：可容纳大量文件事件，方便增删改（`O(lgN)`）
- `rdlist`：内核链表，用于存放当前产生了期待事件产生的文件句柄们（这里的一个文件句柄可以理解为一个epoll_event）
- `wq`：当进程调用epoll_wait等待时，进程加入等待队列wq
- `poll_wait`：eventpoll本身的等待队列，由于eventpoll自己也被当做文件，这个队列用于自己被别人调用select/poll/epoll监听的情况（一般没啥用）
> 
poll_wait在啥时候用呢：
```cpp
fd = socket(...);
efd1 = epoll_create();
efd2 = epoll_create();
epoll_ctl(efd1, EPOLL_CTL_ADD, fd, ...);
epoll_ctl(efd2, EPOLL_CTL_ADD, efd1, ...);
```
如上，efd1监控fd，而efd2监控了efd1，即嵌套的epoll监控：epoll监控另一个epoll句柄 
efd2要监控efd1，将调用efd1的poll函数 
回忆之前说过：文件f_op->poll需要配合驱动提供的等待队列 
对于epollfd，等待队列就是poll_wait 
efd2监听efd1，会调用efd1->f_op->poll，于是把当前进程放到efd1的poll_wait队列上 
在epoll的内核实现中，当efd1本身监听到fd事件产生后，会顺便唤醒poll_wait上的进程
于是，“efd1监听到事件” 被通知到efd2。这样，就实现了epollfd被其他多路复用监听了！
故：poll_wait就是用于epoll句柄被另外的多路复用监听的，配合epoll自己的f_op->poll，看起来一般用不到
#### epoll内核原理2：调用epoll_ctl操作句柄新增监控事件
epoll_ctl：`EPOLL_CTL_ADD`、`EPOLL_CTL_MOD`、`EPOLL_CTL_DEL`新增、修改、删除红黑树上的文件句柄
其中epll_ctl：`EPOLL_CTL_ADD`新增句柄不仅仅新增红黑树节点，更关键的是对文件开始监控！
> 
与`select/poll`的本质区别：并不是调用`epoll_wait`的时候才监听文件，而是`EPOLL_CTL_ADD`的时候就开始监听了
#### 详细分析`EPOLL_CTL_ADD`
##### epoll_ctl(epfd, EPOLL_CTL_ADD, fd, fdevent)核心流程：
- 
对要注册的事件`event->events`追加关心事件：
```
EPOLLERR
 | EPOLLHUP
```
> 
回忆epoll的使用中说过：EPOLLERR、EPOLLHUP事件会被自动监听，即使我们没设置
- 
创建epitem结构，加入到红黑树中
- 【关键】
```
revent
 = file->f_op->poll
```
，即调用poll，把当前进程放到文件的等待队列上且设置回调函数`ep_poll_callback`，返回值revent是文件当前已产生事件掩码
- 检查返回事件：如果revent与关心事件event->events有交集（说明ADD之前事件就准备好了） 
- 把此epitem节点拷贝到rdlist链表中；（就绪句柄拷贝到rdlist）
- 如果有进程在wq等待队列上（即有进程在调用epoll_wait等待），则唤醒之！
- 顺便，如果有进程在poll_wait等待队列上（即有进程调用多路复用来监听当前epoll句柄），则唤醒之！
> 
可以看到，如果在EPOLL_CTL_ADD一个文件之前，这个文件关心的事件就已经产生了的话，由于会唤醒wq队列上的进程，则此时EPOLL_CTL_ADD会使得epoll_wait函数从阻塞中返回
4.1~4.3的逻辑与回调函数干的事情一模一样，故图中先不画
> 
![](https://img-blog.csdn.net/20170616170928669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlua2VkaW5fMzg0NTQ2NjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
简而言之：epoll_ctl_add把当前进程注册到文件等待队列上，并设置回调函数
#### 再说回调函数干了什么
回调函数ep_poll_callback作为等待队列的回调函数： 
当文件事件来临，唤醒文件等待队列上进程，ep_poll_callback函数将被自动调用，并把已产生事件们作为其参数传入
##### 回调函数`ep_poll_callback`核心流程：
ep_poll_callback检查已产生事件与关心事件是否有交集，如果有：
- 将文件的epitem节点拷贝到rdlist链表上（就绪句柄拷贝到rdlist）
- 如果有进程在wq等待队列上（即有进程在调用epoll_wait等待），则唤醒之！
- 顺便，如果有进程在poll_wait等待队列上（即有进程调用多路复用来监听当前epoll句柄），则唤醒之！
> 
简而言之：回调函数把文件句柄拷贝到rdlist，并唤醒epoll_wait等待的进程
#### epoll内核原理3：当文件有事件来临时：
- 对应的等待队列上的进程被唤醒，执行回调函数ep_poll_callback，并把已产生事件们以参数传入
- call ep_poll_callback
:![](https://img-blog.csdn.net/20170616170832747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlua2VkaW5fMzg0NTQ2NjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 
简而言之：事件发生时，文件句柄被自动拷贝到rdlist，调用epoll_wait等待的进程们被唤醒
#### epol内核原理4：调用`epoll_wait`等待事件
epoll_wait并不监听文件句柄，而是等待rdlist不空 or 收到信号 or 超时这三种条件后返回
伪代码：
```cpp
epoll_wait(epfd, events, MAXSIZE, timeout)
    res = 0
    jitimeout = 剩余时间，timeout换算为内核时间
while rdlist为空：
        当前进程放到等待队列wq中；
while 1:
            如果rdlist不空，或者jitimeout = 0超时
break
            如果有信号挂起
                res = EINTR
break
            jitimeout = schedule_timeout(jitimeout)
            让出CPU，唤醒后返回新的剩余时间
    如果res = 0，说明rdlist不空 or 超时了
        则把rdlist中句柄们调用ep_send_events函数拷贝到events数组中，返回拷贝了几个句柄，赋值给res
return res;
//res = -1，收到信号；
//res = 0，超时
//res > 0, 有res个句柄拷贝到events数组
```
##### 主要逻辑：
- 不断让出CPU，直到： 
- rdlist有数据
- 超时
- 收到信号
- 如果rdlist有数据，则拷贝到用户传入的events数组
![](https://img-blog.csdn.net/20170616170848429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlua2VkaW5fMzg0NTQ2NjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 
简而言之：等待rdlist不空或者超时、信号中断，rdlist不空则把句柄们拷贝到用户空间
#### PS：拷贝到用户这个环节看边缘触发与水平触发的区别
拷贝句柄函数`ep_send_events`会先遍历rdlist中每个句柄，对于每个句柄，再次调用poll获取实际事件：
- 
如果与关心事件有交集： 
如果句柄是水平触发（`EPOLLLT`），则再次把句柄加入到rdlist；否则从rdlist中删除
> 
于是水平模式下次还会准备好，这就是EPOLLET 与 EPOLLLT的区别原理
- 
如果与关心事件无交集，从rdlist中删除之
> 
问题：如此一来看起来水平模式的句柄永远都不断重新加入rdlist，这就成永远都通知了吧？
当事件已经被处理完后，调用poll得到的实际事件与关心事件已经无交集了，于是会被删除的！
ep_send_events函数内再次调用poll获取实际事件就是为了EPOLLLT模式而生的，防止其永远加入rdlist！
于是，EPOLLLT读事件 做到了只要有数据就不停通知，直到没数据就不再通知了
