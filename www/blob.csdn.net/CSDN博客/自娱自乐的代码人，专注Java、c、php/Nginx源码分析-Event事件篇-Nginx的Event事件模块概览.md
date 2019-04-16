# Nginx源码分析 - Event事件篇 - Nginx的Event事件模块概览 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年08月22日 19:30:35[initphp](https://me.csdn.net/initphp)阅读数：4441
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









前一章，我们讲解了[《Nginx源码分析 - 主流程篇 - 多进程的惊群和进程负载均衡处理 》](http://blog.csdn.net/initphp/article/details/52266844)  中实际上已经涉及到了一部分事件模块的概念。细心的可以发现，Nginx的事件都是由nginx_event.c文件中的**ngx_process_events_and_timers**进程事件分发器这个函数开始的。

这一章开始，我们会详细透彻的分析Nginx的event模块。如果你还对网络IO模型不太熟悉的，建议先把这块熟悉，也可以参考我的文章[《 转载和积累系列 - 网络IO模型 》](http://blog.csdn.net/initphp/article/details/42011845)

如果你对Linux下的epoll模型也不是很熟悉的，请先学习下epoll的原理，也可以参考我的文章[《Linux c 开发 - libevent 》](http://blog.csdn.net/initphp/article/details/41946061)

一切准备就绪，我们就可以开始了。




### event模块的概览

![](https://img-blog.csdn.net/20160822191620450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

1. ngx_event.c ：这个文件主要放置Nginx事件event模块的核心代码。

包含：进程事件分发器（ngx_process_events_and_timers）、事件模块的模块和配置、模块初始化/配置初始化等事件模块初始化的核心函数。

2. ngx_event_timer.c：定时器事件管理。主要放置定时器的代码。

3. ngx_event_posted.c：主要用于 拿到accept锁的进程 处理accept和read事件的回调函数。上一章惊群处理中，有重点提到**ngx_event_process_posted**这个方法。

4. ngx_event_pipe.c：主要用于处理管道。

5. ngx_event_openssl.c：主要用于处理SSL通道。HTTPS协议。

6. ngx_event_connect.c：主要用于处理TCP的连接通道。

7. ngx_event_accept.c：核心是**ngx_event_accept和ngx_event_recvmsg**，主要是处理accept事件的回调函数handler。而后续的read事件被ngx_event_accept中回调ngx_listen_t结构中的ls->handler回调函数回调，并且将rev->handler修改成**ngx_http_wait_request_handler**方法。

8. modules/xxxx.c：主要封装了各种平台的事件模型。我们这边主要看ngx_epoll_module.c模块。




### 重要数据结构

下面几个数据结构会在event模块中非常常见，必须得非常熟悉。

ngx_listening_s：主要是监听套接字结构，存放socket的信息

ngx_connection_s：存储连接有关的信息和读写事件。

ngx_event_s：主要存放事件的数据结构。





#### ngx_listening_s 侦听结构

主要用于存放连接的socket的侦听结构。



```cpp
/**
 * socket侦听结构
 */
struct ngx_listening_s {
    ngx_socket_t        fd;	/* 文件描述符,即socket */

    struct sockaddr    *sockaddr;	/* socket地址 */
    socklen_t           socklen;    /* size of sockaddr */
    size_t              addr_text_max_len;
    ngx_str_t           addr_text;

    int                 type;

    int                 backlog;/* 日志 */
    int                 rcvbuf;	/* 数据接收buffer */
    int                 sndbuf;	/* 数据发送的buffer */
#if (NGX_HAVE_KEEPALIVE_TUNABLE)
    int                 keepidle;
    int                 keepintvl;
    int                 keepcnt;
#endif

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler; /* 接收连接后的回调函数，回调方法：**ngx_http_init_connection** */

    void               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           log;
    ngx_log_t          *logp;

    size_t              pool_size;
    /* should be here because of the AcceptEx() preread */
    size_t              post_accept_buffer_size;
    /* should be here because of the deferred accept */
    ngx_msec_t          post_accept_timeout;

    ngx_listening_t    *previous; /* 前一个ngx_listening_t */
    ngx_connection_t   *connection; /* 连接对象 */

    ngx_uint_t          worker;

    unsigned            open:1; /* 为1表示监听句柄有效，为0表示正常关闭 */
    unsigned            remain:1;/* 为1表示不关闭原先打开的监听端口，为0表示关闭曾经打开的监听端口 */
    unsigned            ignore:1; /* 为1表示跳过设置当前ngx_listening_t结构体中的套接字，为0时正常初始化套接字 */

    unsigned            bound:1;       /* already bound */
    unsigned            inherited:1;   /* inherited from previous process */
    unsigned            nonblocking_accept:1;
    unsigned            listen:1; /* 为1表示当前结构体对应的套接字已经监听 */
    unsigned            nonblocking:1;
    unsigned            shared:1;    /* shared between threads or processes */
    unsigned            addr_ntop:1;
    unsigned            wildcard:1;

#if (NGX_HAVE_INET6 && defined IPV6_V6ONLY)
    unsigned            ipv6only:1;
#endif
#if (NGX_HAVE_REUSEPORT)
    unsigned            reuseport:1;
    unsigned            add_reuseport:1;
#endif
    unsigned            keepalive:2;

#if (NGX_HAVE_DEFERRED_ACCEPT)
    unsigned            deferred_accept:1;
    unsigned            delete_deferred:1;
    unsigned            add_deferred:1;
#ifdef SO_ACCEPTFILTER
    char               *accept_filter;
#endif
#endif
#if (NGX_HAVE_SETFIB)
    int                 setfib;
#endif

#if (NGX_HAVE_TCP_FASTOPEN)
    int                 fastopen;
#endif

};
```

#### ngx_connection_s socket连接对象结构

存储连接有关的信息和读写事件。



```cpp
struct ngx_connection_s {

	/* 关联其它的 ngx_connection_s */
    void               *data;

    /* 读取数据事件 */
    ngx_event_t        *read;

    /* 写入事件*/
    ngx_event_t        *write;

    /* socket句柄 */
    ngx_socket_t        fd;

    /* 接收数据的函数指针 */
    ngx_recv_pt         recv;

    /* 发送数据的函数指针 */
    ngx_send_pt         send;

    /* 批量接收数据的函数指针 */
    ngx_recv_chain_pt   recv_chain;

    /* 批量发送数据的函数指针 */
    ngx_send_chain_pt   send_chain;

    /* 该连接的网络监听数据结构 */
    ngx_listening_t    *listening;

    off_t               sent;

    /* 日志 */
    ngx_log_t          *log;

    /* 内存池 */
    ngx_pool_t         *pool;

    int                 type;

    /* socket的地址结构 */
    struct sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_text;

    ngx_str_t           proxy_protocol_addr;
    in_port_t           proxy_protocol_port;

#if (NGX_SSL)
    ngx_ssl_connection_t  *ssl;
#endif

    /* 本地监听socket的地址结构 */
    struct sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    /* 用于接收和缓存客户端发来的字符流 */
    ngx_buf_t          *buffer;

    /* 该字段表示将该连接以双向链表形式添加到cycle结构体中的   cycle->free_connections*/
    ngx_queue_t         queue;

    /* 建立一条与后端服务器的连接,number+1   */
    ngx_atomic_uint_t   number;

    /* 处理请求的次数  */
    ngx_uint_t          requests;

    unsigned            buffered:8;

    /* 日志级别   */
    unsigned            log_error:3;     /* ngx_connection_log_error_e */
    /* 不期待字符流结束  */
    unsigned            timedout:1;
    /* 连接处理过程中出现错误 */
    unsigned            error:1;
    /* 标识此链接已经销毁,内存池,套接字等都不可用  */
    unsigned            destroyed:1;

    /* 连接处于空闲状态  */
    unsigned            idle:1;
    /* 连接可以重用  */
    unsigned            reusable:1;
    /* 连接关闭 */
    unsigned            close:1;
    unsigned            shared:1;

    /* 正在将文件中的数据法网另一端 */
    unsigned            sendfile:1;
    unsigned            sndlowat:1;
    /* 使用tcp的nodely特性  */
    unsigned            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    /* 使用tcp的nopush特性   */
    unsigned            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    unsigned            need_last_buf:1;

#if (NGX_HAVE_IOCP)
    unsigned            accept_context_updated:1;
#endif

#if (NGX_HAVE_AIO_SENDFILE)
    unsigned            busy_count:2;
#endif

#if (NGX_THREADS)
    ngx_thread_task_t  *sendfile_task;
#endif
};
```



#### ngx_event_s 事件数据结构



```cpp
struct ngx_event_s {

	/* 事件相关对象, 通常data指向ngx_connection_t连接对象. (开启异步IO后可能指向ngx_event_aio_t结构体) */
    void            *data;

    unsigned         write:1;

    /* 标识是否可以建立连接 */
    unsigned         accept:1;

    /* used to detect the stale events in kqueue and epoll */
    unsigned         instance:1;

    /*
     * the event was passed or would be passed to a kernel;
     * in aio mode - operation was posted.
     */
    /* 表示是否是活动状态 */
    unsigned         active:1;

    unsigned         disabled:1;

    /* the ready event; in aio mode 0 means that no operation can be posted */
    unsigned         ready:1;

    unsigned         oneshot:1;

    /* aio operation is complete */
    unsigned         complete:1;

    unsigned         eof:1;
    unsigned         error:1;

    unsigned         timedout:1;
    unsigned         timer_set:1;

    unsigned         delayed:1;

    unsigned         deferred_accept:1;

    /* the pending eof reported by kqueue, epoll or in aio chain operation */
    unsigned         pending_eof:1;

    unsigned         posted:1;

    unsigned         closed:1;

    /* to test on worker exit */
    unsigned         channel:1;
    unsigned         resolver:1;

    unsigned         cancelable:1;

#if (NGX_WIN32)
    /* setsockopt(SO_UPDATE_ACCEPT_CONTEXT) was successful */
    unsigned         accept_context_updated:1;
#endif

#if (NGX_HAVE_KQUEUE)
    unsigned         kq_vnode:1;

    /* the pending errno reported by kqueue */
    int              kq_errno;
#endif

    /*
     * kqueue only:
     *   accept:     number of sockets that wait to be accepted
     *   read:       bytes to read when event is ready
     *               or lowat when event is set with NGX_LOWAT_EVENT flag
     *   write:      available space in buffer when event is ready
     *               or lowat when event is set with NGX_LOWAT_EVENT flag
     *
     * epoll with EPOLLRDHUP:
     *   accept:     1 if accept many, 0 otherwise
     *   read:       1 if there can be data to read, 0 otherwise
     *
     * iocp: TODO
     *
     * otherwise:
     *   accept:     1 if accept many, 0 otherwise
     */

#if (NGX_HAVE_KQUEUE) || (NGX_HAVE_IOCP)
    int              available;
#else
    unsigned         available:1;
#endif

    ngx_event_handler_pt  handler;


#if (NGX_HAVE_IOCP)
    ngx_event_ovlp_t ovlp;
#endif

    ngx_uint_t       index;

    ngx_log_t       *log;

    ngx_rbtree_node_t   timer;

    /* the posted queue */
    ngx_queue_t      queue;

#if 0

    /* the threads support */

    /*
     * the event thread context, we store it here
     * if $(CC) does not understand __thread declaration
     * and pthread_getspecific() is too costly
     */

    void            *thr_ctx;

#if (NGX_EVENT_T_PADDING)

    /* event should not cross cache line in SMP */

    uint32_t         padding[NGX_EVENT_T_PADDING];
#endif
#endif
};
```













