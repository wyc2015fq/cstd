# Nginx精简真实案例配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月29日 10:32:49[boonya](https://me.csdn.net/boonya)阅读数：3616








### 1.nginx.conf配置

Nginx的简单配置，nginx.conf文件：



```python
#user  nobody;
worker_processes  1;

#error_log  logs/error.log;
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

    sendfile        on;
    #tcp_nopush     on;

    #keepalive_timeout  0;
    keepalive_timeout  65;

    #gzip  on;
    
    upstream wms.50yc.com{
        server localhost:8090;
    }
    upstream yxwms.50yc.com{
        server localhost:8077;
    }
    
    server {
        listen       80;
        server_name  wms.50yc.com;

        location / {
            index index.html;
            proxy_pass http://wms.50yc.com;
        }

        error_page  404              /404.html;

        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
    }

    server {
        listen       80;
        server_name  yxwms.50yc.com;

        location / {
            index index.html;
            proxy_pass http://yxwms.50yc.com;
        }

        error_page  404              /404.html;

        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
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




### 2.真实案例访问地址

物联云仓：[http://wms.50yc.com/](http://wms.50yc.com/)

客户环境：[http://yxwms.50yc.com/admin.html](http://yxwms.50yc.com/admin.html)


注：由于真实服务的原因所以上面涉及到的host都改成了localhost(实际上公网IP也是可以被解析出来的，还是不贴IP为妙)。

![](https://img-blog.csdn.net/20160629104418702)




