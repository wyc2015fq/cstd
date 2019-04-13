
# Nginx反向代理及负载均衡实现过程 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月17日 15:08:42[Jlins](https://me.csdn.net/dyllove98)阅读数：1531个人分类：[linux																](https://blog.csdn.net/dyllove98/article/category/1354777)


首先 安装nginx
.各节点时间同步
|1
|2
|3
|[root@nginx
 ~]|\#
 ntpdate 202.120.2.101
|[root@web1
 ~]|\#
 ntpdate 202.120.2.101
|[root@web2
 ~]|\#
 ntpdate 202.120.2.101
|
6.关闭防火墙与SELinux
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|[root@nginx
 ~]|\#
 service iptables stop
|[root@nginx
 ~]|\#
 chkconfig iptables off
|[root@nginx
 ~]|\#
 getenforce
|Disabled
|[root@web1
 ~]|\#
 service iptables stop
|[root@web1
 ~]|\#
 chkconfig iptables off
|[root@web1
 ~]|\#
 getenforce
|Disabled
|[root@web2
 ~]|\#
 service iptables stop
|[root@web2
 ~]|\#
 chkconfig iptables off
|[root@web2
 ~]|\#
 getenforce
|Disabled
|
安装依赖包
\#yum install gcc pcre pcre-devel zlib zlib-devel
 openssl openssl-devel
下载nginx
\#cd /usr/local/src
\#wget http://www.nginx.org/download/nginx-1.0.9.tar.gz
\#tar zxvf nginx-1.0.9.tar.gz
\#cd nginx-1.0.9
配置安装：
\#./configure --prefix=/usr --sbin-path=/usr/sbin/nginx
 --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --pid-path=/var/run/nginx/nginx.pid --lock-path=/var/lock/nginx.lock
 --user=nginx --group=nginx --with-http_ssl_module --with-http_flv_module --with-http_gzip_static_module --http-log-path=/var/log/nginx/access.log
 --http-client-body-temp-path=/var/tmp/nginx/client/ --http-proxy-temp-path=/var/tmp/nginx/proxy/ --http-fastcgi-temp-path=/var/tmp/nginx/fcgi/
\#make
\#make install
2.新建nginx用户与组
|1
|2
|3
|4
|[root@nginx
 src]|\#
 groupadd -g 108  -r nginx
|[root@nginx
 src]|\#
 useradd -u 108 -r -g 108 nginx
|[root@nginx
 src]|\#
 id nginx
|uid=108(nginx)
 gid=108(nginx) 组=108(nginx)
|

为nginx提供SysV init脚本
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|76
|77
|78
|79
|80
|81
|82
|83
|84
|85
|86
|87
|88
|89
|90
|91
|92
|93
|94
|95
|96
|97
|98
|99
|100
|101
|102
|103
|104
|105
|106
|107
|108
|109
|110
|[root@nginx
 ~]|\#
 cat /etc/init.d/nginx
|\#!/bin/sh
|\#
|\#
 nginx - this script starts and stops the nginx daemon
|\#
|\#
 chkconfig:   - 85 15
|\#
 description:  Nginx is an HTTP(S) server, HTTP(S) reverse \
|\#              
 proxy and IMAP/POP3 proxy server
|\#
 processname: nginx
|\#
 config:      /etc/nginx/nginx.conf
|\#
 config:      /etc/sysconfig/nginx
|\#
 pidfile:     /var/run/nginx.pid
|\#
 Source function library.
|.|/etc/rc|.d|/init|.d|/functions
|\#
 Source networking configuration.
|.|/etc/sysconfig/network
|\#
 Check that networking is up.
|[|"$NETWORKING"|=|"no"|]
 &&|exit|0
|nginx=|"/usr/sbin/nginx"
|prog=$(|basename|$nginx)
|NGINX_CONF_FILE=|"/etc/nginx/nginx.conf"
|[ -f|/etc/sysconfig/nginx|]
 && .|/etc/sysconfig/nginx
