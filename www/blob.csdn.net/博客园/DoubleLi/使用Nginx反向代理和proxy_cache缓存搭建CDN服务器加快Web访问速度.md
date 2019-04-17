# 使用Nginx反向代理和proxy_cache缓存搭建CDN服务器加快Web访问速度 - DoubleLi - 博客园






**碰到问题：**移动用户访问web服务器[www.osyunwei.com](http://www.osyunwei.com/)很慢
**解决办法：**
1、在移动机房放置一台nginx反向代理服务器
2、通过域名DNS[智能](http://lib.csdn.net/base/aiplanning)解析，所有移动用户访问[www.osyunwei.com](http://www.osyunwei.com/)时解析到nginx反向代理服务器
3、nginx反向代理服务器与web服务器之间采用专线连接
系统运维  [www.osyunwei.com](http://www.osyunwei.com/)  温馨提醒：qihang01原创内容©版权所有,转载请注明出处及原文链接
**说明：**
1、web服务器
线路：电信
IP：192.168.21.129
域名：[www.osyunwei.com
2](http://www.osyunwei.com2/)、nginx反向代理服务器
线路：移动
系统：CentOS 6.2
IP：192.168.21.164
vi /etc/hosts   #编辑，在文件最后添加下面一行
192.168.21.129          [www.osyunwei.com
3](http://www.osyunwei.com3/)、客户端
线路：移动
系统：Windows 7
IP:192.168.21.130
C:\Windows\System32\drivers\etc\hosts   #用记事本打开，在文件最后添加下面一行
192.168.21.164          [www.osyunwei.com](http://www.osyunwei.com/)

**###################以下操作在nginx反向代理服务器上配置###################**

**1、关闭SELinux**
vi /etc/selinux/config
#SELINUX=enforcing     #注释掉
#SELINUXTYPE=targeted  #注释掉
SELINUX=disabled  #增加
:wq  保存，关闭。
shutdown -r now重启系统
**2、开启防火墙80端口**
vi /etc/sysconfig/iptables
添加下面的内容
-A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT
/etc/init.d/iptables restart  #重启防火墙使配置生效
**3、安装编译工具**
系统运维  [www.osyunwei.com](http://www.osyunwei.com/)  温馨提醒：qihang01原创内容©版权所有,转载请注明出处及原文链接
yum install wget  make gcc gcc-c++ zlib-devel openssl openssl-devel pcre-devel gd  kernel keyutils  patch perl
**4 、系统约定**
软件源代码包存放位置：/usr/local/src
源码包编译安装位置：/usr/local/软件名字
**5、下载软件**
cd /usr/local/src  #进入目录
（一）、下载nginx（目前稳定版）
wget [http://nginx.org/download/nginx-1.0.12.tar.gz](http://nginx.org/download/nginx-1.0.12.tar.gz)
（二）、下载pcre  （支持nginx伪静态）
wget [ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.21.tar.gz](ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.21.tar.gz)
（二）、下载ngx_cache_purge（清除指定URL缓存）
wget [http://labs.frickle.com/files/ngx_cache_purge-1.5.tar.gz](http://labs.frickle.com/files/ngx_cache_purge-1.5.tar.gz)
**6、安装pcre**
cd /usr/local/src
mkdir /usr/local/pcre    #创建安装目录
tar  zxvf pcre-8.21.tar.gz
cd pcre-8.21
./configure  --prefix=/usr/local/pcre   #配置
make
make install
**7、安装 nginx**
groupadd  www   #添加www组
useradd -g www www -s /bin/false   #创建nginx运行账户www并加入到www组，不允许www用户直接登录系统
cd /usr/local/src
tar  zxvf  ngx_cache_purge-1.5.tar.gz
tar  zxvf nginx-1.0.12.tar.gz
cd nginx-1.0.12
./configure --prefix=/usr/local/nginx  --user=www --group=www  --with-http_stub_status_module --with-http_realip_module --with-openssl=/usr/ --with-pcre=/usr/local/src/pcre-8.21  --add-module=../ngx_cache_purge-1.5

注意:--with-pcre=/usr/local/src/pcre-8.21指向的是源码包解压的路径，而不是安装的路径，否则会报错
make   #编译
make install   #安装
/usr/local/nginx/sbin/nginx    #启动nginx
chown www.www -R /usr/local/nginx/html   #设置目录所有者
chmod 700 -R /usr/local/nginx/html    #设置目录权限
**  vi /etc/rc.d/init.d/nginx    # 设置nginx开启启动，编辑启动文件添加下面内容**
=======================================================
#!/bin/bash
# nginx Startup script for the Nginx HTTP Server
# it is v.0.0.2 version.
# chkconfig: - 85 15
# description: Nginx is a high-performance web and proxy server.
#              It has a lot of features, but it's not for everyone.
# processname: nginx
# pidfile: /var/run/nginx.pid
# config: /usr/local/nginx/conf/nginx.conf
nginxd=/usr/local/nginx/sbin/nginx
nginx_config=/usr/local/nginx/conf/nginx.conf
nginx_pid=/usr/local/nginx/logs/nginx.pid
RETVAL=0
prog="nginx"
# Source function library.
.  /etc/rc.d/init.d/functions
# Source networking configuration.
.  /etc/sysconfig/network
# Check that networking is up.
[ ${NETWORKING} = "no" ] && exit 0
[ -x $nginxd ] || exit 0
# Start nginx daemons functions.
start() {
if [ -e $nginx_pid ];then
echo "nginx already running...."
exit 1
fi
echo -n $"Starting $prog: "
daemon $nginxd -c ${nginx_config}
RETVAL=$?
echo
[ $RETVAL = 0 ] && touch /var/lock/subsys/nginx
return $RETVAL
}
# Stop nginx daemons functions.
stop() {
echo -n $"Stopping $prog: "
killproc $nginxd
RETVAL=$?
echo
[ $RETVAL = 0 ] && rm -f /var/lock/subsys/nginx /usr/local/nginx/logs/nginx.pid
}
reload() {
echo -n $"Reloading $prog: "
#kill -HUP `cat ${nginx_pid}`
killproc $nginxd -HUP
RETVAL=$?
echo
}
# See how we were called.
case "$1" in
start)
start
;;
stop)
stop
;;
reload)
reload
;;
restart)
stop
start
;;

status)
status $prog
RETVAL=$?
;;
*)
echo $"Usage: $prog {start|stop|restart|reload|status|help}"
exit 1
esac
exit $RETVAL
=======================================================
:wq!保存退出
chmod 775 /etc/rc.d/init.d/nginx  #赋予文件执行权限
chkconfig nginx on    #设置开机启动
/etc/rc.d/init.d/nginx restart
service nginx restart
**8、配置nginx**
cp /usr/local/nginx/conf/nginx.conf  /usr/local/nginx/conf/nginx.confbak  #备份nginx配置文件
（一）、设置nginx运行账号
vi /usr/local/nginx/conf/nginx.conf   #编辑
找到user nobody;修改为
user www www;    #在第一行
（二）、禁止nginx空主机头
vi /usr/local/nginx/conf/nginx.conf   #编辑
找到server，在上面一行添加如下内容：
##############################
server {
listen       80 default;
server_name  _;
location / {
root   html;
return 404;
}
location ~ /.ht {
deny  all;
}
}
##############################
/etc/rc.d/init.d/nginx restart     #重启nginx
这样设置之后，空主机头访问会直接跳转到nginx404错误页面。
（三）、添加nginx虚拟主机包含文件
cd /usr/local/nginx/conf/   #进入nginx安装目录
mkdir vhost   #建立虚拟目录
vi  /usr/local/nginx/conf/nginx.conf   #编辑
找到上一步添加的代码，在最后添加如下内容：
include  vhost/*.conf;
例如：
##############################
server {
listen       80 default;
server_name  _;
location / {
root   html;
return 404;
}
location ~ /.ht {
deny  all;
}
}
include  vhost/*.conf;
##############################
（四）、添加proxy_cache参数配置包含文件
cd  /usr/local/nginx/conf/   #进入目录
touch proxy.conf   #建立文件
vi  /usr/local/nginx/conf/nginx.conf   #编辑
找到http {  在下面添加一行
include proxy.conf;
（五）、添加被代理服务器列表包含文件
cd  /usr/local/nginx/conf/   #进入目录
touch  mysvrhost.conf  #建立文件
vi  /usr/local/nginx/conf/nginx.conf   #编辑
找到上一步添加的代码，在下面添加一行
include  mysvrhost.conf;
（六）、设置nginx全局参数
vi  /usr/local/nginx/conf/nginx.conf   #编辑
worker_processes 2;       # 工作进程数,为CPU的核心数或者两倍
events
{
use epoll;   #增加
worker_connections 65535;    #修改为65535，最大连接数。
}
#############以下代码在http { 部分增加与修改##############
server_names_hash_bucket_size 128;   #增加
client_header_buffer_size 32k;       #增加
large_client_header_buffers 4 32k;   #增加
client_max_body_size 300m;           #增加
tcp_nopush     on;      #修改为on
keepalive_timeout  60;  #修改为60
tcp_nodelay on;        #增加
server_tokens off;     #增加，不显示nginx版本信息
gzip  on;  #修改为on
gzip_min_length  1k;      #增加
gzip_buffers     4 16k;   #增加
gzip_http_version 1.1;    #增加
gzip_comp_level 2;        #增加
gzip_types       text/plain application/x-[JavaScript](http://lib.csdn.net/base/javascript) text/css application/xml;  #增加
gzip_vary on;  #增加
（七）、设置proxy_cache参数配置
cd /home  #进入目录
mkdir -p  /home/proxy_temp_dir     #proxy_temp_dir与proxy_cache_dir这两个文件夹必须在同一个分区
mkdir -p  /home/proxy_cache_dir    #proxy_cache_dir与proxy_temp_dir这两个文件夹必须在同一个分区
chown www.www -R  proxy_cache_dir  proxy_temp_dir  #设置目录所有者
chmod -R 777  proxy_cache_dir  proxy_temp_dir    #设置目录权限
系统运维  [www.osyunwei.com](http://www.osyunwei.com/)  温馨提醒：qihang01原创内容©版权所有,转载请注明出处及原文链
cd  /usr/local/nginx/conf/   #进入目录
vi  proxy.conf   #编辑，添加以下代码
proxy_temp_path   /home/proxy_temp_dir;   #指定临时文件目录
proxy_cache_path  /home/proxy_cache_dir levels=1:2 keys_zone=cache_one:50m inactive=1d max_size=1g;
#设置Web缓存区名称为cache_one，内存缓存为50MB，自动清除1天内没有被访问的文件，硬盘缓存为1GB。
client_body_buffer_size  512k; #增加缓冲区代理缓冲客户端请求的最大字节数
proxy_connect_timeout    60;   #增加连接后端服务器超时时间
proxy_read_timeout       60;   #增加后端服务器响应请求超时时间
proxy_send_timeout       60;   #增加后端服务器发送数据超时时间
proxy_buffer_size        32k;   #增加代理请求缓存区大小
proxy_buffers           4 64k;  #增加
proxy_busy_buffers_size 128k;   #增加系统繁忙时可申请的proxy_buffers大小
proxy_temp_file_write_size 128k;  #增加proxy缓存临时文件的大小
proxy_next_upstream error timeout invalid_header http_500 http_502 http_503 http_404;   #增加故障转移，如果后端的服务器返回502、504、执行超时等错误，自动将请求转发到upstream负载均衡池中的另一台服务器，实现故障转移。proxy_cache cache_one;  #增加使用web缓存区cache_one
（八）、设置被代理服务器文件列表
cd  /usr/local/nginx/conf/   #进入目录
vi mysvrhost.conf  #编辑，添加以下代码
upstream osyunweihost {
ip_hash;
server 192.168.21.129:80 weight=1 max_fails=2 fail_timeout=30s;
   }
（九）、新建虚拟主机配置文件
   cd /usr/local/nginx/conf/vhost   #进入虚拟主机目录
touch  [www.osyunwei.com.conf](http://www.osyunwei.com.conf/)   #建立虚拟主机配置文件
vi  [www.osyunwei.com.conf](http://www.osyunwei.com.conf/)   #编辑

server {
listen       80;
server_name  [www.osyunwei.com](http://www.osyunwei.com/) osyunwei.com;

location /
{
proxy_pass [http://osyunweihost](http://osyunweihost/);
proxy_cache_key $host$uri$is_args$args;  #增加设置web缓存的key值，nginx根据key值md5哈希存储缓存
proxy_set_header Host  $host;
proxy_set_header X-Forwarded-For  $remote_addr;
proxy_cache_valid  200 304 12h;
expires      2d;
proxy_ignore_headers "Cache-Control" "Expires" "Set-Cookie";

}
location ~ .*\.([PHP](http://lib.csdn.net/base/php)|jsp|cgi|asp|aspx|flv|swf|xml)?$    #列出的扩展名文件不缓存。

{
proxy_set_header Host  $host;
proxy_set_header X-Forwarded-For  $remote_addr;
proxy_pass [http://osyunweihost](http://osyunweihost/);
}
access_log  off;
}

location ~ /purge(/.*)   #用于清除缓存
{
allow            127.0.0.1;
allow            192.168.21.0/24;   #设置只允许指定的IP或IP段才可以清除URL缓存。
deny            all;
proxy_cache_purge    cache_one   $host$1$is_args$args;
}
**###################以上操作在nginx反向代理服务器上配置###################**
**9、ngx_cache_pure清除缓存模块使用说明**
      说明：根据配置只允许192.168.21.0/24 IP段的主机才可以清除URL缓存，现在我使用的客户机IP是：192.168.21.130，有权限清除URL缓存。

1、浏览图片文件：http://www.osyunwei.com/images/nopic.gif

![](http://www.osyunwei.com/wp-content/uploads/2012/02/ngx_cache_pure_old.jpg)

2、清除这个文件缓存：http://www.osyunwei.com/purge/images/nopic.gif

![](http://www.osyunwei.com/wp-content/uploads/2012/02/ngx_cache_pure_new.jpg)

提示：Successful purge，缓存文件清除成功，如果这个文件没有被缓存过，则提示：404 Not Found

![](http://www.osyunwei.com/wp-content/uploads/2012/02/ngx_cache_pure_ok.jpg)

**备注：**
      1、purge是ngx_cache_pure 模块指令
      2、images/nopic.gif 是要清除的缓存文件URL路径

**至此，使用Nginx反向代理和proxy_cache缓存功能配置CDN服务器教程结束。**

**附件：**

**1、nginx配置文件/usr/local/nginx/conf/nginx.conf**



|`1`|`user  www www;`|
|----|----|


|`2`|`worker_processes  2;`|
|----|----|


|`3`|`#``error_log``logs/error.log;`|
|----|----|


|`4`|`#``error_log``logs/error.log  notice;`|
|----|----|


|`5`|`#``error_log``logs/error.log  info;`|
|----|----|


|`6`|`#pid        logs/nginx.pid;`|
|----|----|


|`7`||
|----|----|


|`8`|`events {`|
|----|----|


|`9`|`use``epoll;`|
|----|----|


|`10`|`worker_connections  65535;`|
|----|----|


|`11`|`}`|
|----|----|


|`12`||
|----|----|


|`13`|`http {`|
|----|----|


|`14`|`include``proxy.conf;`|
|----|----|


|`15`|`include``mysvrhost.conf;`|
|----|----|


|`16`|`include``mime.types;`|
|----|----|


|`17`|`default_type  application/octet-stream;`|
|----|----|


|`18`||
|----|----|


|`19`|`#log_format  main  ``'$remote_addr - $remote_user [$time_local] "$request" '`|
|----|----|


|`20`|`#                  ``'$status $body_bytes_sent "$http_referer" '`|
|----|----|


|`21`|`#                  ``'"$http_user_agent" "$http_x_forwarded_for"'``;`|
|----|----|


|`22`||
|----|----|


|`23`|`#access_log  logs/access.log  main;`|
|----|----|


|`24`||
|----|----|


|`25`|`server_names_hash_bucket_size 128;`|
|----|----|


|`26`|`client_header_buffer_size 32k;`|
|----|----|


|`27`|`large_client_header_buffers 4 32k;`|
|----|----|


|`28`|`client_max_body_size 300m;`|
|----|----|


|`29`|`sendfile        on;`|
|----|----|


|`30`|`tcp_nopush     on;`|
|----|----|


|`31`||
|----|----|


|`32`|`#keepalive_timeout  0;`|
|----|----|


|`33`|`keepalive_timeout  60;`|
|----|----|


|`34`|`tcp_nodelay on;`|
|----|----|


|`35`|`server_tokens off;`|
|----|----|


|`36`||
|----|----|


|`37`|`gzip  on;`|
|----|----|


|`38`|`gzip_min_length  1k;`|
|----|----|


|`39`|`gzip_buffers     4 16k;`|
|----|----|


|`40`|`gzip_http_version 1.1;`|
|----|----|


|`41`|`gzip_comp_level 2;`|
|----|----|


|`42`|`gzip_types       text/plain application/x-javascript text/css application/xml;`|
|----|----|


|`43`|`gzip_vary on;`|
|----|----|


|`44`||
|----|----|


|`45`|`server {`|
|----|----|


|`46`|`listen       80 ``default``;`|
|----|----|


|`47`|`server_name  _;`|
|----|----|


|`48`|`location / {`|
|----|----|


|`49`|`root   html;`|
|----|----|


|`50`|`return``404;`|
|----|----|


|`51`|`}`|
|----|----|


|`52`|`location ~ /.ht {`|
|----|----|


|`53`|`deny  all;`|
|----|----|


|`54`|`}`|
|----|----|


|`55`|`}`|
|----|----|


|`56`|`include``vhost/*.conf;`|
|----|----|


|`57`|`}`|
|----|----|




**2、被代理服务器列表文件/usr/local/nginx/conf/mysvrhost.conf**



|`1`|`upstream osyunweihost {`|
|----|----|


|`2`|`ip_hash;`|
|----|----|


|`3`|`server 192.168.21.129:80 weight=1 max_fails=2 fail_timeout=30s;`|
|----|----|


|`4`|`}`|
|----|----|




**3、proxy_cache参数配置文件/usr/local/nginx/conf/proxy.conf**



|`1`|`proxy_temp_path   /home/proxy_temp_dir;`|
|----|----|


|`2`|`proxy_cache_path  /home/proxy_cache_dir levels=1:2 keys_zone=cache_one:500m inactive=1d max_size=30g;`|
|----|----|


|`3`|`client_body_buffer_size  512k;`|
|----|----|


|`4`|`proxy_connect_timeout    60;`|
|----|----|


|`5`|`proxy_read_timeout       60;`|
|----|----|


|`6`|`proxy_send_timeout       60;`|
|----|----|


|`7`|`proxy_buffer_size        32k;`|
|----|----|


|`8`|`proxy_buffers           4 64k;`|
|----|----|


|`9`|`proxy_busy_buffers_size 128k;`|
|----|----|


|`10`|`proxy_temp_file_write_size 128k;`|
|----|----|


|`11`|`proxy_next_upstream error timeout invalid_header http_500 http_502 http_503 http_404;`|
|----|----|


|`12`|`proxy_cache cache_one;`|
|----|----|




**4、虚拟主机配置文件/usr/local/nginx/conf/vhost/www.osyunwei.com.conf**



|`1`|`server {`|
|----|----|


|`2`|`listen       80;`|
|----|----|


|`3`|`server_name  www.osyunwei.com osyunwei.com;`|
|----|----|


|`4`|`location /`|
|----|----|


|`5`|`{`|
|----|----|


|`6`|`proxy_pass http:``//osyunweihost;`|
|----|----|


|`7`|`proxy_cache_key ``$host``$uri``$is_args``$args``;`|
|----|----|


|`8`|`proxy_set_header Host  ``$host``;`|
|----|----|


|`9`|`proxy_set_header X-Forwarded-For  ``$remote_addr``;`|
|----|----|


|`10`|`proxy_cache_valid  200 304 12h;`|
|----|----|


|`11`|`expires      2d;`|
|----|----|


|`12`|`proxy_ignore_headers ``"Cache-Control"``"Expires"``"Set-Cookie"``;`|
|----|----|


|`13`|`}`|
|----|----|


|`14`||
|----|----|


|`15`|`location ~ /purge(/.*)`|
|----|----|


|`16`|`{`|
|----|----|


|`17`|`allow            127.0.0.1;`|
|----|----|


|`18`|`allow            192.168.21.0/24;`|
|----|----|


|`19`|`deny            all;`|
|----|----|


|`20`|`proxy_cache_purge    cache_one   ``$host``$1``$is_args``$args``;`|
|----|----|


|`21`|`}`|
|----|----|


|`22`||
|----|----|


|`23`|`location ~ .*\.(php|jsp|cgi|asp|aspx|flv|swf|xml)?$`|
|----|----|


|`24`|`{`|
|----|----|


|`25`|`proxy_set_header Host  ``$host``;`|
|----|----|


|`26`|`proxy_set_header X-Forwarded-For  ``$remote_addr``;`|
|----|----|


|`27`|`proxy_pass http:``//osyunweihost;`|
|----|----|


|`28`|`}`|
|----|----|


|`29`|`access_log  off;`|
|----|----|


|`30`|`}`|
|----|----|




**扩展阅读：**
#################################################################
nginx修改版本等信息
1、vi /usr/local/src/nginx-1.0.12/src/core/nginx.h   #编译前编辑
#define nginx_version
#define NGINX_VERSION
#define NGINX_VER
#define NGINX_VAR
修改上面的信息，即可更改nginx显示版本。

2、vi /usr/local/src/nginx-1.0.12/src/http/ngx_http_special_response.c    #编译前编辑
static u_char ngx_http_error_full_tail[] =
static u_char ngx_http_error_tail[] =
修改上面的信息为你自己的。


3、vi /usr/local/src/nginx-1.0.12/src/http/ngx_http_header_filter_module.c    #编译前编辑
static char ngx_http_server_string[]=
修改上面的信息为你自己的。


4、编译完成之后，修改/usr/local/nginx/conf目录下面
fastcgi.conf、fastcgi.conf.default、fastcgi_params、fastcgi_params.default
这四个文件里面的版本名称
/usr/local/nginx/sbin/nginx -V  #查看nginx版本号
#################################################################

![](http://www.osyunwei.com/images/weixin.png)

» 本文链接：[http://www.osyunwei.com/archives/2084.html](http://www.osyunwei.com/archives/2084.html)

» 订阅本站：[http://www.osyunwei.com/feed](http://www.osyunwei.com/feed)

» 转载请注明来源：[系统运维](http://www.osyunwei.com/) » [使用Nginx反向代理和proxy_cache缓存搭建CDN服务器加快Web访问速度](http://www.osyunwei.com/archives/2084.html)









