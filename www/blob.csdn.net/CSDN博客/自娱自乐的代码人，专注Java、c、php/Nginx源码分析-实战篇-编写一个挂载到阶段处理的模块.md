# Nginx源码分析 - 实战篇 - 编写一个挂载到阶段处理的模块 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2017年06月08日 13:19:07[initphp](https://me.csdn.net/initphp)阅读数：2302
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









HTTP模块篇，我们讲过Nginx的HTTP阶段处理 《Nginx源码分析 - HTTP模块篇 - ngx_http_core_run_phases函数和HTTP模块的阶段处理PHASE handler》

我们代码在《Nginx源码分析 - 实战篇 - 编写一个自定义的模块》的基础上，将定义一个回调函数挂载到HTTP处理的阶段上。

先看代码：



```cpp
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_hello_handler(ngx_http_request_t *r);

static char *
ngx_http_hello(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_int_t ngx_http_hello_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_hello_log_handler(ngx_http_request_t *r);

/**
 * 处理nginx.conf中的配置命令解析
 * 例如：
 * location /hello {
 *  	hello
 * }
 * 当用户请求:http://127.0.0.1/hello的时候，请求会跳转到hello这个配置上
 * hello的命令行解析回调函数：ngx_http_hello
 */
static ngx_command_t ngx_http_hello_commands[] = { {
ngx_string("hello"),
NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LMT_CONF
		| NGX_CONF_NOARGS, ngx_http_hello,
NGX_HTTP_LOC_CONF_OFFSET, 0, NULL },
ngx_null_command };

/**
 * 模块上下文
 */
static ngx_http_module_t ngx_http_hello_module_ctx = { NULL, ngx_http_hello_init, NULL, NULL,
		NULL, NULL, NULL, NULL };

/**
 * 模块的定义
 */
ngx_module_t ngx_http_hello_module = {
NGX_MODULE_V1, &ngx_http_hello_module_ctx, ngx_http_hello_commands,
NGX_HTTP_MODULE, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
NGX_MODULE_V1_PADDING };

/**
 * 命令解析的回调函数
 * 该函数中，主要获取loc的配置，并且设置location中的回调函数handler
 */
static char *
ngx_http_hello(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
	ngx_http_core_loc_conf_t *clcf;

	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	/* 设置回调函数。当请求http://127.0.0.1/hello的时候，会调用此回调函数 */
	clcf->handler = ngx_http_hello_handler;

	return NGX_CONF_OK;
}

/**
 * 模块回调函数，输出hello world
 */
static ngx_int_t ngx_http_hello_handler(ngx_http_request_t *r) {
	if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))) {
		return NGX_HTTP_NOT_ALLOWED;
	}

	ngx_int_t rc = ngx_http_discard_request_body(r);
	if (rc != NGX_OK) {
		return rc;
	}

	ngx_str_t type = ngx_string("text/plain");
	ngx_str_t response = ngx_string("Hello World");
	r->headers_out.status = NGX_HTTP_OK;
	r->headers_out.content_length_n = response.len;
	r->headers_out.content_type = type;

	rc = ngx_http_send_header(r);
	if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
		return rc;
	}

	ngx_buf_t *b;
	b = ngx_create_temp_buf(r->pool, response.len);
	if (b == NULL) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	ngx_memcpy(b->pos, response.data, response.len);
	b->last = b->pos + response.len;
	b->last_buf = 1;

	ngx_chain_t out;
	out.buf = b;
	out.next = NULL;

    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                            "hello world ");

	return ngx_http_output_filter(r, &out);
}

/**
 * 初始化
 * 将ngx_http_hello_log_handler挂载到NGX_HTTP_LOG_PHASE日志处理阶段
 */
static ngx_int_t ngx_http_hello_init(ngx_conf_t *cf) {
	ngx_http_handler_pt *h;
	ngx_http_core_main_conf_t *cmcf;

	cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

	h = ngx_array_push(&cmcf->phases[NGX_HTTP_LOG_PHASE].handlers);
	if (h == NULL) {
		return NGX_ERROR;
	}

	*h = ngx_http_hello_log_handler;

	return NGX_OK;
}

/**
 * NGX_HTTP_LOG_PHASE日志处理阶段的回调函数å
 */
static ngx_int_t ngx_http_hello_log_handler(ngx_http_request_t *r) {

	/* 仅仅在日志处理阶段，新增加一行日志 */
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                            "hello ==================================> ");

	return NGX_DECLINED;
}
```


#### 初始化绑定阶段处理
我们定义了一个ngx_http_hello_init的方法，主要用于将ngx_http_hello_log_handler函数挂载到NGX_HTTP_LOG_PHASE日志处理阶段


```cpp
/**
 * 初始化
 * 将ngx_http_hello_log_handler挂载到NGX_HTTP_LOG_PHASE日志处理阶段
 */
static ngx_int_t ngx_http_hello_init(ngx_conf_t *cf) {
	ngx_http_handler_pt *h;
	ngx_http_core_main_conf_t *cmcf;

	cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

	h = ngx_array_push(&cmcf->phases[NGX_HTTP_LOG_PHASE].handlers);
	if (h == NULL) {
		return NGX_ERROR;
	}

	*h = ngx_http_hello_log_handler;

	return NGX_OK;
}
```



其中ngx_http_hello_init方法在模块上下文ngx_http_hello_module_ctx的处理阶段被初始化


```cpp
/**
 * 模块上下文
 */
static ngx_http_module_t ngx_http_hello_module_ctx = { NULL, ngx_http_hello_init, NULL, NULL,
		NULL, NULL, NULL, NULL };
```



#### 回调函数写日志

回调函数ngx_http_hello_log_handler，主要写一行error的日志






```cpp
/**
 * NGX_HTTP_LOG_PHASE日志处理阶段的回调函数å
 */
static ngx_int_t ngx_http_hello_log_handler(ngx_http_request_t *r) {

	/* 仅仅在日志处理阶段，新增加一行日志 */
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                            "hello ==================================> ");

	return NGX_DECLINED;
}
```



#### 编译调试结果

因为我们绑定的是日志处理阶段，所以每次对Nginx的HTTP请求，都会回调**ngx_http_hello_log_hanlder**方法

我们可以看Nginx的日志中发现，我们已经成功了

![](https://img-my.csdn.net/uploads/201706/08/1496898899_6050.png)








