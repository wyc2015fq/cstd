# Nginx源码分析 - HTTP模块篇 -  ngx_http_wait_request_handler函数和HTTP Request解析过程 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2017年01月05日 20:59:43[initphp](https://me.csdn.net/initphp)阅读数：3104
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









上一章我们讲解了整个Nginx的tcp连接过程，我们知道当客户端刚连接上来的时候，会触发ngx_event_accept方法；当read事件进来的时候，就会调用ngx_http_wait_request_handler。**ngx_http_wait_request_handler方法也是http模块数据处理的入口。**

这一章，我们从ngx_http_wait_request_handler函数讲起，深入整个http的request的解析过程。nginx文件：**http/ngx_http_request.c**

### HTTP Request解析流程图

![](https://img-blog.csdn.net/20170106144241547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


说明：

1. Nginx的HTTP核心模块只解析request的请求行和请求头，不会主动读取HTTP 请求body数据，但是提供了**ngx_http_read_client_request_body**方法，供各个filter模块处理。

2.ngx_http_wait_request_handler：等待read事件上来，并且等到HTTP的request数据

3. ngx_http_process_request_line：处理HTTP的request的请求行

4.  ngx_http_process_request_header：处理HTTP的request的请求头

5. ngx_http_handler：HTTP核心处理函数，模块filter链的分发函数

**6. 设置r->write_event_handler = ngx_http_core_run_phases，Nginx的write事件模块，从ngx_http_core_run_phases方法开始**





**为什么不主动读取HTTP BODY数据？**


看到上面的图，可能很多人会比较奇怪了，为什么HTTP的核心模块只读取了HTTP协议的请求行和请求头，而没有读取HTTP的请求body内容。

因为大部分情况下，HTTP协议是不需要用到HTTP body中的数据的，例如你返回一张图片或者一个静态页面。

只有在需要将body数据传输到后端的例如JAVA、PHP等的时候，才会需要将HTTP的body数据带过去（POST请求、文件上传等）。所以Nginx只有在特殊的模块下（比如proxy模块，fastcgi模块，uwsgi模块等），才会去主动调用**ngx_http_read_client_request_body**方法，将body传递到后端。


### 源码分析

#### 1. ngx_http_wait_request_handler 函数

等待read事件上来，并且等到HTTP的request数据。

ngx_http_wait_request_handler主要是一个等待数据到来的功能。里面有一个设计亮点：**此函数会一直等待TCP管道中的数据，如果触发了read事件，但是没有读取到数据，则Nginx会将buf内存删除，然后继续等待read事件的到来，好处是防止大量非法请求上来，又占用内存不释放，导致Nginx内存暴涨。**




```cpp
/**
 * read事件回调函数；主要处理读取事件
 * 等待处理http request 数据
 */
static void
ngx_http_wait_request_handler(ngx_event_t *rev)
{
    u_char                    *p;
    size_t                     size;
    ssize_t                    n;
    ngx_buf_t                 *b;
    ngx_connection_t          *c;
    ngx_http_connection_t     *hc;
    ngx_http_core_srv_conf_t  *cscf;

    /* 从rev中获取连接 ngx_connection_t 对象 */
    c = rev->data;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "http wait request handler");

    /* 客户端超时 */
    if (rev->timedout) {
        ngx_log_error(NGX_LOG_INFO, c->log, NGX_ETIMEDOUT, "client timed out");
        ngx_http_close_connection(c);
        return;
    }

    /* 客户端连接关闭 */
    if (c->close) {
        ngx_http_close_connection(c);
        return;
    }

    /* 获取sev的conf配置 ngx_http_core_srv_conf_t*/
    hc = c->data;
    cscf = ngx_http_get_module_srv_conf(hc->conf_ctx, ngx_http_core_module);

    /* 每次读取数据的buf大小 */
    size = cscf->client_header_buffer_size;

    /* ngx_connection_s 中的buffer：用于接收和缓存客户端发来的字符流 */
    b = c->buffer;

    /* 如果buffer为null了，则直接创建一个临时buf ngx_create_temp_buf*/
    if (b == NULL) {
        b = ngx_create_temp_buf(c->pool, size);
        if (b == NULL) {
            ngx_http_close_connection(c);
            return;
        }

        c->buffer = b;

    /* 如果buf的start为null，则直接创建一块内存，并且调整ngx_buf_t的姿势 */
    } else if (b->start == NULL) {

        b->start = ngx_palloc(c->pool, size);
        if (b->start == NULL) {
            ngx_http_close_connection(c);
            return;
        }

        b->pos = b->start;
        b->last = b->start;
        b->end = b->last + size;
    }

    /**
     * 接收数据的回调函数：os/ngx_recv.c文件夹中的 ngx_unix_recv
     * ngx_unix_recv：主要调用系统的recv函数，循环接收TCP管道中的数据
     */
    n = c->recv(c, b->last, size);

    /*
     * NGX_AGAIN
     * 这个是一个Http连接第一次等待读取数据，如果第一次接收的数据为空，则表示当前客户端连接上来了，但是数据还未上来，
     * 则将当前连接上的读事件添加到定时器机制中，同时将读事件注册到epoll 事件机制中，return 从当前函数返回
     * */
    if (n == NGX_AGAIN) {

        if (!rev->timer_set) {
            ngx_add_timer(rev, c->listening->post_accept_timeout);
            ngx_reusable_connection(c, 1);
        }

        /* 重新把读事件注册到事件中，每次epoll_wait后，fd的事件类型将会清空，需要再次注册读写事件 */
        if (ngx_handle_read_event(rev, 0) != NGX_OK) {
            ngx_http_close_connection(c);
            return;
        }

        /*
         * We are trying to not hold c->buffer's memory for an idle connection.
         * 因为啥也不做，暂时现将buf清空
         */
        if (ngx_pfree(c->pool, b->start) == NGX_OK) {
            b->start = NULL;
        }

        return;
    }

    /* 读取错误 ，则关闭连接 */
    if (n == NGX_ERROR) {
        ngx_http_close_connection(c);
        return;
    }

    /* 客户端主动关闭连接 */
    if (n == 0) {
        ngx_log_error(NGX_LOG_INFO, c->log, 0,
                      "client closed connection");
        ngx_http_close_connection(c);
        return;
    }

    /* 真正读取到数据后的处理 */
    b->last += n;

    if (hc->proxy_protocol) {
        hc->proxy_protocol = 0;

        p = ngx_proxy_protocol_read(c, b->pos, b->last);

        if (p == NULL) {
            ngx_http_close_connection(c);
            return;
        }

        b->pos = p;

        if (b->pos == b->last) {
            c->log->action = "waiting for request";
            b->pos = b->start;
            b->last = b->start;
            ngx_post_event(rev, &ngx_posted_events);
            return;
        }
    }

    c->log->action = "reading client request line";

    ngx_reusable_connection(c, 0);

    /* 调用 ngx_http_create_request 方法构造ngx_http_request_t 请求结构体，并设置到当前连接的data 成员 */
    c->data = ngx_http_create_request(c);
    if (c->data == NULL) {
        ngx_http_close_connection(c);
        return;
    }

    /* 设置当前读事件的回调方法为 ngx_http_process_request_line
     * 并执行该回调方法开始接收并解析HTTP头部的请求行 */
    rev->handler = ngx_http_process_request_line;
    ngx_http_process_request_line(rev);
}
```




#### 2. ngx_http_process_request_line 处理头部请求行

ngx_http_process_request_line：**主要用来处理HTTP协议的请求头。**




```cpp
/**
 * read事件的时候，读取和处理HTTP头部的行
 * 请求行如下：
 * POST /index.php HTTP/1.1
 */
static void
ngx_http_process_request_line(ngx_event_t *rev)
{
    ssize_t              n;
    ngx_int_t            rc, rv;
    ngx_str_t            host;
    ngx_connection_t    *c;
    ngx_http_request_t  *r;


    c = rev->data; //ngx_connection_t
    r = c->data; //ngx_http_request_t

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, rev->log, 0,
                   "http process request line");

    /* 超时清空 */
    if (rev->timedout) {
        ngx_log_error(NGX_LOG_INFO, c->log, NGX_ETIMEDOUT, "client timed out");
        c->timedout = 1;
        ngx_http_close_request(r, NGX_HTTP_REQUEST_TIME_OUT);
        return;
    }

    rc = NGX_AGAIN;
    /* 循环处理请求 NGX_AGAIN为标识服务*/
    for ( ;; ) {

        if (rc == NGX_AGAIN) {

        	/* 读取请求数据  也会调用方法：os/ngx_recv.c文件夹中的 ngx_unix_recv*/
            n = ngx_http_read_request_header(r);

            if (n == NGX_AGAIN || n == NGX_ERROR) {
                return;
            }
        }

        /*
         * 如果数据接收已经完整了，则对HTTP请求行进行 parse解析
         * */
        rc = ngx_http_parse_request_line(r, r->header_in);

        /* 如果解析成功 */
        if (rc == NGX_OK) {

            /* the request line has been parsed successfully */

            r->request_line.len = r->request_end - r->request_start;
            r->request_line.data = r->request_start;
            r->request_length = r->header_in->pos - r->request_start;

            ngx_log_debug1(NGX_LOG_DEBUG_HTTP, c->log, 0,
                           "http request line: \"%V\"", &r->request_line);

            r->method_name.len = r->method_end - r->request_start + 1;
            r->method_name.data = r->request_line.data;

            if (r->http_protocol.data) {
                r->http_protocol.len = r->request_end - r->http_protocol.data;
            }

            if (ngx_http_process_request_uri(r) != NGX_OK) {
                return;
            }

            if (r->host_start && r->host_end) {

                host.len = r->host_end - r->host_start;
                host.data = r->host_start;

                rc = ngx_http_validate_host(&host, r->pool, 0);

                if (rc == NGX_DECLINED) {
                    ngx_log_error(NGX_LOG_INFO, c->log, 0,
                                  "client sent invalid host in request line");
                    ngx_http_finalize_request(r, NGX_HTTP_BAD_REQUEST);
                    return;
                }

                if (rc == NGX_ERROR) {
                    ngx_http_close_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
                    return;
                }

                if (ngx_http_set_virtual_server(r, &host) == NGX_ERROR) {
                    return;
                }

                r->headers_in.server = host;
            }

            if (r->http_version < NGX_HTTP_VERSION_10) {

                if (r->headers_in.server.len == 0
                    && ngx_http_set_virtual_server(r, &r->headers_in.server)
                       == NGX_ERROR)
                {
                    return;
                }

                ngx_http_process_request(r);
                return;
            }


            if (ngx_list_init(&r->headers_in.headers, r->pool, 20,
                              sizeof(ngx_table_elt_t))
                != NGX_OK)
            {
                ngx_http_close_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
                return;
            }

            c->log->action = "reading client request headers";

            /* 请求行解析成功后，将read事件的回调函数设置为：ngx_http_process_request_headers
             * ngx_http_process_request_headers：用于处理http的header数据*/
            rev->handler = ngx_http_process_request_headers;
            ngx_http_process_request_headers(rev);

            return;
        }

        /* 如果ngx_http_parse_request_line解析结果有误，则ngx_http_finalize_request 析构request结构*/
        if (rc != NGX_AGAIN) {

            /* there was error while a request line parsing */

            ngx_log_error(NGX_LOG_INFO, c->log, 0,
                          ngx_http_client_errors[rc - NGX_HTTP_CLIENT_ERROR]);
            ngx_http_finalize_request(r, NGX_HTTP_BAD_REQUEST);
            return;
        }

        /* NGX_AGAIN: a request line parsing is still incomplete */

        /* 如果解析的header头部非常大，则调用ngx_http_alloc_large_header_buffer处理buffer*/
        if (r->header_in->pos == r->header_in->end) {

            rv = ngx_http_alloc_large_header_buffer(r, 1);

            if (rv == NGX_ERROR) {
                ngx_http_close_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
                return;
            }

            if (rv == NGX_DECLINED) {
                r->request_line.len = r->header_in->end - r->request_start;
                r->request_line.data = r->request_start;

                ngx_log_error(NGX_LOG_INFO, c->log, 0,
                              "client sent too long URI");
                ngx_http_finalize_request(r, NGX_HTTP_REQUEST_URI_TOO_LARGE);
                return;
            }
        }
    }
}
```



ngx_http_read_request_header：**主要调用系统的recv函数，循环接收TCP管道中的数据**，接收数据的回调函数：os/ngx_recv.c文件夹中的 ngx_unix_recv



```cpp
/**
 * 读取请求数据
 */
static ssize_t
ngx_http_read_request_header(ngx_http_request_t *r)
{
    ssize_t                    n;
    ngx_event_t               *rev;
    ngx_connection_t          *c;
    ngx_http_core_srv_conf_t  *cscf;

    c = r->connection;
    rev = c->read;

    n = r->header_in->last - r->header_in->pos;

    if (n > 0) {
        return n;
    }

    /**
     * 接收数据的回调函数：os/ngx_recv.c文件夹中的 ngx_unix_recv
     * ngx_unix_recv：主要调用系统的recv函数，循环接收TCP管道中的数据
     */
    if (rev->ready) {
        n = c->recv(c, r->header_in->last,
                    r->header_in->end - r->header_in->last);
    } else {
        n = NGX_AGAIN; //如果还未可以读取，则返回NGX_AGAIN 继续等待数据到来
    }

    /* 如果是等待状态，则继续放进事件event循环中*/
    if (n == NGX_AGAIN) {
        if (!rev->timer_set) {
            cscf = ngx_http_get_module_srv_conf(r, ngx_http_core_module);
            ngx_add_timer(rev, cscf->client_header_timeout);
        }

        /* 重新把读事件注册到事件中，每次epoll_wait后，fd的事件类型将会清空，需要再次注册读写事件 */
        if (ngx_handle_read_event(rev, 0) != NGX_OK) {
            ngx_http_close_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
            return NGX_ERROR;
        }

        return NGX_AGAIN;
    }

    /* 客户端主动关闭连接 */
    if (n == 0) {
        ngx_log_error(NGX_LOG_INFO, c->log, 0,
                      "client prematurely closed connection");
    }

    /* 客户端关闭连接或者请求错误，则对request进行析构处理*/
    if (n == 0 || n == NGX_ERROR) {
        c->error = 1;
        c->log->action = "reading client request headers";

        ngx_http_finalize_request(r, NGX_HTTP_BAD_REQUEST);
        return NGX_ERROR;
    }

    r->header_in->last += n;

    return n;
}
```



#### 3. ngx_http_process_request_headers 处理HTTP协议的header部分

ngx_http_process_request_headers：主要用于解析HTTP头部的header部分数据。例如：

Host: localhost
Accept-Language: zh-cn,zh;q=0.5



```cpp
/**
 * 用于处理http的header数据
 * 请求头：
 * Host: localhost
 * User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:10.0.2) Gecko/20100101 Firefox/10.0.2
 * Accept: text/html,application/xhtml+xml,application/xml;q=0.9,/;q=0.8
 * Accept-Language: zh-cn,zh;q=0.5
 * Accept-Encoding: gzip, deflate
 */
static void
ngx_http_process_request_headers(ngx_event_t *rev)
{
    u_char                     *p;
    size_t                      len;
    ssize_t                     n;
    ngx_int_t                   rc, rv;
    ngx_table_elt_t            *h;
    ngx_connection_t           *c;
    ngx_http_header_t          *hh;
    ngx_http_request_t         *r;
    ngx_http_core_srv_conf_t   *cscf;
    ngx_http_core_main_conf_t  *cmcf;

    c = rev->data;
    r = c->data;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, rev->log, 0,
                   "http process request header line");

    /* 超时 处理*/
    if (rev->timedout) {
        ngx_log_error(NGX_LOG_INFO, c->log, NGX_ETIMEDOUT, "client timed out");
        c->timedout = 1;
        ngx_http_close_request(r, NGX_HTTP_REQUEST_TIME_OUT);
        return;
    }

    cmcf = ngx_http_get_module_main_conf(r, ngx_http_core_module);

    rc = NGX_AGAIN;

    for ( ;; ) {

        if (rc == NGX_AGAIN) {

        	/* 如果buf太小，则扩大buf */
            if (r->header_in->pos == r->header_in->end) {

                rv = ngx_http_alloc_large_header_buffer(r, 0);

                if (rv == NGX_ERROR) {
                    ngx_http_close_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
                    return;
                }

                if (rv == NGX_DECLINED) {
                    p = r->header_name_start;

                    r->lingering_close = 1;

                    if (p == NULL) {
                        ngx_log_error(NGX_LOG_INFO, c->log, 0,
                                      "client sent too large request");
                        ngx_http_finalize_request(r,
                                            NGX_HTTP_REQUEST_HEADER_TOO_LARGE);
                        return;
                    }

                    len = r->header_in->end - p;

                    if (len > NGX_MAX_ERROR_STR - 300) {
                        len = NGX_MAX_ERROR_STR - 300;
                    }

                    ngx_log_error(NGX_LOG_INFO, c->log, 0,
                                "client sent too long header line: \"%*s...\"",
                                len, r->header_name_start);

                    ngx_http_finalize_request(r,
                                            NGX_HTTP_REQUEST_HEADER_TOO_LARGE);
                    return;
                }
            }

            /* 读取请求数据  调用方法：os/ngx_recv.c文件夹中的 ngx_unix_recv */
            n = ngx_http_read_request_header(r);

            if (n == NGX_AGAIN || n == NGX_ERROR) {
                return;
            }
        }

        /* the host header could change the server configuration context */
        cscf = ngx_http_get_module_srv_conf(r, ngx_http_core_module);

        /* 处理HTTP请求头数据 */
        rc = ngx_http_parse_header_line(r, r->header_in,
                                        cscf->underscores_in_headers);

        /* 处理成功，则继续解析下一个http header数据 */
        if (rc == NGX_OK) {

            r->request_length += r->header_in->pos - r->header_name_start;

            if (r->invalid_header && cscf->ignore_invalid_headers) {

                /* there was error while a header line parsing */

                ngx_log_error(NGX_LOG_INFO, c->log, 0,
                              "client sent invalid header line: \"%*s\"",
                              r->header_end - r->header_name_start,
                              r->header_name_start);
                continue;
            }

            /* a header line has been parsed successfully */

            h = ngx_list_push(&r->headers_in.headers);
            if (h == NULL) {
                ngx_http_close_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
                return;
            }

            h->hash = r->header_hash;

            h->key.len = r->header_name_end - r->header_name_start;
            h->key.data = r->header_name_start;
            h->key.data[h->key.len] = '\0';

            h->value.len = r->header_end - r->header_start;
            h->value.data = r->header_start;
            h->value.data[h->value.len] = '\0';

            h->lowcase_key = ngx_pnalloc(r->pool, h->key.len);
            if (h->lowcase_key == NULL) {
                ngx_http_close_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
                return;
            }

            if (h->key.len == r->lowcase_index) {
                ngx_memcpy(h->lowcase_key, r->lowcase_header, h->key.len);

            } else {
                ngx_strlow(h->lowcase_key, h->key.data, h->key.len);
            }

            hh = ngx_hash_find(&cmcf->headers_in_hash, h->hash,
                               h->lowcase_key, h->key.len);

            if (hh && hh->handler(r, h, hh->offset) != NGX_OK) {
                return;
            }

            ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "http header: \"%V: %V\"",
                           &h->key, &h->value);

            continue;
        }

        /* 如果HTTP的header全部处理*/
        if (rc == NGX_HTTP_PARSE_HEADER_DONE) {

            /* a whole header has been parsed successfully */

            ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "http header done");

            r->request_length += r->header_in->pos - r->header_name_start;

            r->http_state = NGX_HTTP_PROCESS_REQUEST_STATE;

            /* 检查解析后的request header是否符合要求，如果不符合要求，返回NGX_ERROR */
            rc = ngx_http_process_request_header(r);

            if (rc != NGX_OK) {
                return;
            }

            /* 请求行和请求头处理完成后，调用ngx_http_process_request，处理HTTP请求body以及响应 */
            ngx_http_process_request(r);

            return;
        }

        if (rc == NGX_AGAIN) {

            /* a header line parsing is still not complete */

            continue;
        }

        /* rc == NGX_HTTP_PARSE_INVALID_HEADER */

        ngx_log_error(NGX_LOG_INFO, c->log, 0,
                      "client sent invalid header line");

        ngx_http_finalize_request(r, NGX_HTTP_BAD_REQUEST);
        return;
    }
}
```

#### 4. ngx_http_process_request 处理request

ngx_http_process_request：设置read和write的回调函数ngx_http_request_handler，ngx_http_request_handler通过状态机来判断是读事件还是写事件。

调用HTTP模块的filter链，filter链分为header和body链两部分



```cpp
/**
 * 
 */
void
ngx_http_process_request(ngx_http_request_t *r)
{
    ngx_connection_t  *c;

    c = r->connection;

#if (NGX_HTTP_SSL)

    if (r->http_connection->ssl) {
        long                      rc;
        X509                     *cert;
        ngx_http_ssl_srv_conf_t  *sscf;

        if (c->ssl == NULL) {
            ngx_log_error(NGX_LOG_INFO, c->log, 0,
                          "client sent plain HTTP request to HTTPS port");
            ngx_http_finalize_request(r, NGX_HTTP_TO_HTTPS);
            return;
        }

        sscf = ngx_http_get_module_srv_conf(r, ngx_http_ssl_module);

        if (sscf->verify) {
            rc = SSL_get_verify_result(c->ssl->connection);

            if (rc != X509_V_OK
                && (sscf->verify != 3 || !ngx_ssl_verify_error_optional(rc)))
            {
                ngx_log_error(NGX_LOG_INFO, c->log, 0,
                              "client SSL certificate verify error: (%l:%s)",
                              rc, X509_verify_cert_error_string(rc));

                ngx_ssl_remove_cached_session(sscf->ssl.ctx,
                                       (SSL_get0_session(c->ssl->connection)));

                ngx_http_finalize_request(r, NGX_HTTPS_CERT_ERROR);
                return;
            }

            if (sscf->verify == 1) {
                cert = SSL_get_peer_certificate(c->ssl->connection);

                if (cert == NULL) {
                    ngx_log_error(NGX_LOG_INFO, c->log, 0,
                                  "client sent no required SSL certificate");

                    ngx_ssl_remove_cached_session(sscf->ssl.ctx,
                                       (SSL_get0_session(c->ssl->connection)));

                    ngx_http_finalize_request(r, NGX_HTTPS_NO_CERT);
                    return;
                }

                X509_free(cert);
            }
        }
    }

#endif

    if (c->read->timer_set) {
        ngx_del_timer(c->read);
    }

#if (NGX_STAT_STUB)
    (void) ngx_atomic_fetch_add(ngx_stat_reading, -1);
    r->stat_reading = 0;
    (void) ngx_atomic_fetch_add(ngx_stat_writing, 1);
    r->stat_writing = 1;
#endif

    /* read和write事件都设置为：ngx_http_request_handler，通过事件状态来判断*/
    c->read->handler = ngx_http_request_handler;
    c->write->handler = ngx_http_request_handler;
    r->read_event_handler = ngx_http_block_reading;

    /* http处理分发核心函数 */
    ngx_http_handler(r);

    ngx_http_run_posted_requests(c);
}
```

ngx_http_request_handler：通过事件状态来判断读或者写事件，然后调用回调函数



```cpp
/**
 * read和write事件都设置为：ngx_http_request_handler，通过事件状态来判断
 */
static void
ngx_http_request_handler(ngx_event_t *ev)
{
    ngx_connection_t    *c;
    ngx_http_request_t  *r;

    c = ev->data;
    r = c->data;

    ngx_http_set_log_request(c->log, r);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "http run request: \"%V?%V\"", &r->uri, &r->args);

    if (ev->write) {
        r->write_event_handler(r);

    } else {
        r->read_event_handler(r);
    }

    /* 主要用于处理subrequest 子请求 */
    ngx_http_run_posted_requests(c);
}
```

**ngx_http_run_posted_requests：主要用于子请求，子请求会将请求链内容合并到主请求上。会损耗一些性能，因为会重新走一遍write的回调函数ngx_http_core_run_phases解析过程。**

例如location配置：



```cpp
location /main.htm {
    # content of main.htm: main
    add_before_body /hello.htm;
    add_after_body /world.htm;
}
location /hello.htm {
    #content of hello.htm: hello
}
location /world.htm {
    #content of world.htm: world
}
```

```cpp
/**
 * 主要用于处理subrequest 子请求
 *
 * location /main.htm {
 *   # content of main.htm: main
 *   add_before_body /hello.htm;
 *   add_after_body /world.htm;
 *	}
 *	location /hello.htm {
 *   	#content of hello.htm: hello
 *	}
 *	location /world.htm {
 *   	#content of world.htm: world
 *	}
 *
 */
void
ngx_http_run_posted_requests(ngx_connection_t *c)
{
    ngx_http_request_t         *r;
    ngx_http_posted_request_t  *pr;

    /* 遍历 */
    for ( ;; ) {

    	/* 如果连接已经销毁，则退出 */
        if (c->destroyed) {
            return;
        }

        r = c->data;
        pr = r->main->posted_requests;

        if (pr == NULL) {
            return;
        }

        r->main->posted_requests = pr->next;

        r = pr->request;

        ngx_http_set_log_request(c->log, r);

        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "http posted request: \"%V?%V\"", &r->uri, &r->args);

        /* 调用write handler.   */
        r->write_event_handler(r);
    }
}
```




5. ngx_http_handler 核心分发函数



ngx_http_handler函数主要用于设置write事件回调函数。**r->write_event_handler = ngx_http_core_run_phases**



```cpp
/**
 * 核心分发函数，主要用于设置write写事件回调函数ngx_http_core_run_phases
 */
void
ngx_http_handler(ngx_http_request_t *r)
{
    ngx_http_core_main_conf_t  *cmcf;

    r->connection->log->action = NULL;

    if (!r->internal) {
        switch (r->headers_in.connection_type) {
        case 0:
            r->keepalive = (r->http_version > NGX_HTTP_VERSION_10);
            break;

        case NGX_HTTP_CONNECTION_CLOSE:
            r->keepalive = 0;
            break;

        case NGX_HTTP_CONNECTION_KEEP_ALIVE:
            r->keepalive = 1;
            break;
        }

        r->lingering_close = (r->headers_in.content_length_n > 0
                              || r->headers_in.chunked);
        r->phase_handler = 0;

    } else {
        cmcf = ngx_http_get_module_main_conf(r, ngx_http_core_module);
        r->phase_handler = cmcf->phase_engine.server_rewrite_index;
    }

    r->valid_location = 1;
#if (NGX_HTTP_GZIP)
    r->gzip_tested = 0;
    r->gzip_ok = 0;
    r->gzip_vary = 0;
#endif

    /* 设置write事件回调函数，并且执行ngx_http_core_run_phases回调函数 */
    r->write_event_handler = ngx_http_core_run_phases;
    ngx_http_core_run_phases(r);
}
```


















