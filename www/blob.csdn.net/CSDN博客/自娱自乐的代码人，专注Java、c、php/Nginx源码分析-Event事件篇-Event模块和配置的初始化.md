# Nginx源码分析 - Event事件篇 - Event模块和配置的初始化 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年09月04日 20:04:31[initphp](https://me.csdn.net/initphp)阅读数：3209
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









Event的模块和配置的初始化，必须得结合我们讲过的两篇文章：[《Nginx源码分析 - 主流程篇 - 模块的初始化》](http://blog.csdn.net/initphp/article/details/51898955)     和  [《Nginx源码分析 - 主流程篇 - 解析配置文件》](http://blog.csdn.net/initphp/article/details/51911189)


前面我们讲解了模块的初始化以及核心模块的配置文件的解析。而Event的配置解析会比核心模块解析会复杂一些，但是原理是一样的。

### Event模块的数据结构

event事件模块的配置：



```cpp
events {  
        use epoll;  
        worker_connections  1024;  
}
```

event事件模块，配置分为两层：**ngx_events_module 事件模块 和 ngx_event_core_module 事件核心模块**。
ngx_events_module：模块类型NGX_CORE_MODULE，所以此模块在最外层核心模块解析“events”命令的时候会回调**ngx_events_block**函数。

ngx_event_core_module：模块类型NGX_EVENT_MODULE，所以此模块在ngx_events_block函数被回调后，才能解析配置信息

**最外层的events模块，类型NGX_CORE_MODULE，属于核心模块，核心模块在最开始配置文件初始化的时候，就会调用指令的命令集。所以在核心模块启动的时候就会调用events的模块配置解析指令函数：ngx_events_block**


#### ngx_events_module 事件模块



```cpp
/**
 * event模块命令集
 * 回调函数：ngx_events_block
 * 用于解析 event{} 块中的配置参数
 */
static ngx_command_t ngx_events_commands[] = {

{ ngx_string("events"), NGX_MAIN_CONF | NGX_CONF_BLOCK | NGX_CONF_NOARGS,
		ngx_events_block, 0, 0, NULL },

ngx_null_command };

/**
 * event模块上下文
 */
static ngx_core_module_t ngx_events_module_ctx = { ngx_string("events"), NULL,
		ngx_event_init_conf };

/**
 * event模块
 * 模块类型：NGX_CORE_MODULE
 * 模块类型为核心模块，所以在ngx_init_cycle就会初始化conf
 */
ngx_module_t ngx_events_module = { NGX_MODULE_V1, &ngx_events_module_ctx, /* module context */
ngx_events_commands, /* module directives */
NGX_CORE_MODULE, /* module type */
NULL, /* init master */
NULL, /* init module */
NULL, /* init process */
NULL, /* init thread */
NULL, /* exit thread */
NULL, /* exit process */
NULL, /* exit master */
NGX_MODULE_V1_PADDING };
```



#### ngx_event_core_module 事件核心模块



```cpp
/**
 * event核心模块名称
 */
static ngx_str_t event_core_name = ngx_string("event_core");

/**
 * 定义Event核心模块的命令参数
 * 主要：
 * worker_connections 工作线程最大连接数
 * use 使用什么模型，例如epoll
 * multi_accept
 * accept_mutex_delay
 * debug_connection
 */
static ngx_command_t ngx_event_core_commands[] = {

{ ngx_string("worker_connections"), NGX_EVENT_CONF | NGX_CONF_TAKE1,
		ngx_event_connections, 0, 0, NULL },

		{ ngx_string("use"), NGX_EVENT_CONF | NGX_CONF_TAKE1, ngx_event_use, 0,
				0, NULL },

		{ ngx_string("multi_accept"), NGX_EVENT_CONF | NGX_CONF_FLAG,
				ngx_conf_set_flag_slot, 0, offsetof(ngx_event_conf_t,
						multi_accept), NULL },

		{ ngx_string("accept_mutex"), NGX_EVENT_CONF | NGX_CONF_FLAG,
				ngx_conf_set_flag_slot, 0, offsetof(ngx_event_conf_t,
						accept_mutex), NULL },

		{ ngx_string("accept_mutex_delay"), NGX_EVENT_CONF | NGX_CONF_TAKE1,
				ngx_conf_set_msec_slot, 0, offsetof(ngx_event_conf_t,
						accept_mutex_delay), NULL },

		{ ngx_string("debug_connection"), NGX_EVENT_CONF | NGX_CONF_TAKE1,
				ngx_event_debug_connection, 0, 0, NULL },

		ngx_null_command };

/**
 * Event核心模块上下文
 * ngx_event_core_create_conf：创建配置文件
 * ngx_event_core_init_conf：初始化配置文件
 */
ngx_event_module_t ngx_event_core_module_ctx = { &event_core_name,
		ngx_event_core_create_conf, /* create configuration */
		ngx_event_core_init_conf, /* init configuration */

		{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } };

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



#### ngx_event_conf_t 事件conf结构



```cpp
/**
 * event事件模块配置的结构对象
 */
typedef struct {
    ngx_uint_t    connections;
    ngx_uint_t    use;

    ngx_flag_t    multi_accept;
    ngx_flag_t    accept_mutex;

    ngx_msec_t    accept_mutex_delay;

    u_char       *name;

#if (NGX_DEBUG)
    ngx_array_t   debug_connection;
#endif
} ngx_event_conf_t;
```

### Event模块结构图

![](https://img-blog.csdn.net/20160905004817101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




### Event模块的初始化

#### 1. ngx_event_module_init 模块初始化函数



ngx_event_module_init方法为事件核心模块的初始化函数。



```cpp
/**
 * event事件核心模块初始化函数
 */
static ngx_int_t ngx_event_module_init(ngx_cycle_t *cycle) {
	void ***cf;
	u_char *shared;
	size_t size, cl;
	ngx_shm_t shm;
	ngx_time_t *tp;
	ngx_core_conf_t *ccf;
	ngx_event_conf_t *ecf;

	/* 获取配置信息 */
	cf = ngx_get_conf(cycle->conf_ctx, ngx_events_module);
	ecf = (*cf)[ngx_event_core_module.ctx_index];

	if (!ngx_test_config && ngx_process <= NGX_PROCESS_MASTER) {
		ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
				"using the \"%s\" event method", ecf->name);
	}

	ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

	ngx_timer_resolution = ccf->timer_resolution;

#if !(NGX_WIN32)
	{
		ngx_int_t limit;
		struct rlimit rlmt;

		if (getrlimit(RLIMIT_NOFILE, &rlmt) == -1) {
			ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
					"getrlimit(RLIMIT_NOFILE) failed, ignored");

		} else {
			if (ecf->connections > (ngx_uint_t) rlmt.rlim_cur
					&& (ccf->rlimit_nofile == NGX_CONF_UNSET
							|| ecf->connections
									> (ngx_uint_t) ccf->rlimit_nofile)) {
				limit = (ccf->rlimit_nofile == NGX_CONF_UNSET) ?
						(ngx_int_t) rlmt.rlim_cur : ccf->rlimit_nofile;

				ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
						"%ui worker_connections exceed "
								"open file resource limit: %i",
						ecf->connections, limit);
			}
		}
	}
