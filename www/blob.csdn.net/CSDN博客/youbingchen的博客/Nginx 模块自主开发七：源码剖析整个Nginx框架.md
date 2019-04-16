# Nginx 模块自主开发七：源码剖析整个Nginx框架 - youbingchen的博客 - CSDN博客





2016年06月30日 11:40:51[youbingchen](https://me.csdn.net/youbingchen)阅读数：11241








在博客提到的[Nginx 模块自主开发四: 模块数据结构](http://blog.csdn.net/youbingchen/article/details/51751107)一个很重要的数据结构ngx_module_s,所有的模块都是用这个结构体来表示。

# ngx_module_t结构体

> 
模块又分为ngx_core_module_t和ngx_xxx_module_t,而在ngx_module_t中会包含这两个结构，只不过不同类的模块包含不同的结构,一般会 用ctx表示，ngx_module_t又用type这个域用来表示在模块的类型


```cpp
struct ngx_module_s {
   /*ctx_index表示当前模块在这类模块中的序号。这个 成员常常是由管理模块的 一个Nginx核心模块设置的，对于所有的http模块而言，ctx_index是由核心模块ngx_http_module设置的，ctx_index非常重要，Nginx的模块化设计非常依赖各个模块的顺序*/
    ngx_uint_t            ctx_index;
    /*index表示当前模块在ngx_modules数组中 的序号。*/
    ngx_uint_t            index; 

    char                 *name;
    // sqare系列的 保留变量，暂未使用
    ngx_uint_t            spare0;
    ngx_uint_t            spare1;
    // 模块的版本
    ngx_uint_t            version;
    const char           *signature;
   /**ctx用于指向一类的模块的上下文结构体，Nginx模块有许多种类，不同模块之间的功能差别很大。例如事件模块主要处理I/O事件相关的功能，每个模块都有自己的特性，ctx将会指向特定类型模块的公共接口，例如在Http 模块 中，ctx指向ngx_http_module_t结构体*/
    void                 *ctx;
    // commands将处理nginx.conf中 的配置项
    ngx_command_t        *commands;
    /*type 表示该模块的类型，它与ctx指针是紧密相关的，NGX_HTTP_MODULE、NGX_CORE_MODULE、NGX_CONF_MODULE、NGX_EVENT_MODULE、NGX_EVENT_MODULE、NGX_STREAM_MODULE*/
    ngx_uint_t            type;
    /*在Nginx 启动、停止过程中，以下7个函数指针表示有7个执行点分别调用这个7个方法，对于任一方法，如果不需要Nginx在某个时刻执行 它，那么 简单地把它设为NULL空指针 即可*/
    /* 虽然字面上理解应当在master进程启动时调用，但到目前为止，框架代码从来不会调用它，因此 要设为NULL*/
    ngx_int_t           (*init_master)(ngx_log_t *log);
    /*init module 回调方法在初始化所有模块被调用，在master/worker模式下，这个阶段将在worker进程前完成 */
    ngx_int_t           (*init_module)(ngx_cycle_t *cycle);
   /*init_process回调方法在正常服务前被调用，在master/worker模式下,多个worker子进程已经产生，在每个worker进程初始化会调用所有模块的init_process函数*/
    ngx_int_t           (*init_process)(ngx_cycle_t *cycle);
    // Nginx目前还支持多线程，所以这个没有被调用过
    ngx_int_t           (*init_thread)(ngx_cycle_t *cycle);
    void                (*exit_thread)(ngx_cycle_t *cycle);
   /* exit_process回调方法在服务停止前调用。在master/worker模式下，worker会在退出前调用它*/
    void                (*exit_process)(ngx_cycle_t *cycle);
    //该方法 会在master进程退出前调用它
    void                (*exit_master)(ngx_cycle_t *cycle);
   //目前还没用
    uintptr_t             spare_hook0;
    uintptr_t             spare_hook1;
    uintptr_t             spare_hook2;
    uintptr_t             spare_hook3;
    uintptr_t             spare_hook4;
    uintptr_t             spare_hook5;
    uintptr_t             spare_hook6;
    uintptr_t             spare_hook7;
};
```

> 
这里看到有两个index，分别是ctx_index和index，他们的区别是这样子的（用http模块举例），ctx_index保存了每一个http module的config的索引，而所有的http module config是分别保存在nginx_conf_t的ctx数组中的.而index保存了每一个core module的config的索引，而每个core module的config都是保存在cycle的conf_ctx中的，下面的代码轻易看出


```cpp
#define ngx_http_conf_get_module_main_conf(cf, module)                        
    ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[module.ctx_index]

#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]
```

> 
ngx_core_module_t都包括(log, event, event_openssl, http, mail,google perftools)，可以看到http module本身也是一个core module。这里要注意还有一个conf module，只不过它也是用core module这个数据结构。


```cpp
typedef struct {
    ngx_str_t             name;
    void               *(*create_conf)(ngx_cycle_t *cycle);
    char               *(*init_conf)(ngx_cycle_t *cycle, void *conf);
} ngx_core_module_t;
```

> 
ngx_http_module_t包括所有src/http/下面的模块，它就包含了所有的http module，它们都从属于http core模块。


```cpp
typedef struct {
    ngx_int_t   (*preconfiguration)(ngx_conf_t *cf);
    ngx_int_t   (*postconfiguration)(ngx_conf_t *cf);

    void       *(*create_main_conf)(ngx_conf_t *cf);
    char       *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    void       *(*create_srv_conf)(ngx_conf_t *cf);
    char       *(*merge_srv_conf)(ngx_conf_t *cf, void *prev, void *conf);

    void       *(*create_loc_conf)(ngx_conf_t *cf);
    char       *(*merge_loc_conf)(ngx_conf_t *cf, void *prev, void *conf);
} ngx_http_module_t;
```

> 
还有一个ngx_module_s还有一个很重要的成员就是ngx_command_t。这个域包含当前 模块 所有包含的指令


> 
解析完上面的数据结构，下面将从整体剖析整个Nginx框架的走向，前面的一篇博客[Nginx模块自主开发六：源码剖析配置文件解析过程](http://blog.csdn.net/youbingchen/article/details/51782491)详细介绍了ngx_init_cycle的流程，这篇博客将主要从ngx_init_cycle如何串通所有模块 的流程说起 。（有些部分会重复，但是为了理清整个Nginx框架的流程还是值得）


### Nginx在ngx_init_cycle中通过下列代码创建所有的   configure，它 通过调用每个core module 的Create_conf的方法来创建对应的conf，每个对象都保存在全局的conf__ctx中。

```cpp
/*
创建这些数据结构的工作都需要在这一步进行。Nginx框架只关心NGX_CORE_MODULE核
心模块，这也是为了降低框架的复杂度。这里将会调用所有核心模块的create conf方法（也只有核心模块才有这个方法），这意味着需要所有的核心
模块开始构造用于存储配置项的结构体。其他非核心模块怎么办呢？其实很简单。这些模块大都从属于一个核心模块，如每个HTTP模块都由ngx_http_module
管理（如图8-2所示），这样ngx_http_module在解析自己感兴趣的“http”配置项时，将会调用所有HTTP模块约定的方法来创建存储配置项的结构体
（xxx_create_main_conf、xxx_create_srv_conf、xxx_create_loc_conf方法）。
*/
    for (i = 0; cycle->modules[i]; i++) {
        if (cycle->modules[i]->type != NGX_CORE_MODULE) {
            continue;
        }
    //得到core module
        module = cycle->modules[i]->ctx;
     /如果create_conf存在，则直接创建config
        if (module->create_conf) {
            rv = module->create_conf(cycle);
            if (rv == NULL) {
                ngx_destroy_pool(pool);
                return NULL;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;  
        }
    }
       .......
    conf.ctx = cycle->conf_ctx;  //这样下面的ngx_conf_param解析配置的时候，里面对conf.ctx赋值操作，实际上就是对cycle->conf_ctx[i]
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;
```

> 
当所有的core module的config都创建完毕后，就要开始解析配置文件了，解析配置文件它会一行行读取，然后如果遇到指令，则会查找到对应的ngx_command_t对象，然后执行对应的回调set方法。这里所有动作都在ngx_conf_parse这个函数中进行。然后在ngx_conf_parse会判断cf是否有handler回调，如果有的话，优先调用handler回调，如果没有，则会进入ngx_conf_handler进行一般处理。 
**一般来说，如果没有自定义handler ，解析文件的过程`ngx_init_cycle()->ngx_conf_parse()->ngx_conf_handler()`解析** ，最后通过cmd->set方法进行回调。这时就进入我们ngx_command_t设置的 回调函数 。比如,ngx_http.c中 的ngx_http_block函数。


```cpp
// 文件中的ngx_http.c的ngx_http_bock函数
// 这个函数在分配 ctx的空间之后，同时给main，serv和conf分配空间之后,初始化每个空间的配置
for (m = 0; cf->cycle->modules[m]; m++) {
        if (cf->cycle->modules[m]->type != NGX_HTTP_MODULE) {
            continue;
        }

        module = cf->cycle->modules[m]->ctx; // 注意module的赋值情况是ngx_module_t的成员ctx
        mi = cf->cycle->modules[m]->ctx_index; //得到对应的索引
     //如果有对应的回调，则调用回调函数，然后将返回的模块config设置到ctx的对应的conf列表中
        if (module->create_main_conf) {  
            ctx->main_conf[mi] = module->create_main_conf(cf);
            if (ctx->main_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }

        if (module->create_srv_conf) { // 
            ctx->srv_conf[mi] = module->create_srv_conf(cf);
            if (ctx->srv_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }

        if (module->create_loc_conf) { //
            ctx->loc_conf[mi] = module->create_loc_conf(cf);
            if (ctx->loc_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }
    }
    ....
    // 递归解析
    rv = ngx_conf_parse(cf, NULL);
```

> 
http module相关的config是在ngx_http_block中创建的，在ngx_http_block中会创建，初始化，合并config，以及整个http handler phase的初始化等等. 

  先是初始化所有的http module的ctx_index. 

  当http block完全parse完毕之后，就需要merge(main和srv或者srv和loc)相关的config了。不过在每次merge之前都会首先初始化main conf。
```cpp
cmcf = ctx->main_conf[ngx_http_core_module.ctx_index];
    cscfp = cmcf->servers.elts;

    for (m = 0; cf->cycle->modules[m]; m++) {
        if (cf->cycle->modules[m]->type != NGX_HTTP_MODULE) {
            continue;
        }

        module = cf->cycle->modules[m]->ctx;
        mi = cf->cycle->modules[m]->ctx_index;

        /* init http{} main_conf's */

        if (module->init_main_conf) {
            rv = module->init_main_conf(cf, ctx->main_conf[mi]);
            if (rv != NGX_CONF_OK) {
                goto failed;
            }
        }
     //然后开始merge config。
     /* 每个级别都会保存对应的ctx(main/ser/loc)，怎么说呢，就是在解析HTTP main中创建了3个ctx(main/srv/loc),而在HTTP srv block中将会创建2个ctx(srv/loc),这时发生重复了，那就需要merge了。比如一个命令(srv_offset)在HTTP main中有一个，那么Nginx将会把它放入到HTTP main的ctx的srv ctx中，然后server block也有一个，那么Nginx会继续把它放到Server ctx的 srv_conf中，最后merge他们。*/
        rv = ngx_http_merge_servers(cf, cmcf, module, mi);//合并server{}及其以下的local{}
        if (rv != NGX_CONF_OK) {
            goto failed;
        }
```

> 
当merge完毕之后，然后就是location的处理，初始化location tree，创建handler phase，调用postconfiguration，以及变量的初始化。


```cpp
/*
    经过配置的读取之后，所有server都被保存在http core模块的main配置中的servers数组中，而每个server里面的location都被按配置中
    出现的顺序保存在http core模块的loc配置的locations队列中，上面的代码中先对每个server的location进行排序和分类处理，这一步
    发生在 ngx_http_init_location()函数中：
    */
      /*location处理比较复杂*/
       if (ngx_http_init_locations(cf, cscfp[s], clcf) != NGX_OK) {
            return NGX_CONF_ERROR;
        }
/*    根据已经按照location字符串排序过的双向链表，快速地构建静态的二叉查找树。与ngx_http_init_locations方法类似，速个操作也是递归进行的
          */
        /*
        下面的ngx_http_init_static_location_trees函数就会将那些普通的location(就是ngx_http_init_locations中name noname regex以外的location(exact/inclusive))，
        即staticlocation，进行树化(一种三叉树)处理，之所以要做这样的处理，是为了在处理http请求时能高效的搜索的匹配的location配置。
        */

        if (ngx_http_init_static_location_trees(cf, clcf) != NGX_OK) {
            return NGX_CONF_ERROR;
        }
    }

  //初始化handler phase
    if (ngx_http_init_phases(cf, cmcf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }
   for (m = 0; ngx_modules[m]; m++) {
        if (ngx_modules[m]->type != NGX_HTTP_MODULE) {
            continue;
        }

        module = ngx_modules[m]->ctx;
//调用回调
        if (module->postconfiguration) {
            if (module->postconfiguration(cf) != NGX_OK) {
                return NGX_CONF_ERROR;
            }
        }
    }
//开始初始化变量
    if (ngx_http_variables_init_vars(cf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }
```

> 
做完上面的工作之后，下面进行初始化socket的相关东西，比如设置读写回调 函数等


```cpp
if (ngx_http_init_phase_handlers(cf, cmcf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }


    /* optimize the lists of ports, addresses and server names */

    if (ngx_http_optimize_servers(cf, cmcf, cmcf->ports) != NGX_OK) {
        return NGX_CONF_ERROR;
    }
```

> 
ngx_cycle_init剩下的部分，当配置文件解析完毕后，就开始初始化core module的config


```cpp
for (i = 0; cycle->modules[i]; i++) {
        if (cycle->modules[i]->type != NGX_CORE_MODULE) {
            continue;
        }

        module = cycle->modules[i]->ctx;

        if (module->init_conf) {
            if (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->index])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                return NULL;
            }
        }
    }
```

> 
接下来就是创建并初始化所有创建的共享内存


```cpp
if (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            goto failed;
        }

        if (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            goto failed;
        }

        if (shm_zone[i].init(&shm_zone[i], NULL) != NGX_OK) {
            goto failed;
        }
```

> 
接下来就是开启监听，初始化所有的模块


```cpp
.....
if (ngx_open_listening_sockets(cycle) != NGX_OK) {
    goto failed;
}
....
  // 初始化所有的模块
    if (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        exit(1);
    }
```

> 
到时为止ngx_init_cycle()函数主要的工作都已经完成了。


# 总结 

要搞清Nginx模块和框架之间的去协作不仅要清楚ngx_command_t结构体、ngx_module_t结构体，ngx_conf_t结构以及ngx_cycle_t结构体的关系，还要学会使用回调方法来分析整个流程 。上面见到ngx_http_block函数中的location的初始化这里 并没有仔细展开，因为mail 和stream中并没有这一步，等以后有涉及到再 详细了解。





