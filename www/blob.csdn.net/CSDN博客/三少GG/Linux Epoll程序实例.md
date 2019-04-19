# Linux Epoll程序实例 - 三少GG - CSDN博客
2013年12月26日 14:28:26[三少GG](https://me.csdn.net/scut1135)阅读数：1080
**Epoll为我们带来什么**
||Q：网络服务器的瓶颈在哪？A：IO效率。　　在大家苦苦的为在线人数的增长而导致的系统资源吃紧上的问题正在发愁的时候，Linux 2.6内核中提供的System Epoll为我们提供了一套完美的解决方案。传统的select以及poll的效率会因为在线人数的线形递增而导致呈二次乃至三次方的下降，这些直接导致了网络服务器可以支持的人数有了个比较明显的限制。　　自从Linux提供了/dev/epoll的设备以及后来2.6内核中对/dev/epoll设备的访问的封装（System Epoll）之后，这种现象得到了大大的缓解，如果说几个月前，大家还对epoll不熟悉，那么现在来说的话，epoll的应用已经得到了大范围的普及。　　那么究竟如何来使用epoll呢？其实非常简单。　　通过在包含一个头文件#include <sys/epoll.h>以及几个简单的API将可以**大大的提高你的网络服务器的支持人数**。　　首先通过epoll_create(int maxfds)来创建一个epoll的句柄，其中maxfds为你epoll所支持的最大句柄数。这个函数会返回一个新的epoll句柄，之后的所有操作将通过这个句柄来进行操作。在用完之后，记得用close()来关闭这个创建出来的epoll句柄。　　之后在你的网络主循环里面，每一帧的调用epoll_wait(int epfd, epoll_event events, int max events, int timeout)来查询所有的网络接口，看哪一个可以读，哪一个可以写了。基本的语法为：　　nfds = epoll_wait(kdpfd, events, maxevents, -1);　　其中kdpfd为用epoll_create创建之后的句柄，events是一个epoll_event*的指针，当epoll_wait这个函数操作成功之后，epoll_events里面将储存所有的读写事件。max_events是当前需要监听的所有socket句柄数。最后一个timeout是epoll_wait的超时，为0的时候表示马上返回，为-1的时候表示一直等下去，直到有事件范围，为任意正整数的时候表示等这么长的时间，如果一直没有事件，则范围。一般如果网络主循环是单独的线程的话，可以用-1来等，这样可以保证一些效率，如果是和主逻辑在同一个线程的话，则可以用0来保证主循环的效率。epoll_wait范围之后应该是一个循环，遍利所有的事件：    for(n = 0; n < nfds; ++n) {if(events[n].data.fd == listener) { //如果是主socket的事件的话，则表示有新连接进入了，进行新连接的处理。client = accept(listener, (struct sockaddr *) &local, &addrlen);if(client < 0){perror("accept");continue;}setnonblocking(client); // 将新连接置于非阻塞模式ev.events = EPOLLIN | EPOLLET; // 并且将新连接也加入EPOLL的监听队列。　　注意，这里的参数EPOLLIN | EPOLLET并没有设置对写socket的监听，如果有写操作的话，这个时候epoll是不会返回事件的，如果要对写操作也监听的话，应该是EPOLLIN | EPOLLOUT | EPOLLETev.data.fd = client;if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, client, &ev) < 0) {// 设置好event之后，将这个新的event通过epoll_ctl加入到epoll的监听队列里面，这里用EPOLL_CTL_ADD来加一个新的epoll事件，通过EPOLL_CTL_DEL来减少一个epoll事件，通过EPOLL_CTL_MOD来改变一个事件的监听方式。fprintf(stderr, "epoll set insertion error: fd=%d0, client);return -1;}}else // 如果不是主socket的事件的话，则代表是一个用户socket的事件，则来处理这个用户socket的事情，比如说read(fd,xxx)之类的，或者一些其他的处理。do_use_fd(events[n].data.fd);}　　对，epoll的操作就这么简单，总共不过4个API：epoll_create, epoll_ctl, epoll_wait和close。　　如果您对epoll的效率还不太了解，请参考我之前关于网络游戏的网络编程等相关的文章。　　世界变了，原来担心的问题，现在已经不是问题了。|
Linux Epoll介绍和程序实例
1. Epoll是何方神圣？
Epoll可是当前在Linux下开发大规模并发网络程序的热门人选，Epoll 在Linux2.6内核中正式引入，和select相似，其实都**I/O多路复用技术而已**，并没有什么神秘的。
其实在Linux下设计并发网络程序，向来不缺少方法，比如典型的Apache模型（Process Per Connection，简称PPC），TPC（Thread PerConnection）模型，以及select模型和poll模型，那为何还要再引入Epoll这个东东呢？那还是有得说说的…
2. 常用模型的缺点
如果不摆出来其他模型的缺点，怎么能对比出Epoll的优点呢。
2.1 PPC/TPC模型
这两种模型思想类似，就是**让每一个到来的连接一边自己做事去，别再来烦我**。只是PPC是为它开了一个进程，而TPC开了一个线程。可是别烦我是有代价的，它要时间和空间啊，连接多了之后，那么多的进程/线程切换，这开销就上来了；因此这类模型能接受的最大连接数都不会高，一般在几百个左右。
2.2 select模型
1. 最大并发数限制，因为一个进程所打开的FD（文件描述符）是有限制的，由FD_SETSIZE设置，默认值是1024/2048，因此Select模型的最大并发数就被相应限制了。自己改改这个FD_SETSIZE？想法虽好，可是先看看下面吧…
2. 效率问题，select每次调用都会线性扫描全部的FD集合，这样效率就会呈现线性下降，把FD_SETSIZE改大的后果就是，大家都慢慢来，什么？都超时了？？！！
3. 内核/用户空间 内存拷贝问题，如何让内核把FD消息通知给用户空间呢？在这个问题上select采取了内存拷贝方法。
2.3 poll模型
基本上效率和select是相同的，select缺点的2和3它都没有改掉。
3. Epoll的提升
把其他模型逐个批判了一下，再来看看Epoll的改进之处吧，其实把select的缺点反过来那就是Epoll的优点了。
3.1. Epoll没有最大并发连接的限制，上限是最大可以打开文件的数目，这个数字一般远大于2048, **一般来说这个数目和系统内存关系很大**，具体数目可以cat /proc/sys/fs/file-max察看。
3.2. 效率提升，Epoll最大的优点就在于它**只管你“活跃”的连接**，而跟连接总数无关，因此在实际的网络环境中，Epoll的效率就会远远高于select和poll。
3.3. 内存拷贝，Epoll在这点上使用了“**共享内存**”，这个内存拷贝也省略了。
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
typedefunion epoll_data {
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
intepoll_ctl(int epfd, intop, int fd, structepoll_event
 *event);
控制某个Epoll文件描述符上的事件：注册、修改、删除。其中参数epfd是epoll_create()创建Epoll专用的文件描述符。相对于select模型中的FD_SET和FD_CLR宏。
intepoll_wait(int epfd,structepoll_event * events,int maxevents,int timeout);
等待I/O事件的发生；参数说明：
epfd:由**epoll_create()** 生成的Epoll专用的文件描述符；
epoll_event:用于回传代处理事件的数组；
maxevents:每次能处理的事件数；
timeout:等待I/O事件发生的超时值；
返回发生事件数。
相对于select模型中的select函数。
7. 例子程序
下面是一个简单Echo Server的例子程序，麻雀虽小，五脏俱全，还包含了一个简单的超时检查机制，简洁起见没有做错误处理。
**[cpp]**[view plain](http://blog.csdn.net/sparkliang/article/details/4770655#)[copy](http://blog.csdn.net/sparkliang/article/details/4770655#)
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
- usingnamespace std;    
- #define MAX_EVENTS 500  
- struct myevent_s    
- {    
- int fd;    
- void (*call_back)(int fd, int events, void *arg);    
- int events;    
- void *arg;    
- int status; // 1: in epoll wait list, 0 not in  
- char buff[128]; // recv data buffer  
- int len, s_offset;    
- long last_active; // last active time  
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
- struct epoll_event epv = {0, {0}};    
- int op;    
-     epv.data.ptr = ev;    
-     epv.events = ev->events = events;    
- if(ev->status == 1){    
-         op = EPOLL_CTL_MOD;    
-     }    
- else{    
-         op = EPOLL_CTL_ADD;    
-         ev->status = 1;    
-     }    
- if(epoll_ctl(epollFd, op, ev->fd, &epv) < 0)    
-         printf("Event Add failed[fd=%d], evnets[%d]\n", ev->fd, events);    
- else
-         printf("Event Add OK[fd=%d], op=%d, evnets[%0X]\n", ev->fd, op, events);    
- }    
- // delete an event from epoll  
- void EventDel(int epollFd, myevent_s *ev)    
- {    
- struct epoll_event epv = {0, {0}};    
- if(ev->status != 1) return;    
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
- struct sockaddr_in sin;    
-     socklen_t len = sizeof(struct sockaddr_in);    
- int nfd, i;    
- // accept  
- if((nfd = accept(fd, (struct sockaddr*)&sin, &len)) == -1)    
-     {    
- if(errno != EAGAIN && errno != EINTR)    
-         {    
-         }  
-         printf("%s: accept, %d", __func__, errno);    
- return;    
-     }    
- do
-     {    
- for(i = 0; i < MAX_EVENTS; i++)    
-         {    
- if(g_Events[i].status == 0)    
-             {    
- break;    
-             }    
-         }    
- if(i == MAX_EVENTS)    
-         {    
-             printf("%s:max connection limit[%d].", __func__, MAX_EVENTS);    
- break;    
-         }    
- // set nonblocking
- int iret = 0;  
- if((iret = fcntl(nfd, F_SETFL, O_NONBLOCK)) < 0)  
-         {  
-             printf("%s: fcntl nonblocking failed:%d", __func__, iret);  
- break;  
-         }  
- // add a read event for receive data  
-         EventSet(&g_Events[i], nfd, RecvData, &g_Events[i]);    
-         EventAdd(g_epollFd, EPOLLIN, &g_Events[i]);    
-     }while(0);    
-     printf("new conn[%s:%d][time:%d], pos[%d]\n", inet_ntoa(sin.sin_addr),   
-             ntohs(sin.sin_port), g_Events[i].last_active, i);    
- }    
- // receive data  
- void RecvData(int fd, int events, void *arg)    
- {    
- struct myevent_s *ev = (struct myevent_s*)arg;    
- int len;    
- // receive data
-     len = recv(fd, ev->buff+ev->len, sizeof(ev->buff)-1-ev->len, 0);      
-     EventDel(g_epollFd, ev);  
- if(len > 0)  
-     {  
-         ev->len += len;  
-         ev->buff[len] = '\0';    
-         printf("C[%d]:%s\n", fd, ev->buff);    
- // change to send event  
-         EventSet(ev, fd, SendData, ev);    
-         EventAdd(g_epollFd, EPOLLOUT, ev);    
-     }    
- elseif(len == 0)    
-     {    
-         close(ev->fd);    
-         printf("[fd=%d] pos[%d], closed gracefully.\n", fd, ev-g_Events);    
-     }    
- else
-     {    
-         close(ev->fd);    
-         printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));    
-     }    
- }    
- // send data  
- void SendData(int fd, int events, void *arg)    
- {    
- struct myevent_s *ev = (struct myevent_s*)arg;    
- int len;    
- // send data  
-     len = send(fd, ev->buff + ev->s_offset, ev->len - ev->s_offset, 0);  
- if(len > 0)    
-     {  
-         printf("send[fd=%d], [%d<->%d]%s\n", fd, len, ev->len, ev->buff);  
-         ev->s_offset += len;  
- if(ev->s_offset == ev->len)  
-         {  
- // change to receive event
-             EventDel(g_epollFd, ev);    
-             EventSet(ev, fd, RecvData, ev);    
-             EventAdd(g_epollFd, EPOLLIN, ev);    
-         }  
-     }    
- else
-     {    
-         close(ev->fd);    
-         EventDel(g_epollFd, ev);    
-         printf("send[fd=%d] error[%d]\n", fd, errno);    
-     }    
- }    
- void InitListenSocket(int epollFd, short port)    
- {    
- int listenFd = socket(AF_INET, SOCK_STREAM, 0);    
-     fcntl(listenFd, F_SETFL, O_NONBLOCK); // set non-blocking  
-     printf("server listen fd=%d\n", listenFd);    
-     EventSet(&g_Events[MAX_EVENTS], listenFd, AcceptConn, &g_Events[MAX_EVENTS]);    
- // add listen socket  
-     EventAdd(epollFd, EPOLLIN, &g_Events[MAX_EVENTS]);    
- // bind & listen  
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
- if(argc == 2){    
-         port = atoi(argv[1]);    
-     }    
- // create epoll  
-     g_epollFd = epoll_create(MAX_EVENTS);    
- if(g_epollFd <= 0) printf("create epoll failed.%d\n", g_epollFd);    
- // create & bind listen socket, and add to epoll, set non-blocking  
-     InitListenSocket(g_epollFd, port);    
- // event loop  
- struct epoll_event events[MAX_EVENTS];    
-     printf("server running:port[%d]\n", port);    
- int checkPos = 0;    
- while(1){    
- // a simple timeout check here, every time 100, better to use a mini-heap, and add timer event  
- long now = time(NULL);    
- for(int i = 0; i < 100; i++, checkPos++) // doesn't check listen fd  
-         {    
- if(checkPos == MAX_EVENTS) checkPos = 0; // recycle  
- if(g_Events[checkPos].status != 1) continue;    
- long duration = now - g_Events[checkPos].last_active;    
- if(duration >= 60) // 60s timeout  
-             {    
-                 close(g_Events[checkPos].fd);    
-                 printf("[fd=%d] timeout[%d--%d].\n", g_Events[checkPos].fd, g_Events[checkPos].last_active, now);    
-                 EventDel(g_epollFd, &g_Events[checkPos]);    
-             }    
-         }    
- // wait for events to happen  
- int fds = epoll_wait(g_epollFd, events, MAX_EVENTS, 1000);    
- if(fds < 0){    
-             printf("epoll_wait error, exit\n");    
- break;    
-         }    
- for(int i = 0; i < fds; i++){    
-             myevent_s *ev = (struct myevent_s*)events[i].data.ptr;    
- if((events[i].events&EPOLLIN)&&(ev->events&EPOLLIN)) // read event  
-             {    
-                 ev->call_back(ev->fd, events[i].events, ev->arg);    
-             }    
- if((events[i].events&EPOLLOUT)&&(ev->events&EPOLLOUT)) // write event  
-             {    
-                 ev->call_back(ev->fd, events[i].events, ev->arg);    
-             }    
-         }    
-     }    
- // free resource  
- return 0;    
- }     
## epoll模型设计海量级连接服务器
最近在参与一个海量级（20000以上）远程连接的服务器设计，由于用户要求的硬件平台仅限于Linux，因此选择了epoll模型来实现对海量客户端连接的处理。
先聊一下epoll模型的基本情况。（一般来说，非海量级客户端连接的服务器，可能用不到epoll模型。一方面是由于epoll模型不能跨平台，另一方面初学者面露难色）。使用到epoll_create，epoll_ctl，epoll_wait，close 4个函数。
Epoll较之select优越的点，有两个方面：
1）select操作的socket对象的集合，要受到FD_SETSIZE的限制，各平台的缺省值也不一样（Windows上是64，AIX、Linux、HP、Solaris上为1024或2048不等）；而epoll操作的socket对象集合仅受系统的句柄限制，一般来说，可以达到65535
2） select和epoll的核心实现机制不同。Select实现中，kernel每次都要遍历FD_SET中的socket，检测是否可读，花费的时间和FD_SET中的socket个数成正比；epoll实现中，当有socket可读时，由核心激活epoll订阅的一个事件，因此节省了sys cpu从而节约了资源，自然就提高了效率
epoll模型提供了二种工作模式：
1） Edge Triggered (ET)
ET是高速工作方式，只支持no-block socket。当IO事件发生时内核通知事件后不再发送更多的通知，直到用户执行的操作导致那个socket（或文件描述符）事件的改变，也就是说如果用户不对socket（或文件描述符）进行IO操作，那么内核也不会再通知事件。
2） Level Triggered (LT)
LT是缺省的工作方式，同时支持block和no-block socket。内核通知事件一个文件描述符是否就绪了，然后可以对这个就绪的fd进行IO操作。如果用户不作任何读、写操作，内核还是会继续通知事件的。
更详细的epoll定义或信息，参照相关网络信息或书籍即可。下面，我来描述一下，这个需求中的epoll模型的设计。
一、业务需求如下图，要求满足高并发、高性能、高可靠等属性。
![epoll模型设计海量级连接服务器](http://s3.sinaimg.cn/bmiddle/659c1e66g9c6313090a12&690)
二、流程设计：
1） 主进程
![epoll模型设计海量级连接服务器](http://s16.sinaimg.cn/bmiddle/659c1e66g9c63152e903f&690)
1） Net_recv_thfunc工作线程
![epoll模型设计海量级连接服务器](http://s15.sinaimg.cn/bmiddle/659c1e66g9c631691216e&690)
Handle_NetRecv函数流程
![epoll模型设计海量级连接服务器](http://s7.sinaimg.cn/bmiddle/659c1e66g9c6317d7f166&690)
2） Net_send_thfunc工作线程 
![epoll模型设计海量级连接服务器](http://s16.sinaimg.cn/bmiddle/659c1e66g9c631ba85e8f&690)
Epoll模型使用中，注意以下几个点：
1）避免多个线程操作同一个epoll对象，否则会出现惊群现象，即当一个可读EPOLLIN或EPOLLOUT事件到达时，所有线程都返回这个事件，但只有一个线程真正能拿到这个事件，其他的线程将返回EWOULDBLOCK
2）操作epoll对象的线程和操作socket的线程是否分开，应视后续的处理效率而定。如果接受到socket数据的后续处理性能高，则可以在同一线程里来搞定；否则，需要创建另一类线程池，来参与socket数据的后续处理。
3）毕竟，一个进程内的句柄数也有限。如果要适应更加海量的客户端连接，可采取在同一台设备（硬件资源足够的情况下）或多台设备上，部署多套“接入管理Server”。
