# 单线程服务器巅峰：Redis服务器原理 - LeechanX - CSDN博客
2017年07月26日 16:17:34[LeechanXBlog](https://me.csdn.net/linkedin_38454662)阅读数：1378
导论
Redis服务器的实现是单进程、基于`IO Multiplex + Nonblock IO`，即Reactor模式，它是单进程Reactor服务器的代表作
本文根据Redis 3.0.7源码分析讨论了其Reactor模式的设计，以及基于Reactor模式的Redis TCP服务设计细节
### Redis的Reactor模式设计
Reactor模式中，事件分为
```
IO
 event
```
和
```
Timer
 event
```
，两者一起由全局Reactor句柄eventLoop管理
在Redis中这个句柄就是
```
struct
 aeEventLoop
```
**`IO event`以数组方式管存，```Timer event```以无序单链表方式管存**
```cpp
typedef struct aeEventLoop {
    int maxfd;   /* 已注册最大文件描述符 */
    int setsize; /* 最多监听多少个文件描述符 */
    long long timeEventNextId; /* 下一个Timer Event的ID */
    aeFileEvent *events; /* 注册的IO事件 */
    aeFiredEvent *fired; /* 已发生IO事件 */
    aeTimeEvent *timeEventHead; /* 注册的Timer事件组成链表，此为链表头 */
    ......
} aeEventLoop;
```
`IO event`结构如下：
```cpp
/* File event structure */
typedef struct aeFileEvent {
    int mask; /* 事件：AE_READABLE或AE_WRITABLE */
    aeFileProc *rfileProc; /* AE_READABLE事件的回调 */
    aeFileProc *wfileProc; /* AE_WRITABLE事件的回调 */
    void *clientData; /* 用户数据 */
} aeFileEvent;
```
`Timer event`结构如下：
```cpp
/* Time event structure */
typedef struct aeTimeEvent {
    long long id; /* Timer ID标识 */
    long when_sec; /* 何时触发（秒） */
    long when_ms; /* 何时触发（毫秒） */
    aeTimeProc *timeProc; /* Timer回调函数 */
    aeEventFinalizerProc *finalizerProc;
    void *clientData; /* 用户数据 */
    struct aeTimeEvent *next; /* 用于组成链表 */
} aeTimeEvent;
```
### IO事件（仅以epoll实现方式探究）
Redis将IO事件仅分为两种：
- `AE_READABLE`：对应文件可读事件`EPOLLIN`
- `AE_WRITABLE`：对应文件可写事件`EPOLLOUT`，异常事件`EPOLLERR`和`EPOLLHUP`
#### IO事件管理
`aeCreateFileEvent`函数负责向eventloop为某个fd注册IO事件 
把IO事件添加到`eventloop->events`数组中
> 
这个数组以fd作为索引，即，如果fd = 3，则eventloop->events[3] = 此IO事件
同时根据要注册的事件类型`AE_READABLE`、`AE_WRITABLE`，epoll监听fd的`EPOLLIN`、`EPOLLOUT`事件
`aeDeleteFileEvent`函数负责在eventloop删除IO事件，很简单
#### IO事件等待
- eventloop会调用epoll_wait等待所监听事件，其中epoll_wait等待时间由最近即将发生的Timer事件决定
- epoll_wait遇到事件返回后，已产生的IO事件会被放置到`event->fired`数组上
- 遍历`event->fired`数组，对每个已产生事件fd，找到fd注册的事件`eventloop->events[fd]`
- 根据实际发生的事件类型`AE_READABLE\AE_WRITABLE`，分别调用此IO事件的读回调`rfileProc`、写回调`wfileProc`
### Timer事件
Redis以无序单链表管理Timer，即不会按照Timer事件发生时间从小到大对Timer排序
注册Timer事件就是简单的添加到`eventloop->timeEventHead`之前，即插入链表头
#### Timer事件触发
Reactor在一次Event Loop中，先IO事件执行、再Timer事件执行
为了减少Timer事件的检查，IO事件的poll等待时间由Timer最近要发生的事件决定 
假设最近要发生的Timer事件在10ms后，则IO事件poll等待时间为10ms
当IO事件全部执行完成、或在给定时间没有IO事件，则检查Timer事件：
- 获取当前时间，当前秒和毫秒；
- 遍历链表每一个Timer `te`，对比当前时间与`te->when_sec`和`te->when_ms`，如果当前时间更大或相等，说明此Timer超时，调用其`te->timeProc`回调函数
- 如果`te->timeProc`返回值不是`AE_NOMORE`，说明是周期任务，则更新此Timer的触发时间；否则从链表中删除此Timer
> 
IO事件触发、Timer事件触发一起由`aeProcessEvents`函数执行
### Redis TCP服务的实现
#### 关键数据结构
Redis服务器以类型为`redisServer`结构的全局变量`server`表示，我们需要介绍的字段如下：
```cpp
struct redisServer {
    int ipfd[REDIS_BINDADDR_MAX]; /* TCP文件描述符集合，用于多IP方式 */
    int port;
    aeEventLoop *el;  /* Redis全局eventloop句柄 */
    ......
}
```
Redis客户连接 表示为`redisClient`结构
```cpp
struct redisClient {
    int fd; //TCP connection fd
    sds querybuf;//输入缓冲区
    char buf[16K];//输出缓冲区1：数组
    int bufpos;//表示输出缓冲区1当前内容长度
    list *reply;//输出缓冲区2：队列
}
```
#### TCP accepter实现
accepter做的事无非就是从内核全连接队列上及时拿出TCP连接返回
Redis在服务启动时：
- 为配置中每个IP与`server.port`创建TCP socket，并保存到`server.ipfd`数组中
- 对每个TCP socket，都依次调用`bind`、`listen`、并设置为非阻塞IO
- 将每个TCP socket都向`server.el`注册`AE_READABLE`事件，回调函数为`acceptTcpHandler`
`acceptTcpHandler`函数流程大致如下：
- 循环调用accept
- return -1时，遇到EINTR就continue，其他情况break
- return fd时，调用`acceptCommonHandler(fd)`
`acceptCommonHandler(fd)`函数流程大致如下：
- 为fd创建`redisClient`
- 设置fd非阻塞IO
- 为fd注册`AE_READABLE`事件，回调函数为`readQueryFromClient`
#### TCP connection实现
#### 数据输入
Redis使用其SDS结构作为输入缓冲区，实例为`client->querybuf`
Redis通信协议中，每一条消息（数据包）都以`\r\n`作为结尾
当可读事件到达，回调`readQueryFromClient`：
- 以16KB作为读取目标，调用`read`读取`client->fd`上的数据，保存到`client->querybuf`中
- 循环处理`client->querybuf`： 
- 如果`client->querybuf`中有`\r\n`，说明此时至少有一个完整数据包
- 按照`\r\n`为结尾读取一个数据包，交给client业务继续处理
- 将此数据包移出`client->querybuf`
- 直到`client->querybuf`中没有`\r\n`，说明当前所有完整的包已经处理完成，`client->querybuf`中为空说明暂时没有剩余数据；否则说明剩余数据是不完整包（发生了半包），先暂存着，等下次来更多数据后再处理
**总之，Redis使用`\r\n`标识符进行粘包的分包，使用`client->querybuf`暂存半包，以应对TCP粘包、分包问题**
#### 数据输出
输出缓冲区是静态数组c->buf[16K]和队列c->reply
数组应对常规写需求、队列应对拓展写需求 
一个connection一般情况下使用c->buf[16K]，当此数组无法放下更多数据后，使用c->reply队列 
c->reply仅在c->buf满后使用
具体来说： 
1、先检查c->buf与c->reply是否都为空，若是，为c->fd注册AE_WRITABLE事件，回调函数为sendReplyToClient 
2、继续检查c->reply队列，如果此队列不为空，说明c->buf已经满了，则直接添加数据到c->reply队列尾端 
3、如果c->reply队列为空，则进一步检查c->buf 
4、消息可以被放到c->buf上，则追加消息到此数组；否则启用c->reply，向此队列尾端新增消息
而写socket的任务是callback sendReplyToClient完成的：
1、如果c->buf或c->reply不为空，就不断循环 
2、循环内部：优先写c->buf数据，写完后再写c->reply队列数据 
3、写遇到-1+errno=EAGAIN就暂时break循环，其他错误则关闭连接 
4、循环结束后，检查c->buf与c->reply是否都为空，若是，为连接c->fd删除AE_WRITABLE事件
这样做可以防止busy-loop现象：有数据需要写才监听EPOLLOUT，因为EPOLLOUT事件总是准备好的，会使得epoll wait产生busy-loop，狂吃CPU
### 其他：信号
Redis服务启动时忽略了SIGPIPE信号，这个很容易理解，防止写socket遇到RST分节导致程序退出； 
还忽略了SIGHUP信号
> 
**为什么忽略SIGHUP，防止退出终端后程序退出？**
终端里./prog &，然后退出终端，终端会发送SIGHUP给后台进程prog，于是prog挂了；这也是nohup命令的原理：忽略SIGHUP。
