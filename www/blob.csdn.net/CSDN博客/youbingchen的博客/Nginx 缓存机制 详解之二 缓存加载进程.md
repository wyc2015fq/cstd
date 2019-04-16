# Nginx 缓存机制 详解之二 缓存加载进程 - youbingchen的博客 - CSDN博客





2016年06月24日 19:05:48[youbingchen](https://me.csdn.net/youbingchen)阅读数：1392








上一篇博客已经详细讲解了[缓存管理进程](http://blog.csdn.net/youbingchen/article/details/51754986#t0),接下来详细讲解缓存加载进程机制

# 缓存加载进程机制

### ngx_cache_loader_process_handler函数

> 
加载进程 和管理进程都是从函数ngx_cache_manager_process_cycle进入，只是接下来的流程函数不一样


```cpp
ngx_process_events_and_timers()->ngx_event_expire_timers()->ngx_cache_loader_process_handler()
```

```cpp
static ngx_cache_manager_ctx_t  ngx_cache_loader_ctx = {
    ngx_cache_loader_process_handler, "cache loader process", 60000
};
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
            path[i]->loader(path[i]->data); //这里的 loader调用的 是ngx_http_file_cache_loader()
            ngx_time_update();
        }
    }

    exit(0);
```

### ngx_http_file_cache_loader函数

```cpp
static void
ngx_http_file_cache_loader(void *data)
{
    ngx_http_file_cache_t  *cache = data;

    ngx_tree_ctx_t  tree;

    if (!cache->sh->cold || cache->sh->loading) {
        return;
    }

    if (!ngx_atomic_cmp_set(&cache->sh->loading, 0, ngx_pid)) {
        return;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache loader");
   //设置回调
    tree.init_handler = NULL;
    tree.file_handler = ngx_http_file_cache_manage_file; // 对于每个cache文件调用这个回调
    tree.pre_tree_handler = ngx_http_file_cache_manage_directory;//主要用来遍历所有cache的 目录 
    tree.post_tree_handler = ngx_http_file_cache_noop;
    tree.spec_handler = ngx_http_file_cache_delete_file;
    //回调数据就是cache
    tree.data = cache;
    tree.alloc = 0;
    tree.log = ngx_cycle->log;
   //last为最后load时间
    cache->last = ngx_current_msec;
    cache->files = 0;
   //开始遍历
    if (ngx_walk_tree(&tree, &cache->path->name) == NGX_ABORT) {
        cache->sh->loading = 0;
        return;
    }
```

上面有一个ngx_tree_ctx_t数据结构

```cpp
struct ngx_tree_ctx_s {
    off_t                      size;
    off_t                      fs_size;
    ngx_uint_t                 access;
    time_t                     mtime;

    ngx_tree_init_handler_pt   init_handler;
    ngx_tree_handler_pt        file_handler;
    ngx_tree_handler_pt        pre_tree_handler;
    ngx_tree_handler_pt        post_tree_handler;
    ngx_tree_handler_pt        spec_handler;

    void                      *data;
    size_t                     alloc;

    ngx_log_t                 *log;
};
/**ctx->init_handler() – see ctx->alloc
* ctx->file_handler() – file handler
* ctx->pre_tree_handler() – handler is called before entering directory
* ctx->post_tree_handler() – handler is called after leaving directory
* ctx->spec_handler() – special (socket, FIFO, etc.) file handler
*
* ctx->data – some data structure, it may be the same on all levels, or
* reallocated if ctx->alloc is nonzero
*
* ctx->alloc – a size of data structure that is allocated at every level
* and is initilialized by ctx->init_handler()
*
* ctx->log – a log*/
```

### ngx_http_file_cache_manage_file函数

```cpp
static ngx_int_t
ngx_http_file_cache_manage_file(ngx_tree_ctx_t *ctx, ngx_str_t *path)
{
    ngx_msec_t              elapsed;
    ngx_http_file_cache_t  *cache;

    cache = ctx->data;
//将文件添加进cache
    if (ngx_http_file_cache_add_file(ctx, path) != NGX_OK) {
        (void) ngx_http_file_cache_delete_file(ctx, path);
    }
//如果文件个数太大，则休眠并清理files计数
    if (++cache->files >= cache->loader_files) {
        ngx_http_file_cache_loader_sleep(cache);

    } else {
        ngx_time_update();
//否则看loader时间是不是过长，如果过长则又进入休眠
        elapsed = ngx_abs((ngx_msec_int_t) (ngx_current_msec - cache->last));

        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                       "http file cache loader time elapsed: %M", elapsed);

        if (elapsed >= cache->loader_threshold) {
            ngx_http_file_cache_loader_sleep(cache);
        }
    }

    return (ngx_quit || ngx_terminate) ? NGX_ABORT : NGX_OK;
}
```

> 
分别是ngx_http_file_cache_add_file和ngx_http_file_cache_loader_sleep。其中sleep函数比较简单，就是休眠,并重置对应的域。


### ngx_http_file_cache_loader_sleep函数

```cpp
static void
ngx_http_file_cache_loader_sleep(ngx_http_file_cache_t *cache)
{
    ngx_msleep(cache->loader_sleep);

    ngx_time_update();

    cache->last = ngx_current_msec;
    cache->files = 0;
}
```

### ngx_http_file_cache_add_file函数

> 
ngx_http_file_cache_add_file,它主要是通过文件名计算hash，然后调用ngx_http_file_cache_add将这个文件加入到cache管理中(也就是添加红黑树以及队列)


```cpp
static ngx_int_t
ngx_http_file_cache_add(ngx_http_file_cache_t *cache, ngx_http_cache_t *c)
{
    ngx_http_file_cache_node_t  *fcn;

    ngx_shmtx_lock(&cache->shpool->mutex);
//首先查找
    fcn = ngx_http_file_cache_lookup(cache, c->key);

    if (fcn == NULL) {
//如果不存在，则新建结构
        fcn = ngx_slab_alloc_locked(cache->shpool,
                                    sizeof(ngx_http_file_cache_node_t));
        if (fcn == NULL) {
            ngx_shmtx_unlock(&cache->shpool->mutex);
            return NGX_ERROR;
        }

        ngx_memcpy((u_char *) &fcn->node.key, c->key, sizeof(ngx_rbtree_key_t));

        ngx_memcpy(fcn->key, &c->key[sizeof(ngx_rbtree_key_t)],
                   NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t));
//插入红黑树
        ngx_rbtree_insert(&cache->sh->rbtree, &fcn->node);

        fcn->uses = 1;
        fcn->count = 0;
        fcn->valid_msec = 0;
        fcn->error = 0;
        fcn->exists = 1;
        fcn->updating = 0;
        fcn->deleting = 0;
        fcn->uniq = 0;
        fcn->valid_sec = 0;
        fcn->body_start = 0;
        fcn->fs_size = c->fs_size;

        cache->sh->size += c->fs_size;

    } else {
//否则删除queue，后续会重新插入
        ngx_queue_remove(&fcn->queue);
    }

    fcn->expire = ngx_time() + cache->inactive;
//重新插入
    ngx_queue_insert_head(&cache->sh->queue, &fcn->queue);

    ngx_shmtx_unlock(&cache->shpool->mutex);

    return NGX_OK;
}
```