#endif /* !(NGX_WIN32) */

	if (ccf->master == 0) {
		return NGX_OK;
	}

	if (ngx_accept_mutex_ptr) {
		return NGX_OK;
	}

	/* cl should be equal to or greater than cache line size */

	cl = 128;

	size = cl /* ngx_accept_mutex */
	+ cl /* ngx_connection_counter */
	+ cl; /* ngx_temp_number */

#if (NGX_STAT_STUB)

	size += cl /* ngx_stat_accepted */
	+ cl /* ngx_stat_handled */
	+ cl /* ngx_stat_requests */
	+ cl /* ngx_stat_active */
	+ cl /* ngx_stat_reading */
	+ cl /* ngx_stat_writing */
	+ cl; /* ngx_stat_waiting */

#endif

	shm.size = size;
	shm.name.len = sizeof("nginx_shared_zone") - 1;
	shm.name.data = (u_char *) "nginx_shared_zone";
	shm.log = cycle->log;

	if (ngx_shm_alloc(&shm) != NGX_OK) {
		return NGX_ERROR;
	}

	shared = shm.addr;

	ngx_accept_mutex_ptr = (ngx_atomic_t *) shared;
	ngx_accept_mutex.spin = (ngx_uint_t) -1;

	if (ngx_shmtx_create(&ngx_accept_mutex, (ngx_shmtx_sh_t *) shared,
			cycle->lock_file.data) != NGX_OK) {
		return NGX_ERROR;
	}

	ngx_connection_counter = (ngx_atomic_t *) (shared + 1 * cl);

	(void) ngx_atomic_cmp_set(ngx_connection_counter, 0, 1);

	ngx_log_debug2(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
			"counter: %p, %uA",
			ngx_connection_counter, *ngx_connection_counter);

	ngx_temp_number = (ngx_atomic_t *) (shared + 2 * cl);

	tp = ngx_timeofday();

	ngx_random_number = (tp->msec << 16) + ngx_pid;

