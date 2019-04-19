# Tengine 常用模块使用介绍 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [JamesXin](http://www.jobbole.com/members/JamesXin) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## Tengine 和 Nginx
### Tengine简介
从2011年12月开始：Tengine是由淘宝网发起的Web服务器项目。它在Nginx的基础上，针对大访问量网站的需求，添加了很多高级功能和特性。Tengine的性能和稳定性已经在大型的网站如淘宝网，天猫商城等得到了很好的检验。它的最终目标是打造一个高效、稳定、安全、易用的Web平台。(来自于[http://tengine.taobao.org/](http://tengine.taobao.org/))
### Nginx简介
Nginx是一个开源项目，是一个高性能的HTTP server 和 reverse proxy，也可以作为IMAP/POP的反向代理，通过简单的配置，就可以实现高性能，且占用系统资源较小。Nginx设计之初是为了解决C10K问题，其非常轻量级，仅支持event模型。
## 特性
### Nginx的特性
- 模块化设计，较好的扩展性
- 高可靠性：master/worker
- 支持热部署：不停机更新配置文件、更换日志文件、更新服务器版本
- 低内存消耗：10000个keep-alive连接模式下的非活动连接仅消耗2.5M内存
- event-driven，aio，mmap
- DOS机制在 2016-04-26   nginx-1.10.0 版本中才支持
### Tengine特性
- 继承Nginx-1.6.2的所有特性，兼容Nginx的配置；
- 动态模块加载（DSO）支持。加入一个模块不再需要重新编译整个Tengine；
- 支持SO_REUSEPORT选项，建连性能提升为官方nginx的三倍；
- 同时支持HTTP v2协议和SPDY v3协议，可同时使用两种协议；
- 流式上传到HTTP后端服务器或FastCGI服务器，大量减少机器的I/O压力；
- 更加强大的负载均衡能力，包括一致性hash模块、会话保持模块，还可以对后端的服务器进行主动健康检查，根据服务器状态自动上线下线，以及动态解析upstream中出现的域名；
- 输入过滤器机制支持。通过使用这种机制Web应用防火墙的编写更为方便；
- 支持设置proxy、memcached、fastcgi、scgi、uwsgi在后端失败时的重试次数
- 动态脚本语言Lua支持。扩展功能非常高效简单；
- 支持管道（pipe）和syslog（本地和远端）形式的日志以及日志抽样；
- 支持按指定关键字(域名，url等)收集Tengine运行状态；
- 组合多个CSS、JavaScript文件的访问请求变成一个请求；
- 自动去除空白字符和注释从而减小页面的体积
- 自动根据CPU数目设置进程个数和绑定CPU亲缘性；
- 监控系统的负载和资源占用从而对系统进行保护；
- 显示对运维人员更友好的出错信息，便于定位出错机器；
- 更强大的防攻击（访问速度限制）模块；
- 更方便的命令行参数，如列出编译的模块列表、支持的指令等；
- 可以根据访问文件类型设置过期时间；
(来自于[http://tengine.taobao.org/](http://tengine.taobao.org/))
## Tengine安装
下载地址：[http://tengine.taobao.org/](http://tengine.taobao.org/)


```
# yum -y install pcre-devel openssl-devel zlib-devel
# ./configure
prefix=/usr/local
sbin-path=/usr/sbin/nginx
conf-path=/etc/nginx/nginx.conf
error-log-path=/var/log/nginx/error.log
http-log-path=/var/log/nginx/access.log
pid-path=/var/run/nginx.pid
lock-path=/var/run/nginx.lock
http-client-body-temp-path=/var/cache/nginx/client_temp
http-proxy-temp-path=/var/cache/nginx/proxy_temp
http-fastcgi-temp-path=/var/cache/nginx/fastcgi_temp
http-uwsgi-temp-path=/var/cache/nginx/uwsgi_temp
http-scgi-temp-path=/var/cache/nginx/scgi_temp
user=nginx
group=nginx
with-http_ssl_module
with-http_realip_module
with-http_addition_module
with-http_sub_module
with-http_dav_module
with-http_flv_module
with-http_mp4_module
with-http_gunzip_module
with-http_gzip_static_module
with-http_random_index_module
with-http_secure_link_module
with-http_stub_status_module
with-http_auth_request_module
with-threads
with-stream
with-stream_ssl_module
with-http_slice_module
with-file-aio
with-http_v2_module
# make && make install
```
启动前配置：


```
echo “export PATH=$PATH:/usr/local/nginx/sbin” >/etc/profile.d/nginx.sh 
useradd -r nginx -s /sbin/nologin 
mkdir /var/cache/nginx/client_temp -pv
```
## 一、核心模块
### 1、Main端配置
> 
约定：
大部分模块使用方式，tengine与nginx相同，nginx模块使用可以参照nginx的官方站点，tengine。
Syntax: 表示功能或者选项，其后面有语法
Default:    默认值
Context:    可出现的上下文
参考文献：[http://tengine.taobao.org/documentation.html](http://tengine.taobao.org/documentation.html)
参考文献：[http://nginx.org/en/docs/](http://nginx.org/en/docs/)
### A、必备常用配置
1、user  指定用于运行worker进程的用户和组
Syntax: user user [group];
Default:    user nobody nobody;
Context:    main
2、pid 指定nginx进程的pid文件路径
yntax: pid file;
Default:
pid nginx.pid;
Context:    main
3、worker_rlimit_nofile number 单个worker进程所能够打开的最大文件数
Syntax: worker_rlimit_nofile number;
Default:    —
Context:    main
### B、性能优化相关配置
1、  worker_processes number | auto worker的进程数；通常应该为CPU的核心数减1
Syntax: worker_processes number | auto;
Default:
worker_processes 1;
Context:    main
2、worker_cpu_affinity cpumask … 将work进程绑定到cpu上
Syntax: worker_cpu_affinity cpumask …;
worker_cpu_affinity auto [cpumask];
Default:    —
Context:    main
例子：


```
2核cpu，开启八个进程 
worker_processes  8; 
worker_cpu_affinity 01 10 01 10 01 10 01 10; 
8核cpu，开启8个进程 
worker_processes  8; 
worker_cpu_affinity 10000000 01000000 00100000 00010000 00001000 00000100 00000010 00000001; 
8核cpu，开启2个进程 
worker_processes  2; 
worker_cpu_affinity 10101010 01010101;
```
3、worker_priority nice 定义work进程的优先级，数字越小优先级越高
[-20,19]
对应的优先级是：100-139
Syntax: worker_priority number;
Default:
worker_priority 0;
Context:    main
### C、调试定位问题
1、daemon on | off 是否以守护进程方式启动nginx进程
Syntax: daemon on | off;
Default:
daemon on;
Context:    main
2、master_process on | off 是否以master/worker模型启动nignx进程
Syntax: master_process on | off;
Default:
master_process on;
Context:    main
3、error_log file | stderr |  syslog:server=address[,parameter=value] | memory:size [debug | info | notice | warn | error | crit | alert | emerg]
错误日志记录方式，及其日志级别


```
Syntax: error_log file [level];
Default:
error_log logs/error.log error;
Context:    main, http, mail, stream, server, location
方式：
file  /PATH/TO/SOME_LOG_FILE;
stderr：发送到错误输出；
syslog:server=address[,parameter=value]：发送给syslog服务器；
memory:size：缓冲区大小
For example：
error_log  logs/error.log  notice
```
## 2、events段配置
1、worker_connections number 每个worker进程所能够并发打开的最大连接数
Syntax: worker_connections number;
Default:
worker_connections 512;
Context:    events
最大并发数=最大连接数*worker_processes 4的数字
2、use method 指明并发连接请求处理时使用的方法
Syntax: use method;
Default:    —
Context:    events
3、accept_mutex on | off 启用时，表示用于让多个worker轮流地、序列化地响应新请求
Syntax: accept_mutex on | off;
Default:
accept_mutex on;
Context:    events
## 3、server段常用配置
1、listen address[:port] [default_server] [ssl] [backlog=number] [rcvbuf=size] [sndbuf=size]
listen port [default_server] [ssl];
listen unix:path [default_server] [ssl]
default_server：默认虚拟主机
ssl：限制只能通过ssl连接提供服务
backlog：后缓队列的长度
rcvbuf：接收缓冲大小
sndbuf：发送缓冲区大小
2、server_name name … 主机名
Syntax: server_name name …;
Default:
server_name “”;
Context:    server
指明当前server的主机名；后可跟一个或空白字符分隔的多个主机；
支持使用*任意长度的任意字符；
支持~起始的正则表达式模式字符串；
应用策略：
(1) 首先精确匹配；
(2) 左则*通配符匹配；
(3) 右侧*通配符匹配；
(4) 正则表达式模式匹配；
For example：
server_name  www.example.com;
server_name *.example.com;
server_name  www.example.*;
server_name ~^..example..$;
mail.example.com, www.example.com
3、tcp_nodelay  on|off 对keepalived模式下的连接是否启用TCP_NODELAY选项
Syntax: tcp_nodelay on | off;
Default: tcp_nodelay on;
Context:    http, server, location
4、sendfile on | off 是否启用sendfile功能
Syntax: sendfile on | off;
Default:
sendfile off;
Context:    http, server, location, if in location
## 4、路径相关配置
1、root path 设置web资源路径映射
Syntax: root path;
Default:
root html;
Context:    http, server, location, if in location
For example：


```
location /i/ { 
   root /data/w3; 
}
```
2、location [ = | ~ | ~* | ^~ ] uri { … }
Syntax: location [ = | ~ | ~* | ^~ ] uri { … }
location @name { … }
Default:    —
Context:    server, location
根据用户请求的URI来匹配定义的location，匹配到时，此请求将被相应的location块中的指令所处理；
=：URI精确匹配；
~：做正则表达式模式匹配，区分字符大小写；
~*：做正则表达式模式匹配，不区分字符大小写；
^~：对URI的左半部分做匹配检查，不区分字符大小写；
匹配优先级：=、^~、~/～*、不带符号；
For example：


```
location = / { 
   [ configuration A ]； 
}
```


```
location / { 
   [ configuration B ]； 
}
```


```
location /documents/ { 
   [ configuration C ]；
}
```


```
location ^~ /images/ { 
   [ configuration D ]；
}
```


```
location ~* .(gif|jpg|jpeg)$ { 
   [ configuration E ]；
}
```
2、alias path 定义路径别名
Syntax: alias path;
Default:    —
Context:    location
For example：


```
location /i/ { 
   alias /data/w3/images/; 
}
```
3、error_page code … [=[response]] uri
根据用户其你去的资源的http响应的状态码实现错误页面重定向
Syntax: error_page code … [=[response]] uri;
Default:    —
Context:    http, server, location, if in location
Tengine配置选项众多，此处就介绍到此，如果有兴趣可以参考nginx官方文档或者tengine的官方文档，此处不在过多介绍，下面介绍几个常见使用的模块，ssl、rewrite、gzip、fastcgi、防盗链、stasus等功能
官方文档：
[http://tengine.taobao.org/documentation.html](http://tengine.taobao.org/documentation.html)
[http://nginx.org/en/docs/](http://nginx.org/en/docs/)
## 二、Tengine常用模块
## 1、Module ngx_http_rewrite_module
ngx_http_rewrite_module 模块可以通过使用正则表达式，重定向请求url。
A、rewrite regex replacement [flag] 重定向请求
把用户请求的URI基于regex做检查，匹配到时将替换为replacement指定的字符串
在同一个location中存在的多个rewrite规则会自上而下逐个被检查（循环），可以使用flag控制此循环功能
如果replacement是以http://或https://开头，则替换结果会直接以重定向方式返回给客户端
[flag]：
- last：重写完成后停止对当前uri在当前location中的后续其它重写操作，改为对新uri的新一轮处理；
- break：重写完成后停止对当前uri在当前location中的后续其它重写操作;
- redirect：重写完成后以临时重定向方式直接返回重写后生成的新URL给客户端，由客户对新URL进行请求；（302）
- permanent：重写完成后以永久重定向方式直接返回重写后生成的新URL给客户端，由客户对新URL进行请求；（301）
B、rewrite_log on | off 是否启用重写日志
启用时，日志信息被发往错误日志
Syntax:rewrite_log on | off;
Default:rewrite_log off;
Context:http, server, location, if
C、if (condition) { … } 条件判断机制
在条件满足时，执行配置块中的配置
condition：
比较表达式：
==, !=
- ~：模式匹配，区分字母大小写；
- ~*：模式匹配，不区分字符大小写；
- !~：模式不匹配，区分字母大小写；
- !~*：模式不匹配，不区分字母大小写；
文件及目录存在性判断：
- -f, !-f：文件
- -d, !-d：目录
- -e, !-e：存在
- -x, !-x：执行
For example：


```
if ($http_user_agent ~ MSIE) {
    rewrite ^(.*)$ /msie/$1 break;
}
if ($http_cookie ~* "id=([^;]+)(?:;|$)") {
    set $id $1;
}
if ($request_method = POST) {
    return 405;
}
if ($slow) {
    limit_rate 10k;
}
if ($invalid_referer) {
    return 403;
}
```
D、return code [text];停止处理并将指定的代码返回给客户端
Syntax:return code [text];
return code URL;
return URL;
Default:—
Context:server, location, if
E、set $variable value用户自定义变量
Syntax:set $variable value;
Default:—
Context:server, location, if
ngx_http_rewrite_module 示例：


```
# 将所有访问172.18.4.70下image都重定向到tupian目录下
# vim /usr/local/nginx/conf/nginx.conf
        location /image {
                rewrite ^/image/(.*) /tupian/$1 last;
                rewrite ^/image/(.*) /tupian/$1  last;
                return  403;
                index index.html;
        }
```
测试效果：


```
# 添加测试页面
# cat /usr/local/nginx/html/image/index.html 
<h1>image</h1>
# cat /usr/local/nginx/html/tupian/index.html
<h1>tupian</h1>
```
![wKioL1czKFqxORRUAAA5KRdBvbU531.png](http://jbcdn2.b0.upaiyun.com/2016/05/4da416c9d47db07982dee6f8310a353b.png)
if (condition) { … } 示例


```
if ($request_method = GET) {
    return 405;
}
```
![wKioL1czKvjjnGDGABYJrAOvjVw840.gif](http://jbcdn2.b0.upaiyun.com/2016/05/20285065d764c2ef800d19c5fdb09993.gif)
## 2、Module ngx_http_gzip_module
过滤器，对指定类型的资源压缩传输以节约带宽
gzip on | off;
启用或禁用gzip压缩响应报文；
Syntax:gzip on | off;
Default:gzip off;
Context:http, server, location, if in location
gzip_comp_level level;
指定压缩比，1-9,默认为1；
Syntax:gzip_comp_level level;
Default: gzip_comp_level 1;
Context:http, server, location
gzip_disable regex …;
regex是匹配客户端浏览器类型的模式，表示对所有匹配到的浏览器不执行压缩响应；
Syntax:gzip_disable regex …;
Default:—
Context:http, server, location
This directive appeared in version 0.6.23.
gzip_min_length length;
触发启用压缩功能的响应报文的最小长度；
Syntax:gzip_min_length length;
Default:gzip_min_length 20;
Context:http, server, location
gzip_http_version 1.0 | 1.1;
设定启用压缩响应功能时，协议的最小版本；
Syntax:gzip_http_version 1.0 | 1.1;
Default:gzip_http_version 1.1;
Context:http, server, location
gzip_types mime-type …;
指定仅执行压缩的资源内容类型；默认为text/html；
Syntax:gzip_types mime-type …;
Default:gzip_types text/html;
Context:http, server, location
gzip_proxied off | expired | no-cache | no-store | private | no_last_modified | no_etag | auth | any …;
对代理的请求基于何种属性判断其是否应该启用压缩功能；
Syntax:gzip_proxied off | expired | no-cache | no-store | private | no_last_modified | no_etag | auth | any …;
Default:gzip_proxied off;
Context:http, server, location
Example Configuration：
示例：
gzip on;
gzip_http_version 1.0;
gzip_comp_level 6;
gzip_disable msie6;
gzip_min_length 2;
gzip_types text/plain text/css text/xml application/x-javascript              application/xml  application/json application/java-script;
测试示例：
未配置之前：查看Response Headers报文中是否有gzip选项
![wKioL1czLwKi3cq4AAG_cROXyVA225.png](http://jbcdn2.b0.upaiyun.com/2016/05/478d425bb39dfec842b570d6b079eb9b.png)
配置之后：查看Response Headers报文中是否有gzip选项
![wKiom1czLtbz0qDVAAGwfV3fsvc931.png](http://jbcdn2.b0.upaiyun.com/2016/05/dc9e62da8889717a56a257fc37e6bfd8.png)
## 3、Module ngx_http_fastcgi_module
fastcgi_pass address address是fpm服务器监听的地址和端口；
示例： fastcgi   127.0.0.1:9000;
fastcgi_index name fastcgi应用的主页名称
fastcgi_param parameter value [if_not_empty] 传递给fpm服务器的参数及其值
fastcgi_cache_path path [levels=levels] [use_temp_path=on|off] keys_zone=name:size [inactive=time] [max_size=size] [loader_files=number] [loader_sleep=time] [loader_threshold=time] [purger=on|off] [purger_files=number] [purger_sleep=time] [purger_threshold=time];
path：文件系统路径，用于存储缓存的文件数据；
max_size=size：定义此路径下的多大空间用于存储缓存数据；
levels=#[:#[:#]]：缓存目录层级定义；
levels=1:2
keys_zone=name:size 内存中用于缓存k/v映射关系的空间名称及大小；
inactive=time
注意：只能定义在http上下文
For example：


```
fastcgi_cache_path /var/cache/nginx/client_temp levels=1:2 keys_zone=one:10m;
```
存储格式：


```
/var/cache/nginx/client_temp/c/29/b7f54b2df7773722d382f4809d65029c
```
fastcgi_cache zone | off 是否启用cache，如果启用，数据缓存于哪个cache中
fastcgi_cache_key string 定义要使用的缓存键
例如： fastcgi_cache_key  $request_uri;
fastcgi_cache_methods GET | HEAD | POST … 缓存哪些类型的请求的相关数据
fastcgi_cache_valid [code …] time 对不同响应码设定其可缓存时长
注意：调用缓存时，至少应该 指定三个参数
fastcgi_cache
fastcgi_cache_key
fastcgi_cache_valid
测试示例：


```
# vim /usr/local/nginx/conf/nginx.conf
fastcgi_cache_path /var/data/nginx/cache levels=1:2 keys_zone=mycache:10m inactive=4m;
# 上面选项必须在http上下文
        location \.php$ {
                fastcgi_pass 127.0.0.1:9000;
                fastcgi_cache mycache;
                fastcgi_cache_key $request_uri;
                fastcgi_cache_valid any 10m;
        }
```
启用前查看:/var/data/nginx/cache目录
![wKiom1czU4nRW19wAAASbCCj9K4054.png](http://jbcdn2.b0.upaiyun.com/2016/05/4621e04221d2b4a9ee8f8006f67ddb27.png)
启动后查看：/var/data/nginx/cache目录
![wKioL1czVJGAf4lcAACs8RKFqkE993.png](http://jbcdn2.b0.upaiyun.com/2016/05/69be1a91d08bf594ee305e24ef8edec1.png)
## 4、Module ngx_http_ssl_module
ssl on | off 是否启用当前虚拟主机的ssl功能
Syntax:ssl on | off;
Default:ssl off;
Context:http, server
ssl_certificate file 当前虚拟主机使用的PEM格式的证书文件
Syntax: ssl_certificate file;
Default:—
Context:http, server
ssl_certificate_key file 当前虚拟机使用的证书文件中的公钥配对儿的私钥文件路径                    Syntax:ssl_certificate_key file;
Default:—
Context:http, server
ssl_protocols [SSLv2] [SSLv3] [TLSv1] [TLSv1.1] [TLSv1.2] SSL协议的版本
Syntax:ssl_protocols [SSLv2] [SSLv3] [TLSv1] [TLSv1.1] [TLSv1.2];
Default:ssl_protocols TLSv1 TLSv1.1 TLSv1.2;
Context:http, server
ssl_session_cache off | none | [builtin[:size]] [shared:name:size] 指明ssl会话的缓存机制
Syntax:ssl_session_cache off | none | [builtin[:size]] [shared:name:size];
Default:ssl_session_cache none;
Context:http, server
builtin：使用openssl内建的缓存机制，此为各worker独有；
shared：由各worker共享的缓存；
name：缓存空间的名称；
size：字节为单位的缓存空间的大小；每1MB内存空间可缓存4000个会话
ssl_session_timeout time ssl会话超时时长，指ssl session cache中缓存条目有效时长
Syntax:ssl_session_timeout time;
Default:ssl_session_timeout 5m;
Context:http, server
示例：
配置文件：证书使用方式请查看我的另外[一篇博客](http://xinzong.blog.51cto.com/10018904/1763615)。


```
# vim /etc/nginx/nginx.conf
    server {
        listen       443 ssl;
        server_name  www.ops.com;
        ssl_certificate      /usr/local/nginx/ssl/nginx.crt;
        ssl_certificate_key  /usr/local/nginx/ssl/nginx.key;
        ssl_session_cache    shared:SSL:1m;
        ssl_session_timeout  5m;
        location / {
            root   html;
            index  index.html index.htm;
        }
    }
# nginx -s reload
```
访问查看:
![wKioL1c15r-Rw4JJAADnEaKmyfs210.png](http://jbcdn2.b0.upaiyun.com/2016/05/b32d8ea42483b92053b09d5ee1f1ccfe.png)
## 4、Module ngx_http_referer_module
valid_referers none | blocked | server_names | string …
referers：定义合法的数据，此模块用来定义哪些内容是合法的
Syntax:valid_referers none | blocked | server_names | string …;
Default:—
Context:server, location
none表示请求报文首部没有referer首部
blocked表示请求报文的referer首部没有值
server_names表示其值是主机名
示例： 表示根据上面的定义，如果不合法就返回403


```
valid_referers none blocked server_names
               *.zx.com zx.* www.zx.org/galleries/
               ~\.google\.;
if ($invalid_referer) {
    return 403;
}
```
## 5、Module ngx_http_stub_status_module
## status_module：用于输出nginx的基本状态信息
示例：


```
# vim /etc/nginx/nginx.conf
       location /status {
          stub_status on;
        }
```
访问并测试：
![wKiom1c17NrCdD7wAAA-n7T42tQ400.png](http://jbcdn2.b0.upaiyun.com/2016/05/697c524ef39475fbb7991135e922741f.png)
各参数的意义：
- Active connection：活跃连接数
- accepts：已经接受的客户端请求的总数量
- handled：已经处理的数量
- request：客户端发来的总请求数量
- Reading：处于读取客户端请求报文首部的连接数
- Writing：处于向客户端发送响应报文过程的连接数
- Waiting：处于等待客户端发出请求的空闲请求连接数
