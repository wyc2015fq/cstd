# Ngnix 的代码分析 - youbingchen的博客 - CSDN博客





2016年06月10日 10:02:25[youbingchen](https://me.csdn.net/youbingchen)阅读数：2294








说明：上一节[Gdb 调Nginx](http://blog.csdn.net/youbingchen/article/details/51619659)，这一节我们深入代码本身，以下是我个人对Nginx代码的初步理解，若有出错，不吝赐教。

# Nginx两大进程
- 核心进程:包括监控进程和工作进程
- Cache进程:Cache管理进程和Cache加载进程

# Nginx核心进程模型

![Nginx 核心进程](https://img-blog.csdn.net/20160611213111753)

> 
Ngnix的进程模型将进程分成监控进程和工作进程两类，启动Nginx的主进程将充当监控进程，而由主进程fork()出来的子进程则充当工作进程。工作进程的任务自然是完成具体的业务逻辑，而监控进程充当整个进程组与用户的交互接口，同时对进程进行监护。 

  分析Nginx多线程模型入口为主进程`ngx_master_process_cycle()`函数，在该函数完成信号处理设置等之后就会调用一个名为`ngx_start_worker_process()`用于`fork()`产生出子进程，子进程作为一个新的实体开始充当工作进程的角色执行`ngx_worker_process_cycle()函数`，该函数主体作为一个无限`for(;;)`循环，持续不断处理客户端的服务请求；而主进程继续执行`ngx_master_process_cycle()`函数，也就是作为监控进程执行主体`for(;;)`循环，指导进程退出。


### 监控进程

监控进程的无限`for(;;)`内有一个关键的sigsuspend()函数调用，该函数使得控制进程大部分时间都处于挂起等待状态，直到监控进程接收到信号为止。信号处理函数一般都要要求足够简单。实际的处理逻辑必须放在主体代码里面运行。 

代码例子:：文件名为:ngx_process_cycle.c

```cpp
void 
ngx_master_process_cycle(ngx_cycle_t *cycle)
{
 for ( ;; ) {
        if (delay) {
            if (ngx_sigalrm) {
                sigio = 0;
                delay *= 2;
                ngx_sigalrm = 0; 
            }    

            ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                           "termination cycle: %M", delay);

            itv.it_interval.tv_sec = 0; 
            itv.it_interval.tv_usec = 0; 
            itv.it_value.tv_sec = delay / 1000;
            itv.it_value.tv_usec = (delay % 1000 ) * 1000;

            if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                              "setitimer() failed");
            }    
        }    

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "sigsuspend");

        sigsuspend(&set);  // 继续等待新的信号

        ngx_time_update();
        ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "wake up, sigio %i", sigio);

        if (ngx_reap) {
            ngx_reap = 0;
            ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "reap children");

            live = ngx_reap_children(cycle);
        }

        if (!live && (ngx_terminate || ngx_quit)) {
            ngx_master_process_exit(cycle);
        }

        if (ngx_terminate) {
            if (delay == 0) {
                delay = 50;
            }

            if (sigio) {
                sigio--;
                continue;
            }

            sigio = ccf->worker_processes + 2 /* cache processes */;
     ......

        }
```

### 工作进程

工作进程的执行主体与监控进程类似，不过关注点是客户端或是真实服务器之间数据可读/可写等I/O交互事件，而不是进程信号。所以工作进程的阻塞点主要是`select(),epoll_wait()`等这样的I/O多路复用函数调用处。 

代码在ngx_process_cycle.c

```cpp
static void 
ngx_worker_process_cycle(ngx_cycle_t *cycle, void *data)
{
    ngx_int_t worker = (intptr_t) data;

    ngx_process = NGX_PROCESS_WORKER;
    ngx_worker = worker;

    ngx_worker_process_init(cycle, worker);

    ngx_setproctitle("worker process");

    for ( ;; ) {

        if (ngx_exiting) {
            ngx_event_cancel_timers(); 

            if (ngx_event_timer_rbtree.root == ngx_event_timer_rbtree.sentinel)
            {    
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "exiting");

                ngx_worker_process_exit(cycle);
            }    
        }    

        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "worker cycle");

        ngx_process_events_and_timers(cycle); // 调用到对应的时间监控阻塞点，ngx_process_events_and_timers()->ngx_process_events()/ngx_epoll_process_events()->epoll_wait()

        if (ngx_terminate) {
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "exiting");
    ngx_worker_process_exit(cycle);
        }

        if (ngx_quit) {
            ngx_quit = 0;
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                          "gracefully shutting down");
            ngx_setproctitle("worker process is shutting down");

            if (!ngx_exiting) {
                ngx_exiting = 1;
                ngx_close_listening_sockets(cycle);
                ngx_close_idle_connections(cycle);
            }
        }

        if (ngx_reopen) {
            ngx_reopen = 0;
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "reopening logs");
            ngx_reopen_files(cycle, -1);
        }
    }
}
```

# Cache进程模型

![这里写图片描述](https://img-blog.csdn.net/20160611213441685)

如果Nginx开启了缓存功能，比如proxy cache，那么Nginx将还创建另外两个Cache相关进程。编写类似下列Nginx配置文件
```
http {
    include       mime.types;
    default_type  application/octet-stream;

    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';

    #access_log  logs/access.log  main;

    sendfile        on;
    #tcp_nopush     on;

    #keepalive_timeout  0;
    keepalive_timeout  65;

    #gzip  on;
    proxy_cache_path /data/nginx/cache/one levels=1:2 keys_zone=one:10m;
    server {
        listen       80;
        server_name  localhost;

        #charset koi8-r;

        #access_log  logs/host.access.log  main;

        location / {
            proxy_cache one;
            proxy_cache_valid 200 302 10m;
            root   html;
            index  index.html index.htm;
  }
 }
```

![这里写图片描述](https://img-blog.csdn.net/20160611214307195)
**Cache 进程处理客户端请求，也没有没监控I/O事件，而其处理的是超时事件在ngx_process_events_and_timers()函数内执行的事件处理函数ngx_event_expire_timers()**
### Cache 管理进程

Cache管理进程与Cache加载进程的主流进程都是`ngx_cache_manager_process_cycle()`函数，但它们附带的函数参数是不同的，管理进程执行到函数ngx_cache_manger_process_cycle()内时传递的数据是ngx_cache_manger_ctx。 

在文件:ngx_process_cycle.c

```cpp
static void 
ngx_cache_manager_process_cycle(ngx_cycle_t *cycle, void *data)
{
    ngx_cache_manager_ctx_t *ctx = data; //cache 两种进程不同功能，就是data的值不一样

    void         *ident[4];
    ngx_event_t   ev;  

    /*   
     * Set correct process type since closing listening Unix domain socket
     * in a master process also removes the Unix domain socket file.
     */
    ngx_process = NGX_PROCESS_HELPER;

    ngx_close_listening_sockets(cycle); // cache不监听，所以关闭监听套接字。

    /* Set a moderate number of connections for a helper process. */
    cycle->connection_n = 512; 

    ngx_worker_process_init(cycle, -1);

    ngx_memzero(&ev, sizeof(ngx_event_t));
    ev.handler = ctx->handler;
    ev.data = ident;
    ev.log = cycle->log;
    ident[3] = (void *) -1; //仅只是因为事件对象的data字段一般挂载的是connet对象，此处设置为-1刚好是把connect对象的fd字段设置为-1，以避免在其他代码里走到异常逻辑。

    ngx_use_accept_mutex = 0;

    ngx_setproctitle(ctx->name);

    ngx_add_timer(&ev, ctx->delay);//ctx->delay 为0，因此超时，执行对应的函数ngx_process_events_timers() -> ngx_event_expire_timers() -> ngx_cache_manager_process_handler()

    for ( ;; ) {

        if (ngx_terminate || ngx_quit) {
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "exiting");
            exit(0);
        }

        if (ngx_reopen) {
            ngx_reopen = 0;
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "reopening logs");
            ngx_reopen_files(cycle, -1);
        }

        ngx_process_events_and_timers(cycle);
    }
}
```

Cache管理进程不接收客户端请求，关闭了监听套接口，其他带代码完成创建一事件对象并设置了对应的超时事件。

```cpp
static void
ngx_cache_manager_process_handler(ngx_event_t *ev)
{
    time_t        next, n;
    ngx_uint_t    i;
    ngx_path_t  **path;

    next = 60 * 60;

    path = ngx_cycle->paths.elts;
    for (i = 0; i < ngx_cycle->paths.nelts; i++) {

        if (path[i]->manager) {
            n = path[i]->manager(path[i]->data);

            next = (n <= next) ? n : next;

            ngx_time_update();
        }
    }

    if (next == 0) {
        next = 1;
    }

    ngx_add_timer(ev, next * 1000);
}
```

函数`ngx_cache_mangager_process_handler()`的处理很简单，调用每一个磁盘缓存管理对象的`manger()`函数，然后重新设置事件对象的下一次超时立刻返回。这里的`manager`函数这是Nginx在调用函数`ngx_http_file_cache_set_slot()`解析配置文件指令`proxy_cache_path`时设置的回调值。 

加入这边设置的manager函数对应的回调值为`ngx_http_file_cache_manager`

这个函数的实现在:ngx_http_file_cache.c
```cpp
static time_t
ngx_http_file_cache_manager(void *data)
{
    ngx_http_file_cache_t  *cache = data;

    off_t       size;
    time_t      next, wait;
    ngx_uint_t  count, watermark;

    next = ngx_http_file_cache_expire(cache);

    cache->last = ngx_current_msec;
    cache->files = 0;

    for ( ;; ) {
        ngx_shmtx_lock(&cache->shpool->mutex);

        size = cache->sh->size;
        count = cache->sh->count;
        watermark = cache->sh->watermark;

        ngx_shmtx_unlock(&cache->shpool->mutex);

        ngx_log_debug3(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                       "http file cache size: %O c:%ui w:%i",
                       size, count, (ngx_int_t) watermark);

        if (size < cache->max_size && count < watermark) {
            return next;
        }

        wait = ngx_http_file_cache_forced_expire(cache);

        if (wait > 0) {
            return wait;
        }

        if (ngx_quit || ngx_terminate) {
            return next;
        }
    }
}
```

**主要实现了删除过期的缓存文件然后检查缓存文件总大小是否超限，如果超限则强行删除。总而言之，cache管理进程的任务就是清理超时缓存文件，限制缓存文件的总大小**

### Cache加载进程

在一段时间后Cache加载进程将消失，这是因为Cache加载进程的功能就是在Nginx正常启动后，将磁盘上次缓存的对象读入内存中，这个过程是一次性，所以完成这个动作之后就退出。

Cache加载进程执行到`ngx_cache_manager_process_cycle()`为止的上层函数调用与cache管理进程一致，但是在该函数内设置的事件对象回调函数为`ngx_cache_loader_process_handler()`

```cpp
static void
ngx_cache_loader_process_handler(ngx_event_t *ev)
{
    ngx_uint_t     i;
    ngx_path_t   **path;
    ngx_cycle_t   *cycle;

    cycle = (ngx_cycle_t *) ngx_cycle;

    path = cycle->paths.elts;
    for (i = 0; i < cycle->paths.nelts; i++) {

        if (ngx_terminate || ngx_quit) {
            break;
        }

        if (path[i]->loader) {
            path[i]->loader(path[i]->data);
            ngx_time_update();
        }
    }

    exit(0);
}
```

注意该函数的最后一行的exit(0),可见Cache加载进程的执行逻辑是一次性的。假设我们这边对应的`loader()`函数被设置为`ngx_http_file_cache_loader()`函数，该函数给磁盘缓存管理对象对应路径下已有的缓存文件建立对应的红黑树。






