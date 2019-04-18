# Nginx防攻击工具教程一 ngx_http_limit_conn_module - weixin_33985507的博客 - CSDN博客
2013年04月17日 13:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
   要限制用户的连接数可以通过Limit zone模块来达到目的，即限制同一用户IP地址的并发连接数。
该模块提供了两个命令limit_zone和limit_conn，其中limit_zone只能用在http区段，而limit_conn可以用在*http, [**server**](http://space.itpub.net/27043155/viewspace-732627), location*区段。
## 示例配置
```
http {
 limit_zone  one $binary_remote_addr 10m;
 
 server {
   location /download/ {
     limit_conn  one 1;
   }
 }
}
```
该模块提供了以下3条指令：
指令名称：limit_zone
    功   能：该指令用于定义一个zone，该zome将会被用于存储会话状态。能够存储的会话数量是由分被交付的变量和memory_max_size的大小决定的。
    例如：
limit_zone one $binary_remote_addr10m;
   客户端的IP地址被用作会话，注意，这里使用的是$binary_remote_addr而不是$remote_addr，这是因为，$remote_addr的长度为7到15个字节，它的会话信息的长度为32或64 bytes；$binary_remote_addr的长度为4字节，会话信息的长度为32字节。当设置1M的一个zone时，那么如果是用$binary_remote_addr方式，该zone将会存放32000个会话。
语   法: limit_zone zone_name $variable memory_max_size
默 认 值: no
使用环境: http
**指令名称：limit_conn**
功   能：该指令用于为一个会话设定最大的并发连接数。如果并发请求数超过这个限制，那么将会出现"Service unavailable" (503)。
例如：
```
limit_zone  one $binary_remote_addr 10m;
 
server {
 location /download/ {
 limit_conn  one 1;
}
```
这个设置将会使得来自用同一个IP的并发连接不能超过1个连接。
语   法：: limit_conn zone_name max_clients_per_ip
默 认 值: no
使用环境: http, server, location
**指令名称：limit_conn_log_level**
语   法: limit_conn_log_level info | notice | warn | error
默 认 值: error
使用环境: http, server, location
功   能：该指令用于设置日志的错误级别，当达到连接限制时，将会产生错误日志。
## 配置实例
看下面的一个例子：
```
root@汪翰翔 conf]# cat nginx.conf
 
worker_processes 4;
 
events {
   worker_connections 10240;
}
 
http {
   include      mime.types;
   default_type application/octet-stream;
   sendfile          on;
   keepalive_timeout 65;
   limit_zone  flv_down $binary_remote_addr 10m;
 
   server {
       listen      80;
       server_name flv.xxx.com;
……
       location /download {
              limit_conn  flv_down 1;                     
       }
 
       error_page  500 502 503 504 /50x.html;
       location = /50x.html {
           root  html;
       }
```
在这个例子中，我们使用了两个命令，limit_zone和limit_conn。
