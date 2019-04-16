# Nginx 代理缓存 - youbingchen的博客 - CSDN博客





2016年06月13日 13:30:56[youbingchen](https://me.csdn.net/youbingchen)阅读数：646








# 何为 代理?

通常说的代理就是正向代理，它的工作原理就像一个跳板，简单来说，我访问不了谷歌，但是但是我能访问一个代理服务器A，A能访问google.com，于是我先连上代理服务器A，告诉他我需要google.com的内容，A就去取回来，然后返回给我。从网站的角度，只在代理服务器来取内容的时候有一次记录，有时候并不知道是用户的请求，也隐藏了用户的资料，这取决于代理告不告诉网站。

总之一句话，正向代理就是位于客户端和原始服务器之间的服务器，为了 得到原始服务器的 内容，客户端 向代理发送一个请求并指定目标（原始服务器），然后代理 向 原始服务器 转交并获得的内容返回给客户端。 

目前代理软件很多，比如 自由门 ，红杏等

#### 反向代理

如我想访问 [http://www.test.com/readme](http://www.test.com/readme)，但www.test.com上并不存在readme页面，于是他是偷偷从另外一台服务器上取回来，然后作为自己的内容返回用户，但用户并不知情。这里所提到的 www.test.com 这个域名对应的服务器就设置了反向代理功能。 

总之，反向代理正好相反，对于客户端而言它就像是原始服务器，并且客户端不需要进行任何特别的设置。客户端向反向代理的命名空间(name-space)中的内容发送普通请求，接着反向代理将判断向何处(原始服务器)转交请求，并将获得的内容返回给客户端，就像这些内容原本就是它自己的一样。 

nginx 使用反向代理，主要是使用location模块下的proxy_pass选项。 
`proxy_pass`选项主要是转发请求到其他的服务器。HTTP/1.0无法使用keepalive（后端服务器将为每个请求创建并且删除连接）。nginx为浏览器发送HTTP/1.1并为后端服务器发送HTTP/1.0，这样浏览器就可以为浏览器处理keepalive
#### 反向代理配置

```
//修改nginx.conf  文件
vim  nginx.conf
location / {
               proxy_pass      http://115.28.70.190;
       }
```

指令:`proxy_pass`

语法: proxy url 

使用字段：location, location中的if字段 

这个指令设置被代理服务器的地址和被映射的URI，地址可以使用主机名或IP加端口号的形式，例如：proxy_pass [http://localhost:8000/uri/](http://localhost:8000/uri/); 
![案例](https://img-blog.csdn.net/20160613132825257)
##### 查看日志文件
`vim  ./logs/access.log`
![access.log](https://img-blog.csdn.net/20160613132848460)

```php
// 修改nginx.cof
location / {
        proxy_pass      http://115.28.70.190;
        proxy_set_header  X-Real-IP  $remote_addr; #加上这一行
}
```

指令说明：proxy_set_header 

语法：proxy_set_header header value 

默认值： Host and Connection  

使用字段：http, server, location  

这个指令允许将发送到被代理服务器的请求头重新定义或者增加一些字段。这个值可以是一个文本，变量或者它们的组合。proxy_set_header在指定的字段中没有定义时会从它的上级字段继承。
```
/usr/local/nginx/sbin/nginx -s reload  //重启nginx
在浏览器重新访问 web地址 
tail ./logs/access.log
```

![access.log_2](https://img-blog.csdn.net/20160613132916773)

##### 查看并修改httpd配置文件

将h%修改为%{X-Real-IP}i

#### Nginx之页面缓存

指令说明 ：proxy_cache_path 

语法：proxy_cache_path path [levels=number] keys_zone=zone_name:zone_size [inactive=time] [max_size=size]; 

默认值：None 

使用字段：http 

指令指定缓存的路径和一些其他参数，缓存的数据存储在文件中，并且使用代理url的哈希值作为关键字与文件名。levels参数指定缓存的子目录数，例如：`proxy_cache_path  /data/nginx/cache  levels=1:2   keys_zone=one:10m;`
文件名 类似于：
`/data/nginx/cache/c/29/b7f54b2df7773722d382f4809d65029c`
> 
levels指定目录结构，可以使用任意的1位或2位数字作为目录结构，如 X, X:X,或X:X:X 例如: “2”, “2:2”, “1:1:2“，但是最多只能是三级目录。  

  所有活动的key和元数据存储在共享的内存池中，这个区域用keys_zone参数指定。one指的是共享池的名称，10m指的是共享池的大小。 

  注意每一个定义的内存池必须是不重复的路径，例如：
```
proxy_cache_path  /data/nginx/cache/one    levels=1      keys_zone=one:10m;
proxy_cache_path  /data/nginx/cache/two    levels=2:2    keys_zone=two:100m;
proxy_cache_path  /data/nginx/cache/three  levels=1:1:2  keys_zone=three:1000m;
```

> 
如果在inactive参数指定的时间内缓存的数据没有被请求则被删除，默认inactive为10分钟。一个名为cache manager的进程控制磁盘的缓存大小，它被用来删除不活动的缓存和控制缓存大小，这些都在max_size参数中定义，当目前缓存的值超出max_size指定的值之后，超过其大小后最少使用数据（LRU替换算法）将被删除。内存池的大小按照缓存页面数的比例进行设置，一个页面（文件）的元数据大小按照操作系统来定，如FreeBSD/i386下为64字节，FreeBSD/amd64下为128字节。


指令 说明 ：proxy_cache 

语法 ：proxy_cache zone_name; 

默认值：None 

使用字段：http, server, location
> 
设置一个缓存区域的名称，一个相同的区域可以在不同的地方使用。在0.7.48后，缓存遵循后端的”Expires”, “Cache-Control: no-cache”, “Cache-Control:max-age=XXX”头部字段，0.7.66版本以后，”Cache-Control:“private”和”no-store”头同样被遵循。nginx在缓存过程中不会处理”Vary”头，为了确保一些私有数据不被所有的用户看到，后端必须设置 “no-cache”或者”max-age=0”头，或者proxy_cache_key包含用户指定的数据如$cookie_xxx，使用cookie的值作为proxy_cache_key的一部分可以防止缓存私有数据，所以可以在不同的location中分别指定proxy_cache_key的值以便分开私有数据和公有数据。 缓存指令依赖代理缓冲区(buffers)，如果proxy_buffers设置为off，缓存不会生效。


指令 说明 ：proxy_cache_valid 

语法：proxy_cache_valid reply_code [reply_code …] time; 

默认值：None 

使用字段：http, server, location 

为不同的应答设置不同的缓存时间，例如：
```bash
proxy_cache_valid  200 302  10m; # 为应答代码为200和302的设置缓存时间为10分钟
proxy_cache_valid  404      1m;  #  404代码缓存1分钟
```

如果只定义时间：
`proxy_cache_valid 5m;`
那么只对代码为200, 301和302的应答进行缓存。 

同样可以使用any参数任何应答。

```
proxy_cache_valid  200 302 10m;
proxy_cache_valid  301 1h;
proxy_cache_valid  any 1m;
```

定义一个简单nginx缓存服务器:

```
[root@nginx ~]# vim /etc/nginx/nginx.conf
proxy_cache_path /data/nginx/cache/webserver levels=1:2 keys_zone=webserver:20m max_size=1g;
   server {
       listen       80;
       server_name  localhost;
       #charset koi8-r;
       #access_log  logs/host.access.log  main;
       location / {
               proxy_pass      http://webservers;
               proxy_set_header  X-Real-IP  $remote_addr;
               proxy_cache webserver;
               proxy_cache_valid 200 10m;
       }
}
```

新建缓存目录
`mkdir -pv /data/nginx/cache/webserver`
现在 测试一下（谷歌浏览器） 
![test](https://img-blog.csdn.net/20160613133006399)
> 
注，大家用谷歌浏览器测试的时候，可以按F12调用开发工具，选择Network选项，我们可以看到，ResponseHeaders，在这里我们可以看到，我们请求的是否是缓存，但现在还看不到，下面我们来配置一下，再来测试。


$server_addr

> 
服务器地址，在完成一次系统调用后可以确定这个值，如果要绕开系统调用，则必须在listen中指定地址并且使用bind参数。


$upstream_cache_status 

1. MISS 未命中 

2. EXPIRED - expired。请求被传送到后端。 

3. UPDATING - expired。由于proxy/fastcgi_cache_use_stale正在更新，将使   用旧的应答。 

4.STALE - expired。由于proxy/fastcgi_cache_use_stale，后端将得到过期的应答。 

5.HIT 命中
```php
//修改nginx.conf文件 
proxy_cache_path /data/nginx/cache/webserver levels=1:2 keys_zone=webserver:20m max_size=1g;
    server {
        listen       80;
        server_name  localhost;
        #charset koi8-r;
        #access_log  logs/host.access.log  main;
       #增加两头部
        add_header X-Via $server_addr;
        add_header X-Cache $upstream_cache_status;
        location / {
                proxy_pass      http://webservers;
                proxy_set_header  X-Real-IP  $remote_addr;
                proxy_cache webserver;
                proxy_cache_valid 200 10m;
        }
}
```

![Test2](https://img-blog.csdn.net/20160613133023822)

如图 命中的 是代理服务器的Ip。 

查看日志`ls /usr/local/nginx/data/cache/webserver/8/50/9d0f432f7ea836f53470bb556c977508`













