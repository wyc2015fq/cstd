# Nginx负载均衡配置实例详解 - gauss的专栏 - CSDN博客
2016年08月26日 23:41:18[gauss](https://me.csdn.net/mathlmx)阅读数：2580
###### 负载均衡是我们大流量网站要做的一个东西，下面我来给大家介绍在NGINX服务器上进行负载均衡配置方法，希望对有需要的同学有所帮助哦。负载均衡先来简单了解一下什么是负载均衡，单从字面上的意思来理解就可以解释N台服务器平均分担负载，不会因为某台服务器负载高宕机而某台服务器闲置的情况。那么负载均衡的前提就是要有多台服务器才能实现，也就是两台以上即可。
直接上成功的参数，实现了本机均衡并本机的NGINX也用起来：
A、主NGINX：
 
#user nobody;
worker_processes 1;
#error_log logs/error.log;
#error_log logs/error.log notice;
#error_log logs/error.log info;
pid /var/run/nginx.pid;
events {
 worker_connections 1024;
}
http {
 include mime.types;
 default_type application/octet-stream;
 #log_format main '$remote_addr - $remote_user [$time_local] "$request" '
 # '$status $body_bytes_sent "$http_referer" '
 # '"$http_user_agent" "$http_x_forwarded_for"';
 #access_log logs/access.log main;
 sendfile on;
 #tcp_nopush on;
 #keepalive_timeout 0;
 keepalive_timeout 65;
 #gzip on;
 
upstream a.com{
# hash $cookie_jsessionid;
 hash $remote_addr consistent;
 server 192.168.142.128:8080 weight=3;
 server 192.168.142.129:80 weight=10;
# hash $cookie_jsessionid;
# server 192.168.142.130:80;
# ip_hash;
# hash_again 1;
# consistent_hash $remote_addr：可以根据客户端ip映射
# consistent_hash $request_uri： 根据客户端请求的uri映射
# consistent_hash $args：根据客户端携带的参数进行映射
 }
server{ 
 listen 80; 
 server_name a.com;
 location / { 
 index index.html; 
 root /usr/html; 
 proxy_pass http://a.com;
 proxy_set_header Host $host;
 proxy_set_header X-Real-IP $remote_addr;
 proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
 }
}
 server {
 listen 8080;
 server_name a.com;
 #access_log logs/host.access.log main;
 location / {
 root html;
 index index.html index.htm index.php;
 }
location ~ .*\.(php|php5)?$
{
fastcgi_pass 127.0.0.1:9000;
fastcgi_index index.php;
fastcgi_param SCRIPT_FILENAME /var/www/blog$fastcgi_script_name; 
include fastcgi.conf;
}
location ~ .*\.(gif|jpg|jpeg|png|bmp|swf)$
{
expires 30d;
}
location ~ .*\.(js|css)?$
{
expires 12h;
}
access_log off;
 
 #error_page 404 /404.html;
 # redirect server error pages to the static page /50x.html
 #
 error_page 500 502 503 504 /50x.html;
 location = /50x.html {
 root html;
 }
 # proxy the PHP scripts to Apache listening on 127.0.0.1:80
 #
 #location ~ \.php$ {
 # proxy_pass http://127.0.0.1;
 #}
 # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
 #
 #location ~ \.php$ {
 # root html;
 # fastcgi_pass 127.0.0.1:9000;
 # fastcgi_index index.php;
 # fastcgi_param SCRIPT_FILENAME /scripts$fastcgi_script_name;
 # include fastcgi_params;
 #}
 # deny access to .htaccess files, if Apache's document root
 # concurs with nginx's one
 #
 #location ~ /\.ht {
 # deny all;
 #}
 }
 # another virtual host using mix of IP-, name-, and port-based configuration
 #
 #server {
 # listen 8000;
 # listen somename:8080;
 # server_name somename alias another.alias;
 # location / {
 # root html;
 # index index.html index.htm;
 # }
 #}
 # HTTPS server
 #
 #server {
 # listen 443 ssl;
 # server_name localhost;
 # ssl_certificate cert.pem;
 # ssl_certificate_key cert.key;
 # ssl_session_cache shared:SSL:1m;
 # ssl_session_timeout 5m;
 # ssl_ciphers HIGH:!aNULL:!MD5;
 # ssl_prefer_server_ciphers on;
 # location / {
 # root html;
 # index index.html index.htm;
 # }
 #}
}
B、NGINX成员
#user nobody;
worker_processes 1;
#error_log logs/error.log;
#error_log logs/error.log notice;
#error_log logs/error.log info;
pid /usr/local/nginx/nginx.pid;
events {
 worker_connections 1024;
}
http {
 include mime.types;
 default_type application/octet-stream;
 #log_format main '$remote_addr - $remote_user [$time_local] "$request" '
 # '$status $body_bytes_sent "$http_referer" '
 # '"$http_user_agent" "$http_x_forwarded_for"';
 #access_log logs/access.log main;
 sendfile on;
 #tcp_nopush on;
 #keepalive_timeout 0;
 keepalive_timeout 65;
 #gzip on;
 server {
 listen 80;
 server_name a.com; 
 index index.html; 
 root /usr/local/nginx/html; 
 #charset koi8-r;
 #access_log logs/host.access.log main;
 location / {
 root html;
 index index.html index.htm index.php;
 }
location ~ .*\.(php|php5)?$
{
fastcgi_pass 127.0.0.1:9000;
fastcgi_index index.php;
fastcgi_param SCRIPT_FILENAME /var/www/blog$fastcgi_script_name;
include fastcgi.conf;
}
location ~ .*\.(gif|jpg|jpeg|png|bmp|swf)$
{
expires 30d;
}
location ~ .*\.(js|css)?$
{
expires 12h;
}
access_log off;
 #error_page 404 /404.html;
 # redirect server error pages to the static page /50x.html
 #
 error_page 500 502 503 504 /50x.html;
 location = /50x.html {
 root html;
 }
 # proxy the PHP scripts to Apache listening on 127.0.0.1:80
 #
 #location ~ \.php$ {
 # proxy_pass http://127.0.0.1;
 #}
 # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
 #
 #location ~ \.php$ {
 # root html;
 # fastcgi_pass 127.0.0.1:9000;
 # fastcgi_index index.php;
 # fastcgi_param SCRIPT_FILENAME /scripts$fastcgi_script_name;
 # include fastcgi_params;
 #}
 # deny access to .htaccess files, if Apache's document root
 # concurs with nginx's one
 #
 #location ~ /\.ht {
 # deny all;
 #}
 }
 # another virtual host using mix of IP-, name-, and port-based configuration
 #
 #server {
 # listen 8000;
 # listen somename:8080;
 # server_name somename alias another.alias;
 # location / {
 # root html;
 # index index.html index.htm;
 # }
 #}
 # HTTPS server
 #
 #server {
 # listen 443 ssl;
 # server_name localhost;
 # ssl_certificate cert.pem;
 # ssl_certificate_key cert.key;
 # ssl_session_cache shared:SSL:1m;
 # ssl_session_timeout 5m;
 # ssl_ciphers HIGH:!aNULL:!MD5;
 # ssl_prefer_server_ciphers on;
 # location / {
 # root html;
 # index index.html index.htm;
 # }
 #}
}
### 一、网上测试环境参考：
由于没有服务器，所以本次测试直接host指定域名，然后在VMware里安装了三台CentOS。
测试域名  ：a.com
A服务器IP ：192.168.5.149 （主）
B服务器IP ：192.168.5.27
C服务器IP ：192.168.5.126
部署思路
A服务器做为主服务器，域名直接解析到A服务器（192.168.5.149）上，由A服务器负载均衡到B服务器（192.168.5.27）与C服务器（192.168.5.126）上。
域名解析
由于不是真实环境，域名就随便使用一个a.com用作测试，所以a.com的解析只能在hosts文件设置。
打开：C:WindowsSystem32driversetchosts
在末尾添加
192.168.5.149   a.com
保存退出，然后启动命令模式ping下看看是否已设置成功
从截图上看已成功将a.com解析到192.168.5.149IP
A服务器[nginx](http://www.php100.com/list-195/).conf设置
打开nginx.conf，文件位置在nginx安装目录的conf目录下。
在http段加入以下代码
u[ps](http://www.php100.com/fw/photo.html)treama.com {
server  192.168.5.126:80;
server  192.168.5.27:80;
}
server{
listen 80;
server_name a.com;
location / {
proxy_pass        http://a.com;
proxy_set_header  Host            $host;
proxy_set_header  X-Real-IP       $remote_addr;
proxy_set_header  X-Forwarded-For  $proxy_add_x_forwarded_for;
}
}
保存重启nginx
B、C服务器nginx.conf设置
打开nginx.confi，在http段加入以下代码
server{
listen 80;
server_name a.com;
index index.html;
root /data0/htdocs/www;
}
保存重启nginx
测试
当访问a.com的时候，为了区分是转向哪台服务器处理我分别在B、C服务器下写一个不同内容的index.html文件，以作区分。
打开浏览器访问a.com结果，刷新会发现所有的请求均分别被主服务器（192.168.5.149）分配到B服务器（192.168.5.27）与C服务器（192.168.5.126）上，实现了负载均衡效果。
B服务器处理页面
C服务器处理页面
假如其中一台服务器宕机会怎样？
当某台服务器宕机了，是否会影响访问呢？
我们先来看看实例，根据以上例子，假设C服务器192.168.5.126这台机子宕机了（由于无法模拟宕机，所以我就把C服务器关机）然后再来访问看看。
访问结果：
我们发现，虽然C服务器（192.168.5.126）宕机了，但不影响网站访问。这样，就不会担心在负载均衡模式下因为某台机子宕机而拖累整个站点了。
如果b.com也要设置负载均衡怎么办？
很简单，跟a.com设置一样。如下：
假设b.com的主服务器IP是192.168.5.149，负载均衡到192.168.5.150和192.168.5.151机器上
现将域名b.com解析到192.168.5.149IP上。
在主服务器(192.168.5.149)的nginx.conf加入以下代码：
upstream b.com {
server  192.168.5.150:80;
server  192.168.5.151:80;
}
server{
listen 80;
server_name b.com;
location / {
proxy_pass        http://b.com;
proxy_set_header  Host            $host;
proxy_set_header  X-Real-IP       $remote_addr;
proxy_set_header  X-Forwarded-For  $proxy_add_x_forwarded_for;
}
}
保存重启nginx
在192.168.5.150与192.168.5.151机器上设置nginx，打开nginx.conf在末尾添加以下代码：
server{
listen 80;
server_name b.com;
index index.html;
root /data0/htdocs/www;
}
保存重启nginx
完成以后步骤后即可实现b.com的负载均衡配置。
主服务器不能提供服务吗？
以上例子中，我们都是应用到了主服务器负载均衡到其它服务器上，那么主服务器本身能不能也加在服务器列表中，这样就不会白白浪费拿一台服务器纯当做转发功能，而是也参与到提供服务中来。
如以上案例三台服务器：
A服务器IP ：192.168.5.149 （主）
B服务器IP ：192.168.5.27
C服务器IP ：192.168.5.126
我们把域名解析到A服务器，然后由A服务器转发到B服务器与C服务器，那么A服务器只做一个转发功能，现在我们让A服务器也提供站点服务。
我们先来分析一下，如果添加主服务器到upstream中，那么可能会有以下两种情况发生：
1、主服务器转发到了其它IP上，其它IP服务器正常处理；
2、主服务器转发到了自己IP上，然后又进到主服务器分配IP那里，假如一直分配到本机，则会造成一个死循环。
怎么解决这个问题呢？因为80端口已经用来监听负载均衡的处理，那么本服务器上就不能再使用80端口来处理a.com的访问请求，得用一个新的。于是我们把主服务器的nginx.conf加入以下一段代码：
server{
listen 8080;
server_name a.com;
index index.html;
root /data0/htdocs/www;
}
重启nginx，在浏览器输入a.com:8080试试看能不能访问。结果可以正常访问
既然能正常访问，那么我们就可以把主服务器添加到upstream中，但是端口要改一下，如下代码：
upstream a.com {
server  192.168.5.126:80;
server  192.168.5.27:80;
server  127.0.0.1:8080;
}
由于这里可以添加主服务器IP192.168.5.149或者127.0.0.1均可以，都表示访问自己。
重启Nginx，然后再来访问a.com看看会不会分配到主服务器上。
主服务器也能正常加入服务了。
最后
一、负载均衡不是nginx独有，著名鼎鼎的[apache](http://www.php100.com/list-121/)也有，但性能可能不如nginx。
二、多台服务器提供服务，但域名只解析到主服务器，而真正的服务器IP不会被ping下即可获得，增加一定安全性。
三、upstream里的IP不一定是内网，外网IP也可以。不过经典的案例是，局域网中某台IP暴露在外网下，域名直接解析到此IP。然后又这台主服务器转发到内网服务器IP中。
四、某台服务器宕机、不会影响网站正常运行，Nginx不会把请求转发到已宕机的IP上
## 二、Nginx负载均衡 理论补充理解
最近迷上了Nginx,真实麻雀虽小,五脏俱全..功能实在强大..
nginx不单可以作为强大的web服务器，也可以作为一个反向代理服务器，而且nginx还可以按照调度规则实现动态、静态页面的分离，可以按照轮询、ip哈希、URL哈希、权重等多种方式对后端服务器做负载均衡，同时还支持后端服务器的健康检查。
如果只有一台服务器时,这个服务器挂了,那么对于网站来说是个灾难.因此，这时候的负载均衡就会大显身手了,它会自动剔除挂掉的服务器.
下面简单的介绍下我使用Nginx做负载的体会
下载---安装Nginx这些不介绍了,前篇有介绍.
windows和Linux下配置Nginx负载的写法一样,故不分开介绍.
Nginx负载均衡一些基础知识:
nginx 的 upstream目前支持 4 种方式的分配
1)、轮询（默认）
每个请求按时间顺序逐一分配到不同的后端服务器，如果后端服务器down掉，能自动剔除。
2)、weight
指定轮询几率，weight和访问比率成正比，用于后端服务器性能不均的情况。
2)、ip_hash
每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题。
3)、fair（第三方）
按后端服务器的响应时间来分配请求，响应时间短的优先分配。
4)、url_hash（第三方）
配置：
在http节点里添加:
#定义负载均衡设备的 Ip及设备状态
upstream myServer {
server 127.0.0.1:9090 down;
server 127.0.0.1:8080 weight=2;
server 127.0.0.1:6060;
server 127.0.0.1:7070 backup;
}
在需要使用负载的Server节点下添加
proxy_pass [http://myServer;](/)
upstream 每个设备的状态:
down 表示单前的server暂时不参与负载
weight  默认为1.weight越大，负载的权重就越大。
max_fails ：允许请求失败的次数默认为1.当超过最大次数时，返回proxy_next_upstream模块定义的错误
fail_timeout:max_fails 次失败后，暂停的时间。
backup： 其它所有的非backup机器down或者忙的时候，请求backup机器。所以这台机器压力会最轻。
Nginx还支持多组的负载均衡,可以配置多个upstream  来服务于不同的Server.
配置负载均衡比较简单,但是最关键的一个问题是怎么实现多台服务器之间session的共享
下面有几种方法(以下内容来源于网络,第四种方法没有实践.)
1) 不使用session，换作cookie
能把session改成cookie，就能避开session的一些弊端，在从前看的一本J2EE的书上，也指明在集群系统中不能用session，否则惹出祸端来就不好办。如果系统不复杂，就优先考虑能否将session去掉，改动起来非常麻烦的话，再用下面的办法。
2) 应用服务器自行实现共享
asp.net可以用数据库或memcached来保存session，从而在asp.net本身建立了一个session集群，用这样的方式可以令session保证稳定，即使某个节点有故障，session也不会丢失，适用于较为严格但请求量不高的场合。但是它的效率是不会很高的，不适用于对效率要求高的场合。
以上两个办法都跟nginx没什么关系，下面来说说用nginx该如何处理：
3) ip_hash
nginx中的ip_hash技术能够将某个ip的请求定向到同一台后端，这样一来这个ip下的某个客户端和某个后端就能建立起稳固的session，ip_hash是在upstream配置中定义的：
upstream backend {
server 127.0.0.1:8080;
server 127.0.0.1:9090;
ip_hash; #此参数本人测试过，是一种方法。
}
ip_hash是容易理解的，但是因为仅仅能用ip这个因子来分配后端，因此ip_hash是有缺陷的，不能在一些情况下使用：
1/nginx不是最前端的服务器。ip_hash要求nginx一定是最前端的服务器，否则nginx得不到正确ip，就不能根据ip作hash。譬如使用的是squid为最前端，那么nginx取ip时只能得到squid的服务器ip地址，用这个地址来作分流是肯定错乱的。
2/nginx的后端还有其它方式的负载均衡。假如nginx后端又有其它负载均衡，将请求又通过另外的方式分流了，那么某个客户端的请求肯定不能定位到同一台session应用服务器上。这么算起来，nginx后端只能直接指向应用服务器，或者再搭一个squid，然后指向应用服务器。最好的办法是用location作一次分流，将需要session的部分请求通过ip_hash分流，剩下的走其它后端去。
4) upstream_hash
为了解决ip_hash的一些问题，可以使用upstream_hash这个第三方模块，这个模块多数情况下是用作url_hash的，但是并不妨碍将它用来做session共享：
假如前端是squid，他会将ip加入x_forwarded_for这个http_header里，用upstream_hash可以用这个头做因子，将请求定向到指定的后端：
可见这篇文档：[http://www.sudone.com/nginx/nginx_url_hash.html](http://www.sudone.com/nginx/nginx_url_hash.html)
在文档中是使用$request_uri做因子，稍微改一下：
hash   $http_x_forwarded_for;
这样就改成了利用x_forwarded_for这个头作因子，在nginx新版本中可支持读取cookie值，所以也可以改成：
hash   $cookie_jsessionid;
假如在php中配置的session为无cookie方式，配合nginx自己的一个userid_module模块就可以用nginx自发一个cookie，可参见userid模块的英文文档：
[http://wiki.nginx.org/NginxHttpUserIdModule](http://wiki.nginx.org/NginxHttpUserIdModule)
另可用姚伟斌编写的模块upstream_jvm_route：[http://code.google.com/p/nginx-upstream-jvm-route/](http://code.google.com/p/nginx-upstream-jvm-route/)
## nginx 负载均衡之 ngx_http_upstream_hash_module
nginx的upstream模块可以定义后端负载集群，负载的分配方式也有好几种，比如ip_hash,RR，weight,url_hash,fair等。如果后端集群session不共享的话，ip_hash,RR,weight,fair等负载均衡方式都将不适用，唯一可用的就是url_hash了。
要用url_hash需要安装第三方模块[ngx_http_upstream_hash_module](http://wiki.nginx.org/HttpUpstreamRequestHashModule) 。安装以及配置方法点击超链接即可。我这里主要介绍如何利用[ngx_http_upstream_hash_module](http://wiki.nginx.org/HttpUpstreamRequestHashModule)对session不共享的集群如何做负载均衡。
upstream的配置如下：
upstream pool1 {
hash  $cookie_jsessionid;
server server1:80;
server server2:80;
server server3:80;
hash_again 1;
}
为了保障同一个用户始终分配到同一个后端服务器，我们需要找到能唯一标示用户的标志，毫无疑问，非sessionid莫属了哈哈。因为服务器端也是根据sessionid区分唯一用户的。所以我们hash用的变量是$cookie_jsessionid。那么hash_again是什么意思呢？顾名思义hash_again就是再做一次hash的意思,那么什么情况下再做一次hash呢？为什么要再做一次hash呢？我们配上access_log来看看nginx都做了些什么。
access_log格式配置如下：
log_format main  ‘$remote_addr – $remote_user [$time_local]“$request” ‘
‘$status $body_bytes_sent ‘
‘”$http_user_agent” “$http_x_forwarded_for”‘
‘”$upstream_addr” “$upstream_cache_status” “$upstream_status”“$upstream_response_time” “$cookie_jsessionid“‘;
我们主要看upstream_addr、upsteam_status和cookie_jsessionid
首先我们访问下index.jsp,日志如下：
123.127.98.133 – -[02/Jul/2011:19:31:42 +0800] “GET /reg/register.jsp HTTP/1.1″ 20014540 “Mozilla/5.0 (Windows NT 5.1; rv:2.0.1)Gecko/20100101 Firefox/4.0.1″ “-”"server1:80” “-” “200” “0.017″“-”
此时访问的是server1sessionid居然为空！其实也可以理解第一次访问时确实木有sessionid，因为sessionid是服务器端生成后写到浏览器的cookie里的，所以第一次访问肯定没有sessionid了。这样一来，我们可以推测所有用户的第一次请求都是发给同一台后端服务器的。这样的话如果恰好宕机的是负责处理第一次用户的请求的服务器的话，那么所有的新访问的用户都将无法访问。
F5刷新页面：
123.127.98.133 – -[02/Jul/2011:19:31:46 +0800] “GET http://abc.efg.com/index.jsp” 2008920 “Mozilla/5.0 (Windows NT 5.1; rv:2.0.1)Gecko/20100101 Firefox/4.0.1″ “-”"server2:80” “-” “200” “0.002″“abcC8pjNRr3jGbAWKNQdt”
有sessionid了，而且请求被重新分配到了server2了，而且之后用户所有的操作都分配到了server2。这个时候我们停掉server2，再次F5刷新页面。日志出如下：
123.127.98.133 – -[02/Jul/2011:19:33:08 +0800] “GET http://abc.efg.com/index.jsp” 20014540 ” “Mozilla/5.0 (Windows NT 5.1; rv:2.0.1) Gecko/20100101Firefox/4.0.1″ “-”"server2:80, server1:80” “-” “502, 200” “0.002,0.015″ “abcC8pjNRr3jGbAWKNQdt”
此时虽然我们已经停掉了server2但是我们仍然可以打开页面，为嘛？！此时hash_again大发神威了，此时我们可以从日志里看到server2返回502,server1返回200。也就是说nginx请求server2是发现，靠！server2挂了，于是基于原来得到的hash值再次进行hash，从而将请求发往了另外一台机器server1。之后所有的请求也都是这样，先请求server2,server2不可用，重新hash，访问server1。那么hash_again=2时代表什么了？如果按照我们的配置hash_again=1，那么当server2和server1都蛋掉，但是server3可用。用户请求时仍然会无法访问。如果我们改成hash_again=2，那么nginx会进行两次hash尝试，尝试访问后端其他可以用的机器。也就是说hash_again的值越大，整个系统的可用性就越高。