|lockfile=|/var/lock/subsys/nginx
|make_dirs()
 {
|\#
 make required directories
|user=`nginx
 -V 2>&1 ||grep|"configure
 arguments:"|||sed|'s/[^*]*--user=\([^
 ]*\).*/\1/g'|-`
|options=`$nginx
 -V 2>&1 ||grep|'configure
 arguments:'|`
|for|opt|in|$options;|do
|if|[
 `|echo|$opt
 ||grep|'.*-temp-path'|`
 ];|then
|value=`|echo|$opt
 ||cut|-d|"="|-f
 2`
|if|[
 ! -d|"$value"|];|then
|\#
 echo "creating" $value
|mkdir|-p
 $value &&|chown|-R
 $user $value
|fi
|fi
|done
|}
|start()
 {
|[
 -x $nginx ] |||exit|5
|[
 -f $NGINX_CONF_FILE ] |||exit|6
|make_dirs
|echo|-n
 $|"Starting
 $prog: "
|daemon
 $nginx -c $NGINX_CONF_FILE
|retval=$?
|echo
|[
 $retval -|eq|0
 ] &&|touch|$lockfile
|return|$retval
|}
|stop()
 {
|echo|-n
 $|"Stopping
 $prog: "
|killproc
 $prog -QUIT
|retval=$?
|echo
|[
 $retval -|eq|0
 ] &&|rm|-f
 $lockfile
|return|$retval
|}
|restart()
 {
|configtest
 |||return|$?
|stop
|sleep|1
|start
|}
|reload()
 {
|configtest
 |||return|$?
|echo|-n
 $|"Reloading
 $prog: "
|killproc
 $nginx -HUP
|RETVAL=$?
|echo
|}
|force_reload()
 {
|restart
|}
|configtest()
 {
|$nginx
 -t -c $NGINX_CONF_FILE
|}
|rh_status()
 {
|status
 $prog
|}
|rh_status_q()
 {
|rh_status
 >|/dev/null|2>&1
|}
|case|"$1"|in
|start)
|rh_status_q
 &&|exit|0
|$1
|;;
|stop)
|rh_status_q
 |||exit|0
|$1
|;;
|restart|configtest)
|$1
|;;
|reload)
|rh_status_q
 |||exit|7
|$1
|;;
|force-reload)
|force_reload
|;;
|status)
|rh_status
|;;
|condrestart|try-restart)
|rh_status_q
 |||exit|0
|;;
|*)
|echo|$|"Usage:
 $0 {start|stop|status|restart|condrestart|try-restart|reload|force-reload|configtest}"
|exit|2
|esac
|
为此脚本赋予执行权限
|1
|[root@nginx
 ~]|\#
 chmod +x /etc/init.d/nginx
|
添加至服务管理列表，并让其开机自动启动
|1
|2
|3
|4
|[root@nginx
 ~]|\#
 chkconfig --add nginx
|[root@nginx
 ~]|\#
 chkconfig nginx on
|[root@nginx
 ~]|\#
 chkconfig nginx --list
|nginx             
 0:关闭    1:关闭    2:启用    3:启用    4:启用    5:启用    6:关闭
|
8.启动nginx
|1
|2
|[root@nginx
 ~]|\#
 service nginx start
|正在启动 nginx：
|
查看一下端口
|1
|2
|[root@nginx
 ~]|\#
 netstat -ntlp | grep :80
