
# GreenSQL(数据库安全)学习 - 3-Number - CSDN博客

置顶2015年08月11日 13:07:49[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：572


### 一、软件简介
该产品是开源产品，目的也是仅仅保护开源的数据库系统MYSQL和PostgreSQL免遭SQL injection攻击。系统工作在Proxy模式。可以运行于多个Linux发行套件以及FreeBSD之上。对于PostgreSQL而言GreenSQL也是仅有的保护方案。
![](https://img-blog.csdn.net/20150811125618049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
该产品可以运行于下述模式：
1.Simulation Mode (database IDS（入侵检测系统）)
是Silent模式，类似于IDS系统，仅仅对于可疑事件进行Log。
2.Blocking Suspicious Commands (database IPS（入侵防御系统）)
类似于IPS系统，GreenSQL将利用其“启发式”引擎检测以及阻塞非法的查询。其阻塞的Action同IPS看起来很不同，不是中断连接，而是仅仅返回空的结果集，这样用户无法察觉其存在，行为也更加graceful。
3.Learning mode
在该模式下，GreenSQL学习查询信息，并自动加入到白名单（笔者注：这个过程应该是离线的，否则没有意义了）。在learn了足够多的东西后，可以将系统配置为下面的模式4.
4.Active protection from unknown queries (db firewall)
利用从模式3学到的白名单作为基础，采用“启发式”引擎，进行下一步的DB IPS的工作。
其引擎基本实现如下：
1.模式匹配
GreenSQL uses a pattern matching engine to find commands that are considered “illegal”. In essence, this is a signature-based subsystem. For example, the following commands are considered “illegal”: database administrative commands; commands that try to change a db structure; and commands used to access system files.
2.根据几个的因素，计算Risk
GreenSQL calculates each query’s risk. Essentially, this is an anomaly detection subsystem. After the risk is calculated, GreenSQL can block the query or just create a warning message (this depends on the application mode). There are a number of heuristics GreenSQL uses when calculating risk. For example, query risk is increased by:
* Access to sensitive tables (users, accounts, credit information)
* Comments inside SQL commands
* An empty password string
* An ‘or’ token inside a query
* An SQL expression that always returns true (SQL tautology)
In order to find anomalies, GreenSQL uses its own SQL language lexer to find SQL tokens.
1.Web Server位于HTTP层2.后面的App server位于App层
3.后面的DB Server位于SQL层
1的层次最低，2的层次最高，3的层次勉强可以说比1高一点点，比3第一点点。因此要想彻底根除SQL Injection要在3保证保证应用层程序员的code没有问题，才能完全保证SQL Injection的消失，但这不可能。在1防范可能不很精确，在3能可以从某个程度说是一个很好的补充，当然也无法完全解决问题，毕竟还低那么一点点。

### 2
### 安装部署
### 3
### 攻击方式

常见的攻击：SQL注入，旁注，XSS跨站，COOKIE欺骗，DDOS，0day 漏洞，社会工程学等。greensql主要是针对sql注入。下面简单介绍sql注入：
![](https://img-blog.csdn.net/20150811125936942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
所谓SQL注入式攻击，就是的输入域或页面请求的查询字符串，欺骗服务器执行恶意的SQL命令。在某些[表单](http://baike.baidu.com/view/296684.htm)中，用户输入的内容直接用来构造（或者影响）动态SQL命令，或作为[存储过程](http://baike.baidu.com/view/68525.htm)的输入参数，这类表单特别容易受到SQL注入式攻击。

### 3
### 网络模块分析
利用libevent事件驱动网络库，网络模型如下：
![](https://img-blog.csdn.net/20150811130045648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
proxy_socket职责：作为服务端，主要负责监听连接数据库socket请求事件，为其创建连接db的socket_to_db和连接客户端的socket；
Client01_socket职责：主要负责读写客户端数据操作，然后送去检查；
Client01_to_db_socket职责：主要负责将客户端查询送到db，然后将db的相应送去检查；
客户端与代理服务器的关系是多对一，这样在高并发的情况下能够应对；
### 5 socket
### 的应用
发现以前对socket编程的理解还停留在简单教学课程上的im通讯上。工作中对底层tcp/ip的学习已经有一段时间了，想借这个项目来理解socket接口和内核tcp/ip协议栈的关系。
### 5.1 socket
### 选项
两个函数：
int getsockopt(int sockfd, int level, int optname,void *optval, socklen_t *optlen);
int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
参数意义：
sockfd：标识一个套接口的描述字。
level：选项定义的层次；支持SOL_SOCKET（通用选项）、IPPROTO_TCP（TCP选项）、IPPROTO_IP（IP选项）和  IPPROTO_IPV6（IPV6选项）。
optname：需设置的选项。详情请见下图3
optval：指针，指向存放选项待设置的新值的缓冲区。
optlen：optval缓冲区长度。
![](https://img-blog.csdn.net/20150811130240796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 5.2
### 项目中
### socket
### 选项详解
### 5.2.1 SO_REUSEADDR
SO_REUSEADDR（顾名思义重用地址）：一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。
TCP的3次握手和4次挥手的状态见下图4：
![](https://img-blog.csdn.net/20150811130354486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
SO_REUSEADDR提供如下四个功能：
SO_REUSEADDR允许启动一个监听服务器并捆绑其众所周知端口，即使以前建立的将此端口用做他们的本地端口的连接仍存在。这通常是重启监听服务器时出现，若不设置此选项，则bind时将出错。
SO_REUSEADDR允许在同一端口上启动同一服务器的多个实例，只要每个实例捆绑一个不同的本地IP地址即可。对于TCP，我们根本不可能启动捆绑相同IP地址和相同端口号的多个服务器。
SO_REUSEADDR允许单个进程捆绑同一端口到多个套接口上，只要每个捆绑指定不同的本地IP地址即可。这一般不用于TCP服务器。
SO_REUSEADDR允许完全重复的捆绑：当一个IP地址和端口绑定到某个套接口上时，还允许此IP地址和端口捆绑到另一个套接口上。一般来说，这个特性仅在支持多播的系统上才有，而且只对UDP套接口而言（TCP不支持多播）。
### 5.2.2 SO_KEEPALIVE
保持连接检测对方主机是否崩溃，避免（服务器）永远阻塞于TCP连接的输入。
设置该选项后，如果2小时内在此套接口的任一方向都没有数据交换，TCP就自动给对方 发一个保持存活探测分节(keepalive probe)。这是一个对方必须响应的TCP分节.它会导致以下三种情况：
1、对方接收一切正常：以期望的ACK响应，2小时后，TCP将发出另一个探测分节。
2、对方已崩溃且已重新启动：以RST响应。套接口的待处理错误被置为ECONNRESET，套接 口本身则被关闭。
3、对方无任何响应：源自berkeley的TCP发送另外8个探测分节，相隔75秒一个，试图得到一个响应。在发出第一个探测分节11分钟15秒 后若仍无响应就放弃。套接口的待处理错误被置为ETIMEOUT，套接口本身则被关闭。如ICMP错误是“host unreachable(主机不可达)”，说明对方主机并没有崩溃，但是不可达，这种情况下待处理错误被置为EHOSTUNREACH。
### 5.2.3 SO_LINGER
此选项指定函数close对面向连接的协议如何操作（如TCP）。内核缺省close操作是立即返回，如果有数据残留在套接口缓冲区中则系统将试着将这些数据发送给对方。
SO_LINGER选项用来改变此缺省设置。使用如下结构：
struct linger {
int l_onoff; /* 0 = off, nozero = on */
int l_linger; /* linger time */
};
有下列三种情况：
1、设置 l_onoff为0，则该选项关闭，l_linger的值被忽略，等于内核缺省情况，close调用会立即返回给调用者，如果可能将会传输任何未发送的数据；
2、设置 l_onoff为非0，l_linger为0，则套接口关闭时TCP夭折连接，TCP将丢弃保留在套接口发送缓冲区中的任何数据并发送一个RST给对方，而不是通常的四分组终止序列，这避免了TIME_WAIT状态；
3、设置 l_onoff 为非0，l_linger为非0，当套接口关闭时内核将拖延一段时间（由l_linger决定）。如果套接口缓冲区中仍残留数据，进程将处于睡眠状态，直 到（a）所有数据发送完且被对方确认，之后进行正常的终止序列（描述字访问计数为0）或（b）延迟时间到。此种情况下，应用程序检查close的返回值是非常重要的，如果在数据发送完并被确认前时间到，close将返回EWOULDBLOCK错误且套接口发送缓冲区中的任何数据都丢失。close的成功返回仅告诉我们发送的数据（和FIN）已由对方TCP确认，它并不能告诉我们对方应用进程是否已读了数据。如果套接口设为非阻塞的，它将不等待close完成。
注释：l_linger的单位依赖于实现: 4.4BSD假设其单位是时钟滴答（百分之一秒），但Posix.1g规定单位为秒。
### 5.2.4 TCP_NODELAY
请参考：http://bbs.chinaunix.net/thread-3767363-1-1.html

### 6libevent
### 的应用
### 6.1
### 基本数据结构和
### api
struct event {
TAILQ_ENTRY (event) ev_next;
TAILQ_ENTRY (event) ev_active_next;
TAILQ_ENTRY (event) ev_signal_next;
unsigned int min_heap_idx; /* for managing timeouts */
struct event_base *ev_base;
int ev_fd;
short ev_events;
short ev_ncalls;
short *ev_pncalls; /* Allows deletes in callback */
struct timeval ev_timeout;
int ev_pri; /* smaller numbers are higher priority */
void (*ev_callback)(int, short, void *arg);
void *ev_arg;
int ev_res; /* result passed to event callback */
int ev_flags;
};
struct event_base *event_init(void)
功能：Initalize the event library
说明：使用libevent时必要操作
/**
Define a timer event.
@param ev event struct to be modified
@param cb callback function
@param arg argument that will be passed to the callback function
*/
\#define evtimer_set(ev, cb, arg) event_set(ev, -1, 0, cb, arg)
/**
Add a timer event.
@param ev the event struct
@param tv timeval struct
*/
\#define evtimer_add(ev, tv) event_add(ev, tv)
/**
Prepare an event structure to be added.
The function event_set() prepares the event structure ev to be used in
future calls to event_add() and event_del().  The event will be prepared to
call the function specified by the fn argument with an int argument
indicating the file descriptor, a short argument indicating the type of
event, and a void * argument given in the arg argument.  The fd indicates
the file descriptor that should be monitored for events.  The events can be
either EV_READ, EV_WRITE, or both.  Indicating that an application can read
or write from the file descriptor respectively without blocking.
The function fn will be called with the file descriptor that triggered the
event and the type of event which will be either EV_TIMEOUT, EV_SIGNAL,
EV_READ, or EV_WRITE.  The additional flag EV_PERSIST makes an event_add()
persistent until event_del() has been called.
@param ev an event struct to be modified
@param fd the file descriptor to be monitored
@param event desired events to monitor; can be EV_READ and/or EV_WRITE
@param fn callback function to be invoked when the event occurs
@param arg an argument to be passed to the callback function
@see event_add(), event_del(), event_once()
*/
void event_set(struct event *, int, short, void (*)(int, short, void *), void *);
/**
Add an event to the set of monitored events.
The function event_add() schedules the execution of the ev event when the
event specified in event_set() occurs or in at least the time specified in
the tv.  If tv is NULL, no timeout occurs and the function will only be
called if a matching event occurs on the file descriptor.  The event in the
ev argument must be already initialized by event_set() and may not be used
in calls to event_set() until it has timed out or been removed with
event_del().  If the event in the ev argument already has a scheduled
timeout, the old timeout will be replaced by the new one.
@param ev an event struct initialized via event_set()
@param timeout the maximum amount of time to wait for the event, or NULL
to wait forever
@return 0 if successful, or -1 if an error occurred
@see event_del(), event_set()
*/
int event_add(struct event *ev, const struct timeval *timeout);
/**
Handle events.
This is a more flexible version of event_dispatch().
@param flags any combination of EVLOOP_ONCE | EVLOOP_NONBLOCK
@return 0 if successful, -1 if an error occurred, or 1 if no events were
registered.
@see event_loopexit(), event_base_loop()
*/
int event_loop(int);
### 6.2
### 定时器
Greensql中定时器功能是定时（每隔1秒）查询数据库是否添加新代理、更新状态、定期回收资源。
struct event tEvent;
memset(&tEvent, 0, sizeof(struct event));
struct timeval delay;
delay.tv_sec = 1;
delay.tv_usec = 0;
evtimer_set(&tEvent, clb_timeout, &tEvent);
evtimer_add(&tEvent, &delay);
### 6.3
### 监听
### socket
### 的读写事件
每一个socket描述符都有监听事件，来处理其变化。
event_set(&serverEvent, sfd, EV_READ | EV_WRITE | EV_PERSIST,
wrap_Server, (void *)iProxyId);
event_add(&serverEvent, 0);
### 7
### 设计方法
### 7.1
### 设计模式的使用
代码中使用了比较常用的设计模式：单例模式（配置文件）、代理模式（数据库相关操作）。
如下单例模式的体现：
class GreenSQLConfig
{
public:
static GreenSQLConfig * getInstance();
.
.
private:
.
.
static GreenSQLConfig * _obj;
GreenSQLConfig();
~GreenSQLConfig();
};
如下代理模式的体现：
### 8
### 检验
### sql
### 语句模块
主要利用pcre进行规则匹配，检验sql语句的规范性。
请求：
解析查询请求报文->规则匹配查询语句是否合法->数据库操作
响应：
解析响应报文->检查响应报文是否合法->给客户端
**9........**



