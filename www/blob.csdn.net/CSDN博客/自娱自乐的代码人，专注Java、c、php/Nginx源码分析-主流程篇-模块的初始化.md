# Nginx源码分析 - 主流程篇 - 模块的初始化 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年07月13日 17:03:10[initphp](https://me.csdn.net/initphp)阅读数：5106
个人分类：[Nginx源码分析](https://blog.csdn.net/initphp/article/category/6081681)

所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









Nginx是高度模块化的，各个功能都会封装在模块中。例如core模块、HTTP模块等。也可以自定义模块。

这一篇文章主要讲解模块的初始化。后续会有一篇文章教你如何编写Nginx的模块。

Nginx实现模块管理的代码主要在：**/src/core/ngx_module.c**文件中。

### 模块的数据结构

#### ngx_module_s 模块结构

结构体ngx_module_s主要用于管理每一个模块的详细信息。

Nginx的所有模块会放置在全局变量cycle的**cycle->modules** 模块数组。通过这个数组，我们就可以拿到每个模块的具体信息。




```cpp
/**
 * 业务模块数据结构
 */
struct ngx_module_s {
    ngx_uint_t            ctx_index;
    ngx_uint_t            index; /* 模块的唯一标识符号 */

    char                 *name;  /* 模块名称 */

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            version;	/* 模块版本 */
    const char           *signature;

    void                 *ctx;	/* 模块上下文 */
    ngx_command_t        *commands; /* 模块支持的命令集 */
    ngx_uint_t            type;	/* 模块类型 */

    /* 回调函数 */
    ngx_int_t           (*init_master)(ngx_log_t *log); /* 主进程初始化的时候调用 */

    ngx_int_t           (*init_module)(ngx_cycle_t *cycle); /* 模块初始化的时候调用 */

    ngx_int_t           (*init_process)(ngx_cycle_t *cycle);  /* 工作进程初始化时调用*/
    ngx_int_t           (*init_thread)(ngx_cycle_t *cycle); /* 线程初始化调用*/
    void                (*exit_thread)(ngx_cycle_t *cycle);	/* 线程退出调用*/
    void                (*exit_process)(ngx_cycle_t *cycle); /* 工作进程退出调用*/

    void                (*exit_master)(ngx_cycle_t *cycle);

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

说明：

1. index 主要用于模块的标识。

cycle->conf_ctx主要存储的是各个模块的配置文件结构的指针地址。


cycle->conf_ctx中获取各个模块配置信息都是通过模块的标识来确定数组位置的。


例如核心模块是放在一个ngx_core_conf_t的数据结构上的。而ngx_core_conf_t这个指针就通过index索引值放在cycle->conf_ctx数组中。

获取核心模块的配置信息：



```cpp
ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);
```
2. commands主要用于配置模块的命令集。Nginx的配置文件都是通过commands命令集来逐个解析具体定义好的配置信息（每个模块不一样）。下一章会详解解读


3. ctx 模块上下文。主要放置一个模块自定义的结构。例如核心模块就是ngx_core_module_t的结构。ngx_core_module_t中可以自定义一些方法或者参数。

4. type。模块类型。

5. init_module：初始化模块的时候会回调的函数。





#### ngx_core_module 核心模块

1. 核心模块在nginx.c的头部就定义了。定义了** ngx_core_module核心模块**和**核心模块的上下文ngx_core_module_ctx**

2. ngx_core_module_t 为核心模块的**上下文结构**。主要用于核心模块的配置文件创建ngx_core_module_create_conf和初始化ngx_core_module_init_conf。放置在**ngx_module_s->ctx**。




```cpp
/**
 * 核心模块core数据结构
 * ngx_module_s->ctx 核心模块的上下文，主要定义了创建配置和初始化配置的结构
 */
typedef struct {
    ngx_str_t             name;
    void               *(*create_conf)(ngx_cycle_t *cycle);
    char               *(*init_conf)(ngx_cycle_t *cycle, void *conf);
} ngx_core_module_t;
```



```cpp
/**
 * 核心模块配置文件
 * ngx_core_module_create_conf 核心模块创建配置文件
 * ngx_core_module_init_conf 核心模块初始化配置文件
 */
static ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};

/**
 * 核心模块
 */
ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};
```





### 模块的初始化

#### 1. 模块编号处理ngx_preinit_modules

在nginx.c的main函数中，第一步对模块的处理就是进行编号处理。仅仅是编号，不做任何其他处理。

nginx.c：




```cpp
/* 初始化所有模块；并对所有模块进行编号处理；
     * ngx_modules数却是在自动编译的时候生成的，位于objs/ngx_modules.c文件中   */
    if (ngx_preinit_modules() != NGX_OK) {
        return 1;
    }
```
ngx_module.c 具体函数实现




```cpp
/**
 * 初始化所有模块；并对所有模块进行编号处理；
 */
ngx_int_t
ngx_preinit_modules(void)
{
    ngx_uint_t  i;

    for (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    return NGX_OK;
}
```

我们可以看到，模块的个数是通过**ngx_modules的数组**得到的。


ngx_modules是一个引用外部的变量。在ngx_modules.h中



```cpp
/* 模块数组，所有的模块都会保存在此数组中   共有四种类型模块："CORE","CONF","EVNT","HTTP" */
extern ngx_module_t  *ngx_modules[];
```



而ngx_modules的模块到底是什么时候确定的呢？

1. 具体的模块可通过编译前的configure命令进行配置，即设置哪些模块需要编译，哪些不被编译。当编译的时候，会生成ngx_modules.c的文件，里面就包含模块数组。

2. 新增模块或者减少模块可以在configure命令执行前 **auto/modules文件**里面修改。


生成的objs/ngx_modules.c文件如下：



```cpp
00001:
00002: #include <ngx_config.h>
00003: #include <ngx_core.h>
00004:
00005:
00006:
00007: extern ngx_module_t ngx_core_module;
00008: extern ngx_module_t ngx_errlog_module;
00009: extern ngx_module_t ngx_conf_module;
00010: extern ngx_module_t ngx_events_module;
00011: extern ngx_module_t ngx_event_core_module;
00012: extern ngx_module_t ngx_epoll_module;
00013: extern ngx_module_t ngx_http_module;
00014: extern ngx_module_t ngx_http_core_module;
00015: extern ngx_module_t ngx_http_log_module;
00016: extern ngx_module_t ngx_http_upstream_module;
00017: extern ngx_module_t ngx_http_static_module;
00018: extern ngx_module_t ngx_http_autoindex_module;
00019: extern ngx_module_t ngx_http_index_module;
00020: extern ngx_module_t ngx_http_auth_basic_module;
00021: extern ngx_module_t ngx_http_access_module;
00022: extern ngx_module_t ngx_http_limit_zone_module;
00023: extern ngx_module_t ngx_http_limit_req_module;
00024: extern ngx_module_t ngx_http_geo_module;
00025: extern ngx_module_t ngx_http_map_module;
00026: extern ngx_module_t ngx_http_split_clients_module;
00027: extern ngx_module_t ngx_http_referer_module;
00028: extern ngx_module_t ngx_http_rewrite_module;
00029: extern ngx_module_t ngx_http_proxy_module;
00030: extern ngx_module_t ngx_http_fastcgi_module;
00031: extern ngx_module_t ngx_http_uwsgi_module;
00032: extern ngx_module_t ngx_http_scgi_module;
00033: extern ngx_module_t ngx_http_memcached_module;
00034: extern ngx_module_t ngx_http_empty_gif_module;
00035: extern ngx_module_t ngx_http_browser_module;
00036: extern ngx_module_t ngx_http_upstream_ip_hash_module;
00037: extern ngx_module_t ngx_http_stub_status_module;
00038: extern ngx_module_t ngx_http_write_filter_module;
00039: extern ngx_module_t ngx_http_header_filter_module;
00040: extern ngx_module_t ngx_http_chunked_filter_module;
00041: extern ngx_module_t ngx_http_range_header_filter_module;
00042: extern ngx_module_t ngx_http_gzip_filter_module;
00043: extern ngx_module_t ngx_http_postpone_filter_module;
00044: extern ngx_module_t ngx_http_ssi_filter_module;
00045: extern ngx_module_t ngx_http_charset_filter_module;
00046: extern ngx_module_t ngx_http_userid_filter_module;
00047: extern ngx_module_t ngx_http_headers_filter_module;
00048: extern ngx_module_t ngx_http_copy_filter_module;
00049: extern ngx_module_t ngx_http_range_body_filter_module;
00050: extern ngx_module_t ngx_http_not_modified_filter_module;
00051:
```


#### 2. 初始化cycle->modules 分配内存

主要在cycle->modules上分配一块用于存放ngx_module_s数据结构的列表内存。并且将原来的 ngx_modules拷贝到cycle->modules上。

ngx_cycle.c




```cpp
/* 创建模块以及创建模块的配置信息 */
    if (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        return NULL;
    }
```
ngx_modules.c




```cpp
/**
 * ngx_init_cycle 在初始化cycle的时候，初始化模块
 * 创建一个列表，并将静态的模块拷贝到列表上
 */
ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * sizeof(ngx_module_t *));
    if (cycle->modules == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * sizeof(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    return NGX_OK;
}
```



#### 3. 每个模块进行初始化ngx_init_modules

1. ngx_module_s结构中定义了init_module的模块初始化回调函数。ngx_init_modules主要用于每个模块的初始化工作。

2. 在编写自定义模块的时候，可以定义init_module方法，主要用于这个模块的初始化工作。


ngx_cycle.c



```cpp
/* 调用每个模块的初始化函数 */
    if (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        exit(1);
    }
```

真正的各个模块初始化 ngx_modules.c：




```cpp
/**
 * 对每个模块进行一次初始化操作
 * 调用 init_module 回调函数，初始化每个模块的数据
 */
ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    for (i = 0; cycle->modules[i]; i++) {
        if (cycle->modules[i]->init_module) {
            if (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                return NGX_ERROR;
            }
        }
    }

    return NGX_OK;
}
```



#### 4. 统计有多少个模块ngx_count_modules



```cpp
/**
 * 统计每个类型下面总共多少个模块
 */
