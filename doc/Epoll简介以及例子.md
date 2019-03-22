# Epoll简介以及例子

 																				2012年09月08日 13:54:13 					[晴天_娃娃](https://me.csdn.net/cuiyifang) 						阅读数：10574 										

 									

分类： [ Linux](http://blog.csdn.net/ycdoit/article/category/909195) [TCP/IP](http://blog.csdn.net/ycdoit/article/category/900685) 2012-03-19 22:24  91人阅读 [评论](http://blog.csdn.net/ycdoit/article/details/7371096#comments)(0) [收藏](https://blog.csdn.net/cuiyifang/article/details/7957937)  [举报](http://blog.csdn.net/ycdoit/article/details/7371096#report) 

## 第一部分：Epoll简介



 **问题** :  Select,Poll和Epoll的区别

 **答案** : 

 

 Epoll和Select的区别

 

------

\1. 

遍历方式的区别

。select判断是否有事件发生是遍历的，而epoll是事件响应的，一旦句柄上有事件来了，就马上选出来。



 \2. 数目的区别。select一般由一个内核参数（1024）限制了监听的句柄数，但是epoll通常受限于打开文件的数目，通常会打得多。

 \3. epoll自身，还有两种触发方式。水平触发和边缘触发。边沿触发的效率更高（高了不少，但是编程的时候要小心处理每个时间，防止漏掉处理某些事件）。

 



 **Select**

 

------

select()系统调用提供一个机制来实现同步多元I/O：

 

```c
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int select (int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

FD_CLR(int fd, fd_set *set);
FD_ISSET(int fd, fd_set *set);
FD_SET(int fd, fd_set *set);
FD_ZERO(fd_set *set);
```

 调用select()将阻塞，直到指定的文件描述符准备好执行I/O，或者可选参数timeout指定的时间已经过去。

 select()成功返回时，**每组set都被修改以使它只包含准备好I/O的文件描述符**。例如，假设有两个文件描述符，值分别是7和9，被放在readfds中。当select()返回时，如果7仍然在set中，则这个文件描述符已经准备好被读取而不会阻塞。如果9已经不在set中，则读取它将可能会阻塞（我说可能是因为数据可能正好在select返回后就可用，这种情况下，下一次调用select()将返回文件描述符准备好读取）。

**第一个参数n**，等于所有set中最大的那个文件描述符的值加1。



**当select()返回时，timeout参数**的状态在不同的系统中是未定义的，因此每次调用select()之前必须重新初始化timeout和文件描述符set。实际上，当前版本的Linux会自动修改timeout参数，设置它的值为剩余时间。因此，如果timeout被设置为5秒，然后在文件描述符准备好之前经过了3秒，则这一次调用select()返回时tv_sec将变为2。



因为文件描述符set是静态创建的，它们对文件描述符的最大数目强加了一个限制，能够放进set中的最大文件描述符的值由FD_SETSIZE指定。在Linux中，这个值是1024。本章后面我们还将看到这个限制的衍生物。


**返回值和错误代码**
select() 成功时返回准备好I/O的文件描述符数目，包括所有三个set。如果提供了timeout，返回值可能是0；错误时返回-1，并且设置errno为下面几个值之一：
EBADF： 给某个set提供了无效文件描述符。
EINTR:：等待时捕获到信号，可以重新发起调用。
EINVAL:：参数n为负数，或者指定的timeout非法。
ENOMEM:：不够可用内存来完成请求。





 

 **Poll**

 

------

和select()不一样，poll()没有使用低效的三个基于位的文件描述符set，而是采用了一个单独的结构体pollfd数组，由fds指针指向这个组。pollfd结构体定义如下：

 

```
#include <sys/poll.h>

int poll (struct pollfd *fds, unsigned int nfds, int timeout);
struct pollfd {
    int fd; /* file descriptor */
    short events; /* requested events to watch */
    short revents; /* returned events witnessed */
};
```

每一个pollfd结构体指定了一个被监视的文件描述符，可以传递多个结构体，指示poll()监视多个文件描述符。每个结构体的events域是监视该文件描述符的事件掩码，由用户来设置这个域。revents域是文件描述符的操作结果事件掩码。内核在调用返回时设置这个域。events域中请求的任何事件都可能在revents域中返回。合法的事件如下：

POLLIN：有数据可读。
POLLRDNORM：有普通数据可读。
POLLRDBAND：有优先数据可读。
POLLPRI：有紧迫数据可读。
POLLOUT：写数据不会导致阻塞。
POLLWRNORM：写普通数据不会导致阻塞。
POLLWRBAND：写优先数据不会导致阻塞。
POLLMSG：SIGPOLL消息可用。

此外，revents域中还可能返回下列事件：
POLLER：指定的文件描述符发生错误。
POLLHUP：指定的文件描述符挂起事件。
POLLNVAL：指定的文件描述符非法。

这些事件在events域中无意义，因为它们在合适的时候总是会从revents中返回。使用poll()和select()不一样，你不需要显式地请求异常情况报告。




POLLIN | POLLPRI等价于select()的读事件，POLLOUT  | POLLWRBAND等价于select()的写事件。POLLIN等价于POLLRDNORM  | POLLRDBAND，而POLLOUT则等价于POLLWRNORM。
例如，要同时监视一个文件描述符是否可读和可写，我们可以设置events为POLLIN | POLLOUT。在poll返回时，我们可以检查revents中的标志，对应于文件描述符请求的events结构体。如果POLLIN事件被设置，则文件描述符可以被读取而不阻塞。如果POLLOUT被设置，则文件描述符可以写入而不导致阻塞。这些标志并不是互斥的：它们可能被同时设置，表示这个文件描述符的读取和写入操作都会正常返回而不阻塞。
timeout参数指定等待的毫秒数，无论I/O是否准备好，poll都会返回。timeout指定为负数值表示无限超时；timeout为0指示poll调用立即返回并列出准备好I/O的文件描述符，但并不等待其它的事件。这种情况下，poll()就像它的名字那样，一旦选举出来，立即返回。
返回值和错误代码
成功时，poll()返回结构体中revents域不为0的文件描述符个数；如果在超时前没有任何事件发生，poll()返回0；失败时，poll()返回-1，并设置errno为下列值之一：
EBADF：一个或多个结构体中指定的文件描述符无效。
EFAULT：fds指针指向的地址超出进程的地址空间。
EINTR：请求的事件之前产生一个信号，调用可以重新发起。
EINVAL：nfds参数超出PLIMIT_NOFILE值。
ENOMEM：可用内存不足，无法完成请求。



 

 **Epoll**

 

------

**Epoll的优点：**
1.支持一个进程打开大数目的socket描述符(FD)
    select 最不能忍受的是一个进程所打开的FD是有一定限制的，由FD_SETSIZE设置，默认值是2048。对于那些需要支持的上万连接数目的IM[服务器](https://www.baidu.com/s?wd=%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)来说显然太少了。这时候你一是可以选择修改这个宏然后重新编译内核，不过资料也同时指出这样会带来网络效率的下降，二是可以选择多进程的解决方案(传统的 Apache方案)，不过虽然linux上面创建进程的代价比较小，但仍旧是不可忽视的，加上进程间[数据同步](https://www.baidu.com/s?wd=%E6%95%B0%E6%8D%AE%E5%90%8C%E6%AD%A5&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)远比不上线程间同步的高效，所以也不是一种完美的方案。不过 epoll则没有这个限制，它所支持的FD上限是最大可以打开文件的数目，这个数字一般远大于2048,举个例子,在1GB内存的机器上大约是10万左右，具体数目可以cat  /proc/sys/fs/file-max察看,一般来说这个数目和系统内存关系很大。

2.IO效率不随FD数目增加而线性下降
    传统的select/poll另一个致命弱点就是当你拥有一个很大的socket集合，不过由于网络延时，任一时间只有部分的socket是"活跃"的，但是select/poll每次调用都会线性扫描全部的集合，导致效率呈现线性下降。但是epoll不存在这个问题，它只会对"活跃"的socket进行操作---这是因为在内核实现中epoll是根据每个fd上面的callback函数实现的。那么，只有"活跃"的socket才会主动的去调用 callback函数，其他idle状态socket则不会，在这点上，epoll实现了一个"伪"AIO，因为这时候推动力在os内核。在一些 benchmark中，如果所有的socket基本上都是活跃的---比如一个高速LAN环境，epoll并不比select/poll有什么效率，相反，如果过多使用epoll_ctl,效率相比还有稍微的下降。但是一旦使用idle  connections[模拟](https://www.baidu.com/s?wd=%E6%A8%A1%E6%8B%9F&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)WAN环境,epoll的效率就远在select/poll之上了。

3.使用mmap加速内核与用户空间的消息传递。
    这点实际上涉及到epoll的具体实现了。无论是select,poll还是epoll都需要内核把FD消息通知给用户空间，如何避免不必要的内存拷贝就很重要，在这点上，epoll是通过内核于用户空间mmap同一块内存实现的。而如果你想我一样从2.5内核就关注epoll的话，一定不会忘记手工 mmap这一步的。



**Epoll简介：**

在linux的网络编程中，很长的时间都在使用select来做事件触发。在linux新的内核中，有了一种替换它的机制，就是epoll。
相比于select，epoll最大的好处在于它不会随着监听fd数目的增长而降低效率。因为在内核中的select实现中，它是采用轮询来处理的，轮询的fd数目越多，自然耗时越多。

epoll的接口非常简单，一共就三个函数：
\1. int epoll_create(int size);
创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大。这个参数不同于select()中的第一个参数，给出最大监听的fd+1的值。需要注意的是，当创建好epoll句柄后，它就是会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，必须调用close()关闭，否则可能导致fd被耗尽。


\2. int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
epoll的事件注册函数，它不同与select()是在监听事件时告诉内核要监听什么类型的事件，而是在这里先注册要监听的事件类型。第一个参数是epoll_create()[上面一个函数]的返回值，第二个参数表示动作，用三个宏来表示：
EPOLL_CTL_ADD：注册新的fd到epfd中；
EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
EPOLL_CTL_DEL：从epfd中删除一个fd；
第三个参数是需要监听的fd，第四个参数是告诉内核需要监听什么事，struct epoll_event结构如下：
struct epoll_event {
  __uint32_t events;  /* Epoll events */
  epoll_data_t data;  /* User data variable */
};

events可以是以下几个宏的集合：
EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge  Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
**EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里**


\3. int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
等待事件的产生，类似于select()调用。参数events用来从内核得到事件的集合，maxevents告之内核这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size，参数timeout是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。该函数返回需要处理的事件数目，如返回0表示已超时。



 

令人高兴的是，2.6内核的epoll比其2.5开发版本的/dev/epoll简洁了许多，所以，大部分情况下，强大的东西往往是简单的。唯一有点麻烦是epoll有2种工作方式：



**LT和ET(水平触发和边缘触发)**

------

　　LT（level  triggered）是缺省的工作方式，并且同时支持block和no-block  socket.在这种做法中，内核告诉你一个文件描述符是否就绪了，然后你可以对这个就绪的fd进行IO操作。如果你不作任何操作，内核还是会继续通知你的，所以，这种模式编程出错误可能性要小一点。传统的select/poll都是这种模型的代表。

　　ET  （edge-triggered）是高速工作方式，只支持no-block  socket。在这种模式下，当描述符从未就绪变为就绪时，内核通过epoll告诉你。然后它会假设你知道文件描述符已经就绪，并且不会再为那个文件描述符发送更多的就绪通知，直到你做了某些操作导致那个文件描述符不再为就绪状态了（比如，你在发送，接收或者接收请求，或者发送接收的数据少于一定量时导致了一个EWOULDBLOCK  错误）。但是请注意，如果一直不对这个fd作IO操作（从而导致它再次变成未就绪），内核不会发送更多的通知（only  once），不过在TCP协议中，ET模式的加速效用仍需要更多的benchmark确认。

第二部分：Epoll的三个例子



epoll用到的所有函数都是在头文件sys/epoll.h中声明的，下面简要说明所用到的数据结构和函数：



所用到的数据结构



```cpp
typedef union epoll_data
{
    void *ptr;
    int fd;
    __uint32_t u32;
    __uint64_t u64;
} epoll_data_t;

struct epoll_event
{
    __uint32_t events;      /* Epoll events */
    epoll_data_t data;      /* User data variable */
};
```


结构体epoll_event  被用于注册所感兴趣的事件和回传所发生待处理的事件，其中epoll_data  联合体用来保存触发事件的某个文件描述符相关的数据，例如一个client连接到服务器，服务器通过调用accept函数可以得到于这个client对应的socket文件描述符，可以把这文件描述符赋给epoll_data的fd字段以便后面的读写操作在这个文件描述符上进行。epoll_event  结构体的events字段是表示感兴趣的事件和被触发的事件可能的取值为：EPOLLIN  ：表示对应的文件描述符可以读；

EPOLLOUT：表示对应的文件描述符可以写；

EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；

EPOLLERR：表示对应的文件描述符发生错误；

EPOLLHUP：表示对应的文件描述符被挂断；

EPOLLET：表示对应的文件描述符有事件发生；

所用到的函数：

1、epoll_create函数

​     函数声明：int epoll_create(int size) 

​    该函数生成一个epoll专用的文件描述符，其中的参数是指定生成描述符的最大范围（我觉得这个参数和select函数的第一个参数应该是类似的但是该怎么设置才好，我也不太清楚）。

2、epoll_ctl函数

​     函数声明：int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)

​     该函数用于控制某个文件描述符上的事件，可以注册事件，修改事件，删除事件。

​    参数：epfd：由 epoll_create 生成的epoll专用的文件描述符；

​                op：要进行的操作例如注册事件，可能的取值：

​                        EPOLL_CTL_ADD 注册；

​                        EPOLL_CTL_MOD 修改；

​                        EPOLL_CTL_DEL 删除

​                fd：关联的文件描述符；

​                event：指向epoll_event的指针；

​    如果调用成功返回0,不成功返回-1

3、epoll_wait函数

​    函数声明:int epoll_wait(int epfd,struct epoll_event * events,int maxevents,int timeout)

> 该函数用于轮询I/O事件的发生；
>
> 参数：
>
> epfd:由epoll_create 生成的epoll专用的文件描述符；
>
> epoll_event:用于回传代处理事件的数组；
>
> maxevents:每次能处理的事件数；
>
> timeout:等待I/O事件发生的超时值；
>
> 返回发生事件数。



**例子1**



```c
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define MAXLINE 10
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5555
#define INFTIM 1000

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if(opts < 0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if(fcntl(sock, F_SETFL, opts) < 0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

int main()
{
    int i, maxi, listenfd, connfd, sockfd, epfd, nfds;
    ssize_t n;
    char line[MAXLINE];
    socklen_t clilen;
    //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
    struct epoll_event ev, events[20];
    //生成用于处理accept的epoll专用的文件描述符
    epfd = epoll_create(256);

    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //把socket设置为非阻塞方式
    setnonblocking(listenfd);
    //设置与要处理的事件相关的文件描述符
    ev.data.fd = listenfd;
    //设置要处理的事件类型
    ev.events = EPOLLIN | EPOLLET;
    //注册epoll事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    char *local_addr = "200.200.200.204";
    inet_aton(local_addr, &(serveraddr.sin_addr)); //htons(SERV_PORT);
    serveraddr.sin_port = htons(SERV_PORT);
    bind(listenfd, (sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);

    maxi = 0;
    for ( ; ; )
    {
        //等待epoll事件的发生
        nfds = epoll_wait(epfd, events, 20, 500);
        //处理所发生的所有事件
        for(i = 0; i < nfds; ++i)
        {
            if(events[i].data.fd == listenfd)
            {

                connfd = accept(listenfd, (sockaddr *)&clientaddr, &clilen);
                if(connfd < 0)
                {
                    perror("connfd<0");
                    exit(1);
                }
                setnonblocking(connfd);

                char *str = inet_ntoa(clientaddr.sin_addr);
                std::cout << "connect from " < _u115 ? tr << std::endl;
                //设置用于读操作的文件描述符
                ev.data.fd = connfd;
                //设置用于注测的读操作事件
                ev.events = EPOLLIN | EPOLLET;
                //注册ev
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            }
            else if(events[i].events & EPOLLIN)
            {
                if ( (sockfd = events[i].data.fd) < 0) continue;
                if ( (n = read(sockfd, line, MAXLINE)) < 0)
                {
                    if (errno == ECONNRESET)
                    {

                        close(sockfd);
                        events[i].data.fd = -1;
                    }
                    else
                        std::cout << "readline error" << std::endl;
                }
                else if (n == 0)
                {
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                //设置用于写操作的文件描述符
                ev.data.fd = sockfd;
                //设置用于注测的写操作事件
                ev.events = EPOLLOUT | EPOLLET;
                //修改sockfd上要处理的事件为EPOLLOUT
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
            else if(events[i].events & EPOLLOUT)
            {
                sockfd = events[i].data.fd;
                write(sockfd, line, n);
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
```





**例子2**

```c
/*

*\ 服务器端的源代码

*/

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <sys/epoll.h>

#define MAXFDS 256
#define EVENTS 100
#define PORT 8888

int epfd;
bool setNonBlock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    if(-1 == fcntl(fd, F_SETFL, flags))
        return false;
    return true;
}

int main(int argc, char *argv[], char *evp[])
{
    int fd, nfds, confd;
    int on = 1;
    char *buffer[512];
    struct sockaddr_in saddr, caddr;
    struct epoll_event ev, events[EVENTS];

    if(-1 == socket(AF_INET, SOCKSTREAM), 0)
    {
        std::cout << "创建套接字出错啦" << std::endl;
        return -1;
    }


    struct sigaction sig;
    sigemptyset(&sig.sa_mask);
    sig_handler = SIG_IGN;
    sigaction(SIGPIPE, &N > sig, NULL);

    epfd = epoll_create(MAXFDS);

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons((short)(PORT));
    saddr.sin_addr.s_addr = INADDR_ANY;
    if(-1 == bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)))
    {
        std::cout << "套接字不能绑定到服务器上" << std::endl;
        return -1;
    }

    if(-1 == listen(fd, 32))
    {
        std::cout << "监听套接字的时候出错了" << std::endl;
        return -1;
    }

    ev.data.fd = fd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

    while(true)
    {
        nfds = epoll_wait(epfd, &events, MAXFDS, 0);

        for(int i = 0; i < nfds; ++ i)
        {
            if(fd == events[i].data.fd)
            {
                memset(&caddr, sizeof(caddr));
                cfd = accept(fd, (struct sockaddr *)&caddr, &sizeof(caddr));
                if(-1 == cfd)
                {
                    std::cout << "服务器接收套接字的时候出问题了" << std::endl;
                    break;
                }

                setNonBlock(cfd);

                ev.data.fd = cfd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            else if(events[i].data.fd & EPOLLIN)
            {
                bzero(&buffer, sizeof(buffer));
                std::cout << "服务器端要读取客户端发过来的消息" << std::endl;
                ret = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if(ret < 0)
                {
                    std::cout << "服务器收到的消息出错了" << endl;
                    return -1;
                }
                std::cout << "接收到的消息为：" << (char *) buffer << std::endl;
                ev.data.fd = events[i].data.fd;
                ev.events = EPOLLOUT;
                epoll_ctl(epfd, EPOLL_CTL_MOD, events[i].data.fd, &ev);
            }
            else if(events[i].data.fd & EPOLLOUT)
            {
                bzero(&buffer, sizeof(buffer));
                bcopy("The Author@: magicminglee@Hotmail.com", buffer, sizeof("The Author@: magicminglee@Hotmail.com"));
                ret = send(events[i].data.fd, buffer, strlen(buffer));
                if(ret < 0)
                {
                    std::cout << "服务器发送消息给客户端的时候出错啦" << std::endl;
                    return -1;
                }
                ev.data.fd = events[i].data.fd;
                epoll_ctl(epfd, EPOLL_CTL_DEL, ev.data.fd, &ev);
            }
        }
    }
    if(fd > 0)
    {
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }
}
```

```c
/*

*\ 客户端源代码

*/
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8888

int main(int argc, char *argv[], char *evp[])
{
    int fd;
    int on = 1;
    char *buffer[512];

    struct sockaddr_in seraddr;
    memset(&seraddr, 0, sizeof(seraddr));

    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "客户端创建套接字出错了" << std::endl;
        return -1;
    }

    //如果用于多次测试，那么打开下面debug选项

#ifdef _Debug_ming
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
#endif

    seraddr.sin_port = htons((short)(PORT));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");//设置自己的ip吧

    //你也可以采用无阻塞连接，不过需要对连接的错误结果进行分析处理

    if(TEMP_FAILURE_RETRY(connect(fd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0))
    {
        std::cout << "连接错误了" << std::endl;
        return -1;
    }
    //下面就进行收发信息

    bcopy("The Author@: magicminglee@Hotmail.com");
    send(fd, buffer, strlen(buffer), 0);
    bzero(&buffer, sizeof(buffer));
    recv(fd, buffer, sizeof(buffer), 0);
    exit(0);
}
```



**例子3**

------

一个使用epoll的服务器

```c
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAXLINE 1024
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5555
#define INFTIM 1000

//线程池任务队列结构体
struct task
{
    int fd;            //需要读写的文件描述符
    struct task *next; //下一个任务
};
//用于保存向客户端发送一次消息所需的相关数据
struct user_data
{
    int fd;
    unsigned int n_size;
    char line[MAXLINE];
};
//线程的任务函数
void *readtask(void *args);
void *writetask(void *args);
//声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
struct epoll_event ev, events[20];
int epfd;
pthread_mutex_t mutex;
pthread_cond_t cond1;
struct task *readhead = NULL, *readtail = NULL, *writehead = NULL;
void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if(opts < 0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if(fcntl(sock, F_SETFL, opts) < 0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}
int main()
{
    int i, maxi, listenfd, connfd, sockfd, nfds;
    pthread_t tid1, tid2;
    struct task *new_task = NULL;
    struct user_data *rdata = NULL;
    socklen_t clilen;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond1, NULL);
    //初始化用于读线程池的线程，开启两个线程来完成任务，两个线程会互斥地访问任务链表
    pthread_create(&tid1, NULL, readtask, NULL);
    pthread_create(&tid2, NULL, readtask, NULL);
    //生成用于处理accept的epoll专用的文件描述符
    epfd = epoll_create(256);
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //把socket设置为非阻塞方式
    setnonblocking(listenfd);
    //设置与要处理的事件相关的文件描述符
    ev.data.fd = listenfd;
    //设置要处理的事件类型，当描述符可读时出发，出发方式为ET模式
    ev.events = EPOLLIN | EPOLLET;
    //注册epoll事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    const char *local_addr = "127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr)); //htons(SERV_PORT);
    serveraddr.sin_port = htons(SERV_PORT);
    bind(listenfd, (sockaddr *)&serveraddr, sizeof(serveraddr));
    //开始监听
    listen(listenfd, LISTENQ);
    maxi = 0;
    for ( ; ; )
    {
        //等待epoll事件的发生
        nfds = epoll_wait(epfd, events, 20, 500);
        //处理所发生的所有事件
        for(i = 0; i < nfds; ++i)
        {
            if(events[i].data.fd == listenfd)
            {
                connfd = accept(listenfd, (sockaddr *)&clientaddr, &clilen);
                if(connfd < 0)
                {
                    perror("connfd<0");
                    exit(1);
                }
                setnonblocking(connfd);
                const char *str = inet_ntoa(clientaddr.sin_addr);
                std::cout << "connec_ from >> " << str << std::endl;
                //设置用于读操作的文件描述符
                ev.data.fd = connfd;
                //设置用于注测的读操作事件
                ev.events = EPOLLIN | EPOLLET;
                //注册ev
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            }
            else if(events[i].events & EPOLLIN)
            {
                printf("reading!\n");
                if ( (sockfd = events[i].data.fd) < 0) continue;
                new_task = new task();
                new_task->fd = sockfd;
                new_task->next = NULL;
                //添加新的读任务
                pthread_mutex_lock(&mutex);
                if(readhead == NULL)
                {
                    readhead = new_task;
                    readtail = new_task;
                }
                else
                {
                    readtail->next = new_task;
                    readtail = new_task;
                }
                //唤醒所有等待cond1条件的线程
                pthread_cond_broadcast(&cond1);
                pthread_mutex_unlock(&mutex);
            }
            else if(events[i].events & EPOLLOUT)
            {
                rdata = (struct user_data *)events[i].data.ptr;
                sockfd = rdata->fd;
                write(sockfd, rdata->line, rdata->n_size);
                delete rdata;
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
void *readtask(void *args)
{
    int fd = -1;
    unsigned int n;
    //用于把读出来的数据传递出去
    struct user_data *data = NULL;
    while(1)
    {
        //互斥访问任务队列
        pthread_mutex_lock(&mutex);
        //等待到任务队列不为空
        while(readhead == NULL)
            pthread_cond_wait(&cond1, &mutex); //线程阻塞，释放互斥锁，当等待的条件等到满足时，它会再次获得互斥锁
        fd = readhead->fd;
        //从任务队列取出一个读任务
        struct task *tmp = readhead;
        readhead = readhead->next;
        delete tmp;
        pthread_mutex_unlock(&mutex);
        data = new user_data();
        data->fd = fd;
        if ( (n = read(fd, data->line, MAXLINE)) < 0)
        {
            if (errno == ECONNRESET)
                close(fd);
            else
                std::cout << "readline error" << std::endl;
            if(data != NULL) delete data;
        }
        else if (n == 0)
        {
            //客户端关闭了，其对应的连接套接字可能也被标记为EPOLLIN，然后服务器去读这个套接字
            //结果发现读出来的内容为0，就知道客户端关闭了。
            close(fd);
            printf("Client close connect!\n");
            if(data != NULL) delete data;
        }
        else
        {
            std::cout << "read from client: " << data->line << std::endl;
            data->n_size = n;
            //设置需要传递出去的数据
            ev.data.ptr = data;
            //设置用于注测的写操作事件
            ev.events = EPOLLOUT | EPOLLET;
            //修改sockfd上要处理的事件为EPOLLOUT
            epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
        }
    }
}
```



=========================================================

给出一个简单的客户端吧，从《Linux编程技术详解》书中拷贝而来。



```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int connect_fd;
    int ret;
    char snd_buf[1024];
    int i;
    int port;
    int len;

    static struct sockaddr_in srv_addr;
    if(argc != 3)
    {
        printf("Usage: %s server_ip_address port\n", argv[0]);
        return 1;
    }
    port = atoi(argv[2]);
    connect_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(connect_fd < 0)
    {
        perror("cannot create communication socket");
        return 1;
    }
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    srv_addr.sin_port = htons(port);
    ret = connect(connect_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if(ret == -1)
    {
        perror("cannot connect to the server");
        close(connect_fd);
        return 1;
    }
    memset(snd_buf, 0, 1024);
    while(1)
    {
        write(STDOUT_FILENO, "input message:", 14);
        bzero(snd_buf, 1024);
        len = read(STDIN_FILENO, snd_buf, 1024);
        if(snd_buf[0] == '@')
            break;
        if(len > 0)
            write(connect_fd, snd_buf, len);
        len = read(connect_fd, snd_buf, len);
        if(len > 0)
            printf("Message from server: %s\n", snd_buf);
    }
    close(connect_fd);
    return 0;
}
```



===========================================

```shell
ecy@ecy-geek:~/C$ ./epoll_server 

connec_ from >> 127.0.0.1

reading!

read from client: ni hao ya ya ya ya ya



reading!

read from client: hello world



reading!

Client close connect!



ecy@ecy-geek:~/C$ pstree | grep epoll

|-gnome-terminal-+-bash---epoll_server---2*[{epoll_server}]



ecy@ecy-geek:~/C$ ./p13.5 127.0.0.1 5555

input message:ni hao ya ya ya ya ya

Message from server: ni hao ya ya ya ya ya



input message:hello world

Message from server: hello world



input message:@
```

