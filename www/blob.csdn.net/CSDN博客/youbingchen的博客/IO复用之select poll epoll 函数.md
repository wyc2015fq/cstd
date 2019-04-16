# IO复用之select poll epoll 函数 - youbingchen的博客 - CSDN博客





2016年06月08日 16:17:42[youbingchen](https://me.csdn.net/youbingchen)阅读数：510标签：[select																[epoll																[函数																[内核](https://so.csdn.net/so/search/s.do?q=内核&t=blog)
个人分类：[Linux网络编程](https://blog.csdn.net/youbingchen/article/category/6264544)





# select函数：

该函数允许进程指示内核等待多个事件中的任何一个发生，并只在有一个或多个事件发生或经历一段时间后才唤醒他

```cpp
#inlcude <sys/select.h>
#include<sys/times.h>
int select(int maxfdp1,fd_set *readset,fd_set *writeset,fd_set *exceptset,const struct timeval *timeout);
```

select 函数返回，返回值为检测到的事件个数并且返回哪些I/O发生了事件，遍历这些事件，进而处理事件。 

参数的含义： 

+ 1.读、写、异常、集合中的文件描述符的最大值+1 

+ 2.读集合 

+ 3.写集合 

+ 4.异常集合 

+ 5.超时结构体
select只能并发，无法达到并行。（这里指的是 单核)

# close 和 shutdown区别
- close 终止了数据传送的两个方向
- shutdown可以有选择终止某个方向的数据或者终止数据传送的两个方向
- shutdown how=1就可以保证对方接收到一个EOF字符，而不管其他进程是否已经打开套接字。而close不能保证，直到套接字应用计数减到0时才能发送，也就说直到所有的进程都关闭了套接字。

```
int conn;
conn = accept(sock,NULL,NUll);
pid_t pid=fork();
if(pid == -1)
     ERR_EXIT("fork");
if(pid == 0)
{
    close(sock);
    //通信
    close(conn); //这时才会向对方发送FIN段（因为这个时候conn引用计数减为0）
}
else if (pid > 0)
{
    shutdown(conn, SHUT_WR); //不理会引用计数，直接向对方发送FIN段
    close(conn); // 不会向客户端发送FIN段 仅仅只是将套接字的引用计数减1. 
}
```

### 套接字超时
- 调用alarm，它在超时时期满时产生SIGALRM信号。这个方法涉及信号处理，而信号处理在不同实现上存在差异，而且可能干扰进程中现有的alarm调用
- 在select中阻塞等待I/O 

```cpp
read_timeout();
write_timeout();
accept_timeout();
connect_timeout();
```
- 使用较新的SO_RECVTIMEO和SO_SNDTIMEO套接字选型，这个问题在于并非所有实现都支持这两个套接字选项。

### select的限制
- 一个进程能打开最大的文件描述符，这个可以通过调整内核的参数
- select中的fd_set集合容量限制（FD_SETSIZE=1024）这需要重新编译内核。

# poll函数

```cpp
#include <poll.h>
int poll(struct pollfd *fds, nfds_t nfds,int timeout);
```

第一参数是指向结构体数组，每个数组元素都是一个pollfd结构。

**select和poll函数的共同点是：内核要遍历所有文件描述符，直到找到所有发生事件的文件描述符**

# epoll 函数 （与select poll的区别）
- 相比于select与poll，epoll最大的好处在于它不会随着监听fd数目的增长而降低效率
- 内核中的select与poll的实现时采用轮询来处理的，轮询的fd数目越多，自然耗时越多
- epoll实现是基于回调的，如果fd有期望的事件发生就通过回调函数将其加入epoll就绪队列中，也就说说它只关心“活跃”的fd，与fd的数目无关
- 内核/用户空间拷贝问题，如何让内核把发fd消息通知给用户空间呢？在这个问题上select/poll采取内存拷贝的方式，而epoll采用的共享内存的方式
- epoll不仅会告诉应用程序有I/O事件到来，还会告诉应用程序相关的信息，这些信息是应用填充的，因此根据这些消息应用程序就能直接定位到事件，而不必遍历整个fd集合。

##### epoll 的2 中工作模式

EPOLLLT(电平触发) 和 EPOLLET （边缘触发）
- EPOLLLT 完全靠kernel epoll 驱动，应用程序只需处理epoll_wait 返回的fds，这些fds我们认为它们处于就绪状态
- EPOLLET 系统仅仅通知应用程序哪些fds变成了就绪状态，一旦fd变成就绪状态，epoll将不再关注这个fd任何状态信息，（从epoll）移除直到应用程序通过读写操作触发EAGAIN状态，epoll认为这个fd状态变为空闲状态，那么epoll又重新关注这个fd的状态变化（epoll 队列）
- 随着epoll_wait 的返回，队列中的fds是减少的，所以在大并发的系统中，EPOLLET更有优势，但是对程序员要求也比较高。

# TCP和UDP协议建立的流程图

1.TCP 
![这里写图片描述](https://img-blog.csdn.net/20160608161709217)

2.UDP 
![这里写图片描述](https://img-blog.csdn.net/20160608161721717)](https://so.csdn.net/so/search/s.do?q=函数&t=blog)](https://so.csdn.net/so/search/s.do?q=epoll&t=blog)](https://so.csdn.net/so/search/s.do?q=select&t=blog)




