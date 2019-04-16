# Nginx源码分析 - Event事件篇 - epoll事件模块 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年11月03日 19:30:33[initphp](https://me.csdn.net/initphp)阅读数：2788
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









这一篇主要讲解一下epoll事件模块。Nginx支持多种事件模块：epoll/pool/select/kqueue等。epoll比较常用，我们也比较熟悉。

Nginx的epoll事件模块在/event/module/ngx_epoll_module.c

![](https://img-blog.csdn.net/20161103193012728?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




### Epoll模块的数据结构

#### 1. epoll模块命令集 ngx_epoll_commands



```cpp
/**
 * epoll模块命令集
 * epoll_events：这个配置项表示调用一次epoll_wait最多可以返回的事件数
 * worker_aio_requests：指明在开启异步I/O且使用io_setup系统调用初始化异步I/O上下文环境时，初始分配的异步I/O事件个数
 */
static ngx_command_t  ngx_epoll_commands[] = {

    { ngx_string("epoll_events"),
      NGX_EVENT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      0,
      offsetof(ngx_epoll_conf_t, events),
      NULL },

    { ngx_string("worker_aio_requests"),
      NGX_EVENT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      0,
      offsetof(ngx_epoll_conf_t, aio_requests),
      NULL },

      ngx_null_command
};
```



#### 2. epoll模块上下文 ngx_epoll_module_ctx****



```cpp
/**
 * epoll模块上下文
 * ngx_epoll_create_conf ： 配置文件创建
 * ngx_epoll_init_conf ： 配置文件初始化
 * ngx_event_actions_t actions ： 事件的action对象结构
 */
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
#if (NGX_HAVE_EVENTFD)
        ngx_epoll_notify,                /* trigger a notify */
#else
        NULL,                            /* trigger a notify */
#endif
        ngx_epoll_process_events,        /* process the events */
        ngx_epoll_init,                  /* init the events */
        ngx_epoll_done,                  /* done the events */
    }
};
```




#### 3. epoll模块配置 ngx_epoll_module



```cpp
/**
 * epoll模块配置
 * ngx_epoll_module_ctx：epoll模块上下文
 * ngx_epoll_commands：epoll模块上下文
 * epoll/kqueue事件模块等没有初始化的方法
 */
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


### Epoll模块的初始化

#### 1. 配置文件初始化

epool模块属于Event模块下面的子模块，配置文件初始化的时候，在Event解析配置文件的核心函数：**ngx_events_block 中。**



```cpp
/* 模块初始化，如果是NGX_EVENT_MODULE，则调用模块的create_conf方法 */
	for (i = 0; cf->cycle->modules[i]; i++) {
		if (cf->cycle->modules[i]->type != NGX_EVENT_MODULE) {
			continue;
		}

		m = cf->cycle->modules[i]->ctx;

		if (m->create_conf) {
			(*ctx)[cf->cycle->modules[i]->ctx_index] = m->create_conf(
					cf->cycle);
			if ((*ctx)[cf->cycle->modules[i]->ctx_index] == NULL) {
				return NGX_CONF_ERROR ;
			}
		}
	}

	/* event*/
	pcf = *cf;
	cf->ctx = ctx;
	cf->module_type = NGX_EVENT_MODULE;
	cf->cmd_type = NGX_EVENT_CONF;

	/* 调用配置解析，这次解析的是 块中的内容，非文件内容 */
	rv = ngx_conf_parse(cf, NULL);

	*cf = pcf;

	if (rv != NGX_CONF_OK) {
		return rv;
	}

	/* 初始化 模块的init_conf 方法*/
	for (i = 0; cf->cycle->modules[i]; i++) {
		if (cf->cycle->modules[i]->type != NGX_EVENT_MODULE) {
			continue;
		}

		m = cf->cycle->modules[i]->ctx;

		if (m->init_conf) {
			rv = m->init_conf(cf->cycle,
					(*ctx)[cf->cycle->modules[i]->ctx_index]);
			if (rv != NGX_CONF_OK) {
				return rv;
			}
		}
	}
```

配置文件初始化




```cpp
/**
 * 创建配置
 */
static void *
ngx_epoll_create_conf(ngx_cycle_t *cycle)
{
    ngx_epoll_conf_t  *epcf;

    epcf = ngx_palloc(cycle->pool, sizeof(ngx_epoll_conf_t));
    if (epcf == NULL) {
        return NULL;
    }

    epcf->events = NGX_CONF_UNSET;
    epcf->aio_requests = NGX_CONF_UNSET;

    return epcf;
}

/**
 * 初始化配置
 */
static char *
ngx_epoll_init_conf(ngx_cycle_t *cycle, void *conf)
{
    ngx_epoll_conf_t *epcf = conf;

    ngx_conf_init_uint_value(epcf->events, 512);
    ngx_conf_init_uint_value(epcf->aio_requests, 32);

    return NGX_CONF_OK;
}
```

#### 2. 模块初始化

epool模块属于Event模块下面的子模块，所以没有设置独立的init_process初始化回调函数。

epoll事件模块的初始化是在Event大模块初始化的时候进行初始化的。

初始化放在：**ngx_event_process_init**中进行



```cpp
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
```


ngx_epoll_init 模块初始化：



```cpp
/**
 * epoll模块初始化
 */
static ngx_int_t
ngx_epoll_init(ngx_cycle_t *cycle, ngx_msec_t timer)
{
	/* 存储配置项的结构体   */
    ngx_epoll_conf_t  *epcf;

    /* 获取配置文件 */
    epcf = ngx_event_get_conf(cycle->conf_ctx, ngx_epoll_module);

    if (ep == -1) {
    	/* 创建epoll对象，在很多Linux内核版本中，并不处理这个函数的参数   */
        ep = epoll_create(cycle->connection_n / 2);

        if (ep == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "epoll_create() failed");
            return NGX_ERROR;
        }

#if (NGX_HAVE_EVENTFD)
        if (ngx_epoll_notify_init(cycle->log) != NGX_OK) {
            ngx_epoll_module_ctx.actions.notify = NULL;
        }
#endif

#if (NGX_HAVE_FILE_AIO)
        ngx_epoll_aio_init(cycle, epcf);
#endif

#if (NGX_HAVE_EPOLLRDHUP)
        ngx_epoll_test_rdhup(cycle);
#endif
    }

    /* 解析配置项所得的epoll_wait一次可最多返回的时间个数较大   */
    if (nevents < epcf->events) {
        if (event_list) {
            ngx_free(event_list);
        }

        event_list = ngx_alloc(sizeof(struct epoll_event) * epcf->events,
                               cycle->log);
        if (event_list == NULL) {
            return NGX_ERROR;
        }
    }

    /* 更新全局变量 */
    nevents = epcf->events;

    ngx_io = ngx_os_io;

    ngx_event_actions = ngx_epoll_module_ctx.actions;

#if (NGX_HAVE_CLEAR_EVENT)
    ngx_event_flags = NGX_USE_CLEAR_EVENT
#else
    ngx_event_flags = NGX_USE_LEVEL_EVENT
#endif
                      |NGX_USE_GREEDY_EVENT
                      |NGX_USE_EPOLL_EVENT;

    return NGX_OK;
}
```




### 核心函数

#### 1. ngx_epoll_process_events  实现了收集、分发事件接口



```cpp
/**
 * 处理事件  ==> ngx_process_events
 * 实现了收集、分发事件接口(void) ngx_process_events(cycle, timer, flags);
 */
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

    /* 调用epoll_wait获取事件   */
    events = epoll_wait(ep, event_list, (int) nevents, timer);

    err = (events == -1) ? ngx_errno : 0;

    /* 更新时间 */
    if (flags & NGX_UPDATE_TIME || ngx_event_timer_alarm) {
        ngx_time_update();
    }

    /* epoll_wait出错处理  */
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

    /* 本次调用没有事件发生  */
    if (events == 0) {
        if (timer != NGX_TIMER_INFINITE) {
            return NGX_OK;
        }

        ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                      "epoll_wait() returned no events without timeout");
        return NGX_ERROR;
    }

    /* 遍历本次epoll_wait返回的所有事件  */
    for (i = 0; i < events; i++) {
    	/* 获取连接ngx_connection_t的地址  */
        c = event_list[i].data.ptr;

        instance = (uintptr_t) c & 1;
        c = (ngx_connection_t *) ((uintptr_t) c & (uintptr_t) ~1);

        /* 取出读事件  */
        rev = c->read;

        if (c->fd == -1 || rev->instance != instance) {

            /*
             * the stale event from a file descriptor
             * that was just closed in this iteration
             */

            ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                           "epoll: stale event %p", c);
            continue;
        }

        /* 取出事件类型  */
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

        /* 读取事件 EPOLLIN */
        if ((revents & EPOLLIN) && rev->active) {

#if (NGX_HAVE_EPOLLRDHUP)
            if (revents & EPOLLRDHUP) {
                rev->pending_eof = 1;
            }

            rev->available = 1;
#endif

            rev->ready = 1;

            /* 如果事件抢到锁，则放入事件队列 */
            if (flags & NGX_POST_EVENTS) {
                queue = rev->accept ? &ngx_posted_accept_events
                                    : &ngx_posted_events;

                ngx_post_event(rev, queue);

            } else {
            	/* 没有抢到锁，立即调用事件回调方法来处理这个事件  */
                rev->handler(rev);
            }
        }

        wev = c->write;

        /* 写事件 EPOLLOUT*/
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

            wev->ready = 1;
