# epoll之二：epoll实战 - youbingchen的博客 - CSDN博客





2016年06月22日 14:56:29[youbingchen](https://me.csdn.net/youbingchen)阅读数：504标签：[epoll																[对象																[内核](https://so.csdn.net/so/search/s.do?q=内核&t=blog)
个人分类：[Linux网络编程																[linux																[面试经典题](https://blog.csdn.net/youbingchen/article/category/6243831)




# epoll_create系统调用

```cpp
int epoll_create(int size);
```

epoll_create返回 一个句柄，之后epoll的 使用都将依靠这个 句柄 来 标识，参数 size是告诉epoll所要处理的大致事件数目，不再使用epoll时，必须调用close 关闭这个句柄。

size这个参数只是 告诉内核这个epoll对象会处理的事件大致数目，而不是 能够处理 的事件的最大个数

# epoll_ctl系统 调用

```cpp
int epoll_ctl(int epfd,int op,int fd,struct epoll_event*event);
```

epoll_ctl向epoll对象中添加、修改或是删除感兴趣的事件，返回0表示成功，否则 返回-1，此时要根据 errno错误 码判断错误类型，epoll_wait方法返回的 事件必然通过epoll_ctl添加到epoll中的。参数epfd是epoll_create返回的句柄。 

参数op的意义表：
|op的取值|意义|
|----|----|
|EPOLL_CTL_ADD|添加新的事件到epoll中|
|EPOLL_CTL_MOD|修改epoll中的 事件|
|EPOLL_CTL_DEL|删除epoll中的事件|

第三个参数fd是待监测的连接套接字，第四个 参数是 告诉 epoll对什么样的事件感兴趣，使用了epoll_event结构体，前面介绍过了epoll实现机制中会对每一个事件创建epitem 结构体，而 在 epitem中有一个epoll_event类型的event成员

```cpp
struct epoll_event{
  _uint32_t events;
  epoll_data_t data;
};
```

```
表 epoll_event中的events的 取值 意义
```
|events的值|意义|
|----|----|
|EPOLLIN|表示对应的连接上 有数据可以读出|
|EPOLLOUT|表示对应的 连接上可以写入 数据发送|
|EPOLLRDHUP|表示TCP连接的远端 关闭或是半关闭连接|
|EPOLLPRI|表示对应的连接上 有紧急数据需要读|
|EPOLLERR|表示对应的连接上发生 错误|
|EPOLLHUP|表示对应的连接已经挂起|
|EPOLLET|表示触发方式设置为边缘触发(ET)系统默认是水平 触发（LT）|
|EPOLLONESHOT|表示对 这个事件只处理 一次，下次需要处理时要重新加入epoll|

而data成员是一个epoll_data联合，定义如下:

```cpp
typedef union  epoll_data{
    void  *ptr;
    int  fd;
    uint32_t u32;
    uint64_t  u64;

}epoll_data_t;
```

data这个成员还与具体使用方式相关，ngx_epoll_module模块只使用了 联合的ptr成员，作为指向ngx_connection_t连接的指针。

# epoll_wait系统调用

```cpp
int epoll_wait(int epfd,struct epoll_event *  events,int maxevents,int timeout);
```

收集在epoll监控的 事件中已经发生的 事件，如果 epoll中没有任何 一个事件 发生，则 最多等待timeout毫秒返回，epoll_wait的 返回值表示当前发生事件的个数，入果返回0，则表示没事件发生，如果为-1，表示出现错误，需要检查errno错误类型判断错误类型。第一参数epfd是epoll的描述符，第二个参数events则是分配好的epoll_event结构体数组，epoll将会把发生的事件复制到events数组中，第三个参数maxevents表示本次可以返回的最大次数数目，通常来说maxevents和预分配的events数组大小是相等。第四个参数timeout表示没有检测到，最多等待的时间（单位为毫秒）。

# epoll有两种工作方式 
- LT(水平 触发)
- ET(边缘 触发）

详细见博客[IO复用之select poll epoll 函数](http://blog.csdn.net/youbingchen/article/details/51613932#t5)](https://blog.csdn.net/youbingchen/article/category/6260822)](https://blog.csdn.net/youbingchen/article/category/6264544)](https://so.csdn.net/so/search/s.do?q=对象&t=blog)](https://so.csdn.net/so/search/s.do?q=epoll&t=blog)




