# libevent基础 - Orisun - 博客园







# [libevent基础](https://www.cnblogs.com/zhangchaoyang/articles/2683826.html)





关于I/O利用，不同的平台上都有相应的策略，比如[select、poll、epoll](http://www.cnblogs.com/zhangchaoyang/articles/2681893.html)、kqueue、devpoll、evport、win32。为libevent提供一组库函数，屏蔽了平台的差异性，底层还是调用的epoll、kqueue、devpoll等函数。libevent会使用优化的策略来选择使用哪个后端方法，以达到效率最高。

#### 创建event_base

struct event_base *event_base_new(void);

event_base中存放你是监听是否就绪的event。

在子进程中event_base要重新初始化：

int event_reinit(struct event_base *base);

```
/* ... add some events to the event_base ... */

if (fork()) {
    /* In parent */
    continue_running_parent(base); /*...*/
} else {
    /* In child */
    event_reinit(base);
    continue_running_child(base); /*...*/
}
```

#### 销毁event_base

void event_base_free(struct event_base *base);

#### 配置event_base

如果你想对event_base有更精准的控制，可以对它进行多项配置。

```
struct event_config *event_config_new(void);
struct event_base *event_base_new_with_config(const struct event_config *cfg);
void event_config_free(struct event_config *cfg);
```

我们知道libevent有很多后端方法：select、poll、epoll、kqueue、devpoll、evport、win32等等。

获取libevent支持哪些后端方法：

```
const char **event_get_supported_methods(void);
```

禁用某个后端方法：

```
int event_config_avoid_method(struct event_config *cfg, const char *method);
```

获取当前event_base中支持哪些后端方法：

```
const char *event_base_get_method(const struct event_base *base);
```

可以要求后端方法具有某些特征，不具有该特征的后端方法将不被启用。

```
enum event_method_feature {
    EV_FEATURE_ET = 0x01,
    EV_FEATURE_O1 = 0x02,
    EV_FEATURE_FDS = 0x04,
};
int event_config_require_features(struct event_config *cfg,enum event_method_feature feature);
```

EV_FEATURE_ET要求后端方法支持Edge Trigger，此时select不会被启用，而epoll是满足的。

EV_FEATURE_O1要求增加、删除一个事件，或者使一个事件变为就绪只需要O(1)的时间。

EV_FEATURE_FDS要求后端方法支持任意的文件描述符，不仅限于sockets。

```
enum event_method_feature event_base_get_features(const struct event_base *base);
```

把上面的知识综合起来，给一段示例代码：

#include<stdio.h>
#include<event2/event.h>

void main(){
	struct event_base *eb1,*eb2;
	struct event_config *ec;
	enum event_method_feature f;
	int i;
	
	const char **support=event_get_supported_methods();
	printf("libevent support backend methods:");
	for(i=0;support[i]!=NULL;++i){
		printf("%s\t",support[i]);
	}
	printf("\n");
	
	eb2=event_base_new();
	printf("current event_base support method:%s\n",event_base_get_method(eb2));
	f=event_base_get_features(eb2);
	if(f&EV_FEATURE_ET)
		printf("Edge-Triggered events are supported.\n");
	if(f&EV_FEATURE_O1)
		printf("O(1) event notification is supported.\n");
	if(f&EV_FEATURE_FDS)
		printf("All FD types are supported.\n");
	event_base_free(eb2);
	printf("\n");

	ec=event_config_new();
	event_config_require_features(ec,EV_FEATURE_ET);
	event_config_avoid_method(ec,"select");
	eb1=event_base_new_with_config(ec);
	const char* have=event_base_get_method(eb1);
	printf("current event_base support method:%s\n",have);
	f=event_base_get_features(eb1);
	if(f&EV_FEATURE_ET)
		printf("Edge-Triggered events are supported.\n");
	if(f&EV_FEATURE_O1)
		printf("O(1) event notification is supported.\n");
	if(f&EV_FEATURE_FDS)
		printf("All FD types are supported.\n");
	event_config_free(ec);
	event_base_free(eb1);
}


运行输出：

libevent support backend methods:epoll poll select 
current event_base support method:epoll
Edge-Triggered events are supported.
O(1) event notification is supported.

current event_base support method:epoll
Edge-Triggered events are supported.
O(1) event notification is supported.

可以看到在默认情况下创建event_base时，method feature就是EV_FEATURE_ET|EV_FEATURE_O1，这样在epoll、poll、 select三者当中只有epoll满足。友情提示epoll不满足EV_FEATURE_FDS，即epoll只能用于socket。

####  优先级

默认情况下libevent只支持一个优先级别，你可以设置让它支持多个级别。

```
int event_base_priority_init(struct event_base *base, int n_priorities);
```

优先级编号为[0 , n_priorities-1]。

查看当前event_base支持几个优先级别：

```
int event_base_get_npriorities(struct event_base *base);
```

#### event

当一些条件满足时，事件就会触发：
- fd已准备好读取或写入。
- fd马上就准备好读取或写入----Edge-Triggered。
- timeout
- 信号中断
- 用户触发的事件

event触发后用户要定义相应的回调函数。一个event需要和一个event_base关联起来以完成初始化。把一个event添加到event_base后它就变成了pending状态，一旦事件被触发event变成了active状态，并且此时回调函数会被执行。注意只有event从pending状态变为active状态时回调函数才会被执行。默认情况下变为active状态后是变不回pending状态的。如果event被设置成persistent，回调函数执行后它就又变成了pending状态。在没有设置persistent时，你可以先把event从evnt_base中delete，使之从active变为non-pending，然后再add到event_base中，从non-pending变为pending。

```
int event_add(struct event *ev, const struct timeval *tv);
```

对一个non-pending的event实行event_add时它就变成了pending状态，注意参数中要提供timeout。

```
int event_del(struct event *ev);
```

 对已初始化的event实行event_del使之变为non-pending、non-active状态。

可以让libevent支持POSIX风格的信号机制：

struct event* event_new(base, signum, EV_SIGNAL|EV_PERSIST, cb, arg)

当有信息signum到来时就会触发回调函数cb。

在极少数的情况下你可能需要强制触发一个事件：

```
void event_active(struct event *ev, int what, short ncalls);
```

 至此一个event发生的5种基本情况下介绍完了，下面看一个些具体的接口。

```
#define EV_TIMEOUT      0x01
#define EV_READ         0x02
#define EV_WRITE        0x04
#define EV_SIGNAL       0x08
#define EV_PERSIST      0x10
#define EV_ET           0x20

typedef void (*event_callback_fn)(evutil_socket_t, short, void *);

struct event *event_new(struct event_base *base, evutil_socket_t fd,short what, event_callback_fn cb,void *arg);

int event_add(struct event *ev, const struct timeval *tv);
int event_del(struct event *ev);

void event_free(struct event *event);
```

在event_new()函数中的参数what就是用来指定EV_READ、EV_SIGNAL等的。

下面来做个最简单的实验，还是socket的例子，服务端接收两个客户端的连接，然后监听这两个套接口上的读事件发生。创建事件时只设置EV_READ标志。

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<event2/event.h>

#define MYPORT 5000
#define BACKLOG 2		//TCP层接收链接池的缓冲队列大小
#define BUF_SIZE 200	//用于读写网络数据的内存缓冲大小

int fd_A[BACKLOG];		//存放处于连接中的socket描述符
int conn_amount;		//目前的TCP连接数量

void ev_callback(evutil_socket_t fd, short tag, void *arg){
	printf("read from file descriptor:%d:",fd);
	char buf[BUF_SIZE]={0};
	int n=read(fd,buf,sizeof(buf));
	buf[n]='\0';
	printf("%s",buf);
	printf("\n");
}

void timeout_callback(evutil_socket_t fd, short tag, void *arg){
	printf("fd %d timeout.\n",fd);
}

void main(){
	int sock_fd;
	int connfd[2]={-1};
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t sin_size;
	int yes=1;
	char buf[BUF_SIZE];
	int ret;
	int i;
	struct event_base *eb;
	struct event *ev[2];
	

	if((sock_fd=socket(PF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(1);
	}

	if(setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
		perror("socket");
		exit(1);
	}

	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(MYPORT);
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(server_addr.sin_zero,'\0',sizeof(server_addr.sin_zero));

	if(bind(sock_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
		perror("bind");
		exit(1);
	}

	if(listen(sock_fd,BACKLOG)==-1){
		perror("listen");
		exit(1);
	}

	printf("listen on port %d\n",MYPORT);

	sin_size=sizeof(client_addr);
	
	for(i=0;i<2;i++){
		connfd[i]=accept(sock_fd,(struct sockaddr*)&client_addr,&sin_size);
		printf("%d connection built.\n",connfd[i]);
	}
	
	eb=event_base_new();
	struct timeval tv={1,0};		//1秒
	for(i=0;i<2;i++){
		ev[i]=event_new(eb, connfd[i],EV_READ,ev_callback,NULL);
		event_add(ev[i],NULL);
	}
	event_base_dispatch(eb);
	printf("loop exit.\n");
	
	for(i=0;i<2;i++){
		close(connfd[i]);
		event_free(ev[i]);
	}
	event_base_free(eb);
}


客户端代码：

#!/usr/bin/perl

use IO::Socket;

my $host="127.0.0.1";
my $port=5000;

my $socket=IO::Socket::INET->new("$host:$port") or die "create socket error $@";
sleep(5);		#建立连接后先休息5秒，再发送数据
my $msg_out="1234567890";
print $socket $msg_out;
print "now send over,go to sleep...\n";
while(1){		#循环，每隔5秒发送一次数据
	sleep(5);
	print "5 seconds gone...send another line\n";
	print $socket $msg_out;
}


服务端输出：

listen on port 5000
4 connection built.
5 connection built.
read from file descriptor:4:1234567890
read from file descriptor:5:1234567890
loop exit.

client在不停地向server发送数据，这什么server只接收了一次数据event_base就自动退出了？请看下文讲的event_base_loop()。

补充一些知识：在关闭套接口时使用close()后则该端口既不能用于读取数据，也不能用于发送数据。而shutdown可以有更多选择，它那允许只关闭一个方向（读或写）的通信或关闭双向通信。上述代码中只要server端close了套接口

server端第83行创建event时使用标记EV_READ|EV_ET，并且我们故意把BUF_SIZE的大小改为5（client每次向server发送10个字节的字符串）。关于ET我们在讲epoll的时候已经说过了。看一下运行结果，在意料之中：

listen on port 5000
4 connection built.
5 connection built.
read from file descriptor:4:12345
read from file descriptor:5:12345
loop exit.

下面的试验还把BUF_SIZE改为200。

把83、84行改为：

ev[i]=event_new(eb, connfd[i],EV_READ|EV_TIMEOUT,timeout_callback,NULL);　　　　//EV_TIMEOUT标记可有可无
		event_add(ev[i],&tv);

则在客户端打印出“now send over,go to sleep...”之前（此时client还没有向server发送数据），服务端就输出：

listen on port 5000
4 connection built.
5 connection built.
fd 4 timeout.
fd 5 timeout.
loop exit.

为了方便，libevent定义了一些纯粹由timeout触发事件的函数调用。

```
#define evtimer_new(base, callback, arg) \
    event_new((base), -1, 0, (callback), (arg))
#define evtimer_add(ev, tv) \
    event_add((ev),(tv))
#define evtimer_del(ev) \
    event_del(ev)
#define evtimer_pending(ev, what, tv_out) \
    event_pending((ev), (what), (tv_out))
```

想由一个信号来触发事件可以调用：

```
#define evsignal_new(base, signum, callback, arg) \
    event_new(base, signum, EV_SIGNAL|EV_PERSIST, cb, arg)
```

比如

```
struct event *hup_event;
struct event_base *base = event_base_new();

/* call sighup_function on a HUP signal */
hup_event = evsignal_new(base, SIGHUP, sighup_function, NULL);
```

有一些方便的宏定义可以使用：

```
#define evsignal_add(ev, tv) \
    event_add((ev),(tv))
#define evsignal_del(ev) \
    event_del(ev)
#define evsignal_pending(ev, what, tv_out) \
    event_pending((ev), (what), (tv_out))
```

注意，你只能往一个event_base中安装信号才有效。

在创建event时，有时候想把event自身作为回调函数的参数传递进去，可以用：

```
void *event_self_cbarg();
```

例如你想在event触发到达一定数量后，把后来的event从event_base中删除。

```
#include <event2/event.h>

static int n_calls = 0;

void cb_func(evutil_socket_t fd, short what, void *arg)
{
    struct event *me = arg;

    printf("cb_func called %d times so far.\n", ++n_calls);

    if (n_calls > 100)
       event_del(me);
}

void run(struct event_base *base)
{
    struct timeval one_sec = { 1, 0 };
    struct event *ev;
    /* We're going to set up a repeating timer to get called called 100
       times. */
    ev = event_new(base, -1, EV_PERSIST, cb_func, event_self_cbarg());
    event_add(ev, &one_sec);
    event_base_dispatch(base);
}
```

上文已提到可以设置event_base总共有多少个优先级，设置某个event的优先级可以用：

```
int event_priority_set(struct event *event, int priority);
```

### 查看event的状态

```
int event_pending(const struct event *ev, short what, struct timeval *tv_out);
```

如果event处于pending状态，则返回true；否则返回false。

@param what the requested event type; any of EV_TIMEOUT|EV_READ|EV_WRITE|EV_SIGNAL
  @param tv_out if this field is not NULL, and the event has a timeout, this field is set to hold the time at which the timeout will expire.

```
short event_get_events(const struct event *ev);
```

获取event的flag，EV_TIMEOUT|EV_READ|EV_WRITE|EV_SIGNAL等。

```
void event_get_assignment(const struct event *event,
        struct event_base **base_out,
        evutil_socket_t *fd_out,
        short *events_out,
        event_callback_fn *callback_out,
        void **arg_out);
```

给一个已经创建好的event重新赋值。

还有一些函数其功能是“顾名思义”的：

```
#define event_get_signal(ev) /* ... */
evutil_socket_t event_get_fd(const struct event *ev);
struct event_base *event_get_base(const struct event *ev);
event_callback_fn event_get_callback(const struct event *ev);
void *event_get_callback_arg(const struct event *ev);
```

### 循环监听

```
#define EVLOOP_ONCE             0x01
#define EVLOOP_NONBLOCK         0x02
#define EVLOOP_NO_EXIT_ON_EMPTY 0x04
int event_base_loop(struct event_base *base, int flags);
int event_base_dispatch(struct event_base *base);
```

先说明一下，我在编译的时候说上面这些宏定义找不到，我grep了安装头文件也确实找不到。

 dispath相当于使用默认flag的loop，默认的flag是EVLOOP_ONCE。

EVLOOP_ONCE当event_base注册的事件都被触发过一次后，循环监听就退出了。你可以设置EVLOOP_NO_EXIT_ON_EMPTY让event_base为空后不退出。而EVLOOP_NONBLOCK不会刻意等待事件被触发，它会立即检察事件是否被触发。

### 退出循环监听

当所有向event_base中add的event全部被delete时，loop自动退出。你也可以强制让loop退出。

```
int event_base_loopexit(struct event_base *base, const struct timeval *timeout);
int event_base_loopbreak(struct event_base *base);
```

使用loopbreak()时立即退出loop。使用loopexit()时，如果timeout到期，则会检查是否有回调函数正在执行，如果有则继续把它执行完，如果没有则退出loop。所以使用loopbreak()和timeout为NULL的loopexit()还是有区别的。

你还可以事后查看loop是以哪种方式退出的：

```
int event_base_got_exit(struct event_base *base);
int event_base_got_break(struct event_base *base);
```

### 查看event_base的状态

event_base的状态就是event_base中所有event的状态。你可以把这些信息dump到一个文件里。

```
void event_base_dump_events(struct event_base *base, FILE *f);
```

### 可移植性

libevent是一整套函数调用，它不仅对I/O利用的后端方法进行了屏蔽，而且它对一些基本数据类型（如上文中已经看到的evutil_socket_t）、系统调用（如event_base_gettimeofday_cached）、socket函数进行了重新定义。下面简单列举一二。

```
int evutil_closesocket(evutil_socket_t s);
#define EVUTIL_CLOSESOCKET(s) evutil_closesocket(s)
```

关闭套接口。

```
int evutil_make_socket_nonblocking(evutil_socket_t sock);
```

设置套接口非阻塞。












