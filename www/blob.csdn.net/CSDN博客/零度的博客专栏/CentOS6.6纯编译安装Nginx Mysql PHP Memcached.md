# CentOS6.6纯编译安装Nginx Mysql PHP Memcached - 零度的博客专栏 - CSDN博客
2017年03月20日 15:54:27[零度anngle](https://me.csdn.net/zmx729618)阅读数：501标签：[mysql																[nginx																[php																[centos																[memcached](https://so.csdn.net/so/search/s.do?q=memcached&t=blog)](https://so.csdn.net/so/search/s.do?q=centos&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[PHP](https://blog.csdn.net/zmx729618/article/category/6243603)
声明：部分编译指令在博客编辑器里好像被处理了,如双横线变成单横线了等等，于是在本地生成了一个pdf版本，在下面地址可以下载。 
[LNMP+Memcached](http://pan.baidu.com/s/1jGoXnAe)
CentOS是红帽发行的免费的稳定Linux版本，在企业服务器应用中都会选用Minimal版本，因为Minimal是CentOS”最纯洁”的服务器系统，因为Minimal版本连vim编辑器都需要自己安装，Minimal组件最少，无桌面，扩展灵活，非常适合做服务器。
1.配置网卡
Minimal版本的CentOS被安装后，网卡驱动默认是down状态，需要手动激活，在连接好网线后需要执行命令: 
[root@jhq0113 soft]# vi /etc/sysconfig/network-scripts/ifcfg-eth0
然后按i进入编辑状态，进行如下修改：
![这里写图片描述](https://img-blog.csdn.net/20150214020515482)
修改完毕后，按ESC，然后shift+:，wq进行保存退出。
然后开始配置DNS服务器地址：
[root@jhq0113 soft]# vi /etc/resolv.conf 
最主要的是配置nameserver属性，可以是自己的网关地址，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20150214020549524)
修改完毕后保存退出。键入以下命令激活网卡：
[root@jhq0113 soft]# ifconfig eth0 up
![这里写图片描述](https://img-blog.csdn.net/20150214020634667)
至此，网卡已经配置好了，如果仍然发现开机网卡未启动等问题，可以执行以下两条命令进行完善。
[root@jhq0113 soft]# chkconfig –add network 
[root@jhq0113 soft]# chkconfig network on
重启网卡命令： 
![这里写图片描述](https://img-blog.csdn.net/20150214020659303)
2.配置防火墙，开放22(SSH),3306([MySQL](http://lib.csdn.net/base/14)),80(Nginx),11211(Memcached),6379(Redis)端口
[root@jhq0113 soft]# vi /etc/sysconfig/iptables 
修改为：
![这里写图片描述](https://img-blog.csdn.net/20150214020733636)
重启防火墙使配置生效: 
[root@jhq0113 soft]# /etc/init.d/iptables restart
3.安装组件 
更新系统： 
[root@jhq0113 soft]# yum update 
稍后键入y进行确认。
安装vim： 
[root@jhq0113 soft]# yum install vim
安装编译工具及依赖： 
[root@jhq0113 soft]# yum install make apr* autoconf automake curl-devel gcc gcc-c++ zlib-devel openssl openssl-devel pcre-devel gd kernel keyutils patch perl kernel-headers compat* mpfr cpp glibc libgomp libstdc++-devel ppl cloog-ppl keyutils-libs-devel libcom_err-devel
 libsepol-devel libselinux-devel krb5-devel zlib-devel libXpm* freetype libjpeg* libpng* php-common php-gd ncurses* libtool* libxml2 libxml2-devel patch
4.下载Cmake,Mysql,PHP,Nginx,libevent,libmcrypt,pcre,memcached 
如果自己寻找这些很困难，推荐给大家几个很好的源地址： 
阿里：[http://mirrors.aliyun.com/](http://mirrors.aliyun.com/)
搜狐：[http://mirrors.sohu.com/](http://mirrors.sohu.com/)
网易：[http://mirrors.163.com/](http://mirrors.163.com/)
我的就是在搜狐下载的，版本如下： 
![这里写图片描述](https://img-blog.csdn.net/20150214020802451)
5.系统约定 
软件源代码包存放位置：/usr/local/src 
源码包编译安装位置：/usr/local/软件名字
源码放在/usr/local/src的实现方案有多种，可以在Windows下载，然后通过Xftp传送至指定目录，也可以cd到指定目录，wget  下载地址; 进行下载。
6.正式编译安装
安装Mysql:
Mysql需要Cmake进行编译安装，所以在安装Mysql前必须先安装Cmake
![这里写图片描述](https://img-blog.csdn.net/20150214020832981)
解压Cmake： 
[root@jhq0113 src]# tar xzvf cmake-3.1.1.tar.gz
进入cmake解压后目录： 
[root@jhq0113 src]# cd cmake-3.1.1
预编译，这个过程可能需要一段时间： 
[root@jhq0113 cmake-3.1.1]# ./configure 
编译并安装： 
[root@jhq0113 cmake-3.1.1]# make && make install
添加mysql组 
[root@jhq0113 cmake-3.1.1]# groupadd mysql 
创建用户mysql并加入到mysql组，不允许mysql用户直接登录系统 
[root@jhq0113 cmake-3.1.1]# useradd -g mysql mysql -s /bin/false 
创建MySQL数据库存放目录 
[root@jhq0113 cmake-3.1.1]# mkdir -p /data/mysql 
设置MySQL数据库存放目录权限 
[root@jhq0113 cmake-3.1.1]# chown -R mysql:mysql /data/mysql 
返回/usr/local/src目录： 
[root@jhq0113 cmake-3.1.1]# cd /usr/local/src
解压mysql: 
[root@jhq0113 src]# tar xzvf mysql-5.6.13.tar.gz 
进入mysql解压目录： 
[root@jhq0113 src]# cd mysql-5.6.13
用Cmake预编译Mysql,此过程可能需要一段时间： 
[root@jhq0113 mysql-5.6.13]# cmake . -DCMAKE_INSTALL_PREFIX=/usr/local/mysql -DMYSQL_DATADIR=/data/mysql -DSYSCONFDIR=/etc/
编译： 
[root@jhq0113 mysql-5.6.13]# make
安装： 
[root@jhq0113 mysql-5.6.13]# make install
拷贝配置文件（如果/etc目录下面默认有一个my.cnf，直接覆盖即可）： 
[root@jhq0113 support-files]# cp /usr/local/mysql/support-files/my-default.cnf /etc/my.cnf
添加Mysql路径： 
[root@jhq0113 support-files]# vim /etc/my.cnf 
修改后： 
![这里写图片描述](https://img-blog.csdn.net/20150214020904446)
生成Mysql系统数据库： 
[root@jhq0113 support-files]# /usr/local/mysql/scripts/mysql_install_db –user=mysql –basedir=/usr/local/mysql –datadir=/data/mysql
把Mysql加入系统启动： 
[root@jhq0113 support-files]# cp /usr/local/mysql/support-files/mysql.server /etc/rc.d/init.d/mysqld
增加mysqld执行权限： 
[root@jhq0113 support-files]# chmod 755 /etc/init.d/mysqld 
mysqld加入开机启动： 
[root@jhq0113 support-files]# chkconfig mysqld on
编辑配置文件： 
[root@jhq0113 support-files]# vim /etc/init.d/mysqld 
修改后：
![这里写图片描述](https://img-blog.csdn.net/20150214020929375)
把Mysql服务加入系统环境变量： 
[root@jhq0113 support-files]# vim /etc/profile
修改后： 
![这里写图片描述](https://img-blog.csdn.net/20150214020954257)
将myslq的库文件链接到系统默认的位置，以确保在编译类似PHP等软件时可以不用指定mysql的库文件地址。 
[root@jhq0113 support-files]# ln -s /usr/local/mysql/lib/mysql /usr/lib/mysql 
[root@jhq0113 support-files]# ln -s /usr/local/mysql/include/mysql /usr/include/mysql
重启系统： 
reboot
建立连接： 
[root@jhq0113 ~]# ln -s /var/lib/mysql/mysql.sock /tmp/mysql.sock
重启Mysqld: 
[root@jhq0113 ~]# service mysqld restart
设置Mysql密码： 
[root@jhq0113 support-files]# /usr/local/mysql/bin/mysql_secure_installation 
修改root密码： 
![这里写图片描述](https://img-blog.csdn.net/20150214021028933)
默认没有密码，直接回车，然后输入两次密码，一路按Y 回车下去
设置Mysql可以远程登录： 
[root@jhq0113 ~]# mysql -uroot -p
输入密码登录，然后执行下面的语句，限制Mysql root用户可以在指定Ip地址下远程登录 
mysql> grant all privileges on *.* to ‘root’@’%’ identified by ‘你的密码’;
更新权限： 
mysql> flush privileges;
退出： 
mysql> exit;
这样你的Mysql就可以在远程用Navicat进行远程登录了。
![这里写图片描述](https://img-blog.csdn.net/20150214021048358)
至此，Mysql安装完毕。
安装Nginx: 
安装Nginx之前需要安装pcre，进入/usr/local/src目录，解压pcre，进入pcre解压目录，执行以下命令进行编译安装： 
[root@jhq0113 pcre-8.35]# ./configure –prefix=/usr/local/pcre && make && make install
创建web组合web用户，不允许web用户直接登录系统： 
[root@jhq0113 pcre-8.35]# groupadd web 
[root@jhq0113 pcre-8.35]# useradd -g web web -s /bin/false 
编译安装nginx,解压，进入安装目录，执行以下命令： 
[root@jhq0113 nginx-1.7.0]# ./configure –prefix=/usr/local/nginx –without-http_memcached_module –user=web –group=web –with-http_stub_status_module –with-openssl=/usr/ –with-pcre=/usr/local/src/pcre-8.35
注意:–with-pcre=/usr/local/src/pcre-8.31指向的是源码包解压的路径，而不是安装的路径，否则会报错
编译安装： 
[root@jhq0113 nginx-1.7.0]# make && make install
编辑Nginx启动命令： 
[root@jhq0113 nginx-1.7.0]# vim /etc/init.d/nginx
加入下面内容：
```bash
#!/bin/bash
# nginx Startup script for the Nginx HTTP Server
# it is v.0.0.2 version.
# chkconfig: - 85 15
# description: Nginx is a high-performance web and proxy server.
# It has a lot of features, but it's not for everyone.
# processname: nginx
# pidfile: /var/run/nginx.pid
# config: /usr/local/nginx/conf/nginx.conf
nginxd=/usr/local/nginx/sbin/nginx
nginx_config=/usr/local/nginx/conf/nginx.conf
nginx_pid=/usr/local/nginx/logs/nginx.pid
RETVAL=0
prog="nginx"
# Source function library.
. /etc/rc.d/init.d/functions
# Source networking configuration.
. /etc/sysconfig/network
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
```
赋予nginx执行权限： 
[root@jhq0113 nginx-1.7.0]# chmod 775 /etc/init.d/nginx 
设置nginx开机启动： 
[root@jhq0113 nginx-1.7.0]# chkconfig nginx on
启动nginx: 
[root@jhq0113 nginx-1.7.0]#  service nginx start
测试Nginx安装和启动状态： 
![这里写图片描述](https://img-blog.csdn.net/20150214021140743)
至此，Nginx安装完毕，待安装好PHP后再做对PHP的支持和指定自定义发布目录设置。
安装PHP 
安装PHP之前需要安装libmcrypt: 
将libmcrypt解压,进入解压目录,执行以下命令: 
[root@jhq0113 libmcrypt-2.5.8]# ./configure && make && make install
安装wget组件： 
[root@jhq0113 src]# yum  install wget
进入/usr/local/src/目录下载mhash： 
[root@jhq0113 src]# wget [http://soft.7dot.com/soft/mhash-0.9.9.9.tar.gz](http://soft.7dot.com/soft/mhash-0.9.9.9.tar.gz)
下载完毕，解压安装： 
[root@jhq0113 mhash-0.9.9.9]# ./configure && make && make install
进入/usr/local/src/目录，下载mcrypt: 
[root@jhq0113 src]# wget [http://110.96.192.10:81/1Q2W3E4R5T6Y7U8I9O0P1Z2X3C4V5B/soft.7dot.com/soft/mcrypt-2.6.8.tar.gz](http://110.96.192.10:81/1Q2W3E4R5T6Y7U8I9O0P1Z2X3C4V5B/soft.7dot.com/soft/mcrypt-2.6.8.tar.gz)
解压后，进入解压目录，执行以下命令： 
[root@jhq0113 mcrypt-2.6.8]# export LD_LIBRARY_PATH=/usr/local/lib
编译安装： 
[root@jhq0113 mcrypt-2.6.8]# ./configure && make && make install
等待安装完成，解压php: 
[root@jhq0113 src]# tar jvxf php-5.6.5.tar.bz2 
安装freetype： 
[root@jhq0113 php-5.6.5]# yum install freetype-devel
Php5.5以上需要安装开发工具： 
[root@jhq0113 php-5.6.5]# yum groupinstall “Development Tools”
建立php安装目录： 
[root@jhq0113 libmcrypt-2.5.8]# mkdir -p /usr/local/php5
进入php解压目录，预编译： 
[root@jhq0113 php-5.6.5]# ./configure –prefix=/usr/local/php5 –with-config-file-path=/usr/local/php5/etc –with-mysql=/usr/local/mysql –with-mysqli=/usr/local/mysql/bin/mysql_config –with-mysql-sock=/tmp/mysql.sock –with-gd –with-iconv –with-zlib –enable-xml
 –enable-bcmath –enable-shmop –enable-sysvsem –enable-inline-optimization  –enable-mbregex –enable-fpm –enable-mbstring –enable-ftp –enable-gd-native-ttf –with-openssl –enable-pcntl –enable-sockets –with-xmlrpc –enable-zip –enable-soap –without-pear –with-gettext
 –enable-session –with-mcrypt –with-curl –with-pdo-mysql=/usr/local/mysql –with-fpm-user=web –with-fpm-group=web –with-png-dir –with-freetype-dir –with-jpeg-dir
如果预编译总是不通过，可以参考下面这篇博客来解决问题再次预编译，知道预编译通过。 
[http://lyp.cn/350_how-to-fix-php-compile-errors](http://lyp.cn/350_how-to-fix-php-compile-errors)
编译,此过程配置不好的电脑时间会很长： 
make
安装： 
make install
复制PHP配置文件到安装目录： 
[root@jhq0113 php-5.6.5]# cp php.ini-production /usr/local/php5/etc/php.ini
删除系统自带的配置文件： 
[root@jhq0113 php-5.6.5]# rm -rf /etc/php.ini 
添加软连接： 
[root@jhq0113 php-5.6.5]# ln -s /usr/local/php5/etc/php.ini /etc/php.ini
拷贝模板文件为php-fpm配置文件： 
[root@jhq0113 php-5.6.5]# cp /usr/local/php5/etc/php-fpm.conf.default /usr/local/php5/etc/php-fpm.conf
编辑php-fpm配置文件： 
[root@jhq0113 php-5.6.5]# vim /usr/local/php5/etc/php-fpm.conf
去掉pid = run/php-fpm.pid前面的；注释 
找到user和group，改成： 
user=web 
group=web
保存退出。拷贝php-fpm到系统启动目录： 
[root@jhq0113 php-5.6.5]# cp /usr/local/src/php-5.6.5/sapi/fpm/init.d.php-fpm /etc/rc.d/init.d/php-fpm
添加php-fpm执行权限： 
[root@jhq0113 php-5.6.5]# chmod +x /etc/rc.d/init.d/php-fpm 
设置php-fpm开机启动： 
[root@jhq0113 php-5.6.5]# chkconfig php-fpm on
编辑php.ini： 
[root@jhq0113 php-5.6.5]# vim /usr/local/php5/etc/php.ini 
设置时区 
修改为：date.timezone = PRC 
禁止显示php版本的信息 
修改为：expose_php = OFF 
支持php短标签 
修改为：short_open_tag = ON 
创建web目录： 
[root@jhq0113 nginx-1.7.0]# mkdir -p /home/data/web
设置nginx支持php： 
[root@jhq0113 php-5.6.5]# vim /usr/local/nginx/conf/nginx.conf
修改为： 
server { 
        listen       80; 
        #域名 
server_name  myphp.com; 
        #网站发布目录 
root   /home/data/web; 
        #charset koi8-r;
```
#access_log  logs/host.access.log  main;
  #支持
    location / {
        index index.php index.html index.htm;
    }
    #error_page  404              /404.html;
    # redirect server error pages to the static page /50x.html
    #
    error_page   500 502 503 504  /50x.html;
    location = /50x.html {
       # root   html;
    }
    #error_page  404              /404.html;
    # redirect server error pages to the static page /50x.html
    #
    error_page   500 502 503 504  /50x.html;
    location = /50x.html {
       # root   html;
    }
    # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
    #
    location ~ \.php$ {
        #root           html;
        fastcgi_pass   127.0.0.1:9000;
        fastcgi_index  index.php;
        fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
        include        fastcgi_params;
    }
    # deny access to .htaccess files, if Apache's document root
    # concurs with nginx's one
    #
    #location ~ /\.ht {
    #    deny  all;
    #}
}
```
设置网站发布目录所有者： 
[root@jhq0113 php-5.6.5]# chown web.web /home/data/web/ -R
设置目录权限： 
[root@jhq0113 php-5.6.5]# chmod 700 /home/data/web/ -R
编辑php探针： 
[root@jhq0113 php-5.6.5]# vim /home/data/web/index.php
内容为php代码，phpinfo信息(CSND博客屏蔽掉了php代码)。
重启系统： 
[root@jhq0113 php-5.6.5]# shutdown -r now
在局域网内访问需要在浏览器端PC配置hosts，即C:\Windows\System32\drivers\etc\hosts文件，添加以下信息 
192.168.1.16 myphp.com
然后在浏览器端访问： 
![这里写图片描述](https://img-blog.csdn.net/20150214210024533)
至此完整的LNMP已经正式完全编译安装完毕。
安装Memcached： 
Memcached是做分布式缓存最简单的工具，学习成本也非常低，他可以将数据缓存到内存中，我们可以把一些临时数据放入到Memcached中，例如分布式的session等等，但是不要将持久化的数据放在memcached中，因为Memcached将数据放在内存中，当服务器宕机或者重启时，Memcached中的数据会被清空的，如果你想做持久化的缓存，可以选用Redis等，Redis的数据是持久化的，Redis会按照一定的规律将内存中的数据同步到磁盘中，服务器重启或者宕机，数据是可以恢复的。
安装Memcached之前先要安装libevent： 
解压： 
[root@jhq0113 src]# tar xzvf libevent-2.0.19-stable.tar.gz
编译安装： 
[root@jhq0113 libevent-2.0.19-stable]# ./configure –prefix=/usr/local/libevent/ && make && make install
添加软连接： 
[root@jhq0113 libevent-2.0.19-stable]# ln -s /usr/local/libevent/lib/libevent-2.0.so.5 /lib/libevent-2.0.so.5
解压，预编译memcached: 
[root@jhq0113 memcached-1.4.22]# ./configure –prefix=/usr/local/memcached/ –with-libevent=/usr/local/libevent/
编译并安装： 
[root@jhq0113 memcached-1.4.22]# make && make install
编辑Memcached启动脚本： 
[root@jhq0113 memcached-1.4.22]# vim /etc/rc.d/init.d/memcached
```bash
#!/bin/sh
#
#memcached:    MemCached Daemon
#
# chkconfig:    - 90 25
# description:  MemCached Daemon
#
# Source function library.
. /etc/rc.d/init.d/functions
. /etc/sysconfig/network
#[ ${NETWORKING} = "no" ] && exit 0
#[ -r /etc/sysconfig/dund ] || exit 0
#. /etc/sysconfig/dund
#[ -z "$DUNDARGS" ] && exit 0
start()
{
        echo -n $"Starting memcached: "
        daemon $MEMCACHED -u daemon -d -m 256  -l 192.168.1.16 -p 11211 -c 128 -P /tmp/memcached.pid
        echo
}
stop()
{
        echo -n $"Shutting down memcached: "
        killproc memcached
        echo
}
MEMCACHED="/usr/local/memcached/bin/memcached"
[ -f $MEMCACHED ] || exit 1
# See how we were called.
case "$1" in
        start)
                start
                ;;
        stop)
                stop
                ;;
        restart)
                stop
                sleep 3
                start
                ;;
        *)
                echo $"Usage: $0 {start|stop|restart}"
                exit 1
esac
exit 0
```
启动参数解释： 
启动参数介绍如下： 
-d选项是启动一个守护进程， 
-m是分配给Memcache使用的内存数量，单位是MB，这里是10MB， 
-u是运行Memcache的用户，这里是root， 
-l是监听的服务器IP地址，如果有多个地址的话，这里指定了服务器的IP地址192.168.0.200， 
-p是设置Memcache监听的端口，这里设置了12000，最好是1024以上的端口， 
-c选项是最大运行的并发连接数，默认是1024，这里设置了256，按照服务器的负载量来设定， 
-P是设置保存Memcache的pid文件，我这里是保存在 /tmp/memcached.pid，也可以启动多个守护进程，不过端口不能重复。
给予memcached执行权限： 
[root@jhq0113 memcached-1.4.22]# chmod +x /etc/init.d/memcached 
设置memcached开机启动： 
[root@jhq0113 memcached-1.4.22]# chkconfig –add memcached 
[root@jhq0113 memcached-1.4.22]# chkconfig memcached on
安装telnet服务： 
[root@jhq0113 bin]# yum install xinetd 
[root@jhq0113 bin]# yum install telnet-server
设置telnet开机启动： 
[root@jhq0113 bin]# chkconfig telnet  on
将telnet启用： 
[root@jhq0113 bin]# vim /etc/xinetd.d/telnet 
将disabled变为no
配置防火墙允许23端口： 
[root@jhq0113 bin]# vim /etc/sysconfig/iptables
重启防火墙，重启xinetd: 
[root@jhq0113 bin]# service xinetd restart
局域网内windows电脑安装telnet客户端： 
![这里写图片描述](https://img-blog.csdn.net/20150214210130565)
然后Windows+R，CMD回车。 
键入：telnet 192.168.1.16 11211 
Windows电脑的dos下开始会有问题，第一行看不见，直接回车从第二行开始操作memcached: 
![这里写图片描述](https://img-blog.csdn.net/20150214210213827)
至此，memcached也安装完毕，如果php想利用memcached做分布式缓存，需要服务器安装memcached扩展，在yii框架中集成了对memcached做分布式缓存的很好的支持，如果你想把session放在memcached中，就自己查阅资料吧，度娘上一大把，好了，就到这里了。
希望可以帮助到你。
