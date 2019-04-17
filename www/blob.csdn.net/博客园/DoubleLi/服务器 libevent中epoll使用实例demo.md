# 服务器 libevent中epoll使用实例demo - DoubleLi - 博客园






名词解释：man epoll之后，得到如下结果：

NAME
       epoll - I/O event notification facility

SYNOPSIS
       #include <sys/epoll.h>

DESCRIPTION
       epoll is a variant of poll(2) that can be used either as Edge or Level
       Triggered interface and scales well to large numbers of  watched  fds.
       Three  system  calls  are provided to set up and control an epoll set:
       epoll_create(2), epoll_ctl(2), epoll_wait(2).

       An epoll set is connected to a file descriptor created  by  epoll_cre-
       ate(2).   Interest for certain file descriptors is then registered via
       epoll_ctl(2).  Finally, the actual wait is started by epoll_wait(2).

其实，一切的解释都是多余的，按照我目前的了解，EPOLL模型似乎只有一种格式，所以大家只要参考我下面的代码，就能够对EPOLL有所了解了，代码的解释都已经在注释中：

while (TRUE)
 {
  int nfds = epoll_wait (m_epoll_fd, m_events, MAX_EVENTS, EPOLL_TIME_OUT);//等待EPOLL时间的发生，相当于监听，至于相关的端口，需要在初始化EPOLL的时候绑定。
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
    PRINTF ("CATCH捕获错误/n");
    continue;
   }
  }
  m_bOnTimeChecking = TRUE;
  OnTimer ();//进行一些定时的操作，主要就是删除一些短线用户等。
 }

　其实EPOLL的精华，按照我目前的理解，也就是上述的几段短短的代码，看来时代真的不同了，以前如何接受大量用户连接的问题，现在却被如此轻松的搞定，真是让人不得不感叹。

今天搞了一天的epoll，想做一个高并发的代理程序。刚开始真是郁闷,一直搞不通，网上也有几篇介绍epoll的文章。但都不深入，没有将一些注意的地方讲明。以至于走了很多弯路，现将自己的一些理解共享给大家,以少走弯路。 

epoll用到的所有函数都是在头文件sys/epoll.h中声明，有什么地方不明白或函数忘记了可以去看一下。 
epoll和select相比，最大不同在于: 

1epoll返回时已经明确的知道哪个sokcet fd发生了事件，不用再一个个比对。这样就提高了效率。 
2select的FD_SETSIZE是有限止的，而epoll是没有限止的只与系统资源有关。 

1、epoll_create函数
函数声明：int epoll_create(int size)
该函数生成一个epoll专用的文件描述符。它其实是在内核申请一空间，用来存放你想关注的socket fd上是否发生以及发生了什么事件。size就是你在这个epoll fd上能关注的最大socket fd数。随你定好了。只要你有空间。可参见上面与select之不同2. 

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













