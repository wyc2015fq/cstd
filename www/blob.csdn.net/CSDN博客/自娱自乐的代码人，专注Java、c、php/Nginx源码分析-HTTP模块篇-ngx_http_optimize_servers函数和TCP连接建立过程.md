# Nginx源码分析 - HTTP模块篇 - ngx_http_optimize_servers函数和TCP连接建立过程 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年12月18日 19:11:42[initphp](https://me.csdn.net/initphp)阅读数：2347
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









上一章，我们讲解了HTTP模块是如何初始化的。这一章节，主要讲解**监听套接字初始化函数ngx_http_optimize_servers和Nginx整个连接的过程**


### 监听套接字初始化函数ngx_http_optimize_servers

ngx_http_optimize_servers主要处理Nginx服务的监听套接字



```cpp
/**
 * ngx_http_optimize_servers：处理Nginx服务的监听套接字
 * 说明：主要遍历Nginx服务器提供的端口，然后根据每一个IP地址:port这种配置创建一个监听套接字
 * ngx_http_init_listening：初始化监听套接字
 */
static ngx_int_t
ngx_http_optimize_servers(ngx_conf_t *cf, ngx_http_core_main_conf_t *cmcf,
    ngx_array_t *ports)
{
    ngx_uint_t             p, a;
    ngx_http_conf_port_t  *port;
    ngx_http_conf_addr_t  *addr;

    if (ports == NULL) {
        return NGX_OK;
    }

    /* 根据Nginx配置的端口号进行遍历 */
    port = ports->elts;
    for (p = 0; p < ports->nelts; p++) {

        ngx_sort(port[p].addrs.elts, (size_t) port[p].addrs.nelts,
                 sizeof(ngx_http_conf_addr_t), ngx_http_cmp_conf_addrs);

        /*
         * check whether all name-based servers have the same
         * configuration as a default server for given address:port
         */

        addr = port[p].addrs.elts;
        for (a = 0; a < port[p].addrs.nelts; a++) {

            if (addr[a].servers.nelts > 1
#if (NGX_PCRE)
                || addr[a].default_server->captures
#endif
               )
            {
                if (ngx_http_server_names(cf, cmcf, &addr[a]) != NGX_OK) {
                    return NGX_ERROR;
                }
            }
        }

        /* 初始化监听套接字 */
        if (ngx_http_init_listening(cf, &port[p]) != NGX_OK) {
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}
```

ngx_http_init_listening 主要初始化侦听套接字




```cpp
/**
 * 初始化侦听套接字
 */
static ngx_int_t
ngx_http_init_listening(ngx_conf_t *cf, ngx_http_conf_port_t *port)
{
    ngx_uint_t                 i, last, bind_wildcard;
    ngx_listening_t           *ls;
    ngx_http_port_t           *hport;
    ngx_http_conf_addr_t      *addr;

    addr = port->addrs.elts;
    last = port->addrs.nelts;

    /*
     * If there is a binding to an "*:port" then we need to bind() to
     * the "*:port" only and ignore other implicit bindings.  The bindings
     * have been already sorted: explicit bindings are on the start, then
     * implicit bindings go, and wildcard binding is in the end.
     */

    if (addr[last - 1].opt.wildcard) {
        addr[last - 1].opt.bind = 1;
        bind_wildcard = 1;

    } else {
        bind_wildcard = 0;
    }

    i = 0;

    /* 根据IP地址 遍历 创建 listening*/
    while (i < last) {

        if (bind_wildcard && !addr[i].opt.bind) {
            i++;
            continue;
        }

        /* 创建侦听套接字 listening */
        ls = ngx_http_add_listening(cf, &addr[i]);
        if (ls == NULL) {
            return NGX_ERROR;
        }

        hport = ngx_pcalloc(cf->pool, sizeof(ngx_http_port_t));
        if (hport == NULL) {
            return NGX_ERROR;
        }

        ls->servers = hport;

        hport->naddrs = i + 1;

        switch (ls->sockaddr->sa_family) {

#if (NGX_HAVE_INET6)
        case AF_INET6:
            if (ngx_http_add_addrs6(cf, hport, addr) != NGX_OK) {
                return NGX_ERROR;
            }
            break;
#endif
        default: /* AF_INET */
            if (ngx_http_add_addrs(cf, hport, addr) != NGX_OK) {
                return NGX_ERROR;
            }
            break;
        }

        if (ngx_clone_listening(cf, ls) != NGX_OK) {
            return NGX_ERROR;
        }

        addr++;
        last--;
    }

    return NGX_OK;
}
```