#if (NGX_STAT_STUB)

	ngx_stat_accepted = (ngx_atomic_t *) (shared + 3 * cl);
	ngx_stat_handled = (ngx_atomic_t *) (shared + 4 * cl);
	ngx_stat_requests = (ngx_atomic_t *) (shared + 5 * cl);
	ngx_stat_active = (ngx_atomic_t *) (shared + 6 * cl);
	ngx_stat_reading = (ngx_atomic_t *) (shared + 7 * cl);
	ngx_stat_writing = (ngx_atomic_t *) (shared + 8 * cl);
	ngx_stat_waiting = (ngx_atomic_t *) (shared + 9 * cl);

#endif

	return NGX_OK;
}
```



### Event模块配置的初始化

#### 1. ngx_events_module 模块配置初始化

ngx_init_cycle方法中的模块初始化。ngx_events_module类型为**NGX_CORE_MODULE**，所以在ngx_init_cycle中就会进行核心模块的初始化。

但是ngx_events_module中的create_conf方法为NULL，所以不会调用创建配置的方法。



```cpp
/*
     * 核心模块的配置文件创建
     * 配置创建调用nginx.c 中的 ngx_core_module_create_conf
     * */
    for (i = 0; cycle->modules[i]; i++) {
        if (cycle->modules[i]->type != NGX_CORE_MODULE) {
            continue;
        }

        module = cycle->modules[i]->ctx;

        if (module->create_conf) {
            rv = module->create_conf(cycle); //模块回调函数，创建模块的配置信息
            if (rv == NULL) {
                ngx_destroy_pool(pool);
                return NULL;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv; //配置文件复制
        }
    }
```



#### 2. ngx_conf_parse 解析顶层“event”的配置

ngx_init_cycle方法中会调用ngx_conf_parse方法，并且解析的/usr/local/nginx/conf/nginx.conf配置文件。此次调用只解析最顶层的配置信息“events”，**而不会解析{}块中的内容**



```cpp
/* 解析命令行中的配置参数；例如：nginx -t -c /usr/local/nginx/conf/nginx.conf */
    if (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        return NULL;
    }

    /* 解析配置文件/usr/local/nginx/conf/nginx.conf 信息 */
    if (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        return NULL;
    }
```



#### 3. ngx_events_block 解析events块block中的内容

ngx_events_block方法为ngx_events_commands命令集的回调函数。在最顶层解析nginx.conf文件的时候，会进行核心模块的命令集遍历。（参考： [Nginx源码分析 - 主流程篇 - 解析配置文件](http://blog.csdn.net/initphp/article/details/51911189)    中的**ngx_conf_handler**）   会遍历模块命令集的cmd->set方法。


ngx_events_block中主要创建**ngx_event_core_module**事件的核心模块以及配置信息。



```cpp
/**
 * 模块解析
 * 事件模块配置如下：
 * events {
    worker_connections  1024;
    }
         光使用核心配置的方式，只能解析到 events 这一层。
         如果需要继续往{}中的内容解析，就得重新调用ngx_conf_parse进行解析
 */
static char *
ngx_events_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
	char *rv;
	void ***ctx;
	ngx_uint_t i;
	ngx_conf_t pcf;
	ngx_event_module_t *m;

	if (*(void **) conf) {
		return "is duplicate";
	}

	/* count the number of the event modules and set up their indices */

	ngx_event_max_module = ngx_count_modules(cf->cycle, NGX_EVENT_MODULE);

	/* 分配内存空间 */
	ctx = ngx_pcalloc(cf->pool, sizeof(void *));
	if (ctx == NULL) {
		return NGX_CONF_ERROR ;
	}

	*ctx = ngx_pcalloc(cf->pool, ngx_event_max_module * sizeof(void *));
	if (*ctx == NULL) {
		return NGX_CONF_ERROR ;
	}

	*(void **) conf = ctx;

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

	return NGX_CONF_OK;
}
```



#### 4. ngx_event_core_create_conf和ngx_event_core_init_conf

ngx_event_core_create_conf：主要是创建event事件核心模块

ngx_event_core_init_conf：初始化event事件核心模块




```cpp
/**
 * 创建Event的核心配置文件
 */
