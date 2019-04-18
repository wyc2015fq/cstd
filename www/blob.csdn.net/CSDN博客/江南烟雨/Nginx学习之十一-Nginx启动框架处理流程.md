# Nginx学习之十一-Nginx启动框架处理流程 - 江南烟雨 - CSDN博客
2013年07月12日 11:24:54[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：14142
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)
## Nginx启动过程流程图
下面首先给出Nginx启动过程的流程图：
![](https://img-blog.csdn.net/20130712112106437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## ngx_cycle_t结构体
Nginx的启动初始化在src/core/nginx.c的main函数中完成，当然main函数是整个Nginx的入口，除了完成启动初始化任务以外，也必定是所有功能模块的入口之处。Nginx的初始化工作主要围绕一个类型为ngx_cycle_t类型的全局变量（cycle）展开。
ngx_cycle_t结构体类型：
```cpp
typedef struct ngx_cycle_s       ngx_cycle_t;
struct ngx_cycle_s {
    void                  ****conf_ctx;  //配置上下文数组(含所有模块)
    ngx_pool_t               *pool;      //内存池
    ngx_log_t                *log;       //日志
    ngx_log_t                 new_log;
    ngx_connection_t        **files;     //连接文件
    ngx_connection_t         *free_connections;  //空闲连接
    ngx_uint_t                free_connection_n; //空闲连接个数
    ngx_queue_t               reusable_connections_queue;  //再利用连接队列
    ngx_array_t               listening;     //监听数组
    ngx_array_t               pathes;        //路径数组
    ngx_list_t                open_files;    //打开文件链表
    ngx_list_t                shared_memory; //共享内存链表
    ngx_uint_t                connection_n;  //连接个数
    ngx_uint_t                files_n;       //打开文件个数
    ngx_connection_t         *connections;   //连接
    ngx_event_t              *read_events;   //读事件
    ngx_event_t              *write_events;  //写事件
    ngx_cycle_t              *old_cycle;     //old cycle指针
    ngx_str_t                 conf_file;     //配置文件
    ngx_str_t                 conf_param;    //配置参数
    ngx_str_t                 conf_prefix;   //配置前缀
    ngx_str_t                 prefix;        //前缀
    ngx_str_t                 lock_file;     //锁文件
    ngx_str_t                 hostname;      //主机名
};
```
## main函数源代码分析
下面具体看一下main函数为Nginx的启动过程做了哪些初始化。
```cpp
//Nginx中main函数，涵盖了Nginx启动过程
int ngx_cdecl
main(int argc, char *const *argv)
{
    ngx_int_t         i;
    ngx_log_t        *log;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_core_conf_t  *ccf;
    ngx_debug_init();
    if (ngx_strerror_init() != NGX_OK) {
        return 1;
    }
    //对参数选项进行处理
    if (ngx_get_options(argc, argv) != NGX_OK) {
        return 1;
    }
    //根据参数选项采取相应动作，比如：显示版本、测试配置等功能
    if (ngx_show_version) {
        ngx_write_stderr("nginx version: " NGINX_VER NGX_LINEFEED);
        if (ngx_show_help) {
            ngx_write_stderr(
                "Usage: nginx [-?hvVtq] [-s signal] [-c filename] "
                             "[-p prefix] [-g directives]" NGX_LINEFEED
                             NGX_LINEFEED
                "Options:" NGX_LINEFEED
                "  -?,-h         : this help" NGX_LINEFEED
                "  -v            : show version and exit" NGX_LINEFEED
                "  -V            : show version and configure options then exit"
                                   NGX_LINEFEED
                "  -t            : test configuration and exit" NGX_LINEFEED
                "  -q            : suppress non-error messages "
                                   "during configuration testing" NGX_LINEFEED
                "  -s signal     : send signal to a master process: "
                                   "stop, quit, reopen, reload" NGX_LINEFEED
#ifdef NGX_PREFIX
                "  -p prefix     : set prefix path (default: "
                                   NGX_PREFIX ")" NGX_LINEFEED
#else
                "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
                "  -c filename   : set configuration file (default: "
                                   NGX_CONF_PATH ")" NGX_LINEFEED
                "  -g directives : set global directives out of configuration "
                                   "file" NGX_LINEFEED NGX_LINEFEED
                );
        }
        if (ngx_show_configure) {
            ngx_write_stderr(
#ifdef NGX_COMPILER
                "built by " NGX_COMPILER NGX_LINEFEED
#endif
#if (NGX_SSL)
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
                "TLS SNI support enabled" NGX_LINEFEED
#else
                "TLS SNI support disabled" NGX_LINEFEED
#endif
#endif
                "configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
        }
        if (!ngx_test_config) {
            return 0;
        }
    }
    /* TODO */ ngx_max_sockets = -1;
    //初始化并更新时间
    ngx_time_init();
#if (NGX_PCRE)
    ngx_regex_init();
#endif
    ngx_pid = ngx_getpid();
    //初始化日志
    log = ngx_log_init(ngx_prefix);
    if (log == NULL) {
        return 1;
    }
    /* STUB */
#if (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif
    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */
    //清零全局变量ngx_cycle，并分配内存池
    ngx_memzero(&init_cycle, sizeof(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;
    init_cycle.pool = ngx_create_pool(1024, log);
    if (init_cycle.pool == NULL) {
        return 1;
    }
    //将命令行参数保存到ngx_os_argv、ngx_argc以及ngx_argv这几个全局的变量中。这算是一个备份存储，方便以后master进程做热代码替换之用。
    if (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        return 1;
    }
    //用Nginx运行是携带的目录参数初始化init_cycle
    if (ngx_process_options(&init_cycle) != NGX_OK) {
        return 1;
    }
    //完成操作系统的一些信息提取，信息会被保存到一些全局变量中
    if (ngx_os_init(log) != NGX_OK) {
        return 1;
    }
    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */
    //初始化一个做循环冗余校验的表，由此可以看出后续的循环冗余校验将采用高效的查表法
    if (ngx_crc32_table_init() != NGX_OK) {
        return 1;
    }
    //通过环境变量NGINX完成socket的继承，继承来的socket将会放到init_cycle的listening数组中。同时可以读取master进程传递的平滑升级信息等等
    if (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        return 1;
    }
    //初始化所有模块的index信息，即对所有模块进行编号
    //ngx_modules数却是在自动编译的时候生成的，位于objs/ngx_modules.c文件中
    ngx_max_module = 0;
    for (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = ngx_max_module++;
    }
    //完成很多信息的初始化工作
    cycle = ngx_init_cycle(&init_cycle);
    if (cycle == NULL) {
        if (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }
        return 1;
    }
    if (ngx_test_config) {
        if (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }
        return 0;
    }
    //若有信号
    if (ngx_signal) {
        return ngx_signal_process(cycle, ngx_signal);
    }
    ngx_os_status(cycle->log);
    ngx_cycle = cycle;
    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);
    //这个地方需要解释下
    //ccf->master是从配置文件中解析master_process配置项所得的值，初始化为NGX_CONF_UNSET（-1），在配置项中，如果flag类型的配置项master_process被设置为on，则其值为1，如果为off，则其值为0
    //ngx_process为全局变量，用于记录要采用的工作模式，未被初始化，因此初始值是0（uint型全局变量会被系统默认初始化为0）
    //相关宏定义如下
    //#define NGX_PROCESS_SINGLE     0
    //#define NGX_PROCESS_MASTER     1
    //#define NGX_PROCESS_SIGNALLER  2
    //#define NGX_PROCESS_WORKER     3
    //#define NGX_PROCESS_HELPER     4
    //因此，下面的if判断语句的含义就是：用来处理一种特殊情况，即如果在配置项中未设置master_process配置项或者是设置为打开，ngx_process未被设置，采用默认值0，这个时候要采用master工作模式。因为master_process优先级高，且nginx默认采用master模式
    ///如果在配置项中设置master_process为off，那么if依据不会执行。最终nginx工作模式取决于ngx_proces的初值0，即采用单进程模式
    if (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }
#if !(NGX_WIN32)
    //初始化信号；主要完成信号处理程序的注册
    if (ngx_init_signals(cycle->log) != NGX_OK) {
        return 1;
    }
    //若无继承sockets，且设置了守护进程表示，则创建守护进程
    if (!ngx_inherited && ccf->daemon) {
        if (ngx_daemon(cycle->log) != NGX_OK) {
            return 1;
        }
        ngx_daemonized = 1;
    }
    if (ngx_inherited) {
        ngx_daemonized = 1;
    }
#endif
    //创建进程记录文件；(非NGX_PROCESS_MASTER=1进程，不创建该文件)
    if (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        return 1;
    }
    if (cycle->log->file->fd != ngx_stderr) {
        if (ngx_set_stderr(cycle->log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");
            return 1;
        }
    }
    if (log->file->fd != ngx_stderr) {
        if (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }
    ngx_use_stderr = 0;
    //进入进程主循环
    //单进程方式运行Nginx
    if (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);
    } else {
	//以master多进程方式运行Nginx
        ngx_master_process_cycle(cycle);
    }
    return 0;
}
```
关于master进程和worker进程的工作原理，参考：[http://blog.csdn.net/xiajun07061225/article/details/9241179](http://blog.csdn.net/xiajun07061225/article/details/9241179)
下面分析初始化过程中比较重要的一个函数ngx_init_cycle()。
cycle就是周期的意思，对应着一次启动过程。也就是说，不论发生了上节介绍的三种启动方式的哪一种，nginx都会创建一个新的cycle与这次启动对应。
ngx_init_cycle提供的是配置解析接口。接口是一个切入点，通过少量代码提供一个完整功能的调用。配置解析接口分为两个阶段，一个是准备阶段，另一个就是真正开始调用配置解析。其核心代码如下：
```cpp
ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    void                *rv;
    char               **senv, **env;
    ngx_uint_t           i, n;
    ngx_log_t           *log;
    ngx_time_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_cycle_t         *cycle, **old;
    ngx_shm_zone_t      *shm_zone, *oshm_zone;
    ngx_list_part_t     *part, *opart;
    ngx_open_file_t     *file;
    ngx_listening_t     *ls, *nls;
    ngx_core_conf_t     *ccf, *old_ccf;
    ngx_core_module_t   *module;
    char                 hostname[NGX_MAXHOSTNAMELEN];
    //下面几行代码对时区和时间进行一次更新操作
    ngx_timezone_update();
    /* force localtime update with a new timezone */
    tp = ngx_timeofday();
    tp->sec = 0;
    ngx_time_update();
    log = old_cycle->log;
    //创建内存池
    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    if (pool == NULL) {
        return NULL;
    }
    pool->log = log;
    //在内存池上分配一个ngx_cycle_t对象
    cycle = ngx_pcalloc(pool, sizeof(ngx_cycle_t));
    if (cycle == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    cycle->pool = pool;
    cycle->log = log;
    cycle->new_log.log_level = NGX_LOG_ERR;
    cycle->old_cycle = old_cycle;
    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    if (cycle->conf_prefix.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    if (cycle->prefix.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    if (cycle->conf_file.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);
    cycle->conf_param.len = old_cycle->conf_param.len;
    cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
    if (cycle->conf_param.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;
    cycle->paths.elts = ngx_pcalloc(pool, n * sizeof(ngx_path_t *));
    if (cycle->paths.elts == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    cycle->paths.nelts = 0;
    cycle->paths.size = sizeof(ngx_path_t *);
    cycle->paths.nalloc = n;
    cycle->paths.pool = pool;
    //打开的文件个数计算
    if (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        for (part = old_cycle->open_files.part.next; part; part = part->next) {
            n += part->nelts;
        }
    } else {
	//采用一个系统默认值
        n = 20;
    }
    //初始化一个单链表容器，维护打开的文件
    if (ngx_list_init(&cycle->open_files, pool, n, sizeof(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        return NULL;
    }
    if (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        for (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }
    } else {
        n = 1;
    }
    //初始化单链表容器，维护共享内存
    if (ngx_list_init(&cycle->shared_memory, pool, n, sizeof(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        return NULL;
    }
    //初始化listening数组（存储监听端口和相关参数ngx_listening_t类型的动态数组）
    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;
    cycle->listening.elts = ngx_pcalloc(pool, n * sizeof(ngx_listening_t));
    if (cycle->listening.elts == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    cycle->listening.nelts = 0;
    cycle->listening.size = sizeof(ngx_listening_t);
    cycle->listening.nalloc = n;
    cycle->listening.pool = pool;
    ngx_queue_init(&cycle->reusable_connections_queue);
    //初始化conf_ctx指针数组（保存所有模块存储配置项结构体的指针）
    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * sizeof(void *));
    if (cycle->conf_ctx == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    if (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        return NULL;
    }
    /* on Linux gethostname() silently truncates name that does not fit */
    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);
    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    if (cycle->hostname.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    ngx_strlow(cycle->hostname.data, (u_char *) hostname, cycle->hostname.len);
    for (i = 0; ngx_modules[i]; i++) {
	//非核心模块跳过
        if (ngx_modules[i]->type != NGX_CORE_MODULE) {
            continue;
        }
        module = ngx_modules[i]->ctx;
	//调用所有核心模块的create_conf方法，构造用于存储配置项的结构体
	//那么非核心模块怎么办呢？其实这些模块大多从属于一个核心模块，如每个http模块都由ngx_http_module管理，这样ngx_http_module在解析自己感兴趣的配置项时，将会调用所有http模块约定的方法来创建存储配置项的结构体
        if (module->create_conf) {
            rv = module->create_conf(cycle);
            if (rv == NULL) {
                ngx_destroy_pool(pool);
                return NULL;
            }
            cycle->conf_ctx[ngx_modules[i]->index] = rv;
        }
    }
    senv = environ;
    ngx_memzero(&conf, sizeof(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, sizeof(ngx_str_t));
    if (conf.args == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    if (conf.temp_pool == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;
#if 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#endif
    //配置项解析
    if (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        return NULL;
    }
    if (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        return NULL;
    }
    if (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.data);
    }
    //调用所有核心模块的init_conf方法，这一步骤的目的在于让所有核心模块在解析玩配置项后可以做综合性处理
    for (i = 0; ngx_modules[i]; i++) {
        if (ngx_modules[i]->type != NGX_CORE_MODULE) {
            continue;
        }
        module = ngx_modules[i]->ctx;
        if (module->init_conf) {
            if (module->init_conf(cycle, cycle->conf_ctx[ngx_modules[i]->index])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                return NULL;
            }
        }
    }
    if (ngx_process == NGX_PROCESS_SIGNALLER) {
        return cycle;
    }
    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);
    if (ngx_test_config) {
        if (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            goto failed;
        }
    } else if (!ngx_is_init_cycle(old_cycle)) {
        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        if (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.data, old_ccf->pid.data) != 0)
        {
            /* new pid file name */
            if (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                goto failed;
            }
            ngx_delete_pidfile(old_cycle);
        }
    }
    if (ngx_test_lockfile(cycle->lock_file.data, log) != NGX_OK) {
        goto failed;
    }
    if (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        goto failed;
    }
    if (cycle->new_log.file == NULL) {
        cycle->new_log.file = ngx_conf_open_file(cycle, &error_log);
        if (cycle->new_log.file == NULL) {
            goto failed;
        }
    }
    //创建目录、打开文件、初始化共享内存等进程间通信方式
    /* open the new files */
    part = &cycle->open_files.part;
    file = part->elts;
    for (i = 0; /* void */ ; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL) {
                break;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }
        if (file[i].name.len == 0) {
            continue;
        }
        file[i].fd = ngx_open_file(file[i].name.data,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);
        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.data);
        if (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.data);
            goto failed;
        }
#if !(NGX_WIN32)
        if (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);
            goto failed;
        }
#endif
    }
    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;
    /* create shared memory */
    part = &cycle->shared_memory.part;
    shm_zone = part->elts;
    for (i = 0; /* void */ ; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL) {
                break;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }
        if (shm_zone[i].shm.size == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            goto failed;
        }
        shm_zone[i].shm.log = cycle->log;
        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;
        for (n = 0; /* void */ ; n++) {
            if (n >= opart->nelts) {
                if (opart->next == NULL) {
                    break;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }
            if (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                continue;
            }
            if (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                continue;
            }
            if (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
                if (shm_zone[i].init(&shm_zone[i], oshm_zone[n].data)
                    != NGX_OK)
                {
                    goto failed;
                }
                goto shm_zone_found;
            }
            ngx_shm_free(&oshm_zone[n].shm);
            break;
        }
        if (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            goto failed;
        }
        if (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            goto failed;
        }
        if (shm_zone[i].init(&shm_zone[i], NULL) != NGX_OK) {
            goto failed;
        }
    shm_zone_found:
        continue;
    }
    /* handle the listening sockets */
    //打开从配置文件中读取到的监听端口
    if (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        for (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }
        nls = cycle->listening.elts;
        for (n = 0; n < cycle->listening.nelts; n++) {
            for (i = 0; i < old_cycle->listening.nelts; i++) {
                if (ls[i].ignore) {
                    continue;
                }
                if (ngx_cmp_sockaddr(nls[n].sockaddr, ls[i].sockaddr) == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];
                    ls[i].remain = 1;
                    if (ls[n].backlog != nls[i].backlog) {
                        nls[n].listen = 1;
                    }
#if (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;
                    if (ls[i].accept_filter && nls[n].accept_filter) {
                        if (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].add_deferred = 1;
                        }
                    } else if (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;
                    } else if (nls[n].accept_filter) {
                        nls[n].add_deferred = 1;
                    }
#endif
#if (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                    if (ls[n].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;
                    } else if (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].add_deferred = 1;
                    }
#endif
                    break;
                }
            }
            if (nls[n].fd == -1) {
                nls[n].open = 1;
            }
        }
    } else {
        ls = cycle->listening.elts;
        for (i = 0; i < cycle->listening.nelts; i++) {
            ls[i].open = 1;
#if (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
            if (ls[i].accept_filter) {
                ls[i].add_deferred = 1;
            }
#endif
#if (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
            if (ls[i].deferred_accept) {
                ls[i].add_deferred = 1;
            }
#endif
        }
    }
    if (ngx_open_listening_sockets(cycle) != NGX_OK) {
        goto failed;
    }
    if (!ngx_test_config) {
        ngx_configure_listening_sockets(cycle);
    }
    /* commit the new cycle configuration */
    if (!ngx_use_stderr && cycle->log->file->fd != ngx_stderr) {
        if (ngx_set_stderr(cycle->log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");
        }
    }
    pool->log = cycle->log;
    //调用所有模块的init_module方法
    for (i = 0; ngx_modules[i]; i++) {
        if (ngx_modules[i]->init_module) {
            if (ngx_modules[i]->init_module(cycle) != NGX_OK) {
                /* fatal */
                exit(1);
            }
        }
    }
    return NULL;
}
```
转载请出名出处：[http://blog.csdn.net/xiajun07061225/article/details/9309273](http://blog.csdn.net/xiajun07061225/article/details/9309273)
## 参考资料：
《深入理解Nginx》-陶辉
[http://www.alidata.org/archives/1092](http://www.alidata.org/archives/1092)
[http://www.alidata.org/archives/1148](http://www.alidata.org/archives/1148)
[http://blog.csdn.net/livelylittlefish/article/details/7243718](http://blog.csdn.net/livelylittlefish/article/details/7243718)
[http://blog.csdn.net/livelylittlefish/article/details/7247080](http://blog.csdn.net/livelylittlefish/article/details/7247080)
[http://tengine.taobao.org/book/chapter_11.html](http://tengine.taobao.org/book/chapter_11.html)
