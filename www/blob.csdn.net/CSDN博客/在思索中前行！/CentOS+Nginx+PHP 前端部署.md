# CentOS+Nginx+PHP 前端部署 - 在思索中前行！ - CSDN博客





2014年06月17日 22:16:20[_Tham](https://me.csdn.net/txl16211)阅读数：776标签：[linux																[PHP环境](https://so.csdn.net/so/search/s.do?q=PHP环境&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)







都说Nginx比Apache性能优越，一直没有时间装测试，今天终于有时间装上试试性能了，其实Nginx的安装非常简单，具体流水步骤记录如下：


1.系统环境：

==================================

CENTOS 5.4或5.5 bit64

NGINX 1.1.5


PHP 5.3.8

############################################

#yum 安装系统环境所需要的软件包

yum -y install yum-fastestmirror ntp

yum -y install patch make flex bison tar

yum -y install libtool libtool-libs kernel-devel 

yum -y install libjpeg libjpeg-devel libpng libpng-devel

yum -y install libtiff libtiff-devel gettext gettext-devel

yum -y install libxml2 libxml2-devel zlib-devel  net-snmp

yum -y install file glib2 glib2-devel bzip2 diff* openldap-devel

yum -y install bzip2-devel ncurses ncurses-devel curl curl-devel e2fsprogs

yum -y install e2fsprogs-devel krb5 krb5-devel libidn libidn-devel

yum -y install openssl openssl-devel vim-minimal unzip

# 安装PHP支持GD库模块

yum -y install freetype freetype-devel png jpeg zlib gd php-gd*

# 安装PHP 5.* 组件

yum -y install libiconv libevent mhash mcrypt

# 安装MYDSQL所需要系统库相关库文件

yum install -y gcc gcc-c++ gcc-g77 autoconf automake  fiex* ncurses-devel libmcrypt* libtool-ltdl-devel*

# 安装NGINX 组件

yum -y install pcre*


2. 软件下载

======================

PHP： wget -c http://www.php.net/get/php-5.3.8.tar.bz2/from/ar.php.net/mirror

NGINX： wget -c http://nginx.org/download/nginx-1.1.5.tar.gz


3. 软件安装

======================

(1) PHP 安装

php和nginx的整合是通过php-FastCGI

FastCGI 是一个可伸缩、高速的在web server和脚本语言间通迅的接口。被许多脚本语言所支持，包括 php多数流行的web server都支持 FastCGI。

 正常情况下，nginx和php直接是完全不认识的，我们就是通过php-fastcgi将二者整合。
php5.3.0之前的版本，php-FastCGI 是需要单独安装的。但是在这之后，php-FastCGI 就整合在了php的源码包中，不必再去单独安装。在这里我用的就是php5.3.8的版本，内置了php-fpm ，编译时开启，并且编译后存在
 php-cgi 文件了。
注意：PHP编译支持php-fpm功能就不能编译支持apache的apxs模块功能，不然报错。

# tar jxf php-5.3.8.tar.bz2 && cd php-5.3.8

# ./configure --prefix=/u/php5 --with-config-file-path=/u/php5/lib --with-oci8=instantclient,/usr/local/instantclient --with-mysql=mysqlnd --with-libxml-dir --enable-ftp --enable-sockets --with-gd --with-jpeg-dir --with-png-dir --with-freetype-dir --with-zlib-dir
 --enable-gd-native-ttf --enable-magic-quotes --with-iconv --enable-mbstring=all --disable-cgi --with-openssl --with-mcrypt --enable-bcmath --enable-calendar --enable-exif --enable-libxml --with-bz2 --with-curl --with-xmlrpc --with-gettext --disable-cli  --disable-debug 
 --enable-zend-multibyte --with-mhash --enable-pcntl --enable-sysvsem --enable-inline-optimization --enable-soap --disable-ipv6 --enable-fpm

# make

# make install

# cp php.ini-development /u/php5/lib/php.ini

编译完成后，在PHP的编译安装目录中，会看到如下目录结构：
![](http://blog.csdn.net/fl49809045/article/details/25886101)

etc目录：php-fpm.conf.default文件

sbin目录：php-fpm执行文件

# cd  /u/php5/etc/  # 切换到安装目录下的配置文件目录

# cp  php-fpm.conf.default  php-fpm.conf

# vi php-fpm.conf 

启用如下几行，即去掉前面的分号(;)

    pid = run/php-fpm.pid

    error_log = log/php-fpm.log

    log_level = notice

    listen = 127.0.0.1:9000

    listen.allowed_clients = 127.0.0.1

    pm = dynamic

    pm.max_children = 50

    pm.start_servers = 5

    pm.min_spare_servers = 5

    pm.max_spare_servers = 35

    pm.max_requests = 500

    env[HOSTNAME] = $HOSTNAME

    env[PATH] = /usr/local/bin:/usr/bin:/bin

    env[TMP] = /tmp

    env[TMPDIR] = /tmp

    env[TEMP] = /tmp


wq保存退出


# /u/php5/sbin/php-fpm （启动PHP）

# netstat -antpl (如果看到9000端口，PHP-FPM配置成功)


(2) NGINX 安装

==================================

# tar zxf nginx-1.1.5.tar.gz && cd nginx-1.1.5

# ./configure --prefix=/u/nginx --with-http_stub_status_module --with-http_ssl_module --with-http_gzip_static_module --with-pcre --http-client-body-temp-path=/tmp/nginx_client --http-proxy-temp-path=/tmp/nginx/proxy --http-fastcgi-temp-path=/tmp/nginx/fastcgi 
 --http-uwsgi-temp-path=/tmp/nginx/uwsgi --http-scgi-temp-path=/tmp/nginx/scgi

或者根据官方网站提供的配置编译

# ./configure \

  --prefix=/u/nginx \

  --sbin-path=/u/nginx/sbin \

  --conf-path=/u/nginx/nginx.conf \

  --pid-path=/tmp/nginx/nginx.pid  \

  --lock-path=/tmp/nginx/nginx.lock \

  --user=nginx \

  --group=nginx \

  --with-pcre \

  --http_rewrite_module \ #支持路径重新，需要pcre模块支持

  --http_gzip_module \    #启用gzip压缩，需要zlib包支持

  --with-http_ssl_module \

  --with-http_flv_module \

  --with-http_stub_status_module \

  --with-http_gzip_static_module \

  --error-log-path=/u/nginx/logs/error.log \

  --http-log-path=/u/nginx/logs/access.log \

  --http-client-body-temp-path=/tmp/nginx/client/ \ #HTTP客户端请求的主体的临时文件的路径

  --http-proxy-temp-path=/tmp/nginx/proxy/ \        #HTTP代理临时文件的路径

  --http-fastcgi-temp-path=/tmp/nginx/fcgi/ \       #HTTP FastCGI的临时文件的路径

  --http-uwsgi-temp-path=/tmp/nginx/uwsgi/ \

  --http-scgi-temp-path=/tmp/nginx/scgi/




#make

# make install

+++++++++++++++++++++++++

NGINX 配置文件设置：这个配置只是让NGX和PHP关联起来，更多配置只好GG了。

+++++++++++++++++++++++++

user  nginx nginx ; （nginx 用户需要新建，注释掉也OK的）

worker_processes  8;


error_log  logs/error.log;

#error_log  logs/error.log  notice;

#error_log  logs/error.log  info;


pid        logs/nginx.pid;


events {

    worker_connections  2048;

}


http {

    include       mime.types;

    default_type  application/octet-stream;

    sendfile        on;

    keepalive_timeout  65;

    server {

        listen       80;

        server_name  127.0.0.1


        # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000

    location ~ \.php$ {

            root           html;

            fastcgi_pass   127.0.0.1:9000;

            fastcgi_index  index.php;

            fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;

         #  include        fastcgi_params;

            include        fastcgi.conf;

        }


}

在/u/nginx/html 中编写Index.php文件

<?php

phpinfo();

?>

打开浏览器 http://IP/index.php,能浏览恭喜你，安装成功了。




