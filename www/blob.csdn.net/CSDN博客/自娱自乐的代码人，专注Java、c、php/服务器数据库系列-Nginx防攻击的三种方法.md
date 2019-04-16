# 服务器数据库系列 - Nginx防攻击的三种方法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年04月17日 09:18:35[initphp](https://me.csdn.net/initphp)阅读数：7766








**1. ngx_http_limit_conn_module 可以用来限制单个IP的连接数：**



`ngx_http_limit_conn_module` 模块可以按照定义的键限定每个键值的连接数。特别的，可以设定单一 IP 来源的连接数。

并不是所有的连接都会被模块计数；只有那些正在被处理的请求（这些请求的头信息已被完全读入）所在的连接才会被计数。

#### 配置范例



> http {
    limit_conn_zone $binary_remote_addr zone=addr:10m;

    ...

    server {

        ...

        location /download/ {
            limit_conn addr 1;
        }




#### 指令

|语法:|`**limit_conn** `*zone*``*number*`;`|
|----|----|
|默认值:|—|
|上下文:|`http`, `server`, `location`|


指定一块已经设定的共享内存空间，以及每个给定键值的最大连接数。当连接数超过最大连接数时，服务器将会返回503 (Service Temporarily Unavailable)错误。比如，如下配置

> limit_conn_zone $binary_remote_addr zone=addr:10m;

server {
    location /download/ {
        limit_conn addr 1;
    }


表示，同一 IP 同一时间只允许有一个连接。

当多个 `limit_conn` 指令被配置时，所有的连接数限制都会生效。比如，下面配置不仅会限制单一IP来源的连接数，同时也会限制单一虚拟服务器的总连接数：

> limit_conn_zone $binary_remote_addr zone=perip:10m;
limit_conn_zone $server_name zone=perserver:10m;

server {
    ...
    limit_conn perip 10;
    limit_conn perserver 100;
}




如果当前配置层级没有`limit_conn`指令，将会从更高层级继承连接限制配置。

|语法:|`**limit_conn_log_level** `info` |`notice` |`warn` |`error`;`|
|----|----|
|默认值:|limit_conn_log_level error;|
|上下文:|`http`, `server`, `location`|

这个指令出现在版本 0.8.18. 


指定当连接数超过设定的最大连接数，服务器限制连接时的日志等级。

|语法:|```**limit_conn_zone** `*$variable*````zone```=`*name*`:`*size*`;```|
|----|----|
|默认值:|—|
|上下文:|`http`|


设定保存各个键的状态的共享内存空间的参数。键的状态中保存了当前连接数。键的值可以是特定变量的任何非空值（空值将不会被考虑）。使用范例：

> limit_conn_zone $binary_remote_addr zone=addr:10m;


这里，设置客户端的IP地址作为键。注意，这里使用的是`$binary_remote_addr`变量，而不是`$remote_addr`变量。`$remote_addr`变量的长度为7字节到15字节不等，而存储状态在32位平台中占用32字节或64字节，在64位平台中占用64字节。而`$binary_remote_addr`变量的长度是固定的4字节，存储状态在32位平台中占用32字节或64字节，在64位平台中占用64字节。一兆字节的共享内存空间可以保存3.2万个32位的状态，1.6万个64位的状态。如果共享内存空间被耗尽，服务器将会对后续所有的请求返回503
 (Service Temporarily Unavailable)错误。

|语法:|`**limit_zone** `*name*``*$variable*``*size*`;`|
|----|----|
|默认值:|—|
|上下文:|`http`|


这条指令在 1.1.8 版本中已经被废弃，应该使用等效的[limit_conn_zone](http://nginx.org/cn/docs/http/ngx_http_limit_conn_module.html#limit_conn_zone)指令。该指令的语法也有变化：

> 
`limit_conn_zone``*$variable*``zone`=`*name*`:`*size*`;









**2. ngx_http_limit_req_module 可以用来限制单个IP每秒请求数**



`ngx_http_limit_req_module`模块(0.7.21)可以通过定义的键值来限制请求处理的频率。特别的，它可以限制来自单个IP地址的请求处理频率。限制的方法是通过一种“漏桶”的方法——固定每秒处理的请求数，推迟过多的请求处理。

#### 配置示例



> http {
    limit_req_zone $binary_remote_addr zone=one:10m rate=1r/s;

    ...

    server {

        ...

        location /search/ {
            limit_req zone=one burst=5;
        }




#### 指令

|语法:|`**limit_req** `zone`=`*name*` [`burst`=`*number*`] [`nodelay`];`|
|----|----|
|默认值:|—|
|上下文:|`http`, `server`, `location`|


设置对应的共享内存限制域和允许被处理的最大请求数阈值。如果请求的频率超过了限制域配置的值，请求处理会被延迟，所以所有的请求都是以定义的频率被处理的。超过频率限制的请求会被延迟，直到被延迟的请求数超过了定义的阈值这时，这个请求会被终止，并返回503 (Service Temporarily Unavailable)错误。这个阈值的默认值等于0。比如这些指令：

> limit_req_zone $binary_remote_addr zone=one:10m rate=1r/s;

server {
    location /search/ {
        limit_req zone=one burst=5;
    }


限制平均每秒不超过一个请求，同时允许超过频率限制的请求数不多于5个。

如果不希望超过的请求被延迟，可以用`nodelay`参数：

> limit_req zone=one burst=5 nodelay;




|语法:|`**limit_req_log_level** `info` |`notice` |`warn` |`error`;`|
|----|----|
|默认值:|limit_req_log_level error;|
|上下文:|`http`, `server`, `location`|

这个指令出现在版本 0.8.18. 


设置你所希望的日志级别，当服务器因为频率过高拒绝或者延迟处理请求时可以记下相应级别的日志。延迟记录的日志级别比拒绝的低一个级别；比如，如果设置“`limit_req_log_level notice`”，延迟的日志就是`info`级别。

|语法:|`**limit_req_zone** `*$variable*``zone`=`*name*`:`*size*``rate`=`*rate*`;`|
|----|----|
|默认值:|—|
|上下文:|`http`|


设置一块共享内存限制域的参数，它可以用来保存键值的状态。它特别保存了当前超出请求的数量。键的值就是指定的变量（空值不会被计算）。示例用法：

> limit_req_zone $binary_remote_addr zone=one:10m rate=1r/s;




这里，状态被存在名为“one”，最大10M字节的共享内存里面。对于这个限制域来说平均处理的请求频率不能超过每秒一次。

键值是客户端的IP地址。如果不使用`$remote_addr`变量，而用`$binary_remote_addr`变量，可以将每条状态记录的大小减少到64个字节，这样1M的内存可以保存大约1万6千个64字节的记录。如果限制域的存储空间耗尽了，对于后续所有请求，服务器都会返回503 (Service Temporarily Unavailable)错误。

请求频率可以设置为每秒几次（r/s）。如果请求的频率不到每秒一次，你可以设置每分钟几次(r/m)。比如每秒半次就是30r/m。




**3. nginx_limit_speed_module 可以用来对IP限速**

https://github.com/yaoweibin/nginx_limit_speed_module




