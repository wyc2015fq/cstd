# Nginx 缓存机制 详解之一 缓存管理进程 - youbingchen的博客 - CSDN博客





2016年06月24日 18:12:22[youbingchen](https://me.csdn.net/youbingchen)阅读数：9679








> 
Nginx缓存的基本思路就是利用请求的局部性原理，下次访问不再连接后端服务器。 

  Nginx是通过设置xxx_cache_path(比如proxy_cache_path),主要用来创建管理cache的共享 内存数据结构（红黑树和对列），另一个是xxx_cache主要 用来前面的参数创建内存空间。


命令xxx_cache_path，会调用ngx_http_file_cache_set_slot函数，同时会用到ngx_http_file_cache_t这个数据结构，它主要用来管理所有的cache文件，它本身不保存cache，只是保存管理cache的数据结构。每一个xxx_cache_path 都会创建一个ngx_http_file_cache_t。因为每一个xxx_cache_path都会创建一个cache文件。

```cpp
typedef struct {
    ngx_rbtree_t                     rbtree;
    ngx_rbtree_node_t                sentinel;
    ngx_queue_t                      queue;
    ngx_atomic_t                     cold;  // 表示这个cache是否已经被加载
    ngx_atomic_t                     loading;  //那个进程正在 加载这个cache
    off_t                            size;   // 文件大小
    ngx_uint_t                       count;
    ngx_uint_t                       watermark;
} ngx_http_file_cache_sh_t;


struct ngx_http_file_cache_s {
    ngx_http_file_cache_sh_t        *sh;
    ngx_slab_pool_t                 *shpool;

    ngx_path_t                      *path;   // cache的 目录
    ngx_path_t                      *temp_path; //当前的path下的 所有cache文件的最大值

    off_t                            max_size;
    size_t                           bsize;

    time_t                           inactive; //如果多久不用就会被删除

    time_t                           fail_time;
    //当前有多少个cache文件(超过loader_files之后会被清0)
    ngx_uint_t                       files;
    //这个值也就是一个阈值，当load的文件个数大于这个值之后，load进程会短暂的休眠(时间位loader_sleep)
    ngx_uint_t                       loader_files;
    //最后被manage或者loader访问的时间
    ngx_msec_t                       last;
    //和上面的loader_files配合使用，当文件个数大于loader_files，就会休眠
    ngx_msec_t                       loader_sleep;
    //配合上面的last，也就是loader遍历的休眠间隔。
    ngx_msec_t                       loader_threshold;
    //共享内存的地址
    ngx_shm_zone_t                  *shm_zone;
};
```

有一个很关键的结构就是ngx_path_t,它保存了当前的cache的一些信息，以及对应的cache管理回调函数manger和loader.

```cpp
typedef struct {
    ngx_str_t                  name;  //cache名称
    size_t                     len;
    size_t                     level[3];
    //对应的回调，以及回调数据
    ngx_path_manager_pt        manager;
    ngx_path_loader_pt         loader;
    void                      *data;

    u_char                    *conf_file;
    ngx_uint_t                 line;
} ngx_path_t;
```

ngx_http_file_cache_set_slot函数 

```cpp
char *
ngx_http_file_cache_set_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf){
   ngx_http_file_cache_t  *cache;
..............................................
//初始化cache对象，这两个回调后续会详细分析
    cache->path->manager = ngx_http_file_cache_manager;
    cache->path->loader = ngx_http_file_cache_loader;
    cache->path->data = cache;
    cache->path->conf_file = cf->conf_file->file.name.data;
    cache->path->line = cf->conf_file->line;
//初始化这几个阈值
    cache->loader_files = loader_files;
    cache->loader_sleep = (ngx_msec_t) loader_sleep;
    cache->loader_threshold = (ngx_msec_t) loader_threshold;
//将path添加到全局的路径管理中
    if (ngx_add_path(cf, &cache->path) != NGX_OK) {
        return NGX_CONF_ERROR;
    }
//创建共享内存
    cache->shm_zone = ngx_shared_memory_add(cf, &name, size, cmd->post);
    if (cache->shm_zone == NULL) {
        return NGX_CONF_ERROR;
    }

    if (cache->shm_zone->data) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "duplicate zone \"%V\"", &name);
        return NGX_CONF_ERROR;
    }

//设置初始化函数
    cache->shm_zone->init = ngx_http_file_cache_init;
    cache->shm_zone->data = cache;
//设置cache
    cache->inactive = inactive;
    cache->max_size = max_size;
    }
```

上面还有一个命令xxx_cache，这个主要是调用ngx_http_fastcgi_cache函数。这个函数更简单,就是简单的查找出上面创建的共享内存

```cpp
static char *
ngx_http_fastcgi_cache(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
 ..................................................
//查找出共享内存
    flcf->upstream.cache = ngx_shared_memory_add(cf, &value[1], 0,
                                                 &ngx_http_fastcgi_module);
    if (flcf->upstream.cache == NULL) {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}
```

Nginx服务器启动后，会对本地磁盘上的缓存文件进行扫描，在内存中建立缓存索引，并有专门的进程对缓存文件进行过期判断、更新等进行管理。这里就牵扯到Nginx的两个 框架中的两个进程：
- 缓存管理器
- 缓存加载器 
**上面两个进程共用的是同一个函数ngx_cache_manager_process_cycle,只是传递的第二个参数是不一样**

第二个参数的结构体
```cpp
typedef struct {
    ngx_event_handler_pt       handler;
    char                      *name;
    ngx_msec_t                 delay;
} ngx_cache_manager_ctx_t;
```

# 缓存管理器

> 
定期检查缓存状态、查看缓存总量是否超出限制、如果超出，就移除其中最少使用的 部分 

  这个进程主要是调用函数ngx_cache_manager_process_cycle


### ngx_cache_manager_process_cycle函数

```cpp
static ngx_cache_manager_ctx_t  ngx_cache_manager_ctx = {
    ngx_cache_manager_process_handler, "cache manager process", 0
};

static void
ngx_cache_manager_process_cycle(ngx_cycle_t *cycle, void *data)
{
    ngx_cache_manager_ctx_t *ctx = data;

    void         *ident[4];
    ngx_event_t   ev;

    /*
     * Set correct process type since closing listening Unix domain socket
     * in a master process also removes the Unix domain socket file.
     */
    ngx_process = NGX_PROCESS_HELPER;

    ngx_close_listening_sockets(cycle);

    /* Set a moderate number of connections for a helper process. */
    cycle->connection_n = 512;

    ngx_worker_process_init(cycle, -1);

    ngx_memzero(&ev, sizeof(ngx_event_t));
    //最重要在这里设置传递进来的ctx->handler为定时回调
    ev.handler = ctx->handler;
    ev.data = ident;
    ev.log = cycle->log;
    ident[3] = (void *) -1;

    ngx_use_accept_mutex = 0;

    ngx_setproctitle(ctx->name);
    // 添加定时器
    ngx_add_timer(&ev, ctx->delay);
    //进入 循环
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

```cpp
ngx_process_events_and_timers()--->ngx_event_expire_timers()---->ngx_cache_manager_process_handler()
```

### ngx_cache_manager_process_handler函数

```cpp
static void
ngx_cache_manager_process_handler(ngx_event_t *ev)
{
    time_t        next, n;
    ngx_uint_t    i;
    ngx_path_t  **path;

    next = 60 * 60;

    path = ngx_cycle->paths.elts;
    //遍历所有的cache目录
    for (i = 0; i < ngx_cycle->paths.nelts; i++) {
    // 调用manager回调
        if (path[i]->manager) {
            n = path[i]->manager(path[i]->data);
           //取得下一次的定时器的时间，可以看到是取n和next的最小值
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

### ngx_http_file_cache_manager函数

> 
这里manger的函数是ngx_http_file_cache_manager() 函数这时Nginx在调用ngx_http_file_cache_set_slot函数解析配置指令proxy_cache_path时设置的回调值


```cpp
static time_t
ngx_http_file_cache_manager(void *data)
{
    ngx_http_file_cache_t  *cache = data;

    off_t       size;
    time_t      next, wait;
    ngx_uint_t  count, watermark;
    //如果有超时的cache，就删除对应的cache
    next = ngx_http_file_cache_expire(cache);
    //最后访问的时间
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
       // 如果没有发现超过最大的大小限制，则直接返回
        if (size < cache->max_size && count < watermark) {
            return next;
        }
     //否则遍历所有的cache文件，然后删除过期的cache.
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

> 
上面的函数中主要调用了两个函数，一个是ngx_http_file_cache_expire,一个是ngx_http_file_cache_forced_expire主要区别：前一个只有过期的cache才会去尝试删除它(在满足引用计数为0),而后一个不管有没有过期，只要引用计数为0，就会去清理。来详细看这两个函数的实现。


### ngx_http_file_cache_expire函数

> 
首先是ngx_http_file_cache_expire，这里注意nginx使用了LRU，也就是队列最尾端保存的是最长时间没有被使用的,并且这个函数返回的就是一个wait值


```cpp
static time_t
ngx_http_file_cache_expire(ngx_http_file_cache_t *cache)
{
    u_char                      *name, *p;
    size_t                       len;
    time_t                       now, wait;
    ngx_path_t                  *path;
    ngx_queue_t                 *q;
    ngx_http_file_cache_node_t  *fcn;
    u_char                       key[2 * NGX_HTTP_CACHE_KEY_LEN];

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache expire");

    path = cache->path;
    len = path->name.len + 1 + path->len + 2 * NGX_HTTP_CACHE_KEY_LEN;

    name = ngx_alloc(len + 1, ngx_cycle->log);
    if (name == NULL) {
        return 10;
    }

    ngx_memcpy(name, path->name.data, path->name.len);

    now = ngx_time();

    ngx_shmtx_lock(&cache->shpool->mutex);

    for ( ;; ) {

        if (ngx_quit || ngx_terminate) {
            wait = 1;
            break;
        }
       //如果cache队列为空，则直接退出返回
        if (ngx_queue_empty(&cache->sh->queue)) {
            wait = 10;
            break;
        }
      //从最后一个开始
        q = ngx_queue_last(&cache->sh->queue);

        fcn = ngx_queue_data(q, ngx_http_file_cache_node_t, queue);

        wait = fcn->expire - now;
        //如果没有超时，则退出循环
        if (wait > 0) {
            wait = wait > 10 ? 10 : wait;
            break;
        }

        ngx_log_debug6(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                       "http file cache expire: #%d %d %02xd%02xd%02xd%02xd",
                       fcn->count, fcn->exists,
                       fcn->key[0], fcn->key[1], fcn->key[2], fcn->key[3]);
        //如果引用计数为0，则删除这个cache节点
        if (fcn->count == 0) {
            ngx_http_file_cache_delete(cache, q, name);
            continue;
        }
      //如果当前节点正在删除，则退出循环
        if (fcn->deleting) {
            wait = 1;
            break;
        }

        p = ngx_hex_dump(key, (u_char *) &fcn->node.key,
                         sizeof(ngx_rbtree_key_t));
        len = NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t);
        (void) ngx_hex_dump(p, fcn->key, len);

        /*
         * abnormally exited workers may leave locked cache entries,
         * and although it may be safe to remove them completely,
         * we prefer to just move them to the top of the inactive queue
         */
        //将当前节点放入队列最前端
        ngx_queue_remove(q);
        fcn->expire = ngx_time() + cache->inactive;
        ngx_queue_insert_head(&cache->sh->queue, &fcn->queue);

        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "ignore long locked inactive cache entry %*s, count:%d",
                      (size_t) 2 * NGX_HTTP_CACHE_KEY_LEN, key, fcn->count);
    }

    ngx_shmtx_unlock(&cache->shpool->mutex);

    ngx_free(name);

    return wait;
}
```

### ngx_http_file_cache_forced_expire函数

ngx_http_file_cache_forced_expire，就是强制删除cache 节点，它的返回值也是wait time。它的遍历也是从后到前的。

```cpp
static time_t
ngx_http_file_cache_forced_expire(ngx_http_file_cache_t *cache)
{
    u_char                      *name;
    size_t                       len;
    time_t                       wait;
    ngx_uint_t                   tries;
    ngx_path_t                  *path;
    ngx_queue_t                 *q;
    ngx_http_file_cache_node_t  *fcn;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache forced expire");

    path = cache->path;
    len = path->name.len + 1 + path->len + 2 * NGX_HTTP_CACHE_KEY_LEN;

    name = ngx_alloc(len + 1, ngx_cycle->log);
    if (name == NULL) {
        return 10;
    }

    ngx_memcpy(name, path->name.data, path->name.len);

    wait = 10;
    //删除节点尝试次数
    tries = 20;

    ngx_shmtx_lock(&cache->shpool->mutex);
    //遍历队列
    for (q = ngx_queue_last(&cache->sh->queue);
         q != ngx_queue_sentinel(&cache->sh->queue);
         q = ngx_queue_prev(q))
    {
        fcn = ngx_queue_data(q, ngx_http_file_cache_node_t, queue);

        ngx_log_debug6(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                  "http file cache forced expire: #%d %d %02xd%02xd%02xd%02xd",
                  fcn->count, fcn->exists,
                  fcn->key[0], fcn->key[1], fcn->key[2], fcn->key[3]);
      //如果引用计数为0则删除cache 
        if (fcn->count == 0) {
            ngx_http_file_cache_delete(cache, q, name);
            wait = 0;

        } else {
        //否则尝试20次
            if (--tries) {
                continue;
            }

            wait = 1;
        }

        break;
    }

    ngx_shmtx_unlock(&cache->shpool->mutex);

    ngx_free(name);

    return wait;
}
```

> 
还有一个函数就是ngx_http_file_cache_delete，这个函数这里就不分析了，它主要有2个功能，一个是删除cache文件，一个是删除cache管理节点。