ngx_http_add_listening 创建侦听套接字 listening




```cpp
/**
 * 创建侦听套接字 listening
 */
static ngx_listening_t *
ngx_http_add_listening(ngx_conf_t *cf, ngx_http_conf_addr_t *addr)
{
    ngx_listening_t           *ls;
    ngx_http_core_loc_conf_t  *clcf;
    ngx_http_core_srv_conf_t  *cscf;

    /* 创建一个套接字 */
    ls = ngx_create_listening(cf, &addr->opt.sockaddr.sockaddr,
                              addr->opt.socklen);
    if (ls == NULL) {
        return NULL;
    }

    ls->addr_ntop = 1;

    /* 侦听套接字 的回调函数。该回调函数在ngx_event_accept函数中回调；
     * 回调之后，会将读取事件回调函数rev->handler()修改成方法：ngx_http_wait_request_handler*/
    ls->handler = ngx_http_init_connection;

    cscf = addr->default_server;
    ls->pool_size = cscf->connection_pool_size;
    ls->post_accept_timeout = cscf->client_header_timeout;

    clcf = cscf->ctx->loc_conf[ngx_http_core_module.ctx_index];

    ls->logp = clcf->error_log;
    ls->log.data = &ls->addr_text;
    ls->log.handler = ngx_accept_log_error;


    return ls;
}
```



### Nginx整个连接的过程

通过[《Nginx源码分析 - Event事件篇 - Event模块的进程初始化ngx_event_process_init 》](blog.csdn.net/initphp/article/details/52550360)       和当前篇章的ngx_http_optimize_servers方法，我们就能将整个Nginx的连接过程串起来了。

整个流程如下：

1. 在Nginx main函数的**ngx_init_cycle()**方法中，调用了**ngx_open_listening_sockets函数**，这个函数负责将创建的监听套接字进行套接字选项的设置（比如非阻塞、接受发送的缓冲区、绑定、监听处理）




2. HTTP模块初始化优先于Event模块，HTTP模块通过**ngx_http_block()**方法进行初始化，然后调用**ngx_http_optimize_servers()**进行套接字的创建和初始化（ngx_http_init_listening、ngx_http_add_listening、ngx_create_listening）。**根据每一个IP地址:port这种配置创建监听套接字。**

****

3. ngx_http_add_listening函数，还会将ls->handler监听套接字的回调函数设置为**ngx_http_init_connection**。ngx_http_init_connection此函数主要初始化一个客户端连接connection。




4. Event模块的初始化主要调用**ngx_event_process_init()**函数。该函数每个worker工作进程都会初始化调用。然后设置read/write的回调函数。




5. ngx_event_process_init函数中，会将接收客户端连接的事件，设置为**rev->handler=****ngx_event_accept**方法，ngx_event_accept方法，只有在第一次客户端和Nginx服务端创建连接关系的时候调用。




6. 当客户端有连接上来，Nginx工作进程就会进入事件循环（epoll事件循环函数：**ngx_epoll_process_events**），发现有read读取的事件，则会调用ngx_event_accept函数。




7. 调用ngx_event_accept函数，会调用ngx_get_connection方法，得到一个客户端连接结构：ngx_connection_t结构。ngx_event_accept函数最终会调用监听套接字的handler回调函数，**ls->handler(c); ** 。




8. 从流程3中，我们知道ls->handler的函数对应**ngx_http_init_connection**方法。此方法主要初始化客户端的连接ngx_connection_t，并将客户端连接read读取事件的回调函数修改成**rev->handler = ngx_http_wait_request_handler**

****


9. 也就是说，当客户端连接上来，第一次事件循环的read事件会调用回调函数：ngx_event_accept函数；而后续的read事件的handler已经被ngx_http_init_connection方法修改掉，改成了ngx_http_wait_request_handler函数了。所以客户端的读取事件都会走ngx_http_wait_request_handler函数。




10. ngx_http_wait_request_handler函数也是整个HTTP模块的数据处理的入口函数了。

如下图表格：

![](https://img-blog.csdn.net/20161219100758401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