ngx_int_t
ngx_count_modules(ngx_cycle_t *cycle, ngx_uint_t type)
{
    ngx_uint_t     i, next, max;
    ngx_module_t  *module;

    next = 0;
    max = 0;

    /* count appropriate modules, set up their indices */

    for (i = 0; cycle->modules[i]; i++) {
        module = cycle->modules[i];

        if (module->type != type) {
            continue;
        }

        if (module->ctx_index != NGX_MODULE_UNSET_INDEX) {

            /* if ctx_index was assigned, preserve it */

            if (module->ctx_index > max) {
                max = module->ctx_index;
            }

            if (module->ctx_index == next) {
                next++;
            }

            continue;
        }

        /* search for some free index */

        module->ctx_index = ngx_module_ctx_index(cycle, type, next);

        if (module->ctx_index > max) {
            max = module->ctx_index;
        }

        next = module->ctx_index + 1;
    }

    /*
     * make sure the number returned is big enough for previous
     * cycle as well, else there will be problems if the number
     * will be stored in a global variable (as it's used to be)
     * and we'll have to roll back to the previous cycle
     */

    if (cycle->old_cycle && cycle->old_cycle->modules) {

        for (i = 0; cycle->old_cycle->modules[i]; i++) {
            module = cycle->old_cycle->modules[i];

            if (module->type != type) {
                continue;
            }

            if (module->ctx_index > max) {
                max = module->ctx_index;
            }
        }
    }

    /* prevent loading of additional modules */

    cycle->modules_used = 1;

    return max + 1;
}
```




#### 5. 模块的工作进程初始化init_process

在nginx_process_cycle.c文件的ngx_worker_process_init方法（Nginx是多进程模式的，此方法是每个进程进行自己的初始化工作）中包含模块的进程初始化。

模块进程初始化，**主要回调模块数据结构中的init_process回调函数。**

此方法，会在后面的章节中详细讲解。



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
























