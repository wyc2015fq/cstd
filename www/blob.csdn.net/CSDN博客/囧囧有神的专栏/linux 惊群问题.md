# linux 惊群问题 - 囧囧有神的专栏 - CSDN博客





2015年01月31日 22:29:11[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：8991









# 1. 结论

对于惊群的资料，网上特别多，良莠不齐，也不全面。看的时候，有的资料说，惊群已经解决了，有的资料说，惊群还没解决。。 哪个才是对的？！  一怒之下，在研究各种公开资料的基础上，特意查对了linux源码，总结了此文。希望对有需要的人略有帮助，希望各位大神轻拍，如有错漏，不吝指教，感激不尽。（814329735@qq.com）


先说结论吧：

1. Linux多进程accept系统调用的惊群问题（注意，这里没有使用select、epoll等事件机制），在linux 2.6版本之前的版本存在，在之后的版本中解决掉了。

2. 使用select epoll等事件机制，在linux早期的版本中，惊群问题依然存在（epoll_create在fork之前）。 原因与之前单纯使用accept导致惊群，原因类似。Epoll的惊群问题，同样在之后的某个版本部分解决了。

3. Epoll_create在fork之后调用，不能避免惊群问题，Nginx使用互斥锁，解决epoll惊群问题。



备注：

1.  本文的几个示例程序，跑在内核3.8.0-35-generic上， 复制的内核代码epoll部分，来自内核版本3.6.   这两个版本，epoll没有重大变化，因此，试验时，我认为他们是一样的。

2. Epoll的惊群问题（epoll_create在fork之前），在某个版本被修复。我没有去查证是在哪个版本被修复的，但是我试验平台内核3.8， 和我看的代码版本3.6, 都是解决了。而在2.6.1版本是存在的

3. 惊群问题出现在多进程 多线程之上。为了简便，所有的测试程序用的多进程模型。



# 2. 惊群是什么

在unix/linux历史上有一个问题，惊群（thundering herd）

惊群是指多个进程/线程在等待同一资源时，每当资源可用，所有的进程/线程都来竞争资源的现象。

让一个进程bind一个网络地址(可能是AF_INET，AF_UNIX或者其他任何你想要的)，然后fork这个进程自己：




```cpp
int s = socket(...)

bind(s, ...)

listen(s, ...)

fork()
```





Fork自己几次之后，每个进程阻塞在accept()函数这里

```cpp
for(;;) {

    int client = accept(...);  //子进程阻塞在这了

    if (client < 0) continue;

    ...

}
```



在较老的unix系统中，当有连接到来时，accept()在每个阻塞在这的进程里被唤醒。

但是，只有这些进程中的一个能够真正的accept这个连接，其他的进程accept将返回EAGAIN

惊群造成的结果是系统对用户进程/线程频繁的做无效的调度、上下文切换，系统系能大打折扣。


# 3. Linux内核解决惊群的方案

在linux 2.6版本之前，监听同一个socket的进程会挂在一个等待队列上，当请求到来时，会唤醒所有等待的子进程。

当时可以使用锁解决这种惊群问题。

代码类似如下：

```cpp
for(;;) {

    lock();// 互斥锁

    int client = accept(...);

    unlock();

    if (client < 0) continue;

    ...

}
```





在linux 2.6版本之后，通过引入一个标记位，解决掉了惊群问题。

测试程序fork了两个子进程，accept为阻塞模式，监听同一个socket。

（测试程序源码test.c 在本文档第6章节中）

当客户端connect这个socket时，显然只应该有一个进程accept成功，哪个子进程会accept成功呢？



以下是测试平台和结果：

系统版本：

```cpp
$ uname -a

Linux liujiyong 3.8.0-35-generic #50-Ubuntu SMP Tue Dec 3 01:24:59 UTC 2013 x86_64 x86_64 x86_64 GNU/Linux
```



运行结果：

![](https://img-blog.csdn.net/20150131220750583?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




实验结果显示，在linux内核版本3.8中，每次只有一个进程唤醒，已经解决掉了惊群问题。



我们从源码中来看看进程是如何解决这个问题的？

首先我们知道当accept的时候，如果没有连接则会一直阻塞(没有设置非阻塞),而阻塞代码是在inet_csk_wait_for_connect中，我们来看代码片断： 

```cpp
struct sock *inet_csk_accept(struct sock *sk, int flags, int *err)// accept的原型函数

{

...

error = inet_csk_wait_for_connect(sk, timeo); // 等待连接

...

}

static int inet_csk_wait_for_connect(struct sock *sk, long timeo)

{

...

/*

 * True wake-one mechanism for incoming connections: only

 * one process gets woken up, not the 'whole herd'.

 * Since we do not 'race & poll' for established sockets

 * anymore, the common case will execute the loop only once.

 *

 * Subtle issue: "add_wait_queue_exclusive()" will be added

 * after any current non-exclusive waiters, and we know that

 * it will always _stay_ after any new non-exclusive waiters

 * because all non-exclusive waiters are added at the

 * beginning of the wait-queue. As such, it's ok to "drop"

 * our exclusiveness temporarily when we get woken up without

 * having to remove and re-insert us on the wait queue.

 */

for (;;) {

// 以上英文注释已经说清楚了，只有一个进程会唤醒

// 非exclusive的元素会加在等待队列前头，exclusive的元素会加在所有非exclusive元素的后头

prepare_to_wait_exclusive(sk_sleep(sk), &wait,

  TASK_INTERRUPTIBLE);

}

...

}

void

prepare_to_wait_exclusive(wait_queue_head_t *q, wait_queue_t *wait, int state)

{

unsigned long flags;

///设置等待队列的flag为EXCLUSIVE，设置这个就是表示一次只会有一个进程被唤醒，我们等会就会看到这个标记的作用。

wait->flags |= WQ_FLAG_EXCLUSIVE; //注意这个标志，唤醒的阶段会使用这个标志

spin_lock_irqsave(&q->lock, flags);

if (list_empty(&wait->task_list))

//  加入等待队列

__add_wait_queue_tail(q, wait);

set_current_state(state);

spin_unlock_irqrestore(&q->lock, flags);

}
```





以上时accept的实现，我们继续来看唤醒的部分

当有tcp连接完成，就会从半连接队列拷贝sock到连接队列，这个时候我们就可以唤醒阻塞的accept了。ok，我们来看关键的代码，首先是tcp_v4_do_rcv： 

```cpp
int tcp_v4_do_rcv(struct sock *sk, struct sk_buff *skb)

{

...

if (sk->sk_state == TCP_LISTEN) {

struct sock *nsk = tcp_v4_hnd_req(sk, skb);

if (!nsk)

goto discard;

if (nsk != sk) {

sock_rps_save_rxhash(nsk, skb);

if (tcp_child_process(sk, nsk, skb)) { // 关注这个函数

rsk = nsk;

goto reset;

}

return 0;

}

}

...

}

int tcp_child_process(struct sock *parent, struct sock *child,

      struct sk_buff *skb)

{

...

if (!sock_owned_by_user(child)) {

ret = tcp_rcv_state_process(child, skb, tcp_hdr(skb),

    skb->len);

/* Wakeup parent, send SIGIO 唤醒父进程*/

if (state == TCP_SYN_RECV && child->sk_state != state)

parent->sk_data_ready(parent, 0); // 通知父进程

}

... 

}
```



调用sk_data_ready通知父socket，查阅资料我们知道tcp中这个函数是sock_def_readable。而这个函数会调用wake_up_interruptible_sync_poll来唤醒队列

```cpp
#define wake_up_interruptible_sync_poll(x, m) \

__wake_up_sync_key((x), TASK_INTERRUPTIBLE, 1, (void *) (m))

void __wake_up_sync_key(wait_queue_head_t *q, unsigned int mode,

int nr_exclusive, void *key)

{

...

__wake_up_common(q, mode, nr_exclusive, wake_flags, key);

spin_unlock_irqrestore(&q->lock, flags);

...

}

// nr_exclusive是1

static void __wake_up_common(wait_queue_head_t *q, unsigned int mode,

int nr_exclusive, int wake_flags, void *key)

{

wait_queue_t *curr, *next;

list_for_each_entry_safe(curr, next, &q->task_list, task_list) {

unsigned flags = curr->flags;

if (curr->func(curr, mode, wake_flags, key) &&

(flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)

break;

}

}
```



 curr->func(curr, mode, wake_flags, key) 是注册函数的执行

(flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)

传进来的nr_exclusive是1, 所以flags & WQ_FLAG_EXCLUSIVE为真的时候，执行一次，就会跳出循环。  我们记得accept的时候，加到等待队列的元素就是WQ_FLAG_EXCLUSIVE的





# 4. Epoll为什么还有惊群

在使用epoll poll select kqueue等事件机制后，

子进程进程处理连接事件程序更复杂，类似如下：

```cpp
for(;;) {

    int interesting_fd = wait_for_fds();

    if (fd_need_accept(interesting_fd)) {

        int client = accept(interesting_fd, ...);

        if (client < 0) continue;

    }

    else if (fd_is_a_signal(interesting_fd)) {

        manage_uwsgi_signal(interesting_fd);

    }

    ...

}
```



wait_for_fds函数：它可能是select(), poll(),或者kqueue(),epoll()

我们以epoll为例

讨论epoll的惊群的时候，我们需要区分两种情况

Epoll_create在fork之前创建

Epoll_create 在fork之后创建

下面分别讨论

## Epoll_create()在fork子进程之前

Epoll_create()在fork子进程之前, 所有子进程共享epoll_create()创建的epfd。

这种问题出现的惊群，与之前accept惊群的原因类似，当有事件发生时，等待同一个文件描述符的所有进程/线程，都将被唤醒。



为什么需要全部唤醒？有的资料是这么说的

因为内核不知道，你是否在等待文件描述符来调用accept()函数，还是做其他事情（信号处理，定时事件）



Epoll部分修复了惊群问题

与accept惊群的解决类似，epoll后来的版本（具体哪个版本，有待考证），修复了这个问题。

测试结果（源码文件test2.c在本文档第6章节中）：

![](https://img-blog.csdn.net/20150131220833549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




可以看到并没有惊群现象发生，每次只唤醒一个进程。



我们来看epoll是如何解决这个问题的，解决思路与accept的一致



下面是epoll_wait的逻辑

```cpp
/*

 * Implement the event wait interface for the eventpoll file. It is the kernel

 * part of the user space epoll_wait(2).

 */

SYSCALL_DEFINE4(epoll_wait, int, epfd, struct epoll_event __user *, events,

int, maxevents, int, timeout)

{

...

/* Time to fish for events ... */

error = ep_poll(ep, events, maxevents, timeout);

...

}

static int ep_poll(struct eventpoll *ep, struct epoll_event __user *events,

           int maxevents, long timeout)

{

....

        init_waitqueue_entry(&wait, current);

// 是不是很眼熟！！！！ Exclusive !! 将exclusive的元素加入到等待队列队尾

        __add_wait_queue_exclusive(&ep->wq, &wait); // **NOTICE**

for (;;) {

//  如果事件队列不为空，就跳出循环，返回了

if (ep_events_available(ep) || timed_out)

break;

       ....

       // 如果事件队列为空，就睡觉了， 除非中途被唤醒

if (!schedule_hrtimeout_range(to, slack, HRTIMER_MODE_ABS))

timed_out = 1;

}

....

}

 __add_wait_queue_exclusive(wait_queue_head_t *q, wait_queue_t *wait)

{

// 设置标记为WQ_FLAG_EXCLUSIVE，并加入队尾

        wait->flags |= WQ_FLAG_EXCLUSIVE;

        __add_wait_queue(q, wait);

}
```





唤醒的程序在回调函数ep_poll_callback中，当设备就绪，ep_poll_callback就会被调用



```
static int ep_poll_callback(wait_queue_t *wait, unsigned mode, int sync, void *key)

{

....

if (waitqueue_active(&ep->wq))

wake_up_locked(&ep->wq);

....

}

#define wake_up_locked(x) __wake_up_locked((x), TASK_NORMAL, 1)

void __wake_up_locked(wait_queue_head_t *q, unsigned int mode, int nr)

{

__wake_up_common(q, mode, nr, 0, NULL);

}

//  __wake_up_common函数还记得吗？？传进来的nr_exclusive是1

static void __wake_up_common(wait_queue_head_t *q, unsigned int mode,

int nr_exclusive, int wake_flags, void *key)

{

wait_queue_t *curr, *next;

list_for_each_entry_safe(curr, next, &q->task_list, task_list) {

unsigned flags = curr->flags;

if (curr->func(curr, mode, wake_flags, key) &&

(flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)

break;

}

}
```



 curr->func(curr, mode, wake_flags, key) 是注册函数的执行

(flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)

传进来的nr_exclusive是1, 所以flags & WQ_FLAG_EXCLUSIVE为真的时候，执行一次，就会跳出循环。



## Epoll_create()在fork子进程之后

epoll_create()在Fork之前还是之后，有神马区别呢？

Fork之前epoll_create的话，所有进程共享一个epoll红黑数。

如果我们只需要处理accept事件的话，貌似世界一片美好了。但是，epoll并不是只处理accept事件，accept后续的读写事件都需要处理，还有定时或者信号事件。

当连接到来时，我们需要选择一个进程来accept，这个时候，任何一个accept都是可以的。当连接建立以后，后续的读写事件，却与进程有了关联。一个请求与a进程建立连接后，后续的读写也应该由a进程来做。

当读写事件发生时，应该通知哪个进程呢？Epoll并不知道，因此，事件有可能错误通知另一个进程，这是不对的。实验中观察到了这种现象

（测试源码文件test2.c在本文档第6章节中）



![](https://img-blog.csdn.net/20150131220817540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



动作：  连接  连接 连接 发送数据0 发送数据1 发送数据2

但是从输出来看，建立连接2 建立连接0 建立连接1 处理数据2 处理数据0 处理数据1



因此，我们使用epoll_create()在fork之后创建，每个进程的读写事件，只注册在自己进程的epoll中。

再次试验（源码文件test1.c在本文档第6章节中）

![](https://img-blog.csdn.net/20150131221221808?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




如预期，处理数据阶段，每个进程正确处理了自己的数据。Accept阶段，出现了惊群。

欢迎回到“惊群”问题！！！



我们知道epoll对惊群的修复，是建立在共享在同一个epoll结构上的。Epoll_create在fork之后执行，每个进程有单独的epoll 红黑树，等待队列，ready事件列表。因此，惊群再次出现了。







试验中，我们发现，有时候唤醒所有进程，有时候唤醒部分进程，为什么？

有部分资料说，因为事件已经被某些进程处理掉了，因此不用在通知另外还未通知到的进程了。并未看到代码的，有待确证。






# 5. Nginx使用epoll，如何解决惊群

## Nginx采用互斥锁

Nginx是在fork之后，再epoll_create的。 

类似于这样

```cpp
lock()

epoll_wait(...);

accept(...);

unlock(...);
```



网上相关资料很多，源码阅读也并不困难，不再赘述




[http://blog.csdn.net/russell_tao/article/details/7204260](http://blog.csdn.net/russell_tao/article/details/7204260)







# 6. 测试源码

## Test.c

```cpp
#include <sys/types.h>

#include <sys/socket.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <stdio.h>

#include <stdlib.h>

#include <errno.h>

#include <strings.h>

#define SERV_PORT  9999

int main(int argc,char **argv)

{

     int listenfd,connfd;

     pid_t  childpid,childpid2;

     socklen_t clilen;

     struct sockaddr_in cliaddr,servaddr;

    

     listenfd = socket(AF_INET,SOCK_STREAM,0);

     bzero(&servaddr,sizeof(servaddr));

     servaddr.sin_family = AF_INET;

     servaddr.sin_addr.s_addr = htonl (INADDR_ANY);

     servaddr.sin_port = htons (SERV_PORT);

     bind(listenfd,  (struct sockaddr *) &servaddr, sizeof(servaddr));

 listen(listenfd,1000);

     clilen = sizeof(cliaddr);

     if( (childpid = fork()) == 0)

     {

         while(1)

         {

             connfd = accept(listenfd,(struct sockaddr *) &cliaddr,&clilen);

             printf("fork 1 is [%d],error is %m\n",connfd);

         }

     }

     if( (childpid2 = fork()) == 0)

     {

         while(1){

             connfd = accept(listenfd,(struct sockaddr *) &cliaddr,&clilen);

             printf("fork 2 is [%d]，error is %m\n",connfd);

         }

     }

     sleep(100);

     return 1;

}
```



## Test1.c

```
#include <sys/socket.h>

#include <sys/types.h>

#include <sys/epoll.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <fcntl.h>

#include <unistd.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <pthread.h>

#include <errno.h>

#define MAXLINE 100

#define OPEN_MAX 100

#define LISTENQ 20

#define SERV_PORT 8888

#define INFTIM 1000

//用于读写两个的两个方面传递参数

struct user_data

{

int fd;

unsigned int n_size;

char line[MAXLINE];

};

//声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件

struct epoll_event ev, events[20];

int epfd;

pthread_mutex_t mutex;

pthread_cond_t cond1;

struct task *readhead = NULL, *readtail = NULL, *writehead = NULL;

int i, 

maxi, 

listenfd, 

connfd, 

sockfd, 

nfds;

unsigned int n;

struct user_data *data = NULL;

struct user_data *rdata = NULL;//用于读写两个的两个方面传递参数

socklen_t clilen;

struct sockaddr_in clientaddr;

struct sockaddr_in serveraddr;

void setnonblocking(int sock)

{

int opts;

opts = fcntl(sock, F_GETFL);

if (opts < 0)

{

perror("fcntl(sock,GETFL)");

exit(1);

}

opts = opts | O_NONBLOCK;

if (fcntl(sock, F_SETFL, opts) < 0)

{

perror("fcntl(sock,SETFL,opts)");

exit(1);

}

}

void init()

{

listenfd = socket(AF_INET, SOCK_STREAM, 0);//协议族、socket类型、协议(0表示选择默认的协议)

//把socket设置为非阻塞方式

setnonblocking(listenfd);

int reuse_socket = 1;

    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof(int)) == -1){

        printf("setsockopt reuse-addr error!");

    }

bzero(&serveraddr, sizeof(serveraddr));

serveraddr.sin_family = AF_INET;

char local_addr[] = "0.0.0.0";

inet_aton(local_addr, &(serveraddr.sin_addr));//htons(SERV_PORT);

serveraddr.sin_port = htons(SERV_PORT);

bind(listenfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

listen(listenfd, LISTENQ);

maxi = 0;

}

void work_cycle(int j)

{

//生成用于处理accept的epoll专用的文件描述符

epfd = epoll_create(256);

//设置与要处理的事件相关的文件描述符

ev.data.fd = listenfd;

//设置要处理的事件类型

ev.events = EPOLLIN | EPOLLET;

//注册epoll事件

epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

for (;;)

{

//等待epoll事件的发生

nfds = epoll_wait(epfd, events, 20, 1000);

//处理所发生的所有事件

//printf("epoll_wait\n");

for (i = 0; i < nfds; ++i)

{

if (events[i].data.fd == listenfd)

{

connfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clilen);

if (connfd < 0)

{

printf("process %d:connfd<0 accept failure\n",j);

continue;

}

setnonblocking(connfd);

char *str = inet_ntoa(clientaddr.sin_addr);

//std::cout << "connec_ from >>" << str << std::endl;

printf("process %d:connect_from >>%s listenfd=%d connfd=%d\n",j, str,listenfd, connfd);

//设置用于读操作的文件描述符

ev.data.fd = connfd;

//设置用于注测的读操作事件

ev.events = EPOLLIN | EPOLLET;

//注册ev

epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);

} else{

if (events[i].events & EPOLLIN)

{

printf("process %d:reading! connfd=%d\n",j,events[i].data.fd);

if ((sockfd = events[i].data.fd) < 0) continue;

data = (struct user_data *)malloc(sizeof(struct user_data));

if(data == NULL)

{

printf("process %d:user_data malloc error",j);

exit(1);

}

data->fd = sockfd;

if ((n = read(sockfd, data->line, MAXLINE)) < 0)

{

if (errno == ECONNRESET)

{

close(sockfd);

} else

printf("process %d:readline error\n",j);

if (data != NULL) {

free(data);

data = NULL;

}

}else {

if (n == 0)

{

close(sockfd);

printf("process %d:Client close connect!\n",j);

if (data != NULL) {

//delete data;

free(data);

data = NULL;

}

} else

{

data->n_size = n;

//设置需要传递出去的数据

ev.data.ptr = data;

//设置用于注测的写操作事件

ev.events = EPOLLOUT | EPOLLET;

//修改sockfd上要处理的事件为EPOLLOUT

epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);

}

}

} else {

if (events[i].events & EPOLLOUT)

{

rdata = (struct user_data *) events[i].data.ptr;

sockfd = rdata->fd;

printf("process %d:writing! connfd=%d\n",j,sockfd);

write(sockfd, rdata->line, rdata->n_size);

//delete rdata;

free(rdata);

//设置用于读操作的文件描述符

ev.data.fd = sockfd;

//设置用于注测的读操作事件

ev.events = EPOLLIN | EPOLLET;

//修改sockfd上要处理的事件为EPOLIN

epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);

}

}

}

}

}

}

typedef void (*fun_ptr)(void);

void pxy_process()

{

printf("Hello,World!\n");

for(;;){

sleep(1);

//pxy_log_error(PXY_LOG_NOTICE,cycle->log,"sleep 1 second!\n");

}

exit(0);

}

int main()

{

int i;

int pid;

init();

for(i=0;i<3;i++)

{

pid = fork();

switch(pid){

case -1:

printf("fork sub process failed!\n");

break;

case 0:

work_cycle(i);

break;

default:

break;

}

}

while(1){

sleep(1);

}

}
```







## Test2.c

```
#include <sys/socket.h>

#include <sys/types.h>

#include <sys/epoll.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <fcntl.h>

#include <unistd.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <pthread.h>

#include <errno.h>

#define MAXLINE 100

#define OPEN_MAX 100

#define LISTENQ 20

#define SERV_PORT 8888

#define INFTIM 1000

//用于读写两个的两个方面传递参数

struct user_data

{

int fd;

unsigned int n_size;

char line[MAXLINE];

};

//声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件

struct epoll_event ev, events[20];

int epfd;

pthread_mutex_t mutex;

pthread_cond_t cond1;

struct task *readhead = NULL, *readtail = NULL, *writehead = NULL;

int i, 

maxi, 

listenfd, 

connfd, 

sockfd, 

nfds;

unsigned int n;

struct user_data *data = NULL;

struct user_data *rdata = NULL;//用于读写两个的两个方面传递参数

socklen_t clilen;

struct sockaddr_in clientaddr;

struct sockaddr_in serveraddr;

void setnonblocking(int sock)

{

int opts;

opts = fcntl(sock, F_GETFL);

if (opts < 0)

{

perror("fcntl(sock,GETFL)");

exit(1);

}

opts = opts | O_NONBLOCK;

if (fcntl(sock, F_SETFL, opts) < 0)

{

perror("fcntl(sock,SETFL,opts)");

exit(1);

}

}

void init()

{

listenfd = socket(AF_INET, SOCK_STREAM, 0);//协议族、socket类型、协议(0表示选择默认的协议)

//把socket设置为非阻塞方式

setnonblocking(listenfd);

int reuse_socket = 1;

    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof(int)) == -1){

        printf("setsockopt reuse-addr error!");

    }

bzero(&serveraddr, sizeof(serveraddr));

serveraddr.sin_family = AF_INET;

char local_addr[] = "0.0.0.0";

inet_aton(local_addr, &(serveraddr.sin_addr));//htons(SERV_PORT);

serveraddr.sin_port = htons(SERV_PORT);

bind(listenfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

listen(listenfd, LISTENQ);

maxi = 0;

}

void work_cycle(int j)

{

//生成用于处理accept的epoll专用的文件描述符

//epfd = epoll_create(256);

//设置与要处理的事件相关的文件描述符

//ev.data.fd = listenfd;

//设置要处理的事件类型

ev.events = EPOLLIN | EPOLLET;

//注册epoll事件

epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

for (;;)

{

//等待epoll事件的发生

nfds = epoll_wait(epfd, events, 20, 1000);

//处理所发生的所有事件

//printf("epoll_wait\n");

for (i = 0; i < nfds; ++i)

{

if (events[i].data.fd == listenfd)

{

connfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clilen);

if (connfd < 0)

{

printf("process %d:connfd<0 accept failure\n",j);

continue;

}

setnonblocking(connfd);

char *str = inet_ntoa(clientaddr.sin_addr);

//std::cout << "connec_ from >>" << str << std::endl;

printf("process %d:connect_from >>%s listenfd=%d connfd=%d\n",j, str,listenfd, connfd);

//设置用于读操作的文件描述符

ev.data.fd = connfd;

//设置用于注测的读操作事件

ev.events = EPOLLIN | EPOLLET;

//注册ev

epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);

} else{

if (events[i].events & EPOLLIN)

{

printf("process %d:reading! connfd=%d\n",j,events[i].data.fd);

if ((sockfd = events[i].data.fd) < 0) continue;

data = (struct user_data *)malloc(sizeof(struct user_data));

if(data == NULL)

{

printf("process %d:user_data malloc error",j);

exit(1);

}

data->fd = sockfd;

if ((n = read(sockfd, data->line, MAXLINE)) < 0)

{

if (errno == ECONNRESET)

{

close(sockfd);

} else

printf("process %d:readline error\n",j);

if (data != NULL) {

free(data);

data = NULL;

}

}else {

if (n == 0)

{

close(sockfd);

printf("process %d:Client close connect!\n",j);

if (data != NULL) {

//delete data;

free(data);

data = NULL;

}

} else

{

data->n_size = n;

//设置需要传递出去的数据

ev.data.ptr = data;

//设置用于注测的写操作事件

ev.events = EPOLLOUT | EPOLLET;

//修改sockfd上要处理的事件为EPOLLOUT

epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);

}

}

} else {

if (events[i].events & EPOLLOUT)

{

rdata = (struct user_data *) events[i].data.ptr;

sockfd = rdata->fd;

printf("process %d:writing! connfd=%d\n",j,sockfd);

write(sockfd, rdata->line, rdata->n_size);

//delete rdata;

free(rdata);

//设置用于读操作的文件描述符

ev.data.fd = sockfd;

//设置用于注测的读操作事件

ev.events = EPOLLIN | EPOLLET;

//修改sockfd上要处理的事件为EPOLIN

epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);

}

}

}

}

}

}

typedef void (*fun_ptr)(void);

void pxy_process()

{

printf("Hello,World!\n");

for(;;){

sleep(1);

//pxy_log_error(PXY_LOG_NOTICE,cycle->log,"sleep 1 second!\n");

}

exit(0);

}

int main()

{

int i;

int pid;

init();

//生成用于处理accept的epoll专用的文件描述符

epfd = epoll_create(256);

//设置与要处理的事件相关的文件描述符

ev.data.fd = listenfd;

for(i=0;i<3;i++)

{

pid = fork();

switch(pid){

case -1:

printf("fork sub process failed!\n");

break;

case 0:

work_cycle(i);

break;

default:

break;

}

}

while(1){

sleep(1);

}

}
```





# 7. 参考

[http://blog.csdn.net/yankai0219/article/details/8453313](http://blog.csdn.net/yankai0219/article/details/8453313) Nginx中的Epoll事件处理机制 



[http://simohayha.iteye.com/blog/561424](http://simohayha.iteye.com/blog/561424) linux已经不存在惊群现象

[http://gmd20.wap.blog.163.com/w2/blogDetail.do;jsessionid=193DBC380EF5E8D6EB26BEC1E73945AA.blog84-8010?blogId=fks_087066085080085070093082084066072087087075080085095065087&showRest=true&p=6&hostID=gmd20](http://gmd20.wap.blog.163.com/w2/blogDetail.do;jsessionid=193DBC380EF5E8D6EB26BEC1E73945AA.blog84-8010?blogId=fks_087066085080085070093082084066072087087075080085095065087&showRest=true&p=6&hostID=gmd20)



[http://blog.csdn.net/yanook/article/details/6582800](http://blog.csdn.net/yanook/article/details/6582800)

linux2.6 kernel已经解决accept的惊群现象 



[http://uwsgi-docs.readthedocs.org/en/latest/articles/SerializingAccept.html](http://uwsgi-docs.readthedocs.org/en/latest/articles/SerializingAccept.html)



[http://simohayha.iteye.com/blog/561424](http://simohayha.iteye.com/blog/561424)

等待队列设置了标志位，唤醒一位，就停止

[http://blog.csdn.net/hdutigerkin/article/details/7517390](http://blog.csdn.net/hdutigerkin/article/details/7517390) epoll详细工作原理 

[http://blog.163.com/hzr163_2004/blog/static/3308607520106194177905/](http://blog.163.com/hzr163_2004/blog/static/3308607520106194177905/)  poll和epoll内核源代码剖析二[转]  

[http://blog.csdn.net/russell_tao/article/details/7204260](http://blog.csdn.net/russell_tao/article/details/7204260) “惊群”，看看nginx是怎么解决它的 







