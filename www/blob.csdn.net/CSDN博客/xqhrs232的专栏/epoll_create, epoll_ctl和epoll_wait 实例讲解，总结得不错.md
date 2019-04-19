# epoll_create, epoll_ctl和epoll_wait 实例讲解，总结得不错 - xqhrs232的专栏 - CSDN博客
2011年12月03日 20:54:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：545
原文地址::[http://apps.hi.baidu.com/share/detail/31300135](http://apps.hi.baidu.com/share/detail/31300135)
NAME
       epoll - I/O event notification facility
SYNOPSIS
       #include <sys/epoll.h>
DEscrīptION
       epoll is a variant of poll(2) that can be used either as Edge or Level
       Triggered interface and scales well to large numbers of watched fds.
       Three system calls are provided to set up and control an epoll set:
       epoll_create(2), epoll_ctl(2), epoll_wait(2).
       An epoll set is connected to a file descrīptor created by epoll_create(2).   Interest for certain file descrīptors is then registered via
       epoll_ctl(2). Finally, the actual wait is started by epoll_wait(2).
其实，一切的解释都是多余的，按照我目前的了解，EPOLL模型似乎只有一种格式，所以大家只要参考我下面的代码，就能够对EPOLL有所了解了，代码的解释都已经在注释中：
while (TRUE)
{
int nfds = epoll_wait (m_epoll_fd, m_events, MAX_EVENTS, EPOLL_TIME_OUT);//等待EPOLL事件的发生，相当于监听，至于相关的端口，需要在初始化EPOLL的时候绑定。
if (nfds <= 0)
   continue;
m_bOnTimeChecking = FALSE;
G_CurTime = time(NULL);
for (int i=0; i<nfds; i++)
{
   try
   {
    if (m_events[i].data.fd == m_listen_http_fd)//如果新监测到一个HTTP用户连接到绑定的HTTP端口，建立新的连接。由于我们新采用了SOCKET连接，所以基本没用。
    {
     OnAcceptHttpEpoll ();
    }
    else if (m_events[i].data.fd == m_listen_sock_fd)//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。
    {
     OnAcceptSockEpoll ();
    }
    else if (m_events[i].events & EPOLLIN)//如果是已经连接的用户，并且收到数据，那么进行读入。
    {
     OnReadEpoll (i);
    }
    OnWriteEpoll (i);//查看当前的活动连接是否有需要写出的数据。
   }
   catch (int)
   {
    PRINTF ("CATCH捕获错误\n");
    continue;
   }
}
m_bOnTimeChecking = TRUE;
OnTimer ();//进行一些定时的操作，主要就是删除一些断线用户等。
}
其实EPOLL的精华，按照我目前的理解，也就是上述的几段短短的代码，看来时代真的不同了，以前如何接受大量用户连接的问题，现在却被如此轻松的搞定，真是让人不得不感叹。
今天搞了一天的epoll，想做一个高并发的代理程序。刚开始真是郁闷,一直搞不通，网上也有几篇介绍epoll的文章。但都不深入，没有将一些注意的地方讲明。以至于走了很多弯路，现将自己的一些理解共享给大家,以少走弯路。
epoll用到的所有函数都是在头文件sys/epoll.h中声明，有什么地方不明白或函数忘记了可以去看一下。
epoll和select相比，最大不同在于:
1epoll返回时已经明确的知道哪个sokcet fd发生了事件，不用再一个个比对。这样就提高了效率。
2select的FD_SETSIZE是有限止的，而epoll是没有限止的只与系统资源有关。
1、epoll_create函数
函数声明：int epoll_create(int size)
该 函数生成一个epoll专用的文件描述符。它其实是在内核申请一空间，用来存放你想关注的socket fd上是否发生以及发生了什么事件。size就是你在这个epoll fd上能关注的最大socket fd数。随你定好了。只要你有空间。可参见上面与select之不同2.
22、epoll_ctl函数
函数声明：int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
该函数用于控制某个epoll文件描述符上的事件，可以注册事件，修改事件，删除事件。
参数：
epfd：由 epoll_create 生成的epoll专用的文件描述符；
op：要进行的操作例如注册事件，可能的取值EPOLL_CTL_ADD 注册、EPOLL_CTL_MOD 修 改、EPOLL_CTL_DEL 删除
fd：关联的文件描述符；
event：指向epoll_event的指针；
如果调用成功返回0,不成功返回-1
用到的数据结构
typedef union epoll_data {
void *ptr;
int fd;
__uint32_t u32;
__uint64_t u64;
} epoll_data_t;
struct epoll_event {
__uint32_t events; /* Epoll events */
epoll_data_t data; /* User data variable */
};
如：
struct epoll_event ev;
//设置与要处理的事件相关的文件描述符
ev.data.fd=listenfd;
//设置要处理的事件类型
ev.events=EPOLLIN|EPOLLET;
//注册epoll事件
epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
常用的事件类型:
EPOLLIN ：表示对应的文件描述符可以读；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET：表示对应的文件描述符有事件发生；
3、epoll_wait函数
函数声明:int epoll_wait(int epfd,struct epoll_event * events,int maxevents,int timeout)
该函数用于轮询I/O事件的发生；
参数：
epfd:由epoll_create 生成的epoll专用的文件描述符；
epoll_event:用于回传代处理事件的数组；
maxevents:每次能处理的事件数；
timeout:等待I/O事件发生的超时值(单位我也不太清楚)；-1相当于阻塞，0相当于非阻塞。一般用-1即可
返回发生事件数。
用法如下：
/*build the epoll enent for recall */
struct epoll_event ev_read[20];
int nfds = 0; //return the events count
nfds=epoll_wait(epoll_fd,ev_read,20, -1);
for(i=0; i
{
if(ev_read[i].data.fd == sock)// the listener port hava data
......
epoll_wait运行的原理是
等侍注册在epfd上的socket fd的事件的发生，如果发生则将发生的sokct fd和事件类型放入到events数组中。
**并 且将注册在epfd上的socket fd的事件类型给清空**，所以如果下一个循环你还要关注这个socket fd的话，则需要用epoll_ctl(epfd,EPOLL_CTL_MOD,listenfd,&ev)来重新设置socket fd的事件类型。这时不用EPOLL_CTL_ADD,因为socket fd并未清空，只是事件类型清空。这一步非常重要。
俺最开始就是没有加这个，白搞了一个上午。
4单个epoll并不能解决所有问题，特别是你的每个操作都比较费时的时候，因为epoll是串行处理的。
所以你还是有必要建立线程池来发挥更大的效能。
//////////////////////////////////////////////////////////////////////////////
man中给出了epoll的用法，example程序如下：
       for(;;) {
           nfds = epoll_wait(kdpfd, events, maxevents, -1);
           for(n = 0; n < nfds; ++n) {
               if(events[n].data.fd == listener) {
                   client = accept(listener, (struct sockaddr *) &local,
                                   &addrlen);
                   if(client < 0){
                       perror("accept");
                       continue;
                   }
                   setnonblocking(client);
                   ev.events = EPOLLIN | EPOLLET;
                   ev.data.fd = client;
                   if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, client, &ev) < 0) {
                       fprintf(stderr, "epoll set insertion error: fd=%d\n",
                               client);
                       return -1;
                   }
               }
               else
                   do_use_fd(events[n].data.fd);
           }
       }
此时使用的是ET模式，即，边沿触发，类似于电平触发，epoll中的边沿触发的意思是只对新到的数据进行通知，而内核缓冲区中如果是旧数据则不进行通知，所以在do_use_fd函数中应该使用如下循环，才能将内核缓冲区中的数据读完。
        while (1) {
           len = recv(*******);
           if (len == -1) {
             if(errno == EAGAIN)
                break;
             perror("recv");
             break;
           }
           do something with the recved data........
        }
在上面例子中没有说明对于listen socket fd该如何处理，有的时候会使用两个线程，一个用来监听accept另一个用来监听epoll_wait，如果是这样使用的话，则listen socket fd使用默认的阻塞方式就行了，而如果epoll_wait和accept处于一个线程中，即，全部由epoll_wait进行监听，则，需将listen socket fd也设置成非阻塞的，这样，对accept也应该使用while包起来（类似于上面的recv），因为，epoll_wait返回时只是说有连接到来了，并没有说有几个连接，而且在ET模式下epoll_wait不会再因为上一次的连接还没读完而返回，这种情况确实存在，我因为这个问题而耗费了一天多的时间，这里需要说明的是，每调用一次accept将从内核中的已连接队列中的队头读取一个连接，因为在并发访问的环境下，有可能有多个连接“同时”到达，而epoll_wait只返回了一次。
唯一有点麻烦是epoll有2种工作方式:LT和ET。
LT(level triggered)是缺省的工作方式，并且同时支持block和no-block socket.在这种做法中，内核告诉你一个文件描述符是否就绪了，然后你可以对这个就绪的fd进行IO操作。如果你不作任何操作，内核还是会继续通知你的，所以，这种模式编程出错误可能性要小一点。传统的select/poll都是这种模型的代表．
ET (edge-triggered)是高速工作方式，只支持no-block socket。在这种模式下，当描述符从未就绪变为就绪时，内核通过epoll告诉你。然后它会假设你知道文件描述符已经就绪，并且不会再为那个文件描述符发送更多的就绪通知，直到你做了某些操作导致那个文件描述符不再为就绪状态了(比如，你在发送，接收或者接收请求，或者发送接收的数据少于一定量时导致了一个EWOULDBLOCK 错误）。但是请注意，如果一直不对这个fd作IO操作(从而导致它再次变成未就绪)，内核不会发送更多的通知(only once),不过在TCP协议中，ET模式的加速效用仍需要更多的benchmark确认。
