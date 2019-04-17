# Nginx基本配置参数说明与文档 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年03月01日 08:47:37[boonya](https://me.csdn.net/boonya)阅读数：113








**目录**

[Nginx配置](#Nginx%E9%85%8D%E7%BD%AE)

[基本配置参数说明](#%E5%9F%BA%E6%9C%AC%E9%85%8D%E7%BD%AE%E4%B8%8E%E5%8F%82%E6%95%B0%E8%AF%B4%E6%98%8E)

[nginx文档](#nginx%E6%96%87%E6%A1%A3)

[介绍](#%E4%BB%8B%E7%BB%8D)

[如何](#%E5%A6%82%E4%BD%95)

[发展](#%E5%8F%91%E5%B1%95)

[模块参考](#%E6%A8%A1%E5%9D%97%E5%8F%82%E8%80%83)

## Nginx配置

### 基本配置与参数说明

原文地址：[http://www.nginx.cn/76.html](http://www.nginx.cn/76.html)

深入了解Nginx就是弄清楚参数配置的具体含义，从而可以改造结合自己的需求配置相应的Nginx服务。

```bash
#运行用户
user nobody;
#启动进程,通常设置成和cpu的数量相等
worker_processes  1;
 
#全局错误日志及PID文件
#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;
 
#pid        logs/nginx.pid;
 
#工作模式及连接数上限
events {
    #epoll是多路复用IO(I/O Multiplexing)中的一种方式,
    #仅用于linux2.6以上内核,可以大大提高nginx的性能
    use   epoll; 
 
    #单个后台worker process进程的最大并发链接数    
    worker_connections  1024;
 
    # 并发总数是 worker_processes 和 worker_connections 的乘积
    # 即 max_clients = worker_processes * worker_connections
    # 在设置了反向代理的情况下，max_clients = worker_processes * worker_connections / 4  为什么
    # 为什么上面反向代理要除以4，应该说是一个经验值
    # 根据以上条件，正常情况下的Nginx Server可以应付的最大连接数为：4 * 8000 = 32000
    # worker_connections 值的设置跟物理内存大小有关
    # 因为并发受IO约束，max_clients的值须小于系统可以打开的最大文件数
    # 而系统可以打开的最大文件数和内存大小成正比，一般1GB内存的机器上可以打开的文件数大约是10万左右
    # 我们来看看360M内存的VPS可以打开的文件句柄数是多少：
    # $ cat /proc/sys/fs/file-max
    # 输出 34336
    # 32000 < 34336，即并发连接总数小于系统可以打开的文件句柄总数，这样就在操作系统可以承受的范围之内
    # 所以，worker_connections 的值需根据 worker_processes 进程数目和系统可以打开的最大文件总数进行适当地进行设置
    # 使得并发总数小于操作系统可以打开的最大文件数目
    # 其实质也就是根据主机的物理CPU和内存进行配置
    # 当然，理论上的并发总数可能会和实际有所偏差，因为主机还有其他的工作进程需要消耗系统资源。
    # ulimit -SHn 65535
 
}
 
 
http {
    #设定mime类型,类型由mime.type文件定义
    include    mime.types;
    default_type  application/octet-stream;
    #设定日志格式
    log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for"';
 
    access_log  logs/access.log  main;
 
    #sendfile 指令指定 nginx 是否调用 sendfile 函数（zero copy 方式）来输出文件，
    #对于普通应用，必须设为 on,
    #如果用来进行下载等应用磁盘IO重负载应用，可设置为 off，
    #以平衡磁盘与网络I/O处理速度，降低系统的uptime.
    sendfile     on;
    #tcp_nopush     on;
 
    #连接超时时间
    #keepalive_timeout  0;
    keepalive_timeout  65;
    tcp_nodelay     on;
 
    #开启gzip压缩
    gzip  on;
    gzip_disable "MSIE [1-6].";
 
    #设定请求缓冲
    client_header_buffer_size    128k;
    large_client_header_buffers  4 128k;
 
 
    #设定虚拟主机配置
    server {
        #侦听80端口
        listen    80;
        #定义使用 www.nginx.cn访问
        server_name  www.nginx.cn;
 
        #定义服务器的默认网站根目录位置
        root html;
 
        #设定本虚拟主机的访问日志
        access_log  logs/nginx.access.log  main;
 
        #默认请求
        location / {
            
            #定义首页索引文件的名称
            index index.php index.html index.htm;   
 
        }
 
        # 定义错误提示页面
        error_page   500 502 503 504 /50x.html;
        location = /50x.html {
        }
 
        #静态文件，nginx自己处理
        location ~ ^/(images|javascript|js|css|flash|media|static)/ {
            
            #过期30天，静态文件不怎么更新，过期可以设大一点，
            #如果频繁更新，则可以设置得小一点。
            expires 30d;
        }
 
        #PHP 脚本请求全部转发到 FastCGI处理. 使用FastCGI默认配置.
        location ~ .php$ {
            fastcgi_pass 127.0.0.1:9000;
            fastcgi_index index.php;
            fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
            include fastcgi_params;
        }
 
        #禁止访问 .htxxx 文件
            location ~ /.ht {
            deny all;
        }
 
    }
}
```

## nginx文档

### 介绍


- [安装nginx](http://nginx.org/en/docs/install.html)
- [从Sources构建nginx](http://nginx.org/en/docs/configure.html)
- [初学者指南](http://nginx.org/en/docs/beginners_guide.html)
- [管理员指南](https://www.nginx.com/resources/admin-guide/)
- [控制nginx](http://nginx.org/en/docs/control.html)
- [连接处理方法](http://nginx.org/en/docs/events.html)
- [设置哈希值](http://nginx.org/en/docs/hash.html)
- [调试日志](http://nginx.org/en/docs/debugging_log.html)
- [登录到syslog](http://nginx.org/en/docs/syslog.html)
- [配置文件测量单位](http://nginx.org/en/docs/syntax.html)
- [命令行参数](http://nginx.org/en/docs/switches.html)
- [适用于Windows的nginx](http://nginx.org/en/docs/windows.html)
- [nginx如何处理请求](http://nginx.org/en/docs/http/request_processing.html)
- [服务器名称](http://nginx.org/en/docs/http/server_names.html)
- [使用nginx作为HTTP负载均衡器](http://nginx.org/en/docs/http/load_balancing.html)
- [配置HTTPS服务器](http://nginx.org/en/docs/http/configuring_https_servers.html)
- [nginx如何处理TCP / UDP会话](http://nginx.org/en/docs/stream/stream_processing.html)
- [使用njs编写脚本](http://nginx.org/en/docs/njs/index.html)
- [“开源应用程序体系结构”中的“nginx”一章](http://www.aosabook.org/en/nginx.html)



### 如何


- [使用Visual C在Win32平台上构建nginx](http://nginx.org/en/docs/howto_build_on_win32.html)
- [在Amazon EC2上设置NGINX Plus环境](https://www.nginx.com/resources/admin-guide/setting-nginx-plus-environment-amazon-ec2/)
- [使用DTrace pid提供程序调试nginx](http://nginx.org/en/docs/nginx_dtrace_pid_provider.html)
- [转换重写规则](http://nginx.org/en/docs/http/converting_rewrite_rules.html)
- [WebSocket代理](http://nginx.org/en/docs/http/websocket.html)





### 发展


- [贡献变化](http://nginx.org/en/docs/contributing_changes.html)
- [开发指南](http://nginx.org/en/docs/dev/development_guide.html)



### 模块参考


- [按字母顺序排列的指令索引](http://nginx.org/en/docs/dirindex.html)
- [按字母顺序排列的变量索引](http://nginx.org/en/docs/varindex.html)
- [核心功能](http://nginx.org/en/docs/ngx_core_module.html)
- [ngx_http_core_module](http://nginx.org/en/docs/http/ngx_http_core_module.html)
- [ngx_http_access_module](http://nginx.org/en/docs/http/ngx_http_access_module.html)
- [ngx_http_addition_module](http://nginx.org/en/docs/http/ngx_http_addition_module.html)
- [ngx_http_api_module](http://nginx.org/en/docs/http/ngx_http_api_module.html)
- [ngx_http_auth_basic_module](http://nginx.org/en/docs/http/ngx_http_auth_basic_module.html)
- [ngx_http_auth_jwt_module](http://nginx.org/en/docs/http/ngx_http_auth_jwt_module.html)
- [ngx_http_auth_request_module](http://nginx.org/en/docs/http/ngx_http_auth_request_module.html)
- [ngx_http_autoindex_module](http://nginx.org/en/docs/http/ngx_http_autoindex_module.html)
- [ngx_http_browser_module](http://nginx.org/en/docs/http/ngx_http_browser_module.html)
- [ngx_http_charset_module](http://nginx.org/en/docs/http/ngx_http_charset_module.html)
- [ngx_http_dav_module](http://nginx.org/en/docs/http/ngx_http_dav_module.html)
- [ngx_http_empty_gif_module](http://nginx.org/en/docs/http/ngx_http_empty_gif_module.html)
- [ngx_http_f4f_module](http://nginx.org/en/docs/http/ngx_http_f4f_module.html)
- [ngx_http_fastcgi_module](http://nginx.org/en/docs/http/ngx_http_fastcgi_module.html)
- [ngx_http_flv_module](http://nginx.org/en/docs/http/ngx_http_flv_module.html)
- [ngx_http_geo_module](http://nginx.org/en/docs/http/ngx_http_geo_module.html)
- [ngx_http_geoip_module](http://nginx.org/en/docs/http/ngx_http_geoip_module.html)
- [ngx_http_grpc_module](http://nginx.org/en/docs/http/ngx_http_grpc_module.html)
- [ngx_http_gunzip_module](http://nginx.org/en/docs/http/ngx_http_gunzip_module.html)
- [ngx_http_gzip_module](http://nginx.org/en/docs/http/ngx_http_gzip_module.html)
- [ngx_http_gzip_static_module](http://nginx.org/en/docs/http/ngx_http_gzip_static_module.html)
- [ngx_http_headers_module](http://nginx.org/en/docs/http/ngx_http_headers_module.html)
- [ngx_http_hls_module](http://nginx.org/en/docs/http/ngx_http_hls_module.html)
- [ngx_http_image_filter_module](http://nginx.org/en/docs/http/ngx_http_image_filter_module.html)
- [ngx_http_index_module](http://nginx.org/en/docs/http/ngx_http_index_module.html)
- [ngx_http_js_module](http://nginx.org/en/docs/http/ngx_http_js_module.html)
- [ngx_http_keyval_module](http://nginx.org/en/docs/http/ngx_http_keyval_module.html)
- [ngx_http_limit_conn_module](http://nginx.org/en/docs/http/ngx_http_limit_conn_module.html)
- [ngx_http_limit_req_module](http://nginx.org/en/docs/http/ngx_http_limit_req_module.html)
- [ngx_http_log_module](http://nginx.org/en/docs/http/ngx_http_log_module.html)
- [ngx_http_map_module](http://nginx.org/en/docs/http/ngx_http_map_module.html)
- [ngx_http_memcached_module](http://nginx.org/en/docs/http/ngx_http_memcached_module.html)
- [ngx_http_mirror_module](http://nginx.org/en/docs/http/ngx_http_mirror_module.html)
- [ngx_http_mp4_module](http://nginx.org/en/docs/http/ngx_http_mp4_module.html)
- [ngx_http_perl_module](http://nginx.org/en/docs/http/ngx_http_perl_module.html)
- [ngx_http_proxy_module](http://nginx.org/en/docs/http/ngx_http_proxy_module.html)
- [ngx_http_random_index_module](http://nginx.org/en/docs/http/ngx_http_random_index_module.html)
- [ngx_http_realip_module](http://nginx.org/en/docs/http/ngx_http_realip_module.html)
- [ngx_http_referer_module](http://nginx.org/en/docs/http/ngx_http_referer_module.html)
- [ngx_http_rewrite_module](http://nginx.org/en/docs/http/ngx_http_rewrite_module.html)
- [ngx_http_scgi_module](http://nginx.org/en/docs/http/ngx_http_scgi_module.html)
- [ngx_http_secure_link_module](http://nginx.org/en/docs/http/ngx_http_secure_link_module.html)
- [ngx_http_session_log_module](http://nginx.org/en/docs/http/ngx_http_session_log_module.html)
- [ngx_http_slice_module](http://nginx.org/en/docs/http/ngx_http_slice_module.html)
- [ngx_http_spdy_module](http://nginx.org/en/docs/http/ngx_http_spdy_module.html)
- [ngx_http_split_clients_module](http://nginx.org/en/docs/http/ngx_http_split_clients_module.html)
- [ngx_http_ssi_module](http://nginx.org/en/docs/http/ngx_http_ssi_module.html)
- [ngx_http_ssl_module](http://nginx.org/en/docs/http/ngx_http_ssl_module.html)
- [ngx_http_status_module](http://nginx.org/en/docs/http/ngx_http_status_module.html)
- [ngx_http_stub_status_module](http://nginx.org/en/docs/http/ngx_http_stub_status_module.html)
- [ngx_http_sub_module](http://nginx.org/en/docs/http/ngx_http_sub_module.html)
- [ngx_http_upstream_module](http://nginx.org/en/docs/http/ngx_http_upstream_module.html)
- [ngx_http_upstream_conf_module](http://nginx.org/en/docs/http/ngx_http_upstream_conf_module.html)
- [ngx_http_upstream_hc_module](http://nginx.org/en/docs/http/ngx_http_upstream_hc_module.html)
- [ngx_http_userid_module](http://nginx.org/en/docs/http/ngx_http_userid_module.html)
- [ngx_http_uwsgi_module](http://nginx.org/en/docs/http/ngx_http_uwsgi_module.html)
- [ngx_http_v2_module](http://nginx.org/en/docs/http/ngx_http_v2_module.html)
- [ngx_http_xslt_module](http://nginx.org/en/docs/http/ngx_http_xslt_module.html)
- [ngx_mail_core_module](http://nginx.org/en/docs/mail/ngx_mail_core_module.html)
- [ngx_mail_auth_http_module](http://nginx.org/en/docs/mail/ngx_mail_auth_http_module.html)
- [ngx_mail_proxy_module](http://nginx.org/en/docs/mail/ngx_mail_proxy_module.html)
- [ngx_mail_ssl_module](http://nginx.org/en/docs/mail/ngx_mail_ssl_module.html)
- [ngx_mail_imap_module](http://nginx.org/en/docs/mail/ngx_mail_imap_module.html)
- [ngx_mail_pop3_module](http://nginx.org/en/docs/mail/ngx_mail_pop3_module.html)
- [ngx_mail_smtp_module](http://nginx.org/en/docs/mail/ngx_mail_smtp_module.html)
- [ngx_stream_core_module](http://nginx.org/en/docs/stream/ngx_stream_core_module.html)
- [ngx_stream_access_module](http://nginx.org/en/docs/stream/ngx_stream_access_module.html)
- [ngx_stream_geo_module](http://nginx.org/en/docs/stream/ngx_stream_geo_module.html)
- [ngx_stream_geoip_module](http://nginx.org/en/docs/stream/ngx_stream_geoip_module.html)
- [ngx_stream_js_module](http://nginx.org/en/docs/stream/ngx_stream_js_module.html)
- [ngx_stream_keyval_module](http://nginx.org/en/docs/stream/ngx_stream_keyval_module.html)
- [ngx_stream_limit_conn_module](http://nginx.org/en/docs/stream/ngx_stream_limit_conn_module.html)
- [ngx_stream_log_module](http://nginx.org/en/docs/stream/ngx_stream_log_module.html)
- [ngx_stream_map_module](http://nginx.org/en/docs/stream/ngx_stream_map_module.html)
- [ngx_stream_proxy_module](http://nginx.org/en/docs/stream/ngx_stream_proxy_module.html)
- [ngx_stream_realip_module](http://nginx.org/en/docs/stream/ngx_stream_realip_module.html)
- [ngx_stream_return_module](http://nginx.org/en/docs/stream/ngx_stream_return_module.html)
- [ngx_stream_split_clients_module](http://nginx.org/en/docs/stream/ngx_stream_split_clients_module.html)
- [ngx_stream_ssl_module](http://nginx.org/en/docs/stream/ngx_stream_ssl_module.html)
- [ngx_stream_ssl_preread_module](http://nginx.org/en/docs/stream/ngx_stream_ssl_preread_module.html)
- [ngx_stream_upstream_module](http://nginx.org/en/docs/stream/ngx_stream_upstream_module.html)
- [ngx_stream_upstream_hc_module](http://nginx.org/en/docs/stream/ngx_stream_upstream_hc_module.html)
- [ngx_stream_zone_sync_module](http://nginx.org/en/docs/stream/ngx_stream_zone_sync_module.html)
- [ngx_google_perftools_module](http://nginx.org/en/docs/ngx_google_perftools_module.html)



