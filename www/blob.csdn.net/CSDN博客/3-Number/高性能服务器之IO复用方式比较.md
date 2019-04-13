
# 高性能服务器之IO复用方式比较 - 3-Number - CSDN博客


2017年10月16日 17:04:10[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：171


# 0x01 缘由
最近，想起3年前做的一个客户端设备，发现我对客户端使用select，现在回想起来，虽然也能够工作，但是强扭的瓜不甜，实际不用select也可以满足实际需求。当时可能是兴奋，想实践select的技术。下面主要复习和介绍select\poll\epoll的相关，并进行对比。
# 0x02 IO复用方式
## 2.1 select
```python
头文件：#include <sys/select.h>
    API:      int select( int nfds, fd_set FAR* readfds,　fd_set * writefds, fd_set * exceptfds,　const struct timeval * timeout);
    参数：     
          nfds：是一个整数值，是指集合中所有文件描述符的范围，即所有文件描述符的最大值加1，不能错！在Windows中这个参数的值无所谓，可以设置不正确。
          readfds：（可选）指针，指向一组等待可读性检查的套接口。
          writefds：（可选）指针，指向一组等待可写性检查的套接口。
          exceptfds：（可选）指针，指向一组等待错误检查的套接口。
          timeout：select()最多等待时间，对阻塞操作则为NUL
```

## 2.2 poll
```python
头文件：#include <poll.h>
  API: int poll(struct pollfd fds[], nfds_t nfds, int timeout)；
  参数:
     fds：是一个struct pollfd结构类型的数组，用于存放需要检测其状态的Socket描述符；每当调用这个函数之后，系统不会清空这个数组，操作起来比较方便；特别是对于socket连接比较多的情况下，在一定程度上可以提高处理的效率；这一点与select()函数不同，调用select()函数之后，select()函数会清空它所检测的socket描述符集合，导致每次调用select()之前都必须把socket描述符重新加入到待检测的集合中；因此，select()函数适合于只检测一个socket描述符的情况，而poll()函数适合于大量socket描述符的情况；
     nfds：nfds_t类型的参数，用于标记数组fds中的结构体元素的总数量；
     timeout：是poll函数调用阻塞的时间，单位：毫秒；
  返回值:
     >0：数组fds中准备好读、写或出错状态的那些socket描述符的总数量；
     ==0：数组fds中没有任何socket描述符准备好读、写，或出错；此时poll超时，超时时间是timeout毫秒；换句话说，如果所检测的socket描述符上没有任何事件发生的话，那么poll()函数会阻塞timeout所指定的毫秒时间长度之后返回，如果                       timeout==0，那么poll() 函数立即返回而不阻塞，如果timeout==INFTIM，那么poll() 函数会一直阻塞下去，直到所检测的socket描述符上的感兴趣的事件发生是才返回，如果感兴趣的事件永远不发生，那么poll()就会永远阻塞下去；
     -1： poll函数调用失败，同时会自动设置全局变量errno；
```

## 2.3 epoll
![](https://img-blog.csdn.net/20171016170452221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 2.4 文件就绪
![](https://img-blog.csdn.net/20171016170514231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 0x03 对比
![](https://img-blog.csdn.net/20171016170528885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 0x04 总结
不用的场景使用的技术不同，只有选择最适合的才能发挥其设计优势。再次复习这些基础知识，如何使用得选择适合场景。

