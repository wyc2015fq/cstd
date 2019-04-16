# 从源码解析Nginx对 Native aio支持 - youbingchen的博客 - CSDN博客





2016年06月27日 11:43:59[youbingchen](https://me.csdn.net/youbingchen)阅读数：6428








# 使用异步I/O大大提高应用程序的性能

> 
linux下有两种aio，一种是glibc实现的aio，这个比较烂，它是直接在用户空间用pthread进行模拟的。还有一种就是内核实现的aio，这些系统调用是以io_xxx开始的。下面将针对 同步和异步模型，以及阻塞和非阻塞的模型进行介绍。而native aio的优点就是能够同时提交多个io请求给内核，然后直接由内核的io调度算法去处理这些请求(direct io)，这样的话，内核就有可能执行一些合并，优化。native aio包含下面几个系统调用：


```cpp
io_setup(2)
io_cancle(2)
io_destroy(2)
io_getevents(2)
io_submit(2)
```

要使用他们必须安装libaio这个库，这个库也就是简单的封装了上面的几个系统调用，而nginx中没有使用libaio这个库，而是直接使用syscall来调用系统调用，这是因为Linux内核实现提供的native AIO机制，要使用这一套机制，可以利用libaio库，也可以手动利用syscall做一层自己的封装，不过这并无大碍，libaio库本身也很简单。

### 上述几个API的含义

io_setup用于建立一个aio的环境，io_cancel用于删除一个提交 句柄的任务，io_getevents用于任务执行 完毕之后的事情信息。io_submit用于提交任务 

nginx是这样处理的，利用了系统调用eventfd,用eventfd建立一个句柄，然后将这个句柄加入epoll监听，然后在io_submit提交任务的时候，将aio_flags设置为IOCB_FLAG_RESFD，并且aio_resfd设置为eventfd建立的那个句柄，这样io请求完成后，会向aio_resfd中写入完成请求数量，然后此时epoll就接到可读通知，从而进行后续操作。

### ngx_epoll_aio_init

> 
在这个函数中建立了aio环境


```cpp
// 文件 ngx_epoll_module.c
// 下面是文件异步I/o机制中定义的全局变量
// 用于通知异步I/O事件的描述符
int ngx_eventfd=-1;
//异步I/O的上下文，全局唯一，必须经过io_setup初始化才可以使用
aio_context_t ngx_aio_ctx = 0;
/*异步I/O事件完成后进行通知的描述符，也就是ngx_eventfd所对应 的ngx_event_t事件*/
static ngx_event_t ngx_eventfd_event;
/*异步I/O事件完成后进行通知描述符，也就是ngx_eventfd所对应的ngx_connection事件*/
static void
ngx_epoll_aio_init(ngx_cycle_t *cycle, ngx_epoll_conf_t *epcf)
{
    int                 n;
    struct epoll_event  ee;

#if (NGX_HAVE_SYS_EVENTFD_H)
    ngx_eventfd = eventfd(0, 0);
#else
    // 调用eventfd
    ngx_eventfd = syscall(SYS_eventfd, 0);
#endif

    if (ngx_eventfd == -1) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                      "eventfd() failed");
        ngx_file_aio = 0;
        return;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                   "eventfd: %d", ngx_eventfd);

    n = 1;
   // 设置ngx_eventfd为无阻塞
    if (ioctl(ngx_eventfd, FIONBIO, &n) == -1) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                      "ioctl(eventfd, FIONBIO) failed");
        goto failed;
    }
     //   安装 aio环境，初始化文件异步I/O上下文
    if (io_setup(epcf->aio_requests, &ngx_aio_ctx) == -1) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                      "io_setup() failed");
        goto failed;
    }
    /*创建aio上下文环境ngx_aio_ctx（全局变量），初始化ngx_eventfd_event和ngx_eventfd_conn（两者都是全局变量，利用conn和event来进行统一描述，便于将eventfd、aio融合并适用到nginx的整体逻辑里*/
   // 设置将要传递给epoll的数据，可以看到都是和eventfd关联的，设置用于异步I/O完成 通知的ngx_eventfd_event事件，它与ngx_eventfd_conn连接是对应的
    ngx_eventfd_event.data = &ngx_eventfd_conn;
    // 这个就是当eventfd可读被通知时，epoll所将要执行的 读方法 
    ngx_eventfd_event.handler = ngx_epoll_eventfd_handler;
    ngx_eventfd_event.log = cycle->log;
    ngx_eventfd_event.active = 1;
    // 初始化 ngx_eventfd_conn连接
    ngx_eventfd_conn.fd = ngx_eventfd;
    // ngx_eventfd_conn连接的读事件就是上面的ngx_eventfd_event
    ngx_eventfd_conn.read = &ngx_eventfd_event;
    ngx_eventfd_conn.log = cycle->log;
// 检测可读事件  边缘触发
    ee.events = EPOLLIN|EPOLLET;
    ee.data.ptr = &ngx_eventfd_conn;
// 加入到epoll中 ,最后将代表aio的文件描述符ngx_eventfd加入到epoll机制里，即完成eventfd与epoll的关联
    if (epoll_ctl(ep, EPOLL_CTL_ADD, ngx_eventfd, &ee) != -1) {
        return;
    }

    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                  "epoll_ctl(EPOLL_CTL_ADD, eventfd) failed");

    if (io_destroy(ngx_aio_ctx) == -1) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      "io_destroy() failed");
    }

failed:

    if (close(ngx_eventfd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      "eventfd close() failed");
    }

    ngx_eventfd = -1;
    ngx_aio_ctx = 0;
    ngx_file_aio = 0;
}

#endif
```

### ngx_file_aio_read

> 
要是设置io_submit所需要的参数，然后传递io请求给io_submit.这里有个关键的就是aio_flags的设置，这个标记说明我们要使用aio_resfd来接收aio执行的结果。其实也就是执行的io任务的个数


```cpp
// 文件名：ngx_linux_aio_read.c
ssize_t
ngx_file_aio_read(ngx_file_t *file, u_char *buf, size_t size, off_t offset,
    ngx_pool_t *pool)
{
    ngx_err_t         err;
    struct iocb      *piocb[1];
    ngx_event_t      *ev;
    ngx_event_aio_t  *aio;

    if (!ngx_file_aio) {
        return ngx_read_file(file, buf, size, offset);
    }

    if (file->aio == NULL && ngx_file_aio_init(file, pool) != NGX_OK) {
        return NGX_ERROR;
    }

    aio = file->aio;
    ev = &aio->event;

    if (!ev->ready) {
        ngx_log_error(NGX_LOG_ALERT, file->log, 0,
                      "second aio post for \"%V\"", &file->name);
        return NGX_AGAIN;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, file->log, 0,
                   "aio complete:%d @%O:%uz %V",
                   ev->complete, offset, size, &file->name);

    if (ev->complete) {
        ev->active = 0;
        ev->complete = 0;

        if (aio->res >= 0) {
            ngx_set_errno(0);
            return aio->res;
        }

        ngx_set_errno(-aio->res);

        ngx_log_error(NGX_LOG_CRIT, file->log, ngx_errno,
                      "aio read \"%s\" failed", file->name.data);

        return NGX_ERROR;
    }

    ngx_memzero(&aio->aiocb, sizeof(struct iocb));

    aio->aiocb.aio_data = (uint64_t) (uintptr_t) ev;
       // 这里设置读，nginx只使用到了异步读取，其中 一个很重要的原因就是文件的异步I/O无法利用缓存，而写操作通常是落入缓存。
    aio->aiocb.aio_lio_opcode = IOCB_CMD_PREAD;
    aio->aiocb.aio_fildes = file->fd;
    aio->aiocb.aio_buf = (uint64_t) (uintptr_t) buf;
    aio->aiocb.aio_nbytes = size;
    aio->aiocb.aio_offset = offset;
    // 设置 了aio_flags
    aio->aiocb.aio_flags = IOCB_FLAG_RESFD;
    aio->aiocb.aio_resfd = ngx_eventfd;

    ev->handler = ngx_file_aio_event_handler;

    piocb[0] = &aio->aiocb;
      // 提交请求
    if (io_submit(ngx_aio_ctx, 1, piocb) == 1) {
        ev->active = 1;
        ev->ready = 0;
        ev->complete = 0;

        return NGX_AGAIN;
    }

    err = ngx_errno;

    if (err == NGX_EAGAIN) {
        return ngx_read_file(file, buf, size, offset);
    }

    ngx_log_error(NGX_LOG_CRIT, file->log, err,
                  "io_submit(\"%V\") failed", &file->name);

    if (err == NGX_ENOSYS) {
        ngx_file_aio = 0;
        return ngx_read_file(file, buf, size, offset);
    }

    return NGX_ERROR;
}
```

### ngx_epoll_process_events

> 
当有aio请求完成时,文件描述符ngx_eventfd将变得可读，阻塞点epoll_wait()函数返回


```cpp
static ngx_int_t
ngx_epoll_process_events(ngx_cycle_t *cycle, ngx_msec_t timer, ngx_uint_t flags)
{
    int                events;
    uint32_t           revents;
    ngx_int_t          instance, i;
    ngx_uint_t         level;
    ngx_err_t          err;
    ngx_event_t       *rev, *wev;
    ngx_queue_t       *queue;
    ngx_connection_t  *c;

    /* NGX_TIMER_INFINITE == INFTIM */

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                   "epoll timer: %M", timer);
   /*调用epoll_wait() 获取事件*/
    events = epoll_wait(ep, event_list, (int) nevents, timer);

    err = (events == -1) ? ngx_errno : 0;
     /*Nginx 对 事件缓存和管理，当 flags标志位指示要 更新时间，就是在这里更新*/
    if (flags & NGX_UPDATE_TIME || ngx_event_timer_alarm) {
        // 更新时间
        ngx_time_update();
    }

    if (err) {
        if (err == NGX_EINTR) {

            if (ngx_event_timer_alarm) {
                ngx_event_timer_alarm = 0;
                return NGX_OK;
            }

            level = NGX_LOG_INFO;

        } else {
            level = NGX_LOG_ALERT;
        }

        ngx_log_error(level, cycle->log, err, "epoll_wait() failed");
        return NGX_ERROR;
    }

    if (events == 0) {
        if (timer != NGX_TIMER_INFINITE) {
            return NGX_OK;
        }

        ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                      "epoll_wait() returned no events without timeout");
        return NGX_ERROR;
    }
    // 遍历 本次epoll_wait返回所有的 事件
    for (i = 0; i < events; i++) {
        /*这个对照ngx_epoll_add_event方法，成员ptr就是ngx_connection_t连接 地址，但 最后一位有特殊含义，需要将它屏蔽掉*/
        c = event_list[i].data.ptr;
        // 将最后一位取出来，用instance变量标识
        instance = (uintptr_t) c & 1;
        /*不论是32位还是64位机器，其地址的最后1位肯定是0，使用下面 的方法把ngx_connection_t的地址还原到真正的地址值*/
        c = (ngx_connection_t *) ((uintptr_t) c & (uintptr_t) ~1);
        // 取出读事件
        rev = c->read; 
        // 判断事件 是否过期
        if (c->fd == -1 || rev->instance != instance) {
        /*套接字 描述符为-1或者instance标示位不相等时，表示这个 事件 已经 过期了，不用处理了*/
            /*
             * the stale event from a file descriptor
             * that was just closed in this iteration
             */

            ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                           "epoll: stale event %p", c);
            continue;
        }
         // 取出事件的类型
        revents = event_list[i].events;

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "epoll: fd:%d ev:%04XD d:%p",
                       c->fd, revents, event_list[i].data.ptr);

        if (revents & (EPOLLERR|EPOLLHUP)) {
            ngx_log_debug2(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                           "epoll_wait() error on fd:%d ev:%04XD",
                           c->fd, revents);
        }

#if 0
        if (revents & ~(EPOLLIN|EPOLLOUT|EPOLLERR|EPOLLHUP)) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "strange epoll_wait() events fd:%d ev:%04XD",
                          c->fd, revents);
        }
#endif

        if ((revents & (EPOLLERR|EPOLLHUP))
             && (revents & (EPOLLIN|EPOLLOUT)) == 0)
        {
            /*
             * if the error events were returned without EPOLLIN or EPOLLOUT,
             * then add these flags to handle the events at least in one
             * active handler
             */

            revents |= EPOLLIN|EPOLLOUT;
        }
         // 如果是读事件且该事件是活跃的
        if ((revents & EPOLLIN) && rev->active) {

#if (NGX_HAVE_EPOLLRDHUP)
            if (revents & EPOLLRDHUP) {
                rev->pending_eof = 1;
            }

            rev->available = 1;
#endif

            rev->ready = 1;
           // flags参数中含有NGX_POST_EVENTS表示这批事件要延后处理
            if (flags & NGX_POST_EVENTS) {
             /*如果要在post队列处理该事件，首先要判断它是 新连接事件还是普通事件，决定把它加入到ngx_posted_acept_events队列或者ngx_posted_events队列中*/
                queue = rev->accept ? &ngx_posted_accept_events
                                    : &ngx_posted_events;

                ngx_post_event(rev, queue);

            } else {
              //  立即调用读事件的回调方法处理这个事件
                rev->handler(rev);
            }
        }
        // 取出写事件
        wev = c->write;

        if ((revents & EPOLLOUT) && wev->active) {

            if (c->fd == -1 || wev->instance != instance) {
               //判断这个读事件是否过期
                /*
                 * the stale event from a file descriptor
                 * that was just closed in this iteration
                 */

                ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                               "epoll: stale event %p", c);
                continue;
            }

            wev->ready = 1;
#if (NGX_THREADS)
            wev->complete = 1;
#endif

            if (flags & NGX_POST_EVENTS) {
               // 这个事件添加到post队列中延后处理
                ngx_post_event(wev, &ngx_posted_events);

            } else {
                // 立即调用这个写事件的回调方法来处理这个事件
                wev->handler(wev);
            }
        }
    }

    return NGX_OK;
}
```

### ngx_epoll_eventfd_handler

> 
io请求完成后，nginx的回调函数如何处理的。前面的代码分析我们知道回调函数是ngx_epoll_eventfd_handler，因此我们就来看这个函数，它的流程比较简单，首先从eventfd中读取返回的事件个数，然后调用io_getevents来获得所完成的io请求事件。


```cpp
static void
ngx_epoll_eventfd_handler(ngx_event_t *ev)
{
    int               n, events;
    long              i;
    uint64_t          ready;
    ngx_err_t         err;
    ngx_event_t      *e;
    ngx_event_aio_t  *aio;
    //一次性最多处理64个事件 
    struct io_event   event[64];
    struct timespec   ts;

    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, ev->log, 0, "eventfd handler");
    // 开始读取完成的事件，并将完成的 数目 设置到ready 中，这个ready可以大于64
    n = read(ngx_eventfd, &ready, 8);

    err = ngx_errno;

    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, ev->log, 0, "eventfd: %d", n);

    if (n != 8) {
        if (n == -1) {
            if (err == NGX_EAGAIN) {
                return;
            }

            ngx_log_error(NGX_LOG_ALERT, ev->log, err, "read(eventfd) failed");
            return;
        }

        ngx_log_error(NGX_LOG_ALERT, ev->log, 0,
                      "read(eventfd) returned only %d bytes", n);
        return;
    }

    ts.tv_sec = 0;
    ts.tv_nsec = 0;
    //  ready  表示还有未处理的事件，当ready大于0时继续处理
    while (ready) {
        //用来得到所完成event，events就是事件的个数,而event则是一个事件数组
        events = io_getevents(ngx_aio_ctx, 1, 64, event, &ts);

        ngx_log_debug1(NGX_LOG_DEBUG_EVENT, ev->log, 0,
                       "io_getevents: %d", events);

        if (events > 0) {
           // 将ready减去已经取出的事件
            ready -= events;
             //遍历event，然后处理事件
            for (i = 0; i < events; i++) {

                ngx_log_debug4(NGX_LOG_DEBUG_EVENT, ev->log, 0,
                               "io_event: %XL %XL %L %L",
                                event[i].data, event[i].obj,
                                event[i].res, event[i].res2);
                // data 成员指向这个异步I/O事件对应着的实际事件
                e = (ngx_event_t *) (uintptr_t) event[i].data;

                e->complete = 1;
                e->active = 0;
                e->ready = 1;

                aio = e->data;
                aio->res = event[i].res;
            //post事件，将该事件放到ngx_posted_events队列 中等待处理
                ngx_post_event(e, &ngx_posted_events);
            }

            continue;
        }

        if (events == 0) {
            return;
        }

        /* events == -1 */
        ngx_log_error(NGX_LOG_ALERT, ev->log, ngx_errno,
                      "io_getevents() failed");
        return;
    }
}
```

# 总结

在linux下，nginx把aio结合到epoll里使用。ngx_epoll_init() -> ngx_epoll_aio_init()