#if (NGX_THREADS)
            wev->complete = 1;
#endif

            if (flags & NGX_POST_EVENTS) {
                ngx_post_event(wev, &ngx_posted_events);

            } else {
                wev->handler(wev);
            }
        }
    }

    return NGX_OK;
}
```



#### 2.  ngx_epoll_add_event 添加一个事件



```cpp
/**
 * 添加一个事件
 */
static ngx_int_t
ngx_epoll_add_event(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags)
{
    int                  op;
    uint32_t             events, prev;
    ngx_event_t         *e;
    ngx_connection_t    *c;
    struct epoll_event   ee;

    /* 每个事件的data成员存放着其对应的ngx_connection_t连接  */
    c = ev->data;

    events = (uint32_t) event;

    /* 判断事件类型。如果是写事件，c->write 如果是读事件，c->read*/
    if (event == NGX_READ_EVENT) {
        e = c->write;
        prev = EPOLLOUT;
#if (NGX_READ_EVENT != EPOLLIN|EPOLLRDHUP)
        events = EPOLLIN|EPOLLRDHUP;
#endif

    } else {
        e = c->read;
        prev = EPOLLIN|EPOLLRDHUP;
#if (NGX_WRITE_EVENT != EPOLLOUT)
        events = EPOLLOUT;
#endif
    }

    /* 依据active标志位确定是否为活跃事件  */
    if (e->active) {
        op = EPOLL_CTL_MOD;
        events |= prev;

    } else {
        op = EPOLL_CTL_ADD;
    }

    ee.events = events | (uint32_t) flags;
    ee.data.ptr = (void *) ((uintptr_t) c | ev->instance);

    ngx_log_debug3(NGX_LOG_DEBUG_EVENT, ev->log, 0,
                   "epoll add event: fd:%d op:%d ev:%08XD",
                   c->fd, op, ee.events);

    /* 新增一个事件 */
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






















