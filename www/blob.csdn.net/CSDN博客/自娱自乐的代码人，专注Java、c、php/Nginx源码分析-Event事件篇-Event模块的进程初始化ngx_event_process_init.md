# Nginx源码分析 - Event事件篇 - Event模块的进程初始化ngx_event_process_init - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年09月15日 17:59:02[initphp](https://me.csdn.net/initphp)阅读数：3004
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









前面一篇我们讲解了《[Nginx源码分析 - Event事件篇 - Event模块和配置的初始化 》](http://blog.csdn.net/initphp/article/details/52434261)    ，了解了整个Event模块分为：**ngx_events_module 事件模块 、ngx_event_core_module 事件核心模块 和 epoll/queue/win32_select事件模型模块**

这一篇，主要讲解一个重要的函数：**ngx_event_process_init**

Event事件模块的大部分初始化都在ngx_event_process_init中完成。

### 源码分析

#### 1. ngx_event_core_module 模块中配置ngx_event_process_init



```cpp
/**
 * Event核心模块
 * ngx_event_module_init：模块初始化
 * ngx_event_process_init：进程初始化
 * 类型：NGX_EVENT_MODULE
 */
ngx_module_t ngx_event_core_module = { NGX_MODULE_V1,
		&ngx_event_core_module_ctx, /* module context */
		ngx_event_core_commands, /* module directives */
		NGX_EVENT_MODULE, /* module type */
		NULL, /* init master */
		ngx_event_module_init, /* init module */
		ngx_event_process_init, /* init process */
		NULL, /* init thread */
		NULL, /* exit thread */
		NULL, /* exit process */
		NULL, /* exit master */
		NGX_MODULE_V1_PADDING };
```

我们可以看到，在ngx_event_core_module模块中，配置了init_process的回调函数。通过这个回调函数，进行event的初始化。


#### 2. ngx_worker_process_init 工作进程初始化的时候，调用init_process的回调函数



```cpp
/* 对模块进程初始化 - 这边初始化的是所有的模块有init_process回调函数的进行初始化工作 */
	for (i = 0; cycle->modules[i]; i++) {
		if (cycle->modules[i]->init_process) {
			if (cycle->modules[i]->init_process(cycle) == NGX_ERROR) {
				/* fatal */
				exit(2);
			}
		}
	}
```



在工作进程初始化的函数中，我们看到就会调用模块的init_process。这个函数，我们在前面的文章中也详细讲解过。

#### 3. ngx_event_process_init 实现



```cpp
/**
 * Event模块的进程初始化
 */
static ngx_int_t ngx_event_process_init(ngx_cycle_t *cycle) {
    ngx_uint_t m, i;
    ngx_event_t *rev, *wev;
    ngx_listening_t *ls;
    ngx_connection_t *c, *next, *old;
    ngx_core_conf_t *ccf;
    ngx_event_conf_t *ecf;
    ngx_event_module_t *module;

    /* 获取event模块的配置 */
    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);
    ecf = ngx_event_get_conf(cycle->conf_ctx, ngx_event_core_module);

    /* 多进程情况下，进程数大于1的情况，使用accept_mutex锁。惊群的时候需要用到。*/
    if (ccf->master && ccf->worker_processes > 1 && ecf->accept_mutex) {
        ngx_use_accept_mutex = 1;
        ngx_accept_mutex_held = 0;
        ngx_accept_mutex_delay = ecf->accept_mutex_delay;

    } else {
        ngx_use_accept_mutex = 0;
    }

#if (NGX_WIN32)

    /*
     * disable accept mutex on win32 as it may cause deadlock if
     * grabbed by a process which can't accept connections
     */

    ngx_use_accept_mutex = 0;

#endif

    /* 初始化全局队列：ngx_posted_accept_events和ngx_posted_events */
    ngx_queue_init(&ngx_posted_accept_events);
    ngx_queue_init(&ngx_posted_events);

    /* 初始化event模块的时间 */
    if (ngx_event_timer_init(cycle->log) == NGX_ERROR) {
        return NGX_ERROR;
    }

    /**
     * 找到事件模型的模块，例如epoll/kqueue
     */
    for (m = 0; cycle->modules[m]; m++) {
        if (cycle->modules[m]->type != NGX_EVENT_MODULE) {
            continue;
        }

        if (cycle->modules[m]->ctx_index != ecf->use) {
            continue;
        }

        module = cycle->modules[m]->ctx;

        /**
         * 调用epoll/kqueue等模型模块的init初始化函数
         * epoll调用的是ngx_epoll_init这个方法
         */
        if (module->actions.init(cycle, ngx_timer_resolution) != NGX_OK) {
            /* fatal */
            exit(2);
        }

        break;
    }

#if !(NGX_WIN32)

    if (ngx_timer_resolution && !(ngx_event_flags & NGX_USE_TIMER_EVENT)) {
        struct sigaction sa;
        struct itimerval itv;

        ngx_memzero(&sa, sizeof(struct sigaction));
        sa.sa_handler = ngx_timer_signal_handler;
        sigemptyset(&sa.sa_mask);

        if (sigaction(SIGALRM, &sa, NULL) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                    "sigaction(SIGALRM) failed");
            return NGX_ERROR;
        }

        itv.it_interval.tv_sec = ngx_timer_resolution / 1000;
        itv.it_interval.tv_usec = (ngx_timer_resolution % 1000) * 1000;
        itv.it_value.tv_sec = ngx_timer_resolution / 1000;
        itv.it_value.tv_usec = (ngx_timer_resolution % 1000) * 1000;

        if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                    "setitimer() failed");
        }
    }

    if (ngx_event_flags & NGX_USE_FD_EVENT) {
        struct rlimit rlmt;

        if (getrlimit(RLIMIT_NOFILE, &rlmt) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                    "getrlimit(RLIMIT_NOFILE) failed");
            return NGX_ERROR;
        }

        cycle->files_n = (ngx_uint_t) rlmt.rlim_cur;

        cycle->files = ngx_calloc(sizeof(ngx_connection_t *) * cycle->files_n,
                cycle->log);
        if (cycle->files == NULL) {
            return NGX_ERROR;
        }
    }

#else

    if (ngx_timer_resolution && !(ngx_event_flags & NGX_USE_TIMER_EVENT)) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                "the \"timer_resolution\" directive is not supported "
                "with the configured event method, ignored");
        ngx_timer_resolution = 0;
    }

#endif

    /* 分配一块内存，存储连接 */
    cycle->connections = ngx_alloc(
            sizeof(ngx_connection_t) * cycle->connection_n, cycle->log);
    if (cycle->connections == NULL) {
        return NGX_ERROR;
    }

    c = cycle->connections;

    /* 分配一块内存，存放读取事件 */
    cycle->read_events = ngx_alloc(sizeof(ngx_event_t) * cycle->connection_n,
            cycle->log);
    if (cycle->read_events == NULL) {
        return NGX_ERROR;
    }

    rev = cycle->read_events;
    for (i = 0; i < cycle->connection_n; i++) {
        rev[i].closed = 1;
        rev[i].instance = 1;
    }

    /* 分配一块内存，存储写入事件*/
    cycle->write_events = ngx_alloc(sizeof(ngx_event_t) * cycle->connection_n,
            cycle->log);
    if (cycle->write_events == NULL) {
        return NGX_ERROR;
    }

    wev = cycle->write_events;
    for (i = 0; i < cycle->connection_n; i++) {
        wev[i].closed = 1;
    }

    i = cycle->connection_n;
    next = NULL;

    /* 初始化连接数据 */
    do {
        i--;

        c[i].data = next;
        c[i].read = &cycle->read_events[i];
        c[i].write = &cycle->write_events[i];
        c[i].fd = (ngx_socket_t) -1;

        next = &c[i];
    } while (i);

    /* 空闲的连接 */
    cycle->free_connections = next;
    cycle->free_connection_n = cycle->connection_n;

    /* for each listening socket */

    /* 初始化侦听器 */
    ls = cycle->listening.elts;
    for (i = 0; i < cycle->listening.nelts; i++) {

#if (NGX_HAVE_REUSEPORT)
        if (ls[i].reuseport && ls[i].worker != ngx_worker) {
            continue;
        }
#endif

        c = ngx_get_connection(ls[i].fd, cycle->log);

        if (c == NULL) {
            return NGX_ERROR;
        }

        c->type = ls[i].type;
        c->log = &ls[i].log;

        c->listening = &ls[i];
        ls[i].connection = c;

        rev = c->read;

        rev->log = c->log;
        rev->accept = 1;

#if (NGX_HAVE_DEFERRED_ACCEPT)
        rev->deferred_accept = ls[i].deferred_accept;
#endif

        if (!(ngx_event_flags & NGX_USE_IOCP_EVENT)) {
            if (ls[i].previous) {

                /*
                 * delete the old accept events that were bound to
                 * the old cycle read events array
                 */

                old = ls[i].previous->connection;

                if (ngx_del_event(old->read, NGX_READ_EVENT,
                        NGX_CLOSE_EVENT) == NGX_ERROR) {
                    return NGX_ERROR;
                }

                old->fd = (ngx_socket_t) -1;
            }
        }

#if (NGX_WIN32)

        if (ngx_event_flags & NGX_USE_IOCP_EVENT) {
            ngx_iocp_conf_t *iocpcf;

            rev->handler = ngx_event_acceptex;

            if (ngx_use_accept_mutex) {
                continue;
            }

            if (ngx_add_event(rev, 0, NGX_IOCP_ACCEPT) == NGX_ERROR) {
                return NGX_ERROR;
            }

            ls[i].log.handler = ngx_acceptex_log_error;

            iocpcf = ngx_event_get_conf(cycle->conf_ctx, ngx_iocp_module);
            if (ngx_event_post_acceptex(&ls[i], iocpcf->post_acceptex)
                    == NGX_ERROR)
            {
                return NGX_ERROR;
            }

        } else {
            rev->handler = ngx_event_accept;

            if (ngx_use_accept_mutex) {
                continue;
            }

            if (ngx_add_event(rev, NGX_READ_EVENT, 0) == NGX_ERROR) {
                return NGX_ERROR;
            }
        }

#else

        /* 事件ev的回调函数handler ngx_event_accept tcp 或者ngx_event_recvmsg udp */
        rev->handler =
                (c->type == SOCK_STREAM) ? ngx_event_accept : ngx_event_recvmsg;

        if (ngx_use_accept_mutex
#if (NGX_HAVE_REUSEPORT)
        && !ls[i].reuseport
#endif
        ) {
            continue;
        }

        if (ngx_add_event(rev, NGX_READ_EVENT, 0) == NGX_ERROR) {
            return NGX_ERROR;
        }

#endif

    }

    return NGX_OK;
}
```



**module->actions.init **主要是调用epoll/kqueque等模型模块的init初始化方法。epoll调用是**ngx_epoll_init**这个方法。

**ecf->use**在配置文件中，我们配置了:“use epoll;”。但是这里存储的是 epoll/kqueue等模块是索引ID。通过索引ID就可以快速在cycle->modules找到对应的模块。

具体如何确定是哪一个Linux事件模型(select/kqueue/epoll)，是在**ngx_event_core_init_conf**方法中定义的。



```cpp
* 找到事件模型的模块，例如epoll/kqueue
	 */
	for (m = 0; cycle->modules[m]; m++) {
		if (cycle->modules[m]->type != NGX_EVENT_MODULE) {
			continue;
		}

		if (cycle->modules[m]->ctx_index != ecf->use) {
			continue;
		}

		module = cycle->modules[m]->ctx;

		/**
		 * 调用epoll/kqueue等模型模块的init初始化函数
		 * epoll调用的是ngx_epoll_init这个方法
		 */
		if (module->actions.init(cycle, ngx_timer_resolution) != NGX_OK) {
			/* fatal */
			exit(2);
		}

		break;
	}
```




事件event的回调函数，主要包含两个：**ngx_event_accept或者ngx_event_recvmsg**。一个是TCP接收连接，一个是UDP方式接收连接。

**TCP连接和读取事件逻辑：**

在 Nginx 的初始化启动过程中，worker 工作进程会调用事件模块的ngx_event_process_init 方法为每个监听套接字ngx_listening_t 分配一个 ngx_connection_t 连接，并设置该连接上读事件的回调方法handler 为ngx_event_accept，同时将读事件挂载到epoll 事件机制中等待监听套接字连接上的可读事件发生，到此，Nginx 就可以接收并处理来自客户端的请求。当监听套接字连接上的可读事件发生时，即该连接上有来自客户端发出的连接请求，则会启动读read事件的handler 回调方法ngx_event_accept，在ngx_event_accept 方法中调用accept() 函数接收来自客户端的连接请求，成功建立连接之后，ngx_event_accept 函数调用监听套接字ngx_listen_t上的handler 回调方法ls->handler(c)（该回调方法就是ngx_http_init_connection 主要用于初始化ngx_connection_t客户端连接）。ngx_http_init_connection 会将rev->handler的回调函数修改成： ngx_http_wait_request_handler，该回调函数主要用于处理read事件的数据读取。后续当有read事件上来的时候，就会回调ngx_http_wait_request_handler函数，而非ngx_event_accept函数。

1. ngx_event_process_init 初始化的时候，rev->handler的回调函数是ngx_event_accept。所以当第一次连接上来，进入事件循环的时候，读取事件调用的是**ngx_event_accept**方法。




```cpp
/* 事件ev的回调函数handler ngx_event_accept或者ngx_event_recvmsg */
        rev->handler =
                (c->type == SOCK_STREAM) ? ngx_event_accept : ngx_event_recvmsg;
```

2. ngx_event_accept中会调用ls->handler回调函数ngx_http_init_connection。



```cpp
log->data = NULL;
		log->handler = NULL;

		/* 回调函数  ngx_http_init_connection */
		ls->handler(c);

		if (ngx_event_flags & NGX_USE_KQUEUE_EVENT) {
			ev->available--;
		}
```
3. ngx_http_init_connection初始化一个http的连接，并且将rev->handler回调函数修改成ngx_http_wait_request_handler，主要用于接收read事件数据。所有后面进入事件循环后，read事件调用的是ngx_http_wait_request_handler函数。





```cpp
void
ngx_http_init_connection(ngx_connection_t *c)
{
....
    rev = c->read;
    rev->handler = ngx_http_wait_request_handler;
    c->write->handler = ngx_http_empty_handler;
}
```

4. ls->handler的回调函数是如何赋值的？


你需要看一下ngx_http.c中的ngx_http_block函数，该函数在模块命令初始化的时候会回调，并且调用  ngx_http_optimize_servers方法，并且进一步调用ngx_http_init_listening初始化的监听器，然后最终调用**ngx_http_add_listening**方法。



```cpp
static ngx_listening_t *
ngx_http_add_listening(ngx_conf_t *cf, ngx_http_conf_addr_t *addr)
{
    ngx_listening_t           *ls;
    ngx_http_core_loc_conf_t  *clcf;
    ngx_http_core_srv_conf_t  *cscf;

    ls = ngx_create_listening(cf, &addr->opt.sockaddr.sockaddr,
                              addr->opt.socklen);
    if (ls == NULL) {
        return NULL;
    }

    ls->addr_ntop = 1;

    ls->handler = ngx_http_init_connection;

    cscf = addr->default_server;
    ls->pool_size = cscf->connection_pool_size;
    ls->post_accept_timeout = cscf->client_header_timeout;

    clcf = cscf->ctx->loc_conf[ngx_http_core_module.ctx_index];

    ls->logp = clcf->error_log;
    ls->log.data = &ls->addr_text;
    ls->log.handler = ngx_accept_log_error;

#if (NGX_WIN32)
    {
    ngx_iocp_conf_t  *iocpcf = NULL;

    if (ngx_get_conf(cf->cycle->conf_ctx, ngx_events_module)) {
        iocpcf = ngx_event_get_conf(cf->cycle->conf_ctx, ngx_iocp_module);
    }
    if (iocpcf && iocpcf->acceptex_read) {
        ls->post_accept_buffer_size = cscf->client_header_buffer_size;
    }
    }
#endif

    ls->backlog = addr->opt.backlog;
    ls->rcvbuf = addr->opt.rcvbuf;
    ls->sndbuf = addr->opt.sndbuf;

    ls->keepalive = addr->opt.so_keepalive;
#if (NGX_HAVE_KEEPALIVE_TUNABLE)
    ls->keepidle = addr->opt.tcp_keepidle;
    ls->keepintvl = addr->opt.tcp_keepintvl;
    ls->keepcnt = addr->opt.tcp_keepcnt;
#endif

#if (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ls->accept_filter = addr->opt.accept_filter;
#endif

#if (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
    ls->deferred_accept = addr->opt.deferred_accept;
#endif

#if (NGX_HAVE_INET6 && defined IPV6_V6ONLY)
    ls->ipv6only = addr->opt.ipv6only;
#endif

#if (NGX_HAVE_SETFIB)
    ls->setfib = addr->opt.setfib;
#endif

#if (NGX_HAVE_TCP_FASTOPEN)
    ls->fastopen = addr->opt.fastopen;
#endif

#if (NGX_HAVE_REUSEPORT)
    ls->reuseport = addr->opt.reuseport;
#endif

    return ls;
}
```








下一篇，我们就可以具体看一下，**ngx_epoll_module.c**具体实现。










