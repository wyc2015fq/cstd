# nginx 基本配置说明 - weixin_33985507的博客 - CSDN博客
2013年10月14日 11:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
```
1. Apache服务器和nginx的优缺点：
我们之前大量使用Apache来作为HTTPServer。
Apache具有很优秀的性能，而且通过模块可以提供各种丰富的功能。
1)首先Apache对客户端的响应是支持并发的 ，运行httpd这个daemon进程之后，它会同时产生多个孩子进程/线程，每个孩子进程/线程分别对客户端的请求进行响应；
2)另外，Apache可以提供静态和动态的服务 ，例如对于PHP的解析不是通过性能较差的CGI实现的而是通过支持PHP的模块来实现的(通常为mod_php5，或者叫做apxs2)。
3)缺点:
因此通常称为Apache的这种Server为process-based server ，也就是基于多进程的HTTPServer，因为它需要对每个用户请求创建一个孩子进程/线程进行响应；
这样的缺点是，如果并发的请求非常多(这在大型门户网站是很常见的)就会需要非常多的线程，从而占用极多的系统资源CPU和内存。因此对于并发处理不是Apache的强项。
4)解决方法：
目前来说出现了另一种WebServer，在并发方面表现更加优越，叫做asynchronous servers异步服务器。最有名的为Nginx和Lighttpd。所谓的异步服务器是事件驱动程序模式的event-driven，除了用户的并发请求通常只需要一个单一的或者几个线程。因此占用系统资源就非常少。这几种又被称为lightweight web server。
举例，对于10,000的并发连接请求，nginx可能仅仅使用几M的内存；而Apache可能需要使用几百M的内存资源。
2. 实际中单一的使用：
1)关于单一使用Apache来作为HTTPServer的情况我们不用再多做介绍，非常常见的应用；
上面我们介绍到Apache对于PHP等服务器端脚本的支持是通过自己的模块来实现的，而且性能优越。
2)我们同样可以单单使用nginx或者lighttpd来作为HTTPServer来使用。
nginx和lighttpd和Apache类似都通过各种模块可以对服务器的功能进行丰富的扩展，同样都是通过conf配置文件对各种选项进行配置。
对于PHP等，nginx和lighttpd都没有内置的模块来对PHP进行支持，而是通过FastCGI来支持的。
Lighttpd通过模块可以提供CGI, FastCGI和SCGI等服务，Lighttpd is capable of automatically spawning FastCGI backends as well as using externally spawned processes.
nginx则没有自己提供处理PHP的功能，需要通过第三方的模块来提供对PHP进行FastCGI方式的集成。
 
 
 
------------------------------- rewrites 所有非www.***.com的访问 => http://www.***.com/
 server_name   web90.***.com;
 
 if ($host = "web90.***.com") {
                rewrite ^(.*)$ http://www.test.com$1 permanent;
        }
 
---------------------------------nginx 停止/平滑重启#p#分页标题#e#
nginx的信号控制
TERM,INT 快速关闭
QUIT  从容关闭
HUP   平滑重启，重新加载配置文件
USR1  重新打开日志文件，在切割日志时用途比较大
USR2  平滑升级可执行程序
WINCH 从容关闭工作进程
 
 
1） 从容停止：
kill -QUIT Nginx主进程号
kill -QUIT '/usr/local/webserver/nginx/logs/nginx.pid'
 
2）快速停止：
kill -TERM Nginx主进程号
kill -TERM '/usr/local/webserver/nginx/logs/nginx.pid'
kill -INTN ginx主进程号
kill -INT  '/usr/local/webserver/nginx/logs/nginx.pid'
 
3）强制停止所有nginx进程
pkill -9 nginx
 
 
 
1）平滑重启
kill -HUP nginx主进程号
kill -HUP '/usr/local/webserver/nginx/logs/nginx.pid'
 
 
-----------------------------nginx.conf
#p#分页标题#e#
 
worker_processes 8;
指定工作衍生进程数
一般等于cpu的总核数或总核数的两倍，例如两个四核的cpu，总核数为8
events
{
  use epoll; //使用的网络i/o模型，linux系统推荐epoll,freebsd推荐kqueue
  worker_connections 65535; //允许的链接数
}
 
 
location ~ .*\.(gif|jpg|jpeg|png|bmp|swf)$ { access_log off;关闭日志 expires 30d;//通过expires指令输出Header头来实现本地缓存，30天 } location ~ .*\.(js|css)$ { access_log off;关闭日志 expires 1h; } =====================每
{
access_log off;关闭日志
expires 30d;//通过expires指令输出Header头来实现本地缓存，30天
}
location ~ .*\.(js|css)$
{
access_log off;关闭日志
expires 1h;
}
 
 
=====================每天定时切割nginx日志脚本
vim /usr/local/webserver/nginx/sbin/cut_nginx_log.sh
#!/bin/bash
# This script run at 00:00
# The Nginx logs path
logs_path="/usr/local/webserver/nginx/logs/";
mkdir -p ${logs_path}$(date -d "yesterday" + "%Y")/$(date -d "yesterday" + "%m")/#p#分页标题#e#
mv ${logs_path}access.log ${logs_path}$(date -d "yesterday" + "%Y")/$(date -d "yesterday" + "%m")/access_$(date -d "yesterday" + "%Y%m%d").log
kill -USR1 'cat /usr/local/webserver/nginx/nginx.pid'
chown -R www:www cut_nginx_log.sh
chmod +x cut_nginx_log.sh
crontab -e
00 00 * * * /bin/bash /usr/local/webserver/nginx/sbin/cut_nginx_log.sh
#/sbin/service crond restart
 
－－－－－－－－－－－－－－nginx 配置 gzip压缩
 
一般情况下压缩后的html、css、js、php、jhtml等文件，大小能降至原来的25%，也就是说，原本一个100k的html，压缩后只剩下25k。这无疑能节省很多带宽，也能降低服务器的负载。
在nginx中配置gzip比较简单
一般情况下只要在nginx.conf的http段中加入下面几行配置即可
引用
   gzip  on;
   gzip_min_length  1000;
   gzip_buffers     4 8k;
   gzip_types       text/plain application/x-javascript text/css text/html application/xml;
重启nginx
可以通过网页gzip检测工具来检测网页是否启用了gzip
http://gzip.zzbaike.com/
 
 
 
---------------重定向nginx错误页面的方法
error_page 404  /404.html;
这个404.html保证在nginx主目录下的html目录中即可，如果需要在出现404错误后直接跳转到另外一个地址，可以直接设置如下：
error_page 404 http://www.***.net ;
同样的方式可以定义常见的403、500等错误。#p#分页标题#e#
特别注意的是404.html文件页面大小要超过512k，不然会被ie浏览器替换为ie默认的错误页面。
 
 
------------------------------虚拟主机配置
server {
    listen   80;
    server_name  localhost;
    access_log  /var/log/nginx/localhost.access.log;
    location / {
        root   /var/www/nginx-default;
        index index.php index.html index.htm;
    }
    location /doc {
        root   /usr/share;
        autoindex on;
        allow 127.0.0.1;
        deny all;
    }
    location /images {
        root   /usr/share;
        autoindex on;
    }
    location ~ \.php$ {
        fastcgi_pass   127.0.0.1:9000;
        fastcgi_index  index.php;
        fastcgi_param  SCRIPT_FILENAME  /var/www/nginx-default$fastcgi_script_name;
        include /etc/nginx/fastcgi_params;
    }
}
server {
    listen   80;
    server_name  sdsssdf.localhost.com;
    access_log  /var/log/nginx/localhost.access.log;
    location / {
        root   /var/www/nginx-default/console;
        index index.php index.html index.htm; } location /doc { root /usr/share; autoindex on; allow 127.0.0.1; deny all; } location /images { root /usr/share; autoindex on; } location ~ \.php$ { fastcgi_pass 127.0.0.1:9000; fastcgi_index index.p
#p#分页标题#e#
    }
    location /doc {
        root   /usr/share;
        autoindex on;
        allow 127.0.0.1;
        deny all;
    }
    location /images {
        root   /usr/share;
        autoindex on;
    }
    location ~ \.php$ {
        fastcgi_pass   127.0.0.1:9000;
        fastcgi_index  index.php;
        fastcgi_param  SCRIPT_FILENAME  /var/www/nginx-default$fastcgi_script_name;
        include /etc/nginx/fastcgi_params;
    }
}
 
----------------------监控  
 
location ~ ^/NginxStatus/ {
stub_status on; #Nginx 状态监控配置     
}
 
 
 
这样通过 http://localhost/NginxStatus/(最后的/不能掉) 监控到 Nginx 的运行信息:
 
Active connections: 1
server accepts handled requests
 1 1 5
Reading: 0 Writing: 1 Waiting: 0
 
 
NginxStatus 显示的内容意思如下：#p#分页标题#e#
    active connections – 当前 Nginx 正处理的活动连接数。
    server accepts handled requests -- 总共处理了 14553819 个连接 , 成功创建 14553819 次握手 ( 证明中间没有失败的 ), 总共处理了 19239266 个请求 ( 平均每次握手处理了 1.3 个数据请求 )。
    reading -- nginx 读取到客户端的 Header 信息数。
    writing -- nginx 返回给客户端的 Header 信息数。
    waiting -- 开启 keep-alive 的情况下，这个值等于 active - (reading + writing)，意思就是 Nginx 已经处理完正在等候下一次请求指令的驻留连接。
 
-------------------------------静态文件处理
通过正则表达式，我们可让 Nginx 识别出各种静态文件
 
location ~ \.(htm|html|gif|jpg|jpeg|png|bmp|ico|css|js|txt)$ {
        root /var/www/nginx-default/html;
        access_log off;
        expires 24h;
        }
对于例如图片、静态 HTML 文件、js 脚本文件和 css 样式文件等，我们希望 Nginx 直接处理并返回给浏览器，这样可以大大的加快网页浏览时的速度。因此对于这类文件我们需要通过 root 指令来指定文件的存放路径 ，同时因为这类文件并不常修改，通过 expires 指令来控制其在浏览器的缓存，以减少不必要的请求。 expires 指令可以控制 HTTP 应答中的“ Expires ”和“ Cache-Control ”的头标（起到控制页面缓存的作用）。您可以使用例如以下的格式来书写 Expires：
 
 
expires 1 January, 1970, 00:00:01 GMT;
expires 60s;
expires 30m;
expires 24h;
expires 1d;
expires max;
expires off;
 
 
这样当你输入http://192.168.200.100/1.html的时候会自动跳转到var/www/nginx-default/html/1.html
 
例如 images 路径下的所有请求可以写为：
 
 
 #p#分页标题#e#
 
location ~ ^/images/ {
    root /opt/webapp/images;
}
 
 
 
 
------------------------动态页面请求处理[集群]
Nginx 本身并不支持现在流行的 JSP、ASP、PHP、PERL 等动态页面，但是它可以通过反向代理将请求发送到后端的服务器，例如 Tomcat、Apache、IIS 等来完成动态页面的请求处理。前面的配置示例中，我们首先定义了由 Nginx 直接处理的一些静态文件请求后，其他所有的请求通过 proxy_pass 指令传送给后端的服务器 （在上述例子中是 Tomcat）。最简单的 proxy_pass 用法如下：
 
location / { proxy_pass http://localhost:8080; proxy_set_header X-Real-IP $remote_addr; } 这里我们没有使用到集群，而是将请求直接送到运行在 8080 端口的 Tomcat 服务上来完成类似 JSP
    proxy_pass        http://localhost:8080;
    proxy_set_header  X-Real-IP  $remote_addr;
}
 
 
这里我们没有使用到集群，而是将请求直接送到运行在 8080 端口的 Tomcat 服务上来完成类似 JSP 和 Servlet 的请求处理。
当页面的访问量非常大的时候，往往需要多个应用服务器来共同承担动态页面的执行操作，这时我们就需要使用集群的架构。 Nginx 通过 upstream 指令来定义一个服务器的集群，最前面那个完整的例子中我们定义了一个名为 tomcats 的集群，这个集群中包括了三台服务器共 6 个 Tomcat 服务。而 proxy_pass 指令的写法变成了：
 
 
 
# 集群中的所有后台服务器的配置信息
    upstream tomcats {
     server 192.168.0.11:8080 weight=10;
     server 192.168.0.11:8081 weight=10;
     server 192.168.0.12:8080 weight=10;
     server 192.168.0.12:8081 weight=10;
     server 192.168.0.13:8080 weight=10;
     server 192.168.0.13:8081 weight=10;#p#分页标题#e#
    }
    location / {
        proxy_pass http://tomcats;# 反向代理
        include proxy.conf;
        }
 
 
----------------------压力测试
wget http://blog.s135.com/soft/linux/webbench/webbench-1.5.tar.gz  
tar zxvf webbench-1.5.tar.gz  
cd webbench-1.5  
make && make install
#webbench -c 100 -t 10 http://192.168.200.100/info.php
参数说明：-c表示并发数，-t表示持续时间(秒)
 
 
root@ubuntu-desktop:/etc/nginx/sites-available# webbench -c 100 -t 10 http://192.168.200.100/info.php
Webbench - Simple Web Benchmark 1.5
Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software.
Benchmarking: GET http://192.168.200.100/info.php
100 clients, running 10 sec.
Speed=19032 pages/min, 18074373 bytes/sec.
Requests: 3172 susceed, 0 failed.
 
 
 
 
 
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－PPC提供nginx详细配置说明
 
#运行用户
user  nobody nobody;
#启动进程
worker_processes  2;#p#分页标题#e#
#全局错误日志及PID文件
error_log  logs/error.log notice;
pid        logs/nginx.pid;
#工作模式及连接数上限
events{use epoll;
worker_connections      1024;}#设定http服务器，利用它的反向代理功能提供负载均衡支持
http{#设定mime类型
            include      conf/mime.types;
            default_type  application/octet-stream;
            #设定日志格式
            log_format main'$remote_addr - $remote_user [$time_local] ''"$request" $status $bytes_sent ''"$http_referer" "$http_user_agent" ''"$gzip_ratio"';
            log_format download'$remote_addr - $remote_user [$time_local] ''"$request" $status $bytes_sent ''"$http_referer" "$http_user_agent" ''"$http_range" "$sent_http_content_range"';
            #设定请求缓冲
            client_header_buffer_size    1k;
            large_client_header_buffers  4 4k;
           
            #开启gzip模块
            gzip on;
            gzip_min_length  1100; 
            gzip_buffers    4 8k;
            gzip_types      text/plain;
            output_buffers  1 32k;
            postpone_output  1460;
           
            #设定access log
            access_log  logs/access.log  main;
            client_header_timeout  3m;
            client_body_timeout    3m;
            send_timeout          3m;
            sendfile                on;
            tcp_nopush              on;
            tcp_nodelay            on;
            keepalive_timeout  65;
           
            #设定负载均衡的服务器列表
            upstream mysvr{#weigth参数表示权值，权值越高被分配到的几率越大
                        #本机上的Squid开启3128端口#p#分页标题#e#
                        server 192.168.8.1:3128 weight=5;
                        server 192.168.8.2:80  weight=1;
                        server 192.168.8.3:80  weight=6;
            }
           
            #设定虚拟主机
            server{listen          80;
                        server_name    192.168.8.1 www.okpython.com;
                        charset gb2312;
                        #设定本虚拟主机的访问日志
                        access_log  logs/www.yejr.com.access.log  main;
                        #如果访问 /img/*, /js/*, /css/* 资源，则直接取本地文件，不通过squid
                        #如果这些文件较多，不推荐这种方式，因为通过squid的缓存效果更好
                        location ~ ^/(img|js|css)/  {
                                    root    /data3/Html;
                                    expires 24h;
                        } #对 / 启用负载均衡 location / { proxy_pass http://mysvr; proxy_redirect off; proxy_set_header Host $host; proxy_set_header X-Real-IP $remote_addr; proxy_set_header X-Forwarded-For $proxy_add_x_
                        #对 "/" 启用负载均衡
                        location / {
                                    proxy_pass      http://mysvr;
                                    proxy_redirect          off;
                                    proxy_set_header        Host $host;
                                    proxy_set_header        X-Real-IP $remote_addr;
                                    proxy_set_header        X-Forwarded-For $proxy_add_x_forwarded_for;
                                    client_max_body_size    10m;
                                    client_body_buffer_size 128k;
                                    proxy_connect_timeout  90;#p#分页标题#e#
                                    proxy_send_timeout      90;
                                    proxy_read_timeout      90;
                                    proxy_buffer_size      4k;
                                    proxy_buffers          4 32k;
                                    proxy_busy_buffers_size 64k;
                                    proxy_temp_file_write_size 64k;
                        }
                        #设定查看Nginx状态的地址
                        location /NginxStatus {
                                    stub_status            on;
                                    access_log              on; 
                                    auth_basic              "NginxStatus";
                                    auth_basic_user_file  conf/htpasswd;   ＃conf/htpasswd 文件的内容用 apache 提供的 htpasswd 工具来产生即可
                  }
      }
}
```
