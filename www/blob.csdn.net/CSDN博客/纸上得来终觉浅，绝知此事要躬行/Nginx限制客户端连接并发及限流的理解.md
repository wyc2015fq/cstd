# Nginx限制客户端连接并发及限流的理解 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年03月13日 15:27:05[boonya](https://me.csdn.net/boonya)阅读数：61标签：[Nginx																[limit_conn_zone](https://so.csdn.net/so/search/s.do?q=limit_conn_zone&t=blog)](https://so.csdn.net/so/search/s.do?q=Nginx&t=blog)
个人分类：[Nginx](https://blog.csdn.net/boonya/article/category/2126311)








限流错误理解：使用Nginx限制连接或者请求的带宽，通过很多尝试那个带宽值本地测试不出来影响关系。

Nginx限流的对象：客户端的IP请求的并发连接数和客户端的IP每秒允许的请求数。

对于网卡而言，我们收到的请求并发数量并未减少，只是Nginx帮我们处理了并发过多的问题，以确保我们后端的服务能够正常。

## Nginx限URL请求连接并发数

```bash
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

#pid        logs/nginx.pid;


events {
    worker_connections  1024;
}


http {
    include       mime.types;
    default_type  application/octet-stream;

    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';

    #access_log  logs/access.log  main;

    #负载均衡
    #upstream xhtserver {
    #  server 192.168.1.111:8080;
    #  server 192.168.1.111:8088;

    #}

    sendfile        on;
    #tcp_nopush     on;

    #keepalive_timeout  0;
    keepalive_timeout  90;

    #gzip  on;

    limit_conn_zone $binary_remote_addr  zone=upload:10m;


    server {
        listen      18080;
        server_name  server-a;
        #charset koi8-r;

        #access_log  logs/host.access.log  main;

        #location / {
        #    root   html;
        #    index  index.html index.htm;
        #}

        #转发到:http://192.168.1.111:28080/webService/uploadAttachWithoutToken.do

        location ^~ /webService/uploadAttachWithoutToken.do {
            limit_conn  upload 300;
            limit_rate  32k;
            proxy_pass  http://192.168.1.111:28080;
        }


        location / {
            root   html;
            index  home.html home.htm;
            proxy_pass http://192.168.1.111:8080;
        }

        #error_page  404              /404.html;

        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }

        # proxy the PHP scripts to Apache listening on 127.0.0.1:80
        #
        #location ~ \.php$ {
        #    proxy_pass   http://127.0.0.1;
        #}

        # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           html;
        #    fastcgi_pass   127.0.0.1:9000;
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
        #    include        fastcgi_params;
        #}

        # deny access to .htaccess files, if Apache's document root
        # concurs with nginx's one
        #
        #location ~ /\.ht {
        #    deny  all;
        #}
    }


    # another virtual host using mix of IP-, name-, and port-based configuration
    #
    #server {
    #    listen       8000;
    #    listen       somename:8080;
    #    server_name  somename  alias  another.alias;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}


    # HTTPS server
    #
    #server {
    #    listen       443 ssl;
    #    server_name  localhost;

    #    ssl_certificate      cert.pem;
    #    ssl_certificate_key  cert.key;

    #    ssl_session_cache    shared:SSL:1m;
    #    ssl_session_timeout  5m;

    #    ssl_ciphers  HIGH:!aNULL:!MD5;
    #    ssl_prefer_server_ciphers  on;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}

}
```

注意：location规则设置-正则表达式，以及location放的先后顺序都会对配置的结果产生影响。




