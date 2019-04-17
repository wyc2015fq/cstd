# nginx phase handler的原理和选择 - 囧囧有神的专栏 - CSDN博客





2014年08月25日 10:58:46[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：4567









## nginx phase handler的原理和选择

### PHASE HANDLER的种类

nginx在接收并解析完请求行，请求头之后，就会依次调用各个phase handler。 phase handler是完成nginx主要功能的阶段。

Nginx有如下11种phase，phase会依次执行。同一个phase，可能会挂载多个handler。其中***斜体加粗***的phase，不允许挂载用户自定义的handler
|**PHASE**|
|----|
|读取请求内容阶段|
|Server请求地址重写阶段|
|配置查找阶段|
|Location请求地址重写阶段|
|请求地址重写提交阶段|
|访问权限检查准备阶段|
|访问权限检查阶段|
|访问权限检查提交阶段|
|配置项try_files处理阶段|
|内容产生阶段|
|日志模块处理阶段|

### 如何注册phase handler

一般情况下，我们自定义的模块，大多数是挂载在NGX_HTTP_CONTENT_PHASE阶段的。挂载的动作一般是在模块上下文调用的postconfiguration函数中。

挂载的代码示例如下：



```cpp
static ngx_int_t
ngx_http_hello_init(ngx_conf_t *cf)    // postconfiguration hook点
{
        ngx_http_handler_pt        *h;
        ngx_http_core_main_conf_t  *cmcf;

        cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

        h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
        if (h == NULL) {
                return NGX_ERROR;
        }

        *h = ngx_http_hello_handler;

        return NGX_OK;
}
```


cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers 是content phase阶段的handler数组，类似的:

cmcf->phases[NGX_HTTP_SERVER_REWRITE_PHASE].handlers 

cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers

cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers

。。。
**每个可挂载的phase，都有一个phase handler数组, 你可以选择挂载在不同的phase数组里**

Nginx在解析http的配置时，会将多个phase handler数组，合成为一个数组

```cpp
static char *
ngx_http_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)  // http指令的hook函数
{
    。。。 
    for (m = 0; ngx_modules[m]; m++) {
        if (ngx_modules[m]->type != NGX_HTTP_MODULE) {
            continue;
        }

        module = ngx_modules[m]->ctx;

        if (module->postconfiguration) {
            if (module->postconfiguration(cf) != NGX_OK) {// 执行各模块的postconfigure hook, phase handler就在此时挂载
                return NGX_CONF_ERROR;
            }
        }
　　}
　　。。。
　　
　　if (ngx_http_init_phase_handlers(cf, cmcf) != NGX_OK) {// 将多个phase handler数组，合成为一个数组
　　    return NGX_CONF_ERROR;
　　}
   。。。
}
```

ngx_http_init_phase_handlers(cf, cmcf) 函数将(除log phase以外的)几个phase handler数组，合成为一个ngx_http_phase_handler_t类型的数组

```cpp
struct ngx_http_phase_handler_s {
    ngx_http_phase_handler_pt  checker; // 检查handler返回结果，决定下一个执行的handler
    ngx_http_handler_pt        handler; // handler 钩子
    ngx_uint_t                 next;    // 指向下一个phase的第一个handler
};
```

以下是不同的phase，对应不同的checker函数
|**PHASE**|
|----|
|ngx_http_core_generic_phase|
|ngx_http_core_rewrite_phase|
|ngx_http_core_find_config_phase|
|ngx_http_core_rewrite_phase|
|ngx_http_core_post_rewrite_phase|
|ngx_http_core_generic_phase|
|ngx_http_core_access_phase|
|ngx_http_core_post_access_phase|
|ngx_http_core_try_files_phase|
|ngx_http_core_content_phase|
|ngx_http_core_generic_phase|

### Phase handler如何执行

nginx在接收并解析完请求行，请求头之后，就会依次调用各个phase handler.

```cpp
void
ngx_http_core_run_phases(ngx_http_request_t *r)
{
    ngx_int_t                   rc;
    ngx_http_phase_handler_t   *ph;
    ngx_http_core_main_conf_t  *cmcf;
    cmcf = ngx_http_get_module_main_conf(r, ngx_http_core_module);
    ph = cmcf->phase_engine.handlers;
    while (ph[r->phase_handler].checker) {
        rc = ph[r->phase_handler].checker(r, &ph[r->phase_handler]);
        if (rc == NGX_OK) {
            return;
        }
    }
}
```

可以看到，其实依次执行的就是我们在初始化阶段，生成的ngx_http_phase_handler_t 数组

我们通过debug，来看一下ngx_http_phase_handler_t数组的实际内容
|**运行中时ph数组的内容**| | |
|----|----|----|
|**checker**|**handler**|**next**|
|ngx_http_core_rewrite_phase|ngx_http_rewrite_handler|1|
|ngx_http_core_find_config_phase|0|0|
|ngx_http_core_rewrite_phase|ngx_http_rewrite_handler|3|
|ngx_http_core_post_rewrite_phase|0|1|
|ngx_http_core_generic_phase|ngx_http_l7waf_handler|7|
|ngx_http_core_generic_phase|ngx_http_limit_req_handler|7|
|ngx_http_core_generic_phase|ngx_http_limit_conn_handler|7|
|ngx_http_core_access_phase|ngx_http_access_handler|10|
|ngx_http_core_access_phase|ngx_http_auth_basic_handler|10|
|ngx_http_core_post_access_phase|0|10|
|ngx_http_core_try_files_phase|0|0|
|ngx_http_core_content_phase|ngx_http_index_handler|14|
|ngx_http_core_content_phase|ngx_http_autoindex_handler|14|
|ngx_http_core_content_phase|ngx_http_static_handler|14|
|0|0|0|

我们来看一个具体的checker函数

```cpp
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

    rc = ph->handler(r);          // 执行挂载的handler

    if (rc == NGX_OK) {           // 返回值是NGX_OK，说明这个phase执行完了，跳到下一个phase的第一个handler去
        r->phase_handler = ph->next;
        return NGX_AGAIN;
    }

    if (rc == NGX_DECLINED) {     // 返回值NGX_DECLINED，继续执行这个phase的下一个handler
        r->phase_handler++;
        return NGX_AGAIN;
    }

    if (rc == NGX_AGAIN || rc == NGX_DONE) { //说明请求处理完了。下面所有的phase handler都不需要再执行了
        return NGX_OK;
    }

    /* rc == NGX_ERROR || rc == NGX_HTTP_...  */

    ngx_http_finalize_request(r, rc);        // 返回错误，结束请求，返回相应的错误页

    return NGX_OK;
}
```

### 如何选择哪个phase

**读取请求内容阶段**

这个阶段没有默认的handler，主要用来读取请求体，并对请求体做相应的处理

**Server请求地址重写阶段**

这个阶段主要是处理全局的(server block)的rewrite规则

**配置查找阶段**

这个阶段主要是通过uri来查找对应的location。然后将uri和location的数据关联起来。这个阶段主要处理逻辑在checker函数中，*不能挂载自定义的handler*

**Location请求地址重写阶段**

这个主要处理location block的rewrite。 

**请求地址重写提交阶段**

post rewrite，这个主要是进行一些校验以及收尾工作，以便于交给后面的模块。*这个phase不能挂载自定义handler*

**访问权限检查准备阶段**

比如流控这种类型的access就放在这个phase，也就是说它主要是进行一些比较粗粒度的access。

**访问权限检查阶段**

这个比如存取控制，权限验证就放在这个phase，一般来说处理动作是交给下面的模块做的.这个主要是做一些细粒度的access。

**访问权限检查提交阶段**

一般来说当上面的access模块得到access_code之后就会由这个模块根据access_code来进行操作 *这个phase不能挂载自定义handler*

**配置项try_files处理阶段**

try_file模块，也就是对应配置文件中的try_files指令。 *这个phase不能挂载自定义handler*

按顺序检查文件是否存在，返回第一个找到的文件。结尾的斜线表示为文件夹 -$uri/。如果所有的文件都找不到，会进行一个内部重定向到最后一个参数。

**内容产生阶段**

内容处理模块，产生文件内容，如果是php，去调用phpcgi，如果是代理，就转发给相应的后端服务器

**日志模块处理阶段**

日志处理模块，是每个请求最后一定会执行的。用于打印访问日志。




通过如上对phase handler的分析，我们可以知道nginx划分不同的phase，是将不同功能，安排在不同的顺序执行。

**选择挂载在哪个phase，就选择了handler执行的顺序，并且选择了不同的checker函数。**

自定义的handler有时候可以挂载在不同的phase，都可以正常运行。

自定义的handler，如果依赖某一个phase的结果，则必须挂载在该phase后面的phase上。

自定义的handler需要遵守nginx对不同phase的功能划分，但不是必需的。




除去4个不能挂载的phase，和log phase，还有如下6个phase可以挂载

NGX_HTTP_POST_READ_PHASE

NGX_HTTP_SERVER_REWRITE_PHASE

NGX_HTTP_REWRITE_PHASE

NGX_HTTP_PREACCESS_PHASE

NGX_HTTP_ACCESS_PHASE

NGX_HTTP_CONTENT_PHASE


很多功能挂载在这6个phase，都可以实现。挂载在越前面，我们的性能会越好，挂载在后面，我们的自由度会更大。

比如说，如果我们实现在NGX_HTTP_POST_READ_PHASE 阶段，我们就不能用 location if 这些后面阶段实现的指令来组合实现一些更复杂的功能。

推荐使用

NGX_HTTP_PREACCESS_PHASE

NGX_HTTP_ACCESS_PHASE

NGX_HTTP_CONTENT_PHASE






