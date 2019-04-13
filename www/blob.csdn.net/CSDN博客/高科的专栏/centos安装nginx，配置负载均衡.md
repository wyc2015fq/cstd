
# centos安装nginx，配置负载均衡 - 高科的专栏 - CSDN博客

2018年04月19日 20:39:39[高科](https://me.csdn.net/pbymw8iwm)阅读数：220个人分类：[开发环境的配置																](https://blog.csdn.net/pbymw8iwm/article/category/1219848)


﻿﻿
1、安装nginx
安装教程，参照：[http://mp.weixin.qq.com/s/RVaRlRpHqZRjCaXGmOlfKw](http://mp.weixin.qq.com/s/RVaRlRpHqZRjCaXGmOlfKw)
2、反向代理的配置
修改部署目录下conf子目录的nginx.conf文件的内容

**[html]**[view plain](https://blog.csdn.net/csdn_ds/article/details/58605591#)[copy](https://blog.csdn.net/csdn_ds/article/details/58605591#)

location / {
\#设置主机头和客户端真实地址，以便服务器获取客户端真实IP
proxy_set_header Host $host;
\# nginx非80端口处理
\#proxy_set_header Host $host:$server_port;
\# 获取真实IP
proxy_set_header X-Real-IP $remote_addr;
\#获取代理者的真实ip
proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
\#禁用缓存
proxy_buffering off;
\#设置反向代理的地址
proxy_pass http://192.168.1.1;(根据实际情况修改)
}

3、负载均衡的配置
nginx 的 upstream默认是以轮询的方式实现负载均衡，这种方式中，每个请求按时间顺序逐一分配到不同的后端服务器，如果后端服务器down掉，能自动剔除。另外一种方式是ip_hash：每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题。
**[html]**[view plain](https://blog.csdn.net/csdn_ds/article/details/58605591#)[copy](https://blog.csdn.net/csdn_ds/article/details/58605591#)

\#user nobody;
worker_processes 2;
\#error_log logs/error.log;
\#error_log logs/error.log notice;
\#error_log logs/error.log info;
\#pid    logs/nginx.pid;

events {
accept_mutex on; \#设置网路连接序列化，防止惊群现象发生，默认为on
multi_accept on; \#设置一个进程是否同时接受多个网络连接，默认为off
worker_connections 1024;\#最大连接数
}

http {
include    mime.types;\#文件扩展名与文件类型映射表，此映射表主要用于部署在本nginx上的静态资源
default_type application/octet-stream;
\#日志格式
log_format main '$remote_addr - $remote_user [$time_local] "$request" '
'$status $body_bytes_sent "$http_referer" '
'"$http_user_agent" "$http_x_forwarded_for"';
access_log logs/access.log main;
sendfile    on;
\#tcp_nopush   on;
\#keepalive_timeout 0;
keepalive_timeout 65;\#连接超时时间
gzip on;
\#反向代理
\#【配置1】此配置是[配置4]和[配置5]的结合
\#此配置将请求转发到两个WEB服务器，根据客户端IP分配目标主机，同时按权重分配流量
upstream app1 {
ip_hash;
server 192.168.14.132:8080weight=5;
server 192.168.14.133:80weight=3;
}
\#【配置2】
\#默认负载平衡配置,nginx应用HTTP负载平衡来分发请求。
\#upstream app1 {
\#  server 192.168.14.132:8080;
\#  server 192.168.14.133:80;
\#}
\#【配置3】
\#最小连接负载平衡配置,nginx将尽量不使用繁忙的服务器，而是将新请求分发给不太忙的服务器。
\#upstream app1 {
\#  least_conn;
\#  server 192.168.14.132:8080;
\#  server 192.168.14.133:80;
\#}
\#【配置4】
\#会话持久性配置，使用ip-hash，客户端的IP地址用作散列密钥，
\#以确定应为客户端请求选择服务器组中的哪个服务器。
\#此方法确保来自同一客户端的请求将始终定向到同一服务器，除非此服务器不可用。
\#upstream app1 {
\#  ip_hash;
\#  server 192.168.14.132:8080;
\#  server 192.168.14.133:80;
\#}
\#【配置5】
\#加权负载平衡配置，通过使用服务器权重进一步影响nginx负载平衡算法。
\#未配置权重的服务器，意味着所有指定的服务器被视为对特定负载平衡方法同等资格。
\#upstream app1 {
\#  ip_hash;
\#  server 192.168.14.132:8080weight=3;
\#  server 192.168.14.133:80weight=2;
\#  server 192.168.14.134:80;
\#  server 192.168.14.135:80;
\#}

server {\#可配置多个server以监听不同IP和不同端口
listen    80;\#监听的端口
server_name localhost;\#监听的服务器
\#charset koi8-r;
\#access_log logs/host.access.log main;
\#反斜杆代表所有连接，此配置目的是将所有连接交给名为app1的upstream代理，实现负载平衡
location / {
\#设置主机头和客户端真实地址，以便服务器获取客户端真实IP
proxy_set_header Host $host;
proxy_set_header X-Real-IP $remote_addr;
proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
\#禁用缓存
proxy_buffering off;
\#反向代理的配置（地址按实际情况配置）
proxy_pass http://app1;
}
\#图片文件路径，一般来说，静态文件会部署在本机以加快响应速度
\#可配置多个这样的location，满足各种需求
location ~\.(gif|jpg|png)$ {
root /home/root/images;
}
location ~\.(iso|zip|txt|doc|docx)$ {
root /home/root/files;
}

\#error_page 404       /404.html;
\# redirect server error pages to the static page /50x.html
\#
error_page  500 502 503 504 /50x.html;
location= /50x.html {
root  html;
}

\# FastCGI是CGI全称是“公共网关接口”(Common Gateway Interface)
\#对于我来说，使用Tomcat代替即可，请忽略此配置。
\#location ~ \.php$ {
\#  root      html;
\#  fastcgi_pass  127.0.0.1:9000;
\#  fastcgi_index index.php;
\#  fastcgi_param SCRIPT_FILENAME /scripts$fastcgi_script_name;
\#  include    fastcgi_params;
\#}
\# 添加黑名单，禁止某某访问特定文件
\# concurs with nginx's one
\#
\#location ~ /\.ht {
\#  deny all;
\#}
}

\# another virtual host using mix of IP-, name-, and port-based configuration
\#
\#server {
\#  listen    8000;
\#  listen    somename:8080;
\#  server_name somename alias another.alias;
\#  location / {
\#    root  html;
\#    index index.html index.htm;
\#  }
\#}

\# HTTPS server
\#
\#server {
\#  listen    443 ssl;
\#  server_name localhost;
\#  ssl_certificate   cert.pem;
\#  ssl_certificate_key cert.key;
\#  ssl_session_cache  shared:SSL:1m;
\#  ssl_session_timeout 5m;
\#  ssl_ciphers HIGH:!aNULL:!MD5;
\#  ssl_prefer_server_ciphers on;
\#  location / {
\#    root  html;
\#    index index.html index.htm;
\#  }
\#}
}
4、配置完后，记得执行以下命令生效配置
**[html]**[view plain](https://blog.csdn.net/csdn_ds/article/details/58605591#)[copy](https://blog.csdn.net/csdn_ds/article/details/58605591#)

nginx -s reload
5、Nginx内置变量含义
**[html]**[view plain](https://blog.csdn.net/csdn_ds/article/details/58605591#)[copy](https://blog.csdn.net/csdn_ds/article/details/58605591#)

名称    版本    说明（变量列表来源于文件 ngx_http_variables ）
$args    1.0.8    请求中的参数;
$binary_remote_addr    1.0.8    远程地址的二进制表示
$body_bytes_sent    1.0.8    已发送的消息体字节数
$content_length    1.0.8    HTTP请求信息里的"Content-Length";
$content_type    1.0.8    请求信息里的"Content-Type";
$document_root    1.0.8    针对当前请求的根路径设置值;
$document_uri    1.0.8    与$uri相同;
$host    1.0.8    请求信息中的"Host"，如果请求中没有Host行，则等于设置的服务器名;
$hostname    1.0.8
$http_cookie    1.0.8    cookie 信息
$http_post    1.0.8
$http_referer    1.0.8    引用地址
$http_user_agent    1.0.8    客户端代理信息
$http_via    1.0.8     最后一个访问服务器的Ip地址。
$http_x_forwarded_for    1.0.8     相当于网络访问路径。
$is_args    1.0.8
$limit_rate    1.0.8    对连接速率的限制;
$nginx_version    1.0.8
$pid    1.0.8
$query_string    1.0.8    与$args相同;
$realpath_root    1.0.8
$remote_addr    1.0.8    客户端地址;
$remote_port    1.0.8    客户端端口号;
$remote_user    1.0.8    客户端用户名，认证用;
$request    1.0.8    用户请求
$request_body    1.0.8
$request_body_file    1.0.8    发往后端的本地文件名称
$request_completion    1.0.8
$request_filename    1.0.8    当前请求的文件路径名
$request_method    1.0.8    请求的方法，比如"GET"、"POST"等;
$request_uri    1.0.8    请求的URI，带参数;
...


