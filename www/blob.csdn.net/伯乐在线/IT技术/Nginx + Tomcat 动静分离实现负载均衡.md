# Nginx + Tomcat 动静分离实现负载均衡 - 文章 - 伯乐在线
原文出处： [无脑仔的小明](http://www.cnblogs.com/wunaozai/p/5001742.html#3317173)
# **0.前期准备**
使用Debian环境。安装Nginx(默认安装)，一个web项目，安装tomcat(默认安装)等。
# **1.一份Nginx.conf配置文件**

JavaScript
```
# 定义Nginx运行的用户 和 用户组 如果对应服务器暴露在外面的话建议使用权限较小的用户 防止被入侵
# user www www;
#Nginx进程数, 建议设置为等于CPU总核心数
worker_processes 8;
#开启全局错误日志类型
error_log /var/log/nginx/error.log info;
#进程文件
pid /var/run/nginx.pid;
#一个Nginx进程打开的最多文件描述数目 建议与ulimit -n一致
#如果面对高并发时 注意修改该值 ulimit -n 还有部分系统参数 而并非这个单独确定
worker_rlimit_nofile 65535;
events{
 #使用epoll模型提高性能
 use epoll;
 #单个进程最大连接数
 worker_connections 65535;
}
http{
 #扩展名与文件类型映射表
 include mime.types;
 #默认类型
 default_type application/octet-stream;
 sendfile on;
 tcp_nopush on;
 tcp_nodelay on;
 keepalive_timeout 65;
 types_hash_max_size 2048;
 #日志
 access_log /var/log/nginx/access.log;
 error_log /var/log/nginx/error.log;
 #gzip 压缩传输
 gzip on;
 gzip_min_length 1k; #最小1K
 gzip_buffers 16 64K;
 gzip_http_version 1.1;
 gzip_comp_level 6;
 gzip_types text/plain application/x-javascript text/css application/xml application/javascript;
 gzip_vary on;
 #负载均衡组
 #静态服务器组
 upstream static.zh-jieli.com {
 server 127.0.0.1:808 weight=1;
 }
 #动态服务器组
 upstream zh-jieli.com {
 server 127.0.0.1:8080;
 #server 192.168.8.203:8080;
 }
 #配置代理参数
 proxy_redirect off;
 proxy_set_header Host $host;
 proxy_set_header X-Real-IP $remote_addr;
 proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
 client_max_body_size 10m;
 client_body_buffer_size 128k;
 proxy_connect_timeout 65;
 proxy_send_timeout 65;
 proxy_read_timeout 65;
 proxy_buffer_size 4k;
 proxy_buffers 4 32k;
 proxy_busy_buffers_size 64k;
 #缓存配置
 proxy_cache_key '$host:$server_port$request_uri';
 proxy_temp_file_write_size 64k;
 proxy_temp_path /dev/shm/JieLiERP/proxy_temp_path;
 proxy_cache_path /dev/shm/JieLiERP/proxy_cache_path levels=1:2 keys_zone=cache_one:200m inactive=5d max_size=1g;
 proxy_ignore_headers X-Accel-Expires Expires Cache-Control Set-Cookie;
server{
 listen 80;
 server_name erp.zh-jieli.com;
 location / {
 index index; #默认主页为 /index
 #proxy_pass http://jieli;
 }
 location ~ .*\.(js|css|ico|png|jpg|eot|svg|ttf|woff) {
 proxy_cache cache_one;
 proxy_cache_valid 200 304 302 5d;
 proxy_cache_valid any 5d;
 proxy_cache_key '$host:$server_port$request_uri';
 add_header X-Cache '$upstream_cache_status from $host';
 proxy_pass http://static.zh-jieli.com;
 #所有静态文件直接读取硬盘
 # root /var/lib/tomcat7/webapps/JieLiERP/WEB-INF ;
 expires 30d; #缓存30天
 }
 #其他页面反向代理到tomcat容器
 location ~ .*$ {
 index index;
 proxy_pass http://zh-jieli.com;
 }
 }
 server{
 listen 808;
 server_name static;
 location / {
}
 location ~ .*\.(js|css|ico|png|jpg|eot|svg|ttf|woff) {
 #所有静态文件直接读取硬盘
 root /var/lib/tomcat7/webapps/JieLiERP/WEB-INF ;
 expires 30d; #缓存30天
 }
 }
}
```
基本配置这个文件，就可以实现负载了。但是里面的各种关系要了解就比较麻烦了。这篇博客，也不是教学篇，是记录一下，方便以后自己看了。
# **2.基础讲解**
现在假使有一台电脑192.168.8.203这台电脑，上面部署了Tomcat，里面8080端口有J2EE的服务，通过浏览器可以正常浏览网页。现在有一个问题tomcat是一个比较全面的web容器，对静态网页的处理，应该是比较费资源的，特别是每次都要从磁盘读取静态页面，然后返回。这中间会消耗Tomcat的资源，可能会使那些动态页面解析性能影响。秉承Linux哲学，一个软件只做一件事的原则。Tomcat就应该只处理JSP动态页面。这里就用到以前了解的Nginx来进行反向代理。第一步代理，实现动静网页分离。这个很简单的。

JavaScript
```
worker_processes 8;
 pid /var/run/nginx.pid;
 worker_rlimit_nofile 65535;
 events{
 use epoll;
 worker_connections 65535;
 }
 http{
 include mime.types;
 default_type application/octet-stream;
 sendfile on;
 tcp_nopush on;
 tcp_nodelay on;
 keepalive_timeout 65;
 types_hash_max_size 2048;
proxy_redirect off;
 proxy_set_header Host $host;
 proxy_set_header X-Real-IP $remote_addr;
 proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
 client_max_body_size 10m;
 client_body_buffer_size 128k;
 proxy_connect_timeout 65;
 proxy_send_timeout 65;
 proxy_read_timeout 65;
 proxy_buffer_size 4k;
 proxy_buffers 4 32k;
 proxy_busy_buffers_size 64k;
 server{
 listen 80;
 server_name xxx.com;
 location / {
 index index; 
 }
 location ~ .*\.(js|css|ico|png|jpg|eot|svg|ttf|woff) {
 proxy_pass http://192.168.8.203:8080;
 expires 30d; 
 }
 location ~ .*$ {
 index index;
 proxy_pass http://192.168.8.203:8080;
 }
 }
 }
```


JavaScript
```
worker_processes 8;
pid /var/run/nginx.pid;
worker_rlimit_nofile 65535;
events{
 use epoll;
 worker_connections 65535;
 }
http{
 include mime.types;
 default_type application/octet-stream;
 sendfile on;
 tcp_nopush on;
 tcp_nodelay on;
 keepalive_timeout 65;
 types_hash_max_size 2048;
proxy_redirect off;
 proxy_set_header Host $host;
 proxy_set_header X-Real-IP $remote_addr;
 proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
 client_max_body_size 10m;
 client_body_buffer_size 128k;
 proxy_connect_timeout 65;
 proxy_send_timeout 65;
 proxy_read_timeout 65;
 proxy_buffer_size 4k;
 proxy_buffers 4 32k;
 proxy_busy_buffers_size 64k;
server{
 listen 80;
 server_name xxx.com;
 location / {
 index index;
 }
 location ~ .*\.(js|css|ico|png|jpg|eot|svg|ttf|woff) {
 proxy_pass http://192.168.8.203:8080;
 expires 30d;
 }
 location ~ .*$ {
 index index;
 proxy_pass http://192.168.8.203:8080;
 }
 }
 }
```
修改nginx的配置文件 /etc/nginx/nginx.conf 默认有个配置文件的。其实大部分都差不多，关键还是server段的设置。这里我设置server段如上所示，其他段复制就可以了。server段里面的解释如下：第35行为监听本机80端口。37-39行表示默认主页，这里的默认主页我是index.jsp 对应到我项目中是一个index。 这里根据需要可以改为
index index.jsp index.html index.htm index.php
具体可参考其他文章。 关键的第40行，这个是正则匹配，网上也有很多介绍。这里匹配我项目中用到的所有静态网页后缀。第41行是代理地址。这里我代理到我的web应用中。expires 30d缓存为30天，这里的缓存是对应到前端页面，用户的Cache-Control字段，
![](http://images2015.cnblogs.com/blog/485067/201511/485067-20151127202715952-228143231.jpg)
第44行中那个正则是匹配无后缀的页面。我项目中jsp页面是无后缀的。这里可以根据需要进行修改。同样代理到192.168.8.203:8080这里。到这里你可能会问，我艹，这有毛意思啊？当然不是这样了。简单的实现静动分离，我们可以把第41行进行修改，改为

JavaScript
```
root   /var/lib/tomcat7/webapps/JieLiERP/WEB-INF
```
表示不代理，直接从本地磁盘拿。通过查tomcat日志可以看到静态页面是没有访问到的。但这样又有一个问题。这样的灵活性不好，对下面要讲到的内存缓存和集群部署来说都是不友好的，所以又有了下面的这种写法。再写一个server段。

JavaScript
```
server{
 listen 808;
 server_name static;
 location / {
}
location ~ .*\.(js|css|ico|png|jpg|eot|svg|ttf|woff) {
 #所有静态文件直接读取硬盘
 root /var/lib/tomcat7/webapps/JieLiERP/WEB-INF ;
 expires 30d; #缓存30天
 }
 }
```
这次监听808端口，然后上上面的代码41行就可以修改为 proxy_pass http://192.168.8.203:808了，到这里就实现了动静分离了。如果多台服务器，就修改对应的ip就可以了。如果发现连接不上的，要检查一下防火墙，权限等外部问题，这个配置是这样的。
如果单纯这样的话，我们会发现页面直接传输过于占用带宽。对应web的优化，这里想到的是通过对页面进行gzip压缩，然后传到用户那里，再解压，这样可以有效的减少带宽。这里就会用到Nginx 的gzip模块了。默认的Nginx是集成有gzip模块的。只需在http段增加下面配置即可。

JavaScript
```
gzip on;
 gzip_min_length 1k; #最小1K
 gzip_buffers 16 64K;
 gzip_http_version 1.1;
 gzip_comp_level 6;
 gzip_types text/plain application/x-javascript text/css application/xml application/javascript;
 gzip_vary on;
```
给个首页看看效果
![](http://images2015.cnblogs.com/blog/485067/201511/485067-20151127204303468-135393383.jpg)
![](http://images2015.cnblogs.com/blog/485067/201511/485067-20151127204311702-270530596.jpg)
不要在意请求数不一样，那两个请求是谷歌插件来的。不用觉得我在骗你。
作为假使有很多人访问的网站来说，缓存肯定是很重要的东西了。一开始是想通过插件，让Nginx和Redis进行合成，然后Nginx使用Redis来缓存的，但是发现配置起来很麻烦，还要自己下载插件，重新编译Nginx，比较麻烦，所以这里觉得用Nginx自带的缓存也是不错的选择。虽然效率比不上redis，但是有还是比没有好。Nginx默认的缓存是磁盘文件系统的缓存，而不是像Redis那样的内存级别的缓存。一开始我以为Nginx就只有这样。后来查了写资料，才知道是我太天真了，对Linux不是很了解导致的。Linux的一切皆文件。原来我们可以把文件缓存到内存对应的Linux文件系统中。我说的可能比较难以理解，请自行搜索/dev/shm 这个文件目录。我们把文件缓存到这个文件目录里，其实就相当与内存的缓存了。只不过还是靠文件系统管理。所以比不上自定义格式的Redis那样的内存缓存。
在http段进行基本配置

JavaScript
```
1 #缓存配置
 2 proxy_cache_key '$host:$server_port$request_uri';
 3 proxy_temp_file_write_size 64k;
 4 proxy_temp_path /dev/shm/JieLiERP/proxy_temp_path;
 5 proxy_cache_path /dev/shm/JieLiERP/proxy_cache_path levels=1:2 keys_zone=cache_one:200m inactive=5d max_size=1g;
 6 proxy_ignore_headers X-Accel-Expires Expires Cache-Control Set-Cookie;
```


JavaScript
```
location ~ .*\.(js|css|ico|png|jpg|eot|svg|ttf|woff) {
 proxy_cache cache_one;
 proxy_cache_valid 200 304 302 5d;
 proxy_cache_valid any 5d;
 proxy_cache_key '$host:$server_port$request_uri';
 add_header X-Cache '$upstream_cache_status from $host';
 proxy_pass http://192.168.8.203:808;
expires 30d; #缓存30天
 }
```
经过这两个的配置就基本能实现了，这里说几个注意项，也是困扰我很久的问题。上面第一段代码第6行，proxy_ignore_headers 如果web项目中的html的head头里面指定

JavaScript
```
1 <meta http-equiv="pragma" content="no-cache">
 2 <meta http-equiv="cache-control" content="no-cache">
 3 <meta http-equiv="expires" content="0">
```
这些不缓存的话，就要加上proxy_ignore_headers的配置项了。还有一点就是/dev/shm下面的文件系统权限默认只给root用户，所以要chmod 777 -R /dev/shm 这样不是很安全的做法，如果实际上线可以给定某个用户组，关于用户组的设置是配置的第一行

JavaScript
```
user www www;
```
上面第二段代码的第6行是增加一个header字段方便查看是否击中缓存。
我们rm -rf /dev/shm/JieLiERP/proxy_* 下面的所有文件(注意这里如果是进行多次测试的话要nginx -s reload 重新读取配置或重启服务，因为你rm -rf只是删除了缓存文件，但是缓存的结构信息还在nginx进程里面，结构还在，如果不重启的话，是会出现访问不到的)
![](http://images2015.cnblogs.com/blog/485067/201511/485067-20151127210809702-1929189298.jpg)
所以要记得重启哦。下面是运行效果
第一次访问
![](http://images2015.cnblogs.com/blog/485067/201511/485067-20151127211035562-771165357.jpg)
第二次访问，在浏览器中Ctrl+Shift+R 强制刷新
![](http://images2015.cnblogs.com/blog/485067/201511/485067-20151127211045437-992023397.jpg)
到这里就可以看到效果了。我们查看一下/dev/shm这个里面
![](http://images2015.cnblogs.com/blog/485067/201511/485067-20151130093144749-114152762.jpg)
到这里已经快结束了。最后也是比较关键的一个技术点，就是集群，集群，集群。这个就要用到upstream了，看到最开头的配置文件了吗，就是那个

JavaScript
```
#负载均衡组
#静态服务器组
upstream static {
 server 127.0.0.1:808 weight=1;
 server 192.168.8.203:808 weight=1;
}
```


JavaScript
```
#动态服务器组
upstream dynamic {
 server 127.0.0.1:8080;
 #server 192.168.8.203:8080;
}
```
上面那个就是集群组了。upstream是关键字，static 和 dynamic是两个服务器集群组的名称。以第一个为例，server 127.0.0.1:808 是服务器地址，后面的weight=1 是权重。有多个就写多个。亲测试过，集群中的一个坏了，不影响系统运行。至于更多的轮询规则，可以参考网上更多的资料。这里不多说。至于怎么使用呢？ proxy_pass http://192.168.8.203:808 改为 proxy_pass http://static; 这样即可实现均衡。
到这里就结束了。把上面各个部分根据自己需求配置起来就可以实现单机房负载均衡了。 上面这种做法有一个缺点就是在前面的那一台nginx如果当机，后面所以机器就失去了被访问的能力了，所以需要在前面实现多个nginx多机房的负载。关于这个就是另外一个话题了。目前还没有研究。以后有机会再说了。
上面动态服务器组如果是那种需要保存用户状态的话，会有问题，就是session问题，比如我在server1进行登录后，下一次动态服务器组进行轮询后可能分配到server2，就会造成要重新登录。治标的办法是，配置轮询规则，根据用户请求的IP进行Hash，然后分配对应的服务器。具体配置如下：

JavaScript
```
upstream dynamic{
 ip_hash;
 server 127.0.0.1:8080;
 server 192.168.0.203:8080;
 }
```
这样就可以实现一个用户对应一个服务器节点。这样就不会有重复登录的问题。另一种治本的办法是，利用缓存系统进行session的统一存储管理。具体的做法我还没有试验过，参考资料有相关的文章，可以了解一下。
- 参考资料:
- [http://www.ha97.com/5194.html](http://www.ha97.com/5194.html%20) Nginx配置文件nginx.conf中文详解
- [http://saiyaren.iteye.com/blog/1956692](http://saiyaren.iteye.com/blog/1956692) nginx cache静态化+tmpfs 高性能cdn方案
- [http://www.oschina.net/question/35243_180072](http://www.oschina.net/question/35243_180072) Nginx配置了proxy_cache为什么不能产生缓存文件
- [http://www.ttlsa.com/nginx/nginx-enforce-cache/](http://www.ttlsa.com/nginx/nginx-enforce-cache/) nginx强制缓存
- [http://www.cnblogs.com/dudu/p/4597351.html](http://www.cnblogs.com/dudu/p/4597351.html) 解决nginx反向代理缓存不起作用的问题
- [http://blog.csdn.net/akon_vm/article/details/8494620](http://blog.csdn.net/akon_vm/article/details/8494620) 缓存nginx服务器的静态文件
- [http://www.cnblogs.com/lengfo/p/4260363.html](http://www.cnblogs.com/lengfo/p/4260363.html) 基于nginx tomcat redis分布式web应用的session共享配置
- 本文地址: [http://www.cnblogs.com/wunaozai/p/5001742.html](http://www.cnblogs.com/wunaozai/p/5001742.html)
