# 四：深入Nginx之事件和连接 （之一） - youbingchen的博客 - CSDN博客





2016年06月20日 15:21:12[youbingchen](https://me.csdn.net/youbingchen)阅读数：6761








Nginx 本质上是基于事件驱动的Web服务器，事件 处理框架所要解决的问题就是如何收集、管理、分发事件。 

事件主要 以
- 网络事件(TCP网络事件为主 ）
- 定时器事件

> 
Nginx 定义一个 核心模块ngx_event_module,参考博客[一深入理解Nginx的 模块化 ，全局观](http://blog.csdn.net/youbingchen/article/details/51682228),Nginx在启动时会调用ngx_init_cycle方法 解析配置文件时，一旦 在nginx.conf中找到感兴趣的是”events {}”配置项，ngx_event_module模块开始工作了。在 核心模块ngx_event_module中的ngx_event_core_module模块定义了这个模块会使用哪种事件驱动机制以及如何管理 事件。

事件 模块是 一种新的模块类型，nginx_module_t表示Nginx模块接口，而针对每一种 不同类型的模块，都有一个结构体来描述这一类模块的通用接口，这个接口保存在ngx_module_t结构体的 ctx成员中。核心 模块的 通用接口是ngx_core_module_t，事件通用 接口 是 ngx_event_module_t结构 体：


```cpp
typedef struct {
//位于文件 ngx_event.h
   //事件模块 的名字
    ngx_str_t              *name;
    //在解析 配置前，这个回调 方法用于创建存储配置选项参数的结构体
    void                 *(*create_conf)(ngx_cycle_t *cycle);
    // 在解析配置项完成 之后,init_conf方法会被调用，用以综合处理当前事件模块感兴趣的全部配置项
    char                 *(*init_conf)(ngx_cycle_t *cycle, void *conf);
    // 对于事件驱动机制，每个事件模块需要实现10个抽象方法 
    ngx_event_actions_t     actions;
} ngx_event_module_t;
```

`ngx_event_module_t`中的`actions`成员是定义事件驱动模块的核心方法。

```cpp
typedef struct {
    /* 添加事件方法，它将负责把1个感兴趣 事件添加到操作 系统提供的事件驱动机制 */
    ngx_int_t  (*add)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    /*删除事件方法，它将1个已经存在于事件驱动机制中的事件移除 */
    ngx_int_t  (*del)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    /*启用 1个 事件，目前事件框架不会调用 这个 方法，大部分事件驱动模块对于该方法的实现都是与上面的add方法完全一致的*/
    ngx_int_t  (*enable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    /*禁用1个事件，目前事件框架不会 调用这个方法 ，大部分事件驱动器对于这个方法的实现与上面的del方法完全一致*/
    ngx_int_t  (*disable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    /*向事件驱动机制中添加一个新的连接，这意味着连接上的读写事件都添加到事件驱动机制中*/
    ngx_int_t  (*add_conn)(ngx_connection_t *c);
    /*从事件驱动机制中移除一个连接的读写事件*/
    ngx_int_t  (*del_conn)(ngx_connection_t *c, ngx_uint_t flags);
    /*仅在多线程环境下会被调用，目前Nginx在产品环境下还不会以多线程方式运行*/
    ngx_int_t  (*notify)(ngx_event_handler_pt handler);
    /*在正常的工作循环中，将调用process_events方法来 处理事件*/
    ngx_int_t  (*process_events)(ngx_cycle_t *cycle, ngx_msec_t timer,
                                 ngx_uint_t flags);
   /*初始化事件驱动模块的方法*/
    ngx_int_t  (*init)(ngx_cycle_t *cycle, ngx_msec_t timer);
    // 退出事件驱动模块前调用的方法
    void       (*done)(ngx_cycle_t *cycle);
} ngx_event_actions_t;
```

> 
ngx_event_core_module 和9个事件驱动模块都必须ngx_module_t结构体的ctx成员 实现ngx_event_module_t 接口


```cpp
struct ngx_event_s {
    /*事件相关的对象，通常data指向ngx_connection_t连接对象。开启文件异步I/O 时，它可能会指向*/
    void            *data;
     /* 标志位，标识事件可写，意味着对应的TCP连接可写，也即连接处于发送网络包状态 */ 
    unsigned         write:1;
     /* 标志位，标识可建立新的连接，一般是在ngx_listening_t对应的读事件中标记 */
    unsigned         accept:1;
     /*检测当前事件是否是过期的，它仅仅是给驱动模块使用的，而事件消费模块可以不用关心 */
    /* used to detect the stale events in kqueue and epoll */
    unsigned         instance:1;

    /*
     * the event was passed or would be passed to a kernel;
     * in aio mode - operation was posted.
     */
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


#if (NGX_HAVE_FILE_AIO)

struct ngx_event_aio_s {
    void                      *data;
    ngx_event_handler_pt       handler;
    ngx_file_t                *file;

#if (NGX_HAVE_AIO_SENDFILE)
    ssize_t                  (*preload_handler)(ngx_buf_t *file);
#endif

    ngx_fd_t                   fd;

#if (NGX_HAVE_EVENTFD)
    int64_t                    res;
#endif

#if !(NGX_HAVE_EVENTFD) || (NGX_TEST_BUILD_EPOLL)
    ngx_err_t                  err;
    size_t                     nbytes;
#endif

    ngx_aiocb_t                aiocb;
    ngx_event_t                event;
};

#endif
```

# Nginx的连接

### 　被动连接

> 
这种连接是指 客户端发起的，服务器被动接受的连接


```cpp
//在 文件ngx_connection.h中
struct ngx_connection_s {
  /*  连接未使用时，data成员用于充当连接池中空闲链表中的next指针。当连接被使用时，data的意义由使用它的Nginx模块而定。在HTTP模块中，data指向ngx_http_request_t请求*/
    void               *data;
    // 连接对应的读事件 
    ngx_event_t        *read;
    // 连接对应的写事件 
    ngx_event_t        *write;
    // 套接字句柄 
    ngx_socket_t        fd;
    //直接接收网络字符流的方法
    ngx_recv_pt         recv;
    // 直接发送网络字符流的办法
    ngx_send_pt         send;
    // 以ngx_chain_t链表为 参数来 接收 网络 字符流的方法 
    ngx_recv_chain_pt   recv_chain;
    // 以ngx_chain_t链表为 参数来 发送 网络 字符流的方法 
    ngx_send_chain_pt   send_chain;
    /*这个连接对应的ngx_listening_t监听对象，此连接由listening监听端口的事件建立*/
    ngx_listening_t    *listening;
    //这个连接上已经发送出去的字节数
    off_t               sent;
    // 可以记录日志的ngx_log_t对象
    ngx_log_t          *log;
    /* 内存池。一般在accept一个新连接时，会创建一个 内存池，而在这个 连接结束时会销毁内存池。所有的ngx_connectionn_t结构 体都是预分配，这个内存池的大小将由上面的listening 监听对象中的 pool_size成员决定*/
    ngx_pool_t         *pool;

    int                 type;
    // 连接客户端的sockaddr结构体
    struct sockaddr    *sockaddr;
    // 连接 sockaddr结构体的 长度
    socklen_t           socklen;
    // 连接客户端字符串形式的IP地址
    ngx_str_t           addr_text;

    ngx_str_t           proxy_protocol_addr;

    in_port_t           proxy_protocol_port;

#if (NGX_SSL)
    ngx_ssl_connection_t  *ssl;
#endif
      /*本机监听端口 对应 的sockaddr结构 体 ，也就是listening监听对象中的sock
    addr成员*/
    struct sockaddr    *local_sockaddr;
    socklen_t           local_socklen;
     /*用于接收、缓存客户端 发来的字节流，每个事件消费模块可自由决定从连接池中分配多大空间给 buffer这个 缓存字段*/
    ngx_buf_t          *buffer;

    ngx_queue_t         queue;
    // 连接使用次数。ngx_connection_t结构体每次建立一条来自客户端的连接，或者主动向后端服务器发起连接时，number都会加1*/
    ngx_atomic_uint_t   number;
    // 处理 请求次数
    ngx_uint_t          requests;

    unsigned            buffered:8;

    unsigned            log_error:3;     /* ngx_connection_log_error_e */

    unsigned            timedout:1;
    unsigned            error:1;
    unsigned            destroyed:1;

    unsigned            idle:1;
    unsigned            reusable:1;
    unsigned            close:1;
    unsigned            shared:1;

    unsigned            sendfile:1;
    unsigned            sndlowat:1;
    unsigned            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
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

### 主动连接

作为Web服务器，Nginx也需要向其他服务器发起连接，使用`ngx_peer_connection_t`结构体来表示 主动连接，一个待处理连接的许多特性在 被动连接ngx_connection_t中都被定义过，因此ngx_peer_connection_t结构体中引用了ngx_connection_t这个结构体。

```cpp
// 当使用长连接与上游服务器通信时，可通过该方法由连接池获取 一个新的连接
typedef ngx_int_t (*ngx_event_get_peer_pt)(ngx_peer_connection_t *pc,
    void *data);
// 当 使用长连接与上游服务器通信时，通过该方法 将使用完毕 的连接释放给连接池
typedef void (*ngx_event_free_peer_pt)(ngx_peer_connection_t *pc, void *data,
    ngx_uint_t state);
struct ngx_peer_connection_s {
    /*一个主动连接实际 上也需要ngx_connection_t结构体的大部分成员，并且处于重用的考虑 而定义 了connecion*/
    ngx_connection_t                *connection;
    // 远端服务器的socketaddr
    struct sockaddr                 *sockaddr;
    // sockaddr地址长度
    socklen_t                        socklen;
    // 远端服务器的名称 
    ngx_str_t                       *name;
    // 表示在连接 一个 远端服务器，当前连接出现 异常失败后可以重试的次数，也就是允许的最多失败的次数
    ngx_uint_t                       tries;
    ngx_msec_t                       start_time;

    ngx_event_get_peer_pt            get;
    ngx_event_free_peer_pt           free;
    void                            *data;

#if (NGX_SSL)
    ngx_event_set_peer_session_pt    set_session;
    ngx_event_save_peer_session_pt   save_session;
#endif
   // 本机地址信息 
    ngx_addr_t                      *local;

    int                              type;
    // 套接字的接收缓冲区大小
    int                              rcvbuf;
    // 记录日志的ngx_log_t对象
    ngx_log_t                       *log;

    unsigned                         cached:1;
#if (NGX_HAVE_TRANSPARENT_PROXY)
    unsigned                         transparent:1;
#endif

                                     /* ngx_connection_log_error_e */
    unsigned                         log_error:2;
};
```



