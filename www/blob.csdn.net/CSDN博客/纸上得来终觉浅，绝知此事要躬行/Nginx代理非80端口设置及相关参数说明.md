# Nginx代理非80端口设置及相关参数说明 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年05月25日 14:31:07[boonya](https://me.csdn.net/boonya)阅读数：1548








﻿﻿

From：[http://blog.csdn.net/luckykapok918/article/details/44461465](http://blog.csdn.net/luckykapok918/article/details/44461465)

Nginx通过80端口反向代理到Tomcat实现很简单，通过Jsp的request.getServerPort()获取到的端口号依然是80，而如果Nginx使用非80端口做反响代理时request.getServerPort()获取到的端口号返回依然会是80，这样边无法正确response.sendRedirect

> 

String path = request.getContextPath(); 

String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";


这样response.sendRedirect会跳转到80端口去



解决办法：


修改proxy.conf获取你把代理的配置写到了nginx内，也可以在其内部修改

> 

proxy_redirect          off;


# nginx非80端口处理 

proxy_set_header        Host $host:$server_port; 

# 获取真实IP 

proxy_set_header        X-Real-IP $remote_addr; 

# 获取代理者的真实ip 

proxy_set_header       X-Forwarded-For   $proxy_add_x_forwarded_for; 

client_max_body_size    10m; 

client_body_buffer_size 128k; 

proxy_connect_timeout   90; 

proxy_send_timeout      90; 

proxy_read_timeout      90; 

proxy_buffer_size       4k; 

proxy_buffers           4 32k; 

proxy_busy_buffers_size 64k; 

proxy_temp_file_write_size 64k;



nginx vhost内虚拟主机代理配置

> 

location ~* "\.(jsp|do)$" { 

        proxy_pass [http://kinggoo.ts;](/)


# 使用include引入就可以了 

        include proxy.conf; 

}


实例：



如果nginx的监听端口不是默认的80端口，改为其他端口如8181端口。

后端服务器中request.getServerPort()无法获得正确的端口，返回的仍然是80；

在response.sendRedirect()时，客户端可能无法获得正确的重定向url。

正确的配置方法为

在 $host之后加上端口号，如$host:8181

**server {        listen       8181;        server_name  localhost;**

**        location / {         proxy_pass **[**http://147.16.24.175:9500**](http://147.16.24.175:9500/)**;         proxy_set_header   Host             $host:8181;         proxy_set_header   X-Real-IP        $remote_addr;         proxy_set_header   X-Forwarded-For  $proxy_add_x_forwarded_for;**

**        } }**




参考帮助

Nginx内置变量含义

> 

名称    版本    说明（变量列表来源于文件 ngx_http_variables ） 

$args    1.0.8    请求中的参数; 

$binary_remote_addr    1.0.8    远程地址的二进制表示 

$body_bytes_sent    1.0.8    已发送的消息体字节数 

$content_length    1.0.8    HTTP请求信息里的"Content-Length"; 

$content_type    1.0.8    请求信息里的"Content-Type"; 

$document_root    1.0.8    针对当前请求的根路径设置值; 

$document_uri    1.0.8    与$uri相同; 

$host    1.0.8    请求信息中的"Host"，如果请求中没有Host行，则等于设置的服务器名; 

$hostname    1.0.8     

$http_cookie    1.0.8    cookie 信息 

$http_post    1.0.8     

$http_referer    1.0.8    引用地址 

$http_user_agent    1.0.8    客户端代理信息 

$http_via    1.0.8     最后一个访问服务器的Ip地址。 

$http_x_forwarded_for    1.0.8     相当于网络访问路径。 

$is_args    1.0.8     

$limit_rate    1.0.8    对连接速率的限制; 

$nginx_version    1.0.8     

$pid    1.0.8     

$query_string    1.0.8    与$args相同; 

$realpath_root    1.0.8     

$remote_addr    1.0.8    客户端地址; 

$remote_port    1.0.8    客户端端口号; 

$remote_user    1.0.8    客户端用户名，认证用; 

$request    1.0.8    用户请求 

$request_body    1.0.8     

$request_body_file    1.0.8    发往后端的本地文件名称 

$request_completion    1.0.8     

$request_filename    1.0.8    当前请求的文件路径名 

$request_method    1.0.8    请求的方法，比如"GET"、"POST"等; 

$request_uri    1.0.8    请求的URI，带参数; 

$scheme    1.0.8     所用的协议，比如http或者是https，比如rewrite^(.+)[Math Processing Error]scheme://example.com$1redirect; 

$sent_http_cache_control    1.0.8     

$sent_http_connection    1.0.8     

$sent_http_content_length    1.0.8     

$sent_http_content_type    1.0.8     

$sent_http_keep_alive    1.0.8     

$sent_http_last_modified    1.0.8     

$sent_http_location    1.0.8     

$sent_http_transfer_encoding    1.0.8     

$server_addr    1.0.8    服务器地址，如果没有用listen指明服务器地址，使用这个变量将发起一次系统调用以取得地址(造成资源浪费); 

$server_name    1.0.8    请求到达的服务器名; 

$server_port    1.0.8    请求到达的服务器端口号; 

$server_protocol    1.0.8    请求的协议版本，"HTTP/1.0"或"HTTP/1.1"; 

$uri    1.0.8    请求的URI，可能和最初的值有不同，比如经过重定向之类的 

名称    版本    说明（变量列表来源于文件 ngx_http_variables ） 

$args    1.0.8    请求中的参数; 

$binary_remote_addr    1.0.8    远程地址的二进制表示 

$body_bytes_sent    1.0.8    已发送的消息体字节数 

$content_length    1.0.8    HTTP请求信息里的"Content-Length"; 

$content_type    1.0.8    请求信息里的"Content-Type"; 

$document_root    1.0.8    针对当前请求的根路径设置值; 

$document_uri    1.0.8    与$uri相同; 

$host    1.0.8    请求信息中的"Host"，如果请求中没有Host行，则等于设置的服务器名; 

$hostname    1.0.8     

$http_cookie    1.0.8    cookie 信息 

$http_post    1.0.8     

$http_referer    1.0.8    引用地址 

$http_user_agent    1.0.8    客户端代理信息 

$http_via    1.0.8     最后一个访问服务器的Ip地址。 

$http_x_forwarded_for    1.0.8     相当于网络访问路径。 

$is_args    1.0.8     

$limit_rate    1.0.8    对连接速率的限制; 

$nginx_version    1.0.8     

$pid    1.0.8     

$query_string    1.0.8    与$args相同; 

$realpath_root    1.0.8     

$remote_addr    1.0.8    客户端地址; 

$remote_port    1.0.8    客户端端口号; 

$remote_user    1.0.8    客户端用户名，认证用; 

$request    1.0.8    用户请求 

$request_body    1.0.8     

$request_body_file    1.0.8    发往后端的本地文件名称 

$request_completion    1.0.8     

$request_filename    1.0.8    当前请求的文件路径名 

$request_method    1.0.8    请求的方法，比如"GET"、"POST"等; 

$request_uri    1.0.8    请求的URI，带参数; 

$scheme    1.0.8     所用的协议，比如http或者是https，比如rewrite^(.+)[Math Processing Error]scheme://example.com$1redirect; 

$sent_http_cache_control 1.0.8  

$sent_http_connection 1.0.8  

$sent_http_content_length 1.0.8  

$sent_http_content_type 1.0.8  

$sent_http_keep_alive 1.0.8  

$sent_http_last_modified 1.0.8  

$sent_http_location 1.0.8  

$sent_http_transfer_encoding 1.0.8  

$server_addr 1.0.8 服务器地址，如果没有用listen指明服务器地址，使用这个变量将发起一次系统调用以取得地址(造成资源浪费); 

$server_name 1.0.8 请求到达的服务器名; 

$server_port 1.0.8 请求到达的服务器端口号; 

$server_protocol 1.0.8 请求的协议版本，"HTTP/1.0"或"HTTP/1.1"; 

$uri 1.0.8 请求的URI，可能和最初的值有不同，比如经过重定向之类的