[java] [view plain](http://blog.csdn.net/haoahua/article/details/2037704)[copy](http://blog.csdn.net/haoahua/article/details/2037704)



- #include <stdio.h>  
- #include <stdlib.h>  
- #include <errno.h>  
- #include <string.h>  
- #include <sys/types.h>  
- #include <netinet/in.h>  
- #include <sys/socket.h>  
- #include <sys/wait.h>  
- #include <unistd.h>  
- #include <arpa/inet.h>  
- #include <openssl/ssl.h>  
- #include <openssl/err.h>  
- #include <fcntl.h>  
- #include <sys/epoll.h>  
- #include <sys/time.h>  
- #include <sys/resource.h>  




[java] [view plain](http://blog.csdn.net/haoahua/article/details/2037704)[copy](http://blog.csdn.net/haoahua/article/details/2037704)



- #define MAXBUF 1024  
- #define MAXEPOLLSIZE 10000  




[java] [view plain](http://blog.csdn.net/haoahua/article/details/2037704)[copy](http://blog.csdn.net/haoahua/article/details/2037704)



- /* 
- setnonblocking - 设置句柄为非阻塞方式 
- */  
- int setnonblocking(int sockfd)  
- {  
- if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)  
-  {  
- return -1;  
-     }  
- return 0;  
- }  




[java] [view plain](http://blog.csdn.net/haoahua/article/details/2037704)[copy](http://blog.csdn.net/haoahua/article/details/2037704)



- /* 
- handle_message - 处理每个 socket 上的消息收发 
- */  
- int handle_message(int new_fd)  
- {  
- char buf[MAXBUF + 1];  
- int len;  
- 
- /* 开始处理每个新连接上的数据收发 */  
-     bzero(buf, MAXBUF + 1);  
- 
- /* 接收客户端的消息 */  
-     len = recv(new_fd, buf, MAXBUF, 0);  
- if (len > 0)  
-  {  
-         printf  
-             ("%d接收消息成功:'%s'，共%d个字节的数据/n",  
-              new_fd, buf, len);  
-  }  
- else  
-  {  
- if (len < 0)  
-      printf  
-                 ("消息接收失败！错误代码是%d，错误信息是'%s'/n",  
-                  errno, strerror(errno));  
-         close(new_fd);  
- return -1;  
-     }  
- /* 处理每个新连接上的数据收发结束 */  
- return len;  
- }  
- /************关于本文档******************************************** 
- *filename: epoll-server.c 
- *purpose: 演示epoll处理海量socket连接的方法 
- *wrote by: zhoulifa(<a href="mailto:zhoulifa@163.com">zhoulifa@163.com</a>) 周立发(<a href="http://zhoulifa.bokee.com">http://zhoulifa.bokee.com</a>) 
- Linux爱好者 Linux知识传播者 SOHO族 开发者 最擅长C语言 
- *date time:2007-01-31 21:00 
- *Note: 任何人可以任意复制代码并运用这些文档，当然包括你的商业用途 
- * 但请遵循GPL 
- *Thanks to:Google 
- *Hope:希望越来越多的人贡献自己的力量，为科学技术发展出力 
- * 科技站在巨人的肩膀上进步更快！感谢有开源前辈的贡献！ 
- *********************************************************************/  
- int main(int argc, char **argv)  
- {  
- int listener, new_fd, kdpfd, nfds, n, ret, curfds;  
-     socklen_t len;  
-     struct sockaddr_in my_addr, their_addr;  
-     unsigned int myport, lisnum;  
-     struct epoll_event ev;  
-     struct epoll_event events[MAXEPOLLSIZE];  
-     struct rlimit rt;  
-  myport = 5000;  
-  lisnum = 2;   
- 
- /* 设置每个进程允许打开的最大文件数 */  
-     rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE;  
- if (setrlimit(RLIMIT_NOFILE, &rt) == -1)   
-  {  
-         perror("setrlimit");  
-         exit(1);  
-     }  
- else   
-     {  
-         printf("设置系统资源参数成功！/n");  
-     }  




[java] [view plain](http://blog.csdn.net/haoahua/article/details/2037704)[copy](http://blog.csdn.net/haoahua/article/details/2037704)



- /* 开启 socket 监听 */  
- if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1)  
-     {  
-         perror("socket");  
-         exit(1);  
-     }  
- else  
-     {  
-         printf("socket 创建成功！/n");  
-  }  
- 
-     setnonblocking(listener);  




[java] [view plain](http://blog.csdn.net/haoahua/article/details/2037704)[copy](http://blog.csdn.net/haoahua/article/details/2037704)



-     bzero(&my_addr, sizeof(my_addr));  
-     my_addr.sin_family = PF_INET;  
-     my_addr.sin_port = htons(myport);  
-     my_addr.sin_addr.s_addr = INADDR_ANY;  




[java] [view plain](http://blog.csdn.net/haoahua/article/details/2037704)[copy](http://blog.csdn.net/haoahua/article/details/2037704)



- if (bind(listener, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)   
-     {  
-         perror("bind");  
-         exit(1);  
-     }   
- else  
-     {  
-         printf("IP 地址和端口绑定成功/n");  
-  }  
- if (listen(listener, lisnum) == -1)   
-     {  
-         perror("listen");  
-         exit(1);  
-     }  
- else  
-     {  
-         printf("开启服务成功！/n");  
-  }  
- 
- /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */  
-     kdpfd = epoll_create(MAXEPOLLSIZE);  
-     len = sizeof(struct sockaddr_in);  
-     ev.events = EPOLLIN | EPOLLET;  
-     ev.data.fd = listener;  
- if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listener, &ev) < 0)   
-  {  
-         fprintf(stderr, "epoll set insertion error: fd=%d/n", listener);  
- return -1;  
-     }  
- else  
-     {  
-   printf("监听 socket 加入 epoll 成功！/n");  
-  }  
-     curfds = 1;  
- while (1)   
-  {  
- /* 等待有事件发生 */  
-         nfds = epoll_wait(kdpfd, events, curfds, -1);  
- if (nfds == -1)  
-   {  
-             perror("epoll_wait");  
- break;  
-         }  
- /* 处理所有事件 */  
- for (n = 0; n < nfds; ++n)  
-   {  
- if (events[n].data.fd == listener)   
-    {  
-                 new_fd = accept(listener, (struct sockaddr *) &their_addr,&len);  
- if (new_fd < 0)   
-     {  
-                     perror("accept");  
- continue;  
-                 }   
- else  
-     {  
-      printf("有连接来自于： %d:%d， 分配的 socket 为:%d/n",  
-                              inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);  
-     }  
-                 setnonblocking(new_fd);  
-                 ev.events = EPOLLIN | EPOLLET;  
-                 ev.data.fd = new_fd;  
- if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new_fd, &ev) < 0)  
-     {  
-                     fprintf(stderr, "把 socket '%d' 加入 epoll 失败！%s/n",  
-                             new_fd, strerror(errno));  
- return -1;  
-                 }  
-                 curfds++;  
-             }   
- else  
-    {  
-                 ret = handle_message(events[n].data.fd);  
- if (ret < 1 && errno != 11)  
-     {  
-                     epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,&ev);  
-                     curfds--;  
-                 }  
-             }  
-         }  
-     }  
-     close(listener);  
- return 0;  
- }  



 epoll_wait运行的原理是 等侍注册在epfd上的socket fd的事件的发生，如果发生则将发生的sokct fd和事件类型放入到events数组中。 并且将注册在epfd上的socket fd的事件类型给清空，所以如果下一个循环你还要关注这个socket fd的话，则需要用epoll_ctl(epfd,EPOLL_CTL_MOD,listenfd,&ev)来重新设置socket fd的事件类型。这时不用EPOLL_CTL_ADD,因为socket fd并未清空，只是事件类型清空。这一步非常重要。 



二、第二个示例：



1. Epoll是何方神圣？

Epoll可是当前在Linux下开发大规模并发网络程序的热门人选，Epoll 在Linux2.6内核中正式引入，和select相似，其实都I/O多路复用技术而已，并没有什么神秘的。

其实在Linux下设计并发网络程序，向来不缺少方法，比如典型的Apache模型（Process Per Connection，简称PPC），TPC（Thread PerConnection）模型，以及select模型和poll模型，那为何还要再引入Epoll这个东东呢？那还是有得说说的…

2. 常用模型的缺点

如果不摆出来其他模型的缺点，怎么能对比出Epoll的优点呢。

2.1 PPC/TPC模型

这两种模型思想类似，就是让每一个到来的连接一边自己做事去，别再来烦我。只是PPC是为它开了一个进程，而TPC开了一个线程。可是别烦我是有代价的，它要时间和空间啊，连接多了之后，那么多的进程/线程切换，这开销就上来了；因此这类模型能接受的最大连接数都不会高，一般在几百个左右。

2.2 select模型

1. 最大并发数限制，因为一个进程所打开的FD（文件描述符）是有限制的，由FD_SETSIZE设置，默认值是1024/2048，因此Select模型的最大并发数就被相应限制了。自己改改这个FD_SETSIZE？想法虽好，可是先看看下面吧…

2. 效率问题，select每次调用都会线性扫描全部的FD集合，这样效率就会呈现线性下降，把FD_SETSIZE改大的后果就是，大家都慢慢来，什么？都超时了？？！！

3. 内核/用户空间 内存拷贝问题，如何让内核把FD消息通知给用户空间呢？在这个问题上select采取了内存拷贝方法。

2.3 poll模型

基本上效率和select是相同的，select缺点的2和3它都没有改掉。

3. Epoll的提升

把其他模型逐个批判了一下，再来看看Epoll的改进之处吧，其实把select的缺点反过来那就是Epoll的优点了。

3.1. Epoll没有最大并发连接的限制，上限是最大可以打开文件的数目，这个数字一般远大于2048, 一般来说这个数目和系统内存关系很大，具体数目可以cat /proc/sys/fs/file-max察看。

3.2. 效率提升，Epoll最大的优点就在于它只管你“活跃”的连接，而跟连接总数无关，因此在实际的网络环境中，Epoll的效率就会远远高于select和poll。

3.3. 内存拷贝，Epoll在这点上使用了“共享内存”，这个内存拷贝也省略了。





4. Epoll为什么高效

Epoll的高效和其数据结构的设计是密不可分的，这个下面就会提到。

首先回忆一下select模型，当有I/O事件到来时，select通知应用程序有事件到了快去处理，而应用程序必须轮询所有的FD集合，测试每个FD是否有事件发生，并处理事件；代码像下面这样：



int res = select(maxfd+1, &readfds, NULL, NULL, 120);

if(res > 0)

{

    for(int i = 0; i < MAX_CONNECTION; i++)

    {

        if(FD_ISSET(allConnection[i],&readfds))

        {

            handleEvent(allConnection[i]);

        }

    }

}

// if(res == 0) handle timeout, res < 0 handle error



Epoll不仅会告诉应用程序有I/0事件到来，还会告诉应用程序相关的信息，这些信息是应用程序填充的，因此根据这些信息应用程序就能直接定位到事件，而不必遍历整个FD集合。

intres = epoll_wait(epfd, events, 20, 120);

for(int i = 0; i < res;i++)

{

    handleEvent(events[n]);

}

5. Epoll关键数据结构

前面提到Epoll速度快和其数据结构密不可分，其关键数据结构就是：

structepoll_event {

    __uint32_t events;      // Epoll events

    epoll_data_t data;      // User datavariable

};

typedef union epoll_data {

    void *ptr;

   int fd;

    __uint32_t u32;

    __uint64_t u64;

} epoll_data_t;

可见epoll_data是一个union结构体,借助于它应用程序可以保存很多类型的信息:fd、指针等等。有了它，应用程序就可以直接定位目标了。

6. 使用Epoll

既然Epoll相比select这么好，那么用起来如何呢？会不会很繁琐啊…先看看下面的三个函数吧，就知道Epoll的易用了。



intepoll_create(int size);

生成一个Epoll专用的文件描述符，其实是申请一个内核空间，用来存放你想关注的socket fd上是否发生以及发生了什么事件。size就是你在这个Epoll fd上能关注的最大socket fd数，大小自定，只要内存足够。

intepoll_ctl(int epfd, intop, int fd, structepoll_event *event);

控制某个Epoll文件描述符上的事件：注册、修改、删除。其中参数epfd是epoll_create()创建Epoll专用的文件描述符。相对于select模型中的FD_SET和FD_CLR宏。

intepoll_wait(int epfd,structepoll_event * events,int maxevents,int timeout);

等待I/O事件的发生；参数说明：

epfd:由epoll_create() 生成的Epoll专用的文件描述符；

epoll_event:用于回传代处理事件的数组；

maxevents:每次能处理的事件数；

timeout:等待I/O事件发生的超时值；

返回发生事件数。

相对于select模型中的select函数。

7. 例子程序

下面是一个简单Echo Server的例子程序，麻雀虽小，五脏俱全，还包含了一个简单的超时检查机制，简洁起见没有做错误处理。



[cpp] [view plain](http://blog.csdn.net/sparkliang/article/details/4770655)[copy](http://blog.csdn.net/sparkliang/article/details/4770655)

- //     
- // a simple echo server using epoll in linux    
- //     
- // 2009-11-05    
- // 2013-03-22:修改了几个问题，1是/n格式问题，2是去掉了原代码不小心加上的ET模式;  
- // 本来只是简单的示意程序，决定还是加上 recv/send时的buffer偏移  
- // by sparkling    
- //     
- #include <sys/socket.h>    
- #include <sys/epoll.h>    
- #include <netinet/in.h>    
- #include <arpa/inet.h>    
- #include <fcntl.h>    
- #include <unistd.h>    
- #include <stdio.h>    
- #include <errno.h>    
- #include <iostream>    
- using namespace std;    
- #define MAX_EVENTS 500    
- struct myevent_s    
- {    
-     int fd;    
-     void (*call_back)(int fd, int events, void *arg);    
-     int events;    
-     void *arg;    
-     int status; // 1: in epoll wait list, 0 not in    
-     char buff[128]; // recv data buffer    
-     int len, s_offset;    
-     long last_active; // last active time    
- };    
- // set event    
- void EventSet(myevent_s *ev, int fd, void (*call_back)(int, int, void*), void *arg)    
- {    
-     ev->fd = fd;    
-     ev->call_back = call_back;    
-     ev->events = 0;    
-     ev->arg = arg;    
-     ev->status = 0;  
-     bzero(ev->buff, sizeof(ev->buff));  
-     ev->s_offset = 0;    
-     ev->len = 0;  
-     ev->last_active = time(NULL);    
- }    
- // add/mod an event to epoll    
- void EventAdd(int epollFd, int events, myevent_s *ev)    
- {    
-     struct epoll_event epv = {0, {0}};    
-     int op;    
-     epv.data.ptr = ev;    
-     epv.events = ev->events = events;    
-     if(ev->status == 1){    
-         op = EPOLL_CTL_MOD;    
-     }    
-     else{    
-         op = EPOLL_CTL_ADD;    
-         ev->status = 1;    
-     }    
-     if(epoll_ctl(epollFd, op, ev->fd, &epv) < 0)    
-         printf("Event Add failed[fd=%d], evnets[%d]\n", ev->fd, events);    
-     else    
-         printf("Event Add OK[fd=%d], op=%d, evnets[%0X]\n", ev->fd, op, events);    
- }    
- // delete an event from epoll    
- void EventDel(int epollFd, myevent_s *ev)    
- {    
-     struct epoll_event epv = {0, {0}};    
-     if(ev->status != 1) return;    
-     epv.data.ptr = ev;    
-     ev->status = 0;  
-     epoll_ctl(epollFd, EPOLL_CTL_DEL, ev->fd, &epv);    
- }    
- int g_epollFd;    
- myevent_s g_Events[MAX_EVENTS+1]; // g_Events[MAX_EVENTS] is used by listen fd    
- void RecvData(int fd, int events, void *arg);    
- void SendData(int fd, int events, void *arg);    
- // accept new connections from clients    
- void AcceptConn(int fd, int events, void *arg)    
- {    
-     struct sockaddr_in sin;    
-     socklen_t len = sizeof(struct sockaddr_in);    
-     int nfd, i;    
-     // accept    
-     if((nfd = accept(fd, (struct sockaddr*)&sin, &len)) == -1)    
-     {    
-         if(errno != EAGAIN && errno != EINTR)    
-         {    
-         }  
-         printf("%s: accept, %d", __func__, errno);    
-         return;    
-     }    
-     do    
-     {    
-         for(i = 0; i < MAX_EVENTS; i++)    
-         {    
-             if(g_Events[i].status == 0)    
-             {    
-                 break;    
-             }    
-         }    
-         if(i == MAX_EVENTS)    
-         {    
-             printf("%s:max connection limit[%d].", __func__, MAX_EVENTS);    
-             break;    
-         }    
-         // set nonblocking  
-         int iret = 0;  
-         if((iret = fcntl(nfd, F_SETFL, O_NONBLOCK)) < 0)  
-         {  
-             printf("%s: fcntl nonblocking failed:%d", __func__, iret);  
-             break;  
-         }  
-         // add a read event for receive data    
-         EventSet(&g_Events[i], nfd, RecvData, &g_Events[i]);    
-         EventAdd(g_epollFd, EPOLLIN, &g_Events[i]);    
-     }while(0);    
-     printf("new conn[%s:%d][time:%d], pos[%d]\n", inet_ntoa(sin.sin_addr),   
-             ntohs(sin.sin_port), g_Events[i].last_active, i);    
- }    
- // receive data    
- void RecvData(int fd, int events, void *arg)    
- {    
-     struct myevent_s *ev = (struct myevent_s*)arg;    
-     int len;    
-     // receive data  
-     len = recv(fd, ev->buff+ev->len, sizeof(ev->buff)-1-ev->len, 0);      
-     EventDel(g_epollFd, ev);  
-     if(len > 0)  
-     {  
-         ev->len += len;  
-         ev->buff[len] = '\0';    
-         printf("C[%d]:%s\n", fd, ev->buff);    
-         // change to send event    
-         EventSet(ev, fd, SendData, ev);    
-         EventAdd(g_epollFd, EPOLLOUT, ev);    
-     }    
-     else if(len == 0)    
-     {    
-         close(ev->fd);    
-         printf("[fd=%d] pos[%d], closed gracefully.\n", fd, ev-g_Events);    
-     }    
-     else    
-     {    
-         close(ev->fd);    
-         printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));    
-     }    
- }    
- // send data    
- void SendData(int fd, int events, void *arg)    
- {    
-     struct myevent_s *ev = (struct myevent_s*)arg;    
-     int len;    
-     // send data    
-     len = send(fd, ev->buff + ev->s_offset, ev->len - ev->s_offset, 0);  
-     if(len > 0)    
-     {  
-         printf("send[fd=%d], [%d<->%d]%s\n", fd, len, ev->len, ev->buff);  
-         ev->s_offset += len;  
-         if(ev->s_offset == ev->len)  
-         {  
-             // change to receive event  
-             EventDel(g_epollFd, ev);    
-             EventSet(ev, fd, RecvData, ev);    
-             EventAdd(g_epollFd, EPOLLIN, ev);    
-         }  
-     }    
-     else    
-     {    
-         close(ev->fd);    
-         EventDel(g_epollFd, ev);    
-         printf("send[fd=%d] error[%d]\n", fd, errno);    
-     }    
- }    
- void InitListenSocket(int epollFd, short port)    
- {    
-     int listenFd = socket(AF_INET, SOCK_STREAM, 0);    
-     fcntl(listenFd, F_SETFL, O_NONBLOCK); // set non-blocking    
-     printf("server listen fd=%d\n", listenFd);    
-     EventSet(&g_Events[MAX_EVENTS], listenFd, AcceptConn, &g_Events[MAX_EVENTS]);    
-     // add listen socket    
-     EventAdd(epollFd, EPOLLIN, &g_Events[MAX_EVENTS]);    
-     // bind & listen    
-     sockaddr_in sin;    
-     bzero(&sin, sizeof(sin));    
-     sin.sin_family = AF_INET;    
-     sin.sin_addr.s_addr = INADDR_ANY;    
-     sin.sin_port = htons(port);    
-     bind(listenFd, (const sockaddr*)&sin, sizeof(sin));    
-     listen(listenFd, 5);    
- }    
- int main(int argc, char **argv)    
- {    
-     unsigned short port = 12345; // default port    
-     if(argc == 2){    
-         port = atoi(argv[1]);    
-     }    
-     // create epoll    
-     g_epollFd = epoll_create(MAX_EVENTS);    
-     if(g_epollFd <= 0) printf("create epoll failed.%d\n", g_epollFd);    
-     // create & bind listen socket, and add to epoll, set non-blocking    
-     InitListenSocket(g_epollFd, port);    
-     // event loop    
-     struct epoll_event events[MAX_EVENTS];    
-     printf("server running:port[%d]\n", port);    
-     int checkPos = 0;    
-     while(1){    
-         // a simple timeout check here, every time 100, better to use a mini-heap, and add timer event    
-         long now = time(NULL);    
-         for(int i = 0; i < 100; i++, checkPos++) // doesn't check listen fd    
-         {    
-             if(checkPos == MAX_EVENTS) checkPos = 0; // recycle    
-             if(g_Events[checkPos].status != 1) continue;    
-             long duration = now - g_Events[checkPos].last_active;    
-             if(duration >= 60) // 60s timeout    
-             {    
-                 close(g_Events[checkPos].fd);    
-                 printf("[fd=%d] timeout[%d--%d].\n", g_Events[checkPos].fd, g_Events[checkPos].last_active, now);    
-                 EventDel(g_epollFd, &g_Events[checkPos]);    
-             }    
-         }    
-         // wait for events to happen    
-         int fds = epoll_wait(g_epollFd, events, MAX_EVENTS, 1000);    
-         if(fds < 0){    
-             printf("epoll_wait error, exit\n");    
-             break;    
-         }    
-         for(int i = 0; i < fds; i++){    
-             myevent_s *ev = (struct myevent_s*)events[i].data.ptr;    
-             if((events[i].events&EPOLLIN)&&(ev->events&EPOLLIN)) // read event    
-             {    
-                 ev->call_back(ev->fd, events[i].events, ev->arg);    
-             }    
-             if((events[i].events&EPOLLOUT)&&(ev->events&EPOLLOUT)) // write event    
-             {    
-                 ev->call_back(ev->fd, events[i].events, ev->arg);    
-             }    
-         }    
-     }    
-     // free resource    
-     return 0;    
- }     











