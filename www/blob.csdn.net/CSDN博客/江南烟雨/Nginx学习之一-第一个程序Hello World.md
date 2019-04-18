# Nginx学习之一-第一个程序Hello World - 江南烟雨 - CSDN博客
2013年06月19日 16:50:42[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：24808标签：[Nginx																[Linux																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)](https://so.csdn.net/so/search/s.do?q=Nginx&t=blog)
个人分类：[Nginx](https://blog.csdn.net/xiajun07061225/article/category/1461389)
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)
本例子实现了一个简单的hello world程序。运行效果：
虚拟机Ubuntu中：
![](https://img-blog.csdn.net/20130619164542734)
win7中chrome浏览器：
![](https://img-blog.csdn.net/20130619170550546)
一、config文件编写
Nginx提供了一种简单的方式将第三方的模块编译到Nginx中。首先把源代码文件全部放到一个目录下，同时在该目录中编写一个文件用于通知Nginx如何编译本模块，这个文件名必须为config。
然后，在configure脚本执行时加入参数--add-module=PATH(新模块源代码以及config文件存放目录)，就可以在执行政策编译安装流程时完成Nginx编译工作。
**config文件格式**
config文件其实是一个可执行的Shell脚本，如果只想开发一个HTTP模块，需要定义三个变量：
（1）ngx_adon_name。
仅在configure执行时使用，一般设置为模块名称。
（2）HTTP_MODULES。
保存所有的HTTP模块名称。每个模块间由空格相连。在重新设置这个变量时，不要直接覆盖，因此要如下设置：
"$HTTP_MODULES ngx_http_mytest_module"
（3）NGX_ADDON_SRCS。
用于指定新模块的源代码，多个待编译的源代码之间可以用空格相连。
注意，在设置这个变量时可以使用$ngx_addon_dir变量，它等价于configure执行时--add-module=PATH的PATH参数。
因此本例中的config文件内容如下：
```cpp
ngx_addon_name=ngx_http_mytest_module
HTTP_MODULES="$HTTP_MODULES ngx_http_mytest_module"
NGX_ADDON_SRCS="$NGX_ADDON_SRCS $ngx_addon_dir/ngx_http_mytest_module.c"
```
二、修改配置文件
/usr/local/nginx/conf/nginx.conf
添加下面内容：
![](https://img-blog.csdn.net/20130619164613015)
三、定义HTTP模块及处理用户请求
源代码：
```cpp
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r);
static char * 
ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
//处理配置项
static ngx_command_t ngx_http_mytest_commands[] = {
    {
        ngx_string("mytest"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,
        ngx_http_mytest,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },
    ngx_null_command
};
//模块上下文
static ngx_http_module_t ngx_http_mytest_module_ctx = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};
//新模块定义
ngx_module_t ngx_http_mytest_module = {
    NGX_MODULE_V1,
    &ngx_http_mytest_module_ctx,
    ngx_http_mytest_commands,
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
//配置项对应的回调函数
static char * 
ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_mytest_handler;
    return NGX_CONF_OK;
}
//实际完成处理的回调函数
static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r)
{
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
四、编译安装新模块
编译安装新模块的命令如下：
```cpp
./configure --prefix=/usr/local/nginx（指定安装部署后的根目录） --add-module=/home/nginx（新模块存放目录）
make
sudo make install
```
五、参考资料：
《深入理解Ngnix》
[淘宝tengine](http://tengine.taobao.org/book/chapter_03.html#hello-handler)
[nginx模块开发入门](http://kb.cnblogs.com/page/98352/)

