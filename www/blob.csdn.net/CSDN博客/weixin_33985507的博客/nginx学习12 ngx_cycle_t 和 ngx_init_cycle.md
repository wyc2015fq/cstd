# nginx学习12  ngx_cycle_t 和 ngx_init_cycle - weixin_33985507的博客 - CSDN博客
2015年07月11日 16:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

在nginx在启动过程，ngx_init_cycle这个函数最初始工作。变量的初始化存储在[ngx_cycle_t](http://blog.csdn.net/xiaoliangsky/article/details/39831035)这个结构体中，为了深入了解这个函数都做了那些初始化工作，就化时间研究了一下。并写下来以便以后參考。
1ngx_cycle_t
关于这个结构体[前面](http://blog.csdn.net/xiaoliangsky/article/details/39831035)已经简介过，这里不再赘述。
2ngx_init_cycle
ngx_cycle_t * ngx_init_cycle(ngx_cycle_t *old_cycle);
先看一下这个old_cycle參数从哪里来的：
```
ngx_cycle_t      *cycle, init_cycle;
    init_cycle.pool = ngx_create_pool(1024, log);//创建内存池
    if (init_cycle.pool == NULL) {
        return 1;
    }
    ......
    ......
    if (ngx_process_options(&init_cycle) != NGX_OK) {
        return 1;
    }
	
    cycle = ngx_init_cycle(&init_cycle);
```
可见在调用ngx_int_cycle之前，这个old_cycle已经保存了一些參数。这些參数包含：prefix,conf_prefix,conf_file, conf_para。主要在ngx_process_options这个函数中初始化old_cycle。这个函数比較简单，看一下代码：
```
static ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;
    if (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;
        if (!ngx_path_separator(*p)) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            if (p == NULL) {
                return NGX_ERROR;
            }
            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }
        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;
    } else {
#ifndef NGX_PREFIX
        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        if (p == NULL) {
            return NGX_ERROR;
        }
        if (ngx_getcwd(p, NGX_MAX_PATH) == 0) {//得到当前的工作文件夹
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            return NGX_ERROR;
        }
        len = ngx_strlen(p);
        p[len++] = '/';
        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;//配置文件夹
        cycle->prefix.len = len;
        cycle->prefix.data = p;//安装文件夹
#else
#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);//配置文件相对于安装的文件夹
#else
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);
#endif
    }
    if (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;
    } else {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }
    if (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        return NGX_ERROR;
    }
    for (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        if (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - ngx_cycle->conf_file.data + 1;
            cycle->conf_prefix.data = ngx_cycle->conf_file.data;
            break;
        }
    }
    if (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }
    if (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }
    return NGX_OK;
}
```
如今開始进入关键:ngx_init_cycle,依照运行步骤来进行
2.1调用ngx_timezone_update()、ngx_timeofday() 、ngx_time_update(0,0)做时间校准
```
ngx_timezone_update();
    /* force localtime update with a new timezone */
    tp = ngx_timeofday();
    tp->sec = 0;
    ngx_time_update();
```
2.2创建一个新的ngx_cycle_t变量cycle，而且初始化其大部分的成员字段。有一些是从传入的old_cycle直接拷贝过来的，这些字段包含：log,conf_prefix,prefix,conf_file,conf_param，初始化pathes数组、初始化open_files链表、初始化shared_memory链表、初始化listening数组等等。
```
cycle = ngx_pcalloc(pool, sizeof(ngx_cycle_t));//返回的新的cycle。以后这个cycle就是全局的
    if (cycle == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    cycle->pool = pool;//全局cycle的内存池
    cycle->log = log;//全局log
    cycle->new_log.log_level = NGX_LOG_ERR;
    cycle->old_cycle = old_cycle;//old_cycle
    cycle->conf_prefix.len = old_cycle->conf_prefix.len;//在nginx.c的ngx_process_options中获得
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    if (cycle->conf_prefix.data == NULL) {//配置文件路径为NULL
        ngx_destroy_pool(pool);
        return NULL;
    }
    cycle->prefix.len = old_cycle->prefix.len;//在nginx.c的ngx_process_options中获得
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    if (cycle->prefix.data == NULL) {//安装文件夹为NULL
        ngx_destroy_pool(pool);
        return NULL;
    }
    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    if (cycle->conf_file.data == NULL) {//配置文件相对于安装文件夹的路径为NULL
        ngx_destroy_pool(pool);
        return NULL;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);//拷贝配置文件相对于安装文件夹的路径
    cycle->conf_param.len = old_cycle->conf_param.len;
    cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
    if (cycle->conf_param.data == NULL) {//配置參数
        ngx_destroy_pool(pool);
        return NULL;
    }
    n = old_cycle->pathes.nelts ? old_cycle->pathes.nelts : 10;//Nginx的操作文件夹个数
    cycle->pathes.elts = ngx_pcalloc(pool, n * sizeof(ngx_path_t *));//数组申请内存
    if (cycle->pathes.elts == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    cycle->pathes.nelts = 0;
    cycle->pathes.size = sizeof(ngx_path_t *);
    cycle->pathes.nalloc = n;
    cycle->pathes.pool = pool;
	/*计算open_files的个数*/
    if (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        for (part = old_cycle->open_files.part.next; part; part = part->next) {
            n += part->nelts;
        }
    } else {
        n = 20;
    }
	//初始化open_files链表。
    if (ngx_list_init(&cycle->open_files, pool, n, sizeof(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        return NULL;
    }
	/*依据old_cycle的shared_memory单链表来初始化cycle的单链表*/
    if (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        for (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }
    } else {
        n = 1;
    }
	//ngx_list_init主要是初始链表的elts,并为其申请内存
    if (ngx_list_init(&cycle->shared_memory, pool, n, sizeof(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        return NULL;
    }
    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;//默认listening的大小为10
	/*初始化listening数组*/
    cycle->listening.elts = ngx_pcalloc(pool, n * sizeof(ngx_listening_t));
    if (cycle->listening.elts == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
    cycle->listening.nelts = 0;
    cycle->listening.size = sizeof(ngx_listening_t);
    cycle->listening.nalloc = n;
    cycle->listening.pool = pool;
	//初始化双向链表。主要存储着可反复用的连接ngx_connection_t
    ngx_queue_init(&cycle->reusable_connections_queue);
	//初始化conf_ctx,ngx_max_module是nginx中总的模块数
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
	//初始化hostname
    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);
    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    if (cycle->hostname.data == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }
	//主机名所有是小写
    ngx_strlow(cycle->hostname.data, (u_char *) hostname, cycle->hostname.len);
```
2.3调用全部NGX_CORE_MODULE的create_conf构建配置结构体。并保存在conf_ctx数组里面
```
for (i = 0; ngx_modules[i]; i++) {
        if (ngx_modules[i]->type != NGX_CORE_MODULE) {
            continue;
        }
        module = ngx_modules[i]->ctx;
        if (module->create_conf) {
            rv = module->create_conf(cycle);
            if (rv == NULL) {
                ngx_destroy_pool(pool);
                return NULL;
            }
			//返回的配置结构指针放到conf_ctx数组中，偏移位置为ngx_module_t.index
            cycle->conf_ctx[ngx_modules[i]->index] = rv;
        }
    }
    senv = environ;
	/*初始化暂时变量ngx_conf_t conf*/
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
    conf.cmd_type = NGX_MAIN_CONF;//
```
2.4从命令行和配置文件里把全部配置更新到cycle的conf_ctx中:首先调用ngx_conf_param把命令行中的指令（-g directives）转换为配置结构并把指针增加到cycle.conf_ctx中;接着调用ngx_conf_parse(..,filename)把配置文件里的指令转换为配置结构并把指针增加到cycle.conf_ctx中。
```
if (ngx_conf_param(&conf) != NGX_CONF_OK) {//src/core/ngx_conf_file.c
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
```
2.5调用全部核心模块的init函数初始化全部核心模块的配置结构体
```
for (i = 0; ngx_modules[i]; i++) {
        if (ngx_modules[i]->type != NGX_CORE_MODULE) {
            continue;
        }
        module = ngx_modules[i]->ctx;
        if (module->init_conf) {//调用模块的init_conf初始化全部核心模块的配置结构体
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
```
2.6获得核心模块ngx_core_dodule的配置结构，然后调用ngx_create_pidfile创建pid文件
```
ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);
    if (ngx_test_config) {
		//创建进程pid文件
        if (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            goto failed;
        }
    } else if (!ngx_is_init_cycle(old_cycle)) {
        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */
		//得到old_ccf配置结构体
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
```
2.7调用ngx_test_lockfile(filename,log)，ngx_create_pathes(cycle,user)，接着打开errorlog文件并赋值给cycle->new_log.file
```
if (ngx_test_lockfile(cycle->lock_file.data, log) != NGX_OK) {
        goto failed;
    }
    if (ngx_create_pathes(cycle, ccf->user) != NGX_OK) {
        goto failed;
    }
    if (cycle->new_log.file == NULL) {
        cycle->new_log.file = ngx_conf_open_file(cycle, &error_log);
        if (cycle->new_log.file == NULL) {
            goto failed;
        }
    }
```
2.8打开新文件，在第2步的时候提到cycle->open_files这个链表是空的。仅仅是给它预先分配了空间，并没有数据。这里之所以可能会有文件被打开，预计是前面读配置文件的时候，调用各个钩子的过程中，填充了这个链表，把ngx_open_file_t结构变量填充进来（结构体中包括要打开文件的路径信息），接着改动一下cycle的成员：cycle->log = &cycle->new_log;pool->log = &cycle->new_log;
2.9创建共享内存，和open_files类似，在第2步的时候cycle－>share_memory也初始化为一个空的链表。也是预分配了空间，假设此时链表中已经被填充了ngx_shm_zone_t结构变量（当中包括须要共享内存的尺寸和标识等信息）。那么这里就会分配共享内存，而且调用合适的初始化钩子初始化分配的共享内存，每块共享内存都会有name标识。这里也会做一些排重，已经分配的就不会再去分配。从对open_files和share_memory的处理过程能够看出，nginx在资源管理上是集中分配的，请求资源的时候分配说明性的结构变量，然后在恰当的时机才去真正分配资源。
2.10处理listening sockets，cycle->listening是ngx_listening_t结构的数组。把cycle->listening于old_cycle->listening进行比較。设置cycle->listening的一些状态信息。接着调用ngx_open_listening_sockets(cycle)启动cycle->listening中的全部监听socket，循环调用socket,bind,listen完毕服务端监听socket的启动。接着调用ngx_configure_listening_sockets(cycle)配置监听socket。会依据ngx_listening_t中的状态信息设置socket的读写缓存和TCP_DEFER_ACCEPT。
```
if (old_cycle->listening.nelts) {//假设old_cycle中已经有套接字句柄
        ls = old_cycle->listening.elts;
        for (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;//正常关闭应经打开的监听port
        }
        nls = cycle->listening.elts;
        for (n = 0; n < cycle->listening.nelts; n++) {
            for (i = 0; i < old_cycle->listening.nelts; i++) {
                if (ls[i].ignore) {//标识位为1表示跳过设置ngx_lisenting_t中的套接字。
                    continue;	   //为0表示正常初始化套接字
                }
/********************************************************************************************************/
				//有个问题:nls[n].sockaddr是什么时候初始化的
/********************************************************************************************************/
                if (ngx_cmp_sockaddr(nls[n].sockaddr, ls[i].sockaddr) == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];
                    ls[i].remain = 1;//不关闭已经打开的套接字
                    if (ls[n].backlog != nls[i].backlog) {
                        nls[n].listen = 1;//套接字已监听
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
	/*
	这里有个问题:cycle->listening中的每一个成员ngx_listening_t的sockaddr在哪里初始化的
	由于ngx_open_listening_sockets函数会用这个变量来创建新的套接字
	*/
	//启动cycle->listening中的全部监听socket，循环调用socket,bind,listen完毕服务端监听socket的启动
    if (ngx_open_listening_sockets(cycle) != NGX_OK) {
        goto failed;
    }
    if (!ngx_test_config) {//接着调用ngx_configure_listening_sockets(cycle)配置监听socket
        ngx_configure_listening_sockets(cycle);
```
这个阶段还有个问题没解决（见凝视）？？？？？？？？？？？？？？？。
2.11调用全部模块的init_module方法，仅仅有ngx_event_core_module定义了init_module。
```
for (i = 0; ngx_modules[i]; i++) {
        if (ngx_modules[i]->init_module) {
            if (ngx_modules[i]->init_module(cycle) != NGX_OK) {
                /* fatal */
                exit(1);
            }
        }
    }
```
2.12关闭或者删除一些残留在old_cycle中的资源。首先释放不用的共享内存，接着关闭不使用的监听socket，再关闭不使用的打开文件，最后把old_cycle放入ngx_old_cycles中，这是一个ngx_cycle_t *的数组，最后设定一个定时器。定期回调ngx_cleaner_event清理ngx_old_cycles。这里设置了30000ms清理一次。
```
if (ngx_temp_pool == NULL) {
        ngx_temp_pool = ngx_create_pool(128, cycle->log);
        if (ngx_temp_pool == NULL) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            exit(1);
        }
        n = 10;
        ngx_old_cycles.elts = ngx_pcalloc(ngx_temp_pool,
                                          n * sizeof(ngx_cycle_t *));
        if (ngx_old_cycles.elts == NULL) {
            exit(1);
        }
        ngx_old_cycles.nelts = 0;
        ngx_old_cycles.size = sizeof(ngx_cycle_t *);
        ngx_old_cycles.nalloc = n;
        ngx_old_cycles.pool = ngx_temp_pool;
		//初始化ngx_cleaner_event事件
        ngx_cleaner_event.handler = ngx_clean_old_cycles;//注意这个handler
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.data = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }
    ngx_temp_pool->log = cycle->log;
    old = ngx_array_push(&ngx_old_cycles);
    if (old == NULL) {
        exit(1);
    }
    *old = old_cycle;
	//将ngx_cleaner_event事件加入到定时器，定时清理old_cycle
    if (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);//30000毫秒
        ngx_cleaner_event.timer_set = 1;
    }
```
[http://blog.csdn.net/xiaoliangsky/article/details/39996563](http://blog.csdn.net/xiaoliangsky/article/details/39996563)
以下附上一张图：
![](https://img-blog.csdn.net/20141011173108406?)
图片来源http://blog.csdn.net/lengzijian/article/details/7575813#plain
參考资料：
http://blog.csdn.net/lengzijian/article/details/7575813#plain
版权声明：本文博客原创文章，博客，未经同意，不得转载。
