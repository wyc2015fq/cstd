
# epoll的一些思考 - 高科的专栏 - CSDN博客

2013年01月06日 09:45:06[高科](https://me.csdn.net/pbymw8iwm)阅读数：6411


Epoll基本介绍
在linux的网络编程中，很长的时间都在使用select来做事件触发。在linux新的内核中，有了一种替换它的机制，就是epoll。相比于select，epoll最大的好处在于它不会随着监听fd数目的增长而降低效率。因为在内核中的select实现中，它是采用轮询来处理的，轮询的fd数目越多，自然耗时越多。并且，在linux/posix_types.h头文件有这样的声明：
\#define __FD_SETSIZE    1024
表示select最多同时监听1024个fd，当然，可以通过修改头文件再重编译内核来扩大这个数目，但这似乎并不治本。
所以先来看看二者的区别：
select的特点：select 选择句柄的时候，是遍历所有句柄，也就是说句柄有事件响应时，select需要遍历所有句柄才能获取到哪些句柄有事件通知，因此效率是非常低。但是如果连接很少的情况下， select和epoll的LT触发模式相比， 性能上差别不大。
这里要多说一句，select支持的句柄数是有限制的， 同时只支持1024个，这个是句柄集合限制的，如果超过这个限制，很可能导致溢出，而且非常不容易发现问题， TAF就出现过这个问题， 调试了n天，才发现：）当然可以通过修改linux的socket内核调整这个参数。
epoll的特点：epoll对于句柄事件的选择不是遍历的，是事件响应的，就是句柄上事件来就马上选择出来，不需要遍历整个句柄链表，因此效率非常高，内核将句柄用红黑树保存的。对于epoll而言还有ET和LT的区别，LT表示水平触发，ET表示边缘触发，两者在性能以及代码实现上差别也是非常大的。
LT：水平触发，效率会低于ET触发，尤其在大并发，大流量的情况下。但是LT对代码编写要求比较低，不容易出现问题。LT模式服务编写上的表现是：只要有数据没有被获取，内核就不断通知你，因此不用担心事件丢失的情况。
ET：边缘触发，效率非常高，在并发，大流量的情况下，会比LT少很多epoll的系统调用，因此效率高。但是对编程要求高，需要细致的处理每个请求，否则容易发生丢失事件的情况。
再详细一点：LT：水平触发  是缺省的工作方式，并且同时支持block和no-block socket.在这种做法中，内核告诉你一个文件描述符是否就绪了，然后你可以对这个就绪的fd进行IO操作。如果你不作任何操作，内核还是会继续通知你 的，所以，这种模式编程出错误可能性要小一点。传统的select/poll都是这种模型的代表。
  效率会低于ET触发，尤其在大并发，大流量的情况下。但是LT对代码编写要求比较低，不容易出现问题。LT模式服务编写上的表现是：只要有数据没有被获取，内核就不断通知你，因此不用担心事件丢失的情况。 ET：边缘触发  是高速工作方式，只支持no-block socket。在这种模式下，当描述符从未就绪变为就绪时，内核通过epoll告诉你。然后它会假设你知道文件描述符已经就绪，并且不会再为那个文件描述 符发送更多的就绪通知，直到你做了某些操作导致那个文件描述符不再为就绪状态了(比如，你在发送，接收或者接收请求，或者发送接收的数据少于一定量时导致
 了一个EWOULDBLOCK 错误）。但是请注意，如果一直不对这个fd作IO操作(从而导致它再次变成未就绪)，内核不会发送更多的通知(only once)。  效率非常高，在并发，大流量的情况下，会比LT少很多epoll的系统调用，因此效率高。但是对编程要求高，需要细致的处理每个请求，否则容易发生丢失事件的情况。
epoll的接口非常简单，一共就三个函数：
1.int epoll_create( int size ); //int close(int epfd);
创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大。这个参数不同于select()中的第一个参数，给出最大监听的fd+1的值。需要注意的是，当创建好epoll句柄后，它就是会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，必须调用close()关闭，否则可能导致fd被耗尽。
2.int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event );
epoll的事件注册函数，它不同与select()是在监听事件时告诉内核要监听什么类型的事件，而是在这里先注册要监听的事件类型。第一个参数是epoll_create()的返回值，第二个参数表示动作，用三个宏来表示：
EPOLL_CTL_ADD：注册新的fd到epfd中；
EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
EPOLL_CTL_DEL：从epfd中删除一个fd；
第三个参数是需要监听的fd，第四个参数是告诉内核需要监听什么事，struct epoll_event结构如下：
struct epoll_event
{
__uint32_t events; /* Epoll events */
epoll_data_t data; /* User data variable */
}typedef union epoll_data {
void *ptr;
int fd;
__uint32_t u32;
__uint64_t u64;
} epoll_data_t;
events可以是以下几个宏的集合：
EPOLLIN：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。只有在使用epoll
 ET(Edge Trigger)模式的时候，才需要关注数据是否读取完毕了。使用select或者epoll的LT模式，其实根本不用关注数据是否读完了，select/epoll检测到有数据可读去读就OK了
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
3. int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
等 待事件的产生，类似于select()调用。参数events用来从内核得到事件的集合，maxevents告之内核这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size，参数timeout是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。该函数返回需要处理的事件数目，如返回0表示已超时。
从man手册中，得到ET和LT的具体描述如下
EPOLL事件有两种模型：
Edge Triggered (ET)
Level Triggered (LT)
假如有这样一个例子：
1.我们已经把一个用来从管道中读取数据的文件句柄(RFD)添加到epoll描述符
2.这个时候从管道的另一端被写入了2KB的数据
3.调用epoll_wait(2)，并且它会返回RFD，说明它已经准备好读取操作
4.然后我们读取了1KB的数据
5.调用epoll_wait(2)......
Edge Triggered工作模式：
如果我们在第1步将RFD添加到epoll描述符的时候使用了EPOLLET标志，那么在第5步调用epoll_wait(2)之后将有可能会挂起，因为剩余的数据还存在于文件的输入缓冲区内，而且数据发出端还在等待一个针对已经发出数据的反馈信息。只有在监视的文件句柄上发生了某个事件的时候ET工作模式才会汇报事件。因此在第5步的时候，调用者可能会放弃等待仍在存在于文件输入缓冲区内的剩余数据。在上面的例子中，会有一个事件产生在RFD句柄上，因为在第2步执行了一个写操作，然后，事件将会在第3步被销毁。因为第4步的读取操作没有读空文件输入缓冲区内的数据，因此我们在第5步调用epoll_wait(2)完成后，是否挂起是不确定的。epoll工作在ET模式的时候，必须使用非阻塞套接口，以避免由于一个文件句柄的阻塞读/阻塞写操作把处理多个文件描述符的任务饿死。最好以下面的方式调用ET模式的epoll接口，在后面会介绍避免可能的缺陷。
i基于非阻塞文件句柄
ii只有当read(2)或者write(2)返回EAGAIN时才需要挂起，等待。但这并不是说每次read()时都需要循环读，直到读到产生一个EAGAIN才认为此次事件处理完成，当read()返回的读到的数据长度小于请求的数据长度时，就可以确定此时缓冲中已没有数据了，也就可以认为此事读事件已处理完成。
Level Triggered工作模式
相反的，以LT方式调用epoll接口的时候，它就相当于一个速度比较快的poll(2)，并且无论后面的数据是否被使用，因此他们具有同样的职能。因为即使使用ET模式的epoll，在收到多个chunk的数据的时候仍然会产生多个事件。调用者可以设定EPOLLONESHOT标志，在epoll_wait(2)收到事件后epoll会与事件关联的文件句柄从epoll描述符中禁止掉。因此当EPOLLONESHOT设定后，使用带有EPOLL_CTL_MOD标志的epoll_ctl(2)处理文件句柄就成为调用者必须作的事情。

然后详细解释ET, LT:
LT(level triggered)是缺省的工作方式，并且同时支持block和no-block socket.在这种做法中，内核告诉你一个文件描述符是否就绪了，然后你可以对这个就绪的fd进行IO操作。如果你不作任何操作，内核还是会继续通知你的，所以，这种模式编程出错误可能性要小一点。传统的select/poll都是这种模型的代表．
ET(edge-triggered)是高速工作方式，只支持no-block socket。在这种模式下，当描述符从未就绪变为就绪时，内核通过epoll告诉你。然后它会假设你知道文件描述符已经就绪，并且不会再为那个文件描述符发送更多的就绪通知，直到你做了某些操作导致那个文件描述符不再为就绪状态了(比如，你在发送，接收或者接收请求，或者发送接收的数据少于一定量时导致了一个EWOULDBLOCK错误）。但是请注意，如果一直不对这个fd作IO操作(从而导致它再次变成未就绪)，内核不会发送更多的通知(only once),不过在TCP协议中，ET模式的加速效用仍需要更多的benchmark确认（这句话不理解）。
在 许多测试中我们会看到如果没有大量的idle -connection或者dead-connection，epoll的效率并不会比select/poll高很多，但是当我们遇到大量的idle- connection(例如WAN环境中存在大量的慢速连接)，就会发现epoll的效率大大高于select/poll。（未测试）

另外，当使用epoll的ET模型来工作时，当产生了一个EPOLLIN事件后，
读数据的时候需要考虑的是当recv()返回的大小如果等于请求的大小，那么很有可能是缓冲区还有数据未读完，也意味着该次事件还没有处理完，所以还需要再次读取：
while(rs)
{
buflen = recv(activeevents[i].data.fd, buf, sizeof(buf), 0);
if(buflen < 0)
{
//由于是非阻塞的模式,所以当errno为EAGAIN时,表示当前缓冲区已无数据可读
//在这里就当作是该次事件已处理处.
if(errno == EAGAIN)
break;
else
return;
}
else if(buflen == 0)
{
//这里表示对端的socket已正常关闭.
}
if(buflen == sizeof(buf)
rs = 1;   //需要再次读取
else
rs = 0;
}

还有，假如发送端流量大于接收端的流量(意思是epoll所在的程序读比转发的socket要快),由于是非阻塞的socket,那么send()函数虽然返回,但实际缓冲区的数据并未真正发给接收端,这样不断的读和发，当缓冲区满后会产生EAGAIN错误(参考man send),同时,不理会这次请求发送的数据.所以,需要封装socket_send()的函数用来处理这种情况,该函数会尽量将数据写完再返回，返回- 1表示出错。在socket_send()内部,当写缓冲已满(send()返回-1,且errno为EAGAIN),那么会等待后再重试.这种方式并不很完美,在理论上可能会长时间的阻塞在socket_send()内部,但暂没有更好的办法.
ssize_t socket_send(int sockfd, const char* buffer, size_t buflen)
{
ssize_t tmp;
size_t total = buflen;
const char *p = buffer;
while(1)
{
tmp = send(sockfd, p, total, 0);
if(tmp < 0)
{
//当send收到信号时,可以继续写,但这里返回-1.
if(errno == EINTR)
return -1;
//当socket是非阻塞时,如返回此错误,表示写缓冲队列已满,
//在这里做延时后再重试.
if(errno == EAGAIN)
{
usleep(1000);
continue;
}
return -1;
}
if((size_t)tmp == total)
return buflen;
total -= tmp;
p += tmp;
}
return tmp;
}

epoll用到的所有函数都是在头文件sys/epoll.h中声明，有什么地方不明白或函数忘记了可以去看一下。
epoll和select相比，最大不同在于:
1epoll返回时已经明确的知道哪个sokcet fd发生了事件，不用再一个个比对。这样就提高了效率。
2select的FD_SETSIZE是有限止的，而epoll是没有限止的只与系统资源有关。
1、epoll_create函数
函数声明：int epoll_create(int size)
该函数生成一个epoll专用的文件描述符。它其实是在内核申请一空间，用来存放你想关注的socket fd上是否发生以及发生了什么事件。size就是你在这个epoll fd上能关注的最大socket fd数。随你定好了。只要你有空间。可参见上面与select之不同2.
22、epoll_ctl函数
函数声明：int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
该函数用于控制某个epoll文件描述符上的事件，可以注册事件，修改事件，删除事件。
参数：
epfd：由epoll_create生成的epoll专用的文件描述符；
op：要进行的操作例如注册事件，可能的取值EPOLL_CTL_ADD注册、EPOLL_CTL_MOD修 改、EPOLL_CTL_DEL删除
fd：关联的文件描述符；
event：指向epoll_event的指针；
如果调用成功返回0,不成功返回-1
用到的数据结构
typedef union epoll_data {
void *ptr;
int fd;
__uint32_t u32;
__uint64_t u64;
} epoll_data_t;
struct epoll_event {
__uint32_t events; /* Epoll events */
epoll_data_t data; /* User data variable */
};

如：
struct epoll_event ev;
//设置与要处理的事件相关的文件描述符
ev.data.fd=listenfd;
//设置要处理的事件类型
ev.events=EPOLLIN|EPOLLET;
//注册epoll事件
epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

常用的事件类型:
EPOLLIN：表示对应的文件描述符可以读；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET：表示对应的文件描述符有事件发生；

3、epoll_wait函数
函数声明:int epoll_wait(int epfd,struct epoll_event * events,int maxevents,int timeout)
该函数用于轮询I/O事件的发生；
参数：
epfd:由epoll_create生成的epoll专用的文件描述符；
epoll_event:用于回传代处理事件的数组；
maxevents:每次能处理的事件数；
timeout:等待I/O事件发生的超时值(单位我也不太清楚)；-1相当于阻塞，0相当于非阻塞。一般用-1即可
返回发生事件数。

用法如下：
/*build the epoll enent for recall */
struct epoll_event ev_read[20];
int nfds = 0; //return the events count
nfds=epoll_wait(epoll_fd,ev_read,20, -1);
for(i=0; i
{
if(ev_read[i].data.fd == sock)// the listener port hava data
......
epoll_wait运行的原理是
等侍注册在epfd上的socket fd的事件的发生，如果发生则将发生的sokct fd和事件类型放入到events数组中。
并且将注册在epfd上的socket fd的事件类型给清空，所以如果下一个循环你还要关注这个socket fd的话，则需要用epoll_ctl(epfd,EPOLL_CTL_MOD,listenfd,&ev)来重新设置socket fd的事件类型。这时不用EPOLL_CTL_ADD,因为socket fd并未清空，只是事件类型清空。这一步非常重要。
俺最开始就是没有加这个，白搞了一个上午。
4单个epoll并不能解决所有问题，特别是你的每个操作都比较费时的时候，因为epoll是串行处理的。
所以你有还是必要建立线程池来发挥更大的效能。
例子：
\#include <iostream>
\#include <sys/socket.h>
\#include <sys/epoll.h>
\#include <netinet/in.h>
\#include <arpa/inet.h>
\#include <fcntl.h>
\#include <unistd.h>
\#include <stdio.h>
\#define MAXLINE 10
\#define OPEN_MAX 100
\#define LISTENQ 20
\#define SERV_PORT 5555
\#define INFTIM 1000
void setnonblocking(int sock)
{
int opts;
opts=fcntl(sock,F_GETFL);
if(opts<0)
{
perror("fcntl(sock,GETFL)");
exit(1);
}
opts = opts|O_NONBLOCK;
if(fcntl(sock,F_SETFL,opts)<0)
{
perror("fcntl(sock,SETFL,opts)");
exit(1);
}
}
int main()
{
int i, maxi, listenfd, connfd, sockfd,epfd,nfds;
ssize_t n;
char line[MAXLINE];
socklen_t clilen;
//声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
struct epoll_event ev,events[20];
//生成用于处理accept的epoll专用的文件描述符
epfd=epoll_create(256);
struct sockaddr_in clientaddr;
struct sockaddr_in serveraddr;
listenfd = socket(AF_INET, SOCK_STREAM, 0);
//把socket设置为非阻塞方式
setnonblocking(listenfd);
//设置与要处理的事件相关的文件描述符
ev.data.fd=listenfd;
//设置要处理的事件类型
ev.events=EPOLLIN|EPOLLET;
//注册epoll事件
epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
bzero(&serveraddr, sizeof(serveraddr));
serveraddr.sin_family = AF_INET;
char *local_addr="200.200.200.204";
inet_aton(local_addr,&(serveraddr.sin_addr));//htons(SERV_PORT);
serveraddr.sin_port=htons(SERV_PORT);
bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
listen(listenfd, LISTENQ);
maxi = 0;
for ( ; ; ) {
//等待epoll事件的发生
nfds=epoll_wait(epfd,events,20,500);
//处理所发生的所有事件
for(i=0;i<nfds;++i)
{
if(events[i].data.fd==listenfd)
{
connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
if(connfd<0){
perror("connfd<0");
exit(1);
}
setnonblocking(connfd);
char *str = inet_ntoa(clientaddr.sin_addr);
std::cout<<"connect from "<_u115 ?tr<<std::endl;
//设置用于读操作的文件描述符
ev.data.fd=connfd;
//设置用于注测的读操作事件
ev.events=EPOLLIN|EPOLLET;
//注册ev
epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
}
else if(events[i].events&EPOLLIN)
{
if ( (sockfd = events[i].data.fd) < 0) continue;
if ( (n = read(sockfd, line, MAXLINE)) < 0) {
if (errno == ECONNRESET) {
close(sockfd);
events[i].data.fd = -1;
} else
std::cout<<"readline error"<<std::endl;
} else if (n == 0) {
close(sockfd);
events[i].data.fd = -1;
}
//设置用于写操作的文件描述符
ev.data.fd=sockfd;
//设置用于注测的写操作事件
ev.events=EPOLLOUT|EPOLLET;
//修改sockfd上要处理的事件为EPOLLOUT
epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
}
else if(events[i].events&EPOLLOUT)
{
sockfd = events[i].data.fd;
write(sockfd, line, n);
//设置用于读操作的文件描述符
ev.data.fd=sockfd;
//设置用于注测的读操作事件
ev.events=EPOLLIN|EPOLLET;
//修改sockfd上要处理的事件为EPOLIN
epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
}
}
}
}
Epoll的具体实现
在ACE中有一个TP_Reactor是基于select的..就是可以多线程轮循select
1.线程1用select去轮循fd_set ,结束后.获得可读的所有句柄.然后将这些句柄从集合中删掉.然后去操作这些可读句柄
2.线程2用select去轮循fd_set (不包含刚才已经可读的句柄).
3. .....
因为select 是每次循环都要重新加载fd_set.所以可以这样实现多线程轮循.
但epoll的话是一次性的, 如果将每次可读的fd 都调用EPOLL_CTL_DEL将其从events队列中行色匆除.我觉得可能得不到预期的高效. 因为使用epoll的环境毕竟是fd量比较大的情况
2008-3-27 12:12cofish
搜一下就能搜到了
我再贴一次吧
[code]
\#include <iostream>
\#include <sys/socket.h>
\#include <sys/epoll.h>
\#include <netinet/in.h>
\#include <arpa/inet.h>
\#include <fcntl.h>
\#include <unistd.h>
\#include <stdio.h>
\#include <pthread.h>
\#define MAXLINE 10
\#define OPEN_MAX 100
\#define LISTENQ 20
\#define SERV_PORT 5555
\#define INFTIM 1000
//线程池任务队列结构体
struct task
{
int fd; //需要读写的文件描述符
struct task *next; //下一个任务
};
//用于读写两个的两个方面传递参数
struct user_data
{
int fd;
unsigned int n_size;
char line[MAXLINE];
};
//线程的任务函数
void * readtask(void *args);
void * writetask(void *args);
//声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
struct epoll_event ev, events[20];
int epfd;
pthread_mutex_t mutex;
pthread_cond_t cond1;
struct task *readhead = NULL, *readtail = NULL, *writehead = NULL;
void setnonblocking(int sock)
{
int opts;
opts = fcntl(sock, F_GETFL);
if (opts < 0)
{
perror("fcntl(sock,GETFL)");
exit(1);
}
opts = opts | O_NONBLOCK;
if (fcntl(sock, F_SETFL, opts) < 0)
{
perror("fcntl(sock,SETFL,opts)");
exit(1);
}
}
int main()
{
int i, maxi, listenfd, connfd, sockfd, nfds;
pthread_t tid1, tid2;
struct task *new_task = NULL;
struct user_data *rdata = NULL;
socklen_t clilen;
pthread_mutex_init(&mutex, NULL);
pthread_cond_init(&cond1, NULL);
//初始化用于读线程池的线程
pthread_create(&tid1, NULL, readtask, NULL);
pthread_create(&tid2, NULL, readtask, NULL);
//生成用于处理accept的epoll专用的文件描述符
epfd = epoll_create(256);
struct sockaddr_in clientaddr;
struct sockaddr_in serveraddr;
listenfd = socket(AF_INET, SOCK_STREAM, 0);
//把socket设置为非阻塞方式
setnonblocking(listenfd);
//设置与要处理的事件相关的文件描述符
ev.data.fd = listenfd;
//设置要处理的事件类型
ev.events = EPOLLIN | EPOLLET;
//注册epoll事件
epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
bzero(&serveraddr, sizeof(serveraddr));
serveraddr.sin_family = AF_INET;
char *local_addr = "200.200.200.222";
inet_aton(local_addr, &(serveraddr.sin_addr));//htons(SERV_PORT);
serveraddr.sin_port = htons(SERV_PORT);
bind(listenfd, (sockaddr *) &serveraddr, sizeof(serveraddr));
listen(listenfd, LISTENQ);
maxi = 0;
for (;;)
{
//等待epoll事件的发生
nfds = epoll_wait(epfd, events, 20, 500);
//处理所发生的所有事件
for (i = 0; i < nfds; ++i)
{
if (events[i].data.fd == listenfd)
{
connfd = accept(listenfd, (sockaddr *) &clientaddr, &clilen);
if (connfd < 0)
{
perror("connfd<0");
exit(1);
}
setnonblocking(connfd);
char *str = inet_ntoa(clientaddr.sin_addr);
std::cout << "connec_ from >>" << str << std::endl;
//设置用于读操作的文件描述符
ev.data.fd = connfd;
//设置用于注测的读操作事件
ev.events = EPOLLIN | EPOLLET;
//注册ev
epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
}
else if (events[i].events & EPOLLIN)
{
printf("reading!\n");
if ((sockfd = events[i].data.fd) < 0)
continue;
new_task = new task();
new_task->fd = sockfd;
new_task->next = NULL;
//添加新的读任务
pthread_mutex_lock(&mutex);
if (readhead == NULL)
{
readhead = new_task;
readtail = new_task;
}
else
{
readtail->next = new_task;
readtail = new_task;
}
//唤醒所有等待cond1条件的线程
pthread_cond_broadcast(&cond1);
pthread_mutex_unlock(&mutex);
}
else if (events[i].events & EPOLLOUT)
{
rdata = (struct user_data *) events[i].data.ptr;
sockfd = rdata->fd;
write(sockfd, rdata->line, rdata->n_size);
delete rdata;
//设置用于读操作的文件描述符
ev.data.fd = sockfd;
//设置用于注测的读操作事件
ev.events = EPOLLIN | EPOLLET;
//修改sockfd上要处理的事件为EPOLIN
epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
}
}
}
}
void * readtask(void *args)
{
int fd = -1;
unsigned int n;
//用于把读出来的数据传递出去
struct user_data *data = NULL;
while (1)
{
pthread_mutex_lock(&mutex);
//等待到任务队列不为空
while (readhead == NULL)
pthread_cond_wait(&cond1, &mutex);
fd = readhead->fd;
//从任务队列取出一个读任务
struct task *tmp = readhead;
readhead = readhead->next;
delete tmp;
pthread_mutex_unlock(&mutex);
data = new user_data();
data->fd = fd;
if ((n = read(fd, data->line, MAXLINE)) < 0)
{
if (errno == ECONNRESET)
{
close(fd);
} else
std::cout << "readline error" << std::endl;
if (data != NULL)
delete data;
}
else if (n == 0)
{
close(fd);
printf("Client close connect!\n");
if (data != NULL)
delete data;
}
else
{
data->n_size = n;
//设置需要传递出去的数据
ev.data.ptr = data;
//设置用于注测的写操作事件
ev.events = EPOLLOUT | EPOLLET;
//修改sockfd上要处理的事件为EPOLLOUT
epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}
}
}
[/code]
[code]
/*-------------------------------------------------------------------------------------------------
gcc -o epolld epoll.c -lpthread
author: wyezl
2006.4.28
---------------------------------------------------------------------------------------------------*/
\#include <sys/socket.h>
\#include <sys/epoll.h>
\#include <netinet/in.h>
\#include <arpa/inet.h>
\#include <fcntl.h>
\#include <unistd.h>
\#include <stdio.h>
\#include <pthread.h>
\#include <errno.h>
\#include <string.h>
\#define PORT 8888
\#define MAXFDS 5000
\#define EVENTSIZE 100
\#define BUFFER "HTTP/1.1 200 OK\r\nContent-Length: 5\r\nConnection: close\r\nContent-Type: text/html\r\n\r\nHello"
int epfd;
void *serv_epoll(void *p);
void setnonblocking(int fd)
{
int opts;
opts = fcntl(fd, F_GETFL);
if (opts < 0)
{
fprintf(stderr, "fcntl failed\n");
return;
}
opts = opts | O_NONBLOCK;
if (fcntl(fd, F_SETFL, opts) < 0)
{
fprintf(stderr, "fcntl failed\n");
return;
}
return;
}
int main(int argc, char *argv[])
{
int fd, cfd, opt = 1;
struct epoll_event ev;
struct sockaddr_in sin, cin;
socklen_t sin_len = sizeof(struct sockaddr_in);
pthread_t tid;
pthread_attr_t attr;
epfd = epoll_create(MAXFDS);
if ((fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
{
fprintf(stderr, "socket failed\n");
return -1;
}
setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void*) &opt, sizeof(opt));
memset(&sin, 0, sizeof(struct sockaddr_in));
sin.sin_family = AF_INET;
sin.sin_port = htons((short) (PORT));
sin.sin_addr.s_addr = INADDR_ANY;
if (bind(fd, (struct sockaddr *) &sin, sizeof(sin)) != 0)
{
fprintf(stderr, "bind failed\n");
return -1;
}
if (listen(fd, 32) != 0)
{
fprintf(stderr, "listen failed\n");
return -1;
}
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
if (pthread_create(&tid, &attr, serv_epoll, NULL) != 0)
{
fprintf(stderr, "pthread_create failed\n");
return -1;
}
while (1)
{
cfd = accept(fd, (struct sockaddr *) &cin, &sin_len);
if (cfd <= 0)
{
sleep(1);
continue;
}
setnonblocking(cfd);
ev.data.fd = cfd;
ev.events = EPOLLIN | EPOLLET;
if (epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev) < 0)
{
close(cfd);
}
//printf("connect from %s\n",inet_ntoa(cin.sin_addr));
//printf("cfd=%d\n",cfd);
}
if (fd > 0)
close(fd);
return 0;
}
void *serv_epoll(void *p)
{
int i, ret, cfd, nfds;
struct epoll_event ev, events[EVENTSIZE];
char buffer[512];
while (1) {
nfds = epoll_wait(epfd, events, EVENTSIZE, -1);
//printf("nfds ........... %d\n",nfds);
for (i = 0; i < nfds; i++)
{
if (events[i].events & EPOLLIN)
{
cfd = events[i].data.fd;
ret = recv(cfd, buffer, sizeof(buffer), 0);
//printf("read ret..........= %d\n",ret);
ev.data.fd = cfd;
ev.events = EPOLLOUT | EPOLLET;
epoll_ctl(epfd, EPOLL_CTL_MOD, cfd, &ev);
}
else if (events[i].events & EPOLLOUT)
{
cfd = events[i].data.fd;
ret = send(cfd, buffer, strlen(buffer), 0);
//printf("send ret...........= %d\n", ret);
ev.data.fd = cfd;
epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, &ev);
//shutdown(cfd, 1);
if (cfd > 0)
close(cfd);
}
}
}
return NULL;
}
[/code]

Epoll的简单操作
那么究竟如何来使用epoll呢？其实非常简单。
通过在包含一个头文件\#include <sys/epoll.h>以及几个简单的API将可以大大的提高你的网络服务器的支持人数。
首先通过epoll_create(int maxfds)来创建一个epoll的句柄，其中maxfds为你epoll所支持的最大句柄数。这个函数会返回一个新的epoll句柄，之后的所有操作将通过这个句柄来进行操作。在用完之后，记得用close()来关闭这个创建出来的epoll句柄。
之后在你的网络主循环里面，每一帧的调用epoll_wait(int epfd, epoll_event events, int max events, int timeout)来查询所有的网络接口，看哪一个可以读，哪一个可以写了。基本的语法为：
nfds = epoll_wait(kdpfd, events, maxevents, -1);
其中kdpfd为用epoll_create创建之后的句柄，events是一个epoll_event*的指针，当epoll_wait这个函数操作成功之后，epoll_events里面将储存所有的读写事件。max_events是当前需要监听的所有socket句柄数。最后一个timeout是epoll_wait的超时，为0的时候表示马上返回，为-1的时候表示一直等下去，直到有事件范围，为任意正整数的时候表示等这么长的时间，如果一直没有事件，则范围。一般如果网络主循环是单独的线程的话，可以用-1来等，这样可以保证一些效率，如果是和主逻辑在同一个线程的话，则可以用0来保证主循环的效率。
epoll_wait范围之后应该是一个循环，遍利所有的事件：
for(n = 0; n < nfds; ++n) {
if(events[n].data.fd == listener) { //如果是主socket的事件的话，则表示有新连接进入了，进行新连接的处理。
client = accept(listener, (struct sockaddr *) &local,
&addrlen);
if(client < 0){
perror("accept");
continue;
}
setnonblocking(client); //将新连接置于非阻塞模式
ev.events = EPOLLIN | EPOLLET; //并且将新连接也加入EPOLL的监听队列。
注意，这里的参数EPOLLIN | EPOLLET并没有设置对写socket的监听，如果有写操作的话，这个时候epoll是不会返回事件的，如果要对写操作也监听的话，应该是EPOLLIN | EPOLLOUT | EPOLLET
ev.data.fd = client;
if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, client, &ev) < 0) {
//设置好event之后，将这个新的event通过epoll_ctl加入到epoll的监听队列里面，这里用EPOLL_CTL_ADD来加一个新的epoll事件，通过EPOLL_CTL_DEL来减少一个epoll事件，通过EPOLL_CTL_MOD来改变一个事件的监听方式。
fprintf(stderr, "epoll set insertion error: fd=%d0,
client);
return -1;
}
}
else //如果不是主socket的事件的话，则代表是一个用户socket的事件，则来处理这个用户socket的事情，比如说read(fd,xxx)之类的，或者一些其他的处理。
do_use_fd(events[n].data.fd);
}
对，epoll的操作就这么简单，总共不过4个API：epoll_create, epoll_ctl, epoll_wait和close。

