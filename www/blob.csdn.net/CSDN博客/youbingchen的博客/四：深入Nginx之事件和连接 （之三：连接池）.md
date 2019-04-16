# 四：深入Nginx之事件和连接 （之三：连接池） - youbingchen的博客 - CSDN博客





2016年06月22日 16:56:21[youbingchen](https://me.csdn.net/youbingchen)阅读数：2898








在Nginx中的connection就是对TCP连接的封装，包括连接的socket、读事件、写事件。利用Nginx封装的connection，我们 可以很方便的使用nginx处理与连接相关的事情 ，所以Nginx不仅可以作为 一个 web服务器，也可以邮件服务器，同时也可以利用nginx提供的connection，可以实现与任何后端服务打交道。

> 
流程：结合一个tcp连接的生命周期，首先nginx在启动时，会解析配置文件，得到需要监听的端口与ip地址，然后在nginx的master进程里面，先初始化这个监控的socket，然后在fork出多个进程出来，然后子进程会 竞争 accept新的连接，此时，客户端就可以向nginx发起连接，当客户端与 服务端通过三次握手建立好一个连接 ，nginx的某个进程会accept成功，得到这个 建立好的连接的socket，然后创建nginx对连接的封装，就是ngx_connection_t 结构 体。当然，nginx也是可以 作为客户端来请求其他server数据，与其他 server创建的连接 ，也封装在ngx_connection_t结构体。只是作为客户端的时候，nginx先获取ngx_connection_t结构体，然后创建socket，并设置socket的属性（比如非阻塞）然后再通过添加读写事件，调用connect/read/write来调用连接，最后关掉连接，并释放ngx_connection_t。

注意：在nginx中，每个进程会有一个连接数的最大上限，这个上限与系统对fd的限制不一样。在操作系统中，通过ulimit -n，我们可以得到一个进程所能够打开的fd的最大数，即nofile，因为每个socket连接会占用掉一个fd，所以这也会限制我们进程的最大连接数，当然也会直接影响到我们程序所能支持的最大并发数，当fd用完后，再创建socket时，就会失败。nginx通过设置worker_connectons来设置每个进程支持的最大连接数。如果该值大于nofile，那么实际的最大连接数是nofile，nginx会有警告。nginx在实现时，是通过一个连接池来管理的，每个worker进程都有一个独立的连接池，连接池的大小是worker_connections。这里的连接池里面保存的其实不是真实的连接，它只是一个worker_connections大小的一个ngx_connection_t结构的数组。并且，nginx会通过一个链表free_connections来保存所有的空闲ngx_connection_t，每次获取一个连接时，就从空闲连接链表中获取一个，用完后，再放回空闲连接链表里面。

在这里，很多人会误解worker_connections这个参数的意思，认为这个值就是nginx所能建立连接的最大值。其实不然，这个值是表示每个worker进程所能建立连接的最大值，所以，一个nginx能建立的最大连接数，应该是worker_connections * worker_processes。当然，这里说的是最大连接数，对于HTTP请求本地资源来说，能够支持的最大并发数量是worker_connections * worker_processes，而如果是HTTP作为反向代理来说，最大并发数量应该是worker_connections * worker_processes/2。因为作为反向代理服务器，每个并发会建立与客户端的连接和与后端服务的连接，会占用两个连接。


# ngx_connection_t连接池

Nginx在接受客户端的连接时，所使用的ngx_connection_t结构体都是在启动阶段就预分配好的，使用时就从连接 池中获取即可。

```cpp
//文件名ngx_cycle.h
struct ngx_cycle_s {
    void                  ****conf_ctx;
    ngx_pool_t               *pool;

    ngx_log_t                *log;
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    ngx_connection_t        **files;
    //指向下一个空闲连接，归还连接的 时候，只需要把该连接插入到free_connections链表表头即可
    ngx_connection_t         *free_connections;
    ngx_uint_t                free_connection_n;

    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_used;    /* unsigned  modules_used:1; */

    ngx_queue_t               reusable_connections_queue;

    ngx_array_t               listening;
    ngx_array_t               paths;
    ngx_array_t               config_dump;
    ngx_list_t                open_files;
    ngx_list_t                shared_memory;

    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;
    // 指向整个连接池数组的首部
    ngx_connection_t         *connections;
    ngx_event_t              *read_events;
    ngx_event_t              *write_events;

    ngx_cycle_t              *old_cycle;

    ngx_str_t                 conf_file;
    ngx_str_t                 conf_param;
    ngx_str_t                 conf_prefix;
    ngx_str_t                 prefix;
    ngx_str_t                 lock_file;
    ngx_str_t                 hostname;
};
```

![nginx_connection ](https://img-blog.csdn.net/20160622165537546)

> 
Nginx 每一个连接 一定需要一个读 事件和一个 写事件，有多少 连接 就 分配多少 个读、写事件，读事件、写 事件 、连接池是 由3个 大小 相同的 数组组成所以根据数组序号就可 将 每一个连接、读事件、写事件对应起来，这个关系在`ngx_event_core_module`模块初始化过程就已经决定了。 

  如果开发自己 的 模块 直接使用了 连接 池 ，那么可以用着两个 方法来获取、释放ngx_connection_t结构体


# 连接池使用的方法
|连接池操作方法 名|参数含义|执行意义|
|----|----|----|
|ngx_connection_t *ngx_get_connection(ngx_socket_t s,ngx_log_t *log)|s是这条连接的套接字句柄，log则是记录日志|从连接池中获取一个ngx_connection_t结构体，同时获取相应的读事件/写事件|
|void ngx_free_connection (ngx_connection_t *c)|c是需要回收的 连接|将这个连接回收到连接池中|



