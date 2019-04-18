# Nginx学习之七-模块ngx_epoll_module详解（epoll机制在nginx中的实现） - 江南烟雨 - CSDN博客
2013年07月05日 11:30:50[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：14544标签：[Linux																[Nginx																[epoll																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=epoll&t=blog)](https://so.csdn.net/so/search/s.do?q=Nginx&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)
个人分类：[Nginx](https://blog.csdn.net/xiajun07061225/article/category/1461389)
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)
本文结合实际代码介绍事件驱动模块接口与epoll用法是如何结合起来发挥作用的。Nginx中ngx_epoll_module模块基于epoll实现了Nginx的事件驱动。
模块定义（src/event/ngx_epoll_module.c）：
```cpp
ngx_module_t  ngx_epoll_module = {
    NGX_MODULE_V1,
    &ngx_epoll_module_ctx,               /* module context */
    ngx_epoll_commands,                  /* module directives */
    NGX_EVENT_MODULE,                    /* module type */
    NULL,                                /* init master */
    NULL,                                /* init module */
    NULL,                                /* init process */
    NULL,                                /* init thread */
    NULL,                                /* exit thread */
    NULL,                                /* exit process */
    NULL,                                /* exit master */
    NGX_MODULE_V1_PADDING
};
```
首先看ngx_epoll_module模块对哪些配置项感兴趣，ngx_epoll_commands数组定义如下：
```cpp
static ngx_command_t  ngx_epoll_commands[] = {
    //这个配置项表示调用一次epoll_wait最多可以返回的事件数
    { ngx_string("epoll_events"),
      NGX_EVENT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,//采用预设的配置项解析方法
      0,
      offsetof(ngx_epoll_conf_t, events),
      NULL },
    //指明在开启异步I/O且使用io_setup系统调用初始化异步I/O上下文环境时，初始分配的异步I/O事件个数
    { ngx_string("worker_aio_requests"),
      NGX_EVENT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      0,
      offsetof(ngx_epoll_conf_t, aio_requests),
      NULL },
      ngx_null_command
};
```
存储配置项的结构体ngx_epoll_conf_t：
```cpp
ngx_event_module_t  ngx_epoll_module_ctx = {
    &epoll_name,
    ngx_epoll_create_conf,               /* create configuration */
    ngx_epoll_init_conf,                 /* init configuration */
    {
        ngx_epoll_add_event,             /* add an event */
        ngx_epoll_del_event,             /* delete an event */
        ngx_epoll_add_event,             /* enable an event */
        ngx_epoll_del_event,             /* disable an event */
        ngx_epoll_add_connection,        /* add an connection */
        ngx_epoll_del_connection,        /* delete an connection */
        NULL,                            /* process the changes */
        ngx_epoll_process_events,        /* process the events */
        ngx_epoll_init,                  /* init the events */
        ngx_epoll_done,                  /* done the events */
    }
};
```
类型ngx_event_module_t（src/event/ngx_event.h）：
```cpp
typedef struct {
    ngx_str_t              *name;//事件模块名称
    void                 *(*create_conf)(ngx_cycle_t *cycle);//解析配置项前，这个回调方法用户创建存储配置项参数的结构体
    char                 *(*init_conf)(ngx_cycle_t *cycle, void *conf);//解析配置项完成后，这个方法被调用用以综合处理当前事件模块感兴趣的全部配置项
    ngx_event_actions_t     actions;
} ngx_event_module_t;
```
其中actions对应着事件驱动机制下，每个事件模块需要实现的10个抽象方法：
```cpp
typedef struct {
    ngx_int_t  (*add)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);//添加事件
    ngx_int_t  (*del)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);//删除事件
    //启用事件和禁用事件，目前事件框架不会调用这两个方法
    ngx_int_t  (*enable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    ngx_int_t  (*disable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    ngx_int_t  (*add_conn)(ngx_connection_t *c);//添加连接，意味着这个连接上的读写事件都添加到事件驱动机制中了
    ngx_int_t  (*del_conn)(ngx_connection_t *c, ngx_uint_t flags);
    //仅在多线程环境下调用，暂时Nginx不支持
    ngx_int_t  (*process_changes)(ngx_cycle_t *cycle, ngx_uint_t nowait);
    //在正常的工作循环中，将通过调用这个方法来处理时间事件。这个方法仅在方法ngx_process_events_and_timers中调用，它是处理、分发事件的核心
    ngx_int_t  (*process_events)(ngx_cycle_t *cycle, ngx_msec_t timer,
                   ngx_uint_t flags);
    ngx_int_t  (*init)(ngx_cycle_t *cycle, ngx_msec_t timer);//初始化事件驱动模块
    void       (*done)(ngx_cycle_t *cycle);//退出事件驱动模块前调用的方法
} ngx_event_actions_t;
```
下面看几个重要的方法。
实现init接口的方法ngx_epoll_init，它做了两件事情：
（1）调用epoll_create方法创建epoll对象
（2）创建event_list数组，用于进行epoll_wait调用时传递内核对象
源代码：
```cpp
static int                  ep = -1;//epoll对象描述符
static struct epoll_event  *event_list;//链表
static ngx_uint_t           nevents;//链表应该分配的大小
static ngx_int_t
ngx_epoll_init(ngx_cycle_t *cycle, ngx_msec_t timer)
{
    ngx_epoll_conf_t  *epcf;//存储配置项的结构体
    epcf = ngx_event_get_conf(cycle->conf_ctx, ngx_epoll_module);//获取解析所得的配置项的值
    if (ep == -1) {
        //创建epoll对象，在很多Linux内核版本中，并不处理这个函数的参数
        ep = epoll_create(cycle->connection_n / 2);
        //创建失败
        if (ep == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "epoll_create() failed");
            return NGX_ERROR;
        }
#if (NGX_HAVE_FILE_AIO)
        //异步I/O
        ngx_epoll_aio_init(cycle, epcf);
#endif
    }
    //解析配置项所得的epoll_wait一次可最多返回的时间个数较大
    if (nevents < epcf->events) {
        if (event_list) {
            //销毁原来的存储返回事件的链表
            ngx_free(event_list);
        }
        //重新分配链表空间
        event_list = ngx_alloc(sizeof(struct epoll_event) * epcf->events,
                               cycle->log);
        if (event_list == NULL) {
            return NGX_ERROR;
        }
    }
    //更新参数
    nevents = epcf->events;
    ngx_io = ngx_os_io;
    //Nginx时间框架处理事件时封装的接口
    ngx_event_actions = ngx_epoll_module_ctx.actions;
#if (NGX_HAVE_CLEAR_EVENT)
    //默认采用E模式使用epoll，NGX_HAVE_CLEAR_EVENT宏实际上就是在告诉nginx使用ET模式
    ngx_event_flags = NGX_USE_CLEAR_EVENT
#else
    ngx_event_flags = NGX_USE_LEVEL_EVENT
#endif
                      |NGX_USE_GREEDY_EVENT
                      |NGX_USE_EPOLL_EVENT;
    return NGX_OK;
}
```
下面以ngx_epoll_add_event方法为例看他们是如何调用epoll_ctl向epoll中添加事件的。
```cpp
static ngx_int_t
ngx_epoll_add_event(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags)
{
    int                  op;
    uint32_t             events, prev;
    ngx_event_t         *e;
    ngx_connection_t    *c;
    struct epoll_event   ee;
    //每个事件的data成员存放着其对应的ngx_connection_t连接
    c = ev->data;
    //下面会根据event参数确定当前事件是读事件还是写事件，这会决定events是加上EPOLLIN标志还是EPOLLOUT标志位
    events = (uint32_t) event;
    //关于下面这段代码的解释：需要结合后面的依据active标志位确定是否为活跃事件的代码来看
    /*首先，
    man epoll:
    Q:  What  happens  if you register the same file descriptor on an epoll  instance twice?
 
    A: You will probably get EEXIST.  However, it is  possible  to  add  a   duplicate  (dup(2),  dup2(2),  fcntl(2)  F_DUPFD) descriptor to the  same epoll instance.  This can be a useful technique for  filtering   events,  if the duplicate file descriptors are registered with different events masks.*/
    /*所以nginx这里就是为了避免这种情况，当要在epoll中加入对一个fd读事件(即NGX_READ_EVENT)的监听时，nginx先看一下与这个fd相关的写事件的状态，即e=c->write，如果此时e->active为1，说明该fd之前已经以NGX_WRITE_EVENT方式被加到epoll中了，此时只需要使用mod方式，将我们的需求加进去，否则才使用add方式，将该fd注册到epoll中。反之处理NGX_WRITE_EVENT时道理是一样的。*/
    
    if (event == NGX_READ_EVENT) {
        e = c->write;
        prev = EPOLLOUT;
#if (NGX_READ_EVENT != EPOLLIN)
        events = EPOLLIN;
#endif
    } else {
        e = c->read;
        prev = EPOLLIN;
#if (NGX_WRITE_EVENT != EPOLLOUT)
        events = EPOLLOUT;
#endif
    }
    //依据active标志位确定是否为活跃事件
    if (e->active) {
        op = EPOLL_CTL_MOD;//是活跃事件，修改事件
        events |= prev;
    } else {
        op = EPOLL_CTL_ADD;//不是活跃事件，添加事件
    }
    ee.events = events | (uint32_t) flags;//设定events标志位
    //ptr存储的是ngx_connection_t连接，instance是过期事件标志位
    ee.data.ptr = (void *) ((uintptr_t) c | ev->instance);
    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, ev->log, 0,
                   "epoll add event: fd:%d op:%d ev:%08XD",
                   c->fd, op, ee.events);
    if (epoll_ctl(ep, op, c->fd, &ee) == -1) {
        ngx_log_error(NGX_LOG_ALERT, ev->log, ngx_errno,
                      "epoll_ctl(%d, %d) failed", op, c->fd);
        return NGX_ERROR;
    }
    ev->active = 1;
#if 0
    ev->oneshot = (flags & NGX_ONESHOT_EVENT) ? 1 : 0;
#endif
    return NGX_OK;
}
```
ngx_epoll_process_events是实现了收集、分发事件的process_events接口的方法，代码如下：
```cpp
static ngx_int_t
ngx_epoll_process_events(ngx_cycle_t *cycle, ngx_msec_t timer, ngx_uint_t flags)
{
    int                events;
    uint32_t           revents;
    ngx_int_t          instance, i;
    ngx_uint_t         level;
    ngx_err_t          err;
    ngx_event_t       *rev, *wev, **queue;
    ngx_connection_t  *c;
    /* NGX_TIMER_INFINITE == INFTIM */
    ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                   "epoll timer: %M", timer);
    //调用epoll_wait获取事件
    events = epoll_wait(ep, event_list, (int) nevents, timer);
    err = (events == -1) ? ngx_errno : 0;
    //更新时间
    if (flags & NGX_UPDATE_TIME || ngx_event_timer_alarm) {
        ngx_time_update();
    }
    //epoll_wait出错处理
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
    //本次调用没有事件发生
    if (events == 0) {
        if (timer != NGX_TIMER_INFINITE) {
            return NGX_OK;
        }
        ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                      "epoll_wait() returned no events without timeout");
        return NGX_ERROR;
    }
    ngx_mutex_lock(ngx_posted_events_mutex);
    //遍历本次epoll_wait返回的所有事件
    for (i = 0; i < events; i++) {
        //获取连接ngx_connection_t的地址
        c = event_list[i].data.ptr;
        //连接的地址最后一位具有特殊意义：用于存储instance变量，将其取出来
        instance = (uintptr_t) c & 1;
        //无论是32位还是64位机器，其地址最后一位一定是0，获取真正地址
        c = (ngx_connection_t *) ((uintptr_t) c & (uintptr_t) ~1);
        //取出读事件
        rev = c->read;
        //判断读事件是否为过期事件
        if (c->fd == -1 || rev->instance != instance) {
            /*
             * the stale event from a file descriptor
             * that was just closed in this iteration
             */
            ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                           "epoll: stale event %p", c);
            continue;
        }
        //取出事件类型
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
        //是读事件且该事件是活跃的
        if ((revents & EPOLLIN) && rev->active) {
            if ((flags & NGX_POST_THREAD_EVENTS) && !rev->accept) {
                rev->posted_ready = 1;
            } else {
                rev->ready = 1;
            }
            //事件需要延后处理
            if (flags & NGX_POST_EVENTS) {
                /*如果要在post队列中延后处理该事件，首先要判断它是新连接时间还是普通事件
                以确定是把它加入到ngx_posted_accept_events队列或者ngx_posted_events队列中。*/
                queue = (ngx_event_t **) (rev->accept ?
                               &ngx_posted_accept_events : &ngx_posted_events);
                //将该事件添加到相应的延后队列中
                ngx_locked_post_event(rev, queue);
            } else {
                //立即调用事件回调方法来处理这个事件
                rev->handler(rev);
            }
        }
        wev = c->write;
        if ((revents & EPOLLOUT) && wev->active) {
            if (c->fd == -1 || wev->instance != instance) {
                /*
                 * the stale event from a file descriptor
                 * that was just closed in this iteration
                 */
                ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                               "epoll: stale event %p", c);
                continue;
            }
            if (flags & NGX_POST_THREAD_EVENTS) {
                wev->posted_ready = 1;
            } else {
                wev->ready = 1;
            }
            if (flags & NGX_POST_EVENTS) {
                ngx_locked_post_event(wev, &ngx_posted_events);
            } else {
                wev->handler(wev);
            }
        }
    }
    ngx_mutex_unlock(ngx_posted_events_mutex);
    return NGX_OK;
}
```
下面解释一下上面的代码中出现的过期事件。
举例，假设epoll_wait一次返回三个事件，在处理第一个事件的过程中，由于业务的需要关闭了一个连接，而这个连接恰好对应第三个事件。这样，在处理到第三个事件时，这个事件就是过期事件了，一旦处理就会出错。
那么，怎么解决这个问题呢？把关闭的这个连接的fd套接字置为-1？这样并不能解决所有问题。原因是ngx_connection_t的复用。
假设第三个事件对应的ngx_connection_t连接中的fd套接字原先是10，处理第一个事件时把这个连接的套接字关闭了，同时置为-1，并且调用ngx_free_connection将这个连接归还给连接池。在ngx_process_events方法的循环中开始i处理第二个事件，恰好第二个事件是建立新连接事件，调用ngx_get_connection从连接池中取出的连接非常可能是刚刚释放的第三个事件对应的连接。由于套接字10刚刚被释放，linux内核非常有可能把刚刚释放的套接字10又分配给新建立的连接。因此，在循环中处理第三个事件的时候，这个时间就是过期的了！它对应的事件是关闭的连接而不是新建立的连接。
如何解决这个问题呢？用instance标志位。当调用ngx_get_connection从连接池中获取一个新连接时，instance标志位会被置反。下面看函数ngx_get_connection中对应的代码（src/core/ngx_connection.c）：
```cpp
ngx_connection_t *
ngx_get_connection(ngx_socket_t s, ngx_log_t *log)
{
    ngx_uint_t         instance;
    ngx_event_t       *rev, *wev;
    ngx_connection_t  *c;
    /* disable warning: Win32 SOCKET is u_int while UNIX socket is int */
    if (ngx_cycle->files && (ngx_uint_t) s >= ngx_cycle->files_n) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "the new socket has number %d, "
                      "but only %ui files are available",
                      s, ngx_cycle->files_n);
        return NULL;
    }
    /* ngx_mutex_lock */
    //从连接池中获取一个连接
    c = ngx_cycle->free_connections;
    if (c == NULL) {
        ngx_drain_connections();
        c = ngx_cycle->free_connections;
    }
    if (c == NULL) {
        ngx_log_error(NGX_LOG_ALERT, log, 0,
                      "%ui worker_connections are not enough",
                      ngx_cycle->connection_n);
        /* ngx_mutex_unlock */
        return NULL;
    }
    ngx_cycle->free_connections = c->data;
    ngx_cycle->free_connection_n--;
    /* ngx_mutex_unlock */
    if (ngx_cycle->files) {
        ngx_cycle->files[s] = c;
    }
    //获取读事件和写事件
    rev = c->read;
    wev = c->write;
    ngx_memzero(c, sizeof(ngx_connection_t));
    c->read = rev;
    c->write = wev;
    c->fd = s;
    c->log = log;
    //获取instance标志位
    instance = rev->instance;
    ngx_memzero(rev, sizeof(ngx_event_t));
    ngx_memzero(wev, sizeof(ngx_event_t));
    //instance标志位置反
    rev->instance = !instance;
    wev->instance = !instance;
    rev->index = NGX_INVALID_INDEX;
    wev->index = NGX_INVALID_INDEX;
    rev->data = c;
    wev->data = c;
    wev->write = 1;
    return c;
}
```
这样，就在不增加任何成本的情况下解决了服务器开发时出现的过期事件问题。
实现done接口的ngx_epoll_done方法，在nginx退出服务时会得到调用。主要完成的工作是关闭epoll描述符ep，同时释放event_list数组。

参考资料：
[CSDN博客：关于ngx_epoll_add_event的一些解释](http://blog.csdn.net/dingyujie/article/details/9080789)
《深入理解Nginx》
[http://www.alidata.org/archives/1296](http://www.alidata.org/archives/1296)

