# nginx windows rtmp 服务器搭建 - tifentan的专栏 - CSDN博客

2017年11月01日 15:02:26[露蛇](https://me.csdn.net/tifentan)阅读数：306


Windows机器配置:
Windows7旗舰版 64位
Intel(R) Core(TM)i5-2520 CPU @2.50GHz 2.50 GHz
内存: 4GB
​
1. 下载 nginx 1.7.11.3 Gryphon
下载链接: http://nginx-win.ecsds.eu/download/nginx 1.7.11.3 Gryphon.zip
下载完成后解压；
将解压后的目录名:
  nginx 1.7.11.3 Gryphon
改成:
  nginx-1.7.11.3-Gryphon

2. 下载服务器状态检查程序 stat.xsl
https://github.com/arut/nginx-rtmp-module/

将nginx-rtmp-module-master.zip解压后复制到目录:nginx-1.7.11.3-Gryphon下，
保证stat.xls的目录为:
nginx-1.7.11.3-Gryphon\nginx-rtmp-module\stat.xsl

3. 配置文件 conf\nginx-win-rtmp.conf 内容如下:

#user  nobody;
# multiple workers works !
worker_processes  2;

#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

#pid        logs/nginx.pid;

events {
    worker_connections  8192;
    # max value 32768, nginx recycling connections+registry optimization = 
    #   this.value * 20 = max concurrent connections currently tested with one worker
    #   C1000K should be possible depending there is enough ram/cpu power
    # multi_accept on;
}

rtmp {
    server {
        listen 1935;
        chunk_size 4000;
        application live {
             live on;
        }
    }
}

http {
    #include      /nginx/conf/naxsi_core.rules;
    include       mime.types;
    default_type  application/octet-stream;

    #log_format  main  '$remote_addr:$remote_port - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';

    #access_log  logs/access.log  main;

#     # loadbalancing PHP
#     upstream myLoadBalancer {
#         server 127.0.0.1:9001 weight=1 fail_timeout=5;
#         server 127.0.0.1:9002 weight=1 fail_timeout=5;
#         server 127.0.0.1:9003 weight=1 fail_timeout=5;
#         server 127.0.0.1:9004 weight=1 fail_timeout=5;
#         server 127.0.0.1:9005 weight=1 fail_timeout=5;
#         server 127.0.0.1:9006 weight=1 fail_timeout=5;
#         server 127.0.0.1:9007 weight=1 fail_timeout=5;
#         server 127.0.0.1:9008 weight=1 fail_timeout=5;
#         server 127.0.0.1:9009 weight=1 fail_timeout=5;
#         server 127.0.0.1:9010 weight=1 fail_timeout=5;
#         least_conn;
#     }

    sendfile        off;
    #tcp_nopush     on;

    server_names_hash_bucket_size 128;

## Start: Timeouts ##
    client_body_timeout   10;
    client_header_timeout 10;
    keepalive_timeout     30;
    send_timeout          10;
    keepalive_requests    10;
## End: Timeouts ##

    #gzip  on;

    server {
        listen       80;
        server_name  localhost;

        location /stat {
            rtmp_stat all;
            rtmp_stat_stylesheet stat.xsl;
        }
        location /stat.xsl {
            root nginx-rtmp-module/;
        }
        location /control {
            rtmp_control all;
        }

        #charset koi8-r;
        #access_log  logs/host.access.log  main;

        ## Caching Static Files, put before first location
        #location ~* \.(jpg|jpeg|png|gif|ico|css|js)$ {
        #    expires 14d;
        #    add_header Vary Accept-Encoding;
        #}

# For Naxsi remove the single # line for learn mode, or the ## lines for full WAF mode
        location / {
            #include    /nginx/conf/mysite.rules; # see also http block naxsi include line
            ##SecRulesEnabled;
         ##DeniedUrl "/RequestDenied";
         ##CheckRule "$SQL >= 8" BLOCK;
         ##CheckRule "$RFI >= 8" BLOCK;
         ##CheckRule "$TRAVERSAL >= 4" BLOCK;
         ##CheckRule "$XSS >= 8" BLOCK;
            root   html;
            index  index.html index.htm;
        }

# For Naxsi remove the ## lines for full WAF mode, redirect location block used by naxsi
        ##location /RequestDenied {
        ##    return 412;
        ##}

## Lua examples !
#         location /robots.txt {
#           rewrite_by_lua '
#             if ngx.var.http_host ~= "localhost" then
#               return ngx.exec("/robots_disallow.txt");
#             end
#           ';
#         }

        #error_page  404              /404.html;

        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }

        # proxy the PHP scripts to Apache listening on 127.0.0.1:80
        #
        #location ~ \.php$ {
        #    proxy_pass   http://127.0.0.1;
        #}

        # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           html;
        #    fastcgi_pass   127.0.0.1:9000; # single backend process
        #    fastcgi_pass   myLoadBalancer; # or multiple, see example above
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
        #    include        fastcgi_params;
        #}

        # deny access to .htaccess files, if Apache's document root
        # concurs with nginx's one
        #
        #location ~ /\.ht {
        #    deny  all;
        #}
    }

    # another virtual host using mix of IP-, name-, and port-based configuration
    #
    #server {
    #    listen       8000;
    #    listen       somename:8080;
    #    server_name  somename  alias  another.alias;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}

    # HTTPS server
    #
    #server {
    #    listen       443 ssl spdy;
    #    server_name  localhost;

    #    ssl                  on;
    #    ssl_certificate      cert.pem;
    #    ssl_certificate_key  cert.key;
    #    ssl_session_timeout  5m;
    #    ssl_prefer_server_ciphers On;
    #    ssl_protocols TLSv1 TLSv1.1 TLSv1.2;
    #    ssl_ciphers ECDH+AESGCM:ECDH+AES256:ECDH+AES128:ECDH+3DES:RSA+AESGCM:RSA+AES:RSA+3DES:!aNULL:!eNULL:!MD5:!DSS:!EXP:!ADH:!LOW:!MEDIUM;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}

}

4. 启动服务器
进入windows的cmd; 
> cd nginx-1.7.11.3-Gryphon
> nginx.exe -c conf\nginx-win-rtmp.conf