|
备份  cp /etc/nginx/nginx.conf /etc/nginx/nginx.conf.bak
反向代理
[root@localhost nginx-1.0.9]\# mkdir /usr/html/forum
[root@localhost nginx-1.0.9]\# vi /usr/html/forum/index.html
然后 通过[http://192.168.16.246/forum/](http://192.168.16.246/forum/)访问页面
在 另一台（服务器 httpd配置如下）
[root@web1 ~]\# cd /var/www/html/
mkdir bbs
vi /var/www/html/bbs/index.html

添加
location /forum/ {
proxy_pass[http://192.168.16.230/bbs;](http://192.168.16.230/bbs;)
proxy_set_header  X-Real-IP  $remote_addr;
}
![wKioL1RoNpry4be6AADYBNRe3B4042.jpg](http://s3.51cto.com/wyfs02/M00/53/76/wKioL1RoNpry4be6AADYBNRe3B4042.jpg)
指令说明：proxy_set_header
语法：proxy_set_header
 header value
默认值： Host and Connection
使用字段：http, server, location
这个指令允许将发送到被代理服务器的请求头重新定义或者增加一些字段。这个值可以是一个文本，变量或者它们的组合。proxy_set_header在指定的字段中没有定义时会从它的上级字段继承。

这样我每次转发给后端的时候 后端都会有一个首部叫X-Real-IP 这个X-Real-IP 也会被传送至客户端
接下来 我们修改vi /etc/httpd/conf/httpd.conf
把LogFormat "%h%l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-Agent}i\"" combined
修改为：LogFormat "%{X-Real-IP}i %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-Agent}i\"" combined
这样在访问web服务器时记录的 不是nginx转发器的ip 而是访问用户的ip 有利于日志分析
![wKiom1RoIFqjU3cCAAEzakjsFKo645.jpg](http://s3.51cto.com/wyfs02/M02/53/77/wKiom1RoIFqjU3cCAAEzakjsFKo645.jpg)
这是将/forum 下的文件转发到 另一台web服务器上 万一后端的web 扛不住了 怎么办》？
负载均衡

安装scp的软件包：
\# yum install openssh-clients
这是我学习的一个命令 scp  2端都要安装openssh
scp /var/www/html/bbs/index.html 192.168.16.240:/var/www/html/ 这样就复制到240上去了 只要做下修改就行
web1  mv /var/www/html/bbs/index.html /var/www/html/index.html
upstream 负载均衡模块说明
在service上添加如下
upstream webserver{
server 192.168.16.230 weight=1;
server 192.168.16.240 weight=1;
}
这里注释掉默认的location 具体如下图
location / {
proxy_pass http://webserver/;
proxy_set_header  X-Real-IP $remote_addr;
}

![wKiom1RoPgmhjLHFAAJGQv7W-r4437.jpg](http://s3.51cto.com/wyfs02/M01/53/78/wKiom1RoPgmhjLHFAAJGQv7W-r4437.jpg)
这样当你访问 192.168.16.246时 刷新第一个页面是web1的页面 刷新第二个页面是第二个web2页面

如果不幸某天web机器down了 nginx 有自带的健康检查功能 可以自己自动设置秒数 默认为10秒
upstream webserver{
server 192.168.16.230 weight=1;max_fails=2 fail_timeout=2;
server 192.168.16.240 weight=1;max_fails=2 fail_timeout=2;
}
我们这里设置的是4秒时间
这样我们把web2 down了 ；刷新的页面永远是web1  web2启用 他又自动循环
假如 2台web服务器down机了那么会带来用户体验的降低我们需要定义sorry页面。。。
这里我就用本地的8080端口来区别
创建一个虚拟主机
server {
listen 8080;
server_name 127.0.0.1;
root /web/errorpages;
index index.html;
}
然后创建mkdir -pv /web/errorpages 目录
vi index.html页面。。。。。
接下来添加
upstream webserver{
server 192.168.16.230 weight=1 max_fails=2 fail_timeout=2;
server 192.168.16.240 weight=1 max_fails=2 fail_timeout=2;
server 127.0.0.1:8080 backup;
这样down掉负载2台  接着访问192.168.16.246 的页面 会如图下
![wKioL1RoUNzz_HgoAAC_DAHT9pE331.jpg](http://s3.51cto.com/wyfs02/M00/53/77/wKioL1RoUNzz_HgoAAC_DAHT9pE331.jpg)
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

