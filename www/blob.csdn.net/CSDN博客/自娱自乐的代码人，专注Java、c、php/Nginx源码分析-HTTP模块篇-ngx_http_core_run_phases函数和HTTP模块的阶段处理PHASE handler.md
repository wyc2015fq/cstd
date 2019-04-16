# Nginx源码分析 - HTTP模块篇 - ngx_http_core_run_phases函数和HTTP模块的阶段处理PHASE handler - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2017年06月07日 14:49:26[initphp](https://me.csdn.net/initphp)阅读数：2351
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









上一章我们讲解了HTTP request的解析过程。我们基本知道了Nginx是如何解析HTTP的行和头。

这一章我们主要讲解Nginx的阶段处理（**PHASE 状态机实现**）。阶段处理的概念如何理解，我举个例子：一个HTTP请求过来，除了解析HTTP的行和头外，还需要解析URI的rewrite、接受HTTP BODY中的POST数据、转发给后端JAVA/PHP服务器进行数据处理等等一系列的操作。

Nginx的PHASE阶段处理共包含11部分，通过这11个阶段的处理，就能完整的处理一个HTTP请求了。




### 11个阶段处理

|**阶段处理**|**对应函数**|**描述信息**|
|----|----|----|
|NGX_HTTP_POST_READ_PHASE|ngx_http_core_generic_phase|读取请求内容阶段，这个阶段没有默认的handler，主要用来读取请求体，并对请求体做相应的处理|
|NGX_HTTP_SERVER_REWRITE_PHASE|ngx_http_core_rewrite_phase|Server请求地址重写阶段，这个阶段主要是处理全局的(server block)的rewrite规则|
|NGX_HTTP_FIND_CONFIG_PHASE|ngx_http_core_find_config_phase|配置查找阶段，不支持ch，这个阶段主要是通过uri来查找对应的location。然后将uri和location的数据关联起来。这个阶段主要处理逻辑在checker函数中，不能挂载自定义的handler|
|NGX_HTTP_REWRITE_PHASE|ngx_http_core_rewrite_phase|Location请求地址重写阶段，这个主要处理location block的rewrite。|
|NGX_HTTP_POST_REWRITE_PHASE|ngx_http_core_post_rewrite_phase|请求地址重写提交阶段，post rewrite，这个主要是进行一些校验以及收尾工作，以便于交给后面的模块。这个phase不能挂载自定义handler|
|NGX_HTTP_PREACCESS_PHASE|ngx_http_core_generic_phase|访问权限检查准备阶段，比如流控这种类型的access就放在这个phase，也就是说它主要是进行一些比较粗粒度的access。|
|NGX_HTTP_ACCESS_PHASE|ngx_http_core_access_phase|访问权限检查阶段，这个比如存取控制，权限验证就放在这个phase，一般来说处理动作是交给下面的模块做的.这个主要是做一些细粒度的access。|
|NGX_HTTP_POST_ACCESS_PHASE|ngx_http_core_post_access_phase|访问权限检查提交阶段，一般来说当上面的access模块得到access_code之后就会由这个模块根据access_code来进行操作 这个phase不能挂载自定义handler|
|NGX_HTTP_TRY_FILES_PHASE|ngx_http_core_try_files_phase|配置项try_files处理阶段，try_file模块，也就是对应配置文件中的try_files指令。 这个phase不能挂载自定义handler|
|NGX_HTTP_CONTENT_PHASE|ngx_http_core_content_phase|内容产生阶段，内容处理模块，产生文件内容，如果是[PHP](http://lib.csdn.net/base/php)，去调用phpcgi，如果是代理，就转发给相应的后端服务器|
|NGX_HTTP_LOG_PHASE|ngx_http_core_generic_phase|日志模块处理阶段，日志处理模块，是每个请求最后一定会执行的。用于打印访问日志。|




```cpp
typedef enum {
    NGX_HTTP_POST_READ_PHASE = 0, /* 读取请求内容阶段 */

    NGX_HTTP_SERVER_REWRITE_PHASE, /* Server请求地址重写阶段 */

    NGX_HTTP_FIND_CONFIG_PHASE, /* 配置查找阶段 */
    NGX_HTTP_REWRITE_PHASE,  /* Location请求地址重写阶段 */
    NGX_HTTP_POST_REWRITE_PHASE, /* 请求地址重写提交阶段 */

    NGX_HTTP_PREACCESS_PHASE,  /* 访问权限检查准备阶段 */

    NGX_HTTP_ACCESS_PHASE,  /* 访问权限检查阶段 */
    NGX_HTTP_POST_ACCESS_PHASE,  /* 访问权限检查提交阶段 */

    NGX_HTTP_TRY_FILES_PHASE,   /* 配置项try_files处理阶段 */
    NGX_HTTP_CONTENT_PHASE,    /* 内容产生阶段 */

    NGX_HTTP_LOG_PHASE  /* 日志模块处理阶段 */
} ngx_http_phases;
```

四个phase是不支持添加http功能的handler的，这四个阶段分别是NGX_FIND_CONFIG_PHASE、NGX_POST_REWRITE_PHASE、NGX_POST_ACCESS_PHASE、NGX_TRY_FILES_PHASE




如果需要自定义添加模块挂载的Nginx的阶段处理上，推荐挂载到这三个阶段：

NGX_HTTP_PREACCESS_PHASE、NGX_HTTP_ACCESS_PHASE、NGX_HTTP_CONTENT_PHASE




### 阶段处理的初始化


我们从前面的篇章中知道，HTTP模块的初始化从**ngx_http_block**这个方法开始。阶段处理的初始化也在这个方法中。


```cpp
/**
 *ngx_http_commands 命令集的回调函数
 *HTTP模块初始化的入口函数
 *
 */
static char *
ngx_http_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
.........
    /* 初始化阶段数组 只有这里面的6个阶段可以挂载模块 */
    if (ngx_http_init_phases(cf, cmcf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }
........
    /**
     * 初始化阶段处理
     */
    if (ngx_http_init_phase_handlers(cf, cmcf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }
}
```




#### ngx_http_init_phases 初始化阶段数组

```cpp
/**
 * 初始化阶段处理数组
 * 每一个可以挂载模块的阶段，都定义了一个cmcf->phases[?].handlers的数组
 * 每个阶段被调用的时候，都会去遍历改阶段处理数组下需要处理的逻辑函数
 */
static ngx_int_t
ngx_http_init_phases(ngx_conf_t *cf, ngx_http_core_main_conf_t *cmcf)
{
    if (ngx_array_init(&cmcf->phases[NGX_HTTP_POST_READ_PHASE].handlers,
                       cf->pool, 1, sizeof(ngx_http_handler_pt))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    if (ngx_array_init(&cmcf->phases[NGX_HTTP_SERVER_REWRITE_PHASE].handlers,
                       cf->pool, 1, sizeof(ngx_http_handler_pt))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    if (ngx_array_init(&cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers,
                       cf->pool, 1, sizeof(ngx_http_handler_pt))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    if (ngx_array_init(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers,
                       cf->pool, 1, sizeof(ngx_http_handler_pt))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    if (ngx_array_init(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers,
                       cf->pool, 2, sizeof(ngx_http_handler_pt))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    if (ngx_array_init(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers,
                       cf->pool, 4, sizeof(ngx_http_handler_pt))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    if (ngx_array_init(&cmcf->phases[NGX_HTTP_LOG_PHASE].handlers,
                       cf->pool, 1, sizeof(ngx_http_handler_pt))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    return NGX_OK;
}
```




#### ngx_http_init_phase_handlers 初始化阶段处理


```cpp
/**
 * 初始化阶段处理
 *
 * nginx在接收并解析完请求行，请求头之后，就会依次调用各个phase handler
 *
 * 	NGX_HTTP_POST_READ_PHASE	读取请求内容阶段
	NGX_HTTP_SERVER_REWRITE_PHASE	Server请求地址重写阶段
	NGX_HTTP_FIND_CONFIG_PHASE	配置查找阶段
	NGX_HTTP_REWRITE_PHASE	Location请求地址重写阶段
	NGX_HTTP_POST_REWRITE_PHASE	请求地址重写提交阶段
	NGX_HTTP_PREACCESS_PHASE	访问权限检查准备阶段
	NGX_HTTP_ACCESS_PHASE	访问权限检查阶段
	NGX_HTTP_POST_ACCESS_PHASE	访问权限检查提交阶段
	NGX_HTTP_TRY_FILES_PHASE	配置项try_files处理阶段
	NGX_HTTP_CONTENT_PHASE	内容产生阶段
	NGX_HTTP_LOG_PHASE	日志模块处理阶段
 */
static ngx_int_t
ngx_http_init_phase_handlers(ngx_conf_t *cf, ngx_http_core_main_conf_t *cmcf)
{
    ngx_int_t                   j;
    ngx_uint_t                  i, n;
    ngx_uint_t                  find_config_index, use_rewrite, use_access;
    ngx_http_handler_pt        *h;
    ngx_http_phase_handler_t   *ph;
    ngx_http_phase_handler_pt   checker;

    cmcf->phase_engine.server_rewrite_index = (ngx_uint_t) -1;
    cmcf->phase_engine.location_rewrite_index = (ngx_uint_t) -1;
    find_config_index = 0;
    use_rewrite = cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers.nelts ? 1 : 0;
    use_access = cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers.nelts ? 1 : 0;

    n = use_rewrite + use_access + cmcf->try_files + 1 /* find config phase */;

    for (i = 0; i < NGX_HTTP_LOG_PHASE; i++) {
        n += cmcf->phases[i].handlers.nelts;
    }

    ph = ngx_pcalloc(cf->pool,
                     n * sizeof(ngx_http_phase_handler_t) + sizeof(void *));
    if (ph == NULL) {
        return NGX_ERROR;
    }

    cmcf->phase_engine.handlers = ph;
    n = 0;

    for (i = 0; i < NGX_HTTP_LOG_PHASE; i++) {
        h = cmcf->phases[i].handlers.elts;

        switch (i) {

        /* server中的rewrite*/
        case NGX_HTTP_SERVER_REWRITE_PHASE:
            if (cmcf->phase_engine.server_rewrite_index == (ngx_uint_t) -1) {
                cmcf->phase_engine.server_rewrite_index = n;
            }
            checker = ngx_http_core_rewrite_phase;

            break;

        /* 根据URI查找 location */
        case NGX_HTTP_FIND_CONFIG_PHASE:
            find_config_index = n;

            ph->checker = ngx_http_core_find_config_phase;
            n++;
            ph++;

            continue;

        /* localtion级别的rewrite */
        case NGX_HTTP_REWRITE_PHASE:
            if (cmcf->phase_engine.location_rewrite_index == (ngx_uint_t) -1) {
                cmcf->phase_engine.location_rewrite_index = n;
            }
            checker = ngx_http_core_rewrite_phase;

            break;

        /* server、location级别的rewrite都是在这个phase进行收尾工作的 */
        case NGX_HTTP_POST_REWRITE_PHASE:
            if (use_rewrite) {
                ph->checker = ngx_http_core_post_rewrite_phase;
                ph->next = find_config_index;
                n++;
                ph++;
            }

            continue;

        /* 细粒度的access，比如权限验证、存取控制 */
        case NGX_HTTP_ACCESS_PHASE:
            checker = ngx_http_core_access_phase;
            n++;
            break;

        /* 根据上述两个phase得到access code进行操作 */
        case NGX_HTTP_POST_ACCESS_PHASE:
            if (use_access) {
                ph->checker = ngx_http_core_post_access_phase;
                ph->next = n;
                ph++;
            }

            continue;

        /* 实现try_files指令 */
        case NGX_HTTP_TRY_FILES_PHASE:
            if (cmcf->try_files) {
                ph->checker = ngx_http_core_try_files_phase;
                n++;
                ph++;
            }

            continue;

        /* 生成http响应 */
        case NGX_HTTP_CONTENT_PHASE:
            checker = ngx_http_core_content_phase;
            break;

        default:
            checker = ngx_http_core_generic_phase;
        }

        n += cmcf->phases[i].handlers.nelts;

        for (j = cmcf->phases[i].handlers.nelts - 1; j >=0; j--) {
            ph->checker = checker;
            ph->handler = h[j];
            ph->next = n;
            ph++;
        }
    }

    return NGX_OK;
}
```


### 阶段处理过程


#### 阶段处理核心函数ngx_http_core_run_phases


先看下阶段处理返回的几个状态含义：


NGX_OK:    表示该阶段已经处理完成，需要转入下一个阶段；

NGX_DECLINED:    表示需要转入本阶段的下一个handler继续处理；

NGX_AGAIN, NGX_DONE:表示需要等待某个事件发生才能继续处理（比如等待网络IO），此时Nginx为了不阻塞其他请求的处理，必须中断当前请求的执行链，等待事件发生之后继续执行该handler；

NGX_ERROR:    表示发生了错误，需要结束该请求。






```cpp
/**
 * 11个阶段处理HTTP请求
 */
void
ngx_http_core_run_phases(ngx_http_request_t *r)
{
    ngx_int_t                   rc;
    ngx_http_phase_handler_t   *ph;
    ngx_http_core_main_conf_t  *cmcf;

    cmcf = ngx_http_get_module_main_conf(r, ngx_http_core_module);

    ph = cmcf->phase_engine.handlers;

    /* 遍历解析和处理各个阶段的HTTP请求 如果返回rc==NGX_AGAIN 则交由下一个阶段处理；返回OK则返回结果  */
    while (ph[r->phase_handler].checker) {

		rc = ph[r->phase_handler].checker(r, &ph[r->phase_handler]);

        if (rc == NGX_OK) {
            return;
        }
    }
}

/**
 * 内容接收阶段
 */
ngx_int_t
ngx_http_core_generic_phase(ngx_http_request_t *r, ngx_http_phase_handler_t *ph)
{
    ngx_int_t  rc;

    /*
     * generic phase checker,
     * used by the post read and pre-access phases
     */

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "generic phase: %ui", r->phase_handler);

    /* handler 回调函数*/
    rc = ph->handler(r);

    /* 本阶段处理完成，跳转到下一个阶段处理 */
    if (rc == NGX_OK) {
        r->phase_handler = ph->next;
        return NGX_AGAIN;
    }

    /* 本阶段当前的回调函数处理完成，继续执行本阶段其他回调函数 */
    if (rc == NGX_DECLINED) {
        r->phase_handler++;
        return NGX_AGAIN;
    }

    if (rc == NGX_AGAIN || rc == NGX_DONE) {
        return NGX_OK;
    }

    /* rc == NGX_ERROR || rc == NGX_HTTP_...  */

    ngx_http_finalize_request(r, rc);

    return NGX_OK;
}
```


### 挂载自定义模块

Nginx的自定义模块在**http/modules/**目录下。如果你编写了一个模块，并且想在阶段处理中，编写自己的模块，那么就非常简单了。只要在模块init初始化的时候，将回调函数注册到阶段上，就能实现自定义阶段处理拦截。

我们看下ngx_http_static_module.c模块的阶段处理的注册。


```cpp
/**
 * 模块初始化
 */
static ngx_int_t
ngx_http_static_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt        *h;
    ngx_http_core_main_conf_t  *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
    /* 注册到NGX_HTTP_CONTENT_PHASE阶段 */
    h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    /* 设置阶段回调函数 */
    *h = ngx_http_static_handler;

    return NGX_OK;
}
```






以上就是整个阶段处理的过程。理解之后发现并不难，而且可扩展性非常强，非常适合在不同阶段中挂载自定义的Nginx模块代码。

下一章，也是最后一章，我们讲解一下自定义模块的实现。




















