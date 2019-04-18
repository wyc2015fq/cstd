# libevent介绍 - 深之JohnChen的专栏 - CSDN博客

2011年07月18日 11:24:45[byxdaz](https://me.csdn.net/byxdaz)阅读数：1887标签：[struct																[signal																[callback																[library																[引擎																[buffer](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=引擎&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
个人分类：[网络编程																[开源软件](https://blog.csdn.net/byxdaz/article/category/577652)](https://blog.csdn.net/byxdaz/article/category/166215)


###### 通常我们在建立服务器的处理模型的时候,主要是下面集中模型;

######  (1)    a new Connection 进来，用 fork() 产生一个 Process 处理。   (2)   a new Connection 进来，用 pthread_create() 产生一个 Thread 处理。   (3)   a new Connection 进来，丢入 Event-based Array，由 Main Process 以 Nonblocking 的方式处理所有的 I/O。这三种方法当然也都有各自的缺点： 用 fork() 的问题在于每一个 Connection 进来时的成本太高,如果同时接入的并发连接数太多容易进程数量很多,进程之间的切换开销会很大,同时对于老的内核(Linux)会产生雪崩效应。  用 Multi-thread 的问题在于 Thread-safe 与 Deadlock 问题难以解决，另外有 Memory-leak 的问题要处理,这个问题对于很多程序员来说无异于恶梦,尤其是对于连续服务器的服务器程序更是不可以接受。 如果才用 Event-based 的方式在于实做上不好写，尤其是要注意到事件产生时必须 Nonblocking，于是会需要实做 Buffering 的问题，而 Multi-thread 所会遇到的 Memory-leak 问题在这边会更严重。而在多 CPU 的系统上没有办法使用到所有的 CPU resource。 

######      针对上面存在的问题,通常采用的方法有: 以 Poll 的方式解决：当一个 Process 处理完一个 Connection 后，不直接死掉，而继续回到 accept() 的状态继续处理，但这样会遇到 Memory-leak 的问题，于是采用这种方式的人通常会再加上「处理过 N 个 Connection 后死掉，由 Parent Process 再 fork() 一只新的」。最有名的例子是 Apache 1.3服务器,大家可以参考其源代码的实现。 hread-safe 的问题可以寻找其他 Thread-safe Library 直接使用。Memory-leak 的问题可以试着透过 Garbage Collection Library 分析出来。Apache 2.0 的 Thread MPM 就是使用这个模式。     然而，目前高效率的 Server 都偏好采用 Event-based，一方面是没有 Create Process/Thread 所造成的 Overhead，另外一方面是不需要透过 Shared Memory 或是 Mutex 在不同的 Process/Thread 之间交换资料。然而，Event-based 在实做上的几个复杂的地方在于： select() 与 poll() 的效率过慢，造成每次要判断「有哪些 Event 发生」这件事情的成本很高，这在 BSD 支援 kqueue()、Linux 支援 epoll()、Solaris 支援 /dev/poll 后就解决了,在Windows平台上通过完成端口的方式解决了.但这两组 Function 都不是 Standard，于是在不同的平台上就必须再改一次。

###### 对于非阻塞的IO模型, 因为 Nonblocking，所以在 write() 或是 send() 时满了需要自己 Buffering。  因为 Nonblocking，所以不能使用 fgets() 或是其他类似的 function，于是需要自己刻一个 Nonblocking 的 fgets()。但是使用者所丢过来的资料又不能保证在一次 read() 或 recv() 就有一行，于是要自己做 Buffering。实际上这三件事情在 libevent 都有 Library 处理掉了.

######    libevent 是一个事件触发的网络库，适用于windows、linux、bsd等多种平台，内部使用select、epoll、kqueue等系统调用管理事件机 制。著名的用于apache的php缓存库memcached据说也是libevent based，而且libevent在使用上可以做到跨平台，如果你将要开发的应用程序需要支持以上所列出的平台中的两个以上，那么强烈建议你采用这个库，即使你的应用程序只需要支持一个平台，选择libevent也是有好处的，因为它可以根据编译/运行环境切换底层的事件驱动机制，这既能充分发挥系统的性能，又增加了软件的可移植性。它封装并且隔离了事件驱动的底层机制，除了一般的文件描述符读写操作外，它还提供有读写超时、定时器和信号回调，另外，它还允许为事件设定不同的优先级，当前版本的libevent还提供dns和http协议的异步封装，这一切都让这个库尤其适合于事件驱动应用程序的开发。

######    下面介绍libevent实现的框架

###### 原文请参考:libevent官方网址:   [http://www.monkey.org/~provos/libevent/](http://www.monkey.org/~provos/libevent/)比较好的文档:[http://unx.ca/log/category/libevent/](http://unx.ca/log/category/libevent/)

###### [http://tb.blog.csdn.net/TrackBack.aspx?PostId=1808095](http://tb.blog.csdn.net/TrackBack.aspx?PostId=1808095)

###### libenvent库的代码结构可以大概分成几个模块:    事件处理框架   事件引擎模块   Buffer管理模块 信号处理模块

######   1. 事件处理框架 1.1 event_init() 初始化  首先要隆重介绍event_base对象:

###### struct event_base {    const struct eventop *evsel;    void *evbase;    int event_count;        /* counts number of total events */    int event_count_active; /* counts number of active events */    int event_gotterm;      /* Set to terminate loop */    /* active event management */    struct event_list **activequeues;    int nactivequeues;    struct event_list eventqueue;    struct timeval event_tv;    RB_HEAD(event_tree, event) timetree;};

######    event_base对象整合了事件处理的一些全局变量,  角色是event对象的"总管家", 他包括了事件引擎函数对象(evsel, evbase), 当前入列事件列表(event_count, event_count_active, eventqueue), 全局终止信号(event_gotterm), 活跃事件列表(avtivequeues), 事件队列树(timetree)...初始化时创建event_base对象, 选择 当前OS支持的事件引擎(epoll, poll, select...)并初始化, 创建全局信号队列(signalqueue), 活跃队列的内存分配( 根据设置的priority个数,默认为1). 1.2 event_set() 事件定义    event_set来设置event对象,包括所有者event_base对象, fd, 事件(EV_READ| EV_WRITE), 回掉函数和参数,事件优先级是当前event_base的中间级别(current_base->nactivequeues/2). event对象的定义见下:

###### struct event {    TAILQ_ENTRY (event) ev_next;    TAILQ_ENTRY (event) ev_active_next;    TAILQ_ENTRY (event) ev_signal_next;    RB_ENTRY (event) ev_timeout_node;    struct event_base *ev_base;    int ev_fd;    short ev_events;    short ev_ncalls;    short *ev_pncalls;  /* Allows deletes in callback */    struct timeval ev_timeout;    int ev_pri;     /* smaller numbers are higher priority */    void (*ev_callback)(int, short, void *arg);    void *ev_arg;    int ev_res;     /* result passed to event callback */    int ev_flags;};

###### 1.3 event_add() 事件添加:    int event_add(struct event *ev, struct timeval *tv)   这个接口有两个参数, 第一个是要添加的事件, 第二个参数作为事件的超时值(timer). 如果该值非NULL, 在添加本事件的同时添加超时事件(EV_TIMEOUT)到时间队列树(timetree), 根据事件类型处理如下:      EV_READ  =>  EVLIST_INSERTED  => eventqueue   EV_WRITE  =>  EVLIST_INSERTED  => eventqueue   EV_TIMEOUT => EVLIST_TIMEOUT => timetree  EV_SIGNAL  => EVLIST_SIGNAL => signalqueue1.4 event_base_loop() 事件处理主循环    这里是事件的主循环,只要flags不是设置为EVLOOP_NONBLOCK, 该函数就会一直循环监听事件/处理事件.   每次循环过程中, 都会处理当前触发(活跃)事件:   (a). 检测当前是否有信号处理(gotterm, gotsig), 这些都是全局参数,不适合多线程   (b). 时间更新,找到离当前最近的时间事件, 得到相对超时事件tv   (c). 调用事件引擎的dispatch wait事件触发, 超时值为tv, 触发事件添加到activequeues   (d). 处理活跃事件, 调用caller的callbacks (event_process_acitve)2. 事件引擎模块 :   Linux下有多种I/O复用机制, .来处理多路事件监听, 常见的有epoll, poll, select, 按照优先级排下来为: evport kqueue epoll devpoll rtsig poll select   在event_init()选择事件引擎时,按照优先级从上向下检测, 如果检测成功,当前引擎被选中.每个引擎需要定义几个处理函数,以epoll为例:

###### struct eventop epollops = {    "epoll",    epoll_init,    epoll_add,    epoll_del,    epoll_recalc,    epoll_dispatch,    epoll_dealloc};

###### 3. Buffer管理模块:    libevent定义了自己的buffer管理机制evbuffer, 支持多种类型数据的read/write功能, 包括不定长字符串,buffer中内存采用预分配/按需分配结合的方式, 可以比较方便的管理多个数据结构映射到内存buffer.   需要拉出来介绍的是evbuffer_expand()函数, 当内部内存不够时,需要expand, 这里采用预分配的方式,如果需要长度<256字节,预分配256字节, 同时内存成倍增长,一直到大于需要的长度.4.  信号处理模块   信号处理单独提出来,主要是libevent的信号处理比较轻巧, 从而很好融合到event机制.   singal模块初始化(evsignal_init)时, 创建了UNIX域socket ( pipe)作为内部消息传递桥梁:

######     if (socketpair(AF_UNIX, SOCK_STREAM, 0, ev_signal_pair) == -1)        event_err(1, "%s: socketpair", __func__);    FD_CLOSEONEXEC(ev_signal_pair[0]);    FD_CLOSEONEXEC(ev_signal_pair[1]);    fcntl(ev_signal_pair[0], F_SETFL, O_NONBLOCK);    event_set(&ev_signal, ev_signal_pair[1], EV_READ,        evsignal_cb, &ev_signal);    ev_signal.ev_flags |= EVLIST_INTERNAL;

######    evsignal_add(), 添加信号事件, 关联信号处理方法(sigaction)   实际运行过程中,如果某singal发生, 对应的信号处理方法被调用, write a character to pipe   同时pipe的另一端被激活, 添加信号到singalqueue, 在事件循环中evsignal_process处理信号callbacks.

###### libevent库的具体使用方法

######    直接写一个很简单的 Time Server 来当作例子：当你连上去以后 Server 端直接提供时间，然后结束连线。event_init() 表示初始化 libevent 所使用到的变数。event_set(&ev, s, EV_READ | EV_PERSIST, connection_accept, &ev) 把 s 这个 File Description 放入 ev (第一个参数与第二个参数)，并且告知当事件 (第三个参数的 EV_READ) 发生时要呼叫 connection_accept() (第四个参数)，呼叫时要把 ev 当作参数丢进去 (第五个参数)。其中的 EV_PERSIST 表示当呼叫进去的时候不要把这个 event 拿掉 (继续保留在 Event Queue 里面)，这点可以跟 connection_accept() 内在注册 connection_time() 的代码做比较。而 event_add(&ev, NULL) 就是把 ev 注册到 event queue 里面，第二个参数指定的是 Timeout 时间，设定成 NULL 表示忽略这项设定。

###### 注:这段代码来自于网络,虽然很粗糙,但是对libevent的使用方法已经说明的很清楚了.

###### 附源码:使用方法

######  #include <netinet/in.h>#include <sys/socket.h>#include <sys/types.h>#include <event.h>#include <stdio.h>#include <time.h>

###### void connection_time(int fd, short event, struct event *arg){    char buf[32];    struct tm t;    time_t now;

######     time(&now);    localtime_r(&now, &t);    asctime_r(&t, buf);

######     write(fd, buf, strlen(buf));    shutdown(fd, SHUT_RDWR);

######     free(arg);}

###### void connection_accept(int fd, short event, void *arg){    /* for debugging */    fprintf(stderr, "%s(): fd = %d, event = %d.\n", __func__, fd, event);

######     /* Accept a new connection. */    struct sockaddr_in s_in;    socklen_t len = sizeof(s_in);    int ns = accept(fd, (struct sockaddr *) &s_in, &len);    if (ns < 0) {        perror("accept");        return;    }

######     /* Install time server. */    struct event *ev = malloc(sizeof(struct event));    event_set(ev, ns, EV_WRITE, (void *) connection_time, ev);    event_add(ev, NULL);}

###### int main(void){    /* Request socket. */    int s = socket(PF_INET, SOCK_STREAM, 0);    if (s < 0) {        perror("socket");        exit(1);    }

######     /* bind() */    struct sockaddr_in s_in;    bzero(&s_in, sizeof(s_in));    s_in.sin_family = AF_INET;    s_in.sin_port = htons(7000);    s_in.sin_addr.s_addr = INADDR_ANY;    if (bind(s, (struct sockaddr *) &s_in, sizeof(s_in)) < 0) {        perror("bind");        exit(1);    }

######     /* listen() */    if (listen(s, 5) < 0) {        perror("listen");        exit(1);    }

######     /* Initial libevent. */    event_init();

######     /* Create event. */    struct event ev;    event_set(&ev, s, EV_READ | EV_PERSIST, connection_accept, &ev);

######     /* Add event. */    event_add(&ev, NULL);

######     event_dispatch();

######     return 0;}

###### 在写 Nonblocking Network Program 通常要处理 Buffering 的问题，但并不好写，主要是因为 read() 或 recv() 不保证可以一次读到一行的份量进来。

###### 在 libevent 里面提供相当不错的 Buffer Library 可以用，完整的说明在 man event 的时候可以看到，最常用的应该就是以 evbuffer_add()、evbuffer_readline() 这两个 Function，其他的知道存在就可以了，需要的时候再去看详细的用法。

###### 下面直接提供 libevent-buff.c 当作范例，编译后看执行结果，再回头来看 source code 应该就有感觉了：

###### #include <sys/time.h>#include <event.h>#include <stdio.h>

###### void printbuf(struct evbuffer *evbuf){    for (;;) {        char *buf = evbuffer_readline(evbuf);        printf("* buf = %p, the string = \"\e[1;33m%s\e[m\"\n", buf, buf);        if (buf == NULL)            break;        free(buf);    }}

###### int main(void){    struct evbuffer *evbuf;

######     evbuf = evbuffer_new();    if (evbuf == NULL) {        fprintf(stderr, "%s(): evbuffer_new() failed.\n", __func__);        exit(1);    }

######     /* Add "gslin" into buffer. */    u_char *buf1 = "gslin";    printf("* Add \"\e[1;33m%s\e[m\".\n", buf1);    evbuffer_add(evbuf, buf1, strlen(buf1));    printbuf(evbuf);

######     u_char *buf2 = " is reading.\nAnd he is at home.\nLast.";    printf("* Add \"\e[1;33m%s\e[m\".\n", buf2);    evbuffer_add(evbuf, buf2, strlen(buf2));    printbuf(evbuf);

######     evbuffer_free(evbuf);}

###### 最后的 event_dispatch() 表示进入 event loop，当 Queue 里面的任何一个 File Description 发生事件的时候就会进入 callback function 执行。


