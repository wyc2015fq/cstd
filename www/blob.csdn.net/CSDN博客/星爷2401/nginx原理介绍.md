# nginx原理介绍 - 星爷2401 - CSDN博客
2017年04月10日 17:27:42[星爷2401](https://me.csdn.net/leifukes)阅读数：1122
       nginx是一款轻量级的web服务器，相比较apache会占用更少的内存资源，并且采用异步非阻塞实现高并发请求，高度的模块化设计，模块编写清晰。
一、nginx功能分类：
> 
按模块（主要包含4大模块）：
> 
> 
1、CORE模块核心模块
> 
> 
2、EVENT模块事件模块
> 
> 
3、HTTP模块HTTP模块
> 
> 
4、MAIL模块邮件模块
> 
按功能：
> 
> 
1、Handlers（处理器模块）：此类模块直接处理请求。
> 
> 
2、Filters（过滤器模块）：对处理器模块输出的内容进行过滤，有nginx输出。
> 
> 
3、Proxies（代理类模块）：主要与后端的一些服务比如FastCGI等进行交互，实现服务代理和负载均衡等功能。
二、nginx模块结构（nginx的重点）
> 
1、nginx的配置文件，先从配置文件中的一条命令说起，如：
> 
```cpp
server 
{
        listen 80;
        server_name     www.xxxx.com;
        root            /var/xxxx/www;
        location / {
               root  /home/www;
               index index.php index.html index.htm;  
        }
}
```
2、nginx配置文件中的每一个命令，都会有ngx_comment_t结构体定义，如：root，root指令的定义（src/http/ngx_http_core_module.c
 文件中）
```cpp
{
       ngx_string("root"),
       NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
       ngx_http_core_root,
       NGX_HTTP_LOC_CONF_OFFSET,
       0,
      NULL 
},
```
### 3、ngx_command_t 结构体定义（具体字段含义下面会讲到）：
```cpp
struct ngx_command_s {  
    ngx_str_t             name;  
    ngx_uint_t            type;  
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);  
    ngx_uint_t            conf;  
    ngx_uint_t            offset;  
    void                 *post;  
};
```
### 4、ngx_module_t 模块结构体以及模块之间的关系
![](https://img-blog.csdn.net/20170414155409946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5、模块初始化过程
![](https://img-blog.csdn.net/20170414155441853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 三、nginx进程模型
> 
1、Master-Worker模型：
> 
Master进程负责处理外部信号，同时管理所有的worker进程
Worker进程负责处理链接的建立、请求和相应
> 
> 
![](https://img-blog.csdn.net/20170414155826776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 
> 2、nginx启动过程
> 
> 
![](https://img-blog.csdn.net/20170414160000346?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 四、nginx事件模型
> 
1、eventmodule数据结构
> 
> 
![](https://img-blog.csdn.net/20170414160256319?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2、event数据结构
> 
> 
![](https://img-blog.csdn.net/20170414160555980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、epoll_create&epoll_ctl添加监听事件
> 
> 
![](https://img-blog.csdn.net/20170414160654468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4、epoll_ctl添加http处理事件
> 
![](https://img-blog.csdn.net/20170414160749078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 五、nginx高并发原理
> 
1、每个worker都是独立进程，不需要加锁，进程互不影响。一般woker数目和cpu数目一致，减少因竞争cpu带来的上下文切换。
2、异步非阻塞。
3、epoll触发方式的理解
> 
> 
![](https://img-blog.csdn.net/20170414161257159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpZnVrZXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以上便是对nginx的模块，进程，事件，以及高并发的简单描述，后面会详细介绍没一个模块。

> 
