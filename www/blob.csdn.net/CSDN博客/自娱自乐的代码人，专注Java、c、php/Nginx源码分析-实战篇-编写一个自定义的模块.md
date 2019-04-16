# Nginx源码分析 - 实战篇 - 编写一个自定义的模块 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2017年06月08日 12:03:28[initphp](https://me.csdn.net/initphp)阅读数：1957
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









从前面的篇章中，我们基本了解了Nginx的运行原理。Nginx整个框架都是通过模块的方式，对整个系统进行解耦和扩展。

在HTTP的http/modules/目录下，有非常多的关于HTTP协议的模块，每个模块都有不同的功能。

![](https://img-my.csdn.net/uploads/201706/08/1496896389_3093.png)





接下去我们就需要创建一个自定义的模块。




#### 创建扩展模块目录

我们在nginx-1.13.1文件夹下，创建一个**/extends/ngx_http_hello_module**的目录




![](https://img-my.csdn.net/uploads/201706/08/1496896694_9613.png)





#### 创建config文件

我们的模块名称为：**ngx_http_hello_module**

我们需要创建一个config文件，这样Nginx在编译的时候，会将你自定义的模块编译进去。

config文件中配置如下：


```cpp
ngx_addon_name=ngx_http_hello_module
HTTP_MODULES="$HTTP_MODULES ngx_http_hello_module"
NGX_ADDON_SRCS="$NGX_ADDON_SRCS $ngx_addon_dir/ngx_http_hello_module.c"
```


#### 创建ngx_http_hello_module.c文件


创建自定义的模块必须遵守Nginx的模块规范，需要定义：

1. ngx_command_t 命令行解析结构

2. ngx_http_module_t 模块的上下文结构

3. ngx_module_t 模块结构

如果对这几个数据结构还不熟悉，先参照我之前的文章吧。


```cpp
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_hello_handler(ngx_http_request_t *r);

static char *
ngx_http_hello(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


/**
 * 处理nginx.conf中的配置命令解析
 * 例如：
 * location /hello {
 *  	hello
 * }
 * 当用户请求:http://127.0.0.1/hello的时候，请求会跳转到hello这个配置上
 * hello的命令行解析回调函数：ngx_http_hello
 */
static ngx_command_t ngx_http_hello_commands[] = {
		{
				ngx_string("hello"),
				NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LMT_CONF | NGX_CONF_NOARGS,
				ngx_http_hello,
				NGX_HTTP_LOC_CONF_OFFSET,
				0,
				NULL
		},
		ngx_null_command
};


/**
 * 模块上下文
 */
static ngx_http_module_t ngx_http_hello_module_ctx = { NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL };

/**
 * 模块的定义
 */
ngx_module_t ngx_http_hello_module = {
		NGX_MODULE_V1,
		&ngx_http_hello_module_ctx,
		ngx_http_hello_commands,
		NGX_HTTP_MODULE,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NGX_MODULE_V1_PADDING
};

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

	return ngx_http_output_filter(r, &out);
}
```







#### 修改Nginx.conf文件

我们需要通过http://127.0.0.1/hello 就能访问到我们自定义的hello模块。所以我们需要修改一下Nginx.conf文件：





```cpp
#ngx_http_hello_module.c
        location /hello {
        	hello
        }
```




#### 编译Nginx源码

编译自定义的模块，需要加上：**--add_module=模块文件夹**



```cpp
sudo ./configure --prefix=/Users/zhuli/Documents/code/soft/nginx --add-module=/Users/xxx/Documents/code/soft/nginx-1.13.1/extends/ngx_http_hello_module
sudo make && make install
```



编译后，生成模块数组文件：objs/ngx_modules.c 里面就包含我们自定义的模块

（模块数组*ngx_modules[] 中的值就是通过这个文件初始化的；然后通过这个模块数组对每个模块进行初始化）


```cpp
extern ngx_module_t  ngx_http_limit_req_module;
extern ngx_module_t  ngx_http_geo_module;
extern ngx_module_t  ngx_http_map_module;
extern ngx_module_t  ngx_http_split_clients_module;
extern ngx_module_t  ngx_http_referer_module;
extern ngx_module_t  ngx_http_rewrite_module;
extern ngx_module_t  ngx_http_proxy_module;
extern ngx_module_t  ngx_http_fastcgi_module;
extern ngx_module_t  ngx_http_uwsgi_module;
extern ngx_module_t  ngx_http_scgi_module;
extern ngx_module_t  ngx_http_memcached_module;
extern ngx_module_t  ngx_http_empty_gif_module;
extern ngx_module_t  ngx_http_browser_module;
extern ngx_module_t  ngx_http_upstream_hash_module;
extern ngx_module_t  ngx_http_upstream_ip_hash_module;
extern ngx_module_t  ngx_http_upstream_least_conn_module;
extern ngx_module_t  ngx_http_upstream_keepalive_module;
extern ngx_module_t  ngx_http_upstream_zone_module;
extern ngx_module_t  ngx_http_hello_module;
extern ngx_module_t  ngx_http_write_filter_module;
extern ngx_module_t  ngx_http_header_filter_module;
extern ngx_module_t  ngx_http_chunked_filter_module;
extern ngx_module_t  ngx_http_range_header_filter_module;
extern ngx_module_t  ngx_http_gzip_filter_module;
extern ngx_module_t  ngx_http_postpone_filter_module;
extern ngx_module_t  ngx_http_ssi_filter_module;
extern ngx_module_t  ngx_http_charset_filter_module;
extern ngx_module_t  ngx_http_userid_filter_module;
extern ngx_module_t  ngx_http_headers_filter_module;
extern ngx_module_t  ngx_http_copy_filter_module;
extern ngx_module_t  ngx_http_range_body_filter_module;
extern ngx_module_t  ngx_http_not_modified_filter_module;
```



#### 浏览器访问


在浏览器中输入http://127.0.0.1/hello 就能访问我们自定义的hello模块了。




![](https://img-my.csdn.net/uploads/201706/08/1496897987_5598.png)