static void *
ngx_event_core_create_conf(ngx_cycle_t *cycle) {
    ngx_event_conf_t *ecf;

    /* 分配配置文件内容 */
    ecf = ngx_palloc(cycle->pool, sizeof(ngx_event_conf_t));
    if (ecf == NULL) {
        return NULL;
    }

    /* 设置默认值 */
    ecf->connections = NGX_CONF_UNSET_UINT;
    ecf->use = NGX_CONF_UNSET_UINT;
    ecf->multi_accept = NGX_CONF_UNSET;
    ecf->accept_mutex = NGX_CONF_UNSET;
    ecf->accept_mutex_delay = NGX_CONF_UNSET_MSEC;
    ecf->name = (void *) NGX_CONF_UNSET;

#if (NGX_DEBUG)

    if (ngx_array_init(&ecf->debug_connection, cycle->pool, 4,
                    sizeof(ngx_cidr_t)) == NGX_ERROR)
    {
        return NULL;
    }

#endif

    return ecf;
}

/**
 * 初始化Event的核心配置文件
 */
static char *
ngx_event_core_init_conf(ngx_cycle_t *cycle, void *conf) {
    ngx_event_conf_t *ecf = conf;

#if (NGX_HAVE_EPOLL) && !(NGX_TEST_BUILD_EPOLL)
    int fd;
#endif
    ngx_int_t i;
    ngx_module_t *module;
    ngx_event_module_t *event_module;

    module = NULL;

#if (NGX_HAVE_EPOLL) && !(NGX_TEST_BUILD_EPOLL)

    fd = epoll_create(100);

    if (fd != -1) {
        (void) close(fd);
        module = &ngx_epoll_module;

    } else if (ngx_errno != NGX_ENOSYS) {
        module = &ngx_epoll_module;
    }

#endif

#if (NGX_HAVE_DEVPOLL) && !(NGX_TEST_BUILD_DEVPOLL)

    module = &ngx_devpoll_module;

#endif

#if (NGX_HAVE_KQUEUE)

    module = &ngx_kqueue_module;

#endif

#if (NGX_HAVE_SELECT)

    if (module == NULL) {
        module = &ngx_select_module;
    }

#endif

    /**
     * 查询使用的事件模型:epoll、kqueue等
     * 因为在模块初始化的时候，epoll\kqueue等event的模型模块都会被初始化
     * 但是每个服务器只能选择一种相应的事件模型，所以选择一个适合自己的模块
     */
    if (module == NULL) {
        for (i = 0; cycle->modules[i]; i++) {

            if (cycle->modules[i]->type != NGX_EVENT_MODULE) {
                continue;
            }

            event_module = cycle->modules[i]->ctx;

            if (ngx_strcmp(event_module->name->data, event_core_name.data)
                    == 0) {
                continue;
            }

            module = cycle->modules[i];
            break;
        }
    }

    if (module == NULL) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0, "no events module found");
        return NGX_CONF_ERROR ;
    }

    ngx_conf_init_uint_value(ecf->connections, DEFAULT_CONNECTIONS);
    cycle->connection_n = ecf->connections;

    /**
     * 存储使用的事件模型模块索引 例如：epoll、kqueue
     * nginx.conf中存储的是：use epoll;
     * 这里会找到cycle->modules的具体模块的索引值，存储最终的索引值
     */
    ngx_conf_init_uint_value(ecf->use, module->ctx_index);

    event_module = module->ctx;
    ngx_conf_init_ptr_value(ecf->name, event_module->name->data);

    ngx_conf_init_value(ecf->multi_accept, 0);
    ngx_conf_init_value(ecf->accept_mutex, 1);
    ngx_conf_init_msec_value(ecf->accept_mutex_delay, 500);

    return NGX_CONF_OK;
}
```

#### 5. 获取event配置

获取event的配置，先获取ngx_events_module配置，然后再到ngx_events_module模块上找到ngx_event_core_module 事件核心模块的配置。




```cpp
#define ngx_event_get_conf(conf_ctx, module)                                  \
             (*(ngx_get_conf(conf_ctx, ngx_events_module))) [module.ctx_index];
```
































